#include "BlankScreenGame.h"


BlankScreenGame::BlankScreenGame(void)
{
}


BlankScreenGame::~BlankScreenGame(void)
{
}


bool BlankScreenGame::LoadContent(void)
{
	return true;
}


void BlankScreenGame::UnloadContent(void)
{
}


void BlankScreenGame::Update(float gameTime)
{
}


void BlankScreenGame::Draw(float gameTime)
{
	if(this->d3dContext_ == 0)
	{
		return;
	}

	float clearColor[4] = {0.392f, 0.584f, 0.929f, 1.000f};

	this->d3dContext_->ClearRenderTargetView(this->backBufferTarget_, clearColor);

	this->swapChain_->Present(0, 0);
}