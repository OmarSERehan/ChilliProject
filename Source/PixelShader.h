#pragma once
#include "IBindable.h"


class PixelShader : public IBindable
{
public:
	PixelShader(Graphics* gfx, std::wstring shaderPath);

	virtual void Bind(Graphics* gfx) noexcept override;

private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pBuffer;
};

