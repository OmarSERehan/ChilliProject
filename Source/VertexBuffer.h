#pragma once
#include "IBindable.h"

class VertexBuffer : public IBindable
{
public:
	template<class Vertex>
	VertexBuffer(Graphics* gfx, const std::vector<Vertex>& vertices)
		:
		m_stride(sizeof(Vertex))
	{
		D3D11_SUBRESOURCE_DATA bufferInitialData;
		{
			bufferInitialData.pSysMem = vertices.data();
		}

		/// Data about the buffer (as a whole) we want to create
		D3D11_BUFFER_DESC bufferDescription = {};
		{
			bufferDescription.Usage = D3D11_USAGE_DEFAULT;
			bufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;

			bufferDescription.CPUAccessFlags = NULL;
			bufferDescription.MiscFlags = NULL;

			bufferDescription.StructureByteStride = sizeof(Vertex);
			bufferDescription.ByteWidth = sizeof(Vertex) * vertices.size();
		}

		/// Create vertex buffer
		{
			HRESULT result = gfx->GetDevice()->CreateBuffer(&bufferDescription, &bufferInitialData, &m_pBuffer);
			/*std::stringstream stringStream;
			stringStream << std::hex << m_pBuffer.Get();
			std::string str = stringStream.str();
			ErrorHandler::Log("Vertex Buffer Address: " + str);*/
			if (FAILED(result) == TRUE)
			{
				ErrorHandler::ErrorBox(L"Error Creating Vertex Buffer", result, __FILE__, __LINE__);
				throw -1;
			}
		}
	}
	~VertexBuffer();

	virtual void Bind(Graphics* gfx) noexcept override;

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pBuffer;
	uint32_t m_stride;
};