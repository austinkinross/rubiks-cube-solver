#include "Sticker.h"
#include "Renderer\Renderer.h"


//-------------------------------------------------------------------------------------------------------------------------------------------------------
// A "Sticker" represents a single face of a single cublet.
// It has several matrices associated with it:
//		1) Its own world matrix that determines where it is positioned in relation to the rest of the cube.
//		2) The rotation matrices of the various slices that a sticker may be in. (Note: only one of these should be non-identity at a time)
//		3) Its parent cube's world matrix
// The sticker's final world matrix is the product of these matrices.
//-------------------------------------------------------------------------------------------------------------------------------------------------------

Sticker::Sticker(StickerColor color, glm::mat4 *pCubeWorld, glm::mat4 *sideRotation, glm::mat4 *faceWorld, int pos1, int pos2)
{
	this->color = color;
	pRotation1 = nullptr;
    pRotation2 = nullptr;
    pRotation3 = nullptr;

    worldMatrix = *sideRotation * glm::translate(glm::mat4(), glm::vec3(2.0f * pos1 - 2.0f, 0.0f, 2.0f * pos2 - 2.0f));
    mFaceMatrix = faceWorld;

    this->pCubeWorld = pCubeWorld;
}

void Sticker::AttachRotationMatrix(glm::mat4 *pRotationMatrix)
{
    if (pRotation1 == nullptr)
	{
		pRotation1 = pRotationMatrix;
		return;
	}
    if (pRotation2 == nullptr)
	{
		pRotation2 = pRotationMatrix;
		return;
	}
    if (pRotation3 == nullptr)
	{
		pRotation3 = pRotationMatrix;
		return;
	}
	assert(0);
}

void Sticker::SetColor(StickerColor color)
{
    this->color = color;
}

StickerColor Sticker::GetColor()
{
	return this->color;
}

void Sticker::Draw(Renderer* pRenderer, glm::mat4 *pViewMatrix, glm::mat4 *pProjectionMatrix)
{
    ConfigureShaderMatrices(pViewMatrix, pProjectionMatrix);

    pRenderer->RenderSticker(this, &m_constantBufferData.model, &m_constantBufferData.view, &m_constantBufferData.projection);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------
// This sets up the sticker's constant buffer before it is drawn
//-------------------------------------------------------------------------------------------------------------------------------------------------------
void Sticker::ConfigureShaderMatrices(glm::mat4 *pViewMatrix, glm::mat4 *pProjectionMatrix)
{
    glm::mat4 cumulativeWorld = worldMatrix;

    // cumulativeWorld = worldMatrix, then rotation1, then rotation2, then rotation3, then cubeWorld

    if (pRotation1 != nullptr)
    {
        cumulativeWorld = *pRotation1 * cumulativeWorld;
    }

    if (pRotation2 != nullptr)
    {
        cumulativeWorld = *pRotation2 * cumulativeWorld;
    }

    if (pRotation3 != nullptr)
    {
        cumulativeWorld = *pRotation3 * cumulativeWorld;
    }

    cumulativeWorld = *pCubeWorld * *mFaceMatrix * cumulativeWorld;

    // Set final values. Note, the matrices must be transposed for the shaders!
    m_constantBufferData.model = glm::transpose(cumulativeWorld);
    m_constantBufferData.view = glm::transpose(*pViewMatrix);
    m_constantBufferData.projection = glm::transpose(*pProjectionMatrix);
}