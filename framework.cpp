#include "framework.h"
#include "Dx-Workshop.h"

VOID OutputDebugStringFormat(CONST TCHAR* pszFormat, ...)
{
    HRESULT hr = S_OK;
    int const arraysize = 2048;
    TCHAR pszDest[arraysize];
    ZeroMemory(&pszDest, arraysize);
    size_t cbDest = arraysize * sizeof(TCHAR);

    va_list argList;
    va_start(argList, pszFormat);
    hr = StringCbVPrintf(pszDest, cbDest, pszFormat, argList);
    va_end(argList);

    if (SUCCEEDED(hr))
    {
        OutputDebugString(pszDest);
    }
}
