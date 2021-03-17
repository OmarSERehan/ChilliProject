#include "Box.h"


Box::Box(
	std::shared_ptr<Graphics> ppGfx,
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
	AddBinds(BindableFactory::GetBoxBindables(ppGfx, m_indexBufferCount));

	/// Transformation Matrix
	AddBind(std::make_shared<TransformMatrixCBuffer>(ppGfx, this));
}

void Box::Update(float deltaTime) noexcept
{
	m_pitch += m_deltaPitch * deltaTime;
	m_yaw += m_deltaYaw * deltaTime;
	m_roll += m_deltaRoll * deltaTime;

	m_theta += m_deltaTheta * deltaTime;
	m_phi += m_deltaPhi * deltaTime;
	m_chi += m_deltaChi * deltaTime;
}

DirectX::XMMATRIX Box::GetModelMatrix() const noexcept
{
	return
		DirectX::XMMatrixRotationRollPitchYaw(m_pitch, m_yaw, m_roll) *
		DirectX::XMMatrixTranslation(m_radius, 0.0f, 0.0f) *
		DirectX::XMMatrixRotationRollPitchYaw(m_theta, m_phi, m_chi) *
		DirectX::XMMatrixTranslation(0.0f, 0.0f, 30.0f);
}