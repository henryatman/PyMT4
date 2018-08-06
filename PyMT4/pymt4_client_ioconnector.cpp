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


#include "pymt4_client_ioconnector.h"

#include <boost/pointer_cast.hpp>
using namespace PyMT4;

IOConnectorPtr IOConnector::m_instance;

IOConnectorPtr IOConnector::Instance()
{
	if (!m_instance)
		m_instance.reset(new IOConnector());

	return m_instance;
}


IOSessionMap& IOConnector::sessionMap()
{
	return m_sessions;
}

IOConnector::IOConnector() :
	m_iosWork(m_ioService),
	m_resolver(m_ioService),
	_poolWork(_poolIOService)
{
	for (size_t index=0;index<16;++index)
		_poolThreadGroup.create_thread(boost::bind(&boost::asio::io_service::run,&_poolIOService));
}

bool IOConnector::connect(const string& address,const uint32_t& port)
{


	m_iosThread.reset(new thread( boost::bind(&io_service::run,&m_ioService)));

	boost::mutex waitMutex;
	boost::condition_variable waitCondition;
	boost::unique_lock<mutex> uniqueLock(waitMutex);
	bool result = false;

	ConnectCallback callback( [&result,&waitMutex,&waitCondition](const bool& connectResult)
		{	
			boost::unique_lock<mutex> uniqueLock(waitMutex);
			result=connectResult;
			waitCondition.notify_all();
			
		} );

	m_resolver.async_resolve(
		tcp::resolver::query(address,boost::lexical_cast<std::string>(port)),
		boost::bind(&IOConnector::resolveHandler,this,callback,asio::placeholders::error,boost::asio::placeholders::iterator));

	waitCondition.wait(uniqueLock);
	return result;
}


void IOConnector::connect(ConnectCallback connectCallback,const string& address,const uint32_t& port)
{

	m_resolver.async_resolve(
		tcp::resolver::query(address,boost::lexical_cast<std::string>(port)),
		boost::bind(&IOConnector::resolveHandler,this,connectCallback,asio::placeholders::error,boost::asio::placeholders::iterator));
}

bool IOConnector::disconnect()
{
	{
	boost::mutex::scoped_lock connectorLock(_connectorMutex);
	BOOST_FOREACH(IOSessionMap::value_type& sessionData,m_sessions)
	{
		IOSession* session = sessionData.second;
		session->disconnect();
	}

	m_ioService.stop();
	_poolIOService.stop();

	}
	if(m_iosThread)
		m_iosThread->join();

	_poolThreadGroup.join_all();
	

	return true;
}

IOConnector::~IOConnector()
{
	disconnect();

}


void IOConnector::resolveHandler(ConnectCallback connectCallback,const boost::system::error_code& error,tcp::resolver::iterator iterator)
{
	if (error || iterator == tcp::resolver::iterator()) return connectCallback(false);

	IOSessionPtr connectSession(new IOSession(m_ioService));

	connectSession->socket().async_connect(iterator->endpoint(),
		boost::bind(&IOConnector::connectHandler,this,connectSession,connectCallback,asio::placeholders::error));

}


OnTickHandlerId IOConnector::registerOnTickHandler(const OnTickHandlerFunc& newHandler)
{
	boost::mutex::scoped_lock connectorLock(_connectorMutex);

	OnTickHandlerId newId = boost::uuids::random_generator()();
	_onTickHandlerList[newId] = make_pair(false,newHandler);
	return newId;
};


void IOConnector::onTickDispatcher(const OnTickHandlerFunc& handlerFunc,IOSessionPtr session,const MessageUID& messageuid,const OnTickHandlerId& id,const std::string& symbol, const double& bid, const double& ask)
{
	handlerFunc(symbol,bid,ask);

	//MessageResultPtr result = MessageResult::Create(MessageEventType,messageuid);

	boost::mutex::scoped_lock connectorLock(_connectorMutex);

	if (_onTickHandlerList.find(id) != _onTickHandlerList.end())
		_onTickHandlerList[id].first = false;
	
	//session->writeMessage(result);
	
}



void IOConnector::notifyOnTick(IOSessionPtr session,const MessageUID& messageuid, const std::string& symbol, const double& bid, const double& ask)
{

	BOOST_FOREACH(OnTickHandlerList::value_type& tickHandlerDetail,_onTickHandlerList)
	{
		boost::mutex::scoped_lock connectorLock(_connectorMutex);
		if (!tickHandlerDetail.second.first)
		{	
			tickHandlerDetail.second.first = true;
			_poolIOService.post(boost::bind(&IOConnector::onTickDispatcher,shared_from_this(),tickHandlerDetail.second.second,session,messageuid,tickHandlerDetail.first,symbol,bid,ask));
		}
	}

};


void IOConnector::connectHandler(IOSessionPtr connectSession,ConnectCallback connectCallback,const boost::system::error_code& error)
{
	if (error)  return connectCallback(false);

	boost::mutex::scoped_lock connectorLock(_connectorMutex);

	tcp::endpoint& remoteEndpoint = connectSession->socket().remote_endpoint();
	m_sessions.insert(make_pair(make_pair(remoteEndpoint.address().to_v4().to_string(),remoteEndpoint.port()),connectSession.get()));

	connectSession->initialize();
	connectCallback(true);
}


PendingResultPtr IOConnector::dispatchMessage(const MessageHeaderPtr message)
{

	boost::mutex::scoped_lock connectorLock(_connectorMutex);
	
	if (!m_sessions.size())
	{
		PendingResultPtr invalidResult = PendingResult::Create();
		invalidResult->setError(ERR_PYMT4_NOT_CONNECTED);
		return invalidResult;
	}

	IOSession* iosession = m_sessions.begin()->second;

	return iosession->dispatchMessage(message);

}