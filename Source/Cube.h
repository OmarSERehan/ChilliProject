#pragma once
#include "IDrawable.h"
#include "BindableFactory.h"
#include "ConstantBuffer.h"
#include "TransformMatrixCBuffer.h"
#include "Topology.h"
#include "Viewport.h"

class Cube : public IDrawable
{
public:
	Cube(
		std::shared_ptr<Graphics> pGfx,
		std::mt19937& rng,
		std::uniform_real_distribution<float>& rDistribution,
		std::uniform_real_distribution<float>& grDistribution,
		std::uniform_real_distribution<float>& laDistribution,
		std::uniform_real_distribution<float>& gaDistribution
	);

	virtual void Update(float deltaTime) noexcept override;

	virtual DirectX::XMMATRIX GetModelMatrix() const noexcept override;


private:
	float m_radius = 0.0f;
	
	/// Rotation around local origin
	float m_pitch = 0.0f;
	float m_yaw = 0.0f;
	float m_roll = 0.0f;
	
	/// Rotation around world origin
	float m_theta = 0.0f;
	float m_phi = 0.0f;
	float m_chi = 0.0f;

	/// Angular speed around local origin
	float m_deltaPitch = 0.0f;
	float m_deltaYaw = 0.0f;
	float m_deltaRoll = 0.0f;

	/// Angular speed around world origin
	float m_deltaTheta = 0.0f;
	float m_deltaPhi = 0.0f;
	float m_deltaChi = 0.0f;


public:
	Cube(const Cube&) = delete;
	Cube& operator = (const Cube&) = delete;

	Cube(Cube&&) = delete;
	Cube& operator = (Cube&&) = delete;
};

