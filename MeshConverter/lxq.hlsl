cbuffer cbPerObject
{
	float4x4 gWorld;
	float4x4 gBoneTransforms[96];
};

cbuffer cbPerFrame
{
	float4x4 gViewProj;
};

Texture2D gTexture;
SamplerState gSampleState;

struct SkeletonVertexIn
{
	float3 PosL : POSITION;
	float3 NormalL : NORMAL;
	float2 Tex : TEXCOORD;
	uint4  BoneIndices : BONEINDICES;
	float3 Weights : WEIGHTS;
};

struct VertexOut
{
	float4 PosH : SV_POSITION;
	float2 Tex : TEXCOORD;
};

VertexOut skeleton_vs_main(SkeletonVertexIn vin)
{
	VertexOut vout;
	float4 PosW = mul(float4(vin.PosL, 1.0f), gWorld);
	vout.PosH = mul(PosW, gViewProj);
	vout.Tex = vin.Tex;
	return vout;
}

float4 ps_main(VertexOut pin) : SV_TARGET
{
	return gTexture.Sample(gSampleState, pin.Tex);
}

