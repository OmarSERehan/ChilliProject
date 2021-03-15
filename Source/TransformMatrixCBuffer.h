#pragma once
#include <DirectXMath.h>
#include "ConstantBuffer.h"
#include "IDrawable.h"


class TransformMatrixCBuffer : public IBindable
{
public:
	static std::unique_ptr<TransformMatrixCBuffer> CreateObject(Graphics* gfx, IDrawable* pParent) noexcept;
	bool DestroyObject() noexcept;

	virtual void Bind(Graphics* gfx) noexcept override;


private:
	std::unique_ptr<VertexConstantBuffer<DirectX::XMMATRIX>> m_pCBuffer;
	
	IDrawable* m_pParent;

	void SetConstantBufferObject(std::unique_ptr<VertexConstantBuffer<DirectX::XMMATRIX>>&& pCBuffer);
	void SetParent(IDrawable* pParent) noexcept;
};

