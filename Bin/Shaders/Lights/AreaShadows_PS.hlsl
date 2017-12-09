
#include "../SceneInfos.hlsl"
#include "LightCommon.hlsl"
#include "LightInfos.hlsl"
#include "ShadowCommon.hlsl"


struct PS_INPUT
{
	float4 vPos : SV_POSITION;
	float4 vPos2 : SV_Position;
};

float4 PSMain(PS_INPUT psin) : SV_Target
{
	float2 vCoord=(psin.vPos2.xy - float2(0.5f,0.5f)) * vInvScreenSize;
	float fDepth=tDepth.Sample(sPoint,vCoord).r;

	float4 vPos=GetWorldPosition(vCoord,fDepth);

	float3 vPointToLight=vLightPos.xyz - vPos.xyz;
	const float fDistance=length(vPointToLight);
	vPointToLight/=fDistance;

	float fFinalFact=1.0f;

	float4 vPosInLightSpace=mul(mAreaViewProj,vPos);
	vPosInLightSpace.xyz/=vPosInLightSpace.w;

	if(vPosInLightSpace.x < -1.0f || vPosInLightSpace.x > 1.0f ||
		vPosInLightSpace.y < -1.0f || vPosInLightSpace.y > 1.0f || 
		vPosInLightSpace.z < 0.0f || vPosInLightSpace.z > 1.0f)
		discard;

	float4 afact;		// x=left border; y=bottom border;	z=right border;	w=top border
	afact.xy=vPosInLightSpace.xy + 1.0f;
	afact.zw=1.0f - vPosInLightSpace.xy;
	afact=saturate(afact * fNearAngle);
	fFinalFact=afact.x * afact.y * afact.z * afact.w;

	float4 vNormalFull=tNormal.Sample(sPoint,vCoord);
	float3 vNormal=vNormalFull.xyz * 2.0f - 1.0f;

	const float fDiffuseFact=dot(vNormal.xyz,vPointToLight);
	if(fDiffuseFact < 0.0f)
		discard;

	fFinalFact*=GetShadowFactPoisson(vPos);

	float fSpec, fSpecInt;
	fSpecInt = vNormalFull.a * 10.0f;
	fSpec = vNormalFull.a * 255.0f;

	float fAtten = GetRangeFact(fDistance) * fFinalFact;
	float fSpecularVal = GetSpecularFact(vNormal, vPointToLight, vPos.xyz, fSpecInt, fSpec);

	float3 cDiffuse = cLightColor.rgb * fDiffuseFact * fAtten * cLightColor.a;
	float fSpecLuminance = getLuminance(cLightColor.rgb * fSpecularVal * cLightColor.a * fFinalFact);

	return float4(cDiffuse, fSpecLuminance);
}