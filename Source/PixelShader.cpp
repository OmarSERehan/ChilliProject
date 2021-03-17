#include "PixelShader.h"


PixelShader::PixelShader(std::shared_ptr<Graphics> pGfx, std::wstring shaderPath)
{
	/// Read Compiled Shader From Disk
	Microsoft::WRL::ComPtr<ID3DBlob> pBlob = nullptr;
	{
		HRESULT result = D3DReadFileToBlob(shaderPath.c_str(), &pBlob);
		if (FAILED(result) == TRUE)
		{
			ErrorHandler::ErrorBox(L"Error Reading Compiled Pixel Shader (cso) File", result, __FILE__, __LINE__);
			throw - 1;
		}
	}

	/// Create Shader Buffer
	{
		HRESULT result = pGfx->GetDevice()->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &m_pBuffer);
		if (FAILED(result) == TRUE)
		{
			ErrorHandler::ErrorBox(L"Error Creating Pixel Shader Object", result, __FILE__, __LINE__);
			throw - 1;
		}
	}
}

void PixelShader::Bind(std::shared_ptr<Graphics> pGfx) noexcept
{
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext = pGfx->GetContext();
	pContext->PSSetShader(m_pBuffer.Get(), nullptr, 0u);
}
