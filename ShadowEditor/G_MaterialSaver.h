
#ifndef _G_MATERIALSAVER_H_
#define _G_MATERIALSAVER_H_

#include "G_ResourceCollector.h"

class G_MaterialSaver
{
protected:

	void					saveMaterial(const Material& m, const string& matName, ofstream& ofs) const;
	void					saveUVAnim(MaterialUVAnimation *a, ofstream& ofs) const;
	void					saveTextureAnim(MaterialTextureAnimation* a, ofstream& ofs) const;
	void					saveFadeAnim(MaterialFadeAnimation* a, ofstream& ofs) const;

	void					outputColor(const string& n, const Color& c, ofstream& o) const
	{
		o << "	" << n << " " << c.r << " " << c.g << " " << c.b << " " << c.a << endl;
	}
	void					outputVector(const string& n, const Vector2& v, ofstream& o) const
	{
		o << "	" << n << " " << v.x << " " << v.y << endl;
	}

public:

	G_MaterialSaver()		{}

	void					saveMaterials(const G_ResourceCollector& c, ofstream& ofs) const;		
};

#endif