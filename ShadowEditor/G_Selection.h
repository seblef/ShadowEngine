
#ifndef _G_SELECTION_H_
#define _G_SELECTION_H_

#include <GameLib.h>
#include "G_SelectionItem.h"
#include "G_Object.h"

class G_Selection : public TSingleton<G_Selection>
{
public:

	typedef set<G_Object*>			SelectionSet;

protected:

	PxPhysics*						_physic;
	PxScene*						_scene;
	PxCooking*						_cooking;
	PxFoundation*					_foundation;
	PxProfileZoneManager*			_zoneManager;
	debugger::comm::PvdConnection*	_connection;

	PxMaterial*						_defaultMat;
	PxRigidDynamic*					_ground;

	SelectionSet					_selection;
	SelectionSet					_savedSelection;
	bool							_groundSelected;

	PxRigidDynamic*					createActor() const
	{
		PxTransform t(PxVec3(0));
		if(!t.isValid())		t.q.normalize();
		PxRigidDynamic *a=_physic->createRigidDynamic(t);
		a->setRigidDynamicFlag(PxRigidDynamicFlag::eKINEMATIC,true);
		return a;
	}

public:

	G_Selection();
	~G_Selection();

	void							createGroundActor(int w, int h);
	void							releaseGroundActor()
	{
		if(_ground)					_scene->removeActor(*_ground);
		_ground->release();
		_ground=0;
	}

	void*							createMeshGeometry(const Geometry& g) const;
	void							deleteMeshGeometry(void* mesh) const;

	G_SelectionItem*				createSphereActor(float radius) const;
	G_SelectionItem*				createBoxActor(const Vector3& size) const;
	G_SelectionItem*				createMeshActor(void* mesh) const;

	void							clearSelection();
	const SelectionSet&				getSelection() const							{ return _selection; }
	const SelectionSet&				getSavedSelection() const						{ return _savedSelection; }
	bool							isGroundSelected() const						{ return _groundSelected; }

	G_Object*						rayTrace(const Vector3& o, const Vector3& d, bool& ground) const;
	G_Object*						select(const Vector3& o, const Vector3& d, bool addToSelection, bool& ground);

	void							select(G_Object* o, bool addToSelection);

	void							refresh();
	void							drawSelection(G_EditorObjDrawer* d);

	void							deleteSelection();

	void							saveSelection();
	void							clearSavedSelection()							{ _savedSelection.clear(); }
	void							copySavedSelection();
};

#endif