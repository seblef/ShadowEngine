
#include "Config.h"
#include "ScriptFile.h"

using namespace Core;

Config::Config(const string& fileName) : _fileName(fileName)
{
	parseFile(fileName);
}

bool Config::parseFile(const string& fileName)
{
	ScriptFile sf(fileName);
	if(sf.good())
	{
		string var;

		while(sf.good())
		{
			var=sf.getToken();
			parseLine(var,sf.getToken());
		}

		return true;
	}
	else
		return false;
}

void Config::parseLine(const string& var, const string& val)
{
	char first=val[0];

	if(first=='(')
	{
		int coma_count=0;
		for(int p=1;p<val.length();++p)
		{
			if(val[p]==',')
				++coma_count;
		}

		if(coma_count==2)
		{
			Vector3 vec;
			if(extract(val,(float*)&vec,3))
				setVar(var,vec);
		}
		else if(coma_count==3)
		{
			Color col;
			if(extract(val,(float*)&col,4))
				setVar(var,col);
		}
	}
	else if ((first >= '0' && first <= '9') || first == '-' || first == '.')
	{
		bool fl = false;
		for (int p = 0; p < val.length(); ++p)
			fl |= (val[p] == '.' || val[p] == 'e' || val[p] == 'E');

		if (fl)
			setVar(var, stof(val));
		else
			setVar(var, stoi(val));
	}
	else if (val == "true" || val == "false")
		setVar(var, val == "true");
	else
		setVar(var,val);
}

bool Config::save() const
{
	ofstream ofs(_fileName,fstream::out);
	if(ofs.good())
	{
		StringMap::const_iterator v(_data.begin());
		for(;v!=_data.end();++v)
			ofs << v->first << "	" << v->second << endl;

		return true;
	}
	else
		return false;
}

void Config::setVar(const string& var, int val)
{
	_data[var]=to_string(val);
}

void Config::setVar(const string& var, bool val)
{
	_data[var]=val ? "true" : "false";
}

void Config::setVar(const string& var, float val)
{
	_data[var]=to_string(val);
}

void Config::setVar(const string& var, const string& val)
{
	_data[var]=val;
}

void Config::setVar(const string& var, const Vector3& val)
{
	string v("(");
	v+=to_string(val.x);
	v+=",";
	v+=to_string(val.y);
	v+=",";
	v+=to_string(val.z);
	v+=")";
	_data[var]=v;
}

void Config::setVar(const string& var, const Color& val)
{
	string v("(");
	v+=to_string(val.r);
	v+=",";
	v+=to_string(val.g);
	v+=",";
	v+=to_string(val.b);
	v+=",";
	v+=to_string(val.a);
	v+=")";
	_data[var]=v;
}

bool Config::getVar(const string& var, int& val) const
{
	StringMap::const_iterator v(_data.find(var));
	if(v==_data.end())
		return false;
	else
	{
		val=stoi(v->second);
		return true;
	}
}

bool Config::getVar(const string& var, bool& val) const
{
	StringMap::const_iterator v(_data.find(var));
	if(v==_data.end())
		return false;
	else
	{
		val=(v->second=="true");
		return true;
	}
}

bool Config::getVar(const string& var, float& val) const
{
	StringMap::const_iterator v(_data.find(var));
	if(v==_data.end())
		return false;
	else
	{
		val=stof(v->second);
		return true;
	}
}

bool Config::getVar(const string& var, string& val) const
{
	StringMap::const_iterator v(_data.find(var));
	if(v==_data.end())
		return false;
	else
	{
		val=v->second;
		return true;
	}
}

bool Config::getVar(const string& var, Vector3& val) const
{
	StringMap::const_iterator v(_data.find(var));
	if(v==_data.end())
		return false;
	else
		return extract(v->second,(float*)&val,3);
}

bool Config::getVar(const string& var, Color& val) const
{
	StringMap::const_iterator v(_data.find(var));
	if(v==_data.end())
		return false;
	else
		return extract(v->second,(float*)&val,4);
}

bool Config::remVar(const string& var)
{
	StringMap::iterator v(_data.find(var));
	if(v==_data.end())
		return false;
	else
	{
		_data.erase(v);
		return true;
	}
}

bool Config::extract(const string& v, float *vals, int val_count) const
{
	if(v[0]=='(' && v[v.length()-1]==')')
	{
		int last=0;
		int p;

		for(int i=0;i<val_count-1;++i)
		{
			p=last+1;
			while(p < v.length() && v[p]!=',')
				++p;

			if(p >= v.length())
				return false;

			vals[i]=stof(v.substr(last+1,p-last-1));
			last=p;
		}

		vals[val_count-1]=stof(v.substr(last+1,v.length() - last-2));
	}

	return true;
}