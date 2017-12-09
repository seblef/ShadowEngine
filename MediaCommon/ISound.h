
#ifndef _ISOUND_H_
#define _ISOUND_H_

#include <string>

using namespace std;

class ISound
{
protected:

	string				_soundName;
	int					_refCount;

public:

	ISound(const string& soundName) : _soundName(soundName), _refCount(1)				{}
	virtual ~ISound()																	{}

	int					addRef()				{ return ++_refCount; }
	int					remRef()				{ return --_refCount; }
	int					getRefCount() const		{ return _refCount; }

	const string&		getSoundName() const	{ return _soundName; }
};

#endif