

struct PS_INPUT
{
	float4 vPos : SV_POSITION;
	float4 cColor : COLOR0;
};


float4 PSMain(PS_INPUT psin) : SV_Target
{
	return psin.cColor;
}

