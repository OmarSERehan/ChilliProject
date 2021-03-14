#include "InputLayout.h"
#include "Graphics.h"

std::shared_ptr<InputLayout> InputLayout::CreateObject(Graphics* gfx, Microsoft::WRL::ComPtr<ID3DBlob> pVSBlob) noexcept
{
	std::shared_ptr<InputLayout> pInputLayout = std::make_shared<InputLayout>();

	/// Specify how the data is laid out in the vertex buffer
	D3D11_INPUT_ELEMENT_DESC inputElementDescription[]
	{
		{
			"UV_Position",					// SemanticName (Vertex shader)
			0,								// SemanticIndex (Vertex shader)

			DXGI_FORMAT_R32G32B32_FLOAT,	// Format (3 floats)
			0,								// InputSlot (keep it zero for now)
			0,								// AlignedByteOffset

			D3D11_INPUT_PER_VERTEX_DATA,	// InputSlotClass
			NULL							// InstanceDataStepRate
		}
	};

	Microsoft::WRL::ComPtr<ID3D11InputLayout> pBuffer;
	HRESULT result = gfx->GetDevice()->CreateInputLayout(inputElementDescription, (UINT)std::size(inputElementDescription), pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &pBuffer);
	if (FAILED(result) == TRUE)
	{
		ErrorHandler::ErrorBox(L"Error Creating Input Layout Object", result, __FILE__, __LINE__);
		return nullptr;
	}
	pInputLayout->SetBuffer(pBuffer);


	return pInputLayout;
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