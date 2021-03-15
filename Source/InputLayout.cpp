#include "InputLayout.h"
#include "Graphics.h"

std::unique_ptr<InputLayout> InputLayout::CreateObject(Graphics* gfx, std::vector<D3D11_INPUT_ELEMENT_DESC> description, Microsoft::WRL::ComPtr<ID3DBlob> pVSBlob) noexcept
{
	std::unique_ptr<InputLayout> pInputLayout = std::make_unique<InputLayout>();

	Microsoft::WRL::ComPtr<ID3D11InputLayout> pBuffer;
	HRESULT result = gfx->GetDevice()->CreateInputLayout(description.data(), (UINT)description.size(), pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &pBuffer);
	if (FAILED(result) == TRUE)
	{
		ErrorHandler::ErrorBox(L"Error Creating Input Layout Object", result, __FILE__, __LINE__);
		return nullptr;
	}
	pInputLayout->SetBuffer(pBuffer);


	return std::move(pInputLayout);
}
bool InputLayout::DestroyObject() noexcept
{
	return true;
}


void InputLayout::Bind(Graphics* gfx) noexcept
{
	gfx->GetContext()->IASetInputLayout(m_pBuffer.Get());
}


void InputLayout::SetBuffer(Microsoft::WRL::ComPtr<ID3D11InputLayout> pBuffer) noexcept
{
	m_pBuffer = pBuffer;
}