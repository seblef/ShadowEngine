
#include "stdafx.h"
#include "resource.h"
#include "CParticlesPropertyGrid.h"
#include "ParticlesEditor.h"

CParticlesPropertyGrid::CParticlesPropertyGrid(CParticlesEditor* e, bool *changed, string *name) :
	_editor(e), _template(0),
	_changed(changed), _name(name)
{

}

CParticlesPropertyGrid::~CParticlesPropertyGrid()
{

}

void CParticlesPropertyGrid::OnPropertyChanged(CMFCPropertyGridProperty *p) const
{
	string name(p->GetName());

	ParticleEmitterParams& p1(_template->getParameters1());
	ParticleEmitterParams& p2(_template->getParameters2());

#define S_UPDATECOLOR(prop,col)		\
	DWORD rgb = ((CMFCPropertyGridColorProperty*)prop)->GetColor();	\
	col.r = ((float)GetRValue(rgb)) / 255.0f;		\
	col.g = ((float)GetGValue(rgb)) / 255.0f;		\
	col.b = ((float)GetBValue(rgb)) / 255.0f;

	if (name == "Name")						(*_name) = wtoc_s(p->GetValue().bstrVal);
	else if (name == "LifeMin")				p1.setLife(p->GetValue().fltVal);
	else if (name == "LifeMax")				p2.setLife(p->GetValue().fltVal);
	else if (name == "MassMin")				p1.setMass(p->GetValue().fltVal);
	else if (name == "MassMax")				p2.setMass(p->GetValue().fltVal);
	else if (name == "FrictionMin")			p1.setFriction(p->GetValue().fltVal);
	else if (name == "FrictionMax")			p2.setFriction(p->GetValue().fltVal);
	else if (name == "VelocityXMin")		p1.getVelocity().x = p->GetValue().fltVal;
	else if (name == "VelocityYMin")		p1.getVelocity().y = p->GetValue().fltVal;
	else if (name == "VelocityZMin")		p1.getVelocity().z = p->GetValue().fltVal;
	else if (name == "VelocityXMax")		p2.getVelocity().x = p->GetValue().fltVal;
	else if (name == "VelocityYMax")		p2.getVelocity().y = p->GetValue().fltVal;
	else if (name == "VelocityZMax")		p2.getVelocity().z = p->GetValue().fltVal;
	else if (name == "StartColorMin")
	{
		S_UPDATECOLOR(p, p1.getStartColor());
	}
	else if (name == "StartAlphaMin")		p1.getStartColor().a = p->GetValue().fltVal;
	else if (name == "StartColorMax")
	{
		S_UPDATECOLOR(p, p2.getStartColor());
	}
	else if (name == "StartAlphaMax")		p2.getStartColor().a = p->GetValue().fltVal;
	else if (name == "EndColorMin")
	{
		S_UPDATECOLOR(p, p1.getEndColor());
	}
	else if (name == "EndAlphaMin")			p1.getEndColor().a = p->GetValue().fltVal;
	else if (name == "EndColorMax")
	{
		S_UPDATECOLOR(p, p2.getEndColor());
	}
	else if (name == "EndAlphaMax")			p2.getEndColor().a = p->GetValue().fltVal;
	else if (name == "StartSizeXMin")		p1.getStartSize().x = p->GetValue().fltVal;
	else if (name == "StartSizeYMin")		p1.getStartSize().y = p->GetValue().fltVal;
	else if (name == "StartSizeZMin")		p1.getStartSize().z = p->GetValue().fltVal;
	else if (name == "StartSizeXMax")		p2.getStartSize().x = p->GetValue().fltVal;
	else if (name == "StartSizeYMax")		p2.getStartSize().y = p->GetValue().fltVal;
	else if (name == "StartSizeZMax")		p2.getStartSize().z = p->GetValue().fltVal;
	else if (name == "EndSizeXMin")			p1.getEndSize().x = p->GetValue().fltVal;
	else if (name == "EndSizeYMin")			p1.getEndSize().y = p->GetValue().fltVal;
	else if (name == "EndSizeZMin")			p1.getEndSize().z = p->GetValue().fltVal;
	else if (name == "EndSizeXMax")			p2.getEndSize().x = p->GetValue().fltVal;
	else if (name == "EndSizeYMax")			p2.getEndSize().y = p->GetValue().fltVal;
	else if (name == "EndSizeZMax")			p2.getEndSize().z = p->GetValue().fltVal;
	else if (name == "GravityX")			_template->getGravity().x = p->GetValue().fltVal;
	else if (name == "GravityY")			_template->getGravity().y = p->GetValue().fltVal;
	else if (name == "GravityZ")			_template->getGravity().z = p->GetValue().fltVal;
	else if (name == "MaxParticles")		_template->setMaxParticles(p->GetValue().intVal);
	else if (name == "EmissionRate")		_template->setEmissionRate(p->GetValue().fltVal);
	else if (name == "Renderer")
	{
		ParticleRenderer *r = ParticleEngine::getSingletonRef().getRenderer(wtoc_s(p->GetValue().bstrVal));
		_template->setRenderer(r);
	}

	*_changed = true;
}

void CParticlesPropertyGrid::setTemplate(ParticleEmitter *t)
{
	_template = t;
	RemoveAll();

	if (!t)
		return;

	const ParticleEmitterParams& p1(t->getParameters1());
	const ParticleEmitterParams& p2(t->getParameters2());

	CMFCPropertyGridProperty *np = new CMFCPropertyGridProperty("Name", (_variant_t)_name->c_str(),
		"Name");
	AddProperty(np);

	CMFCPropertyGridProperty* gp_base = new CMFCPropertyGridProperty("Base");
	AddProperty(gp_base);

	CMFCPropertyGridProperty* gp_vel = new CMFCPropertyGridProperty("Velocity");
	AddProperty(gp_vel);

	CMFCPropertyGridProperty* gp_col = new CMFCPropertyGridProperty("Color");
	AddProperty(gp_col);

	CMFCPropertyGridProperty* gp_size = new CMFCPropertyGridProperty("Size");
	AddProperty(gp_size);

	CMFCPropertyGridProperty *p;
	CMFCPropertyGridColorProperty *c;

#define S_RGB(col)			RGB((int)((col).r * 255.0f),(int)((col).g * 255.0f),(int)((col).b * 255.0f))


#define PPROP(name,var,desc,group)		\
	p = new CMFCPropertyGridProperty(name, (_variant_t)var, desc);	\
	group->AddSubItem(p);

#define CPROP(name,var,desc,group)		\
	c = new CMFCPropertyGridColorProperty(name, S_RGB(var), 0, desc);	\
	c->EnableOtherButton("Others...");	\
	group->AddSubItem(c);

	//		Life
	PPROP("LifeMin", p1.getLife(), "MinimumLife",gp_base);
	PPROP("LifeMax", p2.getLife(), "Maximum life time", gp_base);
	PPROP("MassMin", p1.getMass(), "Minimum mass", gp_base);
	PPROP("MassMax", p2.getMass(), "Maximum mass",gp_base);
	PPROP("FrictionMin", p1.getFriction(), "Minimum friction", gp_base);
	PPROP("FrictionMax", p2.getFriction(), "Maximum friction", gp_base);

	PPROP("VelocityXMin", p1.getVelocity().x, "Minimum Velocity (X)", gp_vel);
	PPROP("VelocityYMin", p1.getVelocity().y, "Minimum Velocity (Y)", gp_vel);
	PPROP("VelocityZMin", p1.getVelocity().z, "Minimum Velocity (Z)", gp_vel);
	PPROP("VelocityXMax", p2.getVelocity().x, "Maximum Velocity (X)", gp_vel);
	PPROP("VelocityYMax", p2.getVelocity().y, "Maximum Velocity (Y)", gp_vel);
	PPROP("VelocityZMax", p2.getVelocity().z, "Maximum Velocity (Z)", gp_vel);

	CPROP("StartColorMin", p1.getStartColor(), "Minimum start color", gp_col);
	PPROP("StartAlphaMin", p1.getStartColor().a, "Minimum start alpha", gp_col);
	CPROP("StartColorMax", p2.getStartColor(), "Maximum start color", gp_col);
	PPROP("StartAlphaMax", p2.getStartColor().a, "Maximum start alpha", gp_col);
	CPROP("EndColorMin", p1.getEndColor(), "Minimum end color", gp_col);
	PPROP("EndAlphaMin", p1.getEndColor().a, "Minimum end alpha", gp_col);
	CPROP("EndColorMax", p2.getEndColor(), "Maximum end color", gp_col);
	PPROP("EndAlphaMax", p2.getEndColor().a, "Maximum end alpha", gp_col);

	PPROP("StartSizeXMin", p1.getStartSize().x, "Minimum Start Size (X)", gp_size);
	PPROP("StartSizeYMin", p1.getStartSize().y, "Minimum Start Size (Y)", gp_size);
	PPROP("StartSizeZMin", p1.getStartSize().z, "Minimum Start Size (Z)", gp_size);
	PPROP("StartSizeXMax", p2.getStartSize().x, "Maximum Start Size (X)", gp_size);
	PPROP("StartSizeYMax", p2.getStartSize().y, "Maximum Start Size (Y)", gp_size);
	PPROP("StartSizeZMax", p2.getStartSize().z, "Maximum Start Size (Z)", gp_size);

	PPROP("EndSizeXMin", p1.getEndSize().x, "Minimum End Size (X)", gp_size);
	PPROP("EndSizeYMin", p1.getEndSize().y, "Minimum End Size (Y)", gp_size);
	PPROP("EndSizeZMin", p1.getEndSize().z, "Minimum End Size (Z)", gp_size);
	PPROP("EndSizeXMax", p2.getEndSize().x, "Maximum End Size (X)", gp_size);
	PPROP("EndSizeYMax", p2.getEndSize().y, "Maximum End Size (Y)", gp_size);
	PPROP("EndSizeZMax", p2.getEndSize().z, "Maximum End Size (Z)", gp_size);

	PPROP("GravityX", t->getGravity().x, "Gravity (X)", gp_base);
	PPROP("GravityY", t->getGravity().y, "Gravity (Y)", gp_base);
	PPROP("GravityZ", t->getGravity().z, "Gravity (Z)", gp_base);

	PPROP("MaxParticles", t->getMaxParticles(), "Maximum particles count", gp_base);
	PPROP("EmissionRate", t->getEmissionRate(), "Emission Rate", gp_base);

	p = new CMFCPropertyGridProperty("Renderer", (_variant_t)t->getRenderer()->getRendererName(), "Renderer type");
	p->AddOption("point");
	p->AddOption("quad");
	gp_base->AddSubItem(p);

	ExpandAll();
}

