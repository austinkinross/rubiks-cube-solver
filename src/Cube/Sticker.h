#pragma once

#include "CubeHelperFunctions.h"

class Renderer;

using namespace DirectX;

struct SideCubeConstantBuffer
{
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;
    glm::vec4 color;
};

class Sticker 
{
public:
	Sticker();
    Sticker(StickerColor color, glm::mat4 *pCubeWorld, glm::mat4 *sideRotation, int pos1, int pos2);

    virtual void Draw(Renderer* pRenderer, glm::mat4 *pViewMatrix, glm::mat4 *pProjectionMatrix);

	void AttachRotationMatrix(glm::mat4 *pRotationMatrix);
	void SetColor(StickerColor color);
	StickerColor GetColor();

protected:

    void ConfigureShaderMatrices(glm::mat4 *pViewMatrix, glm::mat4 *pProjectionMatrix);

	StickerColor color;

    glm::mat4* pCubeProjection;
	glm::mat4* pCubeView;
	glm::mat4* pCubeWorld;

	glm::mat4* pRotation1;
	glm::mat4* pRotation2;
	glm::mat4* pRotation3;

	glm::mat4 worldMatrix;

    SideCubeConstantBuffer m_constantBufferData;
};