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

#include "pymt4_client_command.h"

namespace PyMT4
{

	LastErrorMap _lastErrorMap;
	boost::mutex _lastErrorMapLock;
	boost::mutex _dispatchLock;
	namespace Command
	{
		// New 

		double AccountInfoDouble(const int32_t& property_id)
		{
			return DispatchFunction<double>(PyMT4::CmdId_AccountInfoDouble, property_id);
		}
		int32_t AccountInfoInteger(const int32_t& property_id)
		{
			return DispatchFunction<int32_t>(PyMT4::CmdId_AccountInfoInteger, property_id);
		}
		string AccountInfoString(const int32_t& property_id)
		{
			return DispatchFunction<std::string>(PyMT4::CmdId_AccountInfoString, property_id);
		}




		double AccountBalance()
		{
			return DispatchFunction<double>(PyMT4::CmdId_AccountBalance);
		}

		double AccountCredit()
		{
			return DispatchFunction<double>(PyMT4::CmdId_AccountCredit);
		}

		std::string AccountCompany()
		{
			return DispatchFunction<std::string>(PyMT4::CmdId_AccountCompany);
		}

		std::string AccountCurrency()
		{
			return DispatchFunction<std::string>(PyMT4::CmdId_AccountCurrency);
		}

		double AccountEquity()
		{
			return DispatchFunction<double>(PyMT4::CmdId_AccountEquity);
		}

		double AccountFreeMargin()
		{
			return DispatchFunction<double>(PyMT4::CmdId_AccountFreeMargin);
		}

		double AccountFreeMarginCheck(const std::string& symbol, const int32_t& cmd, const double& volume)
		{
			return DispatchFunction<double>(PyMT4::CmdId_AccountFreeMarginCheck, symbol, cmd, volume);
		}

		double AccountFreeMarginMode()
		{
			return DispatchFunction<double>(PyMT4::CmdId_AccountFreeMarginMode);
		}

		int32_t AccountLeverage()
		{
			return DispatchFunction<int32_t>(PyMT4::CmdId_AccountLeverage);
		}

		double AccountMargin()
		{
			return DispatchFunction<double>(PyMT4::CmdId_AccountMargin);
		}

		std::string AccountName()
		{
			return DispatchFunction<std::string>(PyMT4::CmdId_AccountName);
		}

		int32_t AccountNumber()
		{
			return DispatchFunction<int32_t>(PyMT4::CmdId_AccountNumber);
		}

		double AccountProfit()
		{
			return DispatchFunction<double>(PyMT4::CmdId_AccountProfit);
		}

		std::string AccountServer()
		{
			return DispatchFunction<std::string>(PyMT4::CmdId_AccountServer);
		}

		int32_t AccountStopoutLevel()
		{
			return DispatchFunction<int32_t>(PyMT4::CmdId_AccountStopoutLevel);
		}

		int32_t AccountStopoutMode()
		{
			return DispatchFunction<int32_t>(PyMT4::CmdId_AccountStopoutMode);
		}


		/*Timeseries http://docs.mql4.com/series */


		int32_t iBars(const std::string& symbol, const int32_t& timeframe)
		{
			return DispatchFunction<int32_t>(PyMT4::CmdId_iBars, symbol, timeframe);
		}

		int32_t		iBarShift(const std::string& symbol, const Datetime& datetime, const bool& exact)
		{
			return DispatchFunction<int32_t>(CmdId_iBarShift, symbol, datetime, (BOOL)exact);
		}

		double		iClose(const std::string& symbol, const int32_t& timeframe, const int32_t& shift)
		{
			return DispatchFunction<double>(CmdId_iClose, symbol, timeframe, shift);
		}

		double		iHigh(const std::string& symbol, const int32_t& timeframe, const int32_t& shift)
		{
			return DispatchFunction<double>(CmdId_iHigh, symbol, timeframe, shift);
		}

		double		iHighest(const std::string& symbol, const int32_t& timeframe, const int32_t& type, const int32_t& count, const int32_t& start)
		{
			return DispatchFunction<double>(CmdId_iHighest, symbol, timeframe, type, count, start);
		}

		double		iLow(const std::string& symbol, const int32_t& timeframe, const int32_t& shift)
		{
			return DispatchFunction<double>(CmdId_iLow, symbol, timeframe, shift);
		}

		double		iLowest(const std::string& symbol, const int32_t& timeframe, const int32_t& type, const int32_t& count, const int32_t& start)
		{
			return DispatchFunction<double>(CmdId_iLowest, symbol, timeframe, type, count, start);
		}

		double		iOpen(const std::string& symbol, const int32_t& timeframe, const int32_t& shift)
		{
			return DispatchFunction<double>(CmdId_iOpen, symbol, timeframe, shift);
		}

		Datetime    iTime(const std::string& symbol, const int32_t& timeframe, const int32_t& shift)
		{
			return DispatchFunction<Datetime>(CmdId_iTime, symbol, timeframe, shift);
		}

		double		iVolume(const std::string& symbol, const int32_t& timeframe, const int32_t& shift)
		{
			return DispatchFunction<double>(CmdId_iVolume, symbol, timeframe, shift);
		}


		///* Trading Functions http://docs.mql4.com/trading */

		bool		OrderClose(const int32_t& ticket, const double& lots, const double& price, const int32_t& slippage, const Color& color)
		{
			return DispatchFunction<bool>(CmdId_OrderClose, ticket, lots, price, slippage, color);
		}


		bool		OrderCloseBy(const int32_t& ticket, const double& opposite, const Color& color)
		{
			return DispatchFunction<bool>(CmdId_OrderCloseBy, ticket, opposite, color);
		}

		double		OrderClosePrice(const int32_t& ticket)		/* OrderSelect() */
		{
			return DispatchFunction<double>(CmdId_OrderClosePrice, ticket);
		}

		Datetime	OrderCloseTime(const int32_t& ticket)		/* OrderSelect() */
		{
			return DispatchFunction<Datetime>(CmdId_OrderCloseTime, ticket);
		}

		std::string OrderComment(const int32_t& ticket) 		/* OrderSelect() */
		{
			return DispatchFunction<std::string>(CmdId_OrderComment, ticket);
		}

		double		OrderCommission(const int32_t& ticket)		/* OrderSelect() */
		{
			return DispatchFunction<double>(CmdId_OrderCommission, ticket);
		}

		bool		OrderDelete(const int32_t& ticket, const Color& color)
		{
			return DispatchFunction<bool>(CmdId_OrderDelete, ticket, color);
		}

		Datetime	OrderExpiration(const int32_t& ticket)		/* OrderSelect() */
		{
			return DispatchFunction<Datetime>(CmdId_OrderExpiration, ticket);
		}

		double		OrderLots(const int32_t& ticket)			/* OrderSelect() */
		{
			return DispatchFunction<double>(CmdId_OrderLots, ticket);
		}

		int32_t		OrderMagicNumber(const int32_t& ticket) 	/* OrderSelect() */
		{
			return DispatchFunction<int32_t>(CmdId_OrderMagicNumber, ticket);
		}

		bool		OrderModify(const int32_t& ticket, const double& price, const double& stoploss, const double& takeprofit, const Datetime& expiration, const Color& arrow_color)
		{
			return DispatchFunction<bool>(CmdId_OrderModify, ticket, price, stoploss, takeprofit, expiration, arrow_color);
		}

		double		OrderOpenPrice(const int32_t& ticket)		/* OrderSelect() */
		{
			return DispatchFunction<double>(CmdId_OrderOpenPrice, ticket);
		}

		Datetime	OrderOpenTime(const int32_t& ticket)		/* OrderSelect() */
		{
			return DispatchFunction<Datetime>(CmdId_OrderOpenTime, ticket);
		}

		bool		OrderPrint(const int32_t& ticket)			/* OrderSelect() */
		{
			return DispatchFunction<bool>(CmdId_OrderPrint, ticket);
		}

		double		OrderProfit(const int32_t& ticket)			/* OrderSelect() */
		{
			return DispatchFunction<double>(CmdId_OrderProfit, ticket);
		}

		int32_t		OrderSelect(const int32_t& index, const int32_t& select, const int32_t& pool)
		{
			return DispatchFunction<int32_t>(CmdId_OrderSelect, index, select, pool);
		}

		int32_t		OrderSend(
			const std::string& symbol,
			const int32_t& cmd,
			const double& volume,
			const double& price,
			const int32_t& slippage,
			const double& stoploss,
			const double& takeprofit,
			const std::string& comment,
			const int32_t& magic,
			const Datetime& expiration,
			const Color& arrow_color
			)
		{
			return DispatchFunction<int32_t>
				(CmdId_OrderSend,
					symbol,
					cmd,
					volume,
					price,
					slippage,
					stoploss,
					takeprofit,
					comment,
					magic,
					expiration,
					arrow_color);
		}

		int32_t		OrdersHistoryTotal()
		{
			return DispatchFunction<int32_t>(CmdId_OrdersHistoryTotal);
		}

		double		OrderStopLoss(const int32_t& ticket)	 /* OrderSelect() */
		{
			return DispatchFunction<double>(CmdId_OrderStopLoss, ticket);
		}

		int32_t		OrdersTotal()
		{
			return DispatchFunction<int32_t>(CmdId_OrdersTotal);
		}

		double		OrderSwap(const int32_t& ticket)		 /* OrderSelect() */
		{
			return DispatchFunction<int32_t>(CmdId_OrderSwap, ticket);
		}

		std::string OrderSymbol(const int32_t& ticket)		 /* OrderSelect() */
		{
			return DispatchFunction<std::string>(CmdId_OrderSymbol, ticket);
		}

		double		OrderTakeProfit(const int32_t& ticket)  /* OrderSelect() */
		{
			return DispatchFunction<double>(CmdId_OrderTakeProfit, ticket);
		}

		int32_t		OrderTicket(const int32_t& ticket)		 /* OrderSelect() */
		{
			return DispatchFunction<int32_t>(CmdId_OrderTicket, ticket);
		}

		int32_t		OrderType(const int32_t& ticket)		 /* OrderSelect() */
		{
			return DispatchFunction<int32_t>(CmdId_OrderType, ticket);
		}


		/* Checkup - http://docs.mql4.com/check/ */

		bool IsConnected()
		{
			return DispatchFunction<bool>(CmdId_IsConnected);
		}

		bool IsDemo()
		{
			return DispatchFunction<bool>(CmdId_IsDemo);
		}

		bool IsDllsAllowed()
		{
			return DispatchFunction<bool>(CmdId_IsDllsAllowed);
		}

		bool IsExpertEnabled()
		{
			return DispatchFunction<bool>(CmdId_IsExpertEnabled);
		}

		bool IsLibrariesAllowed()
		{
			return DispatchFunction<bool>(CmdId_IsLibrariesAllowed);
		}

		bool IsOptimization()
		{
			return DispatchFunction<bool>(CmdId_IsOptimization);
		}

		bool IsStopped()
		{
			return DispatchFunction<bool>(CmdId_IsStopped);
		}

		bool IsTesting()
		{
			return DispatchFunction<bool>(CmdId_IsTesting);
		}

		bool IsTradeAllowed()
		{
			return DispatchFunction<bool>(CmdId_IsTradeAllowed);
		}

		bool IsTradeContextBusy()
		{
			return DispatchFunction<bool>(CmdId_IsTradeContextBusy);
		}

		bool IsVisualMode()
		{
			return DispatchFunction<bool>(CmdId_IsVisualMode);
		}

		int32_t  UninitializeReason()
		{
			return DispatchFunction<int32_t>(CmdId_UninitializeReason);
		}

		/* DateTime functions */
		Datetime TimeCurrent()
		{
			return DispatchFunction<Datetime>(CmdId_TimeCurrent);
		}

		Datetime TimeLocal()
		{
			return DispatchFunction<Datetime>(CmdId_TimeLocal);
		}

		Datetime TimeGMT()
		{
			return DispatchFunction<Datetime>(CmdId_TimeGMT);
		}

		int32_t TimeGMTOffset()
		{
			return DispatchFunction<int32_t>(CmdId_TimeGMTOffset);
		}


		/* Client Terminal http://docs.mql4.com/terminal */

		std::string TerminalCompany()
		{
			return DispatchFunction<std::string>(CmdId_TerminalCompany);
		};

		std::string TerminalName()
		{
			return DispatchFunction<std::string>(CmdId_TerminalName);
		};

		std::string TerminalPath()
		{
			return DispatchFunction<std::string>(CmdId_TerminalPath);
		};

		/* Common Functions */
		bool RefreshRates()
		{
			return DispatchFunction<bool>(CmdId_RefreshRates);
		}

		bool Alert(const std::string& alert)
		{
			return DispatchFunction<bool>(CmdId_Alert, alert);
		}

		bool Print(const std::string& print)
		{
			return DispatchFunction<bool>(CmdId_Print, print);
		}

		bool Comment(const std::string& comment)
		{
			return DispatchFunction<bool>(CmdId_Comment, comment);
		}

		int32_t GetTickCount()
		{
			return DispatchFunction<int32_t>(CmdId_GetTickCount);
		}

		double MarketInfo(const std::string& symbol, const int& type)
		{
			return DispatchFunction<double>(CmdId_MarketInfo, symbol, type);
		}

		bool PlaySound(const std::string& filename)
		{
			return DispatchFunction<bool>(CmdId_PlaySound, filename);
		}

		bool SendFTP(const std::string& filename, const std::string& ftp_path)
		{
			return DispatchFunction<bool>(CmdId_SendFTP, filename, ftp_path);
		}

		bool SendMail(const std::string& subject, const std::string& text)
		{
			return DispatchFunction<bool>(CmdId_SendMail, subject, text);
		}

		bool SendNotification(const std::string& message)
		{
			return DispatchFunction<bool>(CmdId_SendNotification, message);
		}

		PyMT4::Error GetLastError()
		{
			boost::mutex::scoped_lock errorMapLock(_lastErrorMapLock);
			LastErrorMap::const_iterator errorMapItem = _lastErrorMap.find(boost::this_thread::get_id());

			if (errorMapItem == _lastErrorMap.end())
				return ERR_NO_ERROR;

			return (PyMT4::Error)errorMapItem->second;
		}
		/*
		CmdId_ObjectName
		CmdId_ObjectCreate

		*/

		std::string		ObjectName(const int32_t& object_index)
		{
			return DispatchFunction<std::string>(CmdId_ObjectName, object_index);
		}

		bool ObjectCreate(const std::string& object_name, const int32_t& object_type, const int32_t& subwindow, const std::vector<std::pair<Datetime, double> >& prices)
		{
			return DispatchFunction<bool>(CmdId_ObjectDelete, object_name);
		}
		/*
		bool ObjectDelete(const int64_t& chart_id, const std::string& object_name)
		{
			return DispatchFunction<bool>(CmdId_ObjectDeleteOnChart, chart_id, object_name);
		}

		int32_t ObjectsDeleteAll(const int32_t& sub_window, const int32_t& object_type)
		{

			return DispatchFunction<int32_t>(CmdId_ObjectDeleteAll, sub_window, object_type);
		}

		int32_t	ObjectsDeleteAll(const int64_t& chart_id, const int32_t& sub_window, const int32_t& object_type)
		{
			return DispatchFunction<int32_t>(CmdId_ObjectDeleteAllOnChart, sub_window, object_type);
		}

		int32_t	ObjectsDeleteAll(const int64_t& chart_id, const std::string& prefix, const int32_t& sub_window, const int32_t& object_type)
		{
			return DispatchFunction<int32_t>(CmdId_ObjectDeleteAllOnChartWithPrefix, chart_id, prefix, sub_window, object_type);
		}

		int32_t	ObjectFind(const std::string& object_name)
		{
			return DispatchFunction<int32_t>(CmdId_ObjectFind, object_name);
		}

		double ObjectGetValueByTime(const long& chart_id, const string& object_name, const Datetime& time, const int& line_id)
		{
			return DispatchFunction<double>(CmdId_ObjectGetValueByTime, chart_id, object_name, time, line_id);
		}

		bool ObjectMove(const string& object_name, const int32_t& point_index, const Datetime& time, const double& price)
		{
			return DispatchFunction<bool>(CmdId_ObjectMove, object_name, point_index, time, price);
		}

		bool ObjectMoveOnChart(const uint64_t& chart_id, const string& object_name, const int32_t& point_index, const Datetime& time, const double& price)
		{
			return DispatchFunction<bool>(CmdId_ObjectMoveOnChart, chart_id, object_name, point_index, time, price);
		}

		//int ObjectsTotal(const long& chart_id, const int& sub_window = -1, const int& type = -1, const int& type = EMPTY)
		//{
		//	return DispatchFunction<int>(CmdId_ObjectsTotal, type = EMPTY);
		//}



		double ObjectGetDouble(const long& chart_id, const string& object_name, const int& prop_id, const int& prop_modifier = 0, const bool& chart_id, const long& object_name, const string& prop_id, const int& prop_modifier, const int& double_var)
		{
			return DispatchFunction<double>(CmdId_ObjectGetDouble, double_var);
		}
		long ObjectGetInteger(const long& chart_id, const string& object_name, const int& prop_id, const int& prop_modifier = 0, const bool& chart_id, const long& object_name, const string& prop_id, const int& prop_modifier, const int& long_var)
		{
			return DispatchFunction<long>(CmdId_ObjectGetInteger, long_var);
		}
		string ObjectGetString(const long& chart_id, const string& object_name, const int& prop_id, const int& prop_modifier = 0, const bool& chart_id, const long& object_name, const string& prop_id, const int& prop_modifier, const int& string_var)
		{
			return DispatchFunction<string>(CmdId_ObjectGetString, string_var);
		}
		bool ObjectSetDouble(const long& chart_id, const string& object_name, const int& prop_id, const double& prop_value, const bool& chart_id, const long& object_name, const string& prop_id, const int& prop_modifier, const int& prop_value)
		{
			return DispatchFunction<bool>(CmdId_ObjectSetDouble, prop_value);
		}
		bool ObjectSetInteger(const long& chart_id, const string& object_name, const int& prop_id, const long& prop_value, const bool& chart_id, const long& object_name, const string& prop_id, const int& prop_modifier, const int& prop_value)
		{
			return DispatchFunction<bool>(CmdId_ObjectSetInteger, prop_value);
		}
		bool ObjectSetString(const long& chart_id, const string& object_name, const int& prop_id, const string& prop_value, const bool& chart_id, const long& object_name, const string& prop_id, const int& prop_modifier, const int& prop_value)
		{
			return DispatchFunction<bool>(CmdId_ObjectSetString, prop_value);
		}
		string ObjectDescription(const string& object_name)
		{
			return DispatchFunction<string>(CmdId_ObjectDescription, object_name);
		}
		double ObjectGet(const string& object_name, const int& index)
		{
			return DispatchFunction<double>(CmdId_ObjectGet, index);
		}
		string ObjectGetFiboDescription(const string& object_name, const int& index)
		{
			return DispatchFunction<string>(CmdId_ObjectGetFiboDescription, index);
		}
		int ObjectGetShiftByValue(const string& object_name, const double& value)
		{
			return DispatchFunction<int>(PyMT4::CmdId_ObjectGetShiftByValue, value);
		}
		double ObjectGetValueByShift(const string& object_name, const int& shift)
		{
			return DispatchFunction<double>(PyMT4::CmdId_ObjectGetValueByShift, shift);
		}
		bool ObjectSet(const string& object_name, const int& index, const double& value)
		{
			return DispatchFunction<bool>(PyMT4::CmdId_ObjectSet, value);
		}
		bool ObjectSetFiboDescription(const string& object_name, const int& index, const string& text)
		{
			return DispatchFunction<bool>(PyMT4::CmdId_ObjectSetFiboDescription, text);
		}

		int ObjectType(const string& object_name)
		{
			return DispatchFunction<int>(PyMT4::CmdId_ObjectType, object_name);
		}

		*/

		/*	bool			ObjectCreateOnChart(const int64_t chart_id, const std::string& object_name, const int32_t& object_type, const int32_t& subwindow, std::vector<std::pair<Datetime, double> > prices);
		bool			ObjectDelete(const std::string& object_name);
		bool			ObjectDeleteOnChart(const int64_t& chart_id, const std::string& object_name);
		int32_t			ObjectsDeleteAll(const int32_t& sub_window = EMPTY, const int32_t& object_type = EMPTY);
		int32_t			ObjectsDeleteAllOnChart(const int64_t& chart_id, const int32_t& sub_window = EMPTY, const int32_t& object_type = EMPTY);
		int32_t			ObjectsDeleteAllOnChartWithPrefix(const int64_t& chart_id, const std::string prefix, const int32_t& sub_window = EMPTY, const int32_t& object_type = EMPTY);

		int32_t			ObjectFind(const std::string& object_name);
		int32_t			ObjectFindOnChart(const int64_t& chart_id, const std::string& object_name);
		Datetime		ObjectGetTimeByValue(const std::string& object_name, const double& value);
		double			ObjectGetValueByTimeOnChart(const int64_t& chart_id, const std::string& object_name, const Datetime& time, const int32_t& line_id = 0);
		bool			ObjectMoveOnChart(const int32_t& chart_id, const std::string& object_name, const int32_t& point32_t_index, const Datetime& time, const double& price);
		bool			ObjectMove(const std::string& object_name, const int32_t& point32_t_index, const Datetime& time, const double& price);
		int32_t			ObjectsTotalOnChart(const int64_t& chart_id, const int32_t& sub_window = -1, const int32_t& type = -1);
		int32_t			ObjectsTotal(const int32_t& type = -1);
		double			ObjectGetDoubleOnChart(const int64_t& chart_id, const std::string& object_name, const int32_t& prop_id, const int32_t& prop_modifier = 0);
		int64_t			ObjectGetIntegerOnChart(const int64_t& chart_id, const std::string& object_name, const int32_t& prop_id, const int32_t& prop_modifier = 0);
		std::string		ObjectGetStringOnChart(const int64_t& chart_id, const std::string& object_name, const int32_t& prop_id, const int32_t& prop_modifier = 0);
		bool			ObjectSetDoubleOnChart(const int64_t& chart_id, const std::string& object_name, const int32_t& prop_id, const double& prop_value);
		bool			ObjectSetIntegerOnChart(const int64_t& chart_id, const std::string& object_name, const int32_t& prop_id, const int64_t& prop_value);
		bool			ObjectSetStringOnChart(const int64_t& chart_id, const std::string& object_name, const int32_t& prop_id, const std::string& prop_value);
		bool			ObjectSetString(const int64_t& chart_id, const std::string& object_name, const int64_t& prop_id, std::string& prop_value);
		bool			ObjectSetStringWithModifier(const int64_t& chart_id, const std::string& object_name, const int64_t& prop_id, const int64_t prop_modifier, const std::string& prop_value);
		*/



		bool WindowRedraw()
		{
			return DispatchFunction<bool>(CmdId_WindowRedraw);
		}
	}
}