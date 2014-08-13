#pragma once

#include "pch.h"
#include "Direct3DBase.h"
#include "CubeHelperFunctions.h"

using namespace DirectX;

class Sticker 
{

public:
	Sticker();
	Sticker(StickerColor color);

    virtual void InitializeModels(XMFLOAT4X4 *pCubeWorld, XMFLOAT4X4 sideRotation, int pos1, int pos2) = 0;

    virtual void Draw(XMFLOAT4X4 *pViewMatrix, XMFLOAT4X4 *pProjectionMatrix) = 0;

	void AttachRotationMatrix(XMFLOAT4X4 *pRotationMatrix);
	void SetColor(StickerColor color);
	EXPORTFORTEST StickerColor GetColor();

protected:

	StickerColor color;

    XMFLOAT4X4* pCubeProjection;
	XMFLOAT4X4* pCubeView;
	XMFLOAT4X4* pCubeWorld;

	XMFLOAT4X4* pRotation1;
	XMFLOAT4X4* pRotation2;
	XMFLOAT4X4* pRotation3;

	XMFLOAT4X4 worldMatrix;
};