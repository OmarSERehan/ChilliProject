#pragma once
#include "IBindable.h"

template <class T>
class ConstantBuffer : public IBindable
{
public:
	static Microsoft::WRL::ComPtr<ID3D11Buffer> CreateBuffer(Graphics* gfx) noexcept
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

		Microsoft::WRL::ComPtr<ID3D11Buffer> pBuffer = nullptr;
		HRESULT result = gfx->GetDevice()->CreateBuffer(&bufferDescription, nullptr, &pBuffer);
		if (FAILED(result) == TRUE)
		{
			ErrorHandler::ErrorBox(L"Error Creating Constant Buffer Object", result, __FILE__, __LINE__);
			return nullptr;
		}

		return pBuffer;
	}
	static Microsoft::WRL::ComPtr<ID3D11Buffer> CreateBuffer(Graphics* gfx, const T& buffer) noexcept
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

		Microsoft::WRL::ComPtr<ID3D11Buffer> pBuffer = nullptr;
		HRESULT result = gfx->GetDevice()->CreateBuffer(&bufferDescription, &bufferInitialData, &pBuffer);
		if (FAILED(result) == TRUE)
		{
			ErrorHandler::ErrorBox(L"Error Creating Constant Buffer Object", result, __FILE__, __LINE__);
			return nullptr;
		}
		
		return pBuffer;
	}

	// update constant buffer data in VRAM
	void Update(Graphics* gfx, const T& buffer) noexcept
	{
		D3D11_MAPPED_SUBRESOURCE mappedSubresource;
		gfx->GetContext()->Map(m_pBuffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedSubresource);
		memcpy(mappedSubresource.pData, &buffer, sizeof(buffer));
		gfx->GetContext()->Unmap(m_pBuffer.Get(), 0u);
	}


protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pBuffer;

	void SetBuffer(Microsoft::WRL::ComPtr<ID3D11Buffer> pBuffer) noexcept
	{
		m_pBuffer = pBuffer;
	}
};


template<typename T>
class VertexConstantBuffer : public ConstantBuffer<T>
{
public:
	static std::unique_ptr<VertexConstantBuffer> CreateObject(Graphics* gfx) noexcept
	{
		std::unique_ptr<VertexConstantBuffer> pVCB = std::make_unique<VertexConstantBuffer>();
		pVCB->SetBuffer(ConstantBuffer<T>::CreateBuffer(gfx));

		return std::move(pVCB);
	}
	static std::unique_ptr<VertexConstantBuffer> CreateObject(Graphics* gfx, const T& buffer) noexcept
	{
		std::unique_ptr<VertexConstantBuffer> pVCB = std::make_unique<VertexConstantBuffer>();
		pVCB->SetBuffer(ConstantBuffer<T>::CreateBuffer(gfx, buffer));

		return std::move(pVCB);
	}
	bool DestroyObject() noexcept
	{
		if (this->m_pBuffer.Release());
	}

	virtual void Bind(Graphics* gfx) noexcept override
	{
		gfx->GetContext()->VSSetConstantBuffers(0u, 1u, this->m_pBuffer.GetAddressOf());
	}
};


template<typename T>
class PixelConstantBuffer : public ConstantBuffer<T>
{
public:
	static std::unique_ptr<PixelConstantBuffer> CreateObject(Graphics* gfx) noexcept
	{
		std::unique_ptr<PixelConstantBuffer> pPCB = std::make_unique<PixelConstantBuffer>();
		pPCB->SetBuffer(ConstantBuffer<T>::CreateBuffer(gfx));

		return std::move(pPCB);
	}
	static std::unique_ptr<PixelConstantBuffer> CreateObject(Graphics* gfx, const T& buffer) noexcept
	{
		std::unique_ptr<PixelConstantBuffer> pPCB = std::make_unique<PixelConstantBuffer>();
		pPCB->SetBuffer(ConstantBuffer<T>::CreateBuffer(gfx, buffer));

		return std::move(pPCB);
	}
	bool DestroyObject() noexcept
	{
		if (this->m_pBuffer.Release());
	}

	virtual void Bind(Graphics* gfx) noexcept override
	{
		gfx->GetContext()->PSSetConstantBuffers(0u, 1u, this->m_pBuffer.GetAddressOf());
	}
};