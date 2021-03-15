#include "VertexShader.h"


std::unique_ptr<VertexShader> VertexShader::CreateObject(Graphics* gfx, std::wstring shaderPath) noexcept
{
	std::unique_ptr<VertexShader> pVertexShader = std::make_unique<VertexShader>();

	/// Read Compiled Shader From Disk
	Microsoft::WRL::ComPtr<ID3DBlob> pBlob = nullptr;
	{
		HRESULT result = D3DReadFileToBlob(shaderPath.c_str(), &pBlob);
		if (FAILED(result) == TRUE)
		{
			ErrorHandler::ErrorBox(L"Error Reading Compiled Vertex Shader (cso) File", result, __FILE__, __LINE__);
			return nullptr;
		}
		pVertexShader->SetBlob(pBlob);
	}

	/// Create Shader Buffer
	{
		Microsoft::WRL::ComPtr<ID3D11VertexShader> pBuffer = nullptr;
		HRESULT result = gfx->GetDevice()->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pBuffer);
		if (FAILED(result) == TRUE)
		{
			ErrorHandler::ErrorBox(L"Error Creating Vertex Shader Object", result, __FILE__, __LINE__);
			return nullptr;
		}
		pVertexShader->SetBuffer(pBuffer);
	}


	return std::move(pVertexShader);
}
bool VertexShader::DestroyObject() noexcept
{
	return true;
}

void VertexShader::Bind(Graphics* gfx) noexcept
{
	gfx->GetContext()->VSSetShader(m_pBuffer.Get(), nullptr, 0u);
}

void VertexShader::SetBuffer(Microsoft::WRL::ComPtr<ID3D11VertexShader> pBuffer) noexcept
{
	m_pBuffer = pBuffer;
}
void VertexShader::SetBlob(Microsoft::WRL::ComPtr<ID3DBlob> pBlob) noexcept
{
	m_pBlob = pBlob;
}

Microsoft::WRL::ComPtr<ID3D11VertexShader> VertexShader::GetBuffer() const noexcept
{
	return m_pBuffer;
}
Microsoft::WRL::ComPtr<ID3DBlob> VertexShader::GetBlob() const noexcept
{
	return m_pBlob;
}