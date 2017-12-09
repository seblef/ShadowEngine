
#ifndef _LIGHTCOMMON_H_
#define _LIGHTCOMMON_H_

SamplerState sPoint		: register(s0);
SamplerState sTrilinear : register(s3);

Texture2D tAldebo		: register(t0);
Texture2D tNormal		: register(t1);
Texture2D tDepth		: register(t2);
Texture2D tSSAO			: register(t3);
Texture2D tAccumulation : register(t4);
Texture2D tHDAO			: register(t5);

float GetSpecularFact(float3 vNormal, float3 vPointToLight, float3 vPoint, float fSpecInt, float fShine)
{
	float3 r = (vNormal * (2.0f * dot(vNormal, vPointToLight))) - vPointToLight;
	float3 v = normalize(vEyePos.xyz - vPoint);
	return pow(saturate(dot(r,v)),1.0f + fShine) * fSpecInt;
		//	return pow(max(dot(r,v),0),fShine) * fSpecInt;

//		return max(dot(r, v), 0);
}

float getLuminance(float3 rgb)
{
	return 0.2126f * rgb.r + 0.7152f * rgb.g + 0.0722f * rgb.b;
}

float3 getSpecularColor(float3 rgb, float lum)
{
	float rgblum = getLuminance(rgb) + 1e-6f;
	return rgb * lum / rgblum;
}

#endif