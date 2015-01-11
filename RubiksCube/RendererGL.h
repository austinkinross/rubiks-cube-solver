#pragma once

#include "pch.h"
#include "Renderer.h"

using namespace DirectX;

#include <agile.h>

class Sticker;

class RendererGL : public Renderer
{
public:
    RendererGL();

    virtual void RenderSticker(Sticker* pSticker, XMFLOAT4X4 *pWorldMatrix, XMFLOAT4X4 *pViewMatrix, XMFLOAT4X4 *pProjectionMatrix);

    virtual void Swap();

    virtual void Clear();

    void UpdateForWindowSizeChange();

protected:

};