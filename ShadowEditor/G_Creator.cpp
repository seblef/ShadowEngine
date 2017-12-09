
#include "stdafx.h"
#include "G_Creator.h"
#include "G_EditorApp.h"
#include "G_ToolSystem.h"
#include "G_BaseToolCreate.h"
#include "G_Mesh.h"
#include "G_ParticleSystem.h"
#include "G_Light.h"
#include "G_Sound.h"
#include "G_Actor.h"
#include "G_Entity.h"
#include "resource.h"
#include "SelectionDlg.h"

void G_Creator::createMesh(G_MeshTemplate *t)
{
	if(!t)
	{
		CSelectionDlg dlg(0);
		const map<string,G_MeshTemplate*>& db(G_EditorApp::getSingletonRef().getMap().getMeshes().getData());
		map<string,G_MeshTemplate*>::const_iterator m(db.begin());

		for(;m!=db.end();++m)
			dlg.addItem(m->first);

		if(dlg.DoModal()==IDCANCEL || dlg.getSelectedItem().empty())
			return;

		t=G_EditorApp::getSingletonRef().getMap().getMeshes().getData(dlg.getSelectedItem());
	}

	G_Mesh *m=new G_Mesh(t);
	G_ToolSystem::getSingletonRef().setCurrentTool(GBT_CREATE);
	((G_BaseToolCreate*)G_ToolSystem::getSingletonRef().getCurrentTool())->setObject(m,t->getGeometry()->getBBox());
}

void G_Creator::createActor(Character *c)
{
	if (!c)
	{
		CSelectionDlg dlg(0);
		const map<string, Character*>& db(G_EditorApp::getSingletonRef().getCharacterDB().getData());
		map<string, Character*>::const_iterator m(db.begin());

		for (; m != db.end(); ++m)
			dlg.addItem(m->first);

		if (dlg.DoModal() == IDCANCEL || dlg.getSelectedItem().empty())
			return;

		c = G_EditorApp::getSingletonRef().getCharacterDB().getData(dlg.getSelectedItem());
	}

	G_Actor* a = new G_Actor(c);
	G_ToolSystem::getSingletonRef().setCurrentTool(GBT_CREATE);
	((G_BaseToolCreate*)G_ToolSystem::getSingletonRef().getCurrentTool())->setObject(a, a->getTemplate()->getActor()->getBBox());

}

void G_Creator::createParticleSystem(G_ParticlesTemplate *t)
{
	if (!t)
	{
		CSelectionDlg dlg(0);
		const map<string, G_ParticlesTemplate*>& db(G_EditorApp::getSingletonRef().getMap().getParticles().getData());
		map<string, G_ParticlesTemplate*>::const_iterator m(db.begin());

		for (; m != db.end(); ++m)
			dlg.addItem(m->first);

		if (dlg.DoModal() == IDCANCEL || dlg.getSelectedItem().empty())
			return;

		t = G_EditorApp::getSingletonRef().getMap().getParticles().getData(dlg.getSelectedItem());
	}

	G_ParticleSystem *m = new G_ParticleSystem(t);
	G_ToolSystem::getSingletonRef().setCurrentTool(GBT_CREATE);
	((G_BaseToolCreate*)G_ToolSystem::getSingletonRef().getCurrentTool())->setObject(m, BBox3(Vector3(-1.0f,-1.0f,-1.0f),Vector3::UnitVector));
}

void G_Creator::createLight(Light::LightType t)
{
	G_Light* l=new G_Light(t);
	G_ToolSystem::getSingletonRef().setCurrentTool(GBT_CREATE);
	((G_BaseToolCreate*)G_ToolSystem::getSingletonRef().getCurrentTool())->setObject(l,BBox3::NullBox);
}

void G_Creator::createSound(G_SoundTemplate* t)
{
	if (!t)
	{
		CSelectionDlg dlg(0);
		const map<string, G_SoundTemplate*>& db(G_EditorApp::getSingletonRef().getMap().getSounds().getData());
		map<string, G_SoundTemplate*>::const_iterator m(db.begin());

		for (; m != db.end(); ++m)
			dlg.addItem(m->first);

		if (dlg.DoModal() == IDCANCEL || dlg.getSelectedItem().empty())
			return;

		t = G_EditorApp::getSingletonRef().getMap().getSounds().getData(dlg.getSelectedItem());
	}

	G_Sound* snd = new G_Sound(t);
	G_ToolSystem::getSingletonRef().setCurrentTool(GBT_CREATE);
	((G_BaseToolCreate*)G_ToolSystem::getSingletonRef().getCurrentTool())->setObject(snd, BBox3(Vector3(-1.0f, -1.0f, -1.0f), Vector3::UnitVector));

}

void G_Creator::createEntity(G_EntityTemplate* e)
{
	if (!e)
	{
		CSelectionDlg dlg(0);
		const map<string, G_EntityTemplate*>& db(G_EditorApp::getSingletonRef().getEntityDB().getData());
		map<string, G_EntityTemplate*>::const_iterator i(db.begin());

		for (; i != db.end(); ++i)
			dlg.addItem(i->first);

		if (dlg.DoModal() == IDCANCEL || dlg.getSelectedItem().empty())
			return;

		e = G_EditorApp::getSingletonRef().getEntityDB().getData(dlg.getSelectedItem());
	}

	G_Entity* et = new G_Entity(e);
	G_ToolSystem::getSingletonRef().setCurrentTool(GBT_CREATE);
	((G_BaseToolCreate*)G_ToolSystem::getSingletonRef().getCurrentTool())->setObject(et, e->getGeometry() ? e->getGeometry()->getBBox() : BBox3::NullBox);

}