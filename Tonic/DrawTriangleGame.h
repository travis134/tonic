#ifndef _DRAW_TRIANGLE_GAME_
#define _DRAW_TRIANGLE_GAME_
#include "Game.h"


class DrawTriangleGame : public Game
{
public:
	DrawTriangleGame(void);
	virtual ~DrawTriangleGame(void);
	bool LoadContent(void);
	void UnloadContent(void);
	void Update(float gameTime);
	void Draw(float gameTime);
private:
	ID3D11VertexShader* solidColorVS_;
	ID3D11PixelShader* solidColorPS_;
	ID3D11InputLayout* inputLayout_;
	ID3D11Buffer* vertexBuffer_;
};

#endif