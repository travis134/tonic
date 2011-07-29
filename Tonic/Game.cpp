




#include "Game.h"


Game::Game(void) : driverType_(D3D_DRIVER_TYPE_NULL), featureLevel_(D3D_FEATURE_LEVEL_11_0), d3dDevice_(0), d3dContext_(0), swapChain_(0), backBufferTarget_(0)
{
}


Game::~Game(void)
{
	this->Finalize();
}


bool Game::Initialize(HINSTANCE hInstance, HWND hwnd)
{
	this->hInstance_ = hInstance;
	this->hwnd_ = hwnd;

	RECT dimensions;
	GetClientRect(this->hwnd_, &dimensions);
	
	unsigned int width = dimensions.right - dimensions.left;
	unsigned int height = dimensions.bottom - dimensions.top;

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_SOFTWARE
	};

	unsigned int totalDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0
	};

	unsigned int totalFeatureLevels = ARRAYSIZE(featureLevels);

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hwnd;
	swapChainDesc.Windowed = true;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	unsigned int creationFlags = 0;

#ifdef _DEBUG
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	HRESULT result;
	unsigned int driver = 0;

	for(driver = 0; driver < totalDriverTypes; ++driver)
	{
		result = D3D11CreateDeviceAndSwapChain(
			0,
			driverTypes[driver],
			0,
			creationFlags,
			featureLevels,
			totalFeatureLevels,
			D3D11_SDK_VERSION,
			&swapChainDesc,
			&this->swapChain_,
			&this->d3dDevice_,
			&this->featureLevel_,
			&this->d3dContext_
			);

		if(SUCCEEDED(result))
		{
			this->driverType_ = driverTypes[driver];
			break;
		}
	}

	if(FAILED(result))
	{
		DXTRACE_MSG("Couldn't create Direct3D device.");
		return false;
	}

	ID3D11Texture2D* backBufferTexture;
	result = this->swapChain_->GetBuffer(
		0,
		__uuidof(ID3D11Texture2D),
		(LPVOID*)&backBufferTexture
		);

	if(FAILED(result))
	{
		DXTRACE_MSG("Couldn't get swap chain back buffer.");
		return false;
	}

	result = this->d3dDevice_->CreateRenderTargetView(
		backBufferTexture,
		0,
		&this->backBufferTarget_
		);

	if(backBufferTexture)
	{
		backBufferTexture->Release();
	}

	if(FAILED(result))
	{
		DXTRACE_MSG("Couldn't create render target view.");
		return false;
	}

	this->d3dContext_->OMSetRenderTargets(
		1,
		&this->backBufferTarget_,
		0
		);

	D3D11_VIEWPORT viewport;
	viewport.Width = static_cast<float>(width);
	viewport.Height = static_cast<float>(height);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	this->d3dContext_->RSSetViewports(1, &viewport);

	return LoadContent();
}


bool Game::LoadContent(void)
{
	//Load Game Assets
	return true;
}


void Game::UnloadContent(void)
{
	//Unload Game Assets
}


void Game::Update(float gameTime)
{
}


void Game::Draw(float gameTime)
{
}


void Game::Finalize(void)
{
	this->UnloadContent();
	if(this->backBufferTarget_)
	{
		this->backBufferTarget_->Release();
	}
	if(this->swapChain_)
	{
		this->swapChain_->Release();
	}
	if(this->d3dContext_)
	{
		this->d3dContext_->Release();
	}
	if(this->d3dDevice_)
	{
		this->d3dDevice_->Release();
	}
	this->d3dDevice_ = 0;
	this->d3dContext_ = 0;
	this->swapChain_ = 0;
	this->backBufferTarget_ = 0;
}