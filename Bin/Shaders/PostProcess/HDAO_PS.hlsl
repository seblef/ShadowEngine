
#include "../SceneInfos.hlsl"

#define USE_NORMALS

// Gather defines
#define RING_1    (1)
#define RING_2    (2)
#define RING_3    (3)
#define RING_4    (4)
#define NUM_RING_1_GATHERS    (2)
#define NUM_RING_2_GATHERS    (6)
#define NUM_RING_3_GATHERS    (12)
#define NUM_RING_4_GATHERS    (20)


#define RING_COUNT			2
#define RING_GATHER_COUNT	6

#define NUM_NORMAL_LOADS (4)
static const int2 _normalLoadPattern[NUM_NORMAL_LOADS] =
{
	{ 1, 8 },
	{ 8, -1 },
	{ 5, 4 },
	{ 4, -4 },
};

// Ring sample pattern
static const float2 _HDAORingPattern[NUM_RING_4_GATHERS] =
{
	// Ring 1
	{ 1, -1 },
	{ 0, 1 },

	// Ring 2
	{ 0, 3 },
	{ 2, 1 },
	{ 3, -1 },
	{ 1, -3 },

	// Ring 3
	{ 1, -5 },
	{ 3, -3 },
	{ 5, -1 },
	{ 4, 1 },
	{ 2, 3 },
	{ 0, 5 },

	// Ring 4
	{ 0, 7 },
	{ 2, 5 },
	{ 4, 3 },
	{ 6, 1 },
	{ 7, -1 },
	{ 5, -3 },
	{ 3, -5 },
	{ 1, -7 },
};

// Ring weights
static const float4 _HDAORingWeight[NUM_RING_4_GATHERS] =
{
	// Ring 1 (Sum = 5.30864)
	{ 1.00000, 0.50000, 0.44721, 0.70711 },
	{ 0.50000, 0.44721, 0.70711, 1.00000 },

	// Ring 2 (Sum = 6.08746)
	{ 0.30000, 0.29104, 0.37947, 0.40000 },
	{ 0.42426, 0.33282, 0.37947, 0.53666 },
	{ 0.40000, 0.30000, 0.29104, 0.37947 },
	{ 0.53666, 0.42426, 0.33282, 0.37947 },

	// Ring 3 (Sum = 6.53067)
	{ 0.31530, 0.29069, 0.24140, 0.25495 },
	{ 0.36056, 0.29069, 0.26000, 0.30641 },
	{ 0.26000, 0.21667, 0.21372, 0.25495 },
	{ 0.29069, 0.24140, 0.25495, 0.31530 },
	{ 0.29069, 0.26000, 0.30641, 0.36056 },
	{ 0.21667, 0.21372, 0.25495, 0.26000 },

	// Ring 4 (Sum = 7.00962)
	{ 0.17500, 0.17365, 0.19799, 0.20000 },
	{ 0.22136, 0.20870, 0.24010, 0.25997 },
	{ 0.24749, 0.21864, 0.24010, 0.28000 },
	{ 0.22136, 0.19230, 0.19799, 0.23016 },
	{ 0.20000, 0.17500, 0.17365, 0.19799 },
	{ 0.25997, 0.22136, 0.20870, 0.24010 },
	{ 0.28000, 0.24749, 0.21864, 0.24010 },
	{ 0.23016, 0.22136, 0.19230, 0.19799 },
};

static const float _ringWeightsTotal[RING_4] =
{
	5.30864,
	11.39610,
	17.92677,
	24.93639,
};

cbuffer HDAOBuffer : register(b1)
{
	float2			_renderTargetSize;
	float			_acceptAngle;
	float			_q;
	float			_qTimesNear;
	float			_normalScale;
	float			_HDAOAcceptRadius;
	float			_HDAORejectRadius;
	float			_HDAOIntensity;
	float3			_HDAOPack;
};

struct PS_INPUT
{
	float4 pos		: SV_POSITION;
	float2 uv		: TEXCOORD0;
};

SamplerState sPoint : register(s0);
SamplerState sTrilinear : register(s3);

Texture2D tNormal : register(t1);
Texture2D tDepth : register(t2);

float4 gatherZSamples(float2 texCoord)
{
//	float4 _gather = tDepth.Gather(sPoint, texCoord);
	float4 _gather;
	_gather.x = tDepth.SampleLevel(sPoint, texCoord, 0, int2(0, 1)).x;
	_gather.y = tDepth.SampleLevel(sPoint, texCoord, 0, int2(1, 1)).x;
	_gather.z = tDepth.SampleLevel(sPoint, texCoord, 0, int2(1, 0)).x;
	_gather.w = tDepth.SampleLevel(sPoint, texCoord, 0, int2(0, 0)).x;

	return -_qTimesNear.xxxx / (_gather - _q.xxxx);
}

float normalRejectionTest(int2 screenCoord, int2 screenSize)
{
	int2	mirrorPattern;
	int2	offsetScreenCoord;
	int2	mirrorOffsetScreenCoord;
	float3	n1, n2;
	float	dotVal;
	float	sumDot = 0.0f;

	for (int normal = 0; normal<NUM_NORMAL_LOADS; ++normal)
	{
		mirrorPattern = (_normalLoadPattern[normal] + int2(1, 1)) * int2(-1, -1);
		offsetScreenCoord = screenCoord + _normalLoadPattern[normal];
		mirrorOffsetScreenCoord = screenCoord + mirrorPattern;

		offsetScreenCoord = clamp(offsetScreenCoord, 0, screenSize);
		mirrorOffsetScreenCoord = clamp(mirrorOffsetScreenCoord, 0, screenSize);

		n1 = tNormal.Load(int3(offsetScreenCoord, 0)).xyz * 2.0f - 1.0f;
		n2 = tNormal.Load(int3(mirrorOffsetScreenCoord, 0)).xyz * 2.0f - 1.0f;

		dotVal = dot(n1, n2);
		sumDot += (dotVal > _acceptAngle) ? 0.0f : (1.0f - (abs(dotVal) * 0.25f));
	}

	return (0.5f + sumDot * 0.25f);
}

float4 gatherNormals(float2 texCoord)
{
	float4 ret;
	float3 n = tNormal.SampleLevel(sPoint, texCoord,0, int2(0, 1)).xyz * 2.0f - 1.0f;
	n = normalize(mul((float3x3)mView, n));
	ret.x = n.z;

	n = tNormal.SampleLevel(sPoint, texCoord, 0,int2(1, 1)).xyz * 2.0f - 1.0f;
	n = normalize(mul((float3x3)mView, n));
	ret.y = n.z;

	n = tNormal.SampleLevel(sPoint, texCoord, 0, int2(1, 0)).xyz * 2.0f - 1.0f;
	n = normalize(mul((float3x3)mView, n));
	ret.z = n.z;

	n = tNormal.SampleLevel(sPoint, texCoord,0, int2(0, 0)).xyz * 2.0f - 1.0f;
	n = normalize(mul((float3x3)mView, n));
	ret.w = n.z;

	return ret;
}


float4 PSMain(PS_INPUT psin) : SV_Target
{
	float2	kernelScale = float2(_renderTargetSize.x / 1024.0f, _renderTargetSize.y / 768.0f);
	int2	screenCoord = int2(psin.uv * _renderTargetSize);
	int2	screenSize = int2(_renderTargetSize)-int2(1, 1);
	float2	fscreenCoord;
	float2	texCoord;
	float3	centerNormal;
	float	depth, centerZ, centerNormalZ, offsetCenterZ;
	float2	mirrorScreenCoord, mirrorTexCoord;
	float4	sampledZ[2];
	float4	diff,compare[2];
	float4	occlusion = 0;
	float4	sampledNormalZ[2], offsetSampledZ[2];
	float	retOcclusion;
		
	float dotVal = normalRejectionTest(screenCoord,screenSize);

//	return float4(dotVal, dotVal, dotVal, 1.0f);

	if (dotVal > 0.5f)
	{
		fscreenCoord = float2(screenCoord);
		texCoord = fscreenCoord * vInvScreenSize;

		depth = tDepth.SampleLevel(sPoint, texCoord,0).x;
		centerZ = -_qTimesNear / (depth - _q);

//		return float4(depth, 0, 0, 1);

#ifdef USE_NORMALS
		//		Check here if normal must be in world or camera space
		centerNormal = tNormal.SampleLevel(sPoint, texCoord, 0).xyz * 2.0f - 1.0f;
		centerNormal = normalize(mul((float3x3)mView, centerNormal));
		centerNormalZ = centerNormal.z;
		offsetCenterZ = centerZ + centerNormalZ * _normalScale;
#endif

		for (int g = 0; g < RING_GATHER_COUNT; ++g)
		{
			mirrorScreenCoord = ((kernelScale * _HDAORingPattern[g]) + float2(1.0f, 1.0f)) * float2(-1.0f, -1.0f);
			texCoord = float2((fscreenCoord + (kernelScale * _HDAORingPattern[g])) * vInvScreenSize);
			mirrorTexCoord = float2((fscreenCoord + mirrorScreenCoord) * vInvScreenSize);

			sampledZ[0] = gatherZSamples(texCoord);
			sampledZ[1] = gatherZSamples(mirrorTexCoord);

			diff = centerZ.xxxx - sampledZ[0];
			compare[0] = diff < _HDAORejectRadius.xxxx ? 1.0f : 0.0f;
			compare[0] *= diff > _HDAOAcceptRadius.xxxx ? 1.0f : 0.0f;

			diff = centerZ.xxxx - sampledZ[1];
			compare[1] = diff < _HDAORejectRadius.xxxx ? 1.0f : 0.0f;
			compare[1] *= diff > _HDAOAcceptRadius.xxxx ? 1.0f : 0.0f;

			occlusion.xyzw += (_HDAORingWeight[g].xyzw * (compare[0].xyzw * compare[1].zwxy) * dotVal);

#ifdef USE_NORMALS
//			sampledNormalZ[0] = tNormal.GatherBlue(sPoint, texCoord) * 2.0f - 1.0f;
//			sampledNormalZ[1] = tNormal.GatherBlue(sPoint, mirrorTexCoord) * 2.0f - 1.0f;

			sampledNormalZ[0] = gatherNormals(texCoord);
			sampledNormalZ[1] = gatherNormals(mirrorTexCoord);

			offsetSampledZ[0] = sampledZ[0] + (sampledNormalZ[0] * _normalScale);
			offsetSampledZ[1] = sampledZ[1] + (sampledNormalZ[1] * _normalScale);

			diff = offsetCenterZ.xxxx - offsetSampledZ[0];
			compare[0] = diff < _HDAORejectRadius.xxxx ? 1.0f : 0.0f;
			compare[0] *= diff > _HDAOAcceptRadius.xxxx ? 1.0f : 0.0f;

			diff = offsetCenterZ.xxxx - offsetSampledZ[1];
			compare[1] = diff < _HDAORejectRadius.xxxx ? 1.0f : 0.0f;
			compare[1] *= diff > _HDAOAcceptRadius.xxxx ? 1.0f : 0.0f;

			occlusion.xyzw += (_HDAORingWeight[g].xyzw * (compare[0].xyzw * compare[1].zwxy) * dotVal);
#endif
		}
	}

#ifdef USE_NORMALS
	retOcclusion = ((occlusion.x + occlusion.y + occlusion.z + occlusion.w) / (3.0f * _ringWeightsTotal[RING_COUNT - 1]));
#else
	retOcclusion = ((occlusion.x + occlusion.y + occlusion.z + occlusion.w) / (2.0f * _ringWeightsTotal[RING_COUNT - 1]));
#endif

	retOcclusion *= _HDAOIntensity;
	retOcclusion = 1.0f - saturate(retOcclusion);

	return retOcclusion.xxxx;
}
