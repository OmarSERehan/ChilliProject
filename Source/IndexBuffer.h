#pragma once
#include "IBindable.h"


class IndexBuffer : public IBindable
{
public:
	static std::shared_ptr<IndexBuffer> CreateObject(Graphics* gfx, const std::vector<uint16_t>& indices) noexcept;
	bool DestroyObject() noexcept;

	virtual void Bind(Graphics* gfx) noexcept override;

	uint32_t GetCount() const noexcept;
	Microsoft::WRL::ComPtr<ID3D11Buffer> GetBuffer() const noexcept;

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pBuffer = nullptr;
	uint32_t m_count = 0u;

	void SetBuffer(Microsoft::WRL::ComPtr<ID3D11Buffer> pBuffer) noexcept;
	void SetCount(uint32_t count) noexcept;
};