
#ifndef _TDATABASE_H_
#define _TDATABASE_H_

#include <string>

using namespace std;

namespace Core
{

template<typename T>
class TDataBase
{
protected:

	bool						_autoDelete;
	typedef map<string,T*>	TMap;
	TMap						_data;

public:

	TDataBase(bool autoDelete=true) : _autoDelete(autoDelete)			{}
	~TDataBase()														{ clear(); }

	void						clear()
	{
		if(_autoDelete)
		{
			TMap::iterator i(_data.begin());
			for(;i!=_data.end();++i)
				delete i->second;
		}

		_data.clear();
	}

	T*							getData(const string& name)
	{
		TMap::iterator d(_data.find(name));
		return d==_data.end() ? 0 : d->second;
	}

	void						registerData(const string& name, T* d)
	{
		T* old=getData(name);
		if(old)			delete old;

		_data[name]=d;
	}
	void						unregisterData(const string& name)
	{
		TMap::iterator d(_data.find(name));
		if (d != _data.end())
			_data.erase(_data.find(name));
	}

	const map<string,T*>&		getData() const								{ return _data; }
};

};

#endif