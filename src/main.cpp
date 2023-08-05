#include "main.h"

#include <windows.h>
#include <stdio.h>

ModMeta __stdcall GetModInfo() {
    static ModMeta meta = {
        "No Cursor Trapping",
        "NoCursorTrap",
        "1.0.0",
        "Kapilarny"
    };

    return meta;
}

typedef BOOL(__stdcall* _ClipCursor)(const RECT* lpRect);
_ClipCursor ClipCursor_Original;

BOOL __stdcall ClipCursorHook(const RECT* lpRect) {
    return ClipCursor_Original(nullptr);
}

void __stdcall ModInit() {
    LPVOID hookAddr = (LPVOID) GetProcAddress(GetModuleHandleA("user32.dll"), "ClipCursor");

    if (!hookAddr) {
        JAPI_LogError("Failed to get ClipCursor address!");
        return;
    }

    Hook hook = {
        hookAddr,
        (void*) ClipCursorHook,
        (LPVOID*)&ClipCursor_Original,
        "ClipCursor"
    };

    if(!JAPI_HookFunction(&hook)) {
        JAPI_LogError("Failed to hook ClipCursor!");
        return;
    }

    JAPI_LogInfo("Applied patches!");
}