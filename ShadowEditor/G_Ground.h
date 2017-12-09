
#ifndef _G_GROUND_H_
#define _G_GROUND_H_

#include <StdRenderer.h>
#include <GameLib.h>
#include "G_GameMaterial.h"

class G_Ground : public GroundData
{
protected:

	int					_currentMat;
	unsigned int		_currentGameMat;
	int					_matCount;

	unsigned int*		_flags;
	unsigned int*		_gameMaterials;
	
	typedef vector<G_GameMaterial*>		GameMaterialVector;
	GameMaterialVector	_gMaterials;

public:

	G_Ground(int w, int h, int matCount, Material **mats, int *mat_ids, G_GameMaterial* nullMat);
	~G_Ground();

	void				onLoad(int mat_count, Material** mats, unsigned int *ids,
		int gmat_count, G_GameMaterial** gmats);

	void				resize(int w, int h);

	void				beginPaint(Material *m, G_GameMaterial* gm);
	void				paint(int x, int y)
	{
		_triangles[(x+y*_width)*2].setMatID(_currentMat);
		_triangles[(x+y*_width)*2+1].setMatID(_currentMat);
		_gameMaterials[x + y*_width] = _currentGameMat;
	}
	void				endPaint()
	{
		_currentMat=-1;
	}

	void				setFlag(int x, int y, unsigned int f)	{ _flags[x+y*_width]|=f; }
	void				unsetFlag(int x, int y, unsigned int f)	{ _flags[x+y*_width]&= ~f; }
	unsigned int		getFlag(int x, int y, unsigned int f) const	{ return _flags[x+y*_width] & f; }
	unsigned int		getGameMaterial(int x, int y)			{ return _gameMaterials[x + y*_width]; }

	int					getMaterialCount() const				{ return _matCount; }
	int					getGameMaterialCount() const			{ return _gMaterials.size(); }
	G_GameMaterial*		getGameMaterial(int n) const			{ return _gMaterials[n]; }
};

#endif