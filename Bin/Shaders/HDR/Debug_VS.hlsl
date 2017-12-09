
cbuffer BrightInfos : register(b0)
{
	float4		g_vOffset;
};

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


VS_OUTPUT VSMain(VS_INPUT vsin)
{
	VS_OUTPUT vsout;
	vsout.vPos=vsin.vPos + g_vOffset;
	vsout.vTex=vsin.vUV;

	return vsout;
}

