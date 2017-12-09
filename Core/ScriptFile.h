
#ifndef _SCRIPTFILE_H_
#define _SCRIPTFILE_H_

#include "Vector3.h"
#include "Vector2.h"
#include "Color.h"
#include <string>
#include <fstream>

namespace Core
{

class ScriptFile
{
	private:

		void				getText();

		ifstream			_ifs;
		string				_fileName;
		string				_token;

	public:

		ScriptFile(const string& fileName);
		~ScriptFile()									{}

		const string&		getFileName() const			{ return _fileName; }
		
		const string&		getToken();
		void				parseColor(Color& c);
		void				parseVector(Vector3& v);
		void				parseVector(Vector2& v);

		bool				good() const				{ return _ifs.good(); }

};

}

#endif