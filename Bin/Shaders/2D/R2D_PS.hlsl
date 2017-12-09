

struct PS_INPUT
{
	float4 vPos : SV_POSITION;
	float4 cColor : COLOR0;
	float2 vUV : TEXCOORD0;

};

Texture2D	tDiffuse : register( t0 );
SamplerState sLinear : register( s0 );


float4 PSMain(PS_INPUT psin) : SV_Target
{
	return tDiffuse.Sample(sLinear,psin.vUV) * psin.cColor;
}

