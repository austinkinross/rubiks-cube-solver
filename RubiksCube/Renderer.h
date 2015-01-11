#pragma once

#include "pch.h"

using namespace DirectX;

class Sticker;

class Renderer
{
public:
    Renderer() { };

    virtual void RenderSticker(Sticker* pSticker, XMFLOAT4X4 *pWorldMatrix, XMFLOAT4X4 *pViewMatrix, XMFLOAT4X4 *pProjectionMatrix) = 0;

    virtual void Swap() = 0;

    virtual void Clear() = 0;

    virtual void UpdateForWindowSizeChange() = 0;
};

