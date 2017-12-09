
#ifndef _ENTITYDYNAMIC_H_
#define _ENTITYDYNAMIC_H_

#include "EntityTemplate.h"

class EntityDynamic : public EntityTemplate
{
protected:

	float					_density;

	virtual void			parseToken(const string& token, ScriptFile& sf);

public:

	EntityDynamic();
	EntityDynamic(ScriptFile& sf);

	virtual void			load();
	virtual void			unload();

	virtual GameEntity*		createInstance(const Matrix4& world);

	float					getDensity() const					{ return _density; }
};

#endif