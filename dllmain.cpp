#include <iostream>
#include"Hook.h"



DWORD WINAPI MainThread(LPVOID lpParam) {
    // Konsolu aç
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);

    std::cout << "Modul basariyla enjekte edildi!" << std::endl;

    HookD3D();
    

    // Döngü şartını parantez içine al ki işlem önceliği bozulmasın
    while (!(GetAsyncKeyState(VK_END) & 1)) {
        Sleep(100);
    }

    // Kapatmadan önce
    if (f) fclose(f);
    FreeConsole();

    FreeLibraryAndExitThread((HMODULE)lpParam, 0);
    return 0;
}

// D harfi büyük, geri kalanlar küçük olmalı!
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        // Thread oluşturmayı garantiye al
        HANDLE hThread = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)MainThread, hModule, 0, nullptr);
        if (hThread) {
            CloseHandle(hThread);
        }
    }
    return TRUE;
}