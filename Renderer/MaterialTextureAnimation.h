
#ifndef _MATERIALTEXTUREANIMATION_H_
#define _MATERIALTEXTUREANIMATION_H_

#include "MaterialAnimation.h"

class MaterialTextureAnimation : public MaterialAnimation
{
protected:

	float			_animTime;
	int				_tilePerRow;
	int				_tilePerColumn;
	int				_tileCount;

public:

	MaterialTextureAnimation(Material *m) : MaterialAnimation(m),
		_animTime(0.1f), _tilePerRow(1), _tilePerColumn(1), _tileCount(1)		{}
	MaterialTextureAnimation(Material *m, float animTime,
		int tilePerRow, int tilePerCol, int tileCount) : MaterialAnimation(m),
		_animTime(animTime), _tilePerRow(tilePerRow), _tilePerColumn(tilePerCol),
		_tileCount(tileCount)													{}
	MaterialTextureAnimation(const MaterialTextureAnimation& a) : MaterialAnimation(a._material),
		_animTime(a._animTime), _tilePerRow(a._tilePerRow), _tilePerColumn(a._tilePerColumn),
		_tileCount(a._tileCount)												{}
	MaterialTextureAnimation(Material *m, ScriptFile& sf);

	const char*	getAnimationName() const				{ return "texture"; }

	void			set(float time);

	int				getTilePerRow() const				{ return _tilePerRow; }
	int				getTilePerColumn() const			{ return _tilePerColumn; }
	int				getTileCount() const				{ return _tileCount; }
	float			getAnimTime() const					{ return _animTime; }

	void			setTilePerRow(int tpr)				{ _tilePerRow=tpr; }
	void			setTilePerColumn(int tpc)			{ _tilePerColumn=tpc; }
	void			setTileCount(int tc)				{ _tileCount=tc; }
	void			setAnimTime(float at)				{ _animTime=at; }
};

#endif