#pragma once

#include "..\Cube\Cube.h"
#include "..\Cube\Sticker.h"

using namespace DirectX;

class Renderer
{
public:
    Renderer() { };

    virtual void DrawCube(Cube* pCube, glm::mat4 *pViewMatrix, glm::mat4 *pProjectionMatrix) { pCube->Draw(this, pViewMatrix, pProjectionMatrix); };

    virtual void RenderSticker(Sticker* pSticker, glm::mat4 *pWorldMatrix, glm::mat4 *pViewMatrix, glm::mat4 *pProjectionMatrix) = 0;

    virtual void Swap() = 0;

    virtual void Clear() = 0;

    virtual void UpdateForWindowSizeChange() = 0;
};
