#include "Graphics.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

Graphics::Graphics(HWND windowHandle)
{
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
		swapChainDescription.BufferCount = 1; // num of back buffers

		/// antialiasing data
		swapChainDescription.SampleDesc.Count = 1;
		swapChainDescription.SampleDesc.Quality = 0;

		/// window data
		swapChainDescription.OutputWindow = windowHandle;
		swapChainDescription.Windowed = TRUE;

		/// miscellaneous
		swapChainDescription.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
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
		&m_pSwapChain,				// output pointer to swap chain
		&m_pDevice,					// output pointer to device
		nullptr,					// output pointer with chosen feature level
		&m_pContext					// output pointer to context
	);
	if (FAILED(result) == TRUE)
	{
		ErrorHandler::ErrorBox(L"Error Creating Device/SwapChain/DeviceContext", result, __FILE__, __LINE__);
		throw - 1;
	}


	/// Create render target view of the back buffer in the swap chain
	{
		Microsoft::WRL::ComPtr<ID3D11Resource> pBackBuffer = nullptr;
		result = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer);
		if (FAILED(result) == TRUE)
		{
			ErrorHandler::ErrorBox(L"Error Creating Buffer", result, __FILE__, __LINE__);
			throw - 1;
		}

		/// create render target view using the back buffer (must've been created as a render target)
		result = m_pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &m_pBackBufferRTV);
		if (FAILED(result) == TRUE)
		{
			ErrorHandler::ErrorBox(L"Error Creating Render Target View", result, __FILE__, __LINE__);
			throw - 1;
		}
	}


	/// Create depth/stencil buffer (z-buffer or texture) & Create target view of the buffer
	{
		/// Create & Bind depth/stencil state
		{
			D3D11_DEPTH_STENCIL_DESC dsStateDescription = {};
			{
				dsStateDescription.DepthEnable = TRUE;
				dsStateDescription.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
				dsStateDescription.DepthFunc = D3D11_COMPARISON_LESS;

				//dsStateDescription.StencilEnable = FALSE;
				//dsStateDescription.StencilReadMask = NULL;
				//dsStateDescription.StencilWriteMask = NULL;
			}

			Microsoft::WRL::ComPtr<ID3D11DepthStencilState> pDsState;
			result = m_pDevice->CreateDepthStencilState(&dsStateDescription, &pDsState);
			if (FAILED(result) == TRUE)
			{
				ErrorHandler::ErrorBox(L"Error Creating depth/stencil buffer", result, __FILE__, __LINE__);
				throw - 1;
			}

			m_pContext->OMSetDepthStencilState(pDsState.Get(), 1u);
		}

		/// Create depth/stencil buffer
		Microsoft::WRL::ComPtr<ID3D11Texture2D> pDsBuffer;
		{
			D3D11_TEXTURE2D_DESC dsBufferDescription = {};
			{
				dsBufferDescription.Format = DXGI_FORMAT_D32_FLOAT;

				dsBufferDescription.Usage = D3D11_USAGE_DEFAULT;
				dsBufferDescription.BindFlags = D3D11_BIND_DEPTH_STENCIL;

				dsBufferDescription.CPUAccessFlags = NULL;
				dsBufferDescription.MiscFlags = NULL;

				dsBufferDescription.Width = 800u;		// same as swap chain back buffer
				dsBufferDescription.Height = 600u;		// same as swap chain back buffer

				dsBufferDescription.MipLevels = 1u;
				dsBufferDescription.ArraySize = 1u;		// number of 2D textures created

				/// anti-aliasing bs
				dsBufferDescription.SampleDesc.Count = 1u;
				dsBufferDescription.SampleDesc.Quality = 0u;
			}

			m_pDevice->CreateTexture2D(&dsBufferDescription, nullptr, &pDsBuffer);
		}

		/// Create target view of the depth/stencil buffer (must've been created as a depth/stencil buffer)
		{
			D3D11_DEPTH_STENCIL_VIEW_DESC dsvDescription = {};
			{
				dsvDescription.Format = DXGI_FORMAT_D32_FLOAT;
				dsvDescription.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
				dsvDescription.Flags = 0u;
				
				dsvDescription.Texture2D.MipSlice = 0u;
			}

			result = m_pDevice->CreateDepthStencilView(pDsBuffer.Get(), &dsvDescription, &m_pDepthBufferDSV);
			if (FAILED(result) == TRUE)
			{
				ErrorHandler::ErrorBox(L"Error Creating Depth Stencil View", result, __FILE__, __LINE__);
				throw - 1;
			}
		}
	}


	/// Bind render target view & depth stencil view to the pipeline
	m_pContext->OMSetRenderTargets(1u, m_pBackBufferRTV.GetAddressOf(), m_pDepthBufferDSV.Get());

	/// Create & Bind viewport
	D3D11_VIEWPORT viewport;
	{
		viewport.Width = 800.0f;
		viewport.Height = 600.0f;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;
	}
	m_pContext->RSSetViewports(1u, &viewport);
}

void Graphics::SwapFrames()
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
		throw - 1;
	}
}

DirectX::XMMATRIX Graphics::GetProjection() noexcept
{
	return m_projMatrix;
}
void Graphics::SetProjection(DirectX::XMMATRIX projMatrix) noexcept
{
	m_projMatrix = projMatrix;
}

void Graphics::DrawIndexed(uint32_t count)
{
	dxgiInfoManager.Mark();
	{
		m_pContext->DrawIndexed(count, 0u, 0u);
	}
	std::vector<std::string> errorMessages = dxgiInfoManager.GetMessages();
	if (!errorMessages.empty())
	{
		ErrorHandler::ErrorBox(L"Error Issuing Indexed Draw Command", errorMessages, __FILE__, __LINE__);
		throw - 1;
	}
}

void Graphics::ClearBackBuffer(float r, float g, float b) noexcept
{
	const float color[] = { r, g, b, 1.0f };
	m_pContext->ClearRenderTargetView(m_pBackBufferRTV.Get(), color);
	m_pContext->ClearDepthStencilView(m_pDepthBufferDSV.Get(), D3D11_CLEAR_DEPTH, 1u, 0u);
}
void Graphics::DrawTestTriangle(float angle, float x, float y)
{

	if (m_bFirstPass)
	{
		m_bFirstPass = false;

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
			{ -0.5f, -0.5f,		000, 000, 255, 255 }
		};
		const uint16_t indices[]
		{ // must be CW not CCW
			0, 1, 2,
		};


		/// Load & Bind Pixel Shader to pipeline
		{
			Microsoft::WRL::ComPtr<ID3DBlob> pPSBlob = nullptr;
			result = D3DReadFileToBlob(L"Source/SimplePS.cso", &pPSBlob);
			if (FAILED(result) == TRUE)
			{
				ErrorHandler::ErrorBox(L"Error Reading Pixel Shader File", result, __FILE__, __LINE__);
				throw - 1;
			}

			Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader = nullptr;
			result = m_pDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &pPixelShader);
			if (FAILED(result) == TRUE)
			{
				ErrorHandler::ErrorBox(L"Error Creating Pixel Shader Object", result, __FILE__, __LINE__);
				throw - 1;
			}

			m_pContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);
		}


		/// Load & bind Vertex Shader to pipeline
		Microsoft::WRL::ComPtr<ID3DBlob> pVSBlob = nullptr;
		{
			result = D3DReadFileToBlob(L"Source/SimpleVS.cso", &pVSBlob);
			if (FAILED(result) == TRUE)
			{
				ErrorHandler::ErrorBox(L"Error Reading Vertex Shader File", result, __FILE__, __LINE__);
				throw - 1;
			}

			Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader = nullptr;
			result = m_pDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &pVertexShader);
			if (FAILED(result) == TRUE)
			{
				ErrorHandler::ErrorBox(L"Error Creating Vertex Shader Object", result, __FILE__, __LINE__);
				throw - 1;
			}

			m_pContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);
		}


		/// Create & Bind Vertex Buffer
		{
			D3D11_SUBRESOURCE_DATA bufferInitialData = {};
			{
				bufferInitialData.pSysMem = vertices;
			}

			/// Data about the buffer (as a whole) we want to create
			D3D11_BUFFER_DESC bufferDescription = {};
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
				ErrorHandler::ErrorBox(L"Error Creating Vertex Buffer", result, __FILE__, __LINE__);
				throw - 1;
			}

			/// Bind buffer to render pipeline
			const UINT stride = sizeof(Vertex);
			const UINT offset = 0u;
			m_pContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);
		}


		/// Create & Bind Index Buffer
		{
			D3D11_SUBRESOURCE_DATA bufferInitialData = {};
			{
				bufferInitialData.pSysMem = indices;
			}

			/// Data about the buffer (as a whole) we want to create
			D3D11_BUFFER_DESC bufferDescription = {};
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
				ErrorHandler::ErrorBox(L"Error Creating Index Buffer", result, __FILE__, __LINE__);
				throw - 1;
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
			result = m_pDevice->CreateInputLayout(inputElementDescription, (UINT)std::size(inputElementDescription), pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &pInputLayout);
			if (FAILED(result) == TRUE)
			{
				ErrorHandler::ErrorBox(L"Error Creating Input Layout Object", result, __FILE__, __LINE__);
				throw - 1;
			}

			m_pContext->IASetInputLayout(pInputLayout.Get());

			m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		}


		/// Create Transformation Matrix Constant Buffer
		{
			struct ConstantBuffer {
				DirectX::XMMATRIX transform;
			};

			const ConstantBuffer constBuffer
			{
				DirectX::XMMatrixTranspose(
					DirectX::XMMatrixIdentity()
					/*DirectX::XMMatrixMultiply(
						DirectX::XMMatrixRotationZ(angle),
						DirectX::XMMatrixTranslation(x, y, 0.0f)
					)*/
				)
			};

			D3D11_SUBRESOURCE_DATA bufferInitialData = {};
			{
				bufferInitialData.pSysMem = &constBuffer;
			}

			/// Data about the buffer (as a whole) we want to create
			D3D11_BUFFER_DESC bufferDescription = {};
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
				ErrorHandler::ErrorBox(L"Error Creating Constant Buffer Object", result, __FILE__, __LINE__);
				throw - 1;
			}

			/// Bind buffer to render pipeline
			m_pContext->VSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());
		}


		/// Configure Viewport
		{
			/*D3D11_VIEWPORT viewport;
			{ /// properties relative to render target view
				viewport.TopLeftX = 0.0f;
				viewport.TopLeftY = 0.0f;

				viewport.Width = 800.0f;
				viewport.Height = 600.0f;

				viewport.MinDepth = 0.0f;
				viewport.MaxDepth = 1.0f;
			}

			m_pContext->RSSetViewports(1u, &viewport);*/
		}

	}
	
	
	/// Issue draw command
	{
		DrawIndexed(3u);
		/*dxgiInfoManager.Mark();
		m_pContext->DrawIndexed((UINT)std::size(indices), 0u, 0u);
		std::vector<std::string> errorMessages = dxgiInfoManager.GetMessages();
		if (!errorMessages.empty())
		{
			ErrorHandler::ErrorBox(L"Error Issuing Draw Command", errorMessages, __FILE__, __LINE__);
			throw -1;
		}*/
	}
}
void Graphics::DrawTestCube(float angle, float x, float y) noexcept
{}

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