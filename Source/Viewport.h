#pragma once
#include "IBindable.h"


class Viewport : public IBindable
{
public:
	Viewport(float width, float height);

	void Bind(std::shared_ptr<Graphics> pGfx) noexcept;

private:
	D3D11_VIEWPORT m_pBuffer;
};