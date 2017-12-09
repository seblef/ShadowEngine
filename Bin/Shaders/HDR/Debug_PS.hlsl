
struct PS_INPUT
{
	float4 vPos : SV_POSITION;
	float2 vTex : TEXCOORD0;
	float4 vPos2 : SV_Position;
};

SamplerState		sPoint			: register(s0);

Texture2D			tSrc			: register(t0);


float4 PSMain(PS_INPUT psin) : SV_Target
{
	return float4(tSrc.Sample(sPoint,psin.vTex).r,0.0f,0.0f,1.0f);
}