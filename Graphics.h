#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <crtdbg.h>
#include <string>

#include "d3dUtil.h"
#include "cPolygon.h"
#include "Rect.h"
#include "Effects.h"
#include "Font.h"

using namespace std;

//! Determines which type of text to draw.
enum TextType
{
	CUSTOM,
	SMALL_DX,
	BIG_DX
};

//! Wrapper for drawing with DirectX.
class Graphics
{
public:
	//! Constructor that sets up the neccessary members.
	Graphics();

	//! Destructor that for cleanin up.
	~Graphics();

	//! Loads and returns a texture from the specified source.
	IDirect3DTexture9* loadTexture(string filename, DWORD colorKey = 0);

	//! Loads the ID3DXFont member.
	void loadDxFont(void);

	//! Builds the static vertex buffer.
	void buildFastBuffer();

	//! Draws a solid or wireframed rectangle.
	void drawRect(float x, float y, int width, int height, D3DCOLOR fillColor = D3DCOLOR_ARGB( 255, 255 , 000, 000 ), bool solid = true);	//:TODO: Fix z and solid!

	//! Draws a solid or wireframed rectangle.
	void drawRect(Rect rect, D3DCOLOR fillColor = D3DCOLOR_ARGB( 155, 155, 000, 000 ), bool solid = true);	
	
	//! Draws a textured rectangle.
	void drawTexture(IDirect3DTexture9 *texture, Rect rect, float rotation = 0.0f);

	//! Draws a textured rectangle.
	void drawTexture(IDirect3DTexture9 *texture, float x, float y, int width, int height, float rotation = 0.0f, Rect* sourceRect = NULL, bool flipped = false);

	//! Draws a part from a texture atlas.
	void drawAnimation(IDirect3DTexture9* texture, float x, float y, int width, int height, Rect *srcRect = NULL, bool flipped = false);

	//! Draws a textured polygon, supports animation.
	bool drawTexturedPolygon(cPolygon shape, IDirect3DTexture9 *texture, Rect *srcRect = NULL, bool flipped = false);

	//! Draws a thin line.
	void drawLine(Vector start, Vector end, float thickness = 20.0f, D3DCOLOR color = D3DCOLOR_ARGB(255, 0, 0, 0));

	void drawTexturedLine(IDirect3DTexture9* texture, Vector start, Vector end, float thickness = 20.0f);

	//! Returns the sprite handler.
	LPD3DXSPRITE getSpriteHandler();

	//! Checks device caps.
	bool checkDeviceCaps();

	//! Draw text with either DX font or cusom font.
	void drawText(string text, int x, int y, TextType type = CUSTOM, int size = 8, DWORD color = 0xff000000);

	//! Draws text using ID3DXFont.
	void drawSmallDxFont(string text, int x, int y, DWORD color = 0xff000000);
	void drawBigDxFont(string text, int x, int y, DWORD color = 0xff000000);

	//! Draws a custom font.
	void drawCustomFont(string text, int x, int y, int size = 8, DWORD color = 0xff000000);

	void onLostDevice();
	void onResetDevice();
private:
	IDirect3DVertexBuffer9* mVB_texture;
	IDirect3DVertexBuffer9* mVB_Rect;
	IDirect3DVertexBuffer9* mVB_fast;
	ID3DXFont* mFont;
	ID3DXFont* mBigFont;

	LPD3DXSPRITE mSpriteHandler;

	Font* mCustomFont;

	// Matrices for drawing
	D3DXMATRIX T, S, R;
};	// Class

extern Graphics* gGraphics;

#endif