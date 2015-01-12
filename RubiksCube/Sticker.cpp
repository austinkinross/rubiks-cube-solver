#include "pch.h"
#include "Sticker.h"
#include "Renderer.h"


//-------------------------------------------------------------------------------------------------------------------------------------------------------
// A "Sticker" represents a single face of a single cublet.
// It has several matrices associated with it:
//		1) Its own world matrix that determines where it is positioned in relation to the rest of the cube.
//		2) The rotation matrices of the various slices that a sticker may be in. (Note: only one of these should be non-identity at a time)
//		3) Its parent cube's world matrix
// The sticker's final world matrix is the product of these matrices.
//-------------------------------------------------------------------------------------------------------------------------------------------------------

Sticker::Sticker(StickerColor color, XMFLOAT4X4 *pCubeWorld, XMFLOAT4X4 sideRotation, int pos1, int pos2)
{
	this->color = color;
	pRotation1 = NULL;
	pRotation2 = NULL;
	pRotation3 = NULL;

    XMStoreFloat4x4(&worldMatrix, XMMatrixMultiply(XMMatrixTranslation(2.0f * pos1 - 2.0f, 0.0f, 2.0f * pos2 - 2.0f), XMLoadFloat4x4(&sideRotation)));

    this->pCubeWorld = pCubeWorld;
}

void Sticker::AttachRotationMatrix(XMFLOAT4X4 *pRotationMatrix)
{
	if (pRotation1 == NULL)
	{
		pRotation1 = pRotationMatrix;
		return;
	}
	if (pRotation2 == NULL)
	{
		pRotation2 = pRotationMatrix;
		return;
	}
	if (pRotation3 == NULL)
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

void Sticker::Draw(Renderer* pRenderer, XMFLOAT4X4 *pViewMatrix, XMFLOAT4X4 *pProjectionMatrix)
{
    ConfigureShaderMatrices(pViewMatrix, pProjectionMatrix);

    pRenderer->RenderSticker(this, &m_constantBufferData.model, &m_constantBufferData.view, &m_constantBufferData.projection);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------
// This sets up the sticker's constant buffer before it is drawn
//-------------------------------------------------------------------------------------------------------------------------------------------------------
void Sticker::ConfigureShaderMatrices(XMFLOAT4X4 *pViewMatrix, XMFLOAT4X4 *pProjectionMatrix)
{
    XMFLOAT4X4 cumulativeWorld = worldMatrix;

    // cumulativeWorld = worldMatrix, then rotation1, then rotation2, then rotation3, then cubeWorld

    if (pRotation1 != NULL)
    {
        XMStoreFloat4x4(&cumulativeWorld, XMMatrixMultiply(XMLoadFloat4x4(&cumulativeWorld), XMLoadFloat4x4(pRotation1)));
    }

    if (pRotation2 != NULL)
    {
        XMStoreFloat4x4(&cumulativeWorld, XMMatrixMultiply(XMLoadFloat4x4(&cumulativeWorld), XMLoadFloat4x4(pRotation2)));
    }

    if (pRotation3 != NULL)
    {
        XMStoreFloat4x4(&cumulativeWorld, XMMatrixMultiply(XMLoadFloat4x4(&cumulativeWorld), XMLoadFloat4x4(pRotation3)));
    }

    XMStoreFloat4x4(&cumulativeWorld, XMMatrixMultiply(XMLoadFloat4x4(&cumulativeWorld), XMLoadFloat4x4(pCubeWorld)));

    // Set final values. Note, the matrices must be transposed for the shaders!
    XMStoreFloat4x4(&m_constantBufferData.model, XMMatrixTranspose((XMLoadFloat4x4(&cumulativeWorld))));
    XMStoreFloat4x4(&m_constantBufferData.view, XMMatrixTranspose(XMLoadFloat4x4(pViewMatrix)));
    XMStoreFloat4x4(&m_constantBufferData.projection, XMMatrixTranspose(XMLoadFloat4x4(pProjectionMatrix)));
}