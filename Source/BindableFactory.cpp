#include "BindableFactory.h"


BindableMap BindableFactory::s_pBoxBindablesMap = BindableMap();


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


	/// Vertex Buffer
	struct Vertex
	{
		struct { float x, y, z; } position;
	};
	const std::vector<Vertex> vertices =
	{
		{ -1.0f,-1.0f,-1.0f },
		{ 1.0f,-1.0f,-1.0f },
		{ -1.0f,1.0f,-1.0f },
		{ 1.0f,1.0f,-1.0f },
		{ -1.0f,-1.0f,1.0f },
		{ 1.0f,-1.0f,1.0f },
		{ -1.0f,1.0f,1.0f },
		{ 1.0f,1.0f,1.0f },
	};
	{
		newBindableGroup.second.push_back(std::make_shared<VertexBuffer>(pGfx, vertices));
	}


	/// Vertex Shader
	Microsoft::WRL::ComPtr<ID3DBlob> pVertexShaderBlob = nullptr;
	{
		std::shared_ptr<VertexShader> pVertexShader = std::make_shared<VertexShader>(pGfx, L"Source/SimpleCubeVS.cso");
		pVertexShaderBlob = pVertexShader->GetBlob();
		newBindableGroup.second.push_back(std::move(pVertexShader));
	}


	/// Pixel Shader
	{
		newBindableGroup.second.push_back(std::make_shared<PixelShader>(pGfx, L"Source/SimpleCubePS.cso"));
	}


	/// Index Buffer
	{
		const std::vector<uint16_t> indices =
		{
			0,2,1, 2,3,1,
			1,3,5, 3,7,5,
			2,6,3, 3,6,7,
			4,5,7, 4,7,6,
			0,4,2, 2,4,6,
			0,1,4, 1,5,4
		};
		newBindableGroup.first = indices.size();
		newBindableGroup.second.push_back(std::make_shared<IndexBuffer>(pGfx, indices));
	}


	/// Pixel Constant Buffer
	{
		struct ColorArray {
			struct { float r, g, b, a; } faceColors[6];
		};
		const ColorArray pConstBuffer
		{
			{
				{ 1.0f, 0.0f, 0.0f },
				{ 0.0f, 1.0f, 0.0f },
				{ 0.0f, 0.0f, 1.0f },
				{ 1.0f, 1.0f, 0.0f },
				{ 1.0f, 0.0f, 1.0f },
				{ 0.0f, 1.0f, 1.0f }
			}
		};
		newBindableGroup.second.push_back(std::make_shared<PixelConstantBuffer<ColorArray>>(pGfx, pConstBuffer));
	}


	/// Input Layout
	std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDescription =
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