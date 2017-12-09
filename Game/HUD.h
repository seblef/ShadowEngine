
#ifndef _HUD_H_
#define _HUD_H_

#include <R2D.h>

class HUDActor;
class HUDActorView;
class HUDCross;
class GameCamera;
class FPSCounter;


class HUD : public TSingleton<HUD>
{
protected:

	FPSCounter*				_fpsCounter;
	HUDCross*				_cross;

	R2D_Frame				*_frame;
	R2D_Font				*_hudFont;
	R2D_Layer				*_hudLayer;

	typedef set<HUDActor*>	ActorSet;
	ActorSet				_actors;
	HUDActorView*			_actorView;

public:

	HUD();
	~HUD();

	HUDActor*				createHUDActor(float maxLife, float actorHeight) const;
	void					registerActor(HUDActor* hudActor)	{ _actors.insert(hudActor); }
	void					unregisterActor(HUDActor* hudActor)	{ _actors.erase(hudActor); }

	void					updateHUD(GameCamera& gc);
};

#endif