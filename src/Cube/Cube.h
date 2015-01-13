#pragma once

#include "Slice.h"
#include "CubeHelperFunctions.h"

class Renderer;

class Cube
{
public:
    Cube();

	void Randomize(unsigned int seed);

	void Draw(Renderer* pRenderer, glm::mat4 *pViewMatrix, glm::mat4 *pProjectionMatrix);
	void SilentlyRotateY();
	void ApplyCommand(CubeCommand command);

    void SetFoldAngle(float cubeFoldAngle);

	Slice* pLeftSlice;
	Slice* pRightSlice;
	Slice* pTopSlice;
	Slice* pBottomSlice;
	Slice* pFrontSlice;
	Slice* pBackSlice;

	glm::mat4 worldMatrix;

	Sticker* leftFaceStickers[3][3];
	Sticker* rightFaceStickers[3][3];
	Sticker* topFaceStickers[3][3];
	Sticker* bottomFaceStickers[3][3];
	Sticker* frontFaceStickers[3][3];
	Sticker* backFaceStickers[3][3];

    glm::mat4 leftFaceWorldMatrix;
    glm::mat4 rightFaceWorldMatrix;
    glm::mat4 topFaceWorldMatrix;
    glm::mat4 bottomFaceWorldMatrix;
    glm::mat4 frontFaceWorldMatrix;
    glm::mat4 backFaceWorldMatrix;

protected:
    void InitializeSlices();
};
