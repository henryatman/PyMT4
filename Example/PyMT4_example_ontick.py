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

def myOnTickHandler(symbol,bid,ask):
    print "[ OnTick Event ] ", symbol,bid,ask
    


if __name__ == "__main__":
    
    connected = Connect()
        
    
    if connected:
        # Ensure you have pymt4_ontick expert loaded onto the chart from which you want to recieve tick events.\nThis must be a different to the chart that runs pymt4_host, which must also be running.
        # NOTE: There is a fake timer setup on 1 second intervals, so your handler may be verified even when there are no ticks happening in the market.
        
        # "*" means this handler recieves ticks for all symbols, in case you have pymt4_ontick running on multiple charts. You can filter which events are recieved by 
        # this handler by putting "EURUSD" for example instead. In which case you MUST have pymt4_ontick running on an EURUSD chart.
        RegisterOnTickHandler("*",myOnTickHandler)
        
        # Run for 10 seconds
        for _ in range(10):
            time.sleep(1)
        
    else:
        print "Unable to connect.\n Check that Metatrader4 is running.\nCheck that you have pymt4_host expert loaded on a chart."
    

    Disconnect()
