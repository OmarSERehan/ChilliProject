#pragma once
#include "IBindable.h"


class VertexShader : public IBindable
{
public:
	VertexShader(Graphics* gfx, std::wstring shaderPath);

	virtual void Bind(Graphics* gfx) noexcept override;

	Microsoft::WRL::ComPtr<ID3DBlob> GetBlob() const noexcept;

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pBuffer;
	Microsoft::WRL::ComPtr<ID3DBlob> m_pBlob;
};
