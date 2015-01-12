#include "pch.h"
#include "CubePlayer.h"


CubePlayer::CubePlayer()
{
	mCube = new Cube();
	Reset();
	bPaused = false;
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

	if (!bPaused && uiCurrentCommandPos < mCubeCommandList->GetLength())
	{
		CubeCommand currentCommand = mCubeCommandList->GetCommandAt(uiCurrentCommandPos);

		fCurrentCommandProportion += timeDelta / fTimePerMove;

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

		float fRotationAngle = (-fCurrentCommandProportion*fCurrentCommandProportion*fCurrentCommandProportion + 2*fCurrentCommandProportion*fCurrentCommandProportion) * (float)(3.141592f / 2);
		if (IsPrimeCubeCommand(currentCommand))
		{
			fRotationAngle *= -1;
		}

		switch(currentCommand)
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