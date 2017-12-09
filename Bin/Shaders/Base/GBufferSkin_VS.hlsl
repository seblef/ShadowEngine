
#include "../SceneInfos.hlsl"
#include "../Common.hlsl"
#include "../SkinCommon.hlsl"


VS_OUTPUT VSMain(VS_INPUTSkin vsin)
{
	VS_OUTPUT vsout;

	int4 iIndices=D3DCOLORtoUBYTE4(vsin.fIndices);
	const float3x3 mRot=(const float3x3)mWorld;

	const float4 vLocalPos=SGetLocalPosition(vsin,iIndices);
	
	const float3 vNormal=SGetLocalNormalBinormalTangent(vsin, vsin.vNorm,mRot,iIndices);

	const float3 vTangent=SGetLocalNormalBinormalTangent(vsin, vsin.vTangent,mRot,iIndices);
	const float3 vBinormal=SGetLocalNormalBinormalTangent(vsin, vsin.vBinormal,mRot,iIndices);

	vsout.mTangent[0]=float3(vTangent.x,vBinormal.x,vsout.vNormal.x);
	vsout.mTangent[1]=float3(vTangent.y,vBinormal.y,vsout.vNormal.y);
	vsout.mTangent[2]=float3(vTangent.z,vBinormal.z,vsout.vNormal.z);

	vsout.vPos=mul(mViewProj,vLocalPos);
	vsout.vNormal=vNormal;
	vsout.vWorldPos=vsout.vPos;

	float2 vUV=float2(dot(vUVRotX,vsin.vTex0),dot(vUVRotY,vsin.vTex0));
	vUV*=vUVScale;
	vUV+=vUVOffset;

	vsout.vTexBase=vsin.vTex0;//vUV;

	return vsout;
}