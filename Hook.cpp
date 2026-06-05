#include "Hook.h"
#include <iostream>

// ======================================================
// GLOBALS & HOOK ADRESLERİ
// ======================================================
uintptr_t g_ModuleBase = (uintptr_t)GetModuleHandleA("mb_warband.exe");
EndScene_t oEndScene = nullptr;
Reset_t oReset = nullptr;

bool g_GodMode = false;

BYTE g_GodOrig[6] = { 0 };
BYTE g_GoldOrig[6] = { 0 };









// ======================================================
// ASSEMBLY HOOKS
// ======================================================
void __declspec(naked) GodmodeHook() {
    __asm {
        // 1. Kendi kodumuzu çalıştır
        cmp dword ptr[esi + 0x34], 1
        je PLAYER
        fst dword ptr[esi + 0x6000] // Orijinal oyun kodu
        jmp GOTO_ORIGINAL

        PLAYER :
        fstp st(0)

            GOTO_ORIGINAL :
            // 2. Orijinal koda geri dön
            // Buraya, hook attığımız yerin 6 bayt sonrasının adresini koyacağız
            mov eax, g_ModuleBase
            add eax, 0x9EE54 + 6
            jmp eax
    }
}


void Patch(BYTE* dst, BYTE* src, unsigned int size) {
    DWORD oProc;
    VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oProc);
    memcpy(dst, src, size);
    VirtualProtect(dst, size, oProc, &oProc);
}

bool Hook(char* src, char* dst, int len) {
    if (len < 5) return false;
    DWORD oProc;
    VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &oProc);
    memset(src, 0x90, len);
    uintptr_t relAddy = (uintptr_t)(dst - src - 5);
    *src = (char)0xE9;
    *(uintptr_t*)(src + 1) = (uintptr_t)relAddy;
    VirtualProtect(src, len, oProc, &oProc);
    return true;
}

char* TrampHook(char* src, char* dst, unsigned int len) {
    if (len < 5) return 0;
    char* gateway = (char*)VirtualAlloc(0, len + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    memcpy(gateway, src, len);
    uintptr_t jumpAddy = (uintptr_t)(src - gateway - 5);
    *(gateway + len) = (char)0xE9;
    *(uintptr_t*)(gateway + len + 1) = jumpAddy;
    if (Hook(src, dst, len)) return gateway;
    else return nullptr;
}

struct Vertex
{
    float x, y, z, rhw;
    DWORD color;
};

#define FVF (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)

void DrawMenu(IDirect3DDevice9* pDevice)
{
    static ID3DXFont* pFont = nullptr;
    static ID3DXLine* pLine = nullptr;

    if (!pFont)
        D3DXCreateFontA(
            pDevice,
            18,
            0,
            FW_BOLD,
            1,
            FALSE,
            DEFAULT_CHARSET,
            OUT_DEFAULT_PRECIS,
            DEFAULT_QUALITY,
            DEFAULT_PITCH | FF_DONTCARE,
            "Arial",
            &pFont);

    if (!pLine)
        D3DXCreateLine(pDevice, &pLine);

   
    D3DRECT menu = { 50, 50, 300, 180 };
    pDevice->Clear(
        1,
        &menu,
        D3DCLEAR_TARGET,
        D3DCOLOR_ARGB(220, 25, 25, 25),
        0,
        0);

    // Başlık çubuğu
    D3DRECT header = { 50, 50, 300, 80 };
    pDevice->Clear(
        1,
        &header,
        D3DCLEAR_TARGET,
        D3DCOLOR_ARGB(255, 0, 120, 220),
        0,
        0);

    // Çerçeve
    D3DXVECTOR2 border[] =
    {
        {50, 50},
        {300, 50},
        {300, 180},
        {50, 180},
        {50, 50}
    };

    pLine->Begin();
    pLine->Draw(border, 5, D3DCOLOR_ARGB(255, 0, 180, 255));
    pLine->End();

    // Başlık
    RECT rHeader = { 60, 55, 290, 80 };

    pFont->DrawTextA(
        NULL,
        "OKYAR STAJ",
        -1,
        &rHeader,
        DT_LEFT,
        D3DCOLOR_XRGB(255, 255, 255));

    // Menü seçenekleri
    RECT rGod = { 65, 95, 290, 120 };

    pFont->DrawTextA(
        NULL,
        g_GodMode ?
        "[X] GodMode (F1)" :
        "[ ] GodMode (F1)",
        -1,
        &rGod,
        DT_LEFT,
        D3DCOLOR_XRGB(255, 255, 255));

    // Alt bilgi
    RECT rVersion = { 65, 145, 290, 170 };

    pFont->DrawTextA(
        NULL,
        "Version 1.0",
        -1,
        &rVersion,
        DT_LEFT,
        D3DCOLOR_ARGB(255, 255, 215, 0));
}
long __stdcall hkEndScene(IDirect3DDevice9* pDevice) {
    if (GetAsyncKeyState(VK_F1) & 1) {
        g_GodMode = !g_GodMode;
        if (g_GodMode) {
            BYTE jmp[6] = { 0xE9, 0, 0, 0, 0, 0x90 };
            *(uintptr_t*)(jmp + 1) = (uintptr_t)GodmodeHook - (g_ModuleBase + 0x9EE54) - 5;
            Patch((BYTE*)(g_ModuleBase + 0x9EE54), jmp, 6);
        }
        else {
            Patch((BYTE*)(g_ModuleBase + 0x9EE54), g_GodOrig, 6);
        }
    }



    DrawMenu(pDevice);
    return oEndScene(pDevice);
}

HRESULT __stdcall hkReset(IDirect3DDevice9* dev, D3DPRESENT_PARAMETERS* pp) {
    return oReset(dev, pp);
}

void HookD3D() {
    HWND hwnd = FindWindowA(NULL, "Mount&Blade Warband");
    if (!hwnd) return;

    // Orijinal baytları yedekle
    memcpy(g_GodOrig, (void*)(g_ModuleBase + 0x9EE54), 6);
    memcpy(g_GoldOrig, (void*)(g_ModuleBase + 0x1472BF), 6);

    IDirect3D9* pD3D = Direct3DCreate9(D3D_SDK_VERSION);
    D3DPRESENT_PARAMETERS pp{ 0 };
    pp.Windowed = TRUE;
    pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    pp.hDeviceWindow = hwnd;

    IDirect3DDevice9* dev;
    if (SUCCEEDED(pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &pp, &dev))) {
        void** vt = *(void***)dev;
        oEndScene = (EndScene_t)TrampHook((char*)vt[42], (char*)hkEndScene, 7);
        oReset = (Reset_t)TrampHook((char*)vt[16], (char*)hkReset, 7);
        dev->Release();
        pD3D->Release();
    }
}