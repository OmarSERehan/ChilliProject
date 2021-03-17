#pragma once
#include "IBindable.h"


class VertexShader : public IBindable
{
public:
	VertexShader(std::shared_ptr<Graphics> pGfx, std::wstring shaderPath);

	virtual void Bind(std::shared_ptr<Graphics> pGfx) noexcept override;

	Microsoft::WRL::ComPtr<ID3DBlob> GetBlob() const noexcept;

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pBuffer;
	Microsoft::WRL::ComPtr<ID3DBlob> m_pBlob;
};
