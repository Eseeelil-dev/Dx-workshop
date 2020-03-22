#include "framework.h"

Model::Model() :
    pVertexShader(NULL),
    pPixelShader(NULL),
    pLayout(NULL),
    pBuffer(NULL)
{
}

Model::~Model()
{
}

HRESULT Model::InitModel(ID3D11Device* pDevice)
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

    return hr;
}

HRESULT Model::InitGeometry(ID3D11Device* pDevice)
{
    if (!pDevice)
    {
        return E_INVALIDARG;
    }

    CustomVertex verticies[] =
    {
        {
            DirectX::XMFLOAT3(0.7f, 0.0f, 0.7f),
            DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f),
            DirectX::XMFLOAT3(0.5f, 0.7f, 0.5f),
            DirectX::XMFLOAT2(1.0f, 0.0f)
        },
        {
            DirectX::XMFLOAT3(-0.7f, 0.5f, 0.7f),
            DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f),
            DirectX::XMFLOAT3(0.5f, 0.7f, 0.5f),
            DirectX::XMFLOAT2(0.0f, 1.0f)
        },
        {
            DirectX::XMFLOAT3(-0.7f, 0.0f, -0.7f),
            DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f),
            DirectX::XMFLOAT3(0.5f, 0.7f, 0.5f),
            DirectX::XMFLOAT2(1.0f, 1.0f)
        },
        {
            DirectX::XMFLOAT3(-0.7f, 0.0f, -0.7f),
            DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f),
            DirectX::XMFLOAT3(0.6f, 0.5f, 0.7f),
            DirectX::XMFLOAT2(1.0f, 0.0f)
        },
        {
            DirectX::XMFLOAT3(0.7f, -0.5f, -0.7f),
            DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f),
            DirectX::XMFLOAT3(0.6f, 0.5f, 0.7f),
            DirectX::XMFLOAT2(1.0f, 1.0f)
        },
        {
            DirectX::XMFLOAT3(0.7f, 0.0f, 0.7f),
            DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f),
            DirectX::XMFLOAT3(0.6f, 0.5f, 0.7f),
            DirectX::XMFLOAT2(0.0f, 1.0f)
        }
    };

    D3D11_BUFFER_DESC bufferDesc;
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.ByteWidth = sizeof(CustomVertex) * 6;
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
    pDeviceContext->Draw(6, 0);
}

void Model::Release()
{
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
