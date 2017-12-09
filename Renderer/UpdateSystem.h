
#ifndef _UPDATESYSTEM_H_
#define _UPDATESYSTEM_H_

#include "IUpdatable.h"
#include <Core.h>

using namespace Core;

class UpdateSystem : public TSingleton<UpdateSystem>
{
protected:

	typedef set<IUpdatable*>	UpdatableSet;
	UpdatableSet				_updatables;

public:

	UpdateSystem()				{}

	void						registerUpdatable(IUpdatable* u)				{ _updatables.insert(u); }
	void						unregisterUpdatable(IUpdatable* u)				{ _updatables.erase(u); }

	void						process(float time) const
	{
		UpdatableSet::const_iterator u(_updatables.begin());
		for(;u!=_updatables.end();++u)
			(*u)->update(time);
	}
};


#endif