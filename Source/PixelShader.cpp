#include "PixelShader.h"


std::unique_ptr<PixelShader> PixelShader::CreateObject(Graphics* gfx, std::wstring shaderPath) noexcept
{
	std::unique_ptr<PixelShader> pPixelShader = std::make_unique<PixelShader>();

	/// Read Compiled Shader From Disk
	Microsoft::WRL::ComPtr<ID3DBlob> pBlob = nullptr;
	{
		HRESULT result = D3DReadFileToBlob(shaderPath.c_str(), &pBlob);
		if (FAILED(result) == TRUE)
		{
			ErrorHandler::ErrorBox(L"Error Reading Compiled Pixel Shader (cso) File", result, __FILE__, __LINE__);
			return nullptr;
		}
	}

	/// Create Shader Buffer
	{
		Microsoft::WRL::ComPtr<ID3D11PixelShader> pBuffer = nullptr;
		HRESULT result = gfx->GetDevice()->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pBuffer);
		if (FAILED(result) == TRUE)
		{
			ErrorHandler::ErrorBox(L"Error Creating Pixel Shader Object", result, __FILE__, __LINE__);
			return nullptr;
		}
		pPixelShader->SetBuffer(pBuffer);
	}


	return pPixelShader;
}
bool PixelShader::DestroyObject() noexcept
{
	return true;
}

void PixelShader::Bind(Graphics* gfx) noexcept
{
	gfx->GetContext()->PSSetShader(m_pBuffer.Get(), nullptr, 0u);
}

void PixelShader::SetBuffer(Microsoft::WRL::ComPtr<ID3D11PixelShader> pBuffer) noexcept
{
	m_pBuffer = pBuffer;
}

Microsoft::WRL::ComPtr<ID3D11PixelShader> PixelShader::GetBuffer() const noexcept
{
	return m_pBuffer;
}