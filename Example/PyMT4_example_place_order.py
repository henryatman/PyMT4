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
import time


if __name__ == "__main__":
    
    connected = Connect()
    if connected:
        
        # You must make sure that all options to allow trading from the EA are enabled in MT4. Your advised to run this on a demo account
    
        print "Placed Order : ",OrderSend("EURUSD",OP_BUY,0.1,MarketInfo("EURUSD",MODE_BID),10,0,0,"PyMT4 Order",0,0,CLR_NONE)

    else:
        print "Unable to connect.\n Check that Metatrader4 is running.\nCheck that you have pymt4_host expert loaded on a chart."
    

    Disconnect()
