#pragma once

#include "pch.h"
#include "Direct3DBase.h"
#include "CubeHelperFunctions.h"
#include "Sticker.h"

using namespace DirectX;

class Sticker_NULL : public Sticker
{

public:
    Sticker_NULL(StickerColor color);

    virtual void InitializeModels(XMFLOAT4X4 *pCubeWorld, XMFLOAT4X4 sideRotation, int pos1, int pos2);
    virtual void Draw(XMFLOAT4X4 *pViewMatrix, XMFLOAT4X4 *pProjectionMatrix);
};