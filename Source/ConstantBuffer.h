#pragma once
#include "IBindable.h"
#include "Graphics.h"

template <class T>
class ConstantBuffer : public IBindable
{
public:
	static std::shared_ptr<ConstantBuffer> CreateObject(Graphics* gfx, const T& buffer) noexcept
	{
		std::shared_ptr<ConstantBuffer> pConstantBuffer = std::make_shared<ConstantBuffer>();

		D3D11_BUFFER_DESC bufferDescription = {};
		{
			bufferDescription.Usage = D3D11_USAGE_DYNAMIC;
			bufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

			bufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // dynamic usage
			bufferDescription.MiscFlags = NULL;

			bufferDescription.StructureByteStride = NULL; // not an array
			bufferDescription.ByteWidth = sizeof(constBuffer);
		}

		D3D11_SUBRESOURCE_DATA bufferInitialData;
		{
			bufferInitialData.pSysMem = &buffer;
		}

		Microsoft::WRL::ComPtr<ID3D11Buffer> pBuffer = nullptr;
		HRESULT result = m_pDevice->CreateBuffer(&bufferDescription, &bufferInitialData, &pBuffer);
		if (FAILED(result) == TRUE)
		{
			ErrorHandler::ErrorBox(L"Error Creating Constant Buffer Object", result, __FILE__, __LINE__);
			return false;
		}
		pConstantBuffer->SetBuffer(pBuffer);
	}
	bool DestroyObject() noexcept
	{
		return true;
	}

	// update constant buffer data in VRAM
	void Update(Graphics* gfx, const T& buffer) noexcept
	{
		D3D11_MAPPED_SUBRESOURCE mappedSubresource;
		gfx->GetContext()->Map(m_pBuffer.Get(), &buffer, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedSubresource);
		memcpy(mappedSubresource, &buffer, sizeof(buffer));
		gfx->GetContext()->Unmap(m_pBuffer.Get(), 0u);
	}


protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pBuffer;


private:
	void SetBuffer(Microsoft::WRL::ComPtr<ID3D11Buffer> pBuffer) noexcept
	{
		m_pBuffer = pBuffer;
	}
};


template<typename T>
class VertexConstantBuffer : public ConstantBuffer<T>
{
public:
	virtual void Bind(Graphics* gfx) noexcept override
	{
		gfx->GetContext()->VSSetConstantBuffers(0u, 1u, m_pBuffer.GetAddressOf());
	}
};


template<typename T>
class PixelConstantBuffer : public ConstantBuffer<T>
{
public:
	virtual void Bind(Graphics* gfx) noexcept override
	{
		gfx->GetContext()->PSSetConstantBuffers(0u, 1u, m_pBuffer.GetAddressOf());
	}
};