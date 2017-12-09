

struct PS_INPUT
{
	float4 pos :		SV_POSITION;
	float4 color :		COLOR0;
	float2 uv :			TEXCOORD0;
};

SamplerState sLinear : register( s0 );

Texture2D	tDiffuse : register( t0 );


float4 PSMain(PS_INPUT psin) : SV_Target
{
	return psin.color * tDiffuse.Sample(sLinear,psin.uv);
}

