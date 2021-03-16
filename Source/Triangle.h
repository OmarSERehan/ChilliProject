#pragma once
#include "IDrawable.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "InputLayout.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ConstantBuffer.h"
#include "TransformMatrixCBuffer.h"
#include "Topology.h"
#include "Viewport.h"


class Triangle : public IDrawable
{
public:
	Triangle(Graphics* gfx);

	virtual void Update(float deltaTime) noexcept override;
	virtual DirectX::XMMATRIX GetModelMatrix() const noexcept override;
};