#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers

// Windows Header Files
#include <windows.h>
#include <strsafe.h>
#include <Unknwn.h>

VOID OutputDebugStringFormat(CONST TCHAR* pszFormat, ...);
template <class T> VOID PrintRefCount(T* object, CONST TCHAR* description);
template <class T> VOID SafeRelease(T** ptr, CONST TCHAR* description);

template <class T>
VOID PrintRefCount(T* object, CONST TCHAR* description)
{
    object->AddRef();
    CONST ULONG refs = object->Release();
    OutputDebugStringFormat(TEXT("DirectX %s has %d refs\n"), description, refs);
}

template <class T>
VOID SafeRelease(T** ptr, CONST TCHAR* description)
{
    IUnknown* temp = *ptr;
    if (temp) {
        PrintRefCount(temp, description);
        ULONG count = temp->Release();
        if (count == 0) {
            if (description) {
                OutputDebugStringFormat(TEXT("DirectX %s is released\n"), description);
            }
            *ptr = nullptr;
        }
        else {
            if (description) {
                OutputDebugStringFormat(TEXT("DirectX %s has %d references\n"), description, count);
            }
        }
    }
}

#include <d3d.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <d3d11_3.h>
#include <DirectXMath.h>
#include <wrl.h>
#include <d3dcompiler.h>

#include "Model.h"
#include "Renderer.h"

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
