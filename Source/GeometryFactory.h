#pragma once
#include <vector>
#include <DirectXMath.h>

class GeometryFactory
{
public:
	static void GenerateCubeData(std::vector<DirectX::XMFLOAT3>& vertices, std::vector<uint16_t>& indices, float side = 1.0f) noexcept;
	static void GenerateSphereData(std::vector<DirectX::XMFLOAT3>& vertices, std::vector<uint16_t>& indices, float radius = 0.5f, uint16_t lattitudeDivisions = 12u, uint16_t longitudeDivisions = 24u) noexcept;


private:
	GeometryFactory() = delete;
	~GeometryFactory() = delete;

	GeometryFactory(const GeometryFactory&) = delete;
	GeometryFactory& operator = (const GeometryFactory&) = delete;

	GeometryFactory(GeometryFactory&&) = delete;
	GeometryFactory& operator = (GeometryFactory&&) = delete;
};

