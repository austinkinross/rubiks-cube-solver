#include "CubeCommandList.h"

CubeCommandList::CubeCommandList()
{
	uiCommandVectorSize = 0;
}

CubeCommand CubeCommandList::GetCommandAt(unsigned int uiPos)
{
	if (0 <= uiPos && uiPos < uiCommandVectorSize)
	{
		return commandVector[uiPos];
	}
	else
	{
		// Hopefully the code using this is smart enough to never hit here
		assert(false);
		return (CubeCommand)0;
	}
}

void CubeCommandList::AppendCommand(CubeCommand command)
{
	commandVector.push_back(command);
	uiCommandVectorSize +=1;
}

unsigned int CubeCommandList::GetLength()
{
	return uiCommandVectorSize;
}

// TODO: Improve this!
void CubeCommandList::Optimize()
{
	// Firstly get rid of any CubeRotateYs from the command list.
	RemoveCubeRotateY();

	// Then try to simplify the command list, eg removing triple-rotations of one side etc
	bool bMadeChanges = true;

	while (bMadeChanges)
	{
		bMadeChanges = false;

		for (unsigned int i = 0; i < uiCommandVectorSize - 2; i++)
		{
			if (commandVector[i] == commandVector[i+1] && commandVector[i+1] == commandVector[i+2])
			{
				commandVector[i] = OppositeCommand(commandVector[i]);
				commandVector.erase(commandVector.begin() + i + 1);
				commandVector.erase(commandVector.begin() + i + 1);
				uiCommandVectorSize -= 2;
				bMadeChanges = true;
			}
		}

		for (unsigned int i = 0; i < uiCommandVectorSize - 1; i++)
		{
			if (commandVector[i] == OppositeCommand(commandVector[i+1]))
			{
				commandVector.erase(commandVector.begin() + i);
				commandVector.erase(commandVector.begin() + i);
				uiCommandVectorSize -= 2;
				bMadeChanges = true;
			}
		}
	}
}

// These aid in solving the cube but aren't necessary during playback
// Strip them out of the command list
// There's probably a nicer way of programming this...
void CubeCommandList::RemoveCubeRotateY()
{
	vector<CubeCommand> newCommandVector;
	unsigned int newCommandVectorSize = 0;

	// How many times did the original command list perform CubeRotateY, before the current move
	unsigned int cubeRotateYCount = 0;

	for (unsigned int i = 0; i < uiCommandVectorSize; i++)
	{
		if (commandVector[i] == CubeRotateY)
		{
			cubeRotateYCount += 1;
		}
		else
		{
			if (cubeRotateYCount % 4 == 0 || commandVector[i] == CubeCommandTop || commandVector[i] == CubeCommandTopPrime
							|| commandVector[i] == CubeCommandBottom	|| commandVector[i] == CubeCommandBottomPrime )
			{
				newCommandVector.push_back(commandVector[i]);
				newCommandVectorSize +=1;
			}
			else if (cubeRotateYCount % 4 == 1)
			{
				CubeCommand newCommand;
				switch(commandVector[i])
				{
				case CubeCommandLeft:
						newCommand = CubeCommandBack; break;
				case CubeCommandLeftPrime:
						newCommand = CubeCommandBackPrime; break;
				case CubeCommandFront:
						newCommand = CubeCommandLeft; break;
				case CubeCommandFrontPrime:
						newCommand = CubeCommandLeftPrime; break;
				case CubeCommandRight:
						newCommand = CubeCommandFront; break;
				case CubeCommandRightPrime:
						newCommand = CubeCommandFrontPrime; break;
				case CubeCommandBack:
						newCommand = CubeCommandRight; break;
				case CubeCommandBackPrime:
						newCommand = CubeCommandRightPrime; break;
				}
				newCommandVector.push_back(newCommand);
				newCommandVectorSize +=1;
			}
			else if (cubeRotateYCount % 4 == 2)
			{
				CubeCommand newCommand;
				switch(commandVector[i])
				{
				case CubeCommandLeft:
						newCommand = CubeCommandRight; break;
				case CubeCommandLeftPrime:
						newCommand = CubeCommandRightPrime; break;
				case CubeCommandFront:
						newCommand = CubeCommandBack; break;
				case CubeCommandFrontPrime:
						newCommand = CubeCommandBackPrime; break;
				case CubeCommandRight:
						newCommand = CubeCommandLeft; break;
				case CubeCommandRightPrime:
						newCommand = CubeCommandLeftPrime; break;
				case CubeCommandBack:
						newCommand = CubeCommandFront; break;
				case CubeCommandBackPrime:
						newCommand = CubeCommandFrontPrime; break;
				}
				newCommandVector.push_back(newCommand);
				newCommandVectorSize +=1;
			}
			else if (cubeRotateYCount % 4 == 3)
			{
				CubeCommand newCommand;
				switch(commandVector[i])
				{
				case CubeCommandLeft:
						newCommand = CubeCommandFront; break;
				case CubeCommandLeftPrime:
						newCommand = CubeCommandFrontPrime; break;
				case CubeCommandFront:
						newCommand = CubeCommandRight; break;
				case CubeCommandFrontPrime:
						newCommand = CubeCommandRightPrime; break;
				case CubeCommandRight:
						newCommand = CubeCommandBack; break;
				case CubeCommandRightPrime:
						newCommand = CubeCommandBackPrime; break;
				case CubeCommandBack:
						newCommand = CubeCommandLeft; break;
				case CubeCommandBackPrime:
						newCommand = CubeCommandLeftPrime; break;
				}
				newCommandVector.push_back(newCommand);
				newCommandVectorSize +=1;
			}
		}
	}

	// Save the new command vector
	commandVector = newCommandVector;
	uiCommandVectorSize = newCommandVectorSize;
}