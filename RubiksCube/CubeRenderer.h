#pragma once

#include "pch.h"

using namespace DirectX;

class Sticker;

class CubeRenderer
{
public:
    CubeRenderer() { };

    virtual void RenderSticker(Sticker* pSticker, XMFLOAT4X4 *pWorldMatrix, XMFLOAT4X4 *pViewMatrix, XMFLOAT4X4 *pProjectionMatrix) = 0;
};

