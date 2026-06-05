#include "dx.h"
#include <iostream>

EndScene_t oEndScene = nullptr;

long __stdcall hkEndScene(IDirect3DDevice9* pDevice) {
    // pDevice bizim ana cihazımız. Tüm çizimler bunun üzerinden geçer.

    // Basit bir yeşil kutu çizimi (Sol üst köşe: 50,50 / Boyut: 100x100)
    // Eğer DrawFilledRect fonksiyonun yoksa, doğrudan D3D9 komutuyla da çizebiliriz:

    D3DRECT rect = { 50, 50, 150, 150 };
    pDevice->Clear(1, &rect, D3DCLEAR_TARGET, D3DCOLOR_ARGB(255, 0, 255, 0), 0, 0);

    // Orijinal fonksiyonu çağır (Bunu unutursan oyun donar!)
    return oEndScene(pDevice);
}
