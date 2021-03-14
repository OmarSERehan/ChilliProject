#pragma once
#include "IBindable.h"


class Topology : public IBindable
{
public:
	static std::shared_ptr<Topology> CreateObject(D3D11_PRIMITIVE_TOPOLOGY type) noexcept;
	bool DestroyObject() noexcept;

	void Bind(Graphics* gfx) noexcept;

	void SetType(D3D11_PRIMITIVE_TOPOLOGY type) noexcept;


private:
	D3D11_PRIMITIVE_TOPOLOGY m_type;
};