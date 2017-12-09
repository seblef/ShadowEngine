
#ifndef _SHADOWCOMMON_H_
#define _SHADOWCOMMON_H_

cbuffer ShadowInfos : register(b3)
{
	float4x4	mLightViewProj;
	float2		vInvShadowMapSize;
	float		fShadowFilter;
	float		fInvShadowFilter;
};

static const float2 poissonDisk[16] = {
  float2( -0.94201624f, -0.39906216f ),
  float2( 0.94558609f, -0.76890725f ),
  float2( -0.094184101f, -0.92938870f ),
  float2( 0.34495938f, 0.29387760f ),
   float2( -0.91588581f, 0.45771432f ), 
   float2( -0.81544232f, -0.87912464f ), 
   float2( -0.38277543f, 0.27676845f ), 
   float2( 0.97484398f, 0.75648379f ), 
   float2( 0.44323325f, -0.97511554f ), 
   float2( 0.53742981f, -0.47373420f ), 
   float2( -0.26496911f, -0.41893023f ), 
   float2( 0.79197514f, 0.19090188f ), 
   float2( -0.24188840f, 0.99706507f ), 
   float2( -0.81409955f, 0.91437590f ), 
   float2( 0.19984126f, 0.78641367f ), 
   float2( 0.14383161f, -0.14100790f ) };


//			Util file for reading shadow maps

Texture2D tShadow : register(t7);

float4 gatherShadow(float2 uv)
{
	float4 _gather;
	_gather.x = tShadow.SampleLevel(sPoint, uv, 0, int2(1, 0)).x;
	_gather.y = tShadow.SampleLevel(sPoint, uv, 0, int2(1, 1)).x;
	_gather.z = tShadow.SampleLevel(sPoint, uv, 0, int2(0, 1)).x;
	_gather.w = tShadow.SampleLevel(sPoint, uv, 0, int2(0, 0)).x;

	return _gather;
}

float GetShadowFact(const float2 uv, const float fLightDist)
{
	const float2 vTexelPos=(uv / vInvShadowMapSize) - 0.5f;
	const float2 vLerps=frac(vTexelPos);

//	float4 fSource=tShadow.Gather(sPoint,uv);
	float4 fSource=gatherShadow(uv);
	fSource=fSource	< fLightDist && fSource < 1.0f ? 0.0f : 1.0f;
	return lerp(lerp(fSource.x,fSource.y,vLerps.x),
				lerp(fSource.z,fSource.w,vLerps.x),
				vLerps.y);
}

float GetShadowFactPoisson(const float4 vPoint)
{
	const float4 vLightRefPos=mul(mLightViewProj,vPoint);

	float2 vShadowUV=0.5f * vLightRefPos.xy  / vLightRefPos.w+ float2(0.5f,0.5f);
	vShadowUV.y=1.0f - vShadowUV.y;

	const float fLightDist=vLightRefPos.z / vLightRefPos.w;
	float fTotal=0.0f;

//	return tShadow.Sample(sPoint,vShadowUV).r;
//	return fLightDist;

//	return tShadow.Sample(sPoint,vShadowUV).r >= fLightDist ? 1.0f : 0.0f;//GetShadowFact(vShadowUV,fLightDist);

	for(int i=0;i<asint(fShadowFilter);++i)
		fTotal+=GetShadowFact(vShadowUV + poissonDisk[i] / 700.0f,fLightDist);

//	return vPoint.xz*0.1f;
//	return GetShadowFact(vShadowUV,fLightDist);
//	return tShadow.Sample(sPoint,vShadowUV).r;

	return fTotal * fInvShadowFilter;
}

#endif