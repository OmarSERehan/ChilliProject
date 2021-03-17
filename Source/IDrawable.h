#pragma once
#include <memory>
#include <random>
#include <DirectXMath.h>

#include "Graphics.h"


class IBindable;

class IDrawable
{
public:
	void AddBind(std::shared_ptr<IBindable> bindable) noexcept;
	void AddBinds(std::vector<std::shared_ptr<IBindable>> bindables) noexcept;

	virtual void Update(float deltaTime) noexcept = 0;
	
	void BindAll(Graphics* gfx) const noexcept;
	void Draw(Graphics* gfx) const noexcept;

	virtual DirectX::XMMATRIX GetModelMatrix() const noexcept = 0;
	

private:
	std::vector<std::shared_ptr<IBindable>> m_bindables;
	
protected:
	uint32_t m_indexBufferCount = 0u;


public:
	IDrawable() = default;
	~IDrawable() = default;

	IDrawable(const IDrawable&) = delete;
	IDrawable& operator = (const IDrawable&) = delete;
};

