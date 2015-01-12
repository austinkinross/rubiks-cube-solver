#pragma once

#include <vector>

#include "CubeHelperFunctions.h"

using namespace std;

class CubeCommandList
{
public:
	CubeCommandList();
	CubeCommand GetCommandAt(unsigned int pos);
	void AppendCommand(CubeCommand command);
	unsigned int GetLength();
	void Optimize();
	void RemoveCubeRotateY();

private:
	vector<CubeCommand> commandVector;
	unsigned int uiCommandVectorSize;
};