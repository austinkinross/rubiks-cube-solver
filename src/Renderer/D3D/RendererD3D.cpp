#include "RendererD3D.h"
#include "AsyncFileLoader.h"

#include "Helpers.h"

using namespace Microsoft::WRL;
using namespace Windows::UI::Core;
using namespace Windows::Foundation;
using namespace Windows::Graphics::Display;

struct VertexPositionColor
{
    glm::vec3 pos;
    glm::vec3 color;
};

RendererD3D::RendererD3D()
{
    m_window = CoreWindow::GetForCurrentThread();

    CreateDeviceResources();
    CreateWindowSizeDependentResources();

    InitializeStickerResources();
}

// Recreate all device resources and set them back to the current state.
void RendererD3D::HandleDeviceLost()
{
    // Reset these member variables to ensure that UpdateForWindowSizeChange recreates all resources.
    m_windowBounds.Width = 0;
    m_windowBounds.Height = 0;
    m_swapChain = nullptr;

    CreateDeviceResources();
    UpdateForWindowSizeChange();
}

// These are the resources that depend on the device.
void RendererD3D::CreateDeviceResources()
{
    // This flag adds support for surfaces with a different color channel ordering
    // than the API default. It is required for compatibility with Direct2D.
    UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(_DEBUG)
    // If the project is in a debug build, enable debugging via SDK Layers with this flag.
    creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    // This array defines the set of DirectX hardware feature levels this app will support.
    // Note the ordering should be preserved.
    // Don't forget to declare your application's minimum required feature level in its
    // description.  All applications are assumed to support 9.1 unless otherwise stated.
    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_9_3,
        D3D_FEATURE_LEVEL_9_2,
        D3D_FEATURE_LEVEL_9_1
    };

    // Create the Direct3D 11 API device object and a corresponding context.
    ComPtr<ID3D11Device> device;
    ComPtr<ID3D11DeviceContext> context;
    ThrowIfFailed(
        D3D11CreateDevice(
        nullptr, // Specify nullptr to use the default adapter.
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        creationFlags, // Set set debug and Direct2D compatibility flags.
        featureLevels, // List of feature levels this app can support.
        ARRAYSIZE(featureLevels),
        D3D11_SDK_VERSION, // Always set this to D3D11_SDK_VERSION for Windows Store apps.
        &device, // Returns the Direct3D device created.
        &m_featureLevel, // Returns feature level of device created.
        &context // Returns the device immediate context.
        )
        );

    // Get the Direct3D 11.1 API device and context interfaces.
    ThrowIfFailed(
        device.As(&mDevice)
        );

    ThrowIfFailed(
        context.As(&mContext)
        );

	ComPtr<ID3D11RasterizerState> noCull;
	D3D11_RASTERIZER_DESC rastDesc;
	ZeroMemory(&rastDesc, sizeof(D3D11_RASTERIZER_DESC));
	rastDesc.FillMode = D3D11_FILL_SOLID;
	rastDesc.CullMode = D3D11_CULL_NONE;
	mDevice->CreateRasterizerState(&rastDesc, &noCull);
	mContext->RSSetState(noCull.Get());
}

// Allocate all memory resources that change on a window SizeChanged event.
void RendererD3D::CreateWindowSizeDependentResources()
{
    // Store the window bounds so the next time we get a SizeChanged event we can
    // avoid rebuilding everything if the size is identical.
    m_windowBounds = m_window->Bounds;

    // Calculate the necessary swap chain and render target size in pixels.
    float windowWidth = ConvertDipsToPixels(m_windowBounds.Width);
    float windowHeight = ConvertDipsToPixels(m_windowBounds.Height);

    // The width and height of the swap chain must be based on the window's
    // landscape-oriented width and height. If the window is in a portrait
    // orientation, the dimensions must be reversed.
    m_orientation = DisplayProperties::CurrentOrientation;
    bool swapDimensions =
        m_orientation == DisplayOrientations::Portrait ||
        m_orientation == DisplayOrientations::PortraitFlipped;
    m_renderTargetSize.Width = swapDimensions ? windowHeight : windowWidth;
    m_renderTargetSize.Height = swapDimensions ? windowWidth : windowHeight;

    if (m_swapChain != nullptr)
    {
        // If the swap chain already exists, resize it.
        ThrowIfFailed(
            m_swapChain->ResizeBuffers(
            2, // Double-buffered swap chain.
            static_cast<UINT>(m_renderTargetSize.Width),
            static_cast<UINT>(m_renderTargetSize.Height),
            DXGI_FORMAT_B8G8R8A8_UNORM,
            0
            )
            );
    }
    else
    {
        // Otherwise, create a new one using the same adapter as the existing Direct3D device.
        DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { 0 };
        swapChainDesc.Width = static_cast<UINT>(m_renderTargetSize.Width); // Match the size of the window.
        swapChainDesc.Height = static_cast<UINT>(m_renderTargetSize.Height);
        swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; // This is the most common swap chain format.
        swapChainDesc.Stereo = false;
        swapChainDesc.SampleDesc.Count = 1; // Don't use multi-sampling.
        swapChainDesc.SampleDesc.Quality = 0;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.BufferCount = 2; // Use double-buffering to minimize latency.
        swapChainDesc.Scaling = DXGI_SCALING_NONE;
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL; // All Windows Store apps must use this SwapEffect.
        swapChainDesc.Flags = 0;

        ComPtr<IDXGIDevice1>  dxgiDevice;
        ThrowIfFailed(
            mDevice.As(&dxgiDevice)
            );

        ComPtr<IDXGIAdapter> dxgiAdapter;
        ThrowIfFailed(
            dxgiDevice->GetAdapter(&dxgiAdapter)
            );

        ComPtr<IDXGIFactory2> dxgiFactory;
        ThrowIfFailed(
            dxgiAdapter->GetParent(
            __uuidof(IDXGIFactory2),
            &dxgiFactory
            )
            );

        Windows::UI::Core::CoreWindow^ window = m_window.Get();
        ThrowIfFailed(
            dxgiFactory->CreateSwapChainForCoreWindow(
            mDevice.Get(),
            reinterpret_cast<IUnknown*>(window),
            &swapChainDesc,
            nullptr, // Allow on all displays.
            &m_swapChain
            )
            );

        // Ensure that DXGI does not queue more than one frame at a time. This both reduces latency and
        // ensures that the application will only render after each VSync, minimizing power consumption.
        ThrowIfFailed(
            dxgiDevice->SetMaximumFrameLatency(1)
            );
    }

    // Set the proper orientation for the swap chain, and generate the
    // 3D matrix transformation for rendering to the rotated swap chain.
    DXGI_MODE_ROTATION rotation = DXGI_MODE_ROTATION_UNSPECIFIED;
    switch (m_orientation)
    {
    case DisplayOrientations::Landscape:
        rotation = DXGI_MODE_ROTATION_IDENTITY;
        m_orientationTransform3D = glm::mat4( // 0-degree Z-rotation
                                              1.0f, 0.0f, 0.0f, 0.0f,
                                              0.0f, 1.0f, 0.0f, 0.0f,
                                              0.0f, 0.0f, 1.0f, 0.0f,
                                              0.0f, 0.0f, 0.0f, 1.0f
                                              );
        break;

    case DisplayOrientations::Portrait:
        rotation = DXGI_MODE_ROTATION_ROTATE270;
        m_orientationTransform3D = glm::mat4( // 90-degree Z-rotation
                                              0.0f, 1.0f, 0.0f, 0.0f,
                                              -1.0f, 0.0f, 0.0f, 0.0f,
                                              0.0f, 0.0f, 1.0f, 0.0f,
                                              0.0f, 0.0f, 0.0f, 1.0f
                                              );
        break;

    case DisplayOrientations::LandscapeFlipped:
        rotation = DXGI_MODE_ROTATION_ROTATE180;
        m_orientationTransform3D = glm::mat4( // 180-degree Z-rotation
                                              -1.0f, 0.0f, 0.0f, 0.0f,
                                              0.0f, -1.0f, 0.0f, 0.0f,
                                              0.0f, 0.0f, 1.0f, 0.0f,
                                              0.0f, 0.0f, 0.0f, 1.0f
                                              );
        break;

    case DisplayOrientations::PortraitFlipped:
        rotation = DXGI_MODE_ROTATION_ROTATE90;
        m_orientationTransform3D = glm::mat4( // 270-degree Z-rotation
                                              0.0f, -1.0f, 0.0f, 0.0f,
                                              1.0f, 0.0f, 0.0f, 0.0f,
                                              0.0f, 0.0f, 1.0f, 0.0f,
                                              0.0f, 0.0f, 0.0f, 1.0f
                                              );
        break;

    default:
        throw ref new Platform::FailureException();
    }

    ThrowIfFailed(
        m_swapChain->SetRotation(rotation)
        );

    // Create a render target view of the swap chain back buffer.
    ComPtr<ID3D11Texture2D> backBuffer;
    ThrowIfFailed(
        m_swapChain->GetBuffer(
        0,
        __uuidof(ID3D11Texture2D),
        &backBuffer
        )
        );

    ThrowIfFailed(
        mDevice->CreateRenderTargetView(
        backBuffer.Get(),
        nullptr,
        &m_renderTargetView
        )
        );

    // Create a depth stencil view.
    CD3D11_TEXTURE2D_DESC depthStencilDesc(
        DXGI_FORMAT_D24_UNORM_S8_UINT,
        static_cast<UINT>(m_renderTargetSize.Width),
        static_cast<UINT>(m_renderTargetSize.Height),
        1,
        1,
        D3D11_BIND_DEPTH_STENCIL
        );

    ComPtr<ID3D11Texture2D> depthStencil;
    ThrowIfFailed(
        mDevice->CreateTexture2D(
        &depthStencilDesc,
        nullptr,
        &depthStencil
        )
        );

    CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
    ThrowIfFailed(
        mDevice->CreateDepthStencilView(
        depthStencil.Get(),
        &depthStencilViewDesc,
        &m_depthStencilView
        )
        );

    // Set the rendering viewport to target the entire window.
    CD3D11_VIEWPORT viewport(
        0.0f,
        0.0f,
        m_renderTargetSize.Width,
        m_renderTargetSize.Height
        );

    mContext->RSSetViewports(1, &viewport);
}

// This method is called in the event handler for the SizeChanged event.
void RendererD3D::UpdateForWindowSizeChange()
{
    // TODO: fix this
    if (m_window->Bounds.Width != m_windowBounds.Width ||
        m_window->Bounds.Height != m_windowBounds.Height ||
        m_orientation != DisplayProperties::CurrentOrientation)
    {
        ID3D11RenderTargetView* nullViews[] = { nullptr };
        mContext->OMSetRenderTargets(ARRAYSIZE(nullViews), nullViews, nullptr);
        m_renderTargetView = nullptr;
        m_depthStencilView = nullptr;
        mContext->Flush();
        CreateWindowSizeDependentResources();
    }
}

// Method to deliver the final image to the display.
void RendererD3D::Swap()
{
    // The application may optionally specify "dirty" or "scroll"
    // rects to improve efficiency in certain scenarios.
    DXGI_PRESENT_PARAMETERS parameters = { 0 };
    parameters.DirtyRectsCount = 0;
    parameters.pDirtyRects = nullptr;
    parameters.pScrollRect = nullptr;
    parameters.pScrollOffset = nullptr;

    // The first argument instructs DXGI to block until VSync, putting the application
    // to sleep until the next VSync. This ensures we don't waste any cycles rendering
    // frames that will never be displayed to the screen.
    HRESULT hr = m_swapChain->Present1(1, 0, &parameters);

    // Discard the contents of the render target.
    // This is a valid operation only when the existing contents will be entirely
    // overwritten. If dirty or scroll rects are used, this call should be removed.
    mContext->DiscardView(m_renderTargetView.Get());

    // Discard the contents of the depth stencil.
    mContext->DiscardView(m_depthStencilView.Get());

    // If the device was removed either by a disconnect or a driver upgrade, we 
    // must recreate all device resources.
    if (hr == DXGI_ERROR_DEVICE_REMOVED)
    {
        HandleDeviceLost();
    }
    else
    {
        ThrowIfFailed(hr);
    }
}

// Method to convert a length in device-independent pixels (DIPs) to a length in physical pixels.
float RendererD3D::ConvertDipsToPixels(float dips)
{
    static const float dipsPerInch = 96.0f;
    return floor(dips * DisplayProperties::LogicalDpi / dipsPerInch + 0.5f); // Round to nearest integer.
}

void RendererD3D::InitializeStickerResources()
{
    // Initialize data to render a sticker

    auto loadVSTask = ReadDataAsync("SimpleVertexShader.cso");
    auto loadPSTask = ReadDataAsync("SimplePixelShader.cso");

    auto createVSTask = loadVSTask.then([this](Platform::Array<byte>^ fileData) {
        ThrowIfFailed(
            mDevice->CreateVertexShader(
            fileData->Data,
            fileData->Length,
            nullptr,
            &mStickerVertexShader
            )
            );

        const D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };

        ThrowIfFailed(
            mDevice->CreateInputLayout(
            vertexDesc,
            ARRAYSIZE(vertexDesc),
            fileData->Data,
            fileData->Length,
            &mStickerInputLayout
            )
            );
    });


    auto createPSTask = loadPSTask.then([this](Platform::Array<byte>^ fileData) {
        ThrowIfFailed(
            mDevice->CreatePixelShader(
            fileData->Data,
            fileData->Length,
            nullptr,
            &mStickerPixelShader
            )
            );

        CD3D11_BUFFER_DESC constantBufferDesc(sizeof(SideCubeConstantBuffer2), D3D11_BIND_CONSTANT_BUFFER);
        ThrowIfFailed(
            mDevice->CreateBuffer(
            &constantBufferDesc,
            nullptr,
            &mStickerConstantBuffer
            )
            );
    });

    auto createCubeTask = (createPSTask && createVSTask).then([this]() {
        VertexPositionColor cubeVertices[] =
        {
            { glm::vec3(-0.95f, 3.0f, -0.95f), glm::vec3(0, 0, 0) },
            { glm::vec3(-0.95f, 3.0f, +0.95f), glm::vec3(0, 0, 0) },
            { glm::vec3(+0.95f, 3.0f, -0.95f), glm::vec3(0, 0, 0) },
            { glm::vec3(+0.95f, 3.0f, +0.95f), glm::vec3(0, 0, 0) },

            // Cheap way to draw some black back faces
            /*		{glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec3(0,0,0)},
            {glm::vec3(-1.0f, 1.0f, +1.0f), glm::vec3(0,0,0)},
            {glm::vec3(+1.0f, 1.0f, -1.0f), glm::vec3(0,0,0)},
            {glm::vec3(+1.0f, 1.0f, +1.0f), glm::vec3(0,0,0)},
            {AddXM3(AddXM3(cubeCenter, MultXM3(dir1, (float)pos1 - 1.0f)), MultXM3(dir2, (float)pos2 - 1.0f)), color},
            {AddXM3(AddXM3(cubeCenter, MultXM3(dir1, (float)pos1 - 1.0f)), MultXM3(dir2, (float)pos2 + 1.0f)), color},
            {AddXM3(AddXM3(cubeCenter, MultXM3(dir1, (float)pos1 + 1.0f)), MultXM3(dir2, (float)pos2 - 1.0f)), color},
            {AddXM3(AddXM3(cubeCenter, MultXM3(dir1, (float)pos1 + 1.0f)), MultXM3(dir2, (float)pos2 + 1.0f)), color}, */

        };

        D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
        vertexBufferData.pSysMem = cubeVertices;
        vertexBufferData.SysMemPitch = 0;
        vertexBufferData.SysMemSlicePitch = 0;
        CD3D11_BUFFER_DESC vertexBufferDesc(sizeof(cubeVertices), D3D11_BIND_VERTEX_BUFFER);
        ThrowIfFailed(
            mDevice->CreateBuffer(
            &vertexBufferDesc,
            &vertexBufferData,
            &mStickerVertexBuffer
            )
            );

        unsigned short cubeIndices[] =
        {
            // Front colored side
            0, 2, 1,
            1, 2, 3,

            // Black back side
            //	4,5,6,
            //	5,7,6,
        };

        mStickerIndexCount = ARRAYSIZE(cubeIndices);

        D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
        indexBufferData.pSysMem = cubeIndices;
        indexBufferData.SysMemPitch = 0;
        indexBufferData.SysMemSlicePitch = 0;
        CD3D11_BUFFER_DESC indexBufferDesc(sizeof(cubeIndices), D3D11_BIND_INDEX_BUFFER);
        ThrowIfFailed(
            mDevice->CreateBuffer(
            &indexBufferDesc,
            &indexBufferData,
            &mStickerIndexBuffer
            )
            );
    });

    createCubeTask.then([this]() {
        mStickerLoadingComplete = true;
    });
}

void RendererD3D::RenderSticker(Sticker* pSticker, glm::mat4 *pWorldMatrix, glm::mat4 *pViewMatrix, glm::mat4 *pProjectionMatrix)
{
    if (!mStickerLoadingComplete)
    {
        return;
    }

    mStickerConstantBufferData.color = ColorToVec4(pSticker->GetColor());

    mStickerConstantBufferData.model = *pWorldMatrix;
    mStickerConstantBufferData.view = *pViewMatrix;
    mStickerConstantBufferData.projection = *pProjectionMatrix;

    mContext->UpdateSubresource(
        mStickerConstantBuffer.Get(),
        0,
        NULL,
        &mStickerConstantBufferData,
        0,
        0
        );

    UINT stride = sizeof(VertexPositionColor);
    UINT offset = 0;
    mContext->IASetVertexBuffers(
        0,
        1,
        mStickerVertexBuffer.GetAddressOf(),
        &stride,
        &offset
        );

    mContext->IASetIndexBuffer(
        mStickerIndexBuffer.Get(),
        DXGI_FORMAT_R16_UINT,
        0
        );

    mContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    mContext->IASetInputLayout(mStickerInputLayout.Get());

    mContext->VSSetShader(
        mStickerVertexShader.Get(),
        nullptr,
        0
        );

    mContext->VSSetConstantBuffers(
        0,
        1,
        mStickerConstantBuffer.GetAddressOf()
        );

    mContext->PSSetShader(
        mStickerPixelShader.Get(),
        nullptr,
        0
        );

    mContext->DrawIndexed(
        mStickerIndexCount,
        0,
        0
        );
}

void RendererD3D::Clear()
{
    const float midnightBlue[] = { 0.098f, 0.098f, 0.439f, 1.000f };
    mContext->ClearRenderTargetView(
    	m_renderTargetView.Get(),
    	midnightBlue
    	);

    mContext->ClearDepthStencilView(
    	m_depthStencilView.Get(),
    	D3D11_CLEAR_DEPTH,
    	1.0f,
    	0
    	);

    mContext->OMSetRenderTargets(
    	1,
    	m_renderTargetView.GetAddressOf(),
    	m_depthStencilView.Get()
    	);
}