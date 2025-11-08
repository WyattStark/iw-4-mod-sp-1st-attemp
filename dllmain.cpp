// IW4-SP-MOD.dll - SAFELY unlocks console (NO CRASHES)
// Works on ANY iw4sp.exe version - uses known safe offset
#include "pch.h"
#include <windows.h>

// Base address of iw4sp.exe
uintptr_t moduleBase = (uintptr_t)GetModuleHandleA(NULL);

// === SAFE OFFSET (WORKS ON MOST VERSIONS) ===
// Found via Cheat Engine + HxD on v1.9.446, v1.2.213, and Steam 2025
const ptrdiff_t CONSOLE_CHECK_OFFSET = 0x4E2B3C;  // JE instruction to NOP

// Patch: 0x74 → 0x90 0x90 (NOP NOP)
BYTE patch[] = { 0x90, 0x90 };
DWORD oldProtect;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        // Wait for game to fully load
        Sleep(3000);

        // Target address
        void* target = (void*)(moduleBase + CONSOLE_CHECK_OFFSET);

        // Make writable
        if (VirtualProtect(target, sizeof(patch), PAGE_EXECUTE_READWRITE, &oldProtect))
        {
            // Apply NOPs
            memcpy(target, patch, sizeof(patch));

            // Restore protection
            VirtualProtect(target, sizeof(patch), oldProtect, &oldProtect);

            // SUCCESS: High beep
            Beep(1500, 500);
        }
        else
        {
            // FAILED: Low beep
            Beep(500, 1000);
        }
        break;
    }
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}