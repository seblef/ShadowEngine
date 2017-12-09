
#include "../SceneInfos.hlsl"
#include "ShadowsCommon.hlsl"
#include "../SkinCommon.hlsl"


VS_OUTPUT VSMain(VS_INPUTSkin vsin)
{
	VS_OUTPUT vsout;

	int4 iIndices=D3DCOLORtoUBYTE4(vsin.fIndices);

	const float4 vLocalPos=SGetLocalPosition(vsin,iIndices);

	vsout.vPos=mul(mViewProj,vLocalPos);
	vsout.vZ.x=vsout.vPos.z;
	vsout.vZ.y=vsout.vPos.w;

	return vsout;
}