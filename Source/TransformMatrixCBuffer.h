#pragma once
#include <DirectXMath.h>
#include "ConstantBuffer.h"
#include "IDrawable.h"


class TransformMatrixCBuffer : public IBindable
{
public:
	TransformMatrixCBuffer(Graphics* gfx, IDrawable* pParent);

	virtual void Bind(Graphics* gfx) noexcept override;

private:
	VertexConstantBuffer<DirectX::XMMATRIX> m_pCBuffer;
	IDrawable* m_pParent;
};

