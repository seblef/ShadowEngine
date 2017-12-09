
#include "ViewFrustum.h"

using namespace Core;

void ViewFrustum::createFromViewProjMatrix(const Matrix4& mat, bool bNearAndFar)
{

#define sw(a,b) (mat(a,b))

	float len;
	Vector3 norm;
	norm.x=-(sw(0,3) + sw(0,0));
	norm.y=-(sw(1,3) + sw(1,0));
	norm.z=-(sw(2,3) + sw(2,0));
	len=1.0f / norm.getLength();
	norm*=len;
	_planes[VFP_LEFT].makePlane(norm,-(sw(3,3) + sw(3,0)) * len);

	norm.x=-(sw(0,3) - sw(0,0));
	norm.y=-(sw(1,3) - sw(1,0));
	norm.z=-(sw(2,3) - sw(2,0));
	len=1.0f / norm.getLength();
	norm*=len;
	_planes[VFP_RIGHT].makePlane(norm,-(sw(3,3) - sw(3,0)) * len);

	norm.x=-(sw(0,3) - sw(0,1));
	norm.y=-(sw(1,3) - sw(1,1));
	norm.z=-(sw(2,3) - sw(2,1));
	len=1.0f / norm.getLength();
	norm*=len;
	_planes[VFP_TOP].makePlane(norm,-(sw(3,3) - sw(3,1)) * len);

	norm.x=-(sw(0,3) + sw(0,1));
	norm.y=-(sw(1,3) + sw(1,1));
	norm.z=-(sw(2,3) + sw(2,1));
	len=1.0f / norm.getLength();
	norm*=len;
	_planes[VFP_BOTTOM].makePlane(norm,-(sw(3,3) + sw(3,1)) * len);
	 
	if(bNearAndFar)
	{
		norm.x=-sw(0,2);
		norm.y=-sw(1,2);
		norm.z=-sw(2,2);
		len=1.0f / norm.getLength();
		norm*=len;
		_planes[VFP_NEAR].makePlane(norm,-sw(3,2) * len);

		norm.x=-(sw(0,3) - sw(0,2));
		norm.y=-(sw(1,3) - sw(1,2));
		norm.z=-(sw(2,3) - sw(2,2));
		len=1.0f / norm.getLength();
		norm*=len;
		_planes[VFP_FAR].makePlane(norm,-(sw(3,3) - sw(3,2)) * len);

		_planeCount=6;
	}
	else
		_planeCount=4;

	_builtFromMatrix=true;
	_viewProj=mat;
}
