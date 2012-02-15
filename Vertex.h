//=============================================================================
// Vertex.h by Frank Luna (C) 2005 All Rights Reserved.
//
// This file contains all of our vertex structures.  We will add to it
// as needed throughout the book.
//=============================================================================

#ifndef VERTEX_H
#define VERTEX_H

#include <d3dx9.h>

// Call in constructor and destructor, respectively, of derived application class.
void InitAllVertexDeclarations();
void DestroyAllVertexDeclarations();

//==============================================================
struct RectVertex
{
	RectVertex():pos(0.0f, 0.0f, 0.0f), color(0x00000000){}
	RectVertex(float x, float y, float z):pos(x,y,z){}
	RectVertex(const D3DXVECTOR3& v):pos(v){}

	D3DXVECTOR3 pos;
	D3DCOLOR color;
	static IDirect3DVertexDeclaration9* Decl;
};

//===============================================================
struct TextureVertex
{
	TextureVertex():pos(0.0f, 0.0f, 0.0f){}
	TextureVertex(float x, float y, float z):pos(x,y,z){}
	TextureVertex(const D3DXVECTOR3& v):pos(v){}
	
	D3DXVECTOR3 pos;
	D3DXVECTOR2 tex0;
	static IDirect3DVertexDeclaration9* Decl;
};

#endif