#include "GeometryFactory.h"

void GeometryFactory::GenerateCubeData(std::vector<DirectX::XMFLOAT3>& vertices, std::vector<uint16_t>& indices, float side) noexcept
{
	const float halfSide = 0.5f * side;
	
	vertices = std::vector<DirectX::XMFLOAT3>
	{
		DirectX::XMFLOAT3{ -halfSide, -halfSide, -halfSide },
		DirectX::XMFLOAT3{ +halfSide, -halfSide, -halfSide },
		DirectX::XMFLOAT3{ -halfSide, +halfSide, -halfSide },
		DirectX::XMFLOAT3{ +halfSide, +halfSide, -halfSide },
		DirectX::XMFLOAT3{ -halfSide, -halfSide, +halfSide },
		DirectX::XMFLOAT3{ +halfSide, -halfSide, +halfSide },
		DirectX::XMFLOAT3{ -halfSide, +halfSide, +halfSide },
		DirectX::XMFLOAT3{ +halfSide, +halfSide, +halfSide },
	};

	indices = std::vector<uint16_t>
	{
		0,2,1, 2,3,1,
		1,3,5, 3,7,5,
		2,6,3, 3,6,7,
		4,5,7, 4,7,6,
		0,4,2, 2,4,6,
		0,1,4, 1,5,4
	};
}

void GeometryFactory::GenerateSphereData(std::vector<DirectX::XMFLOAT3>& vertices, std::vector<uint16_t>& indices, float radius, uint16_t lattitudeDivisions, uint16_t longitudeDivisions) noexcept
{
	const DirectX::XMVECTOR base = DirectX::XMVectorSet(0.0f, 0.0f, radius, 0.0f);

	const float lattitudeDeltaAngle = DirectX::XM_PI / lattitudeDivisions;
	const float longitudeDeltaAngle = DirectX::XM_2PI / longitudeDivisions;


	/// Vertices
	for (uint16_t i = 0u; i <= lattitudeDivisions; i++) // Sweep sphere surface & generate vertices
	{
		// For the near & far poles (special cases)
		if (i == 0u || i == lattitudeDivisions)
		{
			vertices.emplace_back();
			DirectX::XMStoreFloat3(&vertices.back(), i == 0 ? base : DirectX::XMVectorNegate(base));

			continue;
		}

		// Angle around x-axis
		const float lattitudeAngle = lattitudeDeltaAngle * i;
		const DirectX::XMVECTOR lattitudeBase = DirectX::XMVector3Transform(base, DirectX::XMMatrixRotationX(lattitudeAngle));

		for (uint16_t j = 0u; j < longitudeDivisions; j++)
		{
			// Angle around z-axis
			const float longitudeAngle = longitudeDeltaAngle * j;
			const DirectX::XMVECTOR newVertex = DirectX::XMVector3Transform(lattitudeBase, DirectX::XMMatrixRotationZ(longitudeAngle));

			vertices.emplace_back();
			DirectX::XMStoreFloat3(&vertices.back(), newVertex);
		}
	}
	

	/// Indices
	const auto calculateIndex = [longitudeDivisions](uint16_t lattitudeIndex, uint16_t longitudeIndex)
	{
		return (lattitudeIndex - 1) * longitudeDivisions + longitudeIndex + 1u;
	};
	for (uint16_t i = 0u; i < lattitudeDivisions; i++)
	{
		if (i == 0u) // far pole triangles
		{
			for (uint16_t j = 0u; j < longitudeDivisions; j++)
			{
				indices.push_back(0);
				indices.push_back(calculateIndex(i + 1, j));
				indices.push_back(calculateIndex(i + 1, (j + 1) % longitudeDivisions));
			}
		}
		else if (i == lattitudeDivisions) // near pole triangles
		{
			for (uint16_t j = 0u; j < longitudeDivisions; j++)
			{
				indices.push_back(calculateIndex(i, (j + 1) % longitudeDivisions));
				indices.push_back(calculateIndex(i, j));
				indices.push_back((uint16_t)vertices.size());
			}
		}
		else
		{
			for (uint16_t j = 0u; j < longitudeDivisions; j++)
			{
				indices.push_back(calculateIndex(i, j));
				indices.push_back(calculateIndex(i + 1, j));
				indices.push_back(calculateIndex(i, (j + 1) % longitudeDivisions));

				indices.push_back(calculateIndex(i, (j + 1) % longitudeDivisions));
				indices.push_back(calculateIndex(i + 1, j));
				indices.push_back(calculateIndex(i + 1, (j + 1) % longitudeDivisions));
			}
		}
	}
}
