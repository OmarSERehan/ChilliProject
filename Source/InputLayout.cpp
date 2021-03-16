#include "InputLayout.h"
#include "Graphics.h"

InputLayout::InputLayout(Graphics* gfx, std::vector<D3D11_INPUT_ELEMENT_DESC> description, Microsoft::WRL::ComPtr<ID3DBlob> pVSBlob)
{
	HRESULT result = gfx->GetDevice()->CreateInputLayout(description.data(), (UINT)description.size(), pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &m_pBuffer);
	if (FAILED(result) == TRUE)
	{
		ErrorHandler::ErrorBox(L"Error Creating Input Layout Object", result, __FILE__, __LINE__);
		throw -1;
	}
}

void InputLayout::Bind(Graphics* gfx) noexcept
{
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext = gfx->GetContext();
	pContext->IASetInputLayout(m_pBuffer.Get());
}