#ifndef __EVENTS_H
#define __EVENTS_H

#define REPORT_EVENT(EV, QUIET)	\
	announce(EV, QUIET)

typedef struct {
	const char* source;
	const char* data;
	int code;
} event_t;
event_t Event(const char* source, const char* data, int code);

typedef void (*eventHandler_t) (event_t);

void announce(event_t ev, bool quiet);
void installEventHandler(const char* source, eventHandler_t handler);

#endif