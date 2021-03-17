#include "InputLayout.h"
#include "Graphics.h"

InputLayout::InputLayout(std::shared_ptr<Graphics> pGfx, std::vector<D3D11_INPUT_ELEMENT_DESC> description, Microsoft::WRL::ComPtr<ID3DBlob> pVSBlob)
{
	HRESULT result = pGfx->GetDevice()->CreateInputLayout(description.data(), (UINT)description.size(), pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &m_pBuffer);
	if (FAILED(result) == TRUE)
	{
		ErrorHandler::ErrorBox(L"Error Creating Input Layout Object", result, __FILE__, __LINE__);
		throw -1;
	}
}

void InputLayout::Bind(std::shared_ptr<Graphics> pGfx) noexcept
{
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext = pGfx->GetContext();
	pContext->IASetInputLayout(m_pBuffer.Get());
}