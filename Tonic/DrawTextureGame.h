#ifndef _DRAW_TEXTURE_GAME_
#define _DRAW_TEXTURE_GAME_
#include "Game.h"


class DrawTextureGame : public Game
{
public:
	DrawTextureGame(void);
	virtual ~DrawTextureGame(void);
	bool LoadContent(void);
	void UnloadContent(void);
	void Update(float gameTime);
	void Draw(float gameTime);
private:
	ID3D11VertexShader* solidColorVS_;
	ID3D11PixelShader* solidColorPS_;
	ID3D11InputLayout* inputLayout_;
	ID3D11Buffer* vertexBuffer_;
	ID3D11ShaderResourceView* colorMap_;
	ID3D11SamplerState* colorMapSampler_;
};

#endif