#include "Slice.h"

//-------------------------------------------------------------------------------------------------------------------------------------------------------
// A "Slice" represents one  3x3x1 part of the cube that you can rotate. For example, the "front" slice, the "left" slice, the "top" slice etc
// Each slice has a world matrix associated with it that should be a rotation matrix about the slice's natural axis.
// Each slice is also associated with the stickers that make up the slice. This allows the slice's world matrix to be passed into the stickers to draw them at an angle.
//-------------------------------------------------------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------------------------------------------------------
// The constructor takes:
//		1) A SliceRotationType that defines the axis about which the slice rotates, and whether a positive or negative angle is needed to generate a clockwise motion
//-------------------------------------------------------------------------------------------------------------------------------------------------------
Slice::Slice(SliceRotationType rotationType)
{
	this->rotationType = rotationType;
    this->rotationMatrix = glm::mat4();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------
// This lets the cube attach the 9 stickers that make up the slice's main face
//-------------------------------------------------------------------------------------------------------------------------------------------------------
void Slice::AssociateFace(Sticker *paCubeSides[3][3])
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			face[i][j] = paCubeSides[i][j];
			face[i][j]->AttachRotationMatrix(&rotationMatrix);
		}
	}
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------
// This lets the cube attach 3 stickers that are not on the slice's main face, but get rotated when the slice is rotated
//-------------------------------------------------------------------------------------------------------------------------------------------------------
void Slice::AssociatePartialFace(unsigned int faceID, Sticker *pSticker1, Sticker *pSticker2, Sticker *pSticker3)
{
	if(faceID == 0)
	{
		partialFace0[0] = pSticker1;
		partialFace0[1] = pSticker2;
		partialFace0[2] = pSticker3;
	}
	if(faceID == 1)
	{
		partialFace1[0] = pSticker1;
		partialFace1[1] = pSticker2;
		partialFace1[2] = pSticker3;
	}
	if(faceID == 2)
	{
		partialFace2[0] = pSticker1;
		partialFace2[1] = pSticker2;
		partialFace2[2] = pSticker3;
	}
	if(faceID == 3)
	{
		partialFace3[0] = pSticker1;
		partialFace3[1] = pSticker2;
		partialFace3[2] = pSticker3;
	}

	pSticker1->AttachRotationMatrix(&rotationMatrix);
	pSticker2->AttachRotationMatrix(&rotationMatrix);
	pSticker3->AttachRotationMatrix(&rotationMatrix);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------
// This adjusts the slice's world matrix to be a rotation about the slice's natural axis by the given angle
//-------------------------------------------------------------------------------------------------------------------------------------------------------
void Slice::SetAngle(float angle)
{
	glm::mat4 tempRotationMatrix;

	switch(rotationType)
	{
	case SliceRotationType_PositiveX:
        tempRotationMatrix = glm::rotate(glm::mat4(), angle, glm::vec3(1, 0, 0));
		break;
	case SliceRotationType_NegativeX:
        tempRotationMatrix = glm::rotate(glm::mat4(), -angle, glm::vec3(1, 0, 0));
		break;
	case SliceRotationType_PositiveY:
        tempRotationMatrix = glm::rotate(glm::mat4(), angle, glm::vec3(0, 1, 0));
		break;
	case SliceRotationType_NegativeY:
        tempRotationMatrix = glm::rotate(glm::mat4(), -angle, glm::vec3(0, 1, 0));
		break;
	case SliceRotationType_PositiveZ:
        tempRotationMatrix = glm::rotate(glm::mat4(), angle, glm::vec3(0, 0, 1));
		break;
	case SliceRotationType_NegativeZ:
        tempRotationMatrix = glm::rotate(glm::mat4(), - angle, glm::vec3(0, 0, 1));
		break;
	}

    rotationMatrix = glm::transpose(tempRotationMatrix);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------
// This instantaneously performs a twist on a slice by the given twist type
//-------------------------------------------------------------------------------------------------------------------------------------------------------
void Slice::ApplyTwist(TwistType twist)
{
	StickerColor tempFaceColor[3][3];
	StickerColor tempPartialFace[3];
	if (twist == TwistTypeIdentity)
	{
		// do nothing
	}
	else if (twist == TwistType90)
	{
		for (int i = 0; i < 3; i++)
		{
			tempPartialFace[i] = partialFace3[i]->GetColor();
		}

		for (int j = 2; j >= 0; j--)
		{
			for (int i = 0; i < 3; i++)
			{
				tempFaceColor[2-j][i] = face[i][j]->GetColor();
			}
		}

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				face[i][j]->SetColor(tempFaceColor[i][j]);
			}
		}

		for (int i = 0; i < 3; i++)
		{
			partialFace3[i]->SetColor(partialFace0[i]->GetColor());
			partialFace0[i]->SetColor(partialFace1[i]->GetColor());
			partialFace1[i]->SetColor(partialFace2[i]->GetColor());
			partialFace2[i]->SetColor(tempPartialFace[i]);
		}
	} 
	else if (twist == TwistType180)
	{
		// Very lazy implementation. Maybe fix later?
		ApplyTwist(TwistType90);
		ApplyTwist(TwistType90);
	}
	else if (twist == TwistType270)
	{
		// Very lazy implementation. Maybe fix later?
		ApplyTwist(TwistType90);
		ApplyTwist(TwistType90);
		ApplyTwist(TwistType90);
	}
}