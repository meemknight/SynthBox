#include "windowsStuff.h"

#ifdef _WIN32
#include <Windows.h>
#endif


void enableTimeBeginPeriod()
{
#ifdef _WIN32
	timeBeginPeriod(1);
#endif
}