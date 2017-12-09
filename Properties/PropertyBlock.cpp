
#include "stdafx.h"
#include "PropertyBlock.h"

PropertyBlock::PropertyBlock() : _nullProp((int)0)
{
}

PropertyBlock::PropertyBlock(ScriptFile& sf) : _nullProp((int)0)
{
	parse(sf);
}

PropertyBlock::PropertyBlock(const PropertyBlock& pb) : _nullProp((int)0)
{
	(*this) = pb;
}

PropertyBlock& PropertyBlock::operator=(const PropertyBlock& pb)
{
	PropMap::const_iterator p(pb._properties.begin());
	for (; p != pb._properties.end(); ++p)
		addProperty(p->first, p->second);

	return *this;
}

void PropertyBlock::parse(ScriptFile& sf)
{
	PValueType type;
	string name;

	string t(sf.getToken());
	while (sf.good() && t != "end_properties")
	{
		type = getType(t);
		name = sf.getToken();
		if (type == PV_BOOL)
		{
			bool b = sf.getToken() == "true" ? true : false;
			addProperty(name, b);
		}
		else if (type == PV_INT)
		{
			int i = stoi(sf.getToken());
			addProperty(name, i);
		}
		else if (type == PV_FLOAT)
		{
			float f = stof(sf.getToken());
			addProperty(name, f);
		}
		else if (type == PV_STRING)
			addProperty(name, sf.getToken());
		else if (type == PV_VECTOR)
		{
			Vector3 v;
			sf.parseVector(v);
			addProperty(name, v);
		}
		else if (type == PV_COLOR)
		{
			Color c;
			sf.parseColor(c);
			addProperty(name, c);
		}

		t = sf.getToken();
	}
}

void PropertyBlock::save(ostream& ofs) const
{
	PropMap::const_iterator p(_properties.begin());
	for (; p != _properties.end(); ++p)
		ofs << g_PValueNames[p->second.getValueType()] << " " << p->first << " " << p->second << endl;

	ofs << "end_properties" << endl;
}

void PropertyBlock::addProperty(const string& name, const PropValue& v)
{
	_properties[name] = v;
}

void PropertyBlock::remProperty(const string& name)
{
	PropMap::iterator p(_properties.find(name));
	if (p != _properties.end())
		_properties.erase(p);
}

PropValue& PropertyBlock::operator[](const string& name)
{
	return _properties[name];
}

const PropValue& PropertyBlock::operator[](const string& name) const
{
	PropMap::const_iterator p(_properties.find(name));
	return p == _properties.end() ? _nullProp : p->second;
}
