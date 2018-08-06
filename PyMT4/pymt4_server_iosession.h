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

#include "pymt4_common_iosession.h"
#include "pymt4_common_serializer.h"

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
	DECLARE(PendingCommand);

	class  PendingCommand
	{

		PendingCommand();

	public:

		static PendingCommandPtr    Create();

		IOSessionWPtr			session;
		CommandIdentifier       commandId;
		MessageUID				messageUID;
		Buffer					data;
		BufferPos				dataPos;

	};


	class IOSession : public IOSessionCommon
	{

	public:

		IOSession(io_service& ioService);
		~IOSession();
		bool MessageCommandHandler(const MessageTypeIdentifier&, const MessageUID& messageuid, Buffer::const_iterator&, Buffer::const_iterator&,const boost::system::error_code&);
		bool messageResultHandler(const MessageTypeIdentifier& messageId,const MessageUID& messageuid, Buffer::const_iterator& begin, Buffer::const_iterator& end ,const boost::system::error_code& error);
		void   initialize();

	};



}