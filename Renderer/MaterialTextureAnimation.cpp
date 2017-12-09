
#include "stdafx.h"
#include "MaterialTextureAnimation.h"

MaterialTextureAnimation::MaterialTextureAnimation(Material *m, ScriptFile& sf) : MaterialAnimation(m),
	_animTime(0.1f), _tileCount(1), _tilePerColumn(1), _tilePerRow(1)
{
	_animTime=stof(sf.getToken());
	_tileCount=stoi(sf.getToken());
	_tilePerRow=stoi(sf.getToken());
	_tilePerColumn=stoi(sf.getToken());
}

void MaterialTextureAnimation::set(float time)
{
	int idx=((int)(time* ((float)_tileCount) / _animTime)) % _tileCount;
	int row_idx=idx / _tilePerRow;
	int col_idx=idx % _tilePerRow;

	Vector2 s(1 / ((float)_tilePerRow),1 / ((float)_tilePerColumn));

	_material->getStdBuffer().setScale(s);
	_material->getStdBuffer().setOffset(Vector2(s.x * (float)row_idx,s.y * (float)col_idx));
}