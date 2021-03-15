#pragma once
#include "IBindable.h"

class Graphics;

class InputLayout : public IBindable
{
public:
	InputLayout(Graphics* gfx, std::vector<D3D11_INPUT_ELEMENT_DESC> description, Microsoft::WRL::ComPtr<ID3DBlob> pVSBlob);

	virtual void Bind(Graphics* gfx) noexcept override;

private:
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pBuffer;
};