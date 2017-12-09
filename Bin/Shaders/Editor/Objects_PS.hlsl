

struct PS_INPUT
{
	float4 vPos : SV_POSITION;
	float2 uv : TEXCOORD0;
};

SamplerState sLinear : register( s0 );

Texture2D	tDiffuse : register( t0 );


float4 PSMain(PS_INPUT psin) : SV_Target
{
	return tDiffuse.Sample(sLinear,psin.uv);
}

