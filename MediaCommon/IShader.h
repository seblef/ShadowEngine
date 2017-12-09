
#ifndef _ISHADER_H_
#define _ISHADER_H_

#include <string>

using namespace std;

class IShader
{
protected:

	bool				_good;
	string				_name;

public:

	IShader(const string& name) : _good(false), _name(name) {}
	virtual ~IShader() {}

	bool				isGood() const				{ return _good; }
	const string&		getName() const				{ return _name; }

	virtual void		set()=0;
};

#endif