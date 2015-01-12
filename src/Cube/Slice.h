#pragma once

#include "Sticker.h"
#include "CubeHelperFunctions.h"

enum SliceRotationType
{
	SliceRotationType_PositiveX,
	SliceRotationType_NegativeX,
	SliceRotationType_PositiveY,
	SliceRotationType_NegativeY,
	SliceRotationType_PositiveZ,
	SliceRotationType_NegativeZ,
};

class Slice 
{
public:
	Slice(SliceRotationType rotationType);

	void AssociateFace(Sticker *paCubeSides[3][3]);
	void ApplyTwist(TwistType rot);
	void AssociatePartialFace(unsigned int faceID, Sticker *pSticker1, Sticker *pSticker2, Sticker *pSticker3);
	void Slice::SetAngle(float angle);

private:

	// The rotationType determines what axis, and which direction, the slice needs to move in to move clockwise when looking at it
	SliceRotationType rotationType;
	XMFLOAT4X4 rotationMatrix;	

	Sticker* face[3][3];
	Sticker* partialFace0[3];
	Sticker* partialFace1[3];
	Sticker* partialFace2[3];
	Sticker* partialFace3[3];
};