

float4 main(float3 iColor : COLOR0) : SV_TARGET
{
	return float4(iColor, 1.0f);
}