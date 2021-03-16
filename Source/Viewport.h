#pragma once
#include "IBindable.h"


class Viewport : public IBindable
{
public:
	Viewport(float width, float height);

	void Bind(Graphics* gfx) noexcept;

private:
	D3D11_VIEWPORT m_pBuffer;
};