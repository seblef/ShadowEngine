
#ifndef _EVENT_H_
#define _EVENT_H_

enum EventType
{
	ET_CLOSE=0,
	ET_LOSTFOCUS,
	ET_GETFOCUS,
	ET_MINIMIZE,
	ET_RESTORE,
	ET_MOUSEMOVE,
	ET_MOUSEWHEEL,
	ET_MOUSEBUTTONDOWN,
	ET_MOUSEBUTTONUP,
	ET_KEYDOWN,
	ET_KEYUP,

	ET_COUNT
};

class Event
{
	public:

		EventType			_type;
		int					_param1;
		int					_param2;

		Event() : _type(ET_COUNT)															{}
		Event(EventType et, int p1=0, int p2=0) : _type(et), _param1(p1), _param2(p2)			{}
		Event(const Event& e) : _type(e._type), _param1(e._param1), _param2(e._param2)		{}

		Event&				operator=(const Event& e)
		{
			_type=e._type;
			_param1=e._param1;
			_param2=e._param2;
			return *this;
		}
};

#endif