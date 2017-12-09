
#ifndef _G_SOUNDTEMPLATE_H_
#define _G_SOUNDTEMPLATE_H_

#include "G_SoundCone.h"
#include <string>

using namespace std;

class G_SoundTemplate
{
protected:

	int					_priority;
	float				_radius;
	float				_radiusAngle;
	G_SoundCone*		_cone;
	string				_soundFile;
	string				_name;

public:

	G_SoundTemplate(const string& name) : _name(name), _priority(1), _radius(10), _radiusAngle(0), _cone(0)			{}
	G_SoundTemplate(const string& name, int priority, float radius, float radiusAngle, const string& soundFile, G_SoundCone* cone) :
		_name(name), _priority(priority), _radius(radius), _radiusAngle(radiusAngle), _soundFile(soundFile), _cone(cone)			{}
	G_SoundTemplate(const G_SoundTemplate& t) : _priority(t._priority), _radius(t._radius), _radiusAngle(t._radiusAngle), _soundFile(t._soundFile),
		_name(t._name)
	{
		if (t._cone)		_cone = new G_SoundCone(*t._cone);
	}
	~G_SoundTemplate()
	{
		if (_cone)
			delete _cone;
	}

	const string&		getName() const					{ return _name; }
	int					getPriority() const				{ return _priority; }
	float				getRadius() const				{ return _radius; }
	float				getRadiusAngle() const			{ return _radiusAngle; }
	const string&		getSoundFile() const			{ return _soundFile; }
	G_SoundCone*		getCone() const					{ return _cone; }

	void				setName(const string& n)		{ _name = n; }
	void				setPriority(int p)				{ _priority = p; }
	void				setRadius(float r)				{ _radius = r; }
	void				setRadiusAngle(float a)			{ _radiusAngle = a; }
	void				setSoundFile(const string& s)	{ _soundFile = s; }
	void				setCone(G_SoundCone* c)
	{
		if (_cone && _cone != c)
			delete _cone;
		_cone = c;
	}
};

#endif