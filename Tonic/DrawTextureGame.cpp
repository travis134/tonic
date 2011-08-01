#include "DrawTextureGame.h"
#include <xnamath.h>
struct Vertex
{
	XMFLOAT3 position;
	XMFLOAT2 texcoord;
};


DrawTextureGame::DrawTextureGame(void) : solidColorVS_(0), solidColorPS_(0), inputLayout_(0), vertexBuffer_(0)
{
}


DrawTextureGame::~DrawTextureGame(void)
{
}


bool DrawTextureGame::LoadContent(void)
{
	//Vertex Shader
	ID3DBlob* vsBuffer = 0;

	bool compileResult = CompileD3DShader(
		"Decal2D.fx",
		"VS_Main",
		"vs_4_0",
		&vsBuffer
		);

	if(compileResult == false)
	{
		MessageBox(
			0,
			"Couldn't load the vertex shader.",
			"Compile Error",
			MB_OK
			);
		return false;
	}

	HRESULT d3dResult;

	d3dResult = this->d3dDevice_->CreateVertexShader(
		vsBuffer->GetBufferPointer(),
		vsBuffer->GetBufferSize(),
		0,
		&this->solidColorVS_
		);

	if(FAILED(d3dResult))
	{
		if(vsBuffer)
		{
			vsBuffer->Release();
		}
		return false;
	}

	D3D11_INPUT_ELEMENT_DESC solidColorLayout[] =
	{
		{
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			0,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
		{
			"TEXCOORD",
			0,
			DXGI_FORMAT_R32G32_FLOAT,
			0,
			12,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		}
	};

	unsigned int totalLayoutElements = ARRAYSIZE(solidColorLayout);

	d3dResult = this->d3dDevice_->CreateInputLayout(
		solidColorLayout,
		totalLayoutElements,
		vsBuffer->GetBufferPointer(),
		vsBuffer->GetBufferSize(),
		&this->inputLayout_
		);

	vsBuffer->Release();

	if(FAILED(d3dResult))
	{
		return false;
	}

	//Pixel Shader
	ID3DBlob* psBuffer = 0;

	compileResult = CompileD3DShader(
		"Decal2D.fx",
		"PS_Main",
		"ps_4_0",
		&psBuffer
		);

	if(compileResult == false)
	{
		MessageBox(
			0,
			"Couldn't load the pixel shader.",
			"Compile Error",
			MB_OK
			);
		return false;
	}

	d3dResult = this->d3dDevice_->CreatePixelShader(
		psBuffer->GetBufferPointer(),
		psBuffer->GetBufferSize(),
		0,
		&this->solidColorPS_
		);

	psBuffer->Release();

	if(FAILED(d3dResult))
	{
		return false;
	}

	//Vertex Buffer
	Vertex vertices[] =
	{
		{XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f)},
		{XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f)},
		{XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f)},
		{XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f)},
		{XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f)},
		{XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f)}
	};

	unsigned int totalVertices = ARRAYSIZE(vertices);

	D3D11_BUFFER_DESC vertexDesc;
	ZeroMemory(&vertexDesc, sizeof(vertexDesc));
	vertexDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexDesc.ByteWidth = sizeof(Vertex) * totalVertices;

	D3D11_SUBRESOURCE_DATA resourceData;
	ZeroMemory(&resourceData, sizeof(resourceData));
	resourceData.pSysMem = vertices;

	d3dResult = this->d3dDevice_->CreateBuffer(
		&vertexDesc,
		&resourceData,
		&this->vertexBuffer_
		);

	if(FAILED(d3dResult))
	{
		return false;
	}

	d3dResult = D3DX11CreateShaderResourceViewFromFile(
		this->d3dDevice_,
		"decal.dds",
		0,
		0,
		&this->colorMap_,
		0
		);

	if(FAILED(d3dResult))
	{
		DXTRACE_MSG("Couldn't load the texture image.");
		return false;
	}

	D3D11_SAMPLER_DESC colorMapDesc;
	ZeroMemory(&colorMapDesc, sizeof(colorMapDesc));
	colorMapDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	colorMapDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	colorMapDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	colorMapDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	colorMapDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	colorMapDesc.MaxLOD = D3D11_FLOAT32_MAX;

	d3dResult = this->d3dDevice_->CreateSamplerState(&colorMapDesc, &this->colorMapSampler_);
	
	if(FAILED(d3dResult))
	{
		DXTRACE_MSG("Couldn't create color map sampler state.");
		return false;
	}

	return true;
}


void DrawTextureGame::UnloadContent(void)
{
	if(this->solidColorVS_)
	{
		this->solidColorVS_->Release();
	}
	if(this->solidColorPS_)
	{
		this->solidColorPS_->Release();
	}
	if(this->inputLayout_)
	{
		this->inputLayout_->Release();
	}
	if(this->vertexBuffer_)
	{
		this->vertexBuffer_->Release();
	}
	if(this->colorMap_)
	{
		this->colorMap_->Release();
	}
	if(this->colorMapSampler_)
	{
		this->colorMapSampler_->Release();
	}

	this->solidColorVS_ = 0;
	this->solidColorPS_ = 0;
	this->inputLayout_ = 0;
	this->vertexBuffer_ = 0;
	this->colorMap_ = 0;
	this->colorMapSampler_ = 0;
}


void DrawTextureGame::Update(float gameTime)
{
}


void DrawTextureGame::Draw(float gameTime)
{
	if(this->d3dContext_ == 0)
	{
		return;
	}

	float clearColor[4] = {0.392f, 0.584f, 0.929f, 1.000f};

	this->d3dContext_->ClearRenderTargetView(this->backBufferTarget_, clearColor);

	unsigned int stride = sizeof(Vertex);
	unsigned int offset = 0;

	this->d3dContext_->IASetInputLayout(this->inputLayout_);
	this->d3dContext_->IASetVertexBuffers(
		0,
		1,
		&this->vertexBuffer_,
		&stride,
		&offset
		);

	this->d3dContext_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	this->d3dContext_->VSSetShader(
		this->solidColorVS_,
		0,
		0
		);
	this->d3dContext_->PSSetShader(
		this->solidColorPS_,
		0,
		0
		);
	this->d3dContext_->PSSetShaderResources(
		0,
		1,
		&this->colorMap_
		);
	this->d3dContext_->PSSetSamplers(
		0,
		1,
		&this->colorMapSampler_
		);
	this->d3dContext_->Draw(6, 0);

	this->swapChain_->Present(0, 0);
}