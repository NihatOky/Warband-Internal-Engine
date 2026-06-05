#pragma once
#include <d3d9.h>

typedef long(__stdcall* EndScene_t)(IDirect3DDevice9* pDevice);
extern EndScene_t oEndScene; 

long __stdcall hkEndScene(IDirect3DDevice9* pDevice);
