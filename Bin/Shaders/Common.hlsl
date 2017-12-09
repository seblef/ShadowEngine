
#ifndef _GCOMMON_H_
#define _GCOMMON_H_

struct VS_INPUT
{
	float4 vPos : POSITION;
	float3 vNorm : NORMAL;
	float3 vTangent : TANGENT;
	float3 vBinormal : BINORMAL;
	float2 vTex0 : TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 vPos : SV_POSITION;
	float4 vWorldPos : TEXCOORD0;
	float3 vNormal : TEXCOORD1;
	float2 vTexBase : TEXCOORD2;
	float3x3 mTangent : TEXCOORD4;

};

struct PS_OUTPUT
{
	float4 cColor : SV_Target0;
	float4 cNormal : SV_Target1;
};

cbuffer Material : register(b2)
{
	float4				cAmbient;
	float4				cDiffuse;
	float4				cSpecular;
	float4				vShininess;		//	x=Shininess, y=Intensity
	
	float2				vUVOffset;
	float2				vUVScale;
	float2				vUVRotX;
	float2				vUVRotY;
};

SamplerState sLinear : register( s0 );

Texture2D	tDiffuse : register( t0 );
Texture2D	tNormal : register( t1 );
Texture2D	tSpecular : register( t2 );



#endif