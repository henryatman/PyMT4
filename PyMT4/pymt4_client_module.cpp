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


#include "pymt4_client_module.h"
#include <boost/python/stl_iterator.hpp>

using namespace boost::python;


typedef std::list<std::pair<std::string,boost::python::object>> PyOnTickHandlerList;

PyOnTickHandlerList pyOnTickHandlerList;

void onTickHandler(const std::string& symbol,const double& bid,const double& ask)
{
	PyGILState_STATE gilState = PyGILState_Ensure();
	BOOST_FOREACH(PyOnTickHandlerList::value_type& callbackDetail,pyOnTickHandlerList)
	{
		if ( (!callbackDetail.first.compare("*")) || !callbackDetail.first.compare(symbol) )
		{
			try
			{
				boost::python::call<void>(callbackDetail.second.ptr(),symbol,bid,ask);
			} catch ( boost::python::error_already_set& ) {
				PyErr_Print();
			}
		}	
	
	}

	PyGILState_Release(gilState);
}



bool RegisterOnTickHandler(const std::string& symbol,boost::python::object& handler)
{

	if (!PyCallable_Check(handler.ptr()))
		return false;

	BOOST_FOREACH(PyOnTickHandlerList::value_type& callbackDetail,pyOnTickHandlerList)
	{
		if (callbackDetail.second == handler)
			return false;
	}

	pyOnTickHandlerList.push_back(make_pair(symbol,handler));
	return true;
}

bool UnregisterOnTickHandler(boost::python::object& handler)
{

	if (!PyCallable_Check(handler.ptr()))
		return false;

	BOOST_FOREACH(PyOnTickHandlerList::value_type& callbackDetail, pyOnTickHandlerList)
	{
		if (callbackDetail.second == handler)
		{
			pyOnTickHandlerList.remove(callbackDetail);
			return true;
		}
	}
	return false;
}

bool Connect()
{
	static bool isConnected = false;
	
	if (isConnected)
		return true;


	PyMT4::IOConnectorPtr ioConnector = PyMT4::IOConnector::Instance();
	ioConnector->registerOnTickHandler(boost::bind(&onTickHandler,_1,_2,_3));

	isConnected = ioConnector->connect();
	return isConnected;
}

bool Disconnect()
{
	PyMT4::IOConnectorPtr ioConnector = PyMT4::IOConnector::Instance();
	ioConnector->disconnect();
	return true;
}


void exit_module()
{
	Disconnect();
}


void init_module()
{
	PyEval_InitThreads();
	int status = Py_AtExit(exit_module);
}

namespace PyProxyCommand
{
	bool ObjectCreate(const std::string& object_name, const int32_t& object_type, const int32_t& subwindow, boost::python::object& prices)
	{
		if (!PyList_Check(prices.ptr()))
		{
			PyErr_SetString(PyExc_ValueError, "Invalid prices argument. Expected [(int,float),(int,float), ...]");
			throw boost::python::error_already_set();
		}

		else
		{
			boost::python::list priceslist(prices);
			std::vector<std::pair<PyMT4::Datetime, double>> pricesvector;
			pricesvector.reserve(boost::python::len(priceslist));

			for (stl_input_iterator<boost::python::object> item(priceslist), end; item != end;++item)
			{
				if (!PyTuple_Check((*item).ptr()))
				{
					PyErr_SetString(PyExc_ValueError, "Invalid list element. Expected [(int,float),(int,float), ...]");
					throw boost::python::error_already_set();
				}
				else if (boost::python::len(*item) != 2)
				{
					PyErr_SetString(PyExc_ValueError, "Invalid tuple element item count. Expected [(int,float),(int,float), ...]");
					throw boost::python::error_already_set();
				}
				else
				{
					boost::python::tuple pricepair(*item);
					boost::python::object first  = pricepair[0];
					boost::python::object second = pricepair[1];

					if (!PyNumber_Check(first.ptr()) || !PyNumber_Check(second.ptr()))
					{
						PyErr_SetString(PyExc_ValueError, "Invalid tuple element item type. Expected [(int,float),(int,float), ...]");
						throw boost::python::error_already_set();
					}

					uint32_t time  = boost::python::extract<PyMT4::Datetime>(first)();
					double   price = boost::python::extract<double>(second)();
					
					pricesvector.push_back(std::make_pair(time, price));
				}

			}

			return PyMT4::Command::ObjectCreate(object_name, object_type, subwindow, pricesvector);

		}
	}

}

BOOST_PYTHON_MODULE(PyMT4)
{
		
		init_module();

		using namespace PyMT4;
		using namespace PyMT4::Command;

		def("Connect",&Connect);
		def("Disconnect",&Disconnect);

		def("AccountBalance",&AccountBalance);
		def("AccountCredit",&AccountCredit);
		def("AccountCompany",&AccountCompany);
		def("AccountCurrency",&AccountCurrency);
		def("AccountEquity",&AccountEquity);
		def("AccountFreeMargin",&AccountFreeMargin);
		def("AccountFreeMarginCheck",&AccountFreeMarginCheck);
		def("AccountFreeMarginMode",&AccountFreeMarginMode);
		def("AccountLeverage",&AccountLeverage);
		def("AccountMargin",&AccountMargin);
		def("AccountName",&AccountName);
		def("AccountNumber",&AccountNumber);
		def("AccountProfit",&AccountProfit);
		def("AccountServer",&AccountServer);
		def("AccountStopoutLevel",&AccountStopoutLevel);
		def("AccountStopoutMode",&AccountStopoutMode);
		def("AccountInfoDouble", &AccountInfoDouble);
		def("AccountInfoInteger", &AccountInfoInteger);
		def("AccountInfoString", &AccountInfoString);

		def("iBars",&iBars);
		def("iBarShift",&iBarShift);
		def("iClose",&iClose);
		def("iHigh",&iHigh);
		def("iHighest",&iHighest);
		def("iLow",&iLow);
		def("iLowest",&iLowest);
		def("iOpen",&iOpen);
		def("iTime",&iTime);
		def("iVolume",&iVolume);

		def ("OrderClose",&OrderClose);
		def ("OrderCloseBy",&OrderCloseBy);
		def ("OrderClosePrice",&OrderClosePrice);
		def ("OrderCloseTime",&OrderCloseTime);
		def ("OrderComment",&OrderComment);
		def ("OrderCommission",&OrderCommission);
		def ("OrderDelete",&OrderDelete);
		def ("OrderExpiration",&OrderExpiration);
		def ("OrderLots",&OrderLots);
		def ("OrderMagicNumber",&OrderMagicNumber);
		def ("OrderModify",&OrderModify);
		def ("OrderOpenPrice",&OrderOpenPrice);
		def ("OrderOpenTime",&OrderOpenTime);
		def ("OrderPrint",&OrderPrint);
		def ("OrderProfit",&OrderProfit);
		def ("OrderSelect",&OrderSelect);
		def ("OrderSend",&OrderSend);
		def ("OrdersHistoryTotal",&OrdersHistoryTotal);
		def ("OrderStopLoss",&OrderStopLoss);
		def ("OrdersTotal",&OrdersTotal);
		def ("OrderSwap",&OrderSwap);
		def ("OrderSymbol",&OrderSymbol);
		def ("OrderTakeProfit",&OrderTakeProfit);
		def ("OrderTicket",&OrderTicket);
		def ("OrderType",&OrderType);

		def("IsConnected",&IsConnected);
		def("IsDemo",&IsDemo);
		def("IsDllsAllowed",&IsDllsAllowed);
		def("IsExpertEnabled",&IsExpertEnabled);
		def("IsLibrariesAllowed",&IsLibrariesAllowed);
		def("IsOptimization",&IsOptimization);
		def("IsStopped",&IsStopped);
		def("IsTesting",&IsTesting);
		def("IsTradeAllowed",&IsTradeAllowed);
		def("IsTradeContextBusy",&IsTradeContextBusy);
		def("IsVisualMode",&IsVisualMode);
		def("UninitializeReason",&UninitializeReason);

		def("TimeCurrent", &Command::TimeCurrent);
		def("TimeLocal", &Command::TimeLocal);
		def("TimeGMT", &Command::TimeGMT);
		def("TimeGMTOffset", &Command::TimeGMTOffset);

		def("TerminalCompany",&TerminalCompany);
		def("TerminalName",&TerminalName);
		def("TerminalPath",&TerminalPath);

		def("Print",&Print);
		def("Alert",&Alert);
		def("Comment",&Comment);
		def("GetTickCount",&Command::GetTickCount);
		def("MarketInfo",&MarketInfo);
		def("PlaySound",&PlaySound);
		def("SendFTP",&SendFTP);
		def("SendMail",&SendMail);
		def("SendNotification",&SendNotification);
		def("GetLastError",&Command::GetLastError);
		def("RefreshRates", &Command::RefreshRates);

		def("ObjectName", &Command::ObjectName);
		def("ObjectCreate", &PyProxyCommand::ObjectCreate);

		def("WindowRedraw", &Command::WindowRedraw);

		def("RegisterOnTickHandler",&RegisterOnTickHandler);
		def("UnregisterOnTickHandler", &UnregisterOnTickHandler);

		enum_<Constant>("Constant")
			.value("ACCOUNT_STOPOUT_MODE_MONEY", ACCOUNT_STOPOUT_MODE_MONEY)
			.value("ACCOUNT_STOPOUT_MODE_PERCENT", ACCOUNT_STOPOUT_MODE_PERCENT)
			.value("ACCOUNT_TRADE_MODE_REAL", ACCOUNT_TRADE_MODE_REAL)
			.value("ACCOUNT_TRADE_MODE_CONTEST", ACCOUNT_TRADE_MODE_CONTEST)
			.value("ACCOUNT_TRADE_MODE_DEMO", ACCOUNT_TRADE_MODE_DEMO)
			.value("ACCOUNT_COMPANY", ACCOUNT_COMPANY)
			.value("ACCOUNT_CURRENCY", ACCOUNT_CURRENCY)
			.value("ACCOUNT_SERVER", ACCOUNT_SERVER)
			.value("ACCOUNT_NAME", ACCOUNT_NAME)
			.value("ACCOUNT_MARGIN_SO_SO", ACCOUNT_MARGIN_SO_SO)
			.value("ACCOUNT_MARGIN_SO_CALL", ACCOUNT_MARGIN_SO_CALL)
			.value("ACCOUNT_MARGIN_LEVEL", ACCOUNT_MARGIN_LEVEL)
			.value("ACCOUNT_FREEMARGIN", ACCOUNT_FREEMARGIN)
			.value("ACCOUNT_MARGIN", ACCOUNT_MARGIN)
			.value("ACCOUNT_EQUITY", ACCOUNT_EQUITY)
			.value("ACCOUNT_PROFIT", ACCOUNT_PROFIT)
			.value("ACCOUNT_CREDIT", ACCOUNT_CREDIT)
			.value("ACCOUNT_BALANCE", ACCOUNT_BALANCE)
			.value("ACCOUNT_TRADE_EXPERT", ACCOUNT_TRADE_EXPERT)
			.value("ACCOUNT_TRADE_ALLOWED", ACCOUNT_TRADE_ALLOWED)
			.value("ACCOUNT_MARGIN_SO_MODE", ACCOUNT_MARGIN_SO_MODE)
			.value("ACCOUNT_LIMIT_ORDERS", ACCOUNT_LIMIT_ORDERS)
			.value("ACCOUNT_LEVERAGE", ACCOUNT_LEVERAGE)
			.value("ACCOUNT_TRADE_MODE", ACCOUNT_TRADE_MODE)
			.value("ACCOUNT_LOGIN", ACCOUNT_LOGIN)
			.value("PERIOD_M1",PERIOD_M1)
			.value("PERIOD_M5",PERIOD_M5)
			.value("PERIOD_M15",PERIOD_M15)
			.value("PERIOD_M30",PERIOD_M30)
			.value("PERIOD_H1",PERIOD_H1)
			.value("PERIOD_H4",PERIOD_H4)
			.value("PERIOD_D1",PERIOD_D1)
			.value("PERIOD_W1",PERIOD_W1)
			.value("PERIOD_MN1",PERIOD_MN1)
			.value("CHART_TIMEFRAME",CHART_TIMEFRAME)
			.value("OP_BUY",OP_BUY)
			.value("OP_SELL",OP_SELL)
			.value("OP_BUYLIMIT",OP_BUYLIMIT)
			.value("OP_SELLLIMIT",OP_SELLLIMIT)
			.value("OP_BUYSTOP",OP_BUYSTOP)
			.value("OP_SELLSTOP",OP_SELLSTOP)
			.value("PRICE_CLOSE",PRICE_CLOSE)
			.value("PRICE_OPEN",PRICE_OPEN)
			.value("PRICE_HIGH",PRICE_HIGH)
			.value("PRICE_LOW",PRICE_LOW)
			.value("PRICE_MEDIAN",PRICE_MEDIAN)
			.value("PRICE_TYPICAL",PRICE_TYPICAL)
			.value("PRICE_WEIGHTED",PRICE_WEIGHTED)
			.value("MODE_TRADES",MODE_TRADES)
			.value("MODE_HISTORY",MODE_HISTORY)
			.value("MODE_OPEN",MODE_OPEN)
			.value("MODE_LOW",MODE_LOW)
			.value("MODE_HIGH",MODE_HIGH)
			.value("MODE_CLOSE",MODE_CLOSE)
			.value("MODE_VOLUME",MODE_VOLUME)
			.value("MODE_TIME",MODE_TIME)
			.value("MODE_BID",MODE_BID)
			.value("MODE_ASK",MODE_ASK)
			.value("MODE_POINT",MODE_POINT)
			.value("MODE_DIGITS",MODE_DIGITS)
			.value("MODE_SPREAD",MODE_SPREAD)
			.value("MODE_STOPLEVEL",MODE_STOPLEVEL)
			.value("MODE_LOTSIZE",MODE_LOTSIZE)
			.value("MODE_TICKVALUE",MODE_TICKVALUE)
			.value("MODE_TICKSIZE",MODE_TICKSIZE)
			.value("MODE_SWAPLONG",MODE_SWAPLONG)
			.value("MODE_SWAPSHORT",MODE_SWAPSHORT)
			.value("MODE_STARTING",MODE_STARTING)
			.value("MODE_EXPIRATION",MODE_EXPIRATION)
			.value("MODE_TRADEALLOWED",MODE_TRADEALLOWED)
			.value("MODE_MINLOT",MODE_MINLOT)
			.value("MODE_LOTSTEP",MODE_LOTSTEP)
			.value("MODE_MAXLOT",MODE_MAXLOT)
			.value("MODE_SWAPTYPE",MODE_SWAPTYPE)
			.value("MODE_PROFITCALCMODE",MODE_PROFITCALCMODE)
			.value("MODE_MARGINCALCMODE",MODE_MARGINCALCMODE)
			.value("MODE_MARGININIT",MODE_MARGININIT)
			.value("MODE_MARGINMAINTENANCE",MODE_MARGINMAINTENANCE)
			.value("MODE_MARGINHEDGED",MODE_MARGINHEDGED)
			.value("MODE_MARGINREQUIRED",MODE_MARGINREQUIRED)
			.value("MODE_FREEZELEVEL",MODE_FREEZELEVEL)
			.value("DRAW_LINE",DRAW_LINE)
			.value("DRAW_SECTION",DRAW_SECTION)
			.value("DRAW_HISTOGRAM",DRAW_HISTOGRAM)
			.value("DRAW_ARROW",DRAW_ARROW)
			.value("DRAW_ZIGZAG",DRAW_ZIGZAG)
			.value("DRAW_NONE",DRAW_NONE)
			.value("STYLE_SOLID",STYLE_SOLID)
			.value("STYLE_DASH",STYLE_DASH)
			.value("STYLE_DOT",STYLE_DOT)
			.value("STYLE_DASHDOT",STYLE_DASHDOT)
			.value("STYLE_DASHDOTDOT",STYLE_DASHDOTDOT)
			.value("SYMBOL_THUMBSUP",SYMBOL_THUMBSUP)
			.value("SYMBOL_THUMBSDOWN",SYMBOL_THUMBSDOWN)
			.value("SYMBOL_ARROWUP",SYMBOL_ARROWUP)
			.value("SYMBOL_ARROWDOWN",SYMBOL_ARROWDOWN)
			.value("SYMBOL_STOPSIGN",SYMBOL_STOPSIGN)
			.value("SYMBOL_CHECKSIGN",SYMBOL_CHECKSIGN)
			.value("SYMBOL_LEFTPRICE",SYMBOL_LEFTPRICE)
			.value("SYMBOL_RIGHTPRICE",SYMBOL_RIGHTPRICE)
			.value("MODE_MAIN",MODE_MAIN)
			.value("MODE_SIGNAL",MODE_SIGNAL)
			.value("MODE_PLUSDI",MODE_PLUSDI)
			.value("MODE_MINUSDI",MODE_MINUSDI)
			.value("MODE_UPPER",MODE_UPPER)
			.value("MODE_LOWER",MODE_LOWER)
			.value("MODE_TENKANSEN",MODE_TENKANSEN)
			.value("MODE_KIJUNSEN",MODE_KIJUNSEN)
			.value("MODE_SENKOUSPANA",MODE_SENKOUSPANA)
			.value("MODE_SENKOUSPANB",MODE_SENKOUSPANB)
			.value("MODE_CHINKOUSPAN",MODE_CHINKOUSPAN)
			.value("MODE_SMA",MODE_SMA)
			.value("MODE_EMA",MODE_EMA)
			.value("MODE_SMMA",MODE_SMMA)
			.value("MODE_LWMA",MODE_LWMA)
			.value("IDOK",_IDOK)
.value("IDCANCEL", _IDCANCEL)
.value("IDABORT", _IDABORT)
.value("IDRETRY", _IDRETRY)
.value("IDIGNORE", _IDIGNORE)
.value("IDYES", _IDYES)
.value("IDNO", _IDNO)
.value("IDTRYAGAIN", _IDTRYAGAIN)
.value("IDCONTINUE", _IDCONTINUE)
.value("MB_OK", _MB_OK)
.value("MB_OKCANCEL", _MB_OKCANCEL)
.value("MB_ABORTRETRYIGNORE", _MB_ABORTRETRYIGNORE)
.value("MB_YESNOCANCEL", _MB_YESNOCANCEL)
.value("MB_YESNO", _MB_YESNO)
.value("MB_RETRYCANCEL", _MB_RETRYCANCEL)
.value("MB_CANCELTRYCONTINUE", _MB_CANCELTRYCONTINUE)
.value("MB_ICONSTOP", _MB_ICONSTOP)
.value("MB_ICONERROR", _MB_ICONERROR)
.value("MB_ICONHAND", _MB_ICONHAND)
.value("MB_ICONQUESTION", _MB_ICONQUESTION)
.value("MB_ICONEXCLAMATION", _MB_ICONEXCLAMATION)
.value("MB_ICONWARNING", _MB_ICONWARNING)
.value("MB_ICONINFORMATION", _MB_ICONINFORMATION)
.value("MB_ICONASTERISK", _MB_ICONASTERISK)
.value("MB_DEFBUTTON1", _MB_DEFBUTTON1)
.value("MB_DEFBUTTON2", _MB_DEFBUTTON2)
.value("MB_DEFBUTTON3", _MB_DEFBUTTON3)
.value("MB_DEFBUTTON4", _MB_DEFBUTTON4)
.value("OBJ_VLINE", OBJ_VLINE)
.value("OBJ_HLINE", OBJ_HLINE)
.value("OBJ_TREND", OBJ_TREND)
.value("OBJ_TRENDBYANGLE", OBJ_TRENDBYANGLE)
.value("OBJ_REGRESSION", OBJ_REGRESSION)
.value("OBJ_CHANNEL", OBJ_CHANNEL)
.value("OBJ_STDDEVCHANNEL", OBJ_STDDEVCHANNEL)
.value("OBJ_GANNLINE", OBJ_GANNLINE)
.value("OBJ_GANNFAN", OBJ_GANNFAN)
.value("OBJ_GANNGRID", OBJ_GANNGRID)
.value("OBJ_FIBO", OBJ_FIBO)
.value("OBJ_FIBOTIMES", OBJ_FIBOTIMES)
.value("OBJ_FIBOFAN", OBJ_FIBOFAN)
.value("OBJ_FIBOARC", OBJ_FIBOARC)
.value("OBJ_EXPANSION", OBJ_EXPANSION)
.value("OBJ_FIBOCHANNEL", OBJ_FIBOCHANNEL)
.value("OBJ_RECTANGLE", OBJ_RECTANGLE)
.value("OBJ_TRIANGLE", OBJ_TRIANGLE)
.value("OBJ_ELLIPSE", OBJ_ELLIPSE)
.value("OBJ_PITCHFORK", OBJ_PITCHFORK)
.value("OBJ_CYCLES", OBJ_CYCLES)
.value("OBJ_TEXT", OBJ_TEXT)
.value("OBJ_ARROW", OBJ_ARROW)
.value("OBJ_LABEL", OBJ_LABEL)
.value("OBJPROP_TIME1", OBJPROP_TIME1)
.value("OBJPROP_PRICE1", OBJPROP_PRICE1)
.value("OBJPROP_TIME2", OBJPROP_TIME2)
.value("OBJPROP_PRICE2", OBJPROP_PRICE2)
.value("OBJPROP_TIME3", OBJPROP_TIME3)
.value("OBJPROP_PRICE3", OBJPROP_PRICE3)
.value("OBJPROP_COLOR", OBJPROP_COLOR)
.value("OBJPROP_STYLE", OBJPROP_STYLE)
.value("OBJPROP_WIDTH", OBJPROP_WIDTH)
.value("OBJPROP_BACK", OBJPROP_BACK)
.value("OBJPROP_RAY", OBJPROP_RAY)
.value("OBJPROP_ELLIPSE", OBJPROP_ELLIPSE)
.value("OBJPROP_SCALE", OBJPROP_SCALE)
.value("OBJPROP_ANGLE", OBJPROP_ANGLE)
.value("OBJPROP_ARROWCODE", OBJPROP_ARROWCODE)
.value("OBJPROP_TIMEFRAMES", OBJPROP_TIMEFRAMES)
.value("OBJPROP_DEVIATION", OBJPROP_DEVIATION)
.value("OBJPROP_FONTSIZE", OBJPROP_FONTSIZE)
.value("OBJPROP_CORNER", OBJPROP_CORNER)
.value("OBJPROP_XDISTANCE", OBJPROP_XDISTANCE)
.value("OBJPROP_YDISTANCE", OBJPROP_YDISTANCE)
.value("OBJPROP_FIBOLEVELS", OBJPROP_FIBOLEVELS)
.value("OBJPROP_LEVELCOLOR", OBJPROP_LEVELCOLOR)
.value("OBJPROP_LEVELSTYLE", OBJPROP_LEVELSTYLE)
.value("OBJPROP_LEVELWIDTH", OBJPROP_LEVELWIDTH)
.value("OBJPROP_FIRSTLEVEL", OBJPROP_FIRSTLEVEL)
.value("OBJ_PERIOD_M1", OBJ_PERIOD_M1)
.value("OBJ_PERIOD_M5", OBJ_PERIOD_M5)
.value("OBJ_PERIOD_M15", OBJ_PERIOD_M15)
.value("OBJ_PERIOD_M30", OBJ_PERIOD_M30)
.value("OBJ_PERIOD_H1", OBJ_PERIOD_H1)
.value("OBJ_PERIOD_H4", OBJ_PERIOD_H4)
.value("OBJ_PERIOD_D1", OBJ_PERIOD_D1)
.value("OBJ_PERIOD_W1", OBJ_PERIOD_W1)
.value("OBJ_PERIOD_MN1", OBJ_PERIOD_MN1)
.value("OBJ_ALL_PERIODS", OBJ_ALL_PERIODS)
.value("OBJ_NULL", OBJ_NULL)
.value("OBJ_EMPTY", OBJ_EMPTY)
.value("REASON_REMOVE", REASON_REMOVE)
.value("REASON_RECOMPILE", REASON_RECOMPILE)
.value("REASON_CHARTCHANGE", REASON_CHARTCHANGE)
.value("REASON_CHARTCLOSE", REASON_CHARTCLOSE)
.value("REASON_PARAMETERS", REASON_PARAMETERS)
.value("REASON_ACCOUNT", REASON_ACCOUNT)
.value("NULL", _NULL)
.value("EMPTY", EMPTY)
.value("SELECT_BY_POS", SELECT_BY_POS)
.value("SELECT_BY_TICKET", SELECT_BY_TICKET)
.value("EMPTY_VALUE", _EMPTY_VALUE)
.value("CLR_NONE", CLR_NONE)
.value("WHOLE_ARRAY",WHOLE_ARRAY)
.export_values();

		enum_<Error>("Error")
			.value("ERR_NO_ERROR",ERR_NO_ERROR)
			.value("ERR_NO_RESULT",ERR_NO_RESULT)
			.value("ERR_COMMON_ERROR",ERR_COMMON_ERROR)
			.value("ERR_INVALID_TRADE_PARAMETERS",ERR_INVALID_TRADE_PARAMETERS)
			.value("ERR_SERVER_BUSY",ERR_SERVER_BUSY)
			.value("ERR_OLD_VERSION",ERR_OLD_VERSION)
			.value("ERR_NO_CONNECTION",ERR_NO_CONNECTION)
			.value("ERR_NOT_ENOUGH_RIGHTS",ERR_NOT_ENOUGH_RIGHTS)
			.value("ERR_TOO_FREQUENT_REQUESTS",ERR_TOO_FREQUENT_REQUESTS)
			.value("ERR_MALFUNCTIONAL_TRADE",ERR_MALFUNCTIONAL_TRADE)
			.value("ERR_ACCOUNT_DISABLED",ERR_ACCOUNT_DISABLED)
			.value("ERR_INVALID_ACCOUNT",ERR_INVALID_ACCOUNT)
			.value("ERR_TRADE_TIMEOUT",ERR_TRADE_TIMEOUT)
			.value("ERR_INVALID_PRICE",ERR_INVALID_PRICE)
			.value("ERR_INVALID_STOPS",ERR_INVALID_STOPS)
			.value("ERR_INVALID_TRADE_VOLUME",ERR_INVALID_TRADE_VOLUME)
			.value("ERR_MARKET_CLOSED",ERR_MARKET_CLOSED)
			.value("ERR_TRADE_DISABLED",ERR_TRADE_DISABLED)
			.value("ERR_NOT_ENOUGH_MONEY",ERR_NOT_ENOUGH_MONEY)
			.value("ERR_PRICE_CHANGED",ERR_PRICE_CHANGED)
			.value("ERR_OFF_QUOTES",ERR_OFF_QUOTES)
			.value("ERR_BROKER_BUSY",ERR_BROKER_BUSY)
			.value("ERR_REQUOTE",ERR_REQUOTE)
			.value("ERR_ORDER_LOCKED",ERR_ORDER_LOCKED)
			.value("ERR_LONG_POSITIONS_ONLY_ALLOWED",ERR_LONG_POSITIONS_ONLY_ALLOWED)
			.value("ERR_TOO_MANY_REQUESTS",ERR_TOO_MANY_REQUESTS)
			.value("ERR_TRADE_MODIFY_DENIED",ERR_TRADE_MODIFY_DENIED)
			.value("ERR_TRADE_CONTEXT_BUSY",ERR_TRADE_CONTEXT_BUSY)
			.value("ERR_TRADE_EXPIRATION_DENIED",ERR_TRADE_EXPIRATION_DENIED)
			.value("ERR_TRADE_TOO_MANY_ORDERS",ERR_TRADE_TOO_MANY_ORDERS)
			.value("ERR_TRADE_HEDGE_PROHIBITED",ERR_TRADE_HEDGE_PROHIBITED)
			.value("ERR_TRADE_PROHIBITED_BY_FIFO",ERR_TRADE_PROHIBITED_BY_FIFO)
			.value("ERR_NO_MQLERROR",ERR_NO_MQLERROR)
			.value("ERR_WRONG_FUNCTION_POINTER",ERR_WRONG_FUNCTION_POINTER)
			.value("ERR_ARRAY_INDEX_OUT_OF_RANGE",ERR_ARRAY_INDEX_OUT_OF_RANGE)
			.value("ERR_NO_MEMORY_FOR_CALL_STACK",ERR_NO_MEMORY_FOR_CALL_STACK)
			.value("ERR_RECURSIVE_STACK_OVERFLOW",ERR_RECURSIVE_STACK_OVERFLOW)
			.value("ERR_NOT_ENOUGH_STACK_FOR_PARAM",ERR_NOT_ENOUGH_STACK_FOR_PARAM)
			.value("ERR_NO_MEMORY_FOR_PARAM_STRING",ERR_NO_MEMORY_FOR_PARAM_STRING)
			.value("ERR_NO_MEMORY_FOR_TEMP_STRING",ERR_NO_MEMORY_FOR_TEMP_STRING)
			.value("ERR_NOT_INITIALIZED_STRING",ERR_NOT_INITIALIZED_STRING)
			.value("ERR_NOT_INITIALIZED_ARRAYSTRING",ERR_NOT_INITIALIZED_ARRAYSTRING)
			.value("ERR_NO_MEMORY_FOR_ARRAYSTRING",ERR_NO_MEMORY_FOR_ARRAYSTRING)
			.value("ERR_TOO_LONG_STRING",ERR_TOO_LONG_STRING)
			.value("ERR_REMAINDER_FROM_ZERO_DIVIDE",ERR_REMAINDER_FROM_ZERO_DIVIDE)
			.value("ERR_ZERO_DIVIDE",ERR_ZERO_DIVIDE)
			.value("ERR_UNKNOWN_COMMAND",ERR_UNKNOWN_COMMAND)
			.value("ERR_WRONG_JUMP",ERR_WRONG_JUMP)
			.value("ERR_NOT_INITIALIZED_ARRAY",ERR_NOT_INITIALIZED_ARRAY)
			.value("ERR_DLL_CALLS_NOT_ALLOWED",ERR_DLL_CALLS_NOT_ALLOWED)
			.value("ERR_CANNOT_LOAD_LIBRARY",ERR_CANNOT_LOAD_LIBRARY)
			.value("ERR_CANNOT_CALL_FUNCTION",ERR_CANNOT_CALL_FUNCTION)
			.value("ERR_EXTERNAL_CALLS_NOT_ALLOWED",ERR_EXTERNAL_CALLS_NOT_ALLOWED)
			.value("ERR_NO_MEMORY_FOR_RETURNED_STR",ERR_NO_MEMORY_FOR_RETURNED_STR)
			.value("ERR_SYSTEM_BUSY",ERR_SYSTEM_BUSY)
			.value("ERR_INVALID_FUNCTION_PARAMSCNT",ERR_INVALID_FUNCTION_PARAMSCNT)
			.value("ERR_INVALID_FUNCTION_PARAMVALUE",ERR_INVALID_FUNCTION_PARAMVALUE)
			.value("ERR_STRING_FUNCTION_INTERNAL",ERR_STRING_FUNCTION_INTERNAL)
			.value("ERR_SOME_ARRAY_ERROR",ERR_SOME_ARRAY_ERROR)
			.value("ERR_INCORRECT_SERIESARRAY_USING",ERR_INCORRECT_SERIESARRAY_USING)
			.value("ERR_CUSTOM_INDICATOR_ERROR",ERR_CUSTOM_INDICATOR_ERROR)
			.value("ERR_INCOMPATIBLE_ARRAYS",ERR_INCOMPATIBLE_ARRAYS)
			.value("ERR_GLOBAL_VARIABLES_PROCESSING",ERR_GLOBAL_VARIABLES_PROCESSING)
			.value("ERR_GLOBAL_VARIABLE_NOT_FOUND",ERR_GLOBAL_VARIABLE_NOT_FOUND)
			.value("ERR_FUNC_NOT_ALLOWED_IN_TESTING",ERR_FUNC_NOT_ALLOWED_IN_TESTING)
			.value("ERR_FUNCTION_NOT_CONFIRMED",ERR_FUNCTION_NOT_CONFIRMED)
			.value("ERR_SEND_MAIL_ERROR",ERR_SEND_MAIL_ERROR)
			.value("ERR_STRING_PARAMETER_EXPECTED",ERR_STRING_PARAMETER_EXPECTED)
			.value("ERR_INTEGER_PARAMETER_EXPECTED",ERR_INTEGER_PARAMETER_EXPECTED)
			.value("ERR_DOUBLE_PARAMETER_EXPECTED",ERR_DOUBLE_PARAMETER_EXPECTED)
			.value("ERR_ARRAY_AS_PARAMETER_EXPECTED",ERR_ARRAY_AS_PARAMETER_EXPECTED)
			.value("ERR_HISTORY_WILL_UPDATED",ERR_HISTORY_WILL_UPDATED)
			.value("ERR_TRADE_ERROR",ERR_TRADE_ERROR)
			.value("ERR_END_OF_FILE",ERR_END_OF_FILE)
			.value("ERR_SOME_FILE_ERROR",ERR_SOME_FILE_ERROR)
			.value("ERR_WRONG_FILE_NAME",ERR_WRONG_FILE_NAME)
			.value("ERR_TOO_MANY_OPENED_FILES",ERR_TOO_MANY_OPENED_FILES)
			.value("ERR_CANNOT_OPEN_FILE",ERR_CANNOT_OPEN_FILE)
			.value("ERR_INCOMPATIBLE_FILEACCESS",ERR_INCOMPATIBLE_FILEACCESS)
			.value("ERR_NO_ORDER_SELECTED",ERR_NO_ORDER_SELECTED)
			.value("ERR_UNKNOWN_SYMBOL",ERR_UNKNOWN_SYMBOL)
			.value("ERR_INVALID_PRICE_PARAM",ERR_INVALID_PRICE_PARAM)
			.value("ERR_INVALID_TICKET",ERR_INVALID_TICKET)
			.value("ERR_TRADE_NOT_ALLOWED",ERR_TRADE_NOT_ALLOWED)
			.value("ERR_LONGS_NOT_ALLOWED",ERR_LONGS_NOT_ALLOWED)
			.value("ERR_SHORTS_NOT_ALLOWED",ERR_SHORTS_NOT_ALLOWED)
			.value("ERR_OBJECT_ALREADY_EXISTS",ERR_OBJECT_ALREADY_EXISTS)
			.value("ERR_UNKNOWN_OBJECT_PROPERTY",ERR_UNKNOWN_OBJECT_PROPERTY)
			.value("ERR_OBJECT_DOES_NOT_EXIST",ERR_OBJECT_DOES_NOT_EXIST)
			.value("ERR_UNKNOWN_OBJECT_TYPE",ERR_UNKNOWN_OBJECT_TYPE)
			.value("ERR_NO_OBJECT_NAME",ERR_NO_OBJECT_NAME)
			.value("ERR_OBJECT_COORDINATES_ERROR",ERR_OBJECT_COORDINATES_ERROR)
			.value("ERR_NO_SPECIFIED_SUBWINDOW",ERR_NO_SPECIFIED_SUBWINDOW)
			.value("ERR_SOME_OBJECT_ERROR",ERR_SOME_OBJECT_ERROR)
			.value("ERR_PYMT4_TIMEOUT",ERR_PYMT4_TIMEOUT)
			.value("ERR_PYMT4_NOT_CONNECTED",ERR_PYMT4_NOT_CONNECTED)
			.export_values();

}