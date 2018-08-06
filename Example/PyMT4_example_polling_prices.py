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
        
    symbols = ["EURUSD","GBPUSD","USDJPY"]
    if connected:
        
        
        # Run for 10 seconds, at 0.1 second intervals
        for _ in range(100):
        
            prices = ""
            
            for symbol in symbols:
                prices += ", " + symbol + " : " + str(MarketInfo(symbol,MODE_BID)) + ":" + str(MarketInfo(symbol,MODE_ASK))
                
            print "[ Polling Prices ] ",prices
            
            time.sleep(0.1)
    
    else:
        print "Unable to connect.\n Check that Metatrader4 is running.\nCheck that you have pymt4_host expert loaded on a chart."
    

    Disconnect()
