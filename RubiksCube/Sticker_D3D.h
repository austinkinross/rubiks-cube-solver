#pragma once

#include "pch.h"
#include "Direct3DBase.h"
#include "CubeHelperFunctions.h"
#include "Sticker.h"

using namespace DirectX;

struct SideCubeConstantBuffer
{
    DirectX::XMFLOAT4X4 model;
    DirectX::XMFLOAT4X4 view;
    DirectX::XMFLOAT4X4 projection;
    DirectX::XMFLOAT4 color;
};

struct VertexPositionColor
{
    DirectX::XMFLOAT3 pos;
    DirectX::XMFLOAT3 color;
};

class Sticker_D3D : public Sticker
{

public:
    Sticker_D3D(ID3D11Device *pDevice, ID3D11DeviceContext *pContext, StickerColor color);

    virtual void InitializeModels(XMFLOAT4X4 *pCubeWorld, XMFLOAT4X4 sideRotation, int pos1, int pos2);
    virtual void Draw(XMFLOAT4X4 *pViewMatrix, XMFLOAT4X4 *pProjectionMatrix);

protected:

    bool m_loadingComplete;

    void ConfigureShaderMatrices(XMFLOAT4X4 *pViewMatrix, XMFLOAT4X4 *pProjectionMatrix);

    Microsoft::WRL::ComPtr<ID3D11Device> mDevice;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> mContext;

    Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer;
    Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_constantBuffer;

    uint32 m_indexCount;
    SideCubeConstantBuffer m_constantBufferData;

};