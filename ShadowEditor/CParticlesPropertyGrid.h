
#pragma once

#include <ParticleLib.h>

class CParticlesEditor;

class CParticlesPropertyGrid : public CMFCPropertyGridCtrl
{
protected:

	ParticleEmitter*			_template;
	CParticlesEditor*			_editor;
	bool						*_changed;
	string*						_name;

public:

	CParticlesPropertyGrid(CParticlesEditor *editor,bool *changed,string* name);
	virtual ~CParticlesPropertyGrid();

	void						OnPropertyChanged(CMFCPropertyGridProperty *pProp) const;
	void						setTemplate(ParticleEmitter* t);

};