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

#property copyright ""
#property link      ""
#import "pymt4.dll"

bool pymt4_notifyOnTick(char &symbol[],double bid,double ask);
bool pymt4_isinitialized();

int init()
{
   EventSetTimer(1);
   return(0);
}

int deinit()
{

   return(0);
}

void OnTick()
{
   if (pymt4_isinitialized()){

      char symbol[];
      StringToCharArray(Symbol(), symbol);
      pymt4_notifyOnTick(symbol,Bid,Ask);
   }

}

void OnTimer()
{
   if (pymt4_isinitialized()){

      char symbol[];
      StringToCharArray(Symbol(), symbol);
      pymt4_notifyOnTick(symbol,Bid,Ask);
   }
}