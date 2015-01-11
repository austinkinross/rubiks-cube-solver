#pragma once

#include "pch.h"
#include "CubeHelperFunctions.h"
#include "Renderer.h"

using namespace DirectX;

struct SideCubeConstantBuffer
{
    DirectX::XMFLOAT4X4 model;
    DirectX::XMFLOAT4X4 view;
    DirectX::XMFLOAT4X4 projection;
    DirectX::XMFLOAT4 color;
};

class Sticker 
{
public:
	Sticker();
    Sticker(StickerColor color, XMFLOAT4X4 *pCubeWorld, XMFLOAT4X4 sideRotation, int pos1, int pos2);

    virtual void Draw(Renderer* pRenderer, XMFLOAT4X4 *pViewMatrix, XMFLOAT4X4 *pProjectionMatrix);

	void AttachRotationMatrix(XMFLOAT4X4 *pRotationMatrix);
	void SetColor(StickerColor color);
	StickerColor GetColor();

protected:

    void ConfigureShaderMatrices(XMFLOAT4X4 *pViewMatrix, XMFLOAT4X4 *pProjectionMatrix);

	StickerColor color;

    XMFLOAT4X4* pCubeProjection;
	XMFLOAT4X4* pCubeView;
	XMFLOAT4X4* pCubeWorld;

	XMFLOAT4X4* pRotation1;
	XMFLOAT4X4* pRotation2;
	XMFLOAT4X4* pRotation3;

	XMFLOAT4X4 worldMatrix;

    SideCubeConstantBuffer m_constantBufferData;
};