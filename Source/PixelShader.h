#pragma once
#include "IBindable.h"


class PixelShader : public IBindable
{
public:
	PixelShader(std::shared_ptr<Graphics> pGfx, std::wstring shaderPath);

	virtual void Bind(std::shared_ptr<Graphics> pGfx) noexcept override;

private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pBuffer;
};

