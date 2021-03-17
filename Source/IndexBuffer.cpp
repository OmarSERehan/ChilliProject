#include "IndexBuffer.h"
#include "Graphics.h"

IndexBuffer::IndexBuffer(std::shared_ptr<Graphics> pGfx, const std::vector<uint16_t>& indices)
	:
	m_count(indices.size())
{
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
		bufferDescription.ByteWidth = sizeof(uint16_t) * m_count;
	}


	/// Create index buffer
	HRESULT result = pGfx->GetDevice()->CreateBuffer(&bufferDescription, &bufferInitialData, &m_pBuffer);
	if (FAILED(result) == TRUE)
	{
		ErrorHandler::ErrorBox(L"Error Creating Index Buffer", result, __FILE__, __LINE__);
		throw - 1;
	}
}

void IndexBuffer::Bind(std::shared_ptr<Graphics> pGfx) noexcept
{
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext = pGfx->GetContext();
	pContext->IASetIndexBuffer(m_pBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
}