#pragma once
#include "IBindable.h"


class Viewport : public IBindable
{
public:
	static std::unique_ptr<Viewport> CreateObject(float width, float height) noexcept;
	bool DestroyObject() noexcept;

	void Bind(Graphics* gfx) noexcept;

	void SetBuffer(std::unique_ptr<D3D11_VIEWPORT> buffer) noexcept;


private:
	std::unique_ptr<D3D11_VIEWPORT> m_pBuffer;
};