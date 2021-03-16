#include "Topology.h"
#include "Graphics.h"

Topology::Topology(D3D11_PRIMITIVE_TOPOLOGY type)
	:
	m_type(type)
{}

void Topology::Bind(Graphics* gfx) noexcept
{
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext = gfx->GetContext();
	pContext->IASetPrimitiveTopology(m_type);
}