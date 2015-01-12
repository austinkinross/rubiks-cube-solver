#pragma once

#include "Renderer\Renderer.h"

#include <d3d11_1.h>

#include <agile.h>

class Sticker;

struct SideCubeConstantBuffer2
{
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;
    glm::vec4 color;
};

class RendererD3D : public Renderer
{
public:
    RendererD3D();

    virtual void RenderSticker(Sticker* pSticker, glm::mat4 *pWorldMatrix, glm::mat4 *pViewMatrix, glm::mat4 *pProjectionMatrix);

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
    glm::mat4 m_orientationTransform3D;

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

