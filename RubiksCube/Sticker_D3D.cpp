#include "pch.h"
#include "Sticker_D3D.h"

//-------------------------------------------------------------------------------------------------------------------------------------------------------
// A "Sticker" represents a single face of a single cublet.
// It has several matrices associated with it:
//		1) Its own world matrix that determines where it is positioned in relation to the rest of the cube.
//		2) The rotation matrices of the various slices that a sticker may be in. (Note: only one of these should be non-identity at a time)
//		3) Its parent cube's world matrix
// The sticker's final world matrix is the product of these matrices.
//-------------------------------------------------------------------------------------------------------------------------------------------------------

Sticker_D3D::Sticker_D3D(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, StickerColor color) : Sticker(color)
{
    this->color = color;
    pRotation1 = NULL;
    pRotation2 = NULL;
    pRotation3 = NULL;

    mDevice = pDevice;
    mContext = pContext;
}

void Sticker_D3D::InitializeModels(XMFLOAT4X4 *pCubeWorld, XMFLOAT4X4 sideRotation, int pos1, int pos2)
{
    XMStoreFloat4x4(&worldMatrix, XMMatrixMultiply(XMMatrixTranslation(2.0f * pos1 - 2.0f, 0.0f, 2.0f * pos2 - 2.0f), XMLoadFloat4x4(&sideRotation)));

    this->pCubeWorld = pCubeWorld;

    m_constantBufferData.color = ColorToXMFLOAT4(color);

    auto loadVSTask = DX::ReadDataAsync("SimpleVertexShader.cso");
    auto loadPSTask = DX::ReadDataAsync("SimplePixelShader.cso");

    auto createVSTask = loadVSTask.then([this](Platform::Array<byte>^ fileData) {
        DX::ThrowIfFailed(
            mDevice->CreateVertexShader(
            fileData->Data,
            fileData->Length,
            nullptr,
            &m_vertexShader
            )
            );

        const D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };

        DX::ThrowIfFailed(
            mDevice->CreateInputLayout(
            vertexDesc,
            ARRAYSIZE(vertexDesc),
            fileData->Data,
            fileData->Length,
            &m_inputLayout
            )
            );
    });


    auto createPSTask = loadPSTask.then([this](Platform::Array<byte>^ fileData) {
        DX::ThrowIfFailed(
            mDevice->CreatePixelShader(
            fileData->Data,
            fileData->Length,
            nullptr,
            &m_pixelShader
            )
            );

        CD3D11_BUFFER_DESC constantBufferDesc(sizeof(SideCubeConstantBuffer), D3D11_BIND_CONSTANT_BUFFER);
        DX::ThrowIfFailed(
            mDevice->CreateBuffer(
            &constantBufferDesc,
            nullptr,
            &m_constantBuffer
            )
            );
    });

    auto createCubeTask = (createPSTask && createVSTask).then([this]() {
        VertexPositionColor cubeVertices[] =
        {
            { XMFLOAT3(-0.95f, 3.0f, -0.95f), XMFLOAT3(0, 0, 0) },
            { XMFLOAT3(-0.95f, 3.0f, +0.95f), XMFLOAT3(0, 0, 0) },
            { XMFLOAT3(+0.95f, 3.0f, -0.95f), XMFLOAT3(0, 0, 0) },
            { XMFLOAT3(+0.95f, 3.0f, +0.95f), XMFLOAT3(0, 0, 0) },

            // Cheap way to draw some black back faces
            /*		{XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(0,0,0)},
            {XMFLOAT3(-1.0f, 1.0f, +1.0f), XMFLOAT3(0,0,0)},
            {XMFLOAT3(+1.0f, 1.0f, -1.0f), XMFLOAT3(0,0,0)},
            {XMFLOAT3(+1.0f, 1.0f, +1.0f), XMFLOAT3(0,0,0)},
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
        DX::ThrowIfFailed(
            mDevice->CreateBuffer(
            &vertexBufferDesc,
            &vertexBufferData,
            &m_vertexBuffer
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

        m_indexCount = ARRAYSIZE(cubeIndices);

        D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
        indexBufferData.pSysMem = cubeIndices;
        indexBufferData.SysMemPitch = 0;
        indexBufferData.SysMemSlicePitch = 0;
        CD3D11_BUFFER_DESC indexBufferDesc(sizeof(cubeIndices), D3D11_BIND_INDEX_BUFFER);
        DX::ThrowIfFailed(
            mDevice->CreateBuffer(
            &indexBufferDesc,
            &indexBufferData,
            &m_indexBuffer
            )
            );
    });

    createCubeTask.then([this]() {
        m_loadingComplete = true;
    });

}

void Sticker_D3D::Draw(XMFLOAT4X4 *pViewMatrix, XMFLOAT4X4 *pProjectionMatrix)
{
    // Only draw the cube once it is loaded (loading is asynchronous).
    if (!m_loadingComplete)
    {
        return;
    }

    m_constantBufferData.color = ColorToXMFLOAT4(color);

    ConfigureShaderMatrices(pViewMatrix, pProjectionMatrix);

    mContext->UpdateSubresource(
        m_constantBuffer.Get(),
        0,
        NULL,
        &m_constantBufferData,
        0,
        0
        );

    UINT stride = sizeof(VertexPositionColor);
    UINT offset = 0;
    mContext->IASetVertexBuffers(
        0,
        1,
        m_vertexBuffer.GetAddressOf(),
        &stride,
        &offset
        );

    mContext->IASetIndexBuffer(
        m_indexBuffer.Get(),
        DXGI_FORMAT_R16_UINT,
        0
        );

    mContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    mContext->IASetInputLayout(m_inputLayout.Get());

    mContext->VSSetShader(
        m_vertexShader.Get(),
        nullptr,
        0
        );

    mContext->VSSetConstantBuffers(
        0,
        1,
        m_constantBuffer.GetAddressOf()
        );

    mContext->PSSetShader(
        m_pixelShader.Get(),
        nullptr,
        0
        );

    mContext->DrawIndexed(
        m_indexCount,
        0,
        0
        );
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------
// This sets up the sticker's constant buffer before it is drawn
//-------------------------------------------------------------------------------------------------------------------------------------------------------
void Sticker_D3D::ConfigureShaderMatrices(XMFLOAT4X4 *pViewMatrix, XMFLOAT4X4 *pProjectionMatrix)
{
    XMFLOAT4X4 cumulativeWorld = worldMatrix;

    // cumulativeWorld = worldMatrix, then rotation1, then rotation2, then rotation3, then cubeWorld

    if (pRotation1 != NULL)
    {
        XMStoreFloat4x4(&cumulativeWorld, XMMatrixMultiply(XMLoadFloat4x4(&cumulativeWorld), XMLoadFloat4x4(pRotation1)));
    }

    if (pRotation2 != NULL)
    {
        XMStoreFloat4x4(&cumulativeWorld, XMMatrixMultiply(XMLoadFloat4x4(&cumulativeWorld), XMLoadFloat4x4(pRotation2)));
    }

    if (pRotation3 != NULL)
    {
        XMStoreFloat4x4(&cumulativeWorld, XMMatrixMultiply(XMLoadFloat4x4(&cumulativeWorld), XMLoadFloat4x4(pRotation3)));
    }

    XMStoreFloat4x4(&cumulativeWorld, XMMatrixMultiply(XMLoadFloat4x4(&cumulativeWorld), XMLoadFloat4x4(pCubeWorld)));

    // Set final values. Note, the matrices must be transposed for the shaders!
    XMStoreFloat4x4(&m_constantBufferData.model, XMMatrixTranspose((XMLoadFloat4x4(&cumulativeWorld))));
    XMStoreFloat4x4(&m_constantBufferData.view, XMMatrixTranspose(XMLoadFloat4x4(pViewMatrix)));
    XMStoreFloat4x4(&m_constantBufferData.projection, XMMatrixTranspose(XMLoadFloat4x4(pProjectionMatrix)));

}