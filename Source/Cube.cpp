#include "Cube.h"


Cube::Cube(
	Graphics* gfx,
	std::mt19937& rng,
	std::uniform_real_distribution<float>& rDistribution,
	std::uniform_real_distribution<float>& grDistribution,
	std::uniform_real_distribution<float>& laDistribution,
	std::uniform_real_distribution<float>& gaDistribution
)
	:
	m_radius(rDistribution(rng)),

	m_theta(grDistribution(rng)),
	m_phi(grDistribution(rng)),
	m_chi(grDistribution(rng)),

	m_deltaPitch(laDistribution(rng)),
	m_deltaYaw(laDistribution(rng)),
	m_deltaRoll(laDistribution(rng)),

	m_deltaTheta(gaDistribution(rng)),
	m_deltaPhi(gaDistribution(rng)),
	m_deltaChi(gaDistribution(rng))
{
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
	AddBind(std::make_unique<VertexBuffer>(gfx, vertices));


	/// Vertex Shader
	Microsoft::WRL::ComPtr<ID3DBlob> pVsBlob = nullptr;
	{
		std::unique_ptr<VertexShader> pVs = std::make_unique<VertexShader>(gfx, L"Source/SimpleCubeVS.cso");
		pVsBlob = pVs->GetBlob();
		
		AddBind(std::move(pVs));
	}


	/// Pixel Shader
	AddBind(std::make_unique<PixelShader>(gfx, L"Source/SimpleCubePS.cso"));


	/// Index Buffer (CW)
	const std::vector<uint16_t> indices =
	{
		0,2,1, 2,3,1,
		1,3,5, 3,7,5,
		2,6,3, 3,6,7,
		4,5,7, 4,7,6,
		0,4,2, 2,4,6,
		0,1,4, 1,5,4
	};
	AddBind(std::make_unique<IndexBuffer>(gfx, indices));


	/// Color Array
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
	AddBind(std::make_unique<PixelConstantBuffer<ColorArray>>(gfx, pConstBuffer));


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
	AddBind(std::make_unique<InputLayout>(gfx, inputLayoutDescription, pVsBlob));


	/// Topology
	AddBind(std::make_unique<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));


	/// Transformation Matrix
	AddBind(std::make_unique<TransformMatrixCBuffer>(gfx, this));
}

void Cube::Update(float deltaTime) noexcept
{
	m_pitch += m_deltaPitch * deltaTime;
	m_yaw += m_deltaYaw * deltaTime;
	m_roll += m_deltaRoll * deltaTime;

	m_theta += m_deltaTheta * deltaTime;
	m_phi += m_deltaPhi * deltaTime;
	m_chi += m_deltaChi * deltaTime;
}

DirectX::XMMATRIX Cube::GetModelMatrix() const noexcept
{
	return 
		DirectX::XMMatrixRotationRollPitchYaw(m_pitch, m_yaw, m_roll) *
		DirectX::XMMatrixTranslation(m_radius, 0.0f, 0.0f) *
		DirectX::XMMatrixRotationRollPitchYaw(m_theta, m_phi, m_chi) *
		DirectX::XMMatrixTranslation(0.0f, 0.0f, 30.0f);
}