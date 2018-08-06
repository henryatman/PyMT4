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


#include "pymt4_server_iosession.h"
#include "pymt4_common_serializer.h"
#include "pymt4_server_ioserver.h"


#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>

using namespace boost;
namespace PyMT4
{


PendingCommand::PendingCommand()
{}

PendingCommandPtr PendingCommand::Create()
{
	return PendingCommandPtr(new PendingCommand);
}

IOSession::IOSession(io_service& ioService) : IOSessionCommon(ioService)
{
	registerHandler(MessageCommandType,boost::bind(&IOSession::MessageCommandHandler,this,_1,_2,_3,_4,_5));
	registerHandler(MessageResultType,boost::bind(&IOSession::messageResultHandler,this,_1,_2,_3,_4,_5));
}
bool IOSession::messageResultHandler(const MessageTypeIdentifier& messageId, const MessageUID& messageuid, BufferCPos& begin, BufferCPos& end ,const boost::system::error_code& error)
{

	MessageTypeIdentifier replyTo;
	uint32_t msguid_size=0;
	Serializer<MessageTypeIdentifier>::deserializeItem(&replyTo,&begin);
	Serializer<uint32_t>::deserializeItem(&msguid_size,&begin);

	MessageUID replyTomessageuid;


	for(uint8_t index=0;index<MessageUID::static_size();++index)
		Serializer<uint8_t>::deserializeItem(&replyTomessageuid.data[index],&begin);	

	const std::string tmp = boost::lexical_cast<std::string>(replyTomessageuid);
	
	IOServer::Instance()->notifyResult(replyTo,replyTomessageuid);
	
	return true;
}
bool IOSession::MessageCommandHandler(const MessageTypeIdentifier& messageId,const MessageUID& messageuid, BufferCPos& dataPos, BufferCPos& dataEnd ,const boost::system::error_code& error)
{

	PendingCommandPtr pendingCommand = PendingCommand::Create();
	pendingCommand->commandId		 = CommandUnknown;


	Serializer<CommandIdentifier>::deserializeItem(&pendingCommand->commandId,&dataPos);

	if (pendingCommand->commandId == CommandUnknown) 
		return false;
	
	pendingCommand->data.assign(dataPos,dataEnd);
	pendingCommand->dataPos = pendingCommand->data.begin();
	pendingCommand->session = boost::static_pointer_cast<IOSession>(shared_from_this());
	_sessionMutex.unlock();
	IOServer::Instance()->queueCommand(pendingCommand);
	_sessionMutex.lock();
	return true;
}


void IOSession::initialize()
{
		m_socket.set_option(boost::asio::socket_base::keep_alive(true));
		m_socket.set_option(boost::asio::ip::tcp::no_delay(true));
		readHeader();
}

IOSession::~IOSession()
{

	if (IOServer::m_instance)
	{
		boost::mutex::scoped_lock sessionListLock(IOServer::m_instance->_servermutex);
		BOOST_FOREACH(IOSessionMap::value_type& mapItem, IOServer::m_instance->_sessionList)
		{
			if (mapItem.first == id()) {
				IOServer::Instance()->_sessionList.erase(mapItem.first);
				break;
			}
		}
	}
}

}

