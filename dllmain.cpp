#include <iostream>
#include"Hook.h"



DWORD WINAPI MainThread(LPVOID lpParam) {
    
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);

    std::cout << "Modul basariyla enjekte edildi!" << std::endl;

    HookD3D();
    

    
    while (!(GetAsyncKeyState(VK_END) & 1)) {
        Sleep(100);
    }

    
    if (f) fclose(f);
    FreeConsole();

    FreeLibraryAndExitThread((HMODULE)lpParam, 0);
    return 0;
}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
      
        HANDLE hThread = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)MainThread, hModule, 0, nullptr);
        if (hThread) {
            CloseHandle(hThread);
        }
    }
    return TRUE;
}
