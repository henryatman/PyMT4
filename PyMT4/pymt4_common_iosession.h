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
#include <vector>
#include <map>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/asio.hpp>

#include "pymt4_common.h"
#include "pymt4_common_message.h"

#include <boost/function_types/function_type.hpp>
#include <boost/function_types/result_type.hpp>
#include <boost/function_types/parameter_types.hpp>
#include <boost/mpl/pop_front.hpp>
#include <boost/mpl/push_front.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "pymt4_common_serializer.h"

namespace PyMT4
{
using namespace boost;
using namespace boost::asio;
using namespace boost::asio::ip;

DECLARE(IOSessionCommon);

typedef uuids::uuid IOSessionID;


template <typename F>
struct signature_of_member
{
    typedef typename boost::function_types::result_type<F>::type result_type;
    typedef typename boost::function_types::parameter_types<F>::type parameter_types;
    typedef typename boost::mpl::pop_front<parameter_types>::type base;
    typedef typename boost::mpl::push_front<base, result_type>::type L;
    typedef typename boost::function_types::function_type<L>::type type;
};



typedef bool(IOSessionCommon::*MessageHandler)(const MessageTypeIdentifier&,const MessageUID& , BufferCPos&, BufferCPos&,const boost::system::error_code&);
typedef boost::function<signature_of_member<MessageHandler>::type> MessageHandlerFunc;

typedef std::vector<MessageHandlerFunc> MessageHandlerMap;



class IOSessionCommon : public boost::enable_shared_from_this<IOSessionCommon>
{

public:
	

	IOSessionCommon(io_service& ioService);

	virtual ~IOSessionCommon();


	inline tcp::socket&	socket() { return m_socket; }
	inline IOSessionID& id()	 { return m_id; }
	bool   disconnect();
	void   writeMessage(MessageHeaderPtr message);

protected:

	tcp::socket			m_socket;
	IOSessionID			m_id;
	io_service&			m_ioService;
	Buffer				m_readBuffer;
	Buffer				m_writeBuffer;


	MessageHandlerMap	m_messageHandlerMap;

protected:
	boost::mutex        _sessionMutex;
private:

	void headerHandler(const boost::system::error_code& code);
	void messageHandler(const MessageUID& messageuid, const MessageTypeIdentifier&,const boost::system::error_code& code);
	void writeHandler(MessageHeaderPtr message,const boost::system::error_code& code);

protected:
	void registerHandler(const MessageTypeIdentifier& message,const MessageHandlerFunc& function);
	void readHeader();


};

}