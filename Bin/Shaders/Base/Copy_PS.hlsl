
cbuffer CopyBuf : register(b0)
{
	float4			cCopyColor;
};

struct PS_INPUT
{
	float4 vPos : SV_POSITION;
	float2 vUV : TEXCOORD0;

};

Texture2D	tDiffuse : register( t0 );
Texture2D	tNormal : register(t1);
Texture2D	tDepth : register(t2);
SamplerState sLinear : register( s0 );


float4 PSMain(PS_INPUT psin) : SV_Target
{
	float4 dif=tDiffuse.Sample(sLinear,psin.vUV) * cCopyColor.r;
	float4 norm=tNormal.Sample(sLinear,psin.vUV) * cCopyColor.g;
	float depth=tDepth.Sample(sLinear,psin.vUV).r * cCopyColor.b;

	return dif + norm + float4(depth,depth,depth,1);
}

