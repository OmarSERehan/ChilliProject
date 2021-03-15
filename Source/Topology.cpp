#include "Topology.h"
#include "Graphics.h"

std::unique_ptr<Topology> Topology::CreateObject(D3D11_PRIMITIVE_TOPOLOGY type) noexcept
{
	std::unique_ptr<Topology> pTopology = std::make_unique<Topology>();
	pTopology->SetType(type);

	return std::move(pTopology);
}
bool Topology::DestroyObject() noexcept
{
	return true;
}


void Topology::Bind(Graphics* gfx) noexcept
{
	gfx->GetContext()->IASetPrimitiveTopology(m_type);
}


void Topology::SetType(D3D11_PRIMITIVE_TOPOLOGY type) noexcept
{
	m_type = type;
}