
#ifndef _G_SOUND_H_
#define _G_SOUND_H_

#include "G_Object.h"
#include "G_SoundTemplate.h"

class G_Sound : public G_Object
{
protected:

	G_SoundTemplate*			_template;

public:

	G_Sound(G_SoundTemplate* t);
	G_Sound(const G_Sound& ps);
	~G_Sound()														{}

	G_SoundTemplate*			getTemplate() const					{ return _template; }

	G_Object*					copy()								{ return new G_Sound(*this); }

	void						onAddToScene();
	void						onRemFromScene();

	void						drawSelection(G_EditorObjDrawer* d);
};

#endif