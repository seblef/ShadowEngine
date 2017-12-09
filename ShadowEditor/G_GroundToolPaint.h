
#ifndef _G_GROUNDTOOLPAINT_H_
#define _G_GROUNDTOOLPAINT_H_

#include "G_Tool.h"
#include "G_GameMaterial.h"
#include <StdRenderer.h>

class G_GroundToolPaint : public G_Tool
{
protected:

	Material*					_mat;
	G_GameMaterial*				_gameMat;
	Plane						_ground;

public:

	G_GroundToolPaint() : _mat(0), _gameMat(0), _ground(Vector3::NullVector, Vector3::YAxisVector)				{}
	~G_GroundToolPaint()						{}

	void						setMaterial(Material* m)				{ _mat=m; }
	void						setGameMaterial(G_GameMaterial* m)		{ _gameMat = m; }

	void						mouseDown(int x, int y, unsigned short flags);
	void						mouseMove(int x, int y);
	void						mouseUp(int x, int y);
};

#endif