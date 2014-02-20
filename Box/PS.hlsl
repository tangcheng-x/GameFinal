Texture2D gTexture1;
Texture2D gTexture2[3];

SamplerState sampleType
{
	FILTER = MIN_MAG_MIP_LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};


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

/*
XMFLOAT4 Ambient;
XMFLOAT4 Diffuse;
XMFLOAT4 Specular;
XMFLOAT4 Emissive;
*/
struct Material
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float4 Emissive;
};

cbuffer cbPerObject
{
	float4x4 gWorld;
};

cbuffer cbPerFrame
{
	float4x4	gViewProj;
	Light		gLight;
	Material	gMaterial;
	float3		gEyePos;
};

cbuffer cbNoting
{
	float4x4 gNoMatrix;
};

struct VertexOut
{
	float4 PosH		: SV_POSITION;
	float3 PosW		: POSITION;
	float3 Normal	: NORMAL;
	float2 Tex		: TEXCOORD;
};

float4 ps_main(VertexOut pin) : SV_TARGET
{
	float3 PosW = pin.PosW;
	float3 Normal = normalize(pin.Normal);

	float4 Color = gLight.Ambient * gMaterial.Ambient + gMaterial.Emissive;
	float3 lightDir = PosW - gLight.Position;
	float dist = length(lightDir);
	lightDir /= dist;

	float diffAtten = dot(-lightDir, Normal);
	float4 Specular = 0;
	float4 Diffuse = 0;

	[flatten]
	if (diffAtten > 0)
	{
		float3 toEye = normalize(gEyePos - PosW);
		float3 refLightDir = reflect(lightDir, Normal);

		float specAtten = dot(refLightDir, toEye);
		float distAtten = 1.0 / dot(gLight.Attenuations, float3(1, dist, dist * dist));

		Diffuse = gLight.Diffuse * diffAtten * distAtten * gMaterial.Diffuse;
		Specular = gLight.Specular * pow(saturate(specAtten), 32) * distAtten * gMaterial.Specular;
	}

	float4 texColor = gTexture1.Sample(sampleType, pin.Tex);
	Color = (Color + Diffuse) * texColor + Specular;
	Color.a = 0.5f;
	//float4 color = gTexture1.Sample(sampleType, pin.Tex) * Color;
	return Color;
}
