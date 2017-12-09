
#ifndef _TVECTOR_H_
#define _TVECTOR_H_

#include <vector>

using namespace std;

namespace Core
{

template<typename T>
class TVector
{
protected:

	typedef vector<T*>	TVec;

	bool				_autoDelete;
	TVec				_data;

public:

	TVector(bool autoDelete=true) : _autoDelete(autoDelete)				{}
	~TVector()															{ clear(); }

	void				clear()
	{
		if(_autoDelete)
		{
			TVec::iterator i(_data.begin());
			for(;i!=_data.end();++i)
				delete *i;
		}

		_data.clear();
	}

	vector<T*>&			getVector()										{ return _data; }
	const vector<T*>&	getVector() const								{ return _data; }
};

};

#endif