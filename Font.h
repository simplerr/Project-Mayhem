#ifndef FONT_H
#define FONT_H

#include <string>
#include "d3dUtil.h"
using namespace std;

//! Custom font wrapper.
class Font
{
public:
	Font();
	~Font();

	bool loadFont(string source);
	void draw(string text, int x, int y, int size = 10, DWORD color = 0xffffffff);

	void onLostDevice();
	void onResetDevice();
private:
	IDirect3DTexture9* mTexture;
	string mCharacters;
	ID3DXEffect* mEffect;
	D3DXHANDLE mhTech;
	D3DXHANDLE mhColor;
};

#endif