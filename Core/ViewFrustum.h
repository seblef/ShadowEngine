
#ifndef _VIEWFRUSTUM_H_
#define _VIEWFRUSTUM_H_

#include "Plane.h"
#include <assert.h>

#define FRUSTRUM_MAX_PLANES				32
#define FRUSTRUM_MAX_USER_PLANES		30

namespace Core
{

enum ViewFrustumPlane
{
	VFP_LEFT=0,
	VFP_RIGHT,
	VFP_BOTTOM,
	VFP_TOP,
	VFP_FAR,
	VFP_NEAR
};

class ViewFrustum
{
	protected:

		bool				_builtFromMatrix;
		int					_planeCount;

		Vector3				_position;
		Matrix4				_viewProj;
		Plane				_planes[FRUSTRUM_MAX_PLANES];

	public:

		ViewFrustum() : _builtFromMatrix(false), _planeCount(0), _viewProj(Matrix4::Identity) {}
		ViewFrustum(const Matrix4& mat)						{ createFromViewProjMatrix(mat); }
		ViewFrustum(const ViewFrustum& vf)					{ *this=vf; }

		ViewFrustum&		operator=(const ViewFrustum& vf)
		{
			_planeCount=vf._planeCount;
			_position=vf._position;
			_builtFromMatrix=vf._builtFromMatrix;
			_viewProj=vf._viewProj;

			for(int p=0;p<_planeCount;++p)
				_planes[p]=vf._planes[p];

			return *this;
		}

		void				reinit()
		{
			_planeCount=0;
			_builtFromMatrix=false;
			_position=Vector3::NullVector;
			_viewProj.createIdentity();
		}

		void				setPosition(const Vector3& vPos)		{ _position=vPos; };
		const Vector3&		getPosition() const						{ return _position; };

		void				transform(const Matrix4& mat)
		{
			for(int p=0;p<_planeCount;++p)
				_planes[p]*=mat;
			_position*=mat;
		}
		const Matrix4&		getViewProjMatrix() const				{ return _viewProj; }

		void				createFromViewProjMatrix(const Matrix4& mat, bool bNearAndFar=true);

		void				addPlane(const Plane& p)
		{
			assert(_planeCount < FRUSTRUM_MAX_PLANES);
			_planes[_planeCount]=p;
			++_planeCount;
			_builtFromMatrix=false;
		}
		void				addPlane(const Vector3& v1, const Vector3& v2)		{ addPlane(Plane(_position,v1,v2)); }

		int					getPlaneCount() const					{ return _planeCount; }
		const Plane&		getPlane(int n) const					{ return _planes[n]; }

		bool				isPointInside(const Vector3& v) const
		{
			for(int p=0;p<_planeCount;++p)
				if(_planes[p].classifyPoint(v)==PLANE_FRONT)
					return false;

			return true;
		}
		bool				isLineInside(const Vector3& start, const Vector3& end) const
		{
			for(int p=0;p<_planeCount;++p)
				if(_planes[p].classifyPoint(start)==PLANE_FRONT &&
				   _planes[p].classifyPoint(end)==PLANE_FRONT)
				   return false;

			return true;
		}
		bool				isBoxInside(const BBox3& v) const
		{
			for(int p=0;p<_planeCount;++p)
				if(_planes[p].classifyBox(v)==PLANE_FRONT)
					return false;

			return true;
		}
		bool				isSphereInside(const BSphere& s) const
		{
			for(int p=0;p<_planeCount;++p)
				if(_planes[p].classifySphere(s)==PLANE_FRONT)
					return false;

			return true;
		}

		bool				isMatrixCreated() const					{ return _builtFromMatrix; }

		Vector3				getFarLeftUp() const
		{
			Vector3 p;	_planes[VFP_FAR].intersectWithPlanes(_planes[VFP_TOP],_planes[VFP_LEFT],p); return p;
		}

		Vector3				getFarLeftDown() const
		{
			Vector3 p;	_planes[VFP_FAR].intersectWithPlanes(_planes[VFP_BOTTOM],_planes[VFP_LEFT],p); return p;
		}

		Vector3				getFarRightUp() const
		{
			Vector3 p;	_planes[VFP_FAR].intersectWithPlanes(_planes[VFP_TOP],_planes[VFP_RIGHT],p); return p;
		}

		Vector3				getFarRightDown() const
		{
			Vector3 p;	_planes[VFP_FAR].intersectWithPlanes(_planes[VFP_BOTTOM],_planes[VFP_RIGHT],p); return p;
		}

};

}

#endif