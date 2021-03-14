#include "Topology.h"
#include "Graphics.h"

std::shared_ptr<Topology> Topology::CreateObject(D3D11_PRIMITIVE_TOPOLOGY type) noexcept
{
	std::shared_ptr<Topology> pTopology = std::make_shared<Topology>();
	pTopology->SetType(type);

	return pTopology;
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