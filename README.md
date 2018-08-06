PyMT4 - Python bindings for the Metatrader 4 trading platform  ( rmawatson [ at ] hotmail. com )

Overview -

This project provides bindings for using the Metatrader4 MQL API in python. 

The bindings are provided through a tcp connection with the metatrader 4 client
via pymt4_host.mq4 using boost::asio. The python session connects to the host, 
and can then make API calls natively in python. An experimental OnTick handler is 
provided to register charts to notify the python session of new ticks.

A simple example in python:

Prerequisites:
 - Metatrader has a chart running pymt4_host.mq4, allowing execution of dll calls.
 - PyMT4.pyd is available on PYTHONPATH
 - For OnTick events ,a separate chart for the desired pair should be running pymt4_ontick.mq4.
	
The PyMT4 module contains most of the API. Although there are a raft of updated backported from MT5.
Most of these are not added yet. Custom Indicators are also not supported yet.

Please see Examples folder for usage.

----

BTW:

It's seem have some problem with iTime with python call, if someone could fix it, it would be great.

Enjoy it
