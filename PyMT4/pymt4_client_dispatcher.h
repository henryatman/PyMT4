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

#include <typeinfo>
#include <vector>

#include "pymt4_client.h"
#include "pymt4_common_serializer.h"
#include "pymt4_client_ioconnector.h"

namespace PyMT4
{

typedef std::map<boost::thread::id,int32_t> LastErrorMap;

extern LastErrorMap _lastErrorMap;
extern boost::mutex _lastErrorMapLock;
extern boost::mutex _dispatchLock;


template<class Tuple, std::size_t... Is>
void SerializeArguments(Buffer* serialized_buffer,Tuple* tuple, std::index_sequence<Is...>)
{
	using discard = int[];
	(void)discard{ 0, (
		void(

			Serializer< std::remove_pointer<std::remove_const<std::tuple_element<Is, Tuple>::type>::type>::type   >::serializeItem(std::get<Is>(*tuple), serialized_buffer) 

		), 0)... };
}

template <typename R,typename... A> R DispatchFunction(const CommandIdentifier& commandIdentifier, const A&... args)
{

	//boost::mutex::scoped_lock dispatchLock(_dispatchLock);

	std::tuple< const A*...> tuple(&args...);

	MessageCommandPtr messageCommand = MessageCommand::Create(commandIdentifier);
	Buffer& messageBuffer = messageCommand->messageBuffer();
	SerializeArguments(&messageBuffer, &tuple, std::index_sequence_for<A...>{});

	IOConnectorPtr connector = IOConnector::Instance();
	
	PendingResultPtr pendingResult = connector->dispatchMessage(messageCommand);
	//	dispatchLock.unlock();
	
	R resultValue = pendingResult->waitForResult<R>();

	{
		boost::mutex::scoped_lock errorMapLock(_lastErrorMapLock);
		int32_t lastError = pendingResult->error();
		_lastErrorMap[boost::this_thread::get_id()] = lastError;
	}
	//dispatchLock.lock();
	return resultValue;
	
}


}

