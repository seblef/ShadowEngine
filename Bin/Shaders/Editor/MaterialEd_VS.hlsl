
#include "MaterialEd.hlsl"


VS_OUTPUT VSMain(VS_INPUT vsin)
{
	VS_OUTPUT vsout;

	vsout.vPos=mul(mViewProj,vsin.vPos);
	vsout.vNormal=vsin.vNorm;

	vsout.vWorldPos=vsout.vPos;

	float2 vUV=float2(dot(vUVRotX,vsin.vTex0),dot(vUVRotY,vsin.vTex0));
	vUV*=vUVScale;
	vUV+=vUVOffset;

	vsout.vTexBase=vUV;

	float3 vTangent=vsin.vTangent;
	float3 vBinormal=vsin.vBinormal;

	vsout.mTangent[0]=float3(vTangent.x,vBinormal.x,vsout.vNormal.x);
	vsout.mTangent[1]=float3(vTangent.y,vBinormal.y,vsout.vNormal.y);
	vsout.mTangent[2]=float3(vTangent.z,vBinormal.z,vsout.vNormal.z);

	return vsout;
}