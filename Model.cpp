#include "framework.h"

Model::Model() :
    pVertexShader(NULL),
    pPixelShader(NULL),
    pLayout(NULL),
    pBuffer(NULL),
    pTexture(NULL),
    pSampleState(NULL),
    vSize(0)
{
}

Model::~Model()
{
}

HRESULT Model::InitModel(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
    HRESULT hr = S_OK;

    hr = InitGeometry(pDevice);
    if (FAILED(hr))
    {
        return hr;
    }

    hr = InitPipeline(pDevice);
    if (FAILED(hr))
    {
        // Free Geometry
        SafeRelease(&pBuffer, TEXT("Model::ID3D11Buffer"));
        return hr;
    }

    hr = InitTexture(TEXT("t16.png"), pDevice, pDeviceContext);
    if (FAILED(hr))
    {
        // Free Geometry
        SafeRelease(&pBuffer, TEXT("Model::ID3D11Buffer"));
        SafeRelease(&pLayout, TEXT("Model::ID3D11InputLayout"));
        return hr;
    }

    return hr;
}

HRESULT Model::InitGeometry(ID3D11Device* pDevice)
{
    if (!pDevice)
    {
        return E_INVALIDARG;
    }

    /*CustomVertex verticies[] =
    {
        {
            DirectX::XMFLOAT3(0.7f, 0.0f, 0.7f),
            DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f),
            DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),
            DirectX::XMFLOAT2(0.0f, 1.0f)
        },
        {
            DirectX::XMFLOAT3(-0.7f, 0.5f, 0.7f),
            DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f),
            DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),
            DirectX::XMFLOAT2(0.0f, 0.0f)
        },
        {
            DirectX::XMFLOAT3(-0.7f, 0.0f, -0.7f),
            DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f),
            DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),
            DirectX::XMFLOAT2(1.0f, 0.0f)
        },
        {
            DirectX::XMFLOAT3(-0.7f, 0.0f, -0.7f),
            DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f),
            DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f),
            DirectX::XMFLOAT2(1.0f, 0.0f)
        },
        {
            DirectX::XMFLOAT3(0.7f, -0.5f, -0.7f),
            DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f),
            DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f),
            DirectX::XMFLOAT2(1.0f, 1.0f)
        },
        {
            DirectX::XMFLOAT3(0.7f, 0.0f, 0.7f),
            DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f),
            DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f),
            DirectX::XMFLOAT2(0.0f, 1.0f)
        }
    };*/

    CustomVertex verticies[] =
    {
        {
            DirectX::XMFLOAT3(-0.526658f, 0.526658f, -0.526658f),
            DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f),
            DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),
            DirectX::XMFLOAT2(0.875f, 0.5f)
        },
        {
            DirectX::XMFLOAT3(0.526658f, 0.526658f, 0.526658f),
            DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f),
            DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),
            DirectX::XMFLOAT2(0.625f, 0.75f)
        },
        {
            DirectX::XMFLOAT3(0.526658f, 0.526658f, -0.526658f),
            DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f),
            DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),
            DirectX::XMFLOAT2(0.625f, 0.5f)
        },
        {
            DirectX::XMFLOAT3(0.526658f, 0.526658f, 0.526658f),
            DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f),
            DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),
            DirectX::XMFLOAT2(0.625f, 0.75f)
        },
        {
            DirectX::XMFLOAT3(-0.526658f, -0.526658f, 0.526658f),
            DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f),
            DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),
            DirectX::XMFLOAT2(0.375f, 1.0f)
        },
        {
            DirectX::XMFLOAT3(0.526658f, -0.526658f, 0.526658f),
            DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f),
            DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),
            DirectX::XMFLOAT2(0.375f, 0.75f)
        },
        {
            DirectX::XMFLOAT3(-0.526658f, 0.526658f, 0.526658f),
            DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f),
            DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),
            DirectX::XMFLOAT2(0.625f, 0.0f)
        },
        {
            DirectX::XMFLOAT3(-0.526658f, -0.526658f, -0.526658f),
            DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f),
            DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),
            DirectX::XMFLOAT2(0.375f, 0.25f)
        },
        {
            DirectX::XMFLOAT3(-0.526658f, -0.526658f, 0.526658f),
            DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f),
            DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),
            DirectX::XMFLOAT2(0.375f, 0.0f)
        },
        {
            DirectX::XMFLOAT3(0.526658f, -0.526658f, -0.526658f),
            DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f),
            DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),
            DirectX::XMFLOAT2(0.375f, 0.5f)
        },
        {
            DirectX::XMFLOAT3(-0.526658f, -0.526658f, 0.526658f),
            DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f),
            DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),
            DirectX::XMFLOAT2(0.125f, 0.75f)
        },
        {
            DirectX::XMFLOAT3(-0.526658f, -0.526658f, -0.526658f),
            DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f),
            DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),
            DirectX::XMFLOAT2(0.125f, 0.5f)
        },
        {
            DirectX::XMFLOAT3(0.526658f, 0.526658f, -0.526658f),
            DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),
            DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),
            DirectX::XMFLOAT2(0.625f, 0.5f)
        },
        {
            DirectX::XMFLOAT3(0.526658f, -0.526658f, 0.526658f),
            DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),
            DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),
            DirectX::XMFLOAT2(0.375f, 0.75f)
        },
        {
            DirectX::XMFLOAT3(0.526658f, -0.526658f, -0.526658f),
            DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),
            DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),
            DirectX::XMFLOAT2(0.375f, 0.5f)
        },
        {
            DirectX::XMFLOAT3(-0.526658f, 0.526658f, -0.526658f),
            DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f),
            DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),
            DirectX::XMFLOAT2(0.625f, 0.25f)
        },
        {
            DirectX::XMFLOAT3(0.526658f, -0.526658f, -0.526658f),
            DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f),
            DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),
            DirectX::XMFLOAT2(0.375f, 0.5f)
        },
        {
            DirectX::XMFLOAT3(-0.526658f, -0.526658f, -0.526658f),
            DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f),
            DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),
            DirectX::XMFLOAT2(0.375f, 0.25f)
        },
        {
            DirectX::XMFLOAT3(-0.526658f, 0.526658f, -0.526658f),
            DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f),
            DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),
            DirectX::XMFLOAT2(0.875f, 0.5f)
        },
        {
            DirectX::XMFLOAT3(-0.526658f, 0.526658f, 0.526658f),
            DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f),
            DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),
            DirectX::XMFLOAT2(0.875f, 0.75f)
        },
        {
            DirectX::XMFLOAT3(0.526658f, 0.526658f, 0.526658f),
            DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f),
            DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),
            DirectX::XMFLOAT2(0.625f, 0.75f)
        },
        {
            DirectX::XMFLOAT3(0.526658f, 0.526658f, 0.526658f),
            DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f),
            DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),
            DirectX::XMFLOAT2(0.625f, 0.75f)
        },
        {
            DirectX::XMFLOAT3(-0.526658f, 0.526658f, 0.526658f),
            DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f),
            DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),
            DirectX::XMFLOAT2(0.625f, 1.0f)
        },
        {
            DirectX::XMFLOAT3(-0.526658f, -0.526658f, 0.526658f),
            DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f),
            DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),
            DirectX::XMFLOAT2(0.375f, 1.0f)
        },
        {
            DirectX::XMFLOAT3(-0.526658f, 0.526658f, 0.526658f),
            DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f),
            DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),
            DirectX::XMFLOAT2(0.625f, 0.0f)
        },
        {
            DirectX::XMFLOAT3(-0.526658f, 0.526658f, -0.526658f),
            DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f),
            DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),
            DirectX::XMFLOAT2(0.625f, 0.25f)
        },
        {
            DirectX::XMFLOAT3(-0.526658f, -0.526658f, -0.526658f),
            DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f),
            DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),
            DirectX::XMFLOAT2(0.375f, 0.25f)
        },
        {
            DirectX::XMFLOAT3(0.526658f, -0.526658f, -0.526658f),
            DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f),
            DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),
            DirectX::XMFLOAT2(0.375f, 0.5f)
        },
        {
            DirectX::XMFLOAT3(0.526658f, -0.526658f, 0.526658f),
            DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f),
            DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),
            DirectX::XMFLOAT2(0.375f, 0.75f)
        },
        {
            DirectX::XMFLOAT3(-0.526658f, -0.526658f, 0.526658f),
            DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f),
            DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),
            DirectX::XMFLOAT2(0.125f, 0.75f)
        },
        {
            DirectX::XMFLOAT3(0.526658f, 0.526658f, -0.526658f),
            DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),
            DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),
            DirectX::XMFLOAT2(0.625f, 0.5f)
        },
        {
            DirectX::XMFLOAT3(0.526658f, 0.526658f, 0.526658f),
            DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),
            DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),
            DirectX::XMFLOAT2(0.625f, 0.75f)
        },
        {
            DirectX::XMFLOAT3(0.526658f, -0.526658f, 0.526658f),
            DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),
            DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),
            DirectX::XMFLOAT2(0.375f, 0.75f)
        },
        {
            DirectX::XMFLOAT3(-0.526658f, 0.526658f, -0.526658f),
            DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f),
            DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),
            DirectX::XMFLOAT2(0.625f, 0.25f)
        },
        {
            DirectX::XMFLOAT3(0.526658f, 0.526658f, -0.526658f),
            DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f),
            DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),
            DirectX::XMFLOAT2(0.625f, 0.5f)
        },
        {
            DirectX::XMFLOAT3(0.526658f, -0.526658f, -0.526658f),
            DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f),
            DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),
            DirectX::XMFLOAT2(0.375f, 0.5f)
        }
    };

    vSize = ARRAYSIZE(verticies);

    D3D11_BUFFER_DESC bufferDesc;
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.ByteWidth = sizeof(CustomVertex) * vSize;
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.CPUAccessFlags = 0;
    bufferDesc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA InitData;
    InitData.pSysMem = verticies;
    InitData.SysMemPitch = 0;
    InitData.SysMemSlicePitch = 0;

    return pDevice->CreateBuffer(&bufferDesc, &InitData, &pBuffer);
}

HRESULT Model::InitPipeline(ID3D11Device* pDevice)
{
    ID3DBlob* vsBlob = nullptr;
    ID3DBlob* psBlob = nullptr;

    HRESULT hr = S_OK;

    hr = CompileShader(TEXT("VertexShader.hlsl"), "VSMain", "vs_4_0_level_9_1", &vsBlob);
    if (FAILED(hr))
    {
        return hr;
    }

    hr = CompileShader(TEXT("PixelShader.hlsl"), "PSMain", "ps_4_0_level_9_1", &psBlob);
    if (FAILED(hr))
    {
        SafeRelease(&vsBlob, TEXT("Model::ID3DBlob"));
        return hr;
    }

    hr = pDevice->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), NULL, &pVertexShader);
    if (FAILED(hr))
    {
        SafeRelease(&vsBlob, TEXT("Model::ID3DBlob"));
        SafeRelease(&psBlob, TEXT("Model::ID3DBlob"));
        return hr;
    }

    hr = pDevice->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), NULL, &pPixelShader);
    if (FAILED(hr))
    {
        SafeRelease(&vsBlob, TEXT("Model::ID3DBlob"));
        SafeRelease(&psBlob, TEXT("Model::ID3DBlob"));
        SafeRelease(&pVertexShader, TEXT("Model::ID3D11VertexShader"));
        return hr;
    }

    D3D11_INPUT_ELEMENT_DESC ied[] =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    hr = pDevice->CreateInputLayout(ied, ARRAYSIZE(ied), vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &pLayout);
    if (FAILED(hr))
    {
        SafeRelease(&vsBlob, TEXT("Model::ID3DBlob"));
        SafeRelease(&psBlob, TEXT("Model::ID3DBlob"));
        SafeRelease(&pVertexShader, TEXT("Model::ID3D11VertexShader"));
        SafeRelease(&pPixelShader, TEXT("Model::ID3D11PixelShader"));
        return hr;
    }

    return hr;
}

HRESULT Model::InitTexture(LPCTSTR filename, ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
    HRESULT hr = S_OK;

    Texture texture;
    hr = texture.Init(filename, pDevice, pDeviceContext);
    pTexture = texture.Get();
    pTexture->AddRef();
    texture.Release();    

    D3D11_SAMPLER_DESC samplerDesc;
    ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT; // D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.MipLODBias = 0.0f;
    samplerDesc.MaxAnisotropy = 1;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    samplerDesc.BorderColor[0] = 0;
    samplerDesc.BorderColor[1] = 0;
    samplerDesc.BorderColor[2] = 0;
    samplerDesc.BorderColor[3] = 0;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    hr = pDevice->CreateSamplerState(&samplerDesc, &pSampleState);
    if (FAILED(hr)) 
    {
        SafeRelease(&pTexture, TEXT("Model::ID3D11Texture2D"));
        return hr;
    }

    D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
    ZeroMemory(&shaderResourceViewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
    shaderResourceViewDesc.Format = texture.GetFormat();
    shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    shaderResourceViewDesc.Texture2D.MipLevels = 1;

    hr = pDevice->CreateShaderResourceView(pTexture, &shaderResourceViewDesc, &pShaderResourceView);
    if (FAILED(hr)) 
    {
        pTexture->Release();
    }

    return hr;
}

void Model::Render(ID3D11DeviceContext* pDeviceContext)
{
    if (!pDeviceContext)
    {
        return;
    }

    UINT stride = sizeof(CustomVertex);
    UINT offset = 0;
    pDeviceContext->IASetVertexBuffers(0, 1, &pBuffer, &stride, &offset);
    pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    pDeviceContext->IASetInputLayout(pLayout);
    pDeviceContext->PSSetShader(pPixelShader, 0, 0);
    pDeviceContext->VSSetShader(pVertexShader, 0, 0);
    pDeviceContext->PSSetShaderResources(0, 1, &pShaderResourceView);
    pDeviceContext->PSSetSamplers(0, 1, &pSampleState);
    pDeviceContext->Draw(vSize, 0);
}

void Model::Release()
{
    SafeRelease(&pTexture, TEXT("Model::ID3D11Texture2D"));
    SafeRelease(&pSampleState, TEXT("Model::ID3D11SamplerState"));
    SafeRelease(&pShaderResourceView, TEXT("Model::ID3D11ShaderResourceView"));
    SafeRelease(&pVertexShader, TEXT("Model::ID3D11VertexShader"));
    SafeRelease(&pPixelShader, TEXT("Model::ID3D11PixelShader"));
    SafeRelease(&pLayout, TEXT("Model::ID3D11InputLayout"));
    SafeRelease(&pBuffer, TEXT("Model::ID3D11Buffer"));
}

HRESULT Model::CompileShader(LPCWSTR srcFile, LPCSTR entryPoint, LPCSTR profile, ID3DBlob** blob)
{
    if (!srcFile || !entryPoint || !profile || !blob)
    {
        return E_INVALIDARG;
    }

    *blob = nullptr;

    UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
    flags |= D3DCOMPILE_DEBUG;
    flags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

    const D3D_SHADER_MACRO defines[] =
    {
        "EXAMPLE_DEFINE", "1",
        NULL, NULL
    };

    ID3DBlob* shaderBlob = nullptr;
    ID3DBlob* errorBlob = nullptr;
    HRESULT hr = D3DCompileFromFile(srcFile, defines, D3D_COMPILE_STANDARD_FILE_INCLUDE,
        entryPoint, profile, flags, 0, &shaderBlob, &errorBlob);
    if (FAILED(hr))
    {
        if (errorBlob)
        {
            OutputDebugStringA((char*)errorBlob->GetBufferPointer());
            errorBlob->Release();
        }

        if (shaderBlob)
            shaderBlob->Release();

        return hr;
    }

    *blob = shaderBlob;

    return hr;
}
