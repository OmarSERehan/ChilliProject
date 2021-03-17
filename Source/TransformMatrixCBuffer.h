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
	static std::unique_ptr<VertexConstantBuffer<DirectX::XMMATRIX>> s_pCBuffer;
	
	IDrawable* m_pParent;
};

