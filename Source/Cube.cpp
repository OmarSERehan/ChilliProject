#include "Cube.h"


std::unique_ptr<Cube> Cube::CreateObject(
	Graphics* gfx,
	std::mt19937& rng,
	std::uniform_real_distribution<float>& rDistribution,
	std::uniform_real_distribution<float>& grDistribution,
	std::uniform_real_distribution<float>& laDistribution,
	std::uniform_real_distribution<float>& gaDistribution
) noexcept
{
	std::unique_ptr<Cube> pCube = std::make_unique<Cube>();
	{
		pCube->SetRadius(rDistribution(rng));
		pCube->SetGlobalRotation(grDistribution(rng), grDistribution(rng), grDistribution(rng));
		pCube->SetLocalAngularSpeed(laDistribution(rng), laDistribution(rng), laDistribution(rng));
		pCube->SetGlobalAngularSpeed(gaDistribution(rng), gaDistribution(rng), gaDistribution(rng));
	}
	

	/// Vertex Buffer
	struct Vertex
	{
		struct { float x, y, z; } position;
	};
	const std::vector<Vertex> vertices =
	{
		{ -1.0f, -1.0f, -1.0f},
		{ +1.0f, -1.0f, -1.0f},
		{ -1.0f, +1.0f, -1.0f},
		{ +1.0f, +1.0f, -1.0f},
		{ -1.0f, -1.0f, +1.0f},
		{ +1.0f, -1.0f, +1.0f},
		{ -1.0f, +1.0f, +1.0f},
		{ +1.0f, +1.0f, +1.0f}
	};
	pCube->AddBind(VertexBuffer::CreateObject<Vertex>(gfx, vertices));


	/// Vertex Shader
	Microsoft::WRL::ComPtr<ID3DBlob> pVsBlob = nullptr;
	{
		std::unique_ptr<VertexShader> pVs = VertexShader::CreateObject(gfx, L"Source/SimpleCubeVS.cso");
		pVsBlob = pVs->GetBlob();
		
		pCube->AddBind(std::move(pVs));
	}


	/// Pixel Shader
	pCube->AddBind(PixelShader::CreateObject(gfx, L"Source/SimpleCubePS.cso"));


	/// Index Buffer (CW)
	const std::vector<uint16_t> indices =
	{
		// Front Face
		0, 2, 1,
		2, 3, 1,
		// Back Face
		1, 3, 4,
		3, 7, 5,
		// Right Face
		2, 6, 3,
		3, 6, 7,
		// Left Face
		4, 5, 7,
		4, 7, 6,
		// Top Face
		0, 4, 2,
		2, 4, 6,
		// Bottom Face
		0, 1, 4,
		1, 5, 4
	};
	pCube->AddBind(IndexBuffer::CreateObject(gfx, indices));


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
	pCube->AddBind(PixelConstantBuffer<ColorArray>::CreateObject(gfx, pConstBuffer));


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
	pCube->AddBind(InputLayout::CreateObject(gfx, inputLayoutDescription, pVsBlob));


	/// Topology
	pCube->AddBind(Topology::CreateObject(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));


	/// Transformation Matrix
	pCube->AddBind(TransformMatrixCBuffer::CreateObject(gfx, pCube.get()));


	/// Viewport
	pCube->AddBind(Viewport::CreateObject(800.0f, 600.0f));


	return pCube;
}
bool Cube::DestroyObject() noexcept
{
	return true;
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
		DirectX::XMMatrixTranslation(0.0f, 0.0f, 20.0f);
}


void Cube::SetRadius(float radius) noexcept
{
	m_radius = radius;
}

void Cube::SetLocalRotation(float pitch, float yaw, float roll) noexcept
{
	m_pitch = pitch;
	m_yaw = yaw;
	m_roll = roll;
}
void Cube::SetGlobalRotation(float theta, float phi, float chi) noexcept
{
	m_theta = theta;
	m_phi = phi;
	m_chi = chi;
}

void Cube::SetLocalAngularSpeed(float deltaPitch, float deltaYaw, float deltaRoll) noexcept
{
	m_deltaPitch = deltaPitch;
	m_deltaYaw = deltaYaw;
	m_deltaRoll = deltaRoll;
}
void Cube::SetGlobalAngularSpeed(float deltaTheta, float deltaPhi, float deltaChi) noexcept
{
	m_deltaTheta = deltaTheta;
	m_deltaPhi = deltaPhi;
	m_deltaChi = deltaChi;
}