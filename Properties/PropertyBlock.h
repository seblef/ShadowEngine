
#ifndef _PROPERTYBLOCK_H_
#define _PROPERTYBLOCK_H_

#include "PropValue.h"

class PropertyBlock
{
protected:

	typedef map<string, PropValue>		PropMap;

	PropValue			_nullProp;
	PropMap				_properties;

	PValueType			getType(const string& t) const
	{
		PValueType ret = PV_COUNT;
		for (int i = 0; i < PV_COUNT; ++i)
		{
			if (t == g_PValueNames[i])
				ret = (PValueType)i;
		}

		return ret;
	}

public:

	PropertyBlock();
	PropertyBlock(ScriptFile& sf);
	PropertyBlock(const PropertyBlock& pb);

	PropertyBlock&		operator=(const PropertyBlock& pb);
	
	void				parse(ScriptFile& sf);
	void				save(ostream& ofs) const;

	void				addProperty(const string& name, const PropValue& v);
	void				remProperty(const string& name);

	PropValue&			operator[](const string& name);
	const PropValue&	operator[](const string& name) const;

	PropValue&			getProperty(const string& name)					{ return (*this)[name]; }
	const PropValue&	getProperty(const string& name) const			{ return (*this)[name]; }

	bool				hasProperty(const string& name) const			{ return _properties.find(name) != _properties.end(); }
	bool				isNullProperty(const PropValue& pv) const		{ return pv == _nullProp; }
};

inline ostream& operator << (ostream& os, const PropertyBlock& pb)
{
	pb.save(os);
	return os;
}

#endif