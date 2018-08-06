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
#include "pymt4_common_iosession.h"
#include <deque>

namespace PyMT4
{

	DECLARE(IOSession);

	typedef std::deque<PendingResultPtr> PendingResultQueue;

	class IOSession : public IOSessionCommon
	{



		bool messageResultHandler(const MessageTypeIdentifier& messageId,const MessageUID& messageuid, Buffer::const_iterator& begin, Buffer::const_iterator& end ,const boost::system::error_code& error);
		bool messageEventHandler(const MessageTypeIdentifier& messageId,const MessageUID& messageuid, Buffer::const_iterator& begin, Buffer::const_iterator& end ,const boost::system::error_code& error);

		PendingResultQueue _pendingResultQueue;

	public:
		IOSession(io_service& ioService);
		~IOSession();
		PendingResultPtr dispatchMessage(MessageHeaderPtr message);
		void   initialize();
		void   shutdown();

	};




}