#include "pch.h"
#include "CubePlayer.h"


CubePlayer::CubePlayer(Cube *pCube)
{
	this->pPlaybackCube = pCube;
	Reset();
	bPaused = false;
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
			pPlaybackCube->ApplyCommand(currentCommand);		

			fCurrentCommandProportion = 0.0f;
			uiCurrentCommandPos += 1;

			if (uiCurrentCommandPos < pCommandList->GetLength())
			{
				CubeCommand nextCommand = pCommandList->GetCommandAt(uiCurrentCommandPos);
				while (nextCommand == CubeRotateY && uiCurrentCommandPos < pCommandList->GetLength())
				{
					pPlaybackCube->ApplyCommand(nextCommand);	
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
			pPlaybackCube->pLeftSlice->SetAngle(fRotationAngle);
			break;
		case CubeCommandRight:
		case CubeCommandRightPrime:
			pPlaybackCube->pRightSlice->SetAngle(fRotationAngle);
			break;
		case CubeCommandTop:
		case CubeCommandTopPrime:
			pPlaybackCube->pTopSlice->SetAngle(fRotationAngle);
			break;
		case CubeCommandBottom:
		case CubeCommandBottomPrime:
			pPlaybackCube->pBottomSlice->SetAngle(fRotationAngle);
			break;
		case CubeCommandFront:
		case CubeCommandFrontPrime:
			pPlaybackCube->pFrontSlice->SetAngle(fRotationAngle);
			break;
		case CubeCommandBack:
		case CubeCommandBackPrime:
			pPlaybackCube->pBackSlice->SetAngle(fRotationAngle);
			break;
		}
		
	}
}



	//tempCount ++;

	//if (tempCount <= 60)
	//{

	//	if (tempCount % 60 == 0)
	//	{
	//		pPlaybackCube->pBottomSlice->ApplyTwist(TwistType270);
	//		pPlaybackCube->pBottomSlice->SetAngle(0);
	//	}
	//	else
	//	{
	//		pPlaybackCube->pBottomSlice->SetAngle((tempCount % 60) * - XM_PIDIV2 / 60);
	//	}

	//}
	//else if (tempCount <= 120)
	//{

	//	if (tempCount % 60 == 0)
	//	{
	//		pPlaybackCube->pTopSlice->ApplyTwist(TwistType90);
	//		pPlaybackCube->pTopSlice->SetAngle(0);
	//	}
	//	else
	//	{
	//		pPlaybackCube->pTopSlice->SetAngle((tempCount % 60) * XM_PIDIV2 / 60);
	//		// XMStoreFloat4x4(&pTopSlice->rotationMatrix, XMMatrixTranspose(XMMatrixRotationY((tempCount % 60) * XM_PIDIV2 / 60)));
	//	}

	//}
	//else if (tempCount <= 180)
	//{

	//	if (tempCount % 60 == 0)
	//	{
	//		pPlaybackCube->pBackSlice->ApplyTwist(TwistType270);
	//		pPlaybackCube->pBackSlice->SetAngle(0);
	//	}
	//	else
	//	{
	//		pPlaybackCube->pBackSlice->SetAngle((tempCount % 60) * -XM_PIDIV2 / 60);
	//		// XMStoreFloat4x4(&pBackSlice->rotationMatrix, XMMatrixTranspose(XMMatrixRotationZ((tempCount % 60) * -XM_PIDIV2 / 60)));
	//	}

	//}
	//else if (tempCount <= 240)
	//{

	//	if (tempCount % 60 == 0)
	//	{
	//		pPlaybackCube->pFrontSlice->ApplyTwist(TwistType90);
	//		pPlaybackCube->pFrontSlice->SetAngle(0);
	//	}
	//	else
	//	{
	//		pPlaybackCube->pFrontSlice->SetAngle((tempCount % 60) * XM_PIDIV2 / 60);
	//		// XMStoreFloat4x4(&pFrontSlice->rotationMatrix, XMMatrixTranspose(XMMatrixRotationZ((tempCount % 60) * XM_PIDIV2 / 60)));
	//	}

	//}
	//else if (tempCount <= 300)
	//{

	//	if (tempCount % 60 == 0)
	//	{
	//		pPlaybackCube->pRightSlice->ApplyTwist(TwistType90);
	//		pPlaybackCube->pRightSlice->SetAngle(0);
	//	}
	//	else
	//	{
	//		pPlaybackCube->pRightSlice->SetAngle((tempCount % 60) * XM_PIDIV2 / 60);
	//		// XMStoreFloat4x4(&pRightSlice->rotationMatrix, XMMatrixTranspose(XMMatrixRotationX((tempCount % 60) * -XM_PIDIV2 / 60)));
	//	}

	//}
	//else if (tempCount <= 360)
	//{

	//	if (tempCount % 60 == 0)
	//	{
	//		pPlaybackCube->pLeftSlice->ApplyTwist(TwistType270);
	//		pPlaybackCube->pLeftSlice->SetAngle(0);
	//	}
	//	else
	//	{
	//		pPlaybackCube->pLeftSlice->SetAngle((tempCount % 60) * - XM_PIDIV2 / 60);
	//	//	XMStoreFloat4x4(&pLeftSlice->rotationMatrix, XMMatrixTranspose(XMMatrixRotationX((tempCount % 60) * XM_PIDIV2 / 60)));
	//	}

	//}
	//else
	//{
	//	tempCount = 0;
	//}

	//if (tempCount % 90 == 0)
	//{
	////	pRightSlice->Rotate((TwistType)0);
	//}
	//else
	//{
	////	XMStoreFloat4x4(&pRightSlice->rotation, XMMatrixTranspose(XMMatrixRotationX((tempCount % 90) * XM_PIDIV2 / 90)));
	//}
	//		

