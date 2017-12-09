
#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "Vector3.h"
#include "Color.h"
#include <string>
#include <assert.h>
#include <map>

using namespace std;

namespace Core
{

class Config
{
	protected:

		string					_fileName;

		typedef map<string,string>	StringMap;
		StringMap				_data;

		bool					extract(const string& v, float* vals, int val_count) const;
		void					parseLine(const string& var, const string& val);

	public:

		Config(const string& fileName);
		~Config()				{}

		bool					parseFile(const string& fileName);
		bool					save() const;

		void					setVar(const string& var, int val);
		void					setVar(const string& var, bool val);
		void					setVar(const string& var, float val);
		void					setVar(const string& var, const string& val);
		void					setVar(const string& var, const Vector3& val);
		void					setVar(const string& var, const Color& val);

		bool					getVar(const string& var, int& val) const;
		bool					getVar(const string& var, bool& val) const;
		bool					getVar(const string& var, float& val) const;
		bool					getVar(const string& var, string& val) const;
		bool					getVar(const string& var, Vector3& val) const;
		bool					getVar(const string& var, Color& val) const;

		bool					remVar(const string& var);
};

}

#endif