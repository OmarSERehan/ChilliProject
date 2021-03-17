
cbuffer constBuffer
{
	matrix transform;
};

float4 main(float3 iPosition : UV_Position0) : SV_Position
{
	return mul(float4(iPosition, 1.0f), transform);
}