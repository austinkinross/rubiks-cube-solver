#pragma once

#include "..\..\..\RubiksCube\pch.h"
#include "..\Renderer.h"

using namespace DirectX;

#include <agile.h>

class Sticker;

struct SideCubeConstantBuffer2
{
    DirectX::XMFLOAT4X4 model;
    DirectX::XMFLOAT4X4 view;
    DirectX::XMFLOAT4X4 projection;
    DirectX::XMFLOAT4 color;
};

class RendererD3D : public Renderer
{
public:
    RendererD3D();

    virtual void RenderSticker(Sticker* pSticker, XMFLOAT4X4 *pWorldMatrix, XMFLOAT4X4 *pViewMatrix, XMFLOAT4X4 *pProjectionMatrix);

    virtual void Swap();

    virtual void Clear();

    void UpdateForWindowSizeChange();

protected:

    void CreateDeviceResources();
    void CreateWindowSizeDependentResources();
    void HandleDeviceLost();
    float ConvertDipsToPixels(float dips);

    void InitializeStickerResources();

    // Direct3D Objects.
    Microsoft::WRL::ComPtr<ID3D11Device1> mDevice;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext1> mContext;
    Microsoft::WRL::ComPtr<IDXGISwapChain1> m_swapChain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTargetView;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthStencilView;

    // Cached renderer properties.
    D3D_FEATURE_LEVEL m_featureLevel;
    Windows::Foundation::Size m_renderTargetSize;
    Windows::Foundation::Rect m_windowBounds;
    Platform::Agile<Windows::UI::Core::CoreWindow> m_window;
    Windows::Graphics::Display::DisplayOrientations m_orientation;

    // Transform used for display orientation.
    DirectX::XMFLOAT4X4 m_orientationTransform3D;

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

