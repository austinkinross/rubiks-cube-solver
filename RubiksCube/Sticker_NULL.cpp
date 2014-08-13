#include "pch.h"
#include "Sticker_NULL.h"

//-------------------------------------------------------------------------------------------------------------------------------------------------------
// A "Sticker" represents a single face of a single cublet.
// It has several matrices associated with it:
//		1) Its own world matrix that determines where it is positioned in relation to the rest of the cube.
//		2) The rotation matrices of the various slices that a sticker may be in. (Note: only one of these should be non-identity at a time)
//		3) Its parent cube's world matrix
// The sticker's final world matrix is the product of these matrices.
//-------------------------------------------------------------------------------------------------------------------------------------------------------

Sticker_NULL::Sticker_NULL(StickerColor color) : Sticker(color)
{

}

void Sticker_NULL::InitializeModels(XMFLOAT4X4 *pCubeWorld, XMFLOAT4X4 sideRotation, int pos1, int pos2)
{
    throw Platform::Exception::CreateException(E_NOTIMPL, ref new Platform::String(L"Don't initialize the models on the NULL sticker."));
}

void Sticker_NULL::Draw(XMFLOAT4X4 *pViewMatrix, XMFLOAT4X4 *pProjectionMatrix)
{
    throw Platform::Exception::CreateException(E_NOTIMPL, ref new Platform::String(L"Don't draw on the NULL sticker."));
}