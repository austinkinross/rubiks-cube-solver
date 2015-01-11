#include "pch.h"
#include "Cube.h"

//-------------------------------------------------------------------------------------------------------------------------------------------------------
// A "Cube" simply represents a Rubik's Cube. 
// It is made up of 6 sets of 9 'stickers' (that make up its faces), and slices to represent 3x3x1 parts of the cube that can be rotated.
//-------------------------------------------------------------------------------------------------------------------------------------------------------

Cube::Cube()
{
    XMStoreFloat4x4(&worldMatrix, XMMatrixRotationX(0.23835f));

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            leftFaceStickers[i][j] = new Sticker(CUBE_RED, &(this->worldMatrix), XMMATRIXToXMFLOAT4X4(&XMMatrixMultiply(XMMatrixRotationY(XM_PIDIV2), XMMatrixRotationZ(-XM_PIDIV2))), i, j);

            rightFaceStickers[i][j] = new Sticker(CUBE_ORANGE, &(this->worldMatrix), XMMATRIXToXMFLOAT4X4(&XMMatrixMultiply(XMMatrixRotationY(-XM_PIDIV2), XMMatrixRotationZ(XM_PIDIV2))), i, j);

            topFaceStickers[i][j] = new Sticker(CUBE_GREEN, &(this->worldMatrix), XMMATRIXToXMFLOAT4X4(&XMMatrixRotationY(0)), i, j);

            bottomFaceStickers[i][j] = new Sticker(CUBE_BLUE, &(this->worldMatrix), XMMATRIXToXMFLOAT4X4(&XMMatrixMultiply(XMMatrixRotationY(XM_PIDIV2), XMMatrixRotationZ(XM_PI))), i, j);

            frontFaceStickers[i][j] = new Sticker(CUBE_YELLOW, &(this->worldMatrix), XMMATRIXToXMFLOAT4X4(&XMMatrixRotationX(XM_PIDIV2)), i, j);

            backFaceStickers[i][j] = new Sticker(CUBE_WHITE, &(this->worldMatrix), XMMATRIXToXMFLOAT4X4(&XMMatrixMultiply(XMMatrixRotationY(XM_PI), XMMatrixRotationX(-XM_PIDIV2))), i, j);
        }
    }

    InitializeSlices();

}

void Cube::InitializeSlices() 
{
	pLeftSlice = new Slice(SliceRotationType_PositiveX);
	pLeftSlice->AssociateFace(leftFaceStickers);
	pLeftSlice->AssociatePartialFace(0, topFaceStickers[2][0],		topFaceStickers[2][1],		topFaceStickers[2][2]);
	pLeftSlice->AssociatePartialFace(1, frontFaceStickers[2][0],	frontFaceStickers[2][1],	frontFaceStickers[2][2]);
	pLeftSlice->AssociatePartialFace(2, bottomFaceStickers[0][0],	bottomFaceStickers[1][0],	bottomFaceStickers[2][0]);
	pLeftSlice->AssociatePartialFace(3, backFaceStickers[0][2],		backFaceStickers[0][1],		backFaceStickers[0][0]);

	pRightSlice = new Slice(SliceRotationType_NegativeX);
	pRightSlice->AssociateFace(rightFaceStickers);
	pRightSlice->AssociatePartialFace(0, topFaceStickers[0][2],		topFaceStickers[0][1],		topFaceStickers[0][0]);
	pRightSlice->AssociatePartialFace(1, backFaceStickers[2][0],	backFaceStickers[2][1],		backFaceStickers[2][2]);
	pRightSlice->AssociatePartialFace(2, bottomFaceStickers[2][2],	bottomFaceStickers[1][2],	bottomFaceStickers[0][2]);
	pRightSlice->AssociatePartialFace(3, frontFaceStickers[0][2],	frontFaceStickers[0][1],	frontFaceStickers[0][0]);

	pFrontSlice = new Slice(SliceRotationType_PositiveZ);
	pFrontSlice->AssociateFace(frontFaceStickers);
	pFrontSlice->AssociatePartialFace(0, topFaceStickers[2][2],		topFaceStickers[1][2],		topFaceStickers[0][2]);
	pFrontSlice->AssociatePartialFace(1, rightFaceStickers[2][0],	rightFaceStickers[2][1],	rightFaceStickers[2][2]);
	pFrontSlice->AssociatePartialFace(2, bottomFaceStickers[0][2],	bottomFaceStickers[0][1],	bottomFaceStickers[0][0]);
	pFrontSlice->AssociatePartialFace(3, leftFaceStickers[0][2],	leftFaceStickers[0][1],		leftFaceStickers[0][0]);

	pBackSlice = new Slice(SliceRotationType_NegativeZ);
	pBackSlice->AssociateFace(backFaceStickers);
	pBackSlice->AssociatePartialFace(0, topFaceStickers[0][0],		topFaceStickers[1][0],		topFaceStickers[2][0]);
	pBackSlice->AssociatePartialFace(1, leftFaceStickers[2][0],		leftFaceStickers[2][1],		leftFaceStickers[2][2]);
	pBackSlice->AssociatePartialFace(2, bottomFaceStickers[2][0],	bottomFaceStickers[2][1],	bottomFaceStickers[2][2]);
	pBackSlice->AssociatePartialFace(3, rightFaceStickers[0][2],	rightFaceStickers[0][1],	rightFaceStickers[0][0]);

	pTopSlice = new Slice(SliceRotationType_PositiveY);
	pTopSlice->AssociateFace(topFaceStickers);
	pTopSlice->AssociatePartialFace(0, frontFaceStickers[0][0],		frontFaceStickers[1][0],	frontFaceStickers[2][0]);
	pTopSlice->AssociatePartialFace(1, leftFaceStickers[0][0],		leftFaceStickers[1][0],		leftFaceStickers[2][0]);
	pTopSlice->AssociatePartialFace(2, backFaceStickers[0][0],		backFaceStickers[1][0],		backFaceStickers[2][0]);
	pTopSlice->AssociatePartialFace(3, rightFaceStickers[0][0],		rightFaceStickers[1][0],	rightFaceStickers[2][0]);

	pBottomSlice = new Slice(SliceRotationType_NegativeY);
	pBottomSlice->AssociateFace(bottomFaceStickers);
	pBottomSlice->AssociatePartialFace(0, frontFaceStickers[2][2],	frontFaceStickers[1][2],	frontFaceStickers[0][2]);
	pBottomSlice->AssociatePartialFace(1, rightFaceStickers[2][2],	rightFaceStickers[1][2],	rightFaceStickers[0][2]);
	pBottomSlice->AssociatePartialFace(2, backFaceStickers[2][2],	backFaceStickers[1][2],		backFaceStickers[0][2]);
	pBottomSlice->AssociatePartialFace(3, leftFaceStickers[2][2],	leftFaceStickers[1][2],		leftFaceStickers[0][2]);
};

void Cube::InitializeModels()
{

}

void Cube::Randomize(UINT seed)
{
    UINT primes[] = { 29, 73, 107, 37, 59, 67, 151, 89, 13, 71, 29, 199, 11, 61, 109, 139, 257, 499, 151, 89, 73, 23, 401, 37, 433, 223, 251 };

    for (int i = 0; i < _countof(primes); i++)
    {
        UINT rot = (seed / primes[i]) % 12;

        switch (rot)
        {
        case 0:
            pLeftSlice->ApplyTwist(TwistType90); break;
        case 1:
            pLeftSlice->ApplyTwist(TwistType270); break;
        case 2:
            pRightSlice->ApplyTwist(TwistType90); break;
        case 3:
            pRightSlice->ApplyTwist(TwistType270); break;
        case 4:
            pTopSlice->ApplyTwist(TwistType90); break;
        case 5:
            pTopSlice->ApplyTwist(TwistType270); break;
        case 6:
            pBottomSlice->ApplyTwist(TwistType90); break;
        case 7:
            pBottomSlice->ApplyTwist(TwistType270); break;
        case 8:
            pFrontSlice->ApplyTwist(TwistType90); break;
        case 9:
            pFrontSlice->ApplyTwist(TwistType270); break;
        case 10:
            pBackSlice->ApplyTwist(TwistType90); break;
        case 11:
            pBackSlice->ApplyTwist(TwistType270); break;
        }
    }

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------
// Draws the cube in the given context, with the given view/projection matrices
//-------------------------------------------------------------------------------------------------------------------------------------------------------
void Cube::Draw(CubeRenderer* pCubeRenderer, XMFLOAT4X4 *pViewMatrix, XMFLOAT4X4 *pProjectionMatrix)
{
	for (int i = 0; i  < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			leftFaceStickers[i][j]->Draw(pCubeRenderer, pViewMatrix, pProjectionMatrix);
            rightFaceStickers[i][j]->Draw(pCubeRenderer, pViewMatrix, pProjectionMatrix);
            topFaceStickers[i][j]->Draw(pCubeRenderer, pViewMatrix, pProjectionMatrix);
            bottomFaceStickers[i][j]->Draw(pCubeRenderer, pViewMatrix, pProjectionMatrix);
            frontFaceStickers[i][j]->Draw(pCubeRenderer, pViewMatrix, pProjectionMatrix);
            backFaceStickers[i][j]->Draw(pCubeRenderer, pViewMatrix, pProjectionMatrix);
		}
	}	
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------
// This is a special method for solving optimisation. It rotates the cube by 90 degrees about the Y axis, and adjusts the cube's 
//-------------------------------------------------------------------------------------------------------------------------------------------------------
void Cube::SilentlyRotateY()
{
	// Switch the left/front/right/back faces colors around
	StickerColor tempFaceColor[3][3];

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			tempFaceColor[i][j] = rightFaceStickers[i][j]->GetColor();
		}
	}

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			rightFaceStickers[i][j]->SetColor(frontFaceStickers[i][j]->GetColor());
		}
	}

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			frontFaceStickers[i][j]->SetColor(leftFaceStickers[i][j]->GetColor());
		}
	}

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			leftFaceStickers[i][j]->SetColor(backFaceStickers[i][j]->GetColor());
		}
	}

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			backFaceStickers[i][j]->SetColor(tempFaceColor[i][j]);
		}
	}

	// Rotate the top face
	for (int j = 2; j >= 0; j--)
	{
		for (int i = 0; i < 3; i++)
		{
			tempFaceColor[2-j][i] = topFaceStickers[i][j]->GetColor();
		}
	}

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			topFaceStickers[i][j]->SetColor(tempFaceColor[i][j]);
		}
	}

	// Rotate the bottom face
	// TODO: This is an extremely lazy implementation. Fix!
	for (int r = 0; r < 3; r++)
	{
		for (int j = 2; j >= 0; j--)
		{
			for (int i = 0; i < 3; i++)
			{
				tempFaceColor[2-j][i] = bottomFaceStickers[i][j]->GetColor();
			}
		}

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				bottomFaceStickers[i][j]->SetColor(tempFaceColor[i][j]);
			}
		}
	}

	
	XMStoreFloat4x4(&worldMatrix, XMMatrixMultiply(XMMatrixRotationY(XM_PIDIV2), XMLoadFloat4x4(&worldMatrix)));
}

void Cube::ApplyCommand(CubeCommand command)
{
	TwistType twist = IsPrimeCubeCommand(command) ? TwistType270 : TwistType90;

	switch(command)
	{
	case CubeCommandLeft:
	case CubeCommandLeftPrime:
		pLeftSlice->ApplyTwist(twist);
		break;
	case CubeCommandRight:
	case CubeCommandRightPrime:
		pRightSlice->ApplyTwist(twist);
		break;
	case CubeCommandTop:
	case CubeCommandTopPrime:
		pTopSlice->ApplyTwist(twist);
		break;
	case CubeCommandBottom:
	case CubeCommandBottomPrime:
		pBottomSlice->ApplyTwist(twist);
		break;
	case CubeCommandFront:
	case CubeCommandFrontPrime:
		pFrontSlice->ApplyTwist(twist);
		break;
	case CubeCommandBack:
	case CubeCommandBackPrime:
		pBackSlice->ApplyTwist(twist);
		break;
	case CubeRotateY:
		SilentlyRotateY();
	}
		
}