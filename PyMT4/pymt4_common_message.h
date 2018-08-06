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

#include "pymt4_common.h"
#include <vector>

//#ifdef PYMT4_CLIENT
#include <boost/thread/condition_variable.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/atomic.hpp>
#include <boost/uuid/uuid.hpp>
//#endif 
#include "pymt4_common_serializer.h"

#define IO_HEADER_SIZE ( sizeof(uint32_t) + sizeof(uint32_t) + (sizeof(uint8_t)*16 + sizeof(uint32_t)))
#define IO_BUFFER_SIZE 512


namespace PyMT4
{

	typedef boost::uuids::uuid MessageUID;

	#define MESSAGE_MAX 2
	#define RESULT_TIMEOUT 3000

	DECLARE(MessageHeader);
	DECLARE(MessageResult);
	DECLARE(MessageCommand);
	DECLARE(MessageEvent);
	DECLARE(PendingResult);

	class PendingResult
	{

		boost::condition_variable _resultCondition;
		boost::mutex			  _resultMutex;
		Buffer					  _resultBuffer;
		int32_t					  _lastError;
		bool					  _resultSet;

		PendingResult();

	public:
		
		static PendingResultPtr Create();

		template <typename T> T convertResult()
		{
			if (!_resultBuffer.size())
				return T();

			T result;
			Serializer<T>::deserializeItem(&result,&_resultBuffer.begin());
			return result;
		}

		template <typename T> T waitForResult()
		{

			boost::mutex::scoped_lock resultLock(_resultMutex);

			if(_lastError)
				return T();

			if (!_resultSet)
			{
				boost::system_time const timeout = boost::get_system_time()+ boost::posix_time::milliseconds(RESULT_TIMEOUT);

				if (!_resultCondition.timed_wait(resultLock,timeout))
				{
						_lastError = ERR_PYMT4_TIMEOUT;
					return T();
				}
			}

			
			return convertResult<T>();
		}

		int32_t& error();
		void setError(const int32_t& error);
		void setResult(BufferCPos begin,BufferCPos end);

	};

	class MessageHeader
	{


		MessageUID	_messageuid;

	public:
		Buffer& messageBuffer();
		const MessageTypeIdentifier Type;
		
		Buffer _messageBuffer;

		MessageHeader (const MessageTypeIdentifier& type);
		void _serializeHeader();
		
		MessageUID& uid();

	};


	class MessageResult : public MessageHeader
	{

	private:
		MessageResult(const MessageTypeIdentifier& replyTo,const MessageUID& replyId);

	public:
		static MessageResultPtr Create(const MessageTypeIdentifier& replyTo,const MessageUID& replyId);

	};

	class MessageEvent : public MessageHeader
	{
		EventIdentifier _eventIdentifier;
		MessageEvent(const EventIdentifier& eventIdentifier);

	public:
		static MessageEventPtr Create(const EventIdentifier& eventIdentifier);

	};

	class MessageCommand : public MessageHeader
	{
	private:

		
		CommandIdentifier _commandIdentifier;
		MessageCommand(const CommandIdentifier& commandIdentifier);
	public:

		static MessageCommandPtr Create(const CommandIdentifier& commandIdentifier);

	};

}