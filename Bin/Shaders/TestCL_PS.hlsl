

struct PS_INPUT
{
	float4 vPos : SV_POSITION;

};


float4 PSMain(PS_INPUT psin) : SV_Target
{
	return float4(0.8f,0,0,1);
}

