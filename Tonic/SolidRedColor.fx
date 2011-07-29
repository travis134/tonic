float4 VS_Main(float4 position : POSITION) : SV_POSITION
{
	return position;
}

float4 PS_Main(float4 position : SV_POSITION) : SV_TARGET
{
	return float4(1.0f, 0.0f, 0.0f, 1.0f);
}