#include "pch.h"
#include "CubePlayer.h"


CubePlayer::CubePlayer(CubePlayerDesc* desc)
{
	mCube = new Cube();
    mDesc = *desc;
	Reset();
	bPaused = false;

	if (mDesc.populateColors)
	{
		mPlaybackState = PLAYBACK_STATE_POPULATING_COLORS;
		mFoldingAngle = 3.141592653f / 2;

		// Set all the faces to be nearly black
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				mCube->frontFaceStickers[i][j]->SetColor(StickerColor::CUBE_BLACK);
				mCube->backFaceStickers[i][j]->SetColor(StickerColor::CUBE_BLACK);
				mCube->topFaceStickers[i][j]->SetColor(StickerColor::CUBE_BLACK);
				mCube->bottomFaceStickers[i][j]->SetColor(StickerColor::CUBE_BLACK);
				mCube->leftFaceStickers[i][j]->SetColor(StickerColor::CUBE_BLACK);
				mCube->rightFaceStickers[i][j]->SetColor(StickerColor::CUBE_BLACK);
			}
		}
	}
    else if (mDesc.unfoldCubeAtStart)
    {
        mPlaybackState = PLAYBACK_STATE_FOLDING;
        mFoldingAngle = 3.141592653f / 2;
    }
    else
    {
        mPlaybackState = PLAYBACK_STATE_SOLVING;
        mFoldingAngle = 0.0f;
    }

	mCube->SetFoldAngle(mFoldingAngle);
}

CubePlayer::~CubePlayer()
{
    delete mCube;
}

void CubePlayer::Reset()
{
	uiCurrentCommandPos = 0;
	fCurrentCommandProportion = 0.0f;
}

void CubePlayer::UseCommandList(CubeCommandList *pCommandList)
{
	mCubeCommandList = pCommandList;
	Reset();
}

void CubePlayer::Pause()
{
	bPaused = true;
}

void CubePlayer::Play()
{
	bPaused = false;
}

unsigned int tempCount = 0;

//-------------------------------------------------------------------------------------------------------------------------------------------------------
// NOT NEEDED
//-------------------------------------------------------------------------------------------------------------------------------------------------------
void CubePlayer::Update(float timeTotal, float timeDelta)
{
    //	XMStoreFloat4x4(&pPlaybackCube->worldMatrix, XMMatrixTranspose(XMMatrixMultiply(XMMatrixRotationY(timeTotal * XM_PIDIV4), XMMatrixRotationX((float) sin(0/3) * XM_PIDIV4))));

    if (!bPaused)
    {
        if (mPlaybackState == PLAYBACK_STATE_FOLDING)
        {
            mFoldingAngle -= timeDelta / mDesc.speeds.foldingSpeed;

            if (mFoldingAngle < 0.0f)
            {
                mFoldingAngle = 0.0f;
                mPlaybackState = PLAYBACK_STATE_SOLVING;
            }

            mCube->SetFoldAngle(mFoldingAngle);
        }
        else if (mPlaybackState == PLAYBACK_STATE_SOLVING)
        {
            if (uiCurrentCommandPos < mCubeCommandList->GetLength())
            {
                CubeCommand currentCommand = mCubeCommandList->GetCommandAt(uiCurrentCommandPos);

                fCurrentCommandProportion += timeDelta / mDesc.speeds.solvingSpeed;

                if (fCurrentCommandProportion >= 1.0f)
                {
                    mCube->ApplyCommand(currentCommand);

                    fCurrentCommandProportion = 0.0f;
                    uiCurrentCommandPos += 1;

                    if (uiCurrentCommandPos < mCubeCommandList->GetLength())
                    {
                        CubeCommand nextCommand = mCubeCommandList->GetCommandAt(uiCurrentCommandPos);
                        while (nextCommand == CubeRotateY && uiCurrentCommandPos < mCubeCommandList->GetLength())
                        {
                            mCube->ApplyCommand(nextCommand);
                            uiCurrentCommandPos += 1;
                            if (uiCurrentCommandPos < mCubeCommandList->GetLength())
                            {
                                nextCommand = mCubeCommandList->GetCommandAt(uiCurrentCommandPos);
                            }
                        }
                    }

                    // Maybe call Update again with a reduced time delta?
                }

                // Now that we've performed the actual twist (if it was needed), we should update the slices' angles

                float fRotationAngle = (-fCurrentCommandProportion*fCurrentCommandProportion*fCurrentCommandProportion + 2 * fCurrentCommandProportion*fCurrentCommandProportion) * (float)(3.141592f / 2);
                if (IsPrimeCubeCommand(currentCommand))
                {
                    fRotationAngle *= -1;
                }

                switch (currentCommand)
                {
                case CubeCommandLeft:
                case CubeCommandLeftPrime:
                    mCube->pLeftSlice->SetAngle(fRotationAngle);
                    break;
                case CubeCommandRight:
                case CubeCommandRightPrime:
                    mCube->pRightSlice->SetAngle(fRotationAngle);
                    break;
                case CubeCommandTop:
                case CubeCommandTopPrime:
                    mCube->pTopSlice->SetAngle(fRotationAngle);
                    break;
                case CubeCommandBottom:
                case CubeCommandBottomPrime:
                    mCube->pBottomSlice->SetAngle(fRotationAngle);
                    break;
                case CubeCommandFront:
                case CubeCommandFrontPrime:
                    mCube->pFrontSlice->SetAngle(fRotationAngle);
                    break;
                case CubeCommandBack:
                case CubeCommandBackPrime:
                    mCube->pBackSlice->SetAngle(fRotationAngle);
                    break;
                }
            }
        }
    }
}