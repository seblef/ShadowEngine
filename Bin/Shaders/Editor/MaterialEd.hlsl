
#ifndef _MATERIALED_H_
#define _MATERIALED_H_

cbuffer SceneInfos : register(b0)
{
	float4x4			mViewProj;
	float4				vEyePos;
	float4				vLightDir;
	float4				cLightColor;
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

SamplerState sLinear : register( s0 );

Texture2D	tDiffuse : register( t0 );
Texture2D	tNormal : register( t1 );
Texture2D	tSpecular : register( t2 );

void getAldeboAndNormal(VS_OUTPUT psin, out float4 cAldebo, out float3 vNormal, out float fShine, out float fSpecInt)
{
	cAldebo=cDiffuse * tDiffuse.Sample(sLinear,psin.vTexBase);

	float4 sTex=tSpecular.Sample(sLinear,psin.vTexBase);
	fShine=(sTex.a * 255.0f + vShininess.x) * 0.5f;
	fSpecInt=vShininess.y*10.0f;

	vNormal=normalize((tNormal.Sample(sLinear,psin.vTexBase).rgb - 0.5f) * 2.0f);
	vNormal=mul(psin.mTangent,vNormal);

//	vNormal=psin.vNormal;
}

float getSpecularFact(float3 vNormal, float3 vPointToLight, float3 vPoint, float fSpecInt, float fShine)
{
	float3 r=(vNormal * (2.0f * dot(vNormal,vPointToLight))) - vPointToLight;
	float3 v=normalize(vEyePos.xyz - vPoint);
	return max(fSpecInt * pow(max(dot(r,v),0.0f),fShine),0);
}


#endif