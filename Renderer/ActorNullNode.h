
#ifndef _ACTORNULLNODE_H_
#define _ACTORNULLNODE_H_

#include <string>

using namespace std;

class ActorNullNode
{
protected:

	int					_id;
	string				_name;

public:

	ActorNullNode() : _id(0)					{}
	ActorNullNode(const string& name, int id) : _id(id), _name(name)	{}
	ActorNullNode(const ActorNullNode& n) : _id(n._id), _name(n._name)	{}

	void				setID(int id)									{ _id=id; }
	void				setName(const string& name)						{ _name=name; }

	int					getID() const									{ return _id; }
	const string&		getName() const									{ return _name; }
};

#endif