#pragma once
#include <utility>
#include <unordered_map>

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "InputLayout.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ConstantBuffer.h"
#include "Topology.h"

#define BindableMap std::unordered_map< \
						std::shared_ptr<Graphics>, \
						std::pair<uint32_t, std::vector<std::shared_ptr<IBindable>>> \
					> \

class BindableFactory
{
public:
	static std::vector<std::shared_ptr<IBindable>> GetBoxBindables(std::shared_ptr<Graphics> ppGfx, uint32_t& indexCount);

private:
	static BindableMap m_pBoxBindablesMap;
};

