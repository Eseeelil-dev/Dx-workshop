#pragma once

class Texture {
private:
    ID3D11Texture2D* pTexture;
    DXGI_FORMAT format;

public:
    Texture();
    ~Texture();

    HRESULT Init(LPCTSTR filename, ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
    void Release();

    ID3D11Texture2D* Get() const;
    DXGI_FORMAT GetFormat() const;
};
