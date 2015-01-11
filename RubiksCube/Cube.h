#pragma once

#include "pch.h"
#include "Slice.h"
#include "CubeHelperFunctions.h"
#include "CubeRenderer.h"

class Cube
{
public:
    EXPORTFORTEST Cube();

	void InitializeModels();
	EXPORTFORTEST void Randomize(UINT seed);

	void Draw(CubeRenderer* pCubeRenderer, XMFLOAT4X4 *pViewMatrix, XMFLOAT4X4 *pProjectionMatrix);
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

protected:
    void InitializeSlices();
};
