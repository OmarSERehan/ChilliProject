#pragma once
#include "IBindable.h"

template <class T>
class ConstantBuffer : public IBindable
{
public:
	ConstantBuffer(std::shared_ptr<Graphics> pGfx)
	{
		D3D11_BUFFER_DESC bufferDescription = {};
		{
			bufferDescription.Usage = D3D11_USAGE_DYNAMIC;
			bufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

			bufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // dynamic usage
			bufferDescription.MiscFlags = NULL;

			bufferDescription.StructureByteStride = NULL; // not an array
			bufferDescription.ByteWidth = sizeof(T);
		}

		HRESULT result = pGfx->GetDevice()->CreateBuffer(&bufferDescription, nullptr, &m_pBuffer);
		if (FAILED(result) == TRUE)
		{
			ErrorHandler::ErrorBox(L"Error Creating Constant Buffer Object", result, __FILE__, __LINE__);
			throw - 1;
		}
	}
	ConstantBuffer(std::shared_ptr<Graphics> pGfx, const T& buffer)
	{
		D3D11_BUFFER_DESC bufferDescription = {};
		{
			bufferDescription.Usage = D3D11_USAGE_DYNAMIC;
			bufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

			bufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // dynamic usage
			bufferDescription.MiscFlags = NULL;

			bufferDescription.StructureByteStride = NULL; // not an array
			bufferDescription.ByteWidth = sizeof(buffer);
		}

		D3D11_SUBRESOURCE_DATA bufferInitialData;
		{
			bufferInitialData.pSysMem = &buffer;
		}

		HRESULT result = pGfx->GetDevice()->CreateBuffer(&bufferDescription, &bufferInitialData, &m_pBuffer);
		if (FAILED(result) == TRUE)
		{
			ErrorHandler::ErrorBox(L"Error Creating Constant Buffer Object", result, __FILE__, __LINE__);
			throw - 1;
		}
	}

	// update constant buffer data in VRAM
	void Update(std::shared_ptr<Graphics> pGfx, const T& buffer) noexcept
	{
		D3D11_MAPPED_SUBRESOURCE mappedSubresource;
		pGfx->GetContext()->Map(m_pBuffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedSubresource);
		memcpy(mappedSubresource.pData, &buffer, sizeof(buffer));
		pGfx->GetContext()->Unmap(m_pBuffer.Get(), 0u);
	}

protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pBuffer;
};


template<typename T>
class VertexConstantBuffer : public ConstantBuffer<T>
{
public:
	using ConstantBuffer<T>::ConstantBuffer;

	virtual void Bind(std::shared_ptr<Graphics> pGfx) noexcept override
	{
		pGfx->GetContext()->VSSetConstantBuffers(0u, 1u, this->m_pBuffer.GetAddressOf());
	}
};


template<typename T>
class PixelConstantBuffer : public ConstantBuffer<T>
{
public:
	using ConstantBuffer<T>::ConstantBuffer;

	virtual void Bind(std::shared_ptr<Graphics> pGfx) noexcept override
	{
		pGfx->GetContext()->PSSetConstantBuffers(0u, 1u, this->m_pBuffer.GetAddressOf());
	}
};