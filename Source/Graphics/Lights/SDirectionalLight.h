#pragma once

struct DirLight
{
	DirLight() { ZeroMemory(this, sizeof(DirLight)); }
	XMFLOAT3 dir;
	float pad;
	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;
};