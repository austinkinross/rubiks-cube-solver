#include "pch.h"
#include "CubeRendererD3D.h"

#include "pch.h"
#include "Direct3DBase.h"
#include "CubeHelperFunctions.h"
#include "Sticker.h"
#include "CubeRenderer.h"

struct VertexPositionColor
{
    DirectX::XMFLOAT3 pos;
    DirectX::XMFLOAT3 color;
};

CubeRendererD3D::CubeRendererD3D(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    mDevice = pDevice;
    mContext = pContext;

    // Initialize data to render a sticker

    auto loadVSTask = DX::ReadDataAsync("SimpleVertexShader.cso");
    auto loadPSTask = DX::ReadDataAsync("SimplePixelShader.cso");

    auto createVSTask = loadVSTask.then([this](Platform::Array<byte>^ fileData) {
        DX::ThrowIfFailed(
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

        DX::ThrowIfFailed(
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
        DX::ThrowIfFailed(
            mDevice->CreatePixelShader(
            fileData->Data,
            fileData->Length,
            nullptr,
            &mStickerPixelShader
            )
            );

        CD3D11_BUFFER_DESC constantBufferDesc(sizeof(SideCubeConstantBuffer2), D3D11_BIND_CONSTANT_BUFFER);
        DX::ThrowIfFailed(
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
        DX::ThrowIfFailed(
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

void CubeRendererD3D::RenderSticker(Sticker* pSticker, XMFLOAT4X4 *pWorldMatrix, XMFLOAT4X4 *pViewMatrix, XMFLOAT4X4 *pProjectionMatrix)
{
    mStickerConstantBufferData.color = ColorToXMFLOAT4(pSticker->GetColor());

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