
#ifndef _HDRCOMMON_H_
#define _HDRCOMMON_H_

#include "../SceneInfos.hlsl"

cbuffer BrightInfos : register(b4)
{
	float2		g_vInvBloomTex;
	float2		g_vGreyScaleUV;
	float		g_fBrightThreshold;
	float		g_fExposure;
	float		g_fGaussianScalar;
	float		fBrightPack;
}

struct VS_INPUT
{
	float4 vPos : POSITION;
	float4 cColor : COLOR0;
	float2 vUV : TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 vPos : SV_POSITION;
	float2 vTex : TEXCOORD0;
};

struct PS_INPUT
{
	float4 vPos : SV_POSITION;
	float2 vTex : TEXCOORD0;
	float4 vPos2 : SV_Position;
};

SamplerState		sPoint			: register(s0);

Texture2D			tSrc			: register(t0);

static const float g_fGaussianWeights[9]={
	0.091325f,
	0.128537f,
	0.164080f,
	0.189965f,
	0.199471f,
	0.189965f,
	0.164080f,
	0.128537f,
	0.091325f
};

//static const float3 g_vLuminanceVector  = float3(0.2125f, 0.7154f, 0.0721f);
static const float3 g_vLuminanceVector  = float3(0.30f,0.59f,0.11f);


#endif