#pragma once
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

#include "pymt4_server.h"
#include "pymt4_common_serializer.h"
#include "pymt4_server_ioserver.h"

namespace PyMT4
{

	typedef std::map<boost::thread::id, int32_t> LastErrorMap;

	extern LastErrorMap _lastErrorMap;
	extern boost::mutex _lastErrorMapLock;
	extern boost::mutex _dispatchLock;


	template<class Tuple, std::size_t... Is>
	void SerializeArguments(Buffer* serialized_buffer, Tuple* tuple, std::index_sequence<Is...>)
	{
		using discard = int[];
		(void)discard {
			0, (
				void(

					Serializer< std::remove_pointer<std::remove_const<std::tuple_element<Is, Tuple>::type>::type>::type   >::serializeItem(std::get<Is>(*tuple), serialized_buffer)

					), 0)...
		};
	}

	template <typename R, typename... A> R DispatchEvent(const EventIdentifier& eventId, const A&... args)
	{

		//boost::mutex::scoped_lock dispatchLock(_dispatchLock);

		std::tuple< const A*...> tuple(&args...);

		MessageEventPtr messageEvent = MessageEvent::Create(eventId);
		Buffer& messageBuffer = messageEvent->messageBuffer();
		SerializeArguments(&messageBuffer, &tuple, std::index_sequence_for<A...>{});

		IOServerPtr server = IOServer::Instance();
		server->dispatchEvent(messageEvent);
		//dispatchLock.lock();
		return R();

	}


}