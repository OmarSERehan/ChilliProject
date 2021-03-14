#pragma once
#include "IBindable.h"

class VertexBuffer : public IBindable
{
public:
	template<class Vertex>
	static std::shared_ptr<VertexBuffer> CreateObject(Graphics* gfx, const std::vector<Vertex>& vertices) noexcept
	{
		std::shared_ptr<VertexBuffer> pVertexBuffer = std::make_shared<VertexBuffer>();
		pVertexBuffer->SetStride(sizeof(Vertex));

		D3D11_SUBRESOURCE_DATA bufferInitialData;
		{
			bufferInitialData.pSysMem = vertices;
		}

		/// Data about the buffer (as a whole) we want to create
		D3D11_BUFFER_DESC bufferDescription = {};
		{
			bufferDescription.Usage = D3D11_USAGE_DEFAULT;
			bufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;

			bufferDescription.CPUAccessFlags = NULL;
			bufferDescription.MiscFlags = NULL;

			bufferDescription.StructureByteStride = sizeof(Vertex);
			bufferDescription.ByteWidth = sizeof(vertices);
		}

		/// Create vertex buffer
		Microsoft::WRL::ComPtr<ID3D11Buffer> pBuffer = nullptr;
		{
			HRESULT result = gfx->GetDevice()->CreateBuffer(&bufferDescription, &bufferInitialData, &pBuffer);
			if (FAILED(result) == TRUE)
			{
				ErrorHandler::ErrorBox(L"Error Creating Vertex Buffer", result, __FILE__, __LINE__);
				return false;
			}
			pVertexBuffer->SetBuffer(pBuffer);
		}

		
		return pVertexBuffer;
	}
	bool DestroyObject() noexcept;

	virtual void Bind(Graphics* gfx) noexcept override;


private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pBuffer;

	uint32_t m_stride;

	
	void SetBuffer(Microsoft::WRL::ComPtr<ID3D11Buffer> pBuffer) noexcept;
	void SetStride(uint32_t stride) noexcept;
};