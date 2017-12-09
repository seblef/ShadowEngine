
#ifndef _TSET_H_
#define _TSET_H_

#include <set>

using namespace std;

namespace Core
{

template<typename T>
class TSet
{
protected:

	typedef set<T*>	TS;

	bool				_autoDelete;
	TS					_data;

public:

	TSet(bool autoDelete=true) : _autoDelete(autoDelete)				{}
	~TSet()																{ clear(); }

	void				clear()
	{
		if(_autoDelete)
		{
			TS::iterator i(_data.begin());
			for(;i!=_data.end();++i)
				delete *i;
		}

		_data.clear();
	}

	set<T*>&			getSet()									{ return _data; }
	const set<T*>&		getSet() const								{ return _data; }
};

};

#endif