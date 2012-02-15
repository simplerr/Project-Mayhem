//=============================================================================
// Vertex.cpp by Frank Luna (C) 2005 All Rights Reserved.
//=============================================================================

#include "Vertex.h"
#include "d3dUtil.h"

// Initialize static variables.
IDirect3DVertexDeclaration9* RectVertex::Decl = 0;
IDirect3DVertexDeclaration9* TextureVertex::Decl = 0;


void InitAllVertexDeclarations()
{
	//==============================================================
	// VertexPos

	D3DVERTEXELEMENT9 RectVertexElements[] = 
	{
		{0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
		D3DDECL_END()
	};	
	HR(gd3dDevice->CreateVertexDeclaration(RectVertexElements, &RectVertex::Decl));

	D3DVERTEXELEMENT9 TextureVertexElememts[] = 
	{
		{0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
		D3DDECL_END() 
	};
	HR(gd3dDevice->CreateVertexDeclaration(TextureVertexElememts, &TextureVertex::Decl));

	/*D3DVERTEXELEMENT9 VertexPosElements[] = 
	{
		{0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		D3DDECL_END()
	};	
	HR(gd3dDevice->CreateVertexDeclaration(VertexPosElements, &VertexPos::Decl));*/
}

void DestroyAllVertexDeclarations()
{
	ReleaseCOM(RectVertex::Decl);
	//ReleaseCOM(TextureVertex::Decl);
}