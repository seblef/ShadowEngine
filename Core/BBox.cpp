
#include "BBox2.h"
#include "BBox3.h"

using namespace Core;

BBox2	BBox2::NullBox(0,0,0,0);
BBox2	BBox2::BoundBox(WBOUNDS_N,WBOUNDS_N,WBOUNDS_P,WBOUNDS_P);

BBox3	BBox3::NullBox(0,0,0,0,0,0);
BBox3	BBox3::BoundBox(WBOUNDS_N,WBOUNDS_N,WBOUNDS_N,WBOUNDS_P,WBOUNDS_P,WBOUNDS_P);
BBox3	BBox3::InitBox(WBOUNDS_P, WBOUNDS_P, WBOUNDS_P, WBOUNDS_N, WBOUNDS_N, WBOUNDS_N);

bool BBox3::rayIntersect(const Vector3& o, const Vector3& d) const
{
	Vector3 vAbsSegDir,c,s;
	float f1,f2;

	getCenter(c);
	getSize(s);

	const Vector3 vDiff(o-c);

	if(vDiff.x*d.x >= 0.0f && FloatMakePositive(vDiff.x) > s.x) return false;
	if(vDiff.y*d.y >= 0.0f && FloatMakePositive(vDiff.y) > s.y) return false;
	if(vDiff.z*d.z >= 0.0f && FloatMakePositive(vDiff.z) > s.z) return false;

	vAbsSegDir.x=FloatMakePositive(d.x);
	vAbsSegDir.y=FloatMakePositive(d.y);
	vAbsSegDir.z=FloatMakePositive(d.z);

	f1=FloatMakePositive(d.y * vDiff.z - d.z * vDiff.y);
	f2=s.y * vAbsSegDir.z + s.z * vAbsSegDir.y;
	if(f1 > f2) return false;

	f1=FloatMakePositive(d.z * vDiff.x - d.x * vDiff.z);
	f2=s.x * vAbsSegDir.z + s.z * vAbsSegDir.x;
	if(f1 > f2) return false;

	f1=FloatMakePositive(d.x * vDiff.y - d.y * vDiff.x);
	f2=s.x * vAbsSegDir.y + s.y * vAbsSegDir.x;
	if(f1 > f2) return false;

	return true;
}
