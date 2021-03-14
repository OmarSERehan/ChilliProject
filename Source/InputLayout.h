#pragma once
#include "IBindable.h"

class Graphics;

class InputLayout : public IBindable
{
public:
	static std::shared_ptr<InputLayout> CreateObject(Graphics* gfx, Microsoft::WRL::ComPtr<ID3DBlob> pVSBlob) noexcept;
	bool DestroyObject() noexcept;

	virtual void Bind(Graphics* gfx) noexcept override;


private:
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pBuffer;

	void SetBuffer(Microsoft::WRL::ComPtr<ID3D11InputLayout> pBuffer) noexcept;
};