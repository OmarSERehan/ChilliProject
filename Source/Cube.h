#pragma once
#include "IDrawable.h"
#include "IBindable.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "InputLayout.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ConstantBuffer.h"
#include "TransformMatrixCBuffer.h"
#include "Topology.h"
#include "Viewport.h"

class Cube : public IDrawable
{
public:
	static std::unique_ptr<Cube> CreateObject(
		Graphics* gfx,
		std::mt19937& rng,
		std::uniform_real_distribution<float>& rDistribution,
		std::uniform_real_distribution<float>& grDistribution,
		std::uniform_real_distribution<float>& laDistribution,
		std::uniform_real_distribution<float>& gaDistribution
	) noexcept;
	bool DestroyObject() noexcept;

	virtual void Update(float deltaTime) noexcept override;

	virtual DirectX::XMMATRIX GetModelMatrix() const noexcept override;


private:
	float m_radius;
	void SetRadius(float radius) noexcept;
	
	/// Rotation around local origin
	float m_pitch = 0.0f;
	float m_yaw = 0.0f;
	float m_roll = 0.0f;
	void SetLocalRotation(float pitch, float yaw, float roll) noexcept;
	
	/// Rotation around world origin
	float m_theta = 0.0f;
	float m_phi = 0.0f;
	float m_chi = 0.0f;
	void SetGlobalRotation(float theta, float phi, float chi) noexcept;


	/// Angular speed around local origin
	float m_deltaPitch = 0.0f;
	float m_deltaYaw = 0.0f;
	float m_deltaRoll = 0.0f;
	void SetLocalAngularSpeed(float deltaPitch, float deltaYaw, float deltaRoll) noexcept;

	/// Angular speed around world origin
	float m_deltaTheta = 0.0f;
	float m_deltaPhi = 0.0f;
	float m_deltaChi = 0.0f;
	void SetGlobalAngularSpeed(float deltaTheta, float deltaPhi, float deltaChi) noexcept;


public:
	Cube() = default;
	~Cube() = default;

	Cube(const Cube&) = delete;
	Cube& operator = (const Cube&) = delete;
};

