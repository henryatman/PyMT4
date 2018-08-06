"""
 (C) Copyright 2013 Rob Watson rmawatson [at] hotmail.com  and others.

 All rights reserved. This program and the accompanying materials
 are made available under the terms of the GNU Lesser General Public License
 (LGPL) version 2.1 which accompanies this distribution, and is available at
 http://www.gnu.org/licenses/lgpl-2.1.html

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 Lesser General Public License for more details.

 Contributors:
     Rob Watson ( rmawatson [at] hotmail )
"""

from PyMT4 import *

if __name__ == "__main__":
    
    connected = Connect()
        
    
    if connected:
    
        print "[ Account Information ]"
        print "AccountBalance".ljust(10),":",AccountBalance()
        print "AccountCredit".ljust(10),":",AccountCredit()
        print "AccountCompany".ljust(10),":",AccountCompany()
        print "AccountCurrency".ljust(10),":",AccountCurrency()
        print "AccountEquity".ljust(10),":",AccountEquity()
        print "AccountFreeMargin".ljust(10),":",AccountFreeMargin()
        print "AccountFreeMarginCheck".ljust(10),":",AccountFreeMarginCheck("EURUSD",OP_BUY,0.1)
        print "AccountFreeMarginMode".ljust(10),":",AccountFreeMarginMode()
        print "AccountLeverage".ljust(10),":",AccountLeverage()
        print "AccountMargin".ljust(10),":",AccountMargin()
        print "AccountName".ljust(10),":",AccountName()
        print "AccountNumber".ljust(10),":",AccountNumber()
        print "AccountProfit".ljust(10),":",AccountProfit()
    
    else:
        print "Unable to connect.\n Check that Metatrader4 is running.\nCheck that you have pymt4_host expert loaded on a chart."
    

    Disconnect()

