#include "windowsStuff.h"

#include <Windows.h>

void enableTimeBeginPeriod()
{
	timeBeginPeriod(1);
}