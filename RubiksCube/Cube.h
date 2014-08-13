#pragma once

#include "pch.h"
#include "Sticker.h"
#include "Slice.h"
#include "CubeHelperFunctions.h"

class Cube
{
public:
	EXPORTFORTEST Cube();	

	void InitializeModel(ID3D11Device1 *pDevice);
	EXPORTFORTEST void Randomize(UINT seed);

	void Draw(ID3D11DeviceContext1 *pContext, XMFLOAT4X4 *pViewMatrix, XMFLOAT4X4 *pProjectionMatrix);
	void SilentlyRotateY();
	void ApplyCommand(CubeCommand command);

	Slice* pLeftSlice;
	Slice* pRightSlice;
	Slice* pTopSlice;
	Slice* pBottomSlice;
	Slice* pFrontSlice;
	Slice* pBackSlice;

	XMFLOAT4X4 worldMatrix;

	Sticker* leftFaceStickers[3][3];
	Sticker* rightFaceStickers[3][3];
	Sticker* topFaceStickers[3][3];
	Sticker* bottomFaceStickers[3][3];
	Sticker* frontFaceStickers[3][3];
	Sticker* backFaceStickers[3][3];

};
