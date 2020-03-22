#include "framework.h"

CONST TCHAR* GetFeatureLevelString(D3D_FEATURE_LEVEL* pD3dFeatureLevel);

VOID PrintMatrix(DirectX::XMMATRIX* m, CONST TCHAR * name) 
{
    if (!name)
    {
        name = TEXT("Unnamed");
    }

    if (!m)
    {
        OutputDebugStringFormat(TEXT("Matrix %s is not present\n"), name);
        return;
    }

    DirectX::XMVECTOR v1 = m->r[0];
    DirectX::XMVECTOR v2 = m->r[1];
    DirectX::XMVECTOR v3 = m->r[2];
    DirectX::XMVECTOR v4 = m->r[3];

      
    float m11 = DirectX::XMVectorGetX(v1);
    float m12 = DirectX::XMVectorGetY(v1);
    float m13 = DirectX::XMVectorGetZ(v1);
    float m14 = DirectX::XMVectorGetW(v1);

    float m21 = DirectX::XMVectorGetX(v2);
    float m22 = DirectX::XMVectorGetY(v2);
    float m23 = DirectX::XMVectorGetZ(v2);
    float m24 = DirectX::XMVectorGetW(v2);

    float m31 = DirectX::XMVectorGetX(v3);
    float m32 = DirectX::XMVectorGetY(v3);
    float m33 = DirectX::XMVectorGetZ(v3);
    float m34 = DirectX::XMVectorGetW(v3);

    float m41 = DirectX::XMVectorGetX(v4);
    float m42 = DirectX::XMVectorGetY(v4);
    float m43 = DirectX::XMVectorGetZ(v4);
    float m44 = DirectX::XMVectorGetW(v4);

    OutputDebugStringFormat(TEXT(
        "Matrix %s\n"
        "[%8.3f %8.3f %8.3f %8.3f]\n"
        "[%8.3f %8.3f %8.3f %8.3f]\n"
        "[%8.3f %8.3f %8.3f %8.3f]\n"
        "[%8.3f %8.3f %8.3f %8.3f]\n"), 
        name,
        m11, m12, m13, m14,
        m21, m22, m23, m24,
        m31, m32, m33, m34,
        m41, m42, m43, m44
    );
}

Renderer* Renderer::gpRenderer = NULL;

Renderer::Renderer(HWND hWnd) :
    hWnd(hWnd),
    initialized(false),
    pDevice(NULL),
    pDeviceContext(NULL),
    pSwapChain(NULL),
    pBackBuffer(NULL),
    pDepthStencil(NULL),
    pRenderTarget(NULL),
    pConstantBuffer(NULL)
{
    gpRenderer = this;
}

Renderer::~Renderer()
{
}

HRESULT Renderer::Init()
{
    HRESULT hr = S_OK;

    hr = InitDeviceAndSwapChain();
    if (FAILED(hr))
    {
        return hr;
    }

    hr = InitDrawSurface();
    if (FAILED(hr))
    {
        // Free Device and SwapChain
        SafeRelease(&pSwapChain, TEXT("Renderer::IDXGISwapChain"));
        SafeRelease(&pDeviceContext, TEXT("Renderer::ID3D11DeviceContext"));
        SafeRelease(&pDevice, TEXT("Renderer::ID3D11Device"));
        return hr;
    }

    hr = model.InitModel(pDevice, pDeviceContext);
    if (FAILED(hr))
    {
        // Free DrawSurface
        SafeRelease(&pRenderTarget, TEXT("Renderer::ID3D11RenderTargetView"));
        // Free Device and SwapChain
        SafeRelease(&pSwapChain, TEXT("Renderer::IDXGISwapChain"));
        SafeRelease(&pDeviceContext, TEXT("Renderer::ID3D11DeviceContext"));
        SafeRelease(&pDevice, TEXT("Renderer::ID3D11Device"));
        return hr;
    }

    hr = InitConstantBuffer();
    if (FAILED(hr))
    {
        model.Release();
        // Free DrawSurface
        SafeRelease(&pRenderTarget, TEXT("Renderer::ID3D11RenderTargetView"));
        // Free Device and SwapChain
        SafeRelease(&pSwapChain, TEXT("Renderer::IDXGISwapChain"));
        SafeRelease(&pDeviceContext, TEXT("Renderer::ID3D11DeviceContext"));
        SafeRelease(&pDevice, TEXT("Renderer::ID3D11Device"));
        return hr;
    }

    hr = InitRasterizerState();
    if (FAILED(hr))
    {
        model.Release();
        // Free constant buffer
        SafeRelease(&pConstantBuffer, TEXT("Renderer::ID3D11Buffer"));
        // Free DrawSurface
        SafeRelease(&pRenderTarget, TEXT("Renderer::ID3D11RenderTargetView"));
        // Free Device and SwapChain
        SafeRelease(&pSwapChain, TEXT("Renderer::IDXGISwapChain"));
        SafeRelease(&pDeviceContext, TEXT("Renderer::ID3D11DeviceContext"));
        SafeRelease(&pDevice, TEXT("Renderer::ID3D11Device"));
        return hr;
    }

    initialized = true;

    return hr;
}

HRESULT Renderer::InitDeviceAndSwapChain()
{
    HRESULT hr = S_OK;

    D3D_FEATURE_LEVEL levels[] =
    {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_9_3,
        D3D_FEATURE_LEVEL_9_1
    };

    // This flag adds support for surfaces with a color-channel ordering different
    // from the API default. It is required for compatibility with Direct2D.
    UINT deviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(DEBUG) || defined(_DEBUG)
    deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    DXGI_SWAP_CHAIN_DESC desc;
    ZeroMemory(&desc, sizeof(DXGI_SWAP_CHAIN_DESC));
    desc.Windowed = true;
    desc.BufferCount = 2;
    desc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
    desc.OutputWindow = hWnd;

    D3D_FEATURE_LEVEL featureLevel;
    hr = D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        deviceFlags,
        levels,
        ARRAYSIZE(levels),
        D3D11_SDK_VERSION,
        &desc,
        &pSwapChain,
        &pDevice,
        &featureLevel,
        &pDeviceContext
    );

    const TCHAR* szpFeatureLevel = GetFeatureLevelString(&featureLevel);
    OutputDebugStringFormat(TEXT("DirectX Feature Level: %s\n"), szpFeatureLevel);

    {
        PrintRefCount(pSwapChain, TEXT("Renderer::IDXGISwapChain"));
        PrintRefCount(pDevice, TEXT("Renderer::ID3D11Device"));
        PrintRefCount(pDeviceContext, TEXT("Renderer::ID3D11DeviceContext"));
    }

    return hr;
}

HRESULT Renderer::InitDrawSurface()
{
    HRESULT hr = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);
    if (FAILED(hr))
    {
        return hr;
    }

    D3D11_TEXTURE2D_DESC bbDesc;
    pBackBuffer->GetDesc(&bbDesc);

    D3D11_VIEWPORT viewport;
    ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
    viewport.Height = (FLOAT)bbDesc.Height;
    viewport.Width = (FLOAT)bbDesc.Width;
    viewport.MinDepth = 0;
    viewport.MaxDepth = 1;

    pDeviceContext->RSSetViewports(1, &viewport);

    hr = pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &pRenderTarget);
    // Free Back Buffer
    SafeRelease(&pBackBuffer, TEXT("Renderer::ID3D11Texture2D"));
    if (FAILED(hr))
    {
        return hr;
    }
    pDeviceContext->OMSetRenderTargets(1, &pRenderTarget, nullptr);

    D3D11_TEXTURE2D_DESC descDepth;
    descDepth.Width = bbDesc.Width;
    descDepth.Height = bbDesc.Height;
    descDepth.MipLevels = 1;
    descDepth.ArraySize = 1;
    descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // pDeviceSettings->d3d11.AutoDepthStencilFormat;
    descDepth.SampleDesc.Count = 1;
    descDepth.SampleDesc.Quality = 0;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    descDepth.CPUAccessFlags = 0;
    descDepth.MiscFlags = 0;
    hr = pDevice->CreateTexture2D(&descDepth, NULL, &pDepthStencil);
    if (FAILED(hr))
    {
        return hr;
    }

    hr = pDevice->CreateDepthStencilView(pDepthStencil, NULL, &pDepthStencilView);
    if (FAILED(hr)) 
    {
        return hr;
    }

    pDeviceContext->OMSetRenderTargets(1, &pRenderTarget, pDepthStencilView);

    return hr;
}

HRESULT Renderer::InitConstantBuffer()
{
    if (!pDevice)
    {
        return E_INVALIDARG;
    }

    HRESULT hr = S_OK;
    ConstantBuffer buffer;
    
    // Set object world matrix
    buffer.world = DirectX::XMMatrixIdentity();
    PrintMatrix(&buffer.world, TEXT("World Identity"));

    // Set view matrix
    buffer.view = DirectX::XMMatrixLookAtLH(
        DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f), 
        DirectX::XMVectorZero(), 
        DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
    );
    PrintMatrix(&buffer.view, TEXT("View"));

    // Set projection matrix
    XM_CONST FLOAT fov = 90.0f * (DirectX::XM_PI / 180.0f);
    XM_CONST FLOAT aspectRatio = 1424.0f / 728.0f;
    buffer.proj = DirectX::XMMatrixPerspectiveFovLH(fov, aspectRatio, 0.1f, 1000.0f);
    PrintMatrix(&buffer.proj, TEXT("Projection"));

    // Caclulate world * view * proj matrix
    buffer.worldViewProj = buffer.world * buffer.view * buffer.proj;
    PrintMatrix(&buffer.worldViewProj, TEXT("World * View * Projection"));

    buffer.world = DirectX::XMMatrixTranspose(buffer.world);
    buffer.view = DirectX::XMMatrixTranspose(buffer.view);
    buffer.proj = DirectX::XMMatrixTranspose(buffer.proj);
    buffer.worldViewProj = DirectX::XMMatrixTranspose(buffer.worldViewProj);

    D3D11_BUFFER_DESC cbDesc;
    cbDesc.ByteWidth = sizeof(ConstantBuffer);
    cbDesc.Usage = D3D11_USAGE_DYNAMIC;
    cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    cbDesc.MiscFlags = 0;
    cbDesc.StructureByteStride = 0;

    // Fill in the subresource data.
    D3D11_SUBRESOURCE_DATA InitData;
    InitData.pSysMem = &buffer;
    InitData.SysMemPitch = 0;
    InitData.SysMemSlicePitch = 0;

    hr = pDevice->CreateBuffer(&cbDesc, &InitData, &pConstantBuffer);
    if (FAILED(hr))
    {
        return hr;
    }

    pDeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);

    return hr;
}

HRESULT Renderer::InitRasterizerState()
{
    HRESULT hr = S_OK;

    D3D11_RASTERIZER_DESC rs;
    ZeroMemory(&rs, sizeof(D3D11_RASTERIZER_DESC));

    rs.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
    rs.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;

    ID3D11RasterizerState* pRasterizerState;
    hr = pDevice->CreateRasterizerState(&rs, &pRasterizerState);
    if (FAILED(hr)) 
    {
        return hr;
    }

    pDeviceContext->RSSetState(pRasterizerState);
    pRasterizerState->Release();
    pRasterizerState = NULL;

    return hr;
}

void Renderer::Frame()
{
    if (initialized)
    {
        BeginScene();
        model.Render(pDeviceContext);
        pSwapChain->Present(0, 0);
    }
}

void Renderer::BeginScene()
{
    const FLOAT clearColor[4] = { 0.0f, 0.2f, 0.4f, 1.0f };
    pDeviceContext->ClearRenderTargetView(pRenderTarget, clearColor);
    pDeviceContext->OMSetRenderTargets(1, &pRenderTarget, pDepthStencilView);
    pDeviceContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
    pDeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
}

void Renderer::Release()
{
    if (!initialized)
    {
        return;
    }

    initialized = false;

    // Clean device state
    pSwapChain->SetFullscreenState(false, NULL);
    pDeviceContext->IASetVertexBuffers(0, 0, 0, 0, 0);
    pDeviceContext->OMSetRenderTargets(0, 0, 0);
    pDeviceContext->IASetInputLayout(NULL);
    pDeviceContext->PSSetShader(NULL, 0, 0);
    pDeviceContext->VSSetShader(NULL, 0, 0);

    // Remove models
    model.Release();
    
    // Safe remove all stuff
    SafeRelease(&pConstantBuffer, TEXT("Renderer::ID3D11Buffer"));
    SafeRelease(&pRenderTarget, TEXT("Renderer::ID3D11RenderTargetView"));
    SafeRelease(&pSwapChain, TEXT("Renderer::IDXGISwapChain"));
    SafeRelease(&pDeviceContext, TEXT("Renderer::ID3D11DeviceContext"));

    ID3D11Debug* pDebug = NULL;
    HRESULT hr = pDevice->QueryInterface(&pDebug);
    SafeRelease(&pDevice, TEXT("Renderer::ID3D11Device"));
    if (SUCCEEDED(hr))
    {
        hr = pDebug->ReportLiveDeviceObjects(D3D11_RLDO_FLAGS::D3D11_RLDO_DETAIL);
        if (FAILED(hr))
        {
            MessageBox(NULL, TEXT("ReportLiveDeviceObjects is failed"), TEXT("DirectX"), MB_ICONERROR | MB_OK);
        }

        pDebug->Release();
        pDebug = nullptr;
    }
    else
    {
        MessageBox(NULL, TEXT("QueryInterface on ID3D11Debug is failed"), TEXT("DirectX"), MB_ICONERROR | MB_OK);
    }
}

CONST TCHAR* GetFeatureLevelString(D3D_FEATURE_LEVEL* pD3dFeatureLevel)
{
    if (pD3dFeatureLevel)
    {
        const TCHAR* szpFeatureLevel = nullptr;

        switch (*pD3dFeatureLevel)
        {
        case D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_1:
            szpFeatureLevel = TEXT("D3D_FEATURE_LEVEL_11_1");
            break;
        case D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0:
            szpFeatureLevel = TEXT("D3D_FEATURE_LEVEL_11_0");
            break;
        case D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_10_1:
            szpFeatureLevel = TEXT("D3D_FEATURE_LEVEL_10_1");
            break;
        case D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_10_0:
            szpFeatureLevel = TEXT("D3D_FEATURE_LEVEL_10_0");
            break;
        case D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_9_3:
            szpFeatureLevel = TEXT("D3D_FEATURE_LEVEL_9_3");
            break;
        case D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_9_1:
            szpFeatureLevel = TEXT("D3D_FEATURE_LEVEL_9_1");
            break;
        default:
            szpFeatureLevel = TEXT("D3D_FEATURE_LEVEL_UNKNOW");
            break;
        }

        return szpFeatureLevel;
    }

    return TEXT("D3D_FEATURE_LEVEL_UNKNOW");
}
