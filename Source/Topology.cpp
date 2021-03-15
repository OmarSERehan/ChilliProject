#include "Topology.h"
#include "Graphics.h"

Topology::Topology(D3D11_PRIMITIVE_TOPOLOGY type)
	:
	m_type(type)
{}

void Topology::Bind(Graphics* gfx) noexcept
{
	gfx->GetContext()->IASetPrimitiveTopology(m_type);
}