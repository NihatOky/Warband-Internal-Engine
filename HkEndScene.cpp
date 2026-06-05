#include "dx.h"
#include <iostream>

EndScene_t oEndScene = nullptr;

long __stdcall hkEndScene(IDirect3DDevice9* pDevice) {
    

 
    D3DRECT rect = { 50, 50, 150, 150 };
    pDevice->Clear(1, &rect, D3DCLEAR_TARGET, D3DCOLOR_ARGB(255, 0, 255, 0), 0, 0);

  
    return oEndScene(pDevice);
}
