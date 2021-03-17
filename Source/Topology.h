#pragma once
#include "IBindable.h"


class Topology : public IBindable
{
public:
	Topology(D3D11_PRIMITIVE_TOPOLOGY type);

	void Bind(std::shared_ptr<Graphics> pGfx) noexcept;

private:
	D3D11_PRIMITIVE_TOPOLOGY m_type;
};