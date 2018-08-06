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
#include "pymt4_client_dispatcher.h"




#define DECLFUNCTION(NAME,RETURN, ... ) \
namespace PyMT4 { \
	RETURN NAME( __VA_ARGS__ ) { \
		\
	} \
}


namespace PyMT4
{


	namespace Command
	{

		/* Account information http://docs.mql4.com/account */

		// New //
		double AccountInfoDouble(const int32_t& property_id);
		int32_t AccountInfoInteger(const int32_t& property_id);
		std::string AccountInfoString(const int32_t& property_id);

		double		AccountBalance();
		double		AccountCredit();
		std::string AccountCompany();
		std::string AccountCurrency();
		double		AccountEquity();
		double		AccountFreeMargin();
		double		AccountFreeMarginCheck(const std::string& symbol,const int32_t& cmd,const double& volume);
		double		AccountFreeMarginMode();
		int32_t		AccountLeverage();
		double		AccountMargin();
		std::string AccountName();
		int32_t		AccountNumber();
		double		AccountProfit();
		std::string AccountServer();
		int32_t		AccountStopoutLevel();
		int32_t		AccountStopoutMode();

		/* Timeseries Access http://docs.mql4.com/series */

		int32_t		iBars(const std::string& symbol,const int32_t& timeframe);
		int32_t		iBarShift(const std::string& symbol,const Datetime& datetime,const bool& exact=false);
		double		iClose(const std::string& symbol,const int32_t& timeframe, const int32_t& shift);
		double		iHigh(const std::string& symbol, const int32_t& timeframe, const int32_t& shift);
		double		iHighest(const std::string& symbol, const int32_t& timeframe,const int32_t& type,const int32_t& count=0,const int32_t& start=0);
		double		iLow(const std::string& symbol, const int32_t& timeframe,const int32_t& shift);
		double		iLowest(const std::string& symbol, const int32_t& timeframe,const int32_t& type,const int32_t& count=0,const int32_t& start=0);
		double		iOpen(const std::string& symbol, const int32_t& timeframe,const int32_t& shift);
		Datetime    iTime(const std::string& symbol, const int32_t& timeframe, const int32_t& shift);
		double		iVolume(const std::string& symbol, const int32_t& timeframe, const int32_t& shift);

		/*  Trading functions http://docs.mql4.com/trading */

		bool		OrderClose(const int32_t& ticket, const double& lots,const double& price, const int32_t& slippage, const Color& Color=CLR_NONE);
		bool		OrderCloseBy(const int32_t& ticket, const double& opposite,const Color& Color=CLR_NONE);
		double		OrderClosePrice(const int32_t& ticket);		/* OrderSelect() */
		Datetime	OrderCloseTime(const int32_t& ticket);		/* OrderSelect() */
		std::string OrderComment(const int32_t& ticket);		/* OrderSelect() */
		double		OrderCommission(const int32_t& ticket);		/* OrderSelect() */
		bool		OrderDelete(const int32_t& ticket,const Color& Color=CLR_NONE); 
		Datetime	OrderExpiration(const int32_t& ticket);		/* OrderSelect() */
		double		OrderLots(const int32_t& ticket);			/* OrderSelect() */
		int32_t		OrderMagicNumber(const int32_t& ticket);	/* OrderSelect() */
		bool		OrderModify(const int32_t& ticket,const double& price, const double& stoploss, const double& takeprofit, const Datetime& expiration, const Color& arrow_color=CLR_NONE);
		double		OrderOpenPrice(const int32_t& ticket);		/* OrderSelect() */
		Datetime	OrderOpenTime(const int32_t& ticket);		/* OrderSelect() */
		bool		OrderPrint(const int32_t& ticket);			/* OrderSelect() */
		double		OrderProfit(const int32_t& ticket);			/* OrderSelect() */
		int32_t		OrderSelect(const int32_t& index, const int32_t& select, const int32_t& pool=MODE_TRADES);
		int32_t		OrderSend(
								const std::string& symbol, 
								const int32_t& cmd, const double& volume, 
								const double& price, 
								const int32_t& slippage, 
								const double& stoploss, 
								const double& takeprofit, 
								const std::string& comment="",
								const int32_t& magic=0,
								const Datetime& expiration=DATETIME_DEFAULT,
								const Color& arrow_color=CLR_NONE
							);

		int32_t		OrdersHistoryTotal();
		double		OrderStopLoss(const int32_t& ticket);	 /* OrderSelect() */
		int32_t		OrdersTotal();
		double		OrderSwap(const int32_t& ticket);		 /* OrderSelect() */
		std::string OrderSymbol(const int32_t& ticket);		 /* OrderSelect() */
		double		OrderTakeProfit(const int32_t& ticket);  /* OrderSelect() */
		int32_t		OrderTicket(const int32_t& ticket);		 /* OrderSelect() */
		int32_t		OrderType(const int32_t& ticket);		 /* OrderSelect() */

		/* Checkup - http://docs.mql4.com/check/ */

		//int32_t GetLastError();
		bool IsConnected();
		bool IsDemo();
		bool IsDllsAllowed();
		bool IsExpertEnabled();
		bool IsLibrariesAllowed();
		bool IsOptimization();
		bool IsStopped();
		bool IsTesting();
		bool IsTradeAllowed();
		bool IsTradeContextBusy();
		bool IsVisualMode();
		int  UninitializeReason();

		/* Date Time functions */
		Datetime TimeCurrent();
		Datetime TimeLocal();
		Datetime TimeGMT();
		int32_t TimeGMTOffset();

		/* Client Terminal http://docs.mql4.com/terminal */

		std::string TerminalCompany();
		std::string TerminalName();
		std::string TerminalPath();

		/* Common Functions  http://docs.mql4.com/common */

		bool Alert(const std::string&);
		bool Comment(const std::string&);
		bool Print(const std::string&);
		bool RefreshRates();


		int32_t GetTickCount();
		double MarketInfo(const std::string& symbol,const int& type);
		bool PlaySound(const std::string& filename);
		bool SendFTP(const std::string& filename,const std::string& ftp_path);
		bool SendMail(const std::string& subject,const std::string& text);
		bool SendNotification(const std::string& message); 

		PyMT4::Error GetLastError();


		/* Object Functions */

		std::string		ObjectName(const int32_t& object_index);
		bool			ObjectCreate(const std::string& object_name, const int32_t& object_type, const int32_t& subwindow, const std::vector<std::pair<Datetime, double> >& prices);
		bool			ObjectCreateOnChart(const int64_t& chart_id, const std::string& object_name, const int32_t& object_type, const int32_t& subwindow, const std::vector<std::pair<Datetime, double> >& prices);
		bool			ObjectDelete(const std::string& object_name);
		bool			ObjectDeleteOnChart(const int64_t& chart_id, const std::string& object_name);
		int32_t			ObjectsDeleteAll(const int32_t& sub_window = EMPTY, const int32_t& object_type = EMPTY);
		int32_t			ObjectsDeleteAllOnChart(const int64_t& chart_id, const int32_t& sub_window = EMPTY, const int32_t& object_type = EMPTY);
		int32_t			ObjectsDeleteAllOnChartWithPrefix(const int64_t& chart_id, const std::string& prefix, const int32_t& sub_window = EMPTY, const int32_t& object_type = EMPTY);

		int32_t			ObjectFindOnChart(const int64_t& chart_id,const std::string& object_name);
		Datetime		ObjectGetTimeByValue(const std::string& object_name, const double& value);
		double			ObjectGetValueByTimeOnChart(const int64_t& chart_id, const std::string& object_name, const Datetime& time, const int32_t& line_id = 0);
		bool			ObjectMoveOnChart(const int32_t& chart_id,const std::string& object_name, const int32_t& point32_t_index, const Datetime& time, const double& price);
		bool			ObjectMove(const std::string& object_name, const int32_t& point32_t_index, const Datetime& time, const double& price);
		bool			ObjectMoveOnChart(const int32_t& chart_id, const std::string& object_name, const int32_t& point32_t_index, const Datetime& time, const double& price);
		int32_t			ObjectsTotalOnChart(const int64_t& chart_id, const int32_t& sub_window = -1, const int32_t& type = EMPTY);
		int32_t			ObjectsTotal(const int32_t& type = EMPTY);
		double			ObjectGetDoubleOnChart(const int64_t& chart_id, const std::string& object_name, const int32_t& prop_id, const int32_t& prop_modifier = 0);
		int64_t			ObjectGetIntegerOnChart(const int64_t& chart_id, const std::string& object_name, const int32_t& prop_id, const int32_t& prop_modifier = 0);
		std::string		ObjectGetStringOnChart(const int64_t& chart_id, const std::string& object_name, const int32_t& prop_id, const int32_t& prop_modifier = 0);
		bool			ObjectSetDoubleOnChart(const int64_t& chart_id, const std::string& object_name, const int32_t& prop_id, const double& prop_value);
		bool			ObjectSetIntegerOnChart(const int64_t& chart_id, const std::string& object_name, const int32_t& prop_id, const int64_t& prop_value);
		bool			ObjectSetStringOnChart(const int64_t& chart_id, const std::string& object_name, const int32_t& prop_id, const std::string& prop_value);
		bool			ObjectSetString(const int64_t& chart_id, const std::string& object_name, const int64_t& prop_id, std::string& prop_value);
		bool			ObjectSetStringWithModifier(const int64_t& chart_id, const std::string& object_name, const int64_t& prop_id, const int64_t prop_modifier,const std::string& prop_value);
		std::string		ObjectDescription(const std::string& object_name);
		double			ObjectGet(const std::string& object_name, const int32_t& index);
		std::string		ObjectGetFiboDescription(const std::string& object_name, const int32_t& index);
		int32_t			ObjectGetShiftByValue(const std::string& object_name, const double& value);
		double			ObjectGetValueByShift(const std::string& object_name, const int32_t& shift);
		bool			ObjectSet(const std::string& object_name, const int32_t& index, const double& value);
		bool			ObjectSetFiboDescription(const std::string& object_name, const int32_t& index, const std::string& text);
		int32_t			ObjectType(const std::string& object_name);

		


		/* Chart Functions https://docs.mql4.com/chart_operations/ */

		bool			WindowRedraw();

	}
}