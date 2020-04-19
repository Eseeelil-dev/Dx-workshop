#include "framework.h"


Input::Input() :
    pDirectInput(NULL),
    pDirectInputKeyboard(NULL),
    pDirectInputMouse(NULL)
{
    update = NULL;
    ZeroMemory(keyboardState, sizeof(keyboardState));
    ZeroMemory(&mouseLastState, sizeof(mouseLastState));
}

Input::~Input()
{
    update = NULL;
}

HRESULT Input::Init(HWND hWnd)
{
    HRESULT hr = S_OK;
    hr = InitDevices();
    if (FAILED(hr)) 
    {
        return hr;
    }
    
    hr = InitDevicesDataFormat(hWnd);
    if (FAILED(hr))
    {
        ReleaseDevices();
        return hr;
    }
    return hr;
}

HRESULT Input::InitDevices() 
{
    HRESULT hr = S_OK;

    hr = DirectInput8Create(
        GetModuleHandle(0),
        DIRECTINPUT_VERSION,
        IID_IDirectInput8,
        (void**)&pDirectInput,
        NULL
    );

    if (FAILED(hr))
    {
        return hr;
    }

    hr = pDirectInput->CreateDevice(GUID_SysKeyboard, &pDirectInputKeyboard, NULL);
    if (FAILED(hr))
    {
        pDirectInput->Release();
        return hr;
    }

    hr = pDirectInput->CreateDevice(GUID_SysMouse, &pDirectInputMouse, NULL);
    if (FAILED(hr))
    {
        pDirectInputKeyboard->Release();
        pDirectInput->Release();
        return hr;
    }

    return hr;
}

HRESULT Input::InitDevicesDataFormat(HWND hWnd)
{
    HRESULT hr = S_OK;

    hr = pDirectInputKeyboard->SetDataFormat(&c_dfDIKeyboard);
    if (FAILED(hr)) 
    {
        return hr;
    }

    hr = pDirectInputKeyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
    if (FAILED(hr))
    {
        return hr;
    }

    hr = pDirectInputMouse->SetDataFormat(&c_dfDIMouse);
    if (FAILED(hr))
    {
        return hr;
    }

    hr = pDirectInputMouse->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND);
    if (FAILED(hr))
    {
        return hr;
    }

    return hr;
}

VOID Input::ReleaseDevices() 
{
    SafeRelease(&pDirectInputMouse, TEXT("Input::IDirectInputDevice8"));
    SafeRelease(&pDirectInputKeyboard, TEXT("Input::IDirectInputDevice8"));
    SafeRelease(&pDirectInput, TEXT("Input::IDirectInput8"));
}

VOID Input::Update(DOUBLE time) 
{
    HRESULT hr = S_OK;

    hr = pDirectInputKeyboard->Acquire();
    if (SUCCEEDED(hr)) 
    {
        hr = pDirectInputKeyboard->GetDeviceState(sizeof(keyboardState), &keyboardState);
        if (FAILED(hr)) 
        {
            // Add some error handling
        }
    }

    hr = pDirectInputMouse->Acquire();
    if (SUCCEEDED(hr))
    {
        hr = pDirectInputMouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseCurrState);
        if (FAILED(hr))
        {
            // Add some error handling
        }
    }

    // Actions
    if (update)
    {
        update(this);
    }

    mouseLastState = mouseCurrState;
}

VOID Input::Release() 
{
    ReleaseDevices();
}

VOID Input::SetUpdateFunction(UpdateFn fn) 
{
    update = fn;
}

BOOL Input::IsKeyPressed(int keycode) const 
{
    return keyboardState[keycode] & 0x80;
}

DIMOUSESTATE* Input::GetCurrentMouseState() const 
{
    return (DIMOUSESTATE*)&mouseCurrState;
}

DIMOUSESTATE* Input::GetLastMouseState() const 
{
    return (DIMOUSESTATE*)&mouseLastState;
}
