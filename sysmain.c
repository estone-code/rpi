#include "monitor.h"

void sysmain(void)
{
	monitor_init();
	while (1) {
		monitor();
	}
}
