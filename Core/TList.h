
#ifndef _TLIST_H_
#define _TLIST_H_

#include <list>

using namespace std;

namespace Core
{

template<typename T>
class TList
{
protected:

	typedef list<T*>	TL;

	bool				_autoDelete;
	TL					_data;

public:

	TList(bool autoDelete=true) : _autoDelete(autoDelete)				{}
	~TList()															{ clear(); }

	void				clear()
	{
		if(_autoDelete)
		{
			TL::iterator i(_data.begin());
			for(;i!=_data.end();++i)
				delete *i;
		}

		_data.clear();
	}

	list<T*>&			getList()										{ return _data; }
	const list<T*>&		getList() const									{ return _data; }
};

};

#endif