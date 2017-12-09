
#ifndef _GROUND_H_
#define _GROUND_H_

#include <StdRenderer.h>
#include "GameMaterial.h"

#define GFLAG_WAKABLE				(1 << 16)
#define GMATERIAL_MASK				0xff
#define GGAMEMATERIAL_MASK			0x00ff

class Ground
{
protected:

	int				_width, _height;
	unsigned int*	_ground;
	
	typedef vector<Material*>		MaterialVector;
	typedef vector<GameMaterial*>	GameMaterialVector;
	MaterialVector		_materials;
	GameMaterialVector	_gameMaterials;

	unsigned int	getMaterialID(Material* m);

public:

	Ground(int w, int h, GameMaterial* nullMaterial);
	~Ground();

	void			paintRect(int x, int y, int w, int h, Material* m);
	void			finalize() const;

	void			setFlag(unsigned int f, const SRect& r);
	void			unsetFlag(unsigned int f, const SRect& r);

	void			setFlag(int x, int y, unsigned int f)
	{
		_ground[x + y*_width] |= f;
	}
	void			unsetFlag(int x, int y, unsigned int f)
	{
		_ground[x + y*_width] &= ~f;
	}

	bool			isCaseWakable(int x, int y) const					{ return (_ground[x+y*_width] & GFLAG_WAKABLE)!=0; }
	bool			isAreaWakable(const SRect& r) const
	{
		bool w=true;
		for(int y=r.y1;y<=r.y2;++y)
			for(int x=r.x1;x<=r.x2;++x)
				w&=isCaseWakable(x,y);
		return w;
	}

	int				getWidth() const									{ return _width; }
	int				getHeight() const									{ return _height; }

	void			addMaterial(Material* m)							{ _materials.push_back(m); }
	void			addGameMaterial(GameMaterial* m)					{ _gameMaterials.push_back(m); }

	unsigned int*	getGround() const									{ return _ground; }
	GameMaterial*	getGameMaterial(int x, int y) const
	{
		return _gameMaterials[((_ground[x + y*_width]) & GGAMEMATERIAL_MASK) >> 8];
	}
};

#endif