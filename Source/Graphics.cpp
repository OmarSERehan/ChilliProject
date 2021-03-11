#include "Graphics.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

std::shared_ptr<Graphics> Graphics::CreateObject(HWND windowHandle) noexcept
{
	std::shared_ptr<Graphics> pGraphics = std::make_shared<Graphics>();

	/// Create empty pointers to fill
	Microsoft::WRL::ComPtr<ID3D11Device> pDevice = nullptr;
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwapChain = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext = nullptr;

	/// Determine swap chain configurations
	DXGI_SWAP_CHAIN_DESC swapChainDescription = {};
	{
		// buffer data
		swapChainDescription.BufferDesc.Width = NULL;
		swapChainDescription.BufferDesc.Height = NULL;
		swapChainDescription.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		swapChainDescription.BufferDesc.RefreshRate.Numerator = NULL;
		swapChainDescription.BufferDesc.RefreshRate.Denominator = NULL;
		swapChainDescription.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		swapChainDescription.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // frame buffer
		swapChainDescription.BufferCount = 2/*1*/; // num of back buffers

		/// antialiasing data
		swapChainDescription.SampleDesc.Count = 1;
		swapChainDescription.SampleDesc.Quality = 0;

		/// window data
		swapChainDescription.OutputWindow = windowHandle;
		swapChainDescription.Windowed = TRUE;

		/// miscellaneous
		swapChainDescription.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;/* DXGI_SWAP_EFFECT_DISCARD;*/
		swapChainDescription.Flags = NULL;
	}

	UINT swapchainCreationFlags = NULL;
#ifndef NDEBUG
	swapchainCreationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif


	/// Create device, swap chain, and context
	HRESULT result = D3D11CreateDeviceAndSwapChain(
		nullptr,					// graphics card
		D3D_DRIVER_TYPE_HARDWARE,	// driver type
		nullptr,					// handle in case of software driver
		swapchainCreationFlags,		// flags
		nullptr,					// allowed feature levels
		NULL,						// allowed feature levels
		D3D11_SDK_VERSION,			// targeted direct3d sdk version
		&swapChainDescription,		// swap chain configurations
		&pSwapChain,	// output pointer to swap chain
		&pDevice,		// output pointer to device
		nullptr,					// output pointer with chosen feature level
		&pContext		// output pointer to context
	);
	if (FAILED(result) == TRUE)
	{
		switch (result) {
		case DXGI_ERROR_NOT_CURRENTLY_AVAILABLE:
		{
			std::wstring errorString = L"D3D11CreateDeviceAndSwapChain: called in a session 0 process";
			MessageBox(nullptr, errorString.c_str(), L"Error initializing Direct3D", MB_OK | MB_ICONEXCLAMATION);
			return nullptr;
		}
		case E_INVALIDARG:
		{
			std::wstring errorString = L"D3D11CreateDeviceAndSwapChain: pAdapter is non-NULL and the DriverType is set to D3D_DRIVER_TYPE_HARDWARE";
			MessageBox(nullptr, errorString.c_str(), L"Error initializing Direct3D", MB_OK | MB_ICONEXCLAMATION);
			return nullptr;
		}
		case DXGI_ERROR_SDK_COMPONENT_MISSING:
		{
			std::wstring errorString = L"D3D11CreateDeviceAndSwapChain: specified D3D11_CREATE_DEVICE_DEBUG in flags and the incorrect version of the debug layer is installed on your computer";
			MessageBox(nullptr, errorString.c_str(), L"Error initializing Direct3D", MB_OK | MB_ICONEXCLAMATION);
			return nullptr;
		}
		default:
		{
			std::wstring errorString = L"D3D11CreateDeviceAndSwapChain: some error";
			MessageBox(nullptr, errorString.c_str(), L"Error initializing Direct3D", MB_OK | MB_ICONEXCLAMATION);
			return nullptr;
		}
		}
	}

	pGraphics->SetDevice(pDevice);
	pGraphics->SetSwapChain(pSwapChain);
	pGraphics->SetContext(pContext);


	/// Create render target view of the back buffer in the swap chain
	{
		Microsoft::WRL::ComPtr<ID3D11Resource> pBackBuffer = nullptr;
		result = pGraphics->GetSwapChain()->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer);
		if (FAILED(result) == TRUE)
		{
			std::wstring errorString = L"IDXGISwapChain::GetBuffer: failed";
			MessageBox(nullptr, errorString.c_str(), L"Error getting swap chain's back buffer", MB_OK | MB_ICONEXCLAMATION);
			return nullptr;
		}

		/// create render target view using the back buffer (must've been created as a render target)
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pRTV = nullptr;
		result = pGraphics->GetDevice()->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &pRTV);
		if (FAILED(result) == TRUE)
		{
			std::wstring errorString = L"ID3D11Device::CreateRenderTargetView: failed";
			MessageBox(nullptr, errorString.c_str(), L"Error creating render target for back buffer in swap chain", MB_OK | MB_ICONEXCLAMATION);
			return nullptr;
		}
		pGraphics->SetBackBufferRTV(pRTV);
	}


	return pGraphics;
}
bool Graphics::DestroyObject() noexcept 
{
	return true;
}

bool Graphics::SwapFrames() noexcept
{
	HRESULT result = m_pSwapChain->Present(1u, NULL);
	if (FAILED(result) == TRUE)
	{
		result = m_pDevice->GetDeviceRemovedReason();
		switch (result) // TODO: specify error reason
		{
		case DXGI_ERROR_DEVICE_HUNG:
			break;
		case DXGI_ERROR_DEVICE_REMOVED:
			break;
		case DXGI_ERROR_DEVICE_RESET:
			break;
		case DXGI_ERROR_DRIVER_INTERNAL_ERROR:
			break;
		case DXGI_ERROR_INVALID_CALL:
			break;
		default:
			break;
		}
		std::wstring errorString = L"IDXGISwapChain::Present: Failed";
		MessageBox(nullptr, errorString.c_str(), L"Error swapping buffers", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	return true;
}

bool Graphics::ClearBackBuffer(float r, float g, float b) noexcept
{
	const float color[] = { r, g, b, 1.0f };
	m_pContext->ClearRenderTargetView(m_pBackBufferRTV.Get(), color);
	return true;
}
bool Graphics::DrawTestTriangle(float angle) noexcept
{
	HRESULT result;

	/// Actual vertices data
	struct Vertex
	{
		struct { float x, y; } Position;
		struct { unsigned char r, g, b, a; } Color;
	};
	const Vertex vertices[]
	{
		{ +0.0f, +0.5f,		255, 000, 000, 255 },
		{ +0.5f, -0.5f,		000, 255, 000, 255 },
		{ -0.5f, -0.5f,		000, 000, 255, 255 },
		{ -0.3f, +0.3f,		255, 255, 000, 255 },
		{ +0.3f, +0.3f,		255, 000, 255, 255 },
		{ +0.0f, -1.0f,		000, 255, 255, 255 }
	};
	const uint16_t indices[]
	{ // must be CW not CCW
		0, 1, 2,
		0, 2, 3,
		0, 4, 1,
		2, 1, 5
	};
	

	/// Load & bind Pixel Shader to pipeline
	{
		Microsoft::WRL::ComPtr<ID3DBlob> pPSBlob = nullptr;
		result = D3DReadFileToBlob(L"Source/SimplePS.cso", &pPSBlob);
		if (FAILED(result) == TRUE)
		{
			std::wstring errorString = L"D3DReadFileToBlob: Failed";
			MessageBox(nullptr, errorString.c_str(), L"Error loading pixel shader", MB_OK | MB_ICONEXCLAMATION);
			return false;
		}
		Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader = nullptr;
		result = m_pDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &pPixelShader);
		if (FAILED(result) == TRUE)
		{
			std::wstring errorString = L"ID3D11Device::CreatePixelShader: Failed";
			MessageBox(nullptr, errorString.c_str(), L"Error creating pixel shader", MB_OK | MB_ICONEXCLAMATION);
			return false;
		}

		m_pContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);
	}


	/// Load & bind Vertex Shader to pipeline
	Microsoft::WRL::ComPtr<ID3DBlob> pVSBlob = nullptr;
	{
		result = D3DReadFileToBlob(L"Source/SimpleVS.cso", &pVSBlob);
		if (FAILED(result) == TRUE)
		{
			std::wstring errorString = L"D3DReadFileToBlob: Failed";
			MessageBox(nullptr, errorString.c_str(), L"Error loading vertex shader", MB_OK | MB_ICONEXCLAMATION);
			return false;
		}
		Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader = nullptr;
		result = m_pDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &pVertexShader);
		if (FAILED(result) == TRUE)
		{
			std::wstring errorString = L"ID3D11Device::CreateVertexShader: Failed";
			MessageBox(nullptr, errorString.c_str(), L"Error creating vertex shader", MB_OK | MB_ICONEXCLAMATION);
			return false;
		}

		m_pContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);
	}

	
	/// Create & Bind Vertex Buffer
	{
		D3D11_SUBRESOURCE_DATA bufferInitialData;
		{
			bufferInitialData.pSysMem = vertices;
		}

		/// Data about the buffer (as a whole) we want to create
		D3D11_BUFFER_DESC bufferDescription;
		{
			bufferDescription.Usage = D3D11_USAGE_DEFAULT;
			bufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;

			bufferDescription.CPUAccessFlags = NULL;
			bufferDescription.MiscFlags = NULL;

			bufferDescription.StructureByteStride = sizeof(Vertex);
			bufferDescription.ByteWidth = sizeof(vertices);
		}

		/// Create vertex buffer
		Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer = nullptr;
		result = m_pDevice->CreateBuffer(&bufferDescription, &bufferInitialData, &pVertexBuffer);
		if (FAILED(result) == TRUE)
		{
			std::wstring errorString = L"ID3D11Device::CreateBuffer: Failed";
			MessageBox(nullptr, errorString.c_str(), L"Error creating vertex buffer", MB_OK | MB_ICONEXCLAMATION);
			return false;
		}

		/// Bind buffer to render pipeline
		const UINT stride = sizeof(Vertex);
		const UINT offset = 0u;
		m_pContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);

	}


	/// Create & Bind Index Buffer
	{
		D3D11_SUBRESOURCE_DATA bufferInitialData;
		{
			bufferInitialData.pSysMem = indices;
		}

		/// Data about the buffer (as a whole) we want to create
		D3D11_BUFFER_DESC bufferDescription;
		{
			bufferDescription.Usage = D3D11_USAGE_DEFAULT;
			bufferDescription.BindFlags = D3D11_BIND_INDEX_BUFFER;

			bufferDescription.CPUAccessFlags = NULL;
			bufferDescription.MiscFlags = NULL;

			bufferDescription.StructureByteStride = sizeof(uint16_t);
			bufferDescription.ByteWidth = sizeof(indices);
		}

		/// Create index buffer
		Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer = nullptr;
		result = m_pDevice->CreateBuffer(&bufferDescription, &bufferInitialData, &pIndexBuffer);
		if (FAILED(result) == TRUE)
		{
			std::wstring errorString = L"ID3D11Device::CreateBuffer: Failed";
			MessageBox(nullptr, errorString.c_str(), L"Error creating index buffer", MB_OK | MB_ICONEXCLAMATION);
			return false;
		}

		/// Bind buffer to render pipeline
		m_pContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
	}


	/// Create & Set Input Layout
	{
		/// Specify how the data is laid out in the vertex buffer
		D3D11_INPUT_ELEMENT_DESC inputElementDescription[]
		{
			{
				"POSITION",						// SemanticName (Vertex shader)
				0,								// SemanticIndex (Vertex shader)

				DXGI_FORMAT_R32G32_FLOAT,		// Format (2 floats)
				0,								// InputSlot (keep it zero for now)
				0,								// AlignedByteOffset
				
				D3D11_INPUT_PER_VERTEX_DATA,	// InputSlotClass
				NULL							// InstanceDataStepRate
			},

			{
				"COLOR",						// SemanticName (Vertex shader)
				0,								// SemanticIndex (Vertex shader)

				DXGI_FORMAT_R8G8B8A8_UNORM,		// Format (4 8-bit types normalized to floats)
				0,								// InputSlot (keep it zero for now)
				8u,								// AlignedByteOffset

				D3D11_INPUT_PER_VERTEX_DATA,	// InputSlotClass
				NULL							// InstanceDataStepRate
			}
		};

		Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
		result = m_pDevice->CreateInputLayout(inputElementDescription, std::size(inputElementDescription), pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &pInputLayout);
		m_pContext->IASetInputLayout(pInputLayout.Get());

		m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}


	/// Create Transforation Matrix Constant Buffer
	{
		struct ConstantBuffer {
			struct { float matrix[4][4]; } Transformation;
		};

		const ConstantBuffer constBuffer
		{
			{ // Rotation Matrix
				m_inverseAspectRatio * +std::cos(angle),	+std::sin(angle),	0.0f,	0.0f,
				m_inverseAspectRatio * -std::sin(angle),	+std::cos(angle),	0.0f,	0.0f,
				+0.0f,										0.0f,				1.0f,	0.0f,
				+0.0f,										0.0f,				0.0f,	1.0f
			}
		};
	
		D3D11_SUBRESOURCE_DATA bufferInitialData;
		{
			bufferInitialData.pSysMem = &constBuffer;
		}

		/// Data about the buffer (as a whole) we want to create
		D3D11_BUFFER_DESC bufferDescription;
		{
			bufferDescription.Usage = D3D11_USAGE_DYNAMIC;
			bufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

			bufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // dynamic usage
			bufferDescription.MiscFlags = NULL;

			bufferDescription.StructureByteStride = NULL; // not an array
			bufferDescription.ByteWidth = sizeof(constBuffer);
		}

		/// Create constant buffer
		Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer = nullptr;
		result = m_pDevice->CreateBuffer(&bufferDescription, &bufferInitialData, &pConstantBuffer);
		if (FAILED(result) == TRUE)
		{
			std::wstring errorString = L"ID3D11Device::CreateBuffer: Failed";
			MessageBox(nullptr, errorString.c_str(), L"Error creating constant buffer", MB_OK | MB_ICONEXCLAMATION);
			return false;
		}

		/// Bind buffer to render pipeline
		m_pContext->VSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());
	}


	/// Bind Render Target
	{
		m_pContext->OMSetRenderTargets(1u, m_pBackBufferRTV.GetAddressOf(), nullptr);
	}


	/// Configure Viewport
	{
		D3D11_VIEWPORT viewport;
		{ /// properties relative to render target view
			viewport.TopLeftX = 0.0f;
			viewport.TopLeftY = 0.0f;
			
			viewport.Width = 800.0f;
			viewport.Height = 600.0f;
			
			viewport.MinDepth = 0.0f;
			viewport.MaxDepth = 1.0f;
		}
		
		m_pContext->RSSetViewports(1u, &viewport);
	}


	/// Issue draw command
	{
		dxgiInfoManager.Mark();
		m_pContext->DrawIndexed(std::size(indices), 0u, 0u);
		std::vector<std::string> messages = dxgiInfoManager.GetMessages();
		if (!messages.empty())
		{
			std::wstring errorString = L"Errors(" + std::to_wstring(messages.size()) + L"):\n\n";
			for (auto& line : messages) errorString += std::wstring(line.begin(), line.end()) + L"\n\n";
			MessageBox(nullptr, errorString.c_str(), L"ID3D11DeviceContext::Draw: Failed", MB_OK | MB_ICONEXCLAMATION);
			return false;
		}
	}


	return true;
}


Microsoft::WRL::ComPtr<ID3D11Device> Graphics::GetDevice() const
{
	return m_pDevice;
}
Microsoft::WRL::ComPtr<IDXGISwapChain> Graphics::GetSwapChain() const
{
	return m_pSwapChain;
}
Microsoft::WRL::ComPtr<ID3D11DeviceContext> Graphics::GetContext() const
{
	return m_pContext;
}


void Graphics::SetDevice(Microsoft::WRL::ComPtr<ID3D11Device> ppDevice) noexcept
{
	m_pDevice = ppDevice;
}
void Graphics::SetSwapChain(Microsoft::WRL::ComPtr<IDXGISwapChain> ppSwapChain) noexcept
{
	m_pSwapChain = ppSwapChain;
}
void Graphics::SetContext(Microsoft::WRL::ComPtr<ID3D11DeviceContext> ppContext) noexcept
{
	m_pContext = ppContext;
}

void Graphics::SetBackBufferRTV(Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pBackBufferRTV) noexcept
{
	m_pBackBufferRTV = pBackBufferRTV;
}