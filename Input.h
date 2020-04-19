#pragma once

#include <dinput.h>

class Input;

typedef VOID(*UpdateFn)(Input *);

class Input {
private:
    IDirectInput8* pDirectInput;
    
    IDirectInputDevice8* pDirectInputKeyboard;
    IDirectInputDevice8* pDirectInputMouse;

    UpdateFn update;

    DIMOUSESTATE mouseLastState;
    DIMOUSESTATE mouseCurrState;
    BYTE keyboardState[256];

public:
    Input();
    ~Input();

    HRESULT Init(HWND hWnd);
    
    VOID Update(DOUBLE time);
    VOID Release();

    VOID SetUpdateFunction(UpdateFn fn);

    BOOL IsKeyPressed(int keycode) const;

    DIMOUSESTATE* GetCurrentMouseState() const;
    DIMOUSESTATE* GetLastMouseState() const;

private:
    HRESULT InitDevices();
    HRESULT InitDevicesDataFormat(HWND hWnd);
    VOID ReleaseDevices();
};
