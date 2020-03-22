#pragma once

class Model {
private:
    struct CustomVertex
    {
        DirectX::XMFLOAT3 Pos;
        DirectX::XMFLOAT3 Norm;
        DirectX::XMFLOAT3 Color;
        DirectX::XMFLOAT2 TexCoord;
    };

private:
    ID3D11VertexShader* pVertexShader;
    ID3D11PixelShader* pPixelShader;
    ID3D11InputLayout* pLayout;
    ID3D11Buffer* pBuffer;

public:
    Model();
    ~Model();

    HRESULT InitModel(ID3D11Device* pDevice);

    void Render(ID3D11DeviceContext* pDeviceContext);

    void Release();

private:
    HRESULT InitGeometry(ID3D11Device* pDevice);

    HRESULT InitPipeline(ID3D11Device* pDevice);

    HRESULT CompileShader(LPCWSTR srcFile, LPCSTR entryPoint, LPCSTR profile, ID3DBlob** blob);
};
