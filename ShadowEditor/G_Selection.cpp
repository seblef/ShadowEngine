
#include "stdafx.h"
#include "G_Selection.h"
#include "G_EditorApp.h"
#include "MainFrm.h"

class PhysicCallback : public PxSimulationEventCallback
{
	public:

		PhysicCallback()  {}

		void onConstraintBreak(PxConstraintInfo* constraints, PxU32 count) { }
		void onWake(PxActor** actors, PxU32 count) {}
		void onSleep(PxActor** actors, PxU32 count) {}
		void onContact(const PxContactPairHeader& ph, const PxContactPair* pairs, PxU32 nbPairs)	{}
		void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) {}
};

static PxDefaultErrorCallback			g_DefaultErrorCallback;
static PxDefaultAllocator				g_DefaultAllocatorCallback;
static PhysicCallback					g_PhysicCallback;



G_Selection::G_Selection() : _ground(0), _groundSelected(false)
{
	_foundation=PxCreateFoundation(PX_PHYSICS_VERSION, g_DefaultAllocatorCallback, g_DefaultErrorCallback);
	assert(_foundation);

	_zoneManager=&PxProfileZoneManager::createProfileZoneManager(_foundation);

	_physic=PxCreatePhysics(PX_PHYSICS_VERSION,*_foundation,PxTolerancesScale(),true,_zoneManager);
	assert(_physic);

	_cooking=PxCreateCooking(PX_PHYSICS_VERSION,*_foundation,PxCookingParams());
	assert(_cooking);

	PxInitExtensions(*_physic);

	PxSceneDesc desc(_physic->getTolerancesScale());
	desc.cpuDispatcher=PxDefaultCpuDispatcherCreate(1);
	desc.filterShader=PxDefaultSimulationFilterShader;
	desc.gravity.x=0;
	desc.gravity.y=-9.81f;
	desc.gravity.z=0;
	desc.simulationEventCallback=&g_PhysicCallback;
	_scene=_physic->createScene(desc);
	assert(_scene);

	if(_physic->getPvdConnectionManager())
	{
		_connection=PxVisualDebuggerExt::createConnection(_physic->getPvdConnectionManager(),
			"localhost",5425,1000,PxVisualDebuggerExt::getAllConnectionFlags());
	}

	_defaultMat=_physic->createMaterial(0.5,0.5,0.5);
}

G_Selection::~G_Selection()
{
	_defaultMat->release();

	if(_connection)			_connection->release();

	_scene->release();
	_cooking->release();
	_physic->release();

	PxCloseExtensions();

	if(_zoneManager)		_zoneManager->release();

	_foundation->release();
}

void G_Selection::createGroundActor(int w, int h)
{
	float fw=0.5f*(float)w;
	float fh=0.5f*(float)h;

	PxTransform t(PxVec3(0));
	t.createIdentity();
	t.p.x=fw;
	t.p.y=-0.5f;
	t.p.z=fh;

	_ground=_physic->createRigidDynamic(t);
	_ground->setRigidDynamicFlag(PxRigidDynamicFlag::eKINEMATIC,true);
	_ground->createShape(PxBoxGeometry(fw,0.5f,fh),*_defaultMat);

	_scene->addActor(*_ground);
}

void* G_Selection::createMeshGeometry(const Geometry& g) const
{
	PxTriangleMeshDesc desc;
	PxDefaultMemoryOutputStream stream;

	desc.points.count=g.getVertexCount();
	desc.points.data=g.getVertices();
	desc.points.stride=g_VertexSize[VX_3D];
	desc.triangles.count=g.getTriangleCount();
	desc.triangles.data=(const void*)g.getTriangles();
	desc.triangles.stride=3*sizeof(unsigned short);
	desc.flags=PxMeshFlag::e16_BIT_INDICES;

	_cooking->cookTriangleMesh(desc,stream);

	PxDefaultMemoryInputData input(stream.getData(),stream.getSize());
	PxTriangleMesh *mesh=_physic->createTriangleMesh(input);
	return (void*)new PxTriangleMeshGeometry(mesh);
}

void G_Selection::deleteMeshGeometry(void* mesh) const
{
	delete ((PxTriangleMeshGeometry*)mesh);
}

G_SelectionItem *G_Selection::createSphereActor(float radius) const
{
	PxRigidDynamic *a=createActor();
	a->createShape(PxSphereGeometry(radius),*_defaultMat);

	return new G_SelectionItem(a,_scene);
}

G_SelectionItem *G_Selection::createBoxActor(const Vector3& size) const
{
	PxRigidDynamic *a=createActor();
	a->createShape(PxBoxGeometry(size.x*0.5f,size.y*0.5f,size.z*0.5f),*_defaultMat);

	return new G_SelectionItem(a,_scene);
}

G_SelectionItem *G_Selection::createMeshActor(void *mesh) const
{
	PxRigidDynamic *a=createActor();
	a->createShape(*((PxTriangleMeshGeometry*)mesh),*_defaultMat);

	return new G_SelectionItem(a,_scene);
}

G_Object* G_Selection::rayTrace(const Vector3& o, const Vector3& d, bool& ground) const
{
	PxVec3 origin(o.x,o.y,o.z);
	PxVec3 dir(d.x,d.y,d.z);
	dir.normalize();

	PxRaycastHit hit;

	if(_scene->raycastSingle(origin,dir,1e6f,PxSceneQueryFlag::eIMPACT,hit))
	{
		ground= (&hit.shape->getActor()==_ground);
		return (G_Object*)hit.shape->getActor().userData;
	}
	else
		return 0;
}

G_Object *G_Selection::select(const Vector3& o, const Vector3& d, bool addToSel, bool& ground)
{
	G_Object* obj=rayTrace(o,d,ground);

/*	if(!addToSel)
	{
		if(ground==_groundSelected || (_selection.size() > 0 && obj==*_selection.begin()))
			return obj;

		_selection.clear();
		_groundSelected=ground;
	}
	else if(!obj)
		return obj;
*/

	_groundSelected=ground;

	if(!addToSel)
		_selection.clear();

	if(obj)
		_selection.insert(obj);

	((CMainFrame*)::AfxGetMainWnd())->onSelectionChange();
	return obj;
}

void G_Selection::select(G_Object* o, bool addToSelection)
{
	if(!addToSelection)
	{
		_groundSelected=false;
		_selection.clear();
	}

	_selection.insert(o);
	if(_selection.size()==1)
		((CMainFrame*)::AfxGetMainWnd())->onSelectionChange();
}

void G_Selection::clearSelection()
{
	_selection.clear();
	_groundSelected=false;

	((CMainFrame*)::AfxGetMainWnd())->onSelectionChange();
}

void G_Selection::refresh()
{
	_scene->simulate(0.02f);
	_scene->fetchResults(true);
}

void G_Selection::drawSelection(G_EditorObjDrawer* d)
{
	SelectionSet::iterator o(_selection.begin());
	for(;o!=_selection.end();++o)
		(*o)->drawSelection(d);
}

void G_Selection::deleteSelection()
{
	SelectionSet::iterator o(_selection.begin());
	for(;o!=_selection.end();++o)
	{
		G_EditorApp::getSingletonRef().getMap().remObject(*o);
		delete *o;
	}

	clearSelection();
	G_EditorApp::getSingletonRef().refresh();
}

void G_Selection::saveSelection()
{
	clearSavedSelection();

	SelectionSet::const_iterator o(_selection.begin());
	for(;o!=_selection.end();++o)
		_savedSelection.insert(*o);
}

void G_Selection::copySavedSelection()
{
	clearSelection();

	SelectionSet::const_iterator o(_savedSelection.begin());
	for(;o!=_savedSelection.end();++o)
		_selection.insert((*o)->copy());
}