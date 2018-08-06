
/*testing */
#ifdef _CONSOLE
#include "pymt4_common.h"
#include <windows.h>

#include <boost/thread.hpp>

namespace PyMT4 {
	
#define CmdId_CheckShutdownCondition 100

	EXPORT(BOOL) pymt4_initialize(const char* windowName, int32_t updateWindowHandle, int32_t port);
	EXPORT(int) pymt4_requestPendingCommand();
	EXPORT(const wchar_t*) pymt4_getStringArgument(wchar_t* string);
	EXPORT(int) pymt4_getIntArgument();
	EXPORT(BOOL) pymt4_setIntResult(int32_t result, int32_t error);
	EXPORT(BOOL) pymt4_setDoubleResult(double result, int32_t error);
	EXPORT(BOOL) pymt4_notifyOnTick(const char* symbol, double bid, double ask);
}


using namespace PyMT4;


void init()
{

	pymt4_initialize("EURUSD",0, 5621);

	for (;;)
	{
		std::cout << "Polling" << std::endl;

		int pendingCommandId = pymt4_requestPendingCommand();
		if (pendingCommandId != CmdId_CheckShutdownCondition)
		{
			if (pendingCommandId == 6003)
			{
				wchar_t buff[1024] = {0};

				pymt4_getStringArgument(buff);
				pymt4_getIntArgument();

				pymt4_setDoubleResult(100, 0);

			}
			
		}

	}

}

void tick()
{
	for (;;)
	{
		pymt4_notifyOnTick("EURUSD", 1.0,1.0);

	}


}


void main()
{
	
	boost::thread polling_thread(init);
	boost::thread ticking_thread(tick);

	polling_thread.join();
	ticking_thread.join();


}


#endif