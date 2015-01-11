#pragma once

#include "pch.h"
#include "CubeRenderer.h"

using namespace DirectX;

class Sticker;

struct SideCubeConstantBuffer2
{
    DirectX::XMFLOAT4X4 model;
    DirectX::XMFLOAT4X4 view;
    DirectX::XMFLOAT4X4 projection;
    DirectX::XMFLOAT4 color;
};

class CubeRendererD3D : public CubeRenderer
{
public:
    CubeRendererD3D(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

    virtual void RenderSticker(Sticker* pSticker, XMFLOAT4X4 *pWorldMatrix, XMFLOAT4X4 *pViewMatrix, XMFLOAT4X4 *pProjectionMatrix);

protected:
    Microsoft::WRL::ComPtr<ID3D11Device> mDevice;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> mContext;

    Microsoft::WRL::ComPtr<ID3D11InputLayout> mStickerInputLayout;
    Microsoft::WRL::ComPtr<ID3D11Buffer> mStickerVertexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> mStickerIndexBuffer;
    Microsoft::WRL::ComPtr<ID3D11VertexShader> mStickerVertexShader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> mStickerPixelShader;
    Microsoft::WRL::ComPtr<ID3D11Buffer> mStickerConstantBuffer;

    uint32 mStickerIndexCount;

    bool mStickerLoadingComplete;

    SideCubeConstantBuffer2 mStickerConstantBufferData;
};

