#pragma once
#include "Graphics.h"
#include "wrl.h"
#include <memory>
#include <vector>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>


class IBindable
{
public:
	virtual void Bind(std::shared_ptr<Graphics> pGfx) noexcept = 0;
};