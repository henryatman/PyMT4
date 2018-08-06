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

#import "pymt4.dll"
#include <stderror.mqh>
extern const short LISTEN_PORT=5621;

bool pymt4_isdebug();
bool pymt4_initialize(string windowName,int windowHandle,short port);
bool pymt4_uninitialize(string windowName,int windowHandle);
bool pymt4_isinitialized();
bool pymt4_notify(string windowName,int windowHandle);
bool pymt4_notifyOnTick(string symbol,double bid,double ask);
int  pymt4_requestPendingCommand();

string pymt4_getStringArgument(string);
double pymt4_getDoubleArgument();
int    pymt4_getIntArgument();
bool   pymt4_getBoolArgument();

bool pymt4_setDoubleResult(double result,int error);
bool pymt4_setStringResult(string result,int error);
bool pymt4_setIntResult(int result,int error);
bool pymt4_setBoolResult(int result,int error );
//+------------------------------------------------------------------+
//| script program start function                                    |
//+------------------------------------------------------------------+
#define StrBuffer "                                                                                                                               "
#define CmdId_CheckShutdownCondition 100



#define CmdId_AccountBalance 1000
#define CmdId_AccountCredit 1001
#define CmdId_AccountCompany 1002
#define CmdId_AccountCurrency 1003
#define CmdId_AccountEquity 1004
#define CmdId_AccountFreeMargin 1005
#define CmdId_AccountFreeMarginCheck 1006
#define CmdId_AccountFreeMarginMode 1007
#define CmdId_AccountLeverage 1008
#define CmdId_AccountMargin 1009
#define CmdId_AccountName 1010
#define CmdId_AccountNumber 1011
#define CmdId_AccountProfit 1012
#define CmdId_AccountServer 1013
#define CmdId_AccountStopoutLevel 1014
#define CmdId_AccountStopoutMode 1015
#define CmdId_AccountInfoDouble 1016
#define CmdId_AccountInfoInteger 1017
#define CmdId_AccountInfoString  1018


#define CmdId_iBars 2000
#define CmdId_iBarShift 2001
#define CmdId_iClose 2002
#define CmdId_iHigh 2003
#define CmdId_iHighest 2004
#define CmdId_iLow 2005
#define CmdId_iLowest 2006
#define CmdId_iOpen 2007
#define CmdId_iTime 2008
#define CmdId_iVolume 2009

#define CmdId_OrderClose 3000
#define CmdId_OrderCloseBy 3001
#define CmdId_OrderClosePrice 3002
#define CmdId_OrderCloseTime 3003
#define CmdId_OrderComment 3004
#define CmdId_OrderCommission 3005
#define CmdId_OrderDelete 3006
#define CmdId_OrderExpiration 3007
#define CmdId_OrderLots 3008
#define CmdId_OrderMagicNumber 3009
#define CmdId_OrderModify 3010
#define CmdId_OrderOpenPrice 3011
#define CmdId_OrderOpenTime 3012
#define CmdId_OrderPrint 3013
#define CmdId_OrderProfit 3014
#define CmdId_OrderSelect 3015
#define CmdId_OrderSend 3016
#define CmdId_OrdersHistoryTotal 3017
#define CmdId_OrderStopLoss 3018
#define CmdId_OrdersTotal 3019
#define CmdId_OrderSwap 3020
#define CmdId_OrderSymbol 3021
#define CmdId_OrderTakeProfit 3022
#define CmdId_OrderTicket 3023
#define CmdId_OrderType 3024


#define CmdId_IsConnected 4000
#define CmdId_IsDemo 4001
#define CmdId_IsDllsAllowed 4002
#define CmdId_IsExpertEnabled 4003
#define CmdId_IsLibrariesAllowed 4004
#define CmdId_IsOptimization 4005
#define CmdId_IsStopped 4006
#define CmdId_IsTesting 4007
#define CmdId_IsTradeAllowed 4008
#define CmdId_IsTradeContextBusy 4009
#define CmdId_IsVisualMode 4010
#define CmdId_UninitializeReason 4011

#define CmdId_TerminalCompany 5000
#define CmdId_TerminalName 5001
#define CmdId_TerminalPath 5002

#define CmdId_Alert 6000
#define CmdId_Comment 6001
#define CmdId_GetTickCount 6002
#define CmdId_MarketInfo 6003
#define CmdId_PlaySound 6004
#define CmdId_Print 6005
#define CmdId_SendFTP 6006
#define CmdId_SendMail 6007
#define CmdId_SendNotification 6008

#define CmdId_TimeCurrent 7000
#define CmdId_TimeLocal 7001
#define CmdId_TimeGMT 7002
#define CmdId_TimeGMTOffset 7003

#define CmdId_RefreshRates 9000	

#define CmdId_ObjectName   10000
#define CmdId_ObjectCreate 10001

#define CmdId_WindowRedraw 11000

	

void checkSelectedTicket(int ticketId)
{
   static int lastTicketId = 0;
   
   if (ticketId != lastTicketId)
      bool res = OrderSelect(ticketId,SELECT_BY_TICKET);
      
   lastTicketId = ticketId;
}

bool evaluateCommand(int pendingCommandId)
{

   bool   bres = false;
   double dres = 0;
   string sres = "";
   int    ires = 0;
   int    err  = 0;
   
   string sarga,sargb,sargc,sargd,sarge;
   int    iarga,iargb,iargc,iargd,iarge;
   double darga,dargb,dargc,dargd,darge;
   bool   barga,bargb,bargc,bargd,barge;
   
   switch(pendingCommandId)
   {
   
	   /* Account Functions */   
	   
	   
      case CmdId_AccountInfoDouble:
      	dres = AccountInfoDouble(pymt4_getIntArgument());
      	err  = GetLastError();
      	pymt4_setDoubleResult(dres,err);
      	break;
      case CmdId_AccountInfoInteger:
      	ires = AccountInfoInteger(pymt4_getIntArgument());
      	err  = GetLastError();
      	pymt4_setIntResult(ires,err);
      	break;
      case CmdId_AccountInfoString:
      	sres = AccountInfoString(pymt4_getIntArgument());
      	err  = GetLastError();
      	pymt4_setStringResult(sres,err);
      	break;
      case CmdId_AccountBalance: 
         dres = AccountBalance();
         err = GetLastError();
         pymt4_setDoubleResult(dres,err);
         break;
      case CmdId_AccountCredit:
         dres = AccountCredit();
         err = GetLastError();
         pymt4_setDoubleResult(dres,err);
	      break;
      case CmdId_AccountCompany:
         sres = AccountCompany();
         err = GetLastError();
         pymt4_setStringResult(sres,err);      
	      break;
      case CmdId_AccountCurrency:
         sres = AccountCurrency();
         err  = GetLastError();
         pymt4_setStringResult(sres,err);
	      break;
      case CmdId_AccountEquity:
         dres = AccountEquity();
         err  = GetLastError();
         pymt4_setDoubleResult(dres,err);
	      break;
      case CmdId_AccountFreeMargin:
         dres = AccountFreeMargin();
         err  = GetLastError();
         pymt4_setDoubleResult(dres,err);
	      break;
      case CmdId_AccountFreeMarginCheck:
         sarga = pymt4_getStringArgument(StrBuffer);
         iargb = pymt4_getIntArgument();
         dargc = pymt4_getDoubleArgument();
         dres = AccountFreeMarginCheck(sarga,iargb,dargc);
         err  = GetLastError();
         pymt4_setDoubleResult(dres,err);
         break;
      case CmdId_AccountFreeMarginMode:
         dres = AccountFreeMarginMode();
         err  = GetLastError();
         pymt4_setDoubleResult(dres, err);
	      break;
      case CmdId_AccountLeverage:
         ires = AccountLeverage();
         err  = GetLastError();
         pymt4_setIntResult(ires,err);
	      break;
      case CmdId_AccountMargin:
         dres = AccountMargin();
         err  = GetLastError();
         pymt4_setDoubleResult(dres,err);
	      break;
      case CmdId_AccountName:
         sres = AccountName();
         
         pymt4_setStringResult(sres,err);
	      break;
      case CmdId_AccountNumber:
         ires = AccountNumber();
         err  = GetLastError();
         pymt4_setIntResult(ires,err);
	      break;
      case CmdId_AccountProfit:
         dres = AccountProfit();
         err  = GetLastError();
         pymt4_setDoubleResult(dres,err);
	      break;
      case CmdId_AccountServer:
         sres = AccountServer();
         pymt4_setStringResult(sres,err);
	      break;
      case CmdId_AccountStopoutLevel:
         ires = AccountStopoutLevel();
         err  = GetLastError();
         pymt4_setIntResult(ires,err);
	      break;
      case CmdId_AccountStopoutMode:
         ires = AccountStopoutMode();
         err  = GetLastError();
         pymt4_setIntResult(ires,err);
	      break;
	      
	   /* Trading Functions */
	      
      case CmdId_iBars:
         sarga = pymt4_getStringArgument(StrBuffer);
         iargb     = pymt4_getIntArgument();  
         ires  = iBars(sarga,iargb);
         err   = GetLastError();
         pymt4_setIntResult(ires,err);
	      break;
      case CmdId_iBarShift:
         sarga = pymt4_getStringArgument(StrBuffer);
         iargb = pymt4_getIntArgument();
         bargc = pymt4_getBoolArgument();
         ires = iBarShift(sarga,iargb,bargc);
         err  = GetLastError();
         pymt4_setIntResult(ires,err);
	      break;	      
      case CmdId_iClose:
         sarga = pymt4_getStringArgument(StrBuffer);
         iargb = pymt4_getIntArgument();
         iargc = pymt4_getIntArgument();
         dres = iClose(sarga,iargb,iargc); 
         err = GetLastError();
         pymt4_setDoubleResult(dres,err);
	      break;	      
	   case CmdId_iHigh:
         sarga = pymt4_getStringArgument(StrBuffer);
         iargb = pymt4_getIntArgument();
         iargc = pymt4_getIntArgument();
         dres  = iHigh(sarga,iargb,iargc);
         err   = GetLastError();
         pymt4_setDoubleResult(dres,err);
	      break;
      case CmdId_iHighest:
         sarga = pymt4_getStringArgument(StrBuffer);
         iargb = pymt4_getIntArgument();
         iargc = pymt4_getIntArgument();
         iargd = pymt4_getIntArgument();
         iarge = pymt4_getIntArgument();
         dres = iHighest(sarga,iargb,iargc,iargd,iarge);
         err  = GetLastError();
         pymt4_setDoubleResult(dres,err);
         break;
      case CmdId_iLowest:
         sarga = pymt4_getStringArgument(StrBuffer);
         iargb = pymt4_getIntArgument();
         iargc = pymt4_getIntArgument();
         dres  = iLowest(sarga,iargb,iargc);
         err   = GetLastError();
	      pymt4_setDoubleResult(dres,err);
      case CmdId_iOpen:
         sarga = pymt4_getStringArgument(StrBuffer);
         iargb = pymt4_getIntArgument();
         iargc = pymt4_getIntArgument();
         dres  = iOpen(sarga,iargb,iargc);
         err   = GetLastError();
         pymt4_setDoubleResult(dres,err);
	      break;
      case CmdId_iTime:
         sarga = pymt4_getStringArgument(StrBuffer);
         iarga = pymt4_getIntArgument();
         iargb = pymt4_getIntArgument();
         dres  = iTime(sarga,iargb,iargc);
         err   = GetLastError();      
         pymt4_setDoubleResult(dres,err);
	      break;
      case CmdId_iVolume:
         sarga = pymt4_getStringArgument(StrBuffer);
         iargb = pymt4_getIntArgument();
         iargc = pymt4_getIntArgument();
         dres = iVolume(sarga,iargb,iargc);
         err  = GetLastError();
         pymt4_setDoubleResult(dres,err);
         break;    	      	      	      	      	      	      
    
    
    /* Order Functions */
    
      case CmdId_OrderClose:
         iarga = pymt4_getIntArgument();
         dargb = pymt4_getDoubleArgument();
         dargc = pymt4_getDoubleArgument();
         iargd = pymt4_getIntArgument();
         iarge = pymt4_getIntArgument();
         bres = OrderClose(iarga,dargb,dargc,iargd,iarge);
         err  = GetLastError();
         pymt4_setBoolResult(bres,err);
	      break;
      case CmdId_OrderCloseBy:
         iarga = pymt4_getIntArgument();
         dargb = pymt4_getDoubleArgument();
         iargc = pymt4_getIntArgument();
         bres  = OrderCloseBy(iarga,dargb,iargc);
         err   = GetLastError();
         pymt4_setBoolResult(bres,err);
	      break;
      case CmdId_OrderClosePrice:
         checkSelectedTicket(pymt4_getIntArgument());
         dres = OrderClosePrice();
	      pymt4_setDoubleResult(dres,GetLastError());
	      break;
      case CmdId_OrderCloseTime:
         checkSelectedTicket(pymt4_getIntArgument());
         ires = OrderCloseTime();
	      pymt4_setIntResult(ires,GetLastError());
	      break;
      case CmdId_OrderComment:
         checkSelectedTicket(pymt4_getIntArgument());
         sres = OrderComment();
	      pymt4_setStringResult(sres,GetLastError());
	      break;
      case CmdId_OrderCommission:
         checkSelectedTicket(pymt4_getIntArgument());
         dres = OrderCommission();
	      pymt4_setDoubleResult(dres,GetLastError());
	      break;
      case CmdId_OrderDelete:
         iarga = pymt4_getIntArgument();
         iargb = pymt4_getIntArgument();
         bres  = OrderDelete(iarga,iargb);
	      pymt4_setBoolResult(bres,GetLastError());
	      break;
      case CmdId_OrderExpiration:
	      checkSelectedTicket(pymt4_getIntArgument()); 
	      ires = OrderExpiration();
	      pymt4_setIntResult(ires,GetLastError());
	      break;
      case CmdId_OrderLots:
	      checkSelectedTicket(pymt4_getIntArgument()); 
	      dres = OrderLots();
	      pymt4_setDoubleResult(dres,GetLastError());
	      break;
      case CmdId_OrderMagicNumber:
	      checkSelectedTicket(pymt4_getIntArgument()); 
	      ires = OrderMagicNumber();
	      pymt4_setIntResult(ires,GetLastError());
	      break;
      case CmdId_OrderModify:
         iarga = pymt4_getIntArgument();
         dargb = pymt4_getDoubleArgument();
         dargc = pymt4_getDoubleArgument();
         dargd = pymt4_getDoubleArgument();
         iargb = pymt4_getIntArgument();
         iargc = pymt4_getIntArgument();
         bres = OrderModify(iarga,dargb,dargc,dargd,iargb,iargc);
         err = GetLastError();
	      pymt4_setBoolResult(bres,err);
	      break;
      case CmdId_OrderOpenPrice:
	      checkSelectedTicket(pymt4_getIntArgument()); 
	      dres = OrderOpenPrice();
	      pymt4_setDoubleResult(dres,GetLastError());
	      break;
      case CmdId_OrderOpenTime:
	      checkSelectedTicket(pymt4_getIntArgument()); 
	      ires = OrderOpenTime();
	      pymt4_setIntResult(ires,GetLastError());
	      break;
      case CmdId_OrderPrint:
	      checkSelectedTicket(pymt4_getIntArgument()); 
	      OrderPrint();
	      pymt4_setBoolResult(True,GetLastError());
	      break;
      case CmdId_OrderProfit:
	      checkSelectedTicket(pymt4_getIntArgument()); 
	      dres = OrderProfit();
	      pymt4_setDoubleResult(dres,GetLastError());
	      break;
      case CmdId_OrderSelect:
         bres = OrderSelect(pymt4_getIntArgument(),pymt4_getIntArgument(),pymt4_getIntArgument());
         
         if (bres) 
         {  
            ires = OrderTicket();
            pymt4_setIntResult(ires,GetLastError());
            
         }
         else 
         {  
            pymt4_setIntResult(-1,GetLastError());  
         }

	      break;
      case CmdId_OrderSend:
         sarga = pymt4_getStringArgument(StrBuffer);
         iarga = pymt4_getIntArgument();
         darga = pymt4_getDoubleArgument();
         dargb = pymt4_getDoubleArgument();
         iargb = pymt4_getIntArgument();
         dargc = pymt4_getDoubleArgument();
         dargd = pymt4_getDoubleArgument();
         sargb = pymt4_getStringArgument(StrBuffer);
         iargc = pymt4_getIntArgument();
         iargd = pymt4_getIntArgument();
         iarge = pymt4_getIntArgument();
         ires  = OrderSend(sarga,iarga,darga,dargb,iargb,dargc,dargd,sargb,iargc,iargd,iarge);
         err   = GetLastError();
	      pymt4_setIntResult(ires,err);
	      break;
      case CmdId_OrdersHistoryTotal:
         ires = OrdersHistoryTotal();
	      pymt4_setIntResult(ires,GetLastError());
	      break;
      case CmdId_OrderStopLoss:
         checkSelectedTicket(pymt4_getIntArgument());
         dres = OrderStopLoss();
	      pymt4_setDoubleResult(dres,GetLastError());
	      break;
      case CmdId_OrdersTotal:
         ires = OrdersTotal();
	      pymt4_setIntResult(ires,GetLastError());
	      break;
      case CmdId_OrderSwap:
         checkSelectedTicket(pymt4_getIntArgument());
         dres = OrderStopLoss();
	      pymt4_setDoubleResult(dres,GetLastError());
	      break;
      case CmdId_OrderSymbol:
         checkSelectedTicket(pymt4_getIntArgument());
         sres = OrderSymbol();
	      pymt4_setStringResult(sres,GetLastError());
	      break;
      case CmdId_OrderTakeProfit:
         checkSelectedTicket(pymt4_getIntArgument());
         dres = OrderTakeProfit();
	      pymt4_setDoubleResult(dres,GetLastError());
	      break;
      case CmdId_OrderTicket:
         checkSelectedTicket(pymt4_getIntArgument());
         ires = OrderTicket();
	      pymt4_setIntResult(ires,GetLastError());
	      break;
      case CmdId_OrderType:
         checkSelectedTicket(pymt4_getIntArgument());
         ires = OrderType();
	      pymt4_setIntResult(ires,GetLastError());
	      break;    
    
      /* Checkup */
      case CmdId_IsConnected:
         bres = IsConnected();
	      pymt4_setBoolResult(bres,GetLastError());
	      break;
      case CmdId_IsDemo:
         bres = IsDemo();
	      pymt4_setBoolResult(bres,GetLastError());
	      break;
      case CmdId_IsDllsAllowed:
         bres = IsDllsAllowed();
	      pymt4_setBoolResult(bres,GetLastError());
	      break;
      case CmdId_IsExpertEnabled:
         bres = IsExpertEnabled();
	      pymt4_setBoolResult(bres,GetLastError());
	      break;
      case CmdId_IsLibrariesAllowed:
         bres = IsLibrariesAllowed();
	      pymt4_setBoolResult(bres,GetLastError());
	      break;
      case CmdId_IsOptimization:
         bres = IsOptimization();
	      pymt4_setBoolResult(bres,GetLastError());
	      break;
      case CmdId_IsStopped:
         bres = IsStopped();
	      pymt4_setBoolResult(bres,GetLastError());
	      break;
      case CmdId_IsTesting:
         bres = IsTesting();
	      pymt4_setBoolResult(bres,GetLastError());
	      break;
      case CmdId_IsTradeAllowed:
         bres = IsTradeAllowed();
	      pymt4_setBoolResult(bres,GetLastError());
	      break;
      case CmdId_IsTradeContextBusy:
         bres = IsTradeContextBusy();
	      pymt4_setBoolResult(bres,GetLastError());
	      break;
      case CmdId_IsVisualMode:
         bres = IsVisualMode();
	      pymt4_setBoolResult(bres,GetLastError());
	      break;
      case CmdId_UninitializeReason:
         ires = UninitializeReason();
	      pymt4_setIntResult(ires,GetLastError());
	      break;
	      
	   /* Client Terminal */  
	      
      case CmdId_TerminalCompany:
         sres = TerminalCompany();
	      pymt4_setStringResult(sres,GetLastError());
	      break;
      case CmdId_TerminalName:
         sres = TerminalName();
	      pymt4_setStringResult(sres,GetLastError());
	      break;
      case CmdId_TerminalPath:
         sres = TerminalPath();
	      pymt4_setStringResult(sres,GetLastError());
	      break;
      
      /* Common Functions */

      case CmdId_Alert:
	      Alert(pymt4_getStringArgument(StrBuffer));
	      pymt4_setBoolResult(True,GetLastError());
	      break;
      case CmdId_Comment:
	      Comment(pymt4_getStringArgument(StrBuffer));
	      pymt4_setBoolResult(True,GetLastError());
	      break;
      case CmdId_GetTickCount:
         ires = GetTickCount();
	      pymt4_setIntResult(ires,GetLastError());
	      break;
      case CmdId_MarketInfo:
         sarga = pymt4_getStringArgument(StrBuffer);
         iargb = pymt4_getIntArgument();
         dres = MarketInfo(sarga,iargb);  
         err  = GetLastError();
	      pymt4_setDoubleResult(dres,err);
	      break;
      case CmdId_PlaySound:
         PlaySound(pymt4_getStringArgument(StrBuffer));
	      pymt4_setBoolResult(True,GetLastError());
	      break;
      case CmdId_Print:
	      Print(pymt4_getStringArgument(StrBuffer));
	      pymt4_setBoolResult(True,GetLastError());
	      break;
      case CmdId_SendFTP:
         sarga = pymt4_getStringArgument(StrBuffer);
         sargb = pymt4_getStringArgument(StrBuffer);
         bres = SendFTP(sarga,sargb);
         err  = GetLastError();
	      pymt4_setBoolResult(bres,err);
	      break;
      case CmdId_SendMail:
         sarga = pymt4_getStringArgument(StrBuffer);
         sargb = pymt4_getStringArgument(StrBuffer);
	      SendMail(sarga,sargb);
	      pymt4_setBoolResult(True,GetLastError());
	      break;
      case CmdId_SendNotification:
         SendNotification(pymt4_getStringArgument(StrBuffer));
	      pymt4_setBoolResult(True,GetLastError());  
	      break;
		  
 	   /* DateTime functions */
	   
	   case CmdId_TimeCurrent:	      
	      ires = TimeCurrent();
	      pymt4_setIntResult(ires,GetLastError());
	      break;
	   case CmdId_TimeLocal:	    
	      ires = TimeLocal();
	      pymt4_setIntResult(ires,GetLastError());
	      break;
	   case CmdId_TimeGMT:
	      ires = TimeGMT();
	      pymt4_setIntResult(ires,GetLastError());
	      break;
	   case CmdId_TimeGMTOffset:
	      ires = TimeGMTOffset();
	      pymt4_setIntResult(ires,GetLastError());
	      break;
		  
	   case CmdId_RefreshRates:
	      bres = RefreshRates();
	      pymt4_setBoolResult(bres, GetLastError());
	      break;
	   
	   
	   /* Object Functions */
	   
	   case CmdId_ObjectCreate:
	   
	      const string object_name = pymt4_getStringArgument(StrBuffer);
	      int    object_type = pymt4_getIntArgument();
         int    subwindow   = pymt4_getIntArgument();
         
         if (ObjectCreate(object_name,(ENUM_OBJECT)object_type,subwindow,0,0,0,0,0,0) )
         {
            int    array_sz    = pymt4_getIntArgument();
            for (int index =0;index<array_sz;++index)
            {
               int time  = pymt4_getIntArgument();
               double price = pymt4_getDoubleArgument();
               
               
               long chart_id = ChartID();

               ObjectSetInteger(chart_id ,object_name,OBJPROP_TIME,index,time);
               ObjectSetDouble(chart_id,object_name,OBJPROP_PRICE,index,price);               
               
            }
            
            err = GetLastError();
            pymt4_setBoolResult(true,err);
	      } else {
	         err = GetLastError();
	         pymt4_setBoolResult(false,err);
	      }
	      break;
	   case CmdId_WindowRedraw:
	      WindowRedraw();
	      pymt4_setBoolResult(true,0);
	      break;
	   
  }
   
    return(true);
}

int last_uninit  = 0;
int init()
{



   Print("PyMT4 Debug Mode : ",pymt4_isdebug());
   if (last_uninit != REASON_CHARTCHANGE)
   {
      if(!pymt4_initialize(Symbol(),WindowHandle(Symbol(),Period()),LISTEN_PORT))
      {
         Print("PyMT4 Already Intialized ...");
         return(7);
      }
      Print("PyMT4 Host Initialized ...");
   }

   while(true)
   {
      
      int pendingCommandId = pymt4_requestPendingCommand();
      if (pendingCommandId != CmdId_CheckShutdownCondition)
         evaluateCommand(pendingCommandId);    
      
      if (IsStopped())
      {
         printf("Stopped");
         break;   
      }
   }



   return(False);
}

void OnTick()
{


}


int deinit()
{ 
   last_uninit = UninitializeReason();
   
   if (last_uninit != REASON_CHARTCHANGE)
   {
      pymt4_uninitialize(Symbol(),WindowHandle(Symbol(),Period()));
      Print("PyMT4 Host Uninitialized ...");
   }
   return(0); 
   
}

