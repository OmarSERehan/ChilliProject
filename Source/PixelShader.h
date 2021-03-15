#pragma once
#include "IBindable.h"


class PixelShader : public IBindable
{
public:
	static std::unique_ptr<PixelShader> CreateObject(Graphics* gfx, std::wstring shaderPath) noexcept;
	bool DestroyObject() noexcept;

	virtual void Bind(Graphics* gfx) noexcept override;


private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pBuffer;

	void SetBuffer(Microsoft::WRL::ComPtr<ID3D11PixelShader> pBuffer) noexcept;

	Microsoft::WRL::ComPtr<ID3D11PixelShader> GetBuffer() const noexcept;
};

