#ifndef _GAME_H_
#define _GAME_H_
#include <D3D11.h>
#include <D3DX11.h>
#include <DxErr.h>
#include <D3Dcompiler.h>


class Game
{
public:
	Game(void);
	virtual ~Game(void);
	bool Initialize(HINSTANCE hInstance, HWND hwnd);
	void Finalize(void);
	virtual bool LoadContent(void);
	virtual void UnloadContent(void);
	virtual void Update(float gameTime);
	virtual void Draw(float gameTime);
	bool CompileD3DShader(char* filePath, char* entry, char* shaderModel, ID3DBlob** buffer);
protected:
	HINSTANCE hInstance_;
	HWND hwnd_;
	D3D_DRIVER_TYPE driverType_;
	D3D_FEATURE_LEVEL featureLevel_;
	ID3D11Device* d3dDevice_;
	ID3D11DeviceContext* d3dContext_;
	IDXGISwapChain* swapChain_;
	ID3D11RenderTargetView* backBufferTarget_;
};

#endif