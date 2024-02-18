#ifndef __LOGGING_H
#define __LOGGING_H

#include "drivers/uart.h"

#ifndef ___debug_defined
#define ___debug_defined
	#ifndef __debug
		#define ___f_debug(x)
		#define ___f_debugln(x)
	#else
		#define ___f_debug(x) \
			uart_puts(x)
		#define ___f_debugln(x) \
			uart_puts(x << '\n')
	#endif
#endif

void logString(const char* str);
void logLine(const char* str);
void logData(const char* src, const char* data, int code);

#endif