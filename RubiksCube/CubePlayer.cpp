#include "pch.h"
#include "CubePlayer.h"


CubePlayer::CubePlayer()
{
	pCube = new Cube();
	Reset();
	bPaused = false;
}

CubePlayer::~CubePlayer()
{
    delete pCube;
}

void CubePlayer::Reset()
{
	uiCurrentCommandPos = 0;
	fCurrentCommandProportion = 0.0f;
}

void CubePlayer::UseCommandList(CubeCommandList *pCommandList)
{
	this->pCommandList = pCommandList;
	Reset();
}

void CubePlayer::Pause()
{
	// bPaused = true;
}

void CubePlayer::Play()
{
	bPaused = false;
}

UINT tempCount = 0;

//-------------------------------------------------------------------------------------------------------------------------------------------------------
// NOT NEEDED
//-------------------------------------------------------------------------------------------------------------------------------------------------------
void CubePlayer::Update(float timeTotal, float timeDelta)
{
//	XMStoreFloat4x4(&pPlaybackCube->worldMatrix, XMMatrixTranspose(XMMatrixMultiply(XMMatrixRotationY(timeTotal * XM_PIDIV4), XMMatrixRotationX((float) sin(0/3) * XM_PIDIV4))));

	if (!bPaused && uiCurrentCommandPos < pCommandList->GetLength())
	{
		CubeCommand currentCommand = pCommandList->GetCommandAt(uiCurrentCommandPos);

		fCurrentCommandProportion += timeDelta / fTimePerMove;

		if (fCurrentCommandProportion >= 1.0f)
		{
            pCube->ApplyCommand(currentCommand);

			fCurrentCommandProportion = 0.0f;
			uiCurrentCommandPos += 1;

			if (uiCurrentCommandPos < pCommandList->GetLength())
			{
				CubeCommand nextCommand = pCommandList->GetCommandAt(uiCurrentCommandPos);
				while (nextCommand == CubeRotateY && uiCurrentCommandPos < pCommandList->GetLength())
				{
                    pCube->ApplyCommand(nextCommand);
					uiCurrentCommandPos += 1;	
					if (uiCurrentCommandPos < pCommandList->GetLength())
					{
						nextCommand = pCommandList->GetCommandAt(uiCurrentCommandPos);
					}
				}
			}

			// Maybe call Update again with a reduced time delta?
		}

		// Now that we've performed the actual twist (if it was needed), we should update the slices' angles

		float fRotationAngle = (-fCurrentCommandProportion*fCurrentCommandProportion*fCurrentCommandProportion + 2*fCurrentCommandProportion*fCurrentCommandProportion) * (float)(XM_PIDIV2);
		if (IsPrimeCubeCommand(currentCommand))
		{
			fRotationAngle *= -1;
		}

		switch(currentCommand)
		{
		case CubeCommandLeft:
		case CubeCommandLeftPrime:
            pCube->pLeftSlice->SetAngle(fRotationAngle);
			break;
		case CubeCommandRight:
		case CubeCommandRightPrime:
            pCube->pRightSlice->SetAngle(fRotationAngle);
			break;
		case CubeCommandTop:
		case CubeCommandTopPrime:
            pCube->pTopSlice->SetAngle(fRotationAngle);
			break;
		case CubeCommandBottom:
		case CubeCommandBottomPrime:
            pCube->pBottomSlice->SetAngle(fRotationAngle);
			break;
		case CubeCommandFront:
		case CubeCommandFrontPrime:
            pCube->pFrontSlice->SetAngle(fRotationAngle);
			break;
		case CubeCommandBack:
		case CubeCommandBackPrime:
            pCube->pBackSlice->SetAngle(fRotationAngle);
			break;
		}
		
	}
}