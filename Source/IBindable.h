#pragma once
#include "Graphics.h"
#include "wrl.h"
#include <memory>
#include <vector>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

//class Graphics;

class IBindable
{
public:
	virtual void Bind(Graphics* gfx) noexcept = 0;
	virtual ~IBindable() = default;
};