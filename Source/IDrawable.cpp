#include "IDrawable.h"
#include "IBindable.h"
#include "IndexBuffer.h"

void IDrawable::BindAll(std::shared_ptr<Graphics> pGfx) const noexcept
{
	for (auto& bindable : m_bindables)
		bindable->Bind(pGfx);
}

void IDrawable::Draw(std::shared_ptr<Graphics> pGfx) const noexcept
{
	BindAll(pGfx);
	pGfx->DrawIndexed(m_indexBufferCount);
}

void IDrawable::AddBind(std::shared_ptr<IBindable> pBindable) noexcept
{
	m_bindables.push_back(std::move(pBindable));
}
void IDrawable::AddBinds(std::vector<std::shared_ptr<IBindable>> bindables) noexcept
{
	m_bindables.insert(m_bindables.end(), bindables.begin(), bindables.end());
}
