
#ifndef _ACTORSUB_H_
#define _ACTORSUB_H_

#include <string>

using namespace std;

class ActorSub
{
protected:

	int				_triOffset;
	int				_triCount;
	string			_name;

public:

	ActorSub() : _triOffset(0), _triCount(0)				{}
	ActorSub(int triOffset, int triCount, const string& name) : _triOffset(triOffset), _triCount(triCount), _name(name)		{}
	ActorSub(const ActorSub& a) : _triOffset(a._triOffset), _triCount(a._triCount), _name(a._name)										{}

	void			setTriCount(int count)							{ _triCount=count; }
	void			setTriOffset(int offset)						{ _triOffset=offset; }
	void			setName(const string& name)						{ _name=name; }

	int				getTriCount() const								{ return _triCount; }
	int				getTriOffset() const							{ return _triOffset; }
	const string&	getName() const									{ return _name; }
};

#endif