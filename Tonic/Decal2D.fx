Texture2D colorMap_ : register(t0);
SamplerState colorSampler_ : register(s0);

struct VS_Input
{
	float4 position : POSITION;
	float2 texcoord : TEXCOORD0;
};

struct PS_Input
{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD0;
};

PS_Input VS_Main(VS_Input vertex)
{
	PS_Input vsOut = (PS_Input)0;
	vsOut.position = vertex.position;
	vsOut.texcoord = vertex.texcoord;
	return vsOut;
}

float4 PS_Main(PS_Input fragment) : SV_TARGET
{
	return colorMap_.Sample(colorSampler_, fragment.texcoord);
}