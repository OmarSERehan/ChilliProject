#include "IDrawable.h"
#include "IBindable.h"
#include "IndexBuffer.h"

void IDrawable::BindAll(Graphics* gfx) const noexcept
{
	for (auto& bindable : m_bindables)
		bindable->Bind(gfx);
}

void IDrawable::Draw(Graphics* gfx) const noexcept
{
	BindAll(gfx);
	gfx->DrawIndexed(m_indexBufferCount);
}

void IDrawable::AddBind(std::unique_ptr<IBindable> pBindable) noexcept
{
	if (typeid(*pBindable) == typeid(IndexBuffer))
	{
		m_indexBufferCount = static_cast<IndexBuffer*>(pBindable.get())->GetCount();
	}

	m_bindables.push_back(std::move(pBindable));
}
