#include "Topology.h"
#include "Graphics.h"

Topology::Topology(D3D11_PRIMITIVE_TOPOLOGY type)
	:
	m_type(type)
{}

void Topology::Bind(std::shared_ptr<Graphics> pGfx) noexcept
{
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext = pGfx->GetContext();
	pContext->IASetPrimitiveTopology(m_type);
}