
cbuffer constBuffer
{
	float4 faceColors[6];
};

float4 main(uint triangleId : SV_PrimitiveID) : SV_Target
{
	return faceColors[triangleId / 2];
}