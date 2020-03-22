#pragma once

class Renderer {
private:
    HWND hWnd;

    bool initialized;

    ID3D11Device* pDevice;
    ID3D11DeviceContext* pDeviceContext;
    IDXGISwapChain* pSwapChain;
    ID3D11Texture2D* pBackBuffer;
    ID3D11RenderTargetView* pRenderTarget;

    Model model;

    struct ConstantBuffer
    {
        DirectX::XMMATRIX world;
        DirectX::XMMATRIX view;
        DirectX::XMMATRIX proj;
        DirectX::XMMATRIX worldViewProj;

    };
    ID3D11Buffer* pConstantBuffer;

public:
    static Renderer* gpRenderer;

public:
    Renderer(HWND hWnd);
    ~Renderer();

public:
    HRESULT Init();
private:
    HRESULT InitDeviceAndSwapChain();
    HRESULT InitDrawSurface();
    HRESULT InitConstantBuffer();
    HRESULT InitRasterizerState();
    
public:
    void Frame();
private:
    void BeginScene();
    
public:
    void Release();
};
