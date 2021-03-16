#include "VertexShader.h"


VertexShader::VertexShader(Graphics* gfx, std::wstring shaderPath)
{
	/// Read Compiled Shader From Disk
	{
		HRESULT result = D3DReadFileToBlob(shaderPath.c_str(), &m_pBlob);
		if (FAILED(result) == TRUE)
		{
			ErrorHandler::ErrorBox(L"Error Reading Compiled Vertex Shader (cso) File", result, __FILE__, __LINE__);
			throw - 1;
		}
	}

	/// Create Shader Buffer
	{
		HRESULT result = gfx->GetDevice()->CreateVertexShader(m_pBlob->GetBufferPointer(), m_pBlob->GetBufferSize(), nullptr, &m_pBuffer);
		if (FAILED(result) == TRUE)
		{
			ErrorHandler::ErrorBox(L"Error Creating Vertex Shader Object", result, __FILE__, __LINE__);
			throw -1;
		}
	}
}

void VertexShader::Bind(Graphics* gfx) noexcept
{
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext = gfx->GetContext();
	pContext->VSSetShader(m_pBuffer.Get(), nullptr, 0u);
}

Microsoft::WRL::ComPtr<ID3DBlob> VertexShader::GetBlob() const noexcept
{
	return m_pBlob;
}