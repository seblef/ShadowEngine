
#ifndef _PARTICLEAPP_H_
#define _PARTICLEAPP_H_

class ParticleApp
{
protected:

	void					updateCamera(Camera& c, const Vector2& rot, float dist) const;

public:

	ParticleApp()			{}

	void					run(HINSTANCE hInst);
};

#endif