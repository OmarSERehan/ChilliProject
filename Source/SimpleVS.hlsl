
struct VSOut
{
	float4 oColor : COLOR0;
	float4 oPosition : SV_POSITION;
};

cbuffer constBuffer
{
	matrix transform;
};

VSOut main(float2 iPosition : POSITION0, float4 iColor : COLOR0)
{
	VSOut output;
	
	output.oColor = iColor;
	output.oPosition = mul(float4(iPosition, 0.0f, 1.0f), transform);
	
	return output;
}