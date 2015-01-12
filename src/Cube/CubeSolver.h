#pragma once

#include "pch.h"
#include "Cube.h"
#include "CubeCommandList.h"


enum CubeSide
{
	cRight,
	cLeft,
	cTop,
	cBottom,
	cFront,
	cBack
};

class CubeSolver
{
public:
	CubeSolver();	
    ~CubeSolver();

	void Solve();

	void GetCubeCommandList(CubeCommandList **ppCommandList);

    Cube* GetCube() { return mCube; };

	// Public for unit tests to access
	void SolveStage1();
	void SolveStage2();
	void SolveStage3();
	void SolveStage4();

private:

    bool SolveStage2Helper(bool bUseRealColors);
    unsigned int SerializeCubeStage3Orientation();
    unsigned int SerializeOrientation(unsigned int pos1, unsigned int pos2, unsigned int pos3, unsigned int pos4, unsigned int pos5, unsigned int pos6, unsigned int pos7, unsigned int pos8, unsigned int pos9);

	StickerColor CC(CubeSide side, int i, int j);
	void CubeSolver::ApplyCommand(CubeCommand command);
	void ParseCommandString(char *pCommandString, unsigned int stringLength);

    Cube* mCube;
	CubeCommandList* mCubeCommandList;
};
