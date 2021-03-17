#pragma once
#include <DirectXMath.h>
#include "ConstantBuffer.h"
#include "IDrawable.h"


class TransformMatrixCBuffer : public IBindable
{
public:
	TransformMatrixCBuffer(std::shared_ptr<Graphics> pGfx, IDrawable* pParent);

	virtual void Bind(std::shared_ptr<Graphics> pGfx) noexcept override;

private:
	VertexConstantBuffer<DirectX::XMMATRIX> m_pCBuffer;
	IDrawable* m_pParent;
};

