#include "BindableFactory.h"


BindableMap BindableFactory::s_pBoxBindablesMap = BindableMap();
BindableMap BindableFactory::s_pBallBindablesMap = BindableMap();


std::vector<std::shared_ptr<IBindable>> BindableFactory::GetBoxBindables(std::shared_ptr<Graphics> pGfx, uint32_t& indexCount)
{
	if (s_pBoxBindablesMap.find(pGfx) != s_pBoxBindablesMap.end())
	{
		const auto& bindableGroup = s_pBoxBindablesMap[pGfx];
		
		indexCount = bindableGroup.first;
		return bindableGroup.second;
	}
	
	s_pBoxBindablesMap.insert({ pGfx, { 0u, std::vector<std::shared_ptr<IBindable>>() } });
	auto& newBindableGroup = s_pBoxBindablesMap[pGfx];


	std::vector<DirectX::XMFLOAT3> vertices;
	std::vector<uint16_t> indices;
	GeometryFactory::GenerateCubeData(vertices, indices);

	/// Vertex Buffer
	newBindableGroup.second.push_back(std::make_shared<VertexBuffer>(pGfx, vertices));

	/// Index Buffer
	newBindableGroup.first = indices.size();
	newBindableGroup.second.push_back(std::make_shared<IndexBuffer>(pGfx, indices));


	/// Vertex Shader
	Microsoft::WRL::ComPtr<ID3DBlob> pVertexShaderBlob = nullptr;
	{
		std::shared_ptr<VertexShader> pVertexShader = std::make_shared<VertexShader>(pGfx, L"Source/SimpleCubeVS.cso");
		pVertexShaderBlob = pVertexShader->GetBlob();
		newBindableGroup.second.push_back(std::move(pVertexShader));
	}


	/// Pixel Shader
	newBindableGroup.second.push_back(std::make_shared<PixelShader>(pGfx, L"Source/SimpleCubePS.cso"));


	/// Pixel Constant Buffer
	{
		struct ColorArray {
			struct { DirectX::XMFLOAT4 color; } faceColors[6];
		};
		const ColorArray colorArray
		{
			{
				DirectX::XMFLOAT4{ 1.0f, 0.0f, 0.0f, 1.0f },
				DirectX::XMFLOAT4{ 0.0f, 1.0f, 0.0f, 1.0f },
				DirectX::XMFLOAT4{ 0.0f, 0.0f, 1.0f, 1.0f },
				DirectX::XMFLOAT4{ 1.0f, 1.0f, 0.0f, 1.0f },
				DirectX::XMFLOAT4{ 1.0f, 0.0f, 1.0f, 1.0f },
				DirectX::XMFLOAT4{ 0.0f, 1.0f, 1.0f, 1.0f }
			}
		};
		newBindableGroup.second.push_back(std::make_shared<PixelConstantBuffer<ColorArray>>(pGfx, colorArray));
	}


	/// Input Layout
	std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDescription
	{
		{
			"UV_Position",					// SemanticName (Vertex shader)
			0,								// SemanticIndex (Vertex shader)

			DXGI_FORMAT_R32G32B32_FLOAT,	// Format (3 floats)
			0,								// InputSlot (keep it zero for now)
			0,								// AlignedByteOffset

			D3D11_INPUT_PER_VERTEX_DATA,	// InputSlotClass
			NULL							// InstanceDataStepRate
		}
	};
	{
		newBindableGroup.second.push_back(std::make_shared<InputLayout>(pGfx, inputLayoutDescription, pVertexShaderBlob));
	}


	/// Topology
	{
		newBindableGroup.second.push_back(std::make_shared<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}


	indexCount = newBindableGroup.first;
	return newBindableGroup.second;
}

std::vector<std::shared_ptr<IBindable>> BindableFactory::GetBallBindables(std::shared_ptr<Graphics> pGfx, uint32_t& indexCount)
{
	if (s_pBallBindablesMap.find(pGfx) != s_pBallBindablesMap.end())
	{
		const auto& bindableGroup = s_pBallBindablesMap[pGfx];
		
		indexCount = bindableGroup.first;
		return bindableGroup.second;
	}
	

	s_pBallBindablesMap.insert({ pGfx, { 0u, std::vector<std::shared_ptr<IBindable>>() } });
	auto& newBindableGroup = s_pBallBindablesMap[pGfx];

	std::vector<DirectX::XMFLOAT3> vertices;
	std::vector<uint16_t> indices;
	GeometryFactory::GenerateSphereData(vertices, indices);

	/// Vertex Buffer
	newBindableGroup.second.push_back(std::make_shared<VertexBuffer>(pGfx, vertices));

	/// Index Buffer
	newBindableGroup.first = indices.size();
	newBindableGroup.second.push_back(std::make_shared<IndexBuffer>(pGfx, indices));


	/// Vertex Shader
	Microsoft::WRL::ComPtr<ID3DBlob> pVertexShaderBlob = nullptr;
	{
		std::shared_ptr<VertexShader> pVertexShader = std::make_shared<VertexShader>(pGfx, L"Source/SimpleSphereVS.cso");
		pVertexShaderBlob = pVertexShader->GetBlob();
		newBindableGroup.second.push_back(std::move(pVertexShader));
	}


	/// Pixel Shader
	newBindableGroup.second.push_back(std::make_shared<PixelShader>(pGfx, L"Source/SimpleSpherePS.cso"));


	/// Input Layout
	std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDescription
	{
		{
			"UV_Position",					// SemanticName (Vertex shader)
			0,								// SemanticIndex (Vertex shader)

			DXGI_FORMAT_R32G32B32_FLOAT,	// Format (3 floats)
			0,								// InputSlot (keep it zero for now)
			0,								// AlignedByteOffset

			D3D11_INPUT_PER_VERTEX_DATA,	// InputSlotClass
			NULL							// InstanceDataStepRate
		}
	};
	newBindableGroup.second.push_back(std::make_shared<InputLayout>(pGfx, inputLayoutDescription, pVertexShaderBlob));


	/// Topology
	newBindableGroup.second.push_back(std::make_shared<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));


	indexCount = newBindableGroup.first;
	return newBindableGroup.second;
}