#pragma once
#include "IBindable.h"


class IndexBuffer : public IBindable
{
public:
	IndexBuffer(std::shared_ptr<Graphics> pGfx, const std::vector<uint16_t>& indices);

	virtual void Bind(std::shared_ptr<Graphics> pGfx) noexcept override;

	uint32_t GetCount() const noexcept
	{
		return m_count;
	}
	
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pBuffer = nullptr;
	uint32_t m_count = 0u;
};