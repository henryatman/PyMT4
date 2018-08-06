/* (C) Copyright 2013 Rob Watson rmawatson [at] hotmail.com  and others.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Lesser General Public License
 * (LGPL) version 2.1 which accompanies this distribution, and is available at
 * http://www.gnu.org/licenses/lgpl-2.1.html
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * Contributors:
 *     Rob Watson ( rmawatson [at] hotmail )
 *
 */


#pragma once

#include "pymt4_server.h"
#include "pymt4_server_iosession.h"

#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <boost/asio.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/random_generator.hpp>
#include <Windows.h>
#include <boost/thread.hpp>
#include <map>
#include <deque>


extern char* lmo;
namespace PyMT4
{
	using namespace std;
	using namespace boost;
	using namespace boost::asio;
	using namespace boost::asio::ip;

	using boost::shared_ptr;
	using boost::bind;

	DECLARE(IOServer);
	DECLARE(IOSession);
	
	typedef std::map<IOSessionID,IOSessionWPtr>		  IOSessionMap;
	typedef std::map<std::string,HWND>				  ChartWindowList;
	typedef std::deque<PendingCommandPtr>	PendingCommandList;
	typedef std::map<MessageUID,PendingResultPtr> OnTickResultList;

	class IOServer : public boost::noncopyable
	{
		friend class IOSession;

		IOServer();

		/*General*/
		PendingCommandList			   _pendingCommandList;

		ChartWindowList		_registeredWindowList;
		
		/*Asio*/
		io_service			m_ioService;
		tcp::acceptor		m_acceptor;
		io_service::work	m_iosWork;
		shared_ptr<thread>	m_iosThread;

		OnTickResultList _onTickResults;
			
		IOSessionMap		_sessionList;

		boost::mutex		_servermutex;
		boost::condition_variable
							_processCondition;
		
		bool				_shutdownRequest;
		
		PendingCommandPtr   _currentCommand;
		/*Static*/
		static IOServerPtr m_instance;
		static uint16_t	   default_port;

		void acceptSession();
		void acceptHandler(IOSessionPtr,const system::error_code&);

	public:
		~IOServer();
		static int32_t WindowUpdateMsg;
		static IOServerPtr		Instance(); 
		static void SetDefaultPort(const uint16_t default_port);
		void registerChartWindow(const char* chartName,HWND chartHandle);
		void chartWindowNotify(const char* chartName,HWND chartHandle);
		void requestChartsUpdate();
		int32_t  requestPendingCommand();




		//bool dispatchOnTick(const std::string& symbol, const double& bid, const double& ask);
		bool dispatchEvent(const MessageEventPtr message);


		void notifyResult(const MessageTypeIdentifier&,const MessageUID& replyToUid);





		template <typename T> T getPodArgument()
		{
			T result;
			Serializer<T>::deserializeItem(&result,&_currentCommand->dataPos);
			return result;
		}

		const wchar_t* getStringArgument(wchar_t* stringbuffer);

		void shutdown();
		void queueCommand(PendingCommandPtr pendingCommand);
		int32_t pendingCommand();

		template <typename T> bool completeCommand(const T& result,const int32_t& error)
		{
			MessageResultPtr resultMessage; IOSessionPtr session;

			{
			boost::mutex::scoped_lock scopedLock(_servermutex);
			lmo = "completeCommand";
			resultMessage = MessageResult::Create(MessageCommandType,_currentCommand->messageUID);

			Serializer<int32_t>::serializeItem(&(const_cast<int32_t&>(error)),&resultMessage->messageBuffer());

			Serializer<T>::serializeItem(&(const_cast<T&>(result)),&resultMessage->messageBuffer());

				session = _currentCommand->session.lock();


			_currentCommand.reset();
			_processCondition.notify_all();

			}
			if (session)
				session->writeMessage(resultMessage);
			lmo = "";
						return true;

		}


	};

}