#include "IndexBuffer.h"
#include "Graphics.h"

std::shared_ptr<IndexBuffer> IndexBuffer::CreateObject(Graphics* gfx, const std::vector<uint16_t>& indices) noexcept
{
	std::shared_ptr<IndexBuffer> pIndexBuffer = std::make_shared<IndexBuffer>();
	{
		pIndexBuffer->SetCount(indices.size());
	}

	
	D3D11_SUBRESOURCE_DATA bufferInitialData;
	{
		bufferInitialData.pSysMem = indices.data();
	}

	
	/// Data about the buffer (as a whole) we want to create
	D3D11_BUFFER_DESC bufferDescription = {};
	{
		bufferDescription.Usage = D3D11_USAGE_DEFAULT;
		bufferDescription.BindFlags = D3D11_BIND_INDEX_BUFFER;

		bufferDescription.CPUAccessFlags = NULL;
		bufferDescription.MiscFlags = NULL;

		bufferDescription.StructureByteStride = sizeof(uint16_t);
		bufferDescription.ByteWidth = sizeof(uint16_t) * pIndexBuffer->GetCount();
	}


	/// Create index buffer
	Microsoft::WRL::ComPtr<ID3D11Buffer> pBuffer = nullptr;
	HRESULT result = gfx->GetDevice()->CreateBuffer(&bufferDescription, &bufferInitialData, &pBuffer);
	if (FAILED(result) == TRUE)
	{
		ErrorHandler::ErrorBox(L"Error Creating Index Buffer", result, __FILE__, __LINE__);
		return nullptr;
	}
	pIndexBuffer->SetBuffer(pBuffer);


	return pIndexBuffer;
}
bool IndexBuffer::DestroyObject() noexcept
{
	return true;
}


void IndexBuffer::Bind(Graphics* gfx) noexcept
{
	gfx->GetContext()->IASetIndexBuffer(m_pBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
}


void IndexBuffer::SetCount(uint32_t count) noexcept
{
	m_count = count;
}
void IndexBuffer::SetBuffer(Microsoft::WRL::ComPtr<ID3D11Buffer> pBuffer) noexcept
{
	m_pBuffer = pBuffer;
}


uint32_t IndexBuffer::GetCount() const noexcept
{
	return m_count;
}
Microsoft::WRL::ComPtr<ID3D11Buffer> IndexBuffer::GetBuffer() const noexcept
{
	return m_pBuffer;
}