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


#include "pymt4_common_iosession.h"
#include "pymt4_common_serializer.h"
#include <boost/bind/placeholders.hpp>

namespace PyMT4 
{


bool IOSessionCommon::disconnect()
{
	 socket().close();
	 return true;
}

IOSessionCommon::~IOSessionCommon()
{

}

IOSessionCommon::IOSessionCommon(io_service& ioService) :
	m_ioService(ioService),
	m_socket(ioService),
	m_id(uuids::random_generator()())
{
	m_messageHandlerMap.resize(MessageMax);
	m_readBuffer.reserve(IO_BUFFER_SIZE);
	m_writeBuffer.reserve(IO_BUFFER_SIZE);
}

void IOSessionCommon::registerHandler(const MessageTypeIdentifier& message,const MessageHandlerFunc& function)
{
	if (message > MESSAGE_MAX)
		throw Exception("Message out of range. Value greater than MessageType::MESSAGE_MAX");

	if (m_messageHandlerMap[message] != 0)
		throw Exception(std::string("Message handler already defined for ") + boost::lexical_cast<std::string,int32_t>(message));

	m_messageHandlerMap[message] = function;
}


void IOSessionCommon::readHeader()
{
	boost::mutex::scoped_lock scopedLock(_sessionMutex);
	m_readBuffer.clear();
	m_readBuffer.resize(IO_HEADER_SIZE);
	async_read(m_socket,buffer(m_readBuffer),bind(&IOSessionCommon::headerHandler,shared_from_this(),asio::placeholders::error));

}

void IOSessionCommon::writeMessage(MessageHeaderPtr message)
{
	boost::mutex::scoped_lock scopedLock(_sessionMutex);

	Buffer& messageBuffer = message->messageBuffer();
	size_t payloadSize = messageBuffer.size() - IO_HEADER_SIZE;
	(*reinterpret_cast<size_t*>(messageBuffer.data()+sizeof(MessageTypeIdentifier))) = payloadSize;

	async_write(m_socket,buffer(message->messageBuffer()),bind(&IOSessionCommon::writeHandler,shared_from_this(),message,asio::placeholders::error));
}

void IOSessionCommon::writeHandler(MessageHeaderPtr message,const boost::system::error_code& error)
{
	if (error)
	{

	}
}

void IOSessionCommon::headerHandler(const boost::system::error_code& error)
{

	boost::mutex::scoped_lock scopedLock(_sessionMutex);
	if (!error)
	{

		Buffer::iterator readBufferIter = m_readBuffer.begin();
		MessageTypeIdentifier			messageTypeIdentifier;
		uint32_t							messageContentSize;


		uint32_t msgtype32 = 0;
		Serializer<uint32_t>::deserializeItem(&msgtype32,&readBufferIter);
		Serializer<uint32_t>::deserializeItem(&messageContentSize,&readBufferIter);
		messageTypeIdentifier = (MessageTypeIdentifier)msgtype32;

		MessageUID messageuid;
		uint32_t  msguuid_size=0;
		Serializer<uint32_t>::deserializeItem(&msguuid_size,&readBufferIter);


		for(uint8_t index=0;index<MessageUID::static_size();++index)
			Serializer<uint8_t>::deserializeItem(&messageuid.data[index],&readBufferIter);	

		m_readBuffer.resize(m_readBuffer.size() + messageContentSize);

		async_read(m_socket,
			buffer(&m_readBuffer[IO_HEADER_SIZE],
				   messageContentSize),
			transfer_at_least(messageContentSize),
			bind(&IOSessionCommon::messageHandler,shared_from_this(),messageuid,messageTypeIdentifier,asio::placeholders::error));
	}

}

void IOSessionCommon::messageHandler(const MessageUID& messageuid,const MessageTypeIdentifier& messageTypeIdentifier,const boost::system::error_code& error)
{
	if(!error)
	{
		{
		boost::mutex::scoped_lock scopedLock(_sessionMutex);
		if (!m_messageHandlerMap[messageTypeIdentifier]) {
			return; // No Handler Defined.
		}
		Buffer::iterator deserializeIter = m_readBuffer.begin()+IO_HEADER_SIZE;


		m_messageHandlerMap[messageTypeIdentifier](messageTypeIdentifier,messageuid,deserializeIter,m_readBuffer.end(),error);
		}

		readHeader();
	}
	
}

}