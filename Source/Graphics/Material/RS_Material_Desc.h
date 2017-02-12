#pragma once

#include <DirectXMath.h>

using namespace DirectX;

struct RS_Material_Desc
{
	wchar_t* diffuseTextureDir;
	wchar_t* normalTextureDir;
	wchar_t* rougnessTextureDir;
	wchar_t* metallicTextureDir;
	XMFLOAT4 diffuseColor;
};