
#include "../SceneInfos.hlsl"
#include "LightCommon.hlsl"
#include "LightInfos.hlsl"

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

	const float fDotDirPointToLight=-dot(vLightDir.xyz,vPointToLight);
	if(fDotDirPointToLight < fFarAngle || fDistance > fRange)
		discard;

	fFinalFact=saturate((fDotDirPointToLight - fFarAngle) / (fNearAngle - fFarAngle));
	
	float4 vNormalFull=tNormal.Sample(sPoint,vCoord);
	float3 vNormal=vNormalFull.xyz * 2.0f - 1.0f;

	const float fDiffuseFact=dot(vNormal.xyz,vPointToLight);
	if(fDiffuseFact < 0.0f)
		discard;

	float fSpec, fSpecInt;
	fSpecInt = vNormalFull.a * 10.0f;
	fSpec = vNormalFull.a * 255.0f;

	float fAtten = GetRangeFact(fDistance) * fFinalFact;
	float fSpecularVal = GetSpecularFact(vNormal, vPointToLight, vPos.xyz, fSpecInt, fSpec);

	float3 cDiffuse = cLightColor.rgb * fDiffuseFact * fAtten * cLightColor.a;
		float fSpecLuminance = getLuminance(cLightColor.rgb * fSpecularVal * cLightColor.a * fFinalFact);

	return float4(cDiffuse, fSpecLuminance);
}