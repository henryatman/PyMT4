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

#include "pymt4_client.h"
#include <map>
#include <boost/thread.hpp>
#include "pymt4_client_iosession.h"
#include "pymt4_common_message.h"
#include <boost/noncopyable.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>


namespace PyMT4
{
	using namespace std;
	using namespace boost::asio;
	using namespace boost::asio::ip;

	typedef pair<string,uint32_t> IOSessionAddress;
	typedef map<IOSessionAddress,IOSession*> IOSessionMap;

	typedef boost::shared_ptr<mutex> SMutex;
	typedef boost::shared_ptr<condition_variable> SCondition;

	typedef boost::function<void(const std::string& ,const double&,const double&)> OnTickHandlerFunc;
	typedef boost::uuids::uuid OnTickHandlerId;
	
	typedef std::map<OnTickHandlerId,std::pair<bool,OnTickHandlerFunc> > OnTickHandlerList;

	typedef boost::function<void(const bool&)> ConnectCallback;

	DECLARE(IOConnector);

	class IOSession;

	class IOConnector : public boost::noncopyable, public boost::enable_shared_from_this<IOConnector>
	{
		friend class IOSession;



	public:

		bool connect(const string& address="127.0.0.1",const uint32_t& port=DEFAULT_PORT);
		void connect(ConnectCallback,const string& address="127.0.0.1",const uint32_t& port=DEFAULT_PORT);

		bool disconnect();
		static IOConnectorPtr Instance();

		IOSessionMap& sessionMap();
		PendingResultPtr dispatchMessage(const MessageHeaderPtr message);

		OnTickHandlerId registerOnTickHandler(const OnTickHandlerFunc& func);
		void		    onTickDispatcher(const OnTickHandlerFunc& handlerFunc,IOSessionPtr session,const MessageUID& messageuid,const OnTickHandlerId& id,const std::string& symbol, const double& bid, const double& ask);
		void		    notifyOnTick(IOSessionPtr session,const MessageUID& messageuid,const std::string& symbol, const double& bid, const double& ask);
			

		~IOConnector();
	private:

		boost::thread_group	    _poolThreadGroup;
		boost::asio::io_service _poolIOService;
		boost::asio::io_service::work _poolWork;

		OnTickHandlerList _onTickHandlerList;

		static IOConnectorPtr m_instance;

		IOConnector();

		void resolveHandler(ConnectCallback, const boost::system::error_code& error,tcp::resolver::iterator iterator);
		void connectHandler(IOSessionPtr,ConnectCallback,const boost::system::error_code& error);


		boost::mutex		_connectorMutex;
		io_service			m_ioService;
		tcp::resolver		m_resolver;

		io_service::work	m_iosWork;
		boost::shared_ptr<thread>	m_iosThread;

		IOSessionMap		m_sessions;

	};

}