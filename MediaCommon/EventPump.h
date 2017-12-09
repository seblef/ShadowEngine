
#ifndef _EVENTPUMP_H_
#define _EVENTPUMP_H_

#include "Event.h"
#include <Core.h>
#include <list>

using namespace std;

class EventPump : public Core::TSingleton<EventPump>
{
protected:

	typedef list<Event>		EventList;
	EventList				_events;

public:

	EventPump()				{}
	~EventPump()			{}

	void					postEvent(const Event& e)				{ _events.push_back(e); }
	int						getEvent(Event& e)
	{
		if(_events.size() > 0)
		{
			e=_events.front();
			_events.pop_front();
			return _events.size()+1;
		}
		else
			e._type=ET_COUNT;

		return _events.size();
	}
};


#endif