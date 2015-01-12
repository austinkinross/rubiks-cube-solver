#pragma once

#include "..\Cube\Cube.h"
#include "..\Cube\Sticker.h"

using namespace DirectX;

class Renderer
{
public:
    Renderer() { };

    virtual void DrawCube(Cube* pCube, XMFLOAT4X4 *pViewMatrix, XMFLOAT4X4 *pProjectionMatrix) { pCube->Draw(this, pViewMatrix, pProjectionMatrix); };

    virtual void RenderSticker(Sticker* pSticker, XMFLOAT4X4 *pWorldMatrix, XMFLOAT4X4 *pViewMatrix, XMFLOAT4X4 *pProjectionMatrix) = 0;

    virtual void Swap() = 0;

    virtual void Clear() = 0;

    virtual void UpdateForWindowSizeChange() = 0;
};
