#include "DrawTriangleGame.h"
#include <xnamath.h>
struct Vertex
{
	XMFLOAT3 position;
};


DrawTriangleGame::DrawTriangleGame(void) : solidColorVS_(0), solidColorPS_(0), inputLayout_(0), vertexBuffer_(0)
{
}


DrawTriangleGame::~DrawTriangleGame(void)
{
}


bool DrawTriangleGame::LoadContent(void)
{
	//Vertex Shader
	ID3DBlob* vsBuffer = 0;

	bool compileResult = CompileD3DShader(
		"SolidRedColor.fx",
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
		"SolidRedColor.fx",
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
		XMFLOAT3(0.5f, 0.5f, 0.5f),
		XMFLOAT3(0.5f, -0.5f, 0.5f),
		XMFLOAT3(-0.5f, -0.5f, 0.5f)
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

	return true;
}


void DrawTriangleGame::UnloadContent(void)
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

	this->solidColorVS_ = 0;
	this->solidColorPS_ = 0;
	this->inputLayout_ = 0;
	this->vertexBuffer_ = 0;
}


void DrawTriangleGame::Update(float gameTime)
{
}


void DrawTriangleGame::Draw(float gameTime)
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
	this->d3dContext_->Draw(3, 0);

	this->swapChain_->Present(0, 0);
}