#pragma once
#include "IBindable.h"


class VertexShader : public IBindable
{
public:
	static std::unique_ptr<VertexShader> CreateObject(Graphics* gfx, std::wstring shaderPath) noexcept;
	bool DestroyObject() noexcept;

	virtual void Bind(Graphics* gfx) noexcept override;

	Microsoft::WRL::ComPtr<ID3DBlob> GetBlob() const noexcept;

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pBuffer;
	Microsoft::WRL::ComPtr<ID3DBlob> m_pBlob;

	void SetBuffer(Microsoft::WRL::ComPtr<ID3D11VertexShader> pBuffer) noexcept;
	void SetBlob(Microsoft::WRL::ComPtr<ID3DBlob> pBlob) noexcept;

	Microsoft::WRL::ComPtr<ID3D11VertexShader> GetBuffer() const noexcept;
};

