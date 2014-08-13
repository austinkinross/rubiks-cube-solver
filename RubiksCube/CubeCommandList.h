#pragma once

#include "pch.h"
#include <vector>

#include "CubeHelperFunctions.h"

using namespace std;

class CubeCommandList
{
public:
	CubeCommandList();
	CubeCommand GetCommandAt(UINT pos);
	void AppendCommand(CubeCommand command);
	UINT GetLength();
	void Optimize();
	void RemoveCubeRotateY();

private:
	vector<CubeCommand> commandVector;
	UINT uiCommandVectorSize;
};