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


#include "pymt4_common_message.h"
#include "pymt4_common_serializer.h"
#include <boost/uuid/random_generator.hpp>

using namespace PyMT4;


MessageUID& MessageHeader::uid()
{
	return _messageuid;
}

MessageHeader::MessageHeader (const MessageTypeIdentifier& type) : Type(type), _messageuid(boost::uuids::random_generator()())
{
	uint32_t sizedummy = 0;
		

	uint32_t type32 = type;
	Serializer<const uint32_t>::serializeItem(&type32,&_messageBuffer);
	Serializer<const uint32_t>::serializeItem(&sizedummy,&_messageBuffer);
	Serializer<const MessageUID>::serializeItem(&_messageuid,&_messageBuffer);

}

Buffer& MessageHeader::messageBuffer()
{ 
	return _messageBuffer;
}

MessageCommand::MessageCommand(const CommandIdentifier& commandIdentifier ) : MessageHeader(MessageCommandType),_commandIdentifier(commandIdentifier)
{	
	_messageBuffer.reserve(128);
	uint32_t cid32 = commandIdentifier;
	Serializer<const uint32_t>::serializeItem(&cid32,&_messageBuffer);
}


MessageCommandPtr MessageCommand::Create(const CommandIdentifier& commandIdentifier)
{
	MessageCommandPtr messageCommandPtr = boost::shared_ptr<MessageCommand>(new MessageCommand(commandIdentifier));
	return messageCommandPtr;
}




MessageEvent::MessageEvent(const EventIdentifier& eventIdentifier) : MessageHeader(MessageEventType),_eventIdentifier(eventIdentifier)
{	
	_messageBuffer.reserve(128);

	uint32_t eventId32 = eventIdentifier;
	Serializer<const uint32_t>::serializeItem(&eventId32,&_messageBuffer);
}



MessageEventPtr MessageEvent::Create(const EventIdentifier& eventIdentifier)
{
	MessageEventPtr messageEventPtr = boost::shared_ptr<MessageEvent>(new MessageEvent(eventIdentifier));
	return messageEventPtr;
}




void PendingResult::setError(const int32_t& error)
{
	_lastError = error;
}

int32_t& PendingResult::error()
{
	return _lastError;
}

void PendingResult::setResult(BufferCPos databegin,BufferCPos dataend)
{   

	boost::mutex::scoped_lock resultLock(_resultMutex);	
	size_t distance = std::distance(databegin,dataend);
	_resultBuffer.reserve(std::distance(databegin,dataend));
	std::copy(databegin,dataend,std::back_inserter(_resultBuffer));
	_resultSet = true;
	_resultCondition.notify_all();
}

MessageResult::MessageResult(const MessageTypeIdentifier& replyTo,const MessageUID& replyId) : MessageHeader(MessageResultType)
{

	uint32_t replyTo32 = replyTo;
	Serializer<const uint32_t>::serializeItem(&replyTo32,&_messageBuffer);
	Serializer<const MessageUID>::serializeItem(&replyId,&_messageBuffer);
}

MessageResultPtr MessageResult::Create(const MessageTypeIdentifier& replyTo,const MessageUID& replyId)
{
	MessageResultPtr messageResultPtr = boost::shared_ptr<MessageResult>(new MessageResult(replyTo,replyId));
	return messageResultPtr;

}


PendingResult::PendingResult() : _lastError(0), _resultSet(false)
{}

PendingResultPtr PendingResult::Create()
{
	PendingResultPtr pendingResult = boost::shared_ptr<PendingResult>(new PendingResult());
	return pendingResult;
}