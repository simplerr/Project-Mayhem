#include "Font.h"
#include "Graphics.h"

Font::Font()
{
	loadFont("Data\\imgs\\font.bmp");
	mCharacters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ 0123456789.,!?'\"-+=/\\%()<>:;";

	// Load the effect
	// Create the FX from a .fx file
	ID3DXBuffer* errors = 0;
	HR(D3DXCreateEffectFromFile(gd3dDevice, "font_effect.fx", 
		0, 0, D3DXSHADER_DEBUG, 0, &mEffect, &errors));	// Weird that it needs common\\fxs\\..... :NOTE:
	if( errors ) 
		MessageBox(0, (char*)errors->GetBufferPointer(), 0, 0);

	mhTech = mEffect->GetTechniqueByName("FontTech");
	mhColor = mEffect->GetParameterByName(0, "gColor");
}
	
Font::~Font()
{
	ReleaseCOM(mTexture);
	ReleaseCOM(mEffect);
}

bool Font::loadFont(string source)
{
	mTexture = gGraphics->loadTexture(source, 0xff000000);	// Black as color key
	return true;
}
	
void Font::draw(string text, int x, int y, int size, DWORD color)
{
	Vector c;
	c.x = (0x00ff0000 & color) >> 16;	// Red
	c.y = (0x0000ff00 & color) >> 8;	// Green
	c.z = (0xff & color);				// Blue

	HR(mEffect->SetTechnique(mhTech));
	mEffect->SetValue(mhColor, &c, sizeof(Vector));

	UINT numPasses = 0;
	HR(mEffect->Begin(&numPasses, 0));
	for(UINT i = 0; i < numPasses; ++i)
	{
		HR(mEffect->BeginPass(i));

		for(int i = 0; i < text.size(); i++)	{  
			int pos = mCharacters.find(toupper(text[i]));

			if(pos <= 25)	{
				Rect sourceRect((float)pos * 8 + 1, (float)pos * 8 + 8, (float)0, (float)7);
				gGraphics->drawAnimation(mTexture, x + (size + 2) * i, y, size, size, &sourceRect);
			}
			else if(pos > 25)	{
				pos -= 27;
				Rect sourceRect((float)pos * 8 + 1, (float)pos * 8 + 8, (float)8, (float)15);
				gGraphics->drawAnimation(mTexture, x + (size + 2) *i, y, size, size, &sourceRect);
			}
		}

		HR(mEffect->EndPass());
	}

	HR(mEffect->End());
}

void Font::onLostDevice()
{
	mEffect->OnLostDevice();
}
	
void Font::onResetDevice()
{
	mEffect->OnResetDevice();
}