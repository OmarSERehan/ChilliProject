#include "Triangle.h"

Triangle::Triangle(std::shared_ptr<Graphics> pGfx)
{
	AddBind(std::make_unique<PixelShader>(pGfx, L"Source/SimplePS.cso"));
	

	Microsoft::WRL::ComPtr<ID3DBlob> pVertexShaderBlob = nullptr;
	{
		std::unique_ptr<VertexShader> pVertexShader = std::make_unique<VertexShader>(pGfx, L"Source/SimpleVS.cso");
		pVertexShaderBlob = pVertexShader->GetBlob();
		AddBind(std::move(pVertexShader));
	}


	/*struct Vertex
	{
		struct { float x, y; } Position;
		struct { unsigned char r, g, b, a; } Color;
	};
	std::vector<Vertex> vertices
	{
		{ +0.0f, +0.5f,		255, 000, 000, 255 },
		{ +0.5f, -0.5f,		000, 255, 000, 255 },
		{ -0.5f, -0.5f,		000, 000, 255, 255 }
	};
	AddBind(std::make_unique<VertexBuffer>(pGfx, vertices));*/


	std::vector<uint16_t> indices
	{ // must be CW not CCW
		0, 1, 2
	};
	AddBind(std::make_unique<IndexBuffer>(pGfx, indices));


	std::vector<D3D11_INPUT_ELEMENT_DESC> inputElementDescription
	{
		{
			"POSITION",						// SemanticName (Vertex shader)
			0,								// SemanticIndex (Vertex shader)

			DXGI_FORMAT_R32G32_FLOAT,		// Format (2 floats)
			0,								// InputSlot (keep it zero for now)
			0,								// AlignedByteOffset

			D3D11_INPUT_PER_VERTEX_DATA,	// InputSlotClass
			NULL							// InstanceDataStepRate
		},
		{
			"COLOR",						// SemanticName (Vertex shader)
			0,								// SemanticIndex (Vertex shader)

			DXGI_FORMAT_R8G8B8A8_UNORM,		// Format (4 8-bit types normalized to floats)
			0,								// InputSlot (keep it zero for now)
			8u,								// AlignedByteOffset

			D3D11_INPUT_PER_VERTEX_DATA,	// InputSlotClass
			NULL							// InstanceDataStepRate
		}
	};
	AddBind(std::make_unique<InputLayout>(pGfx, inputElementDescription, pVertexShaderBlob.Get()));

	
	AddBind(std::make_unique<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));


	AddBind(std::make_unique<TransformMatrixCBuffer>(pGfx, this));
}

void Triangle::Update(float deltaTime) noexcept
{}

DirectX::XMMATRIX Triangle::GetModelMatrix() const noexcept
{
	return 
		DirectX::XMMatrixRotationZ(5.0f) *
		DirectX::XMMatrixTranslation(0.0f, 0.0f, 1.0f);
}