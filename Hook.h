#pragma once
#include <d3d9.h>
#include<d3dx9.h>
#include <windows.h>

typedef long(__stdcall* EndScene_t)(IDirect3DDevice9*);
typedef HRESULT(__stdcall* Reset_t)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);

extern EndScene_t oEndScene;
extern Reset_t oReset;

void HookD3D();
long __stdcall hkEndScene(IDirect3DDevice9* pDevice);
HRESULT __stdcall hkReset(IDirect3DDevice9* dev, D3DPRESENT_PARAMETERS* pp);
char* TrampHook(char* src, char* dst, unsigned int len);