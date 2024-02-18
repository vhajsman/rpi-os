#include "events.h"
#include "kernel/utils/logging.h"

#include <libc/stddef.h>

#define _EVENT_HANDLERS_MAX 128

eventHandler_t _handlers[_EVENT_HANDLERS_MAX] = {NULL};

event_t Event(const char* source, const char* data, int code) {
	event_t ev;

	ev.source = source;
	ev.data = data;
	ev.code = code;

	return ev;
}

void _callHandlers(event_t ev) {
	for (int i = 0; i < _EVENT_HANDLERS_MAX; i++)
		_handlers[i](ev);
}

void announce(event_t ev, bool quiet) {
	if (!quiet) {
		
	}
}