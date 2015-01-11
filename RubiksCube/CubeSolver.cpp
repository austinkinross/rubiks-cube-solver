#include "pch.h"
#include "CubeSolver.h"

CubeSolver::CubeSolver()
{
    pCube = new Cube();
}

CubeSolver::~CubeSolver()
{
    delete pCube;
}

void CubeSolver::Solve()
{
	PrepareForSolve();

	AddRandomMoves();
 	SolveStage1();
	SolveStage2();
	SolveStage3();
	SolveStage4();
}

void CubeSolver::PrepareForSolve()
{
	m_pCubeCommandList = new CubeCommandList();
}

void CubeSolver::GetCubeCommandList(CubeCommandList **ppCommandList)
{
	*ppCommandList = m_pCubeCommandList;
}

void CubeSolver::AddRandomMoves()
{
//	ApplyCommand(CubeCommandRightPrime);
//	ApplyCommand(CubeCommandRight);

	return;

	// For now, hard code some commands into the command list
	for (int i = 0; i < 10; i++)
	{
		m_pCubeCommandList->AppendCommand(CubeCommandFront);
		m_pCubeCommandList->AppendCommand(CubeCommandBack);
		m_pCubeCommandList->AppendCommand(CubeCommandTop);
		m_pCubeCommandList->AppendCommand(CubeCommandLeft);
		m_pCubeCommandList->AppendCommand(CubeCommandFrontPrime);
		m_pCubeCommandList->AppendCommand(CubeCommandFrontPrime);
		m_pCubeCommandList->AppendCommand(CubeCommandBack);
		m_pCubeCommandList->AppendCommand(CubeCommandBottomPrime);
	}
}


// Solves for the cross on the bottom
void CubeSolver::SolveStage1()
{
	for (int i = 0; i < 4; i++)
	{
        StickerColor rightColor = pCube->rightFaceStickers[1][1]->GetColor();
        StickerColor bottomColor = pCube->bottomFaceStickers[1][1]->GetColor();

		// continue;

		// The four times that the piece is on the right face, and just needs spun into position
		if ( CC(cRight, 1, 0) == rightColor && CC(cTop, 0, 1) == bottomColor)
		{
			ApplyCommand(CubeCommandRight);
			ApplyCommand(CubeCommandRight);
		}
		else if ( CC(cRight, 0, 1) == rightColor && CC(cBack, 2, 1) == bottomColor)
		{
			ApplyCommand(CubeCommandRightPrime);
		}
		else if ( CC(cRight, 2, 1) == rightColor && CC(cFront, 0, 1) == bottomColor)
		{
			ApplyCommand(CubeCommandRight);
		}
		else if ( CC(cRight, 1, 2) == rightColor && CC(cBottom, 1, 2) == bottomColor)
		{
			// Already in the correct place
			int a = 0;
		}
		// The four times that the piece is on the right face, but isn't in a nice orientation
		else if ( CC(cRight, 1, 0) == bottomColor && CC(cTop, 0, 1) == rightColor)
		{
			ApplyCommand(CubeCommandTopPrime);
			ApplyCommand(CubeCommandFrontPrime);
			ApplyCommand(CubeCommandRight);
			ApplyCommand(CubeCommandFront);
		}
		else if ( CC(cRight, 0, 1) == bottomColor && CC(cBack, 2, 1) == rightColor)
		{
			ApplyCommand(CubeCommandBottomPrime);
			ApplyCommand(CubeCommandBack);
			ApplyCommand(CubeCommandBottom);
		}
		else if ( CC(cRight, 2, 1) == bottomColor && CC(cFront, 0, 1) == rightColor)
		{
			ApplyCommand(CubeCommandBottom);
			ApplyCommand(CubeCommandFrontPrime);
			ApplyCommand(CubeCommandBottomPrime);
		}
		else if ( CC(cRight, 1, 2) == bottomColor && CC(cBottom, 1, 2) == rightColor)
		{
			ApplyCommand(CubeCommandBottom);
			ApplyCommand(CubeCommandFront);
			ApplyCommand(CubeCommandBottomPrime);
			ApplyCommand(CubeCommandRight);
		}
		// The four positions where it's on the top of the the middle slice, i.e. not on the right or on the left
		else if ( CC(cTop, 1, 2) == rightColor && CC(cFront, 1, 0) == bottomColor)
		{
			ApplyCommand(CubeCommandFrontPrime);
			ApplyCommand(CubeCommandRight);
			ApplyCommand(CubeCommandFront);
		}
		else if ( CC(cTop, 1, 2) == bottomColor && CC(cFront, 1, 0) == rightColor)
		{
			ApplyCommand(CubeCommandTop);
			ApplyCommand(CubeCommandRight);
			ApplyCommand(CubeCommandRight);
		}
		else if( CC(cTop, 1, 0) == rightColor && CC(cBack, 1, 0) == bottomColor)
		{
			ApplyCommand(CubeCommandBack);
			ApplyCommand(CubeCommandRightPrime);
			ApplyCommand(CubeCommandBackPrime);
		}
		else if( CC(cTop, 1, 0) == bottomColor && CC(cBack, 1, 0) == rightColor)
		{
			ApplyCommand(CubeCommandTopPrime);
			ApplyCommand(CubeCommandRight);
			ApplyCommand(CubeCommandRight);
		}
		// The four positions where it's on the bottom of the middle slice, i.e. not on the right or the left
		else if ( CC(cBottom, 0, 1) == rightColor && CC(cFront, 1, 2) == bottomColor)
		{
			ApplyCommand(CubeCommandFront);
			ApplyCommand(CubeCommandRight);
		}
		else if ( CC(cBottom, 0, 1) == bottomColor && CC(cFront, 1, 2) == rightColor)
		{
			ApplyCommand(CubeCommandFront);
			ApplyCommand(CubeCommandBottom);
			ApplyCommand(CubeCommandFrontPrime);
			ApplyCommand(CubeCommandBottomPrime);
		}
		else if ( CC(cBottom, 2, 1) == rightColor && CC(cBack, 1, 2) == bottomColor)
		{
			ApplyCommand(CubeCommandBackPrime);
			ApplyCommand(CubeCommandRightPrime);
		}
		else if ( CC(cBottom, 2, 1) == bottomColor && CC(cBack, 1, 2) == rightColor)
		{
			ApplyCommand(CubeCommandBackPrime);
			ApplyCommand(CubeCommandBottomPrime);
			ApplyCommand(CubeCommandBack);
			ApplyCommand(CubeCommandBottom);
		}
		// Now, the 8 positions where it's on the left hand face
		else if ( CC(cTop, 2, 1) == rightColor && CC(cLeft, 1, 0) == bottomColor)
		{
			ApplyCommand(CubeCommandLeft);
			ApplyCommand(CubeCommandBottomPrime);
			ApplyCommand(CubeCommandBackPrime);
			ApplyCommand(CubeCommandBottom);
			ApplyCommand(CubeCommandLeftPrime);
		}
		else if ( CC(cTop, 2, 1) == bottomColor && CC(cLeft, 1, 0) == rightColor)
		{
			ApplyCommand(CubeCommandTop);
			ApplyCommand(CubeCommandTop);
			ApplyCommand(CubeCommandRight);
			ApplyCommand(CubeCommandRight);
		}
		else if ( CC(cBack, 0, 1) == rightColor && CC(cLeft, 2, 1) == bottomColor)
		{
			ApplyCommand(CubeCommandBottomPrime);
			ApplyCommand(CubeCommandBackPrime);
			ApplyCommand(CubeCommandBottom);
		}
		else if ( CC(cBack, 0, 1) == bottomColor && CC(cLeft, 2, 1) == rightColor)
		{
			ApplyCommand(CubeCommandBottom);
			ApplyCommand(CubeCommandBottom);
			ApplyCommand(CubeCommandLeft);
			ApplyCommand(CubeCommandBottom);
			ApplyCommand(CubeCommandBottom);
		}
		else if ( CC(cFront, 2, 1) == rightColor && CC(cLeft, 0, 1) == bottomColor)
		{
			ApplyCommand(CubeCommandBottom);
			ApplyCommand(CubeCommandFront);
			ApplyCommand(CubeCommandBottomPrime);	
		}
		else if ( CC(cFront, 2, 1) == bottomColor && CC(cLeft, 0, 1) == rightColor)
		{
			ApplyCommand(CubeCommandBottom);
			ApplyCommand(CubeCommandBottom);
			ApplyCommand(CubeCommandLeftPrime);
			ApplyCommand(CubeCommandBottom);
			ApplyCommand(CubeCommandBottom);	
		}
		else if (CC(cBottom, 1, 0) == bottomColor && CC(cLeft, 1, 2) == rightColor)
		{
			ApplyCommand(CubeCommandLeft);
			ApplyCommand(CubeCommandBottom);
			ApplyCommand(CubeCommandBottom);
			ApplyCommand(CubeCommandLeftPrime);
			ApplyCommand(CubeCommandBottom);
			ApplyCommand(CubeCommandBottom);
		}
		else if (CC(cBottom, 1, 0) == rightColor && CC(cLeft, 1, 2) == bottomColor)
		{
			ApplyCommand(CubeCommandLeft);
			ApplyCommand(CubeCommandBottom);
			ApplyCommand(CubeCommandFront);
			ApplyCommand(CubeCommandBottomPrime);
		}
		else
		{
			// shouldn't get here
			int a = 0;
		}
		ApplyCommand(CubeRotateY);

	}

}

// Solves F2L
void CubeSolver::SolveStage2()
{
	// Stage 2 is complicated. This is how it works:
	// 1) Try to use the algorithms below to get the right color into the current bottom corner of the cube
	//		-> This might require rotations of the top face, so rotate that three times and repeat above again
	// 2) Try this for each bottom corner of the cube.
	// 3) This might not work, eg if bottom corners are permutations of each other. In this case, try putting a dummy piece into the current corner (a piece with the top color on it!). Then start again...

    StickerColor rightColor = pCube->topFaceStickers[1][1]->GetColor(); // CUBE_ANY; // pPrivateCube->rightFaceStickers[1][1]->GetColor();
	StickerColor backColor = CUBE_ANY; // pPrivateCube->backFaceStickers[1][1]->GetColor();
	StickerColor bottomColor = CUBE_ANY; // pPrivateCube->bottomFaceStickers[1][1]->GetColor();

	enum cornerStatus 
	{
		CORNER_SOLVED	 = 0,
		CORNER_TOPPIECES = 1,
		CORNER_GARBAGE   = 2,
	};

	cornerStatus cornerStatuses[4] = {CORNER_GARBAGE, CORNER_GARBAGE, CORNER_GARBAGE, CORNER_GARBAGE };

	for (int attemptsPuttingTopIntoSides = 0; attemptsPuttingTopIntoSides < 4; attemptsPuttingTopIntoSides++)
	{


		for (int i = 0; i < 4; i++)
		{
			// int restartCount = 0;
			cornerStatuses[i] = (SolveStage2Helper(true) ? CORNER_SOLVED : cornerStatuses[i]);
			ApplyCommand(CubeRotateY);
		}

		if (cornerStatuses[0] == CORNER_SOLVED && cornerStatuses[1] == CORNER_SOLVED && cornerStatuses[2] == CORNER_SOLVED && cornerStatuses[3] == CORNER_SOLVED)
		{
			// Success. That was lucky!
			return;
		}
		else if (!(cornerStatuses[0] == CORNER_GARBAGE || cornerStatuses[1] == CORNER_GARBAGE || cornerStatuses[2] == CORNER_GARBAGE || cornerStatuses[3] == CORNER_GARBAGE))
		{
			// There's no garbage ones left, so either just TOPPIECEs or fully solved.
			// We should try solving, in this case!
			// We're nearly there, just loop again and it should be solved!
			attemptsPuttingTopIntoSides -= 1;
			continue;
		}

		// At least one of the corners is garbage
		assert (cornerStatuses[0] == CORNER_GARBAGE || cornerStatuses[1] == CORNER_GARBAGE || cornerStatuses[2] == CORNER_GARBAGE || cornerStatuses[3] == CORNER_GARBAGE);

		bool bFoundGarbageCorner = false;
		int iFirstGarbageCorner = 0;
		while (!bFoundGarbageCorner)
		{
			assert (iFirstGarbageCorner < 4);
			// If the above wasn't successful, then we need to put a wrong piece (i.e. a top layer piece) into one of the sides, and then retry
			if (cornerStatuses[iFirstGarbageCorner] == CORNER_GARBAGE)
			{
				bFoundGarbageCorner = true;
			}
			else
			{
				iFirstGarbageCorner++;
			}
		}

		// rotate the cube until we're in the garbage corner
		for (int i = 0; i < iFirstGarbageCorner; i++)
		{
			ApplyCommand(CubeRotateY);
		}

		// Put some top pieces in this garbage corner
		if (!(SolveStage2Helper(false)))
		{
			return;		
		} 
		else
		{
			cornerStatuses[iFirstGarbageCorner] = CORNER_TOPPIECES;
		}

		// Flip the cube back to the way it was before
		for (int i = 0; i < 4 - iFirstGarbageCorner; i++)
		{
			ApplyCommand(CubeRotateY);
		}

		// Now retry...
	}

	// We shouldn't get here. In theory it should be solved by now...
//	assert (false);
}

// There has GOT to be a better way of doing this!
bool AND(int in1, int in2)
{
	return in1 && in2;
}

bool OR(int in1, int in2)
{
	return in1 || in2;
}

bool CubeSolver::SolveStage2Helper(bool bUseRealColors)
{
	StickerColor rightColor;
	StickerColor backColor;
	StickerColor bottomColor;

	bool (*AND_OR)(int, int);

	if (bUseRealColors)
	{
        rightColor = pCube->rightFaceStickers[1][1]->GetColor();
        backColor = pCube->backFaceStickers[1][1]->GetColor();
        bottomColor = pCube->bottomFaceStickers[1][1]->GetColor();
		AND_OR = (&AND);
	}
	else
	{
        rightColor = pCube->topFaceStickers[1][1]->GetColor();
        backColor = pCube->topFaceStickers[1][1]->GetColor();
        bottomColor = pCube->topFaceStickers[1][1]->GetColor();
		AND_OR = (&OR);
	}

	


	bool bModifiedCurrentCorner = false;
	int timesRotatedTopForCurrentCorner = 0;

// RESTARTSTAGE2:

	while (!bModifiedCurrentCorner && timesRotatedTopForCurrentCorner < 4)
	{
		if (AND_OR(AND_OR((CC(cBottom, 2, 2) & bottomColor), (CC(cRight, 0, 2) & rightColor)), (CC(cBack, 2, 2) & backColor)))
		{
			// Corner is in the correct place, is the side?
			if (AND_OR((CC(cRight, 0, 1) & rightColor) , (CC(cBack, 2, 1) & backColor)))
			{
				// The side is in the correct place too.
				bModifiedCurrentCorner = true; // just to get out of the current loop...
			}
			else if (AND_OR((CC(cRight, 0, 1) & backColor) , (CC(cBack, 2, 1) & rightColor)))
			{
				// F'UF'U2R'F2RU2F2
				char commands[] = "fUfUUrFFRUUFF";
				ParseCommandString( commands, _countof(commands) ); bModifiedCurrentCorner = true;
			}
			else if (AND_OR((CC (cBack, 1, 0) & backColor) , (CC(cTop, 1, 0) & rightColor)))
			{
				// URUR'U'F'U'F
				char commands[] = "URUrufuF";
				ParseCommandString( commands, _countof(commands) ); bModifiedCurrentCorner = true;
			}
			else if (AND_OR((CC (cTop, 0, 1) & backColor) , (CC(cRight, 1, 0) & rightColor)))
			{
				// U'F'U'FURUR'
				char commands[] = "ufuFURUr";
				ParseCommandString( commands, _countof(commands) ); bModifiedCurrentCorner = true;
			}
			else
			{
				//restartCount +=1;
				//ApplyCommand(CubeCommandTop);
				//goto RESTARTSTAGE2;
			}
		}
		else if (AND_OR(AND_OR((CC(cBottom, 2, 2) & rightColor) , (CC(cRight, 0, 2) & backColor)), (CC(cBack, 2, 2) & bottomColor)))
		{
			if (AND_OR((CC(cRight, 0, 1) & rightColor) , (CC(cBack, 2, 1) & backColor)))
			{
				// RU'R'F2L'U'LUF2  R2U2R'U'RU'R'U2R'	
				char commands[] = "RurFFluLUFF"; // "RRUUruRurUUR";
				ParseCommandString( commands, _countof(commands) );	bModifiedCurrentCorner = true;
			}
			else if (AND_OR((CC(cRight, 0, 1) & backColor) , (CC(cBack, 2, 1) & rightColor)))
			{
				// RU'RUBU'B'R2
				char commands[] = "RuRUBubRR";
				ParseCommandString( commands, _countof(commands) ); bModifiedCurrentCorner = true;
			}
			else if (AND_OR((CC (cTop, 0, 1) & rightColor) , (CC(cRight, 1, 0) & backColor)))
			{
				// U'RU'R'F'U'F
				char commands[] = "uRurfuF";
				ParseCommandString( commands, _countof(commands) ); bModifiedCurrentCorner = true;
			}
			else if (AND_OR((CC (cTop, 0, 1) & backColor) , (CC(cRight, 1, 0) & rightColor)))
			{
				// RU2B'RBR2
				char commands[] = "RUUbRBRR";
				ParseCommandString( commands, _countof(commands) ); bModifiedCurrentCorner = true;
			}
			else
			{
				//restartCount += 1;
				//ApplyCommand(CubeCommandTop);
				//goto RESTARTSTAGE2;
			}
		}
		else if (AND_OR(AND_OR((CC(cBottom, 2, 2) & backColor) , (CC(cRight, 0, 2) & bottomColor)), (CC(cBack, 2, 2) & rightColor)))
		{
			// StickerColor rightColor2 = pPrivateCube->rightFaceStickers[1][1]->GetColor();
			// StickerColor backColor2 = pPrivateCube->backFaceStickers[1][1]->GetColor();
			// StickerColor bottomColor2 = pPrivateCube->bottomFaceStickers[1][1]->GetColor();

			if (AND_OR((CC(cRight, 0, 1) & rightColor) , (CC(cBack, 2, 1) & backColor)))
			{
				// RU2RUR'URU2R2	
				char commands[] = "RUURUrURUURR";
				ParseCommandString( commands, _countof(commands) );	bModifiedCurrentCorner = true;
			}
			else if (AND_OR((CC(cRight, 0, 1) & backColor) , (CC(cBack, 2, 1) & rightColor)))
			{
				// RU'R'F'L'U2LF
				char commands[] = "RurflUULF";
				ParseCommandString( commands, _countof(commands) );	bModifiedCurrentCorner = true;
			}
			else if (AND_OR((CC (cBack, 1, 0) & backColor) , (CC(cTop, 1, 0) & rightColor)))
			{
				// F'UFU'F'UF
				char commands[] = "fUFufUF";
				ParseCommandString( commands, _countof(commands) );	bModifiedCurrentCorner = true;
			}
			else if (AND_OR((CC (cBack, 1, 0) & rightColor) , (CC(cTop, 1, 0) & backColor)))
			{
				// UF'UFRUR'
				char commands[] = "UfUFRUr";
				ParseCommandString( commands, _countof(commands) );	bModifiedCurrentCorner = true;
			}
			else
			{
				//restartCount +=1;
				//ApplyCommand(CubeCommandTop);
				//if (restartCount < 4)
				//	goto RESTARTSTAGE2;
			}
		}
		else if (AND_OR(AND_OR((CC(cTop, 0, 0) & rightColor) , (CC(cRight, 0, 0) & bottomColor)), (CC(cBack, 2, 0) & backColor)))
		{
			// 10 possibilities

			if (AND_OR((CC(cRight, 0, 1) & rightColor) , (CC(cBack, 2, 1) & backColor)))
			{
				// LD'L'ULDL'
				char commands[] = "LdlULDl";
				ParseCommandString( commands, _countof(commands) ); bModifiedCurrentCorner = true;
			}
			else if (AND_OR((CC(cRight, 0, 1) & backColor) , (CC(cBack, 2, 1) & rightColor)))
			{
				// UF'U'FU'RUR'
				char commands[] = "UfuFuRUr";
				ParseCommandString( commands, _countof(commands) ); bModifiedCurrentCorner = true;
			}
			else if (AND_OR((CC(cBack, 1, 0) & backColor) , (CC(cTop, 1, 0) & rightColor)))
			{
				// U'F'UF
				char commands[] = "ufUF";
				ParseCommandString( commands, _countof(commands) ); bModifiedCurrentCorner = true;
			}
			else if (AND_OR((CC(cBack, 1, 0) & rightColor) , (CC(cTop, 1, 0) & backColor)))
			{
				// UF'U2FU'RUR'	 
				char commands[] = "UfUUFuRUr";
				ParseCommandString( commands, _countof(commands) ); bModifiedCurrentCorner = true;
			}
			else if (AND_OR((CC(cLeft, 1, 0) & backColor) , (CC(cTop, 2, 1) & rightColor)))
			{
				// R2BUB'U'R2
				char commands[] = "RRBUbuRR";
				ParseCommandString( commands, _countof(commands) ); bModifiedCurrentCorner = true;
			}
			else if (AND_OR((CC(cLeft, 1, 0) & rightColor) , (CC(cTop, 2, 1) & backColor)))
			{
				// RU'BUB'U2R'
				char commands[] = "RuBUbUUr";
				ParseCommandString( commands, _countof(commands) ); bModifiedCurrentCorner = true;
			}
			else if (AND_OR((CC(cFront, 1, 0) & backColor) , (CC(cTop, 1, 2) & rightColor)))
			{
				// F'U'L'U2LU'F	
				char commands[] = "fulUULuF";
				ParseCommandString( commands, _countof(commands) ); bModifiedCurrentCorner = true;
			}
			else if (AND_OR((CC(cFront, 1, 0) & rightColor) , (CC(cTop, 1, 2) & backColor)))
			{
				// RUR'
				char commands[] = "RUr";
				ParseCommandString( commands, _countof(commands) ); bModifiedCurrentCorner = true;
			}
			else if (AND_OR((CC(cRight, 1, 0) & backColor) , (CC(cTop, 0, 1) & rightColor)))
			{
				// F'L'B'UBLF
				char commands[] = "flbUBLF";
				ParseCommandString( commands, _countof(commands) ); bModifiedCurrentCorner = true;
			}
			else if (AND_OR((CC(cRight, 1, 0) & rightColor) , (CC(cTop, 0, 1) & backColor)))
			{
				// U'RU'R'URUR'
				char commands[] = "uRurURUr";
				ParseCommandString( commands, _countof(commands) ); bModifiedCurrentCorner = true;
			}
		}
		else if (AND_OR(AND_OR((CC(cTop, 0, 0) & backColor) , (CC(cRight, 0, 0) & rightColor)) , (CC(cBack, 2, 0) & bottomColor)))
		{
			// 10 possibilities

			if (AND_OR((CC(cRight, 0, 1) & rightColor) , (CC(cBack, 2, 1) & backColor)))
			{
				// B'DBU'B'D'B
				char commands[] = "bDBubdB";
				ParseCommandString( commands, _countof(commands) ); bModifiedCurrentCorner = true;
			}
			else if (AND_OR((CC(cRight, 0, 1) & backColor) , (CC(cBack, 2, 1) & rightColor)))
			{
				// U'RUR'UF'U'F
				char commands[] = "uRUrUfuF";
				ParseCommandString( commands, _countof(commands) ); bModifiedCurrentCorner = true;
			}
			else if (AND_OR((CC(cBack, 1, 0) & backColor) , (CC(cTop, 1, 0) & rightColor)))
			{
				// UF'UFU'F'U'F
				char commands[] = "UfUFufuF";
				ParseCommandString( commands, _countof(commands) ); bModifiedCurrentCorner = true;
			}
			else if (AND_OR((CC(cBack, 1, 0) & rightColor) , (CC(cTop, 1, 0) & backColor)))
			{
				// F'UFU2RUR'	 
				char commands[] = "fUFUURUr";
				ParseCommandString( commands, _countof(commands) ); bModifiedCurrentCorner = true;
			}
			else if (AND_OR((CC(cLeft, 1, 0) & backColor) , (CC(cTop, 2, 1) & rightColor)))
			{
				// F'U'F
				char commands[] = "fuF";
				ParseCommandString( commands, _countof(commands) ); bModifiedCurrentCorner = true;
			}
			else if (AND_OR((CC(cLeft, 1, 0) & rightColor) , (CC(cTop, 2, 1) & backColor)))
			{
				// RUBU2B'UR'
				char commands[] = "RUBUUbUr";
				ParseCommandString( commands, _countof(commands) ); bModifiedCurrentCorner = true;
			}
			else if (AND_OR((CC(cFront, 1, 0) & backColor) , (CC(cTop, 1, 2) & rightColor)))
			{
				// R2UBU'B'R2
				char commands[] = "RRUBubRR";
				ParseCommandString( commands, _countof(commands) ); bModifiedCurrentCorner = true;
			}
			else if (AND_OR((CC(cFront, 1, 0) & rightColor) , (CC(cTop, 1, 2) & backColor)))
			{
				// F2L'U'LUF2
				char commands[] = "FFluLUFF";
				ParseCommandString( commands, _countof(commands) ); bModifiedCurrentCorner = true;
			}
			else if (AND_OR((CC(cRight, 1, 0) & backColor) , (CC(cTop, 0, 1) & rightColor)))
			{
				// F'UL'U2LU2F
				char commands[] = "fUlUULUUF";
				ParseCommandString( commands, _countof(commands) ); bModifiedCurrentCorner = true;
			}
			else if (AND_OR((CC(cRight, 1, 0) & rightColor) , (CC(cTop, 0, 1) & backColor)))
			{
				// URU'R'
				char commands[] = "URur";
				ParseCommandString( commands, _countof(commands) ); bModifiedCurrentCorner = true;
			}
		}
		else if (AND_OR(AND_OR((CC(cTop, 0, 0) & bottomColor) , (CC(cRight, 0, 0) & backColor)), (CC(cBack, 2, 0) & rightColor)))
		{
			// 10 possibilities

			if (AND_OR((CC(cRight, 0, 1) & rightColor) , (CC(cBack, 2, 1) & backColor)))
			{
				// U2F'UL'U'LU'F
				char commands[] = "UUfUluLuF";
				ParseCommandString( commands, _countof(commands) ); bModifiedCurrentCorner = true;
			}
			else if (AND_OR((CC(cRight, 0, 1) & backColor) , (CC(cBack, 2, 1) & rightColor)))
			{
				// F'UFRU2R'
				char commands[] = "fUFRUUr";
				ParseCommandString( commands, _countof(commands) ); bModifiedCurrentCorner = true;
			}
			else if (AND_OR((CC(cBack, 1, 0) & backColor) , (CC(cTop, 1, 0) & rightColor)))
			{
				// F'U2FUF'U'F	
				char commands[] = "fUUFUfuF";
				ParseCommandString( commands, _countof(commands) ); bModifiedCurrentCorner = true;
			}
			else if (AND_OR((CC(cBack, 1, 0) & rightColor) , (CC(cTop, 1, 0) & backColor)))
			{
				// RURB'R'BU2R'	 
				char commands[] = "RURbrBUUr";
				ParseCommandString( commands, _countof(commands) ); bModifiedCurrentCorner = true;
			}
			else if (AND_OR((CC(cLeft, 1, 0) & backColor) , (CC(cTop, 2, 1) & rightColor)))
			{
				// U'F'U'LF'L'F2
				char commands[] = "ufuLflFF";
				ParseCommandString( commands, _countof(commands) ); bModifiedCurrentCorner = true;
			}
			else if (AND_OR((CC(cLeft, 1, 0) & rightColor) , (CC(cTop, 2, 1) & backColor)))
			{
				// RBU2B'R'	
				char commands[] = "RBUUbr";
				ParseCommandString( commands, _countof(commands) ); bModifiedCurrentCorner = true;
			}
			else if (AND_OR((CC(cFront, 1, 0) & backColor) , (CC(cTop, 1, 2) & rightColor)))
			{
				// F'L'U2LF
				char commands[] = "flUULF";
				ParseCommandString( commands, _countof(commands) ); bModifiedCurrentCorner = true;
			}
			else if (AND_OR((CC(cFront, 1, 0) & rightColor) , (CC(cTop, 1, 2) & backColor)))
			{
				// URUB'RBR2
				char commands[] = "URUbRBRR";
				ParseCommandString( commands, _countof(commands) ); bModifiedCurrentCorner = true;
			}
			else if (AND_OR((CC(cRight, 1, 0) & backColor) , (CC(cTop, 0, 1) & rightColor)))
			{
				// F'U'F'LFL'U2F
				char commands[] = "fufLFlUUF";
				ParseCommandString( commands, _countof(commands) ); bModifiedCurrentCorner = true;
			}
			else if (AND_OR((CC(cRight, 1, 0) & rightColor) , (CC(cTop, 0, 1) & backColor)))
			{
				// RU2R'U'RUR'
				char commands[] = "RUUruRUr";
				ParseCommandString( commands, _countof(commands) ); bModifiedCurrentCorner = true;
			}
		}
		else
		{
		//		assert(false);
		}
		timesRotatedTopForCurrentCorner++;

		// Rotate the top, and try again if we need to
		char commands[] = "U";
		ParseCommandString( commands, _countof(commands) );
	} 

	return bModifiedCurrentCorner;
}

// This should make the top face all be the correct color, but not necessarily solve the adjacent sides. That might need Stage 4.
// http://www.speedcubing.com/chris/3-orientations.html
void CubeSolver::SolveStage3()
{

	bool bSolvedStage3 = false;
	UINT attempts = 0;

	while (!bSolvedStage3 && attempts < 4)
	{
		UINT cubeStage3Serial = SerializeCubeStage3Orientation();

		if (cubeStage3Serial == SerializeOrientation(0, 1, 2, 3, 4, 5, 6, 7, 8))
		{
			// It's already solved! don't do anything
			bSolvedStage3 = true;
		}
		else if (cubeStage3Serial == SerializeOrientation(4, 9, 10, 11, 13, 15, 16, 17, 19))
		{
			// R U2 R2 F R F' U2 R' F R F'
			char commands[] = "RUURRFRfUUrFRf";
			ParseCommandString( commands, _countof(commands) ); bSolvedStage3 = true;
		}
		else if (cubeStage3Serial == SerializeOrientation(4, 9, 10, 11, 13, 14, 16, 18, 19))
		{
			// F' L' U' L U F B' U' R' U R B
			char commands[] = "fluLUFburURB";
			ParseCommandString( commands, _countof(commands) ); bSolvedStage3 = true;
		}
		else if (cubeStage3Serial == SerializeOrientation(2, 4, 9, 10, 13, 15, 16, 18, 19))
		{
			// R' U2 R' F R F' U' F' U' F U' R
			char commands[] = "rUUrFRfufuFuR";
			ParseCommandString( commands, _countof(commands) ); bSolvedStage3 = true;
		}
		else if (cubeStage3Serial == SerializeOrientation(4, 8, 10, 11, 13, 16, 17, 19, 20))
		{
			// L U2 L F' L' F U F U F' U L'
			char commands[] = "LUULflFUFUfUl";
			ParseCommandString( commands, _countof(commands) ); bSolvedStage3 = true;
		}
		else if (cubeStage3Serial == SerializeOrientation(0, 4, 8, 10, 11, 13, 16, 18, 19))
		{
			// R' F R F' U' L R' D' F D R L'
			char commands[] = "rFRfuLrdFDRl";
			ParseCommandString( commands, _countof(commands) ); bSolvedStage3 = true;
		}
		else if (cubeStage3Serial == SerializeOrientation(0, 4, 6, 10, 12, 13, 14, 16, 19))
		{
			// F R U R' U F' U2 F' L F L'
			char commands[] = "FRUrUfUUfLFl";
			ParseCommandString( commands, _countof(commands) ); bSolvedStage3 = true;
		}
		else if (cubeStage3Serial == SerializeOrientation(2, 4, 8, 9, 10, 13, 16, 17, 19))
		{
			// L U2 F' L' U' L U F2 U2 F' L'
			char commands[] = "LUUfluLUFFUUfl";
			ParseCommandString( commands, _countof(commands) ); bSolvedStage3 = true;
		}
		else if (cubeStage3Serial == SerializeOrientation(0, 2, 4, 6, 8, 10, 13, 16, 19))
		{
			// M F' U' F U F U F U' F' M'
			// -> R L' U' B' U B U B U B' U' L R'
			char commands[] = "RlubUBUBUbuLr";
			ParseCommandString( commands, _countof(commands) ); bSolvedStage3 = true;
			//	assert(false);		// This one is tricky! Fix...
		}
		else if (cubeStage3Serial == SerializeOrientation(3, 4, 5, 9, 10, 11, 15, 16, 17))
		{
			// L U2 L2 U' L U' L' U2 B L B'
			char commands[] = "LUULLuLulUUBLb";
			ParseCommandString( commands, _countof(commands) ); bSolvedStage3 = true;
		}
		else if (cubeStage3Serial == SerializeOrientation(1, 4, 7, 9, 11, 13, 15, 17, 19))
		{
			// L' B' L U' R' U R U' R' U R L' B L
			char commands[] = "lbLurURurURlBL";
			ParseCommandString( commands, _countof(commands) ); bSolvedStage3 = true;
		}
		else if (cubeStage3Serial == SerializeOrientation(3, 4, 5, 10, 11, 15, 16, 18, 20))
		{
			// L U F U' F' U F U' F' L'
			char commands[] = "LUFufUFufl";
			ParseCommandString( commands, _countof(commands) ); bSolvedStage3 = true;
		}
		else if (cubeStage3Serial == SerializeOrientation(3, 4, 5, 9, 10, 11, 14, 16, 18))
		{
			// R' U' F' U F' L F L' F R
			char commands[] = "rufUfLFlFR";
			ParseCommandString( commands, _countof(commands) ); bSolvedStage3 = true;
		}
		else if (cubeStage3Serial == SerializeOrientation(3, 4, 5, 10, 12, 15, 16, 17, 20))
		{
			// L U F U' F R' F' R F' L'
			char commands[] = "LUFuFrfRfl";
			ParseCommandString( commands, _countof(commands) ); bSolvedStage3 = true;
		}
		else if (cubeStage3Serial == SerializeOrientation(3, 4, 7, 9, 10, 11, 13, 15, 17))
		{
			// R B L' B L B' L' B L B2 R'
			char commands[] = "RBlBLblBLBBr";
			ParseCommandString( commands, _countof(commands) ); bSolvedStage3 = true;
		}
		else if (cubeStage3Serial == SerializeOrientation(1, 3, 4, 9, 11, 13, 15, 16, 17))
		{
			// L' B' R B' R' B R B' R' B2 L
			char commands[] = "lbRbrBRbrBBL";
			ParseCommandString( commands, _countof(commands) ); bSolvedStage3 = true;
		}
		else if (cubeStage3Serial == SerializeOrientation(3, 4, 7, 9, 10, 11, 13, 14, 18))
		{
			// R F' U2 F U2 F R2 F' R
			char commands[] = "RfUUFUUFRRfR";
			ParseCommandString( commands, _countof(commands) ); bSolvedStage3 = true;
		}
		else if (cubeStage3Serial == SerializeOrientation(1, 3, 4, 12, 13, 15, 16, 17, 20))
		{
			// L' F U2 F' U2 F' L2 F L'
			char commands[] = "lFUUfUUfLLFl";
			ParseCommandString( commands, _countof(commands) ); bSolvedStage3 = true;
		}
		else if (cubeStage3Serial == SerializeOrientation(3, 4, 7, 10, 11, 13, 15, 18, 20))
		{
			// R B U B' U' B U B' U' R'
			char commands[] = "RBUbuBUbur";
			ParseCommandString( commands, _countof(commands) ); bSolvedStage3 = true;
		}
		else if (cubeStage3Serial == SerializeOrientation(1, 3, 4, 11, 13, 15, 16, 18, 20))
		{
			// 14 refl - L' B' U' B U B' U' B U L
			char commands[] = "lbuBUbuBUL";
			ParseCommandString( commands, _countof(commands) ); bSolvedStage3 = true;
		}
		else if (cubeStage3Serial == SerializeOrientation(1, 2, 3, 4, 13, 14, 16, 17, 20))
		{
			//   R B L' B L B2 R'
			char commands[] = "RBlBLBBr";
			ParseCommandString( commands, _countof(commands) ); bSolvedStage3 = true;
		}
		else if (cubeStage3Serial == SerializeOrientation(3, 4, 7, 8, 9, 10, 12, 13, 18))
		{
			//   L' B' R B' R' B2 L
			char commands[] = "lbRbrBBL";
			ParseCommandString( commands, _countof(commands) ); bSolvedStage3 = true;
		}
		else if (cubeStage3Serial == SerializeOrientation(0, 3, 4, 7, 10, 12, 13, 15, 18))
		{
			//   R L2 B' L B' L' B2 L B' L R'
			char commands[] = "RLLbLblBBLbLr";
			ParseCommandString( commands, _countof(commands) ); bSolvedStage3 = true;
		}
		else if (cubeStage3Serial == SerializeOrientation(1, 3, 4, 6, 11, 13, 14, 16, 20))
		{
			//   L' R2 B R' B R B2 R' B R' L
			char commands[] = "lRRBrBRBBrBrL";
			ParseCommandString( commands, _countof(commands) ); bSolvedStage3 = true;
		}
		else if (cubeStage3Serial == SerializeOrientation(1, 3, 4, 8, 9, 12, 13, 16, 18))
		{
			//  L' U' L B L' B' U B L B' 
			char commands[] = "luLBlbUBLb";
			ParseCommandString( commands, _countof(commands) ); bSolvedStage3 = true;
		}
		else if (cubeStage3Serial == SerializeOrientation(2, 3, 4, 7, 10, 13, 14, 17, 20))
		{
			//   R U R' B' R B U' B' R' B
			char commands[] = "RUrbRBubrB";
			ParseCommandString( commands, _countof(commands) ); bSolvedStage3 = true;
		}
		else if (cubeStage3Serial == SerializeOrientation(0, 1, 3, 4, 11, 13, 14, 16, 17))
		{
			//   L' B2 R B R' B L
			char commands[] = "lBBRBrBL";
			ParseCommandString( commands, _countof(commands) ); bSolvedStage3 = true;
		}
		else if (cubeStage3Serial == SerializeOrientation(3, 4, 6, 7, 9, 10, 12, 13, 15))
		{
			//   R B2 L' B' L B' R'
			char commands[] = "RBBlbLbr";
			ParseCommandString( commands, _countof(commands) ); bSolvedStage3 = true;
		}
		else if (cubeStage3Serial == SerializeOrientation(1, 2, 4, 7, 13, 14, 17, 19, 20))
		{
			//   R B' R' U' R B R' B' U B
			char commands[] = "RbruRBrbUB";
			ParseCommandString( commands, _countof(commands) ); bSolvedStage3 = true;
		}
		else if (cubeStage3Serial == SerializeOrientation(1, 4, 7, 8, 9, 12, 13, 18, 19))
		{
			//   L' B L U L' B' L B U' B'
			char commands[] = "lBLUlbLBub";
			ParseCommandString( commands, _countof(commands) ); bSolvedStage3 = true;
		}
		else if (cubeStage3Serial == SerializeOrientation(1, 2, 4, 7, 9, 13, 15, 18, 19))
		{
			//   L F L' R U R' U' L F' L'
			char commands[] = "LFlRUruLfl";
			ParseCommandString( commands, _countof(commands) ); bSolvedStage3 = true;
		}
		else if (cubeStage3Serial == SerializeOrientation(1, 4, 7, 8, 11, 13, 17, 19, 20))
		{
			//  R' F' R L' U' L U R' F R 
			char commands[] = "rfRluLUrFR";
			ParseCommandString( commands, _countof(commands) ); bSolvedStage3 = true;
		}
		else if (cubeStage3Serial == SerializeOrientation(1, 2, 3, 4, 8, 13, 16, 18, 20))
		{
			//   R L2 D' B' D B L B' L R'
			char commands[] = "RLLdbDBLbLr";
			ParseCommandString( commands, _countof(commands) ); bSolvedStage3 = true;
		}
		else if (cubeStage3Serial == SerializeOrientation(2, 3, 4, 7, 8, 10, 13, 18, 20))
		{
			//  L' R2 D B D' B' R' B R' L 
			char commands[] = "lRRDBdbrBrL";
			ParseCommandString( commands, _countof(commands) ); bSolvedStage3 = true;
		}
		else if (cubeStage3Serial == SerializeOrientation(1, 2, 3, 4, 8, 9, 13, 16, 17))
		{
			//   B U L U' F L' B' L F' L'
			char commands[] = "BULuFlbLfl";
			ParseCommandString( commands, _countof(commands) ); bSolvedStage3 = true;
		}
		else if (cubeStage3Serial == SerializeOrientation(2, 3, 4, 7, 8, 9, 10, 13, 17))
		{
			//   B' U' R' U F' R B R' F R
			char commands[] = "burUfRBrFR";
			ParseCommandString( commands, _countof(commands) ); bSolvedStage3 = true;
		}
		else if (cubeStage3Serial == SerializeOrientation(4, 5, 6, 7, 8, 10, 12, 19, 20))
		{
			//   L U F' U' L' U L F L'
			char commands[] = "LUfulULFl";
			ParseCommandString( commands, _countof(commands) ); bSolvedStage3 = true;
		}
		else if (cubeStage3Serial == SerializeOrientation(0, 1, 2, 4, 5, 14, 16, 18, 19))
		{
			//   R' U' F U R U' R' F' R
			char commands[] = "ruFURurfR";
			ParseCommandString( commands, _countof(commands) ); bSolvedStage3 = true;
		}
		else if (cubeStage3Serial == SerializeOrientation(0, 1, 3, 4, 8, 12, 13, 16, 17))
		{
			//   24 - B L' B' L U L U' L'
			char commands[] = "BlbLULul";
			ParseCommandString( commands, _countof(commands) ); bSolvedStage3 = true;
		}
		else if (cubeStage3Serial == SerializeOrientation(0, 1, 2, 4, 5, 15, 16, 17, 19))
		{
			//   25a - F' U' L' U L F
			char commands[] = "fulULF";
			ParseCommandString( commands, _countof(commands) ); bSolvedStage3 = true;
		}
		else if (cubeStage3Serial == SerializeOrientation(4, 5, 6, 7, 8, 9, 10, 11, 19))
		{
			//   25a refl - F U R U' R' F'
			char commands[] = "FURurf";
			ParseCommandString( commands, _countof(commands) ); bSolvedStage3 = true;
		}
		else if (cubeStage3Serial == SerializeOrientation(1, 2, 4, 5, 6, 9, 14, 16, 19))
		{
			//   26a -  R' U2 R2 B' R' B R' U2 R
			char commands[] = "rUURRbrBrUUR";
			ParseCommandString( commands, _countof(commands) ); bSolvedStage3 = true;
		}
		else if (cubeStage3Serial == SerializeOrientation(0, 4, 5, 7, 8, 10, 12, 17, 19))
		{
			//   26a refl - L U2 L2 B L B' L U2 L'
			char commands[] = "LUULLBLbLUUl";
			ParseCommandString( commands, _countof(commands) ); bSolvedStage3 = true;
		}
		else if (cubeStage3Serial == SerializeOrientation(1, 4, 6, 7, 8, 9, 11, 13, 19))
		{
			//   27a - F' L' U' L U F
			char commands[] = "fluLUF";
			ParseCommandString( commands, _countof(commands) ); bSolvedStage3 = true;
		}
		else if (cubeStage3Serial == SerializeOrientation(1, 4, 6, 7, 8, 12, 13, 19, 20))
		{
			//   28a - L' U' L U L F' L' F
			char commands[] = "luLULflF";
			ParseCommandString( commands, _countof(commands) ); bSolvedStage3 = true;
		}
		else if (cubeStage3Serial == SerializeOrientation(1, 2, 4, 6, 7, 13, 15, 19, 20))
		{
			//   29a - R B' R' U' R U B U' R'
			char commands[] = "RbruRUBur";
			ParseCommandString( commands, _countof(commands) ); bSolvedStage3 = true;
		}
		else if (cubeStage3Serial == SerializeOrientation(0, 1, 4, 7, 8, 11, 13, 18, 19))
		{
			//   29a-refl - L' B L U L' U' B' U L
			char commands[] = "lBLUlubUL";
			ParseCommandString( commands, _countof(commands) ); bSolvedStage3 = true;
		}
		else if (cubeStage3Serial == SerializeOrientation(1, 2, 4, 7, 8, 9, 13, 17, 19))
		{
			//   30a - L U L' U' F' L' B L F B'
			char commands[] = "LUluflBLFb";
			ParseCommandString( commands, _countof(commands) ); bSolvedStage3 = true;
		}
		else if (cubeStage3Serial == SerializeOrientation(3, 4, 5, 6, 8, 9, 10, 11, 16))
		{
			//   31a - R B U' L U L' B' R'
			char commands[] = "RBuLUlbr";
			ParseCommandString( commands, _countof(commands) ); bSolvedStage3 = true;
		}
		else if (cubeStage3Serial == SerializeOrientation(0, 1, 4, 5, 8, 11, 16, 18, 19))
		{
			//   32a - B L U L2 U' B' U B L B'
			char commands[] = "BLULLubUBLb";
			ParseCommandString( commands, _countof(commands) ); bSolvedStage3 = true;
		}
		else if (cubeStage3Serial == SerializeOrientation(2, 4, 5, 6, 7, 10, 15, 19, 20))
		{
			//   32a refl - B' R' U' R2 U B U' B' R' B
			char commands[] = "bruRRUBubrB";
			ParseCommandString( commands, _countof(commands) ); bSolvedStage3 = true;
		}
		else if (cubeStage3Serial == SerializeOrientation(1, 3, 4, 5, 7, 9, 11, 15, 17))
		{
			//   33a - L' U' L U' L' U L U' L' U2 L
			char commands[] = "luLulULulUUL";
			ParseCommandString( commands, _countof(commands) ); bSolvedStage3 = true;
		}
		else if (cubeStage3Serial == SerializeOrientation(1, 3, 4, 5, 7, 12, 15, 17, 20))
		{
			//   34a - R' U L U' R U' L' U' L U' L'
			char commands[] = "rULuRuluLul";
			ParseCommandString( commands, _countof(commands) ); bSolvedStage3 = true;
		}
		else if (cubeStage3Serial == SerializeOrientation(0, 1, 3, 4, 5, 6, 7, 12, 14))
		{
			//   35a - R2 D' L F2 L' D R U2 R
			char commands[] = "RRdLFFlDRUUR";
			ParseCommandString( commands, _countof(commands) ); bSolvedStage3 = true;
		}
		else if (cubeStage3Serial == SerializeOrientation(1, 3, 4, 5, 6, 7, 8, 12, 20))
		{
			//   36a -  R' F' L F R F' L' F
			char commands[] = "rfLFRflF";
			ParseCommandString( commands, _countof(commands) ); bSolvedStage3 = true;
		}
		else if (cubeStage3Serial == SerializeOrientation(0, 1, 3, 4, 5, 7, 8, 12, 17))
		{
			//   37a - L' B L F' L' B' L F
			char commands[] = "lBLflbLF";
			ParseCommandString( commands, _countof(commands) ); bSolvedStage3 = true;
		}
		else if (cubeStage3Serial == SerializeOrientation(1, 3, 4, 5, 6, 7, 9, 12, 15))
		{
			//   38a - L U2 L' U' L U' L'
			char commands[] = "LUUluLul";
			ParseCommandString( commands, _countof(commands) ); bSolvedStage3 = true;
		}
		else if (cubeStage3Serial == SerializeOrientation(0, 1, 3, 4, 5, 7, 11, 14, 17))
		{
			//   38a refl -  R' U2 R U R' U R
			char commands[] = "rUURUrUR";
			ParseCommandString( commands, _countof(commands) ); bSolvedStage3 = true;
		}
		else if (cubeStage3Serial == SerializeOrientation(0, 2, 4, 5, 6, 7, 8, 10, 19))
		{
			//   39a - L F' L F2 R' F R F2 L2
			char commands[] = "LfLFFrFRFFLL";
			ParseCommandString( commands, _countof(commands) ); bSolvedStage3 = true;
		}
		else if (cubeStage3Serial == SerializeOrientation(0, 1, 2, 4, 6, 7, 8, 13, 19))
		{
			//   40 - R U R' U' R' L F R F' L'
			char commands[] = "RUrurLFRfl";
			ParseCommandString( commands, _countof(commands) ); bSolvedStage3 = true;
		}
	
		if(!bSolvedStage3)
		{
			ApplyCommand(CubeRotateY); attempts +=1;
		}
	}

	assert(bSolvedStage3);

}

UINT CubeSolver::SerializeCubeStage3Orientation()
{
	UINT serializedValue = 0;
    StickerColor topColor = pCube->topFaceStickers[1][1]->GetColor();

	if (CC(cTop, 0, 0) == topColor)
		serializedValue += 1 << 0;
	if (CC(cTop, 0, 1) == topColor)
		serializedValue += 1 << 1;
	if (CC(cTop, 0, 2) == topColor)
		serializedValue += 1 << 2;
	if (CC(cTop, 1, 0) == topColor)
		serializedValue += 1 << 3;
	if (CC(cTop, 1, 1) == topColor)
		serializedValue += 1 << 4;
	if (CC(cTop, 1, 2) == topColor)
		serializedValue += 1 << 5;
	if (CC(cTop, 2, 0) == topColor)
		serializedValue += 1 << 6;
	if (CC(cTop, 2, 1) == topColor)
		serializedValue += 1 << 7;
	if (CC(cTop, 2, 2) == topColor)
		serializedValue += 1 << 8;

	if (CC(cRight, 0, 0) == topColor)
		serializedValue += 1 << 9;
	if (CC(cRight, 1, 0) == topColor)
		serializedValue += 1 << 10;
	if (CC(cRight, 2, 0) == topColor)
		serializedValue += 1 << 11;

	if (CC(cFront, 0, 0) == topColor)
		serializedValue += 1 << 12;
	if (CC(cFront, 1, 0) == topColor)
		serializedValue += 1 << 13;
	if (CC(cFront, 2, 0) == topColor)
		serializedValue += 1 << 14;

	if (CC(cLeft, 0, 0) == topColor)
		serializedValue += 1 << 15;
	if (CC(cLeft, 1, 0) == topColor)
		serializedValue += 1 << 16;
	if (CC(cLeft, 2, 0) == topColor)
		serializedValue += 1 << 17;

	if (CC(cBack, 0, 0) == topColor)
		serializedValue += 1 << 18;
	if (CC(cBack, 1, 0) == topColor)
		serializedValue += 1 << 19;
	if (CC(cBack, 2, 0) == topColor)
		serializedValue += 1 << 20;

	return serializedValue;
}

UINT CubeSolver::SerializeOrientation(UINT pos1, UINT pos2, UINT pos3, UINT pos4, UINT pos5, UINT pos6, UINT pos7, UINT pos8, UINT pos9)
{
	UINT serializedValue = 0;
	serializedValue += 1 << pos1;
	serializedValue += 1 << pos2;
	serializedValue += 1 << pos3;
	serializedValue += 1 << pos4;
	serializedValue += 1 << pos5;
	serializedValue += 1 << pos6;
	serializedValue += 1 << pos7;
	serializedValue += 1 << pos8;
	serializedValue += 1 << pos9;
	return serializedValue;
}

void CubeSolver::SolveStage4()
{
	bool bSolvedStage4 = false;
	UINT cubeYRotationsMade = 0;	// Gets reset every time we make moves, e.g. if we solve corners we might need to rotate four more times to solve edges...
    UINT topRotationsMade = 0;

    while (!bSolvedStage4 && topRotationsMade < 4)
    {
        cubeYRotationsMade = 0;

        while (!bSolvedStage4 && cubeYRotationsMade < 4)
        {
            StickerColor frontColor = pCube->frontFaceStickers[1][1]->GetColor();
            StickerColor backColor = pCube->backFaceStickers[1][1]->GetColor();
            StickerColor leftColor = pCube->leftFaceStickers[1][1]->GetColor();
            StickerColor rightColor = pCube->rightFaceStickers[1][1]->GetColor();

            bool bNoEdges = false;
            bool bNoCorners = false;

            for (int i = 0; i < 4; i++)
            {
                if (CC(cFront, 0, 0) == frontColor && CC(cFront, 1, 0) == frontColor && CC(cFront, 2, 0) == frontColor &&
                    CC(cLeft, 0, 0) == leftColor && CC(cLeft, 1, 0) == leftColor && CC(cLeft, 2, 0) == leftColor &&
                    CC(cRight, 0, 0) == rightColor && CC(cRight, 1, 0) == rightColor && CC(cRight, 2, 0) == rightColor &&
                    CC(cBack, 0, 0) == backColor && CC(cBack, 1, 0) == backColor && CC(cBack, 2, 0) == backColor)
                {
                    return;
                }
                ApplyCommand(CubeCommandTop);
            }

            // Edges Only
            if (CC(cFront, 1, 0) == backColor && CC(cBack, 1, 0) == frontColor && CC(cLeft, 1, 0) == rightColor && CC(cRight, 1, 0) == leftColor)
            {
                // H Permutation - L R U2 L' R' F' B' U2 F B
                char commands[] = "LRUUlrfbUUFB";
                ParseCommandString(commands, _countof(commands)); cubeYRotationsMade = 0; topRotationsMade = 0;
            }
            else if (CC(cFront, 1, 0) == leftColor && CC(cLeft, 1, 0) == rightColor && CC(cRight, 1, 0) == frontColor)
            {
                //	U permutation : a - R2 U' R' U' R U R U R U' R
                char commands[] = "RRuruRURURuR";
                ParseCommandString(commands, _countof(commands)); cubeYRotationsMade = 0; topRotationsMade = 0;
            }
            else if (CC(cFront, 1, 0) == rightColor && CC(cLeft, 1, 0) == frontColor && CC(cRight, 1, 0) == leftColor)
            {
                //	U permutation : b - R' U R' U' R' U' R' U R U R2
                char commands[] = "rUrururURURR";
                ParseCommandString(commands, _countof(commands)); cubeYRotationsMade = 0; topRotationsMade = 0;
            }
            else if (CC(cFront, 1, 0) == leftColor && CC(cLeft, 1, 0) == frontColor && CC(cRight, 1, 0) == backColor && CC(cBack, 1, 0) == rightColor)
            {
                //	Z permutation - R' U' R' F R F' U R F' U' L' U L F
                char commands[] = "rurFRfURfulULF";
                ParseCommandString(commands, _countof(commands)); cubeYRotationsMade = 0; topRotationsMade = 0;
            }
            else
            {
                bNoEdges = true;
            }

            // Corners Only
            if (CC(cFront, 0, 0) == rightColor && CC(cFront, 2, 0) == rightColor
                && CC(cRight, 2, 0) == backColor && CC(cRight, 0, 0) == leftColor
                && CC(cLeft, 0, 0) == frontColor)
            {
                // A permutation - a
                // R' F R' B2 R F' R' B2 R2
                char commands[] = "rFrBBRfrBBRR";
                ParseCommandString(commands, _countof(commands)); cubeYRotationsMade = 0; topRotationsMade = 0;
            }
            else if (CC(cFront, 0, 0) == backColor
                     && CC(cRight, 2, 0) == leftColor && CC(cRight, 0, 0) == frontColor
                     && CC(cLeft, 2, 0) == backColor
                     && CC(cBack, 0, 0) == rightColor && CC(cBack, 2, 0) == rightColor)
            {
                // A permutation - b
                // R B' R F2 R' B R F2 R2
                char commands[] = "RbRFFrBRFFRR";
                ParseCommandString(commands, _countof(commands)); cubeYRotationsMade = 0; topRotationsMade = 0;
            }
            else if (CC(cRight, 2, 0) == frontColor && CC(cRight, 0, 0) == backColor
                     && CC(cLeft, 0, 0) == frontColor && CC(cLeft, 2, 0) == backColor)
            {
                // E permutation
                //	R' U L' D2 L U' R L' U R' D2 R U' L
                char commands[] = "rUlDDLuRlUrDDRuL";
                ParseCommandString(commands, _countof(commands)); cubeYRotationsMade = 0; topRotationsMade = 0;
            }
            else
            {
                bNoCorners = true;
            }

            if (bNoCorners && bNoEdges)
            {
                // TODO: more efficient algorithms for Corners AND Edges
                ApplyCommand(CubeRotateY); cubeYRotationsMade += 1;
            }
        }

        StickerColor frontColor = pCube->frontFaceStickers[1][1]->GetColor();
        StickerColor backColor = pCube->backFaceStickers[1][1]->GetColor();
        StickerColor leftColor = pCube->leftFaceStickers[1][1]->GetColor();
        StickerColor rightColor = pCube->rightFaceStickers[1][1]->GetColor();

        if (! (CC(cFront, 0, 0) == frontColor && CC(cFront, 1, 0) == frontColor && CC(cFront, 2, 0) == frontColor &&
            CC(cLeft, 0, 0) == leftColor && CC(cLeft, 1, 0) == leftColor && CC(cLeft, 2, 0) == leftColor &&
            CC(cRight, 0, 0) == rightColor && CC(cRight, 1, 0) == rightColor && CC(cRight, 2, 0) == rightColor &&
            CC(cBack, 0, 0) == backColor && CC(cBack, 1, 0) == backColor && CC(cBack, 2, 0) == backColor))
        {
            ApplyCommand(CubeCommandTop); topRotationsMade += 1;
        }

    }
//	assert(bSolvedStage4);
}

StickerColor CubeSolver::CC(CubeSide side, int i, int j)
{
	switch(side)
	{
	case cRight:
		return pCube->rightFaceStickers[i][j]->GetColor();
	case cLeft:
        return pCube->leftFaceStickers[i][j]->GetColor();
	case cTop:
        return pCube->topFaceStickers[i][j]->GetColor();
	case cBottom:
        return pCube->bottomFaceStickers[i][j]->GetColor();
	case cFront:
        return pCube->frontFaceStickers[i][j]->GetColor();
	case cBack:
        return pCube->backFaceStickers[i][j]->GetColor();
	}

	assert(false);
	return CUBE_PINK;
}

void CubeSolver::ApplyCommand(CubeCommand command)
{
	m_pCubeCommandList->AppendCommand(command);
    pCube->ApplyCommand(command);
}

void CubeSolver::ParseCommandString(char *pCommandString, UINT stringLength)
{
	for (UINT i = 0; i < stringLength - 1; i++)
	{
		char c = pCommandString[i];
		switch (c)
		{
		case 'U':
			ApplyCommand(CubeCommandTop); break;
		case 'u':
			ApplyCommand(CubeCommandTopPrime); break;
		case 'B':
			ApplyCommand(CubeCommandFront); break;
		case 'b':
			ApplyCommand(CubeCommandFrontPrime); break;
		case 'D':
			ApplyCommand(CubeCommandBottom); break;
		case 'd':
			ApplyCommand(CubeCommandBottomPrime); break;
		case 'F':
			ApplyCommand(CubeCommandBack); break;
		case 'f':
			ApplyCommand(CubeCommandBackPrime); break;
		case 'R':
			ApplyCommand(CubeCommandRight); break;
		case 'r':
			ApplyCommand(CubeCommandRightPrime); break;
		case 'L':
			ApplyCommand(CubeCommandLeft); break;
		case 'l':
			ApplyCommand(CubeCommandLeftPrime); break;
		default:
			assert(false);
		}

	}
}