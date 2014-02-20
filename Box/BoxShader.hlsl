
/*
struct SLightData
{
XMFLOAT4	Diffuse;
XMFLOAT4	Specular;
XMFLOAT4	Ambient;
XMFLOAT3	Position;
f32			Range;
XMFLOAT3	Direction;
f32			Falloff;
XMFLOAT3	Attenuations;
f32			Theta;
};
*/

struct Light
{
	float4 Diffuse;
	float4 Specular;
	float4 Ambient;
	float3 Position;
	float  Range;
	float3 Direction;
	float  Falloff;
	float3 Attenuations;
	float  Theta;
};

cbuffer cbPerObject
{
	float4x4 gWorld;
	float4   gEyePos;
};

cbuffer cbPerFrame
{
	float4x4 gViewProj;
	Light	 gLight;
};

struct VertexIn
{
	float3 PosL		: POSITION;
	float3 Normal	: NORMAL;
	float3 Tangent	: TANGENT;
	float2 Tex		: TEXCOORD;
};

struct VertexOut
{
	float4 PosH		: SV_POSITION;
	float3 PosW		: POSITION;
	float3 Normal	: NORMAL; 
	float2 Tex		: TEXCOORD;
};

VertexOut vs_main(VertexIn vin)
{
	VertexOut vout;
	float4 PosW = mul(float4(vin.PosL, 1.0f), gWorld);
	vout.PosH = mul(PosW, gViewProj);
	vout.PosW = PosW.xyz;
	vout.Normal = mul(vin.Normal, (float3x3)gWorld);
	vout.Tex = vin.Tex;

	return vout;
}

