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

    Cube* GetCube() { return pCube; };

	// Public for unit tests to access
	void PrepareForSolve();

	void SolveStage1();

	void SolveStage2();
	bool SolveStage2Helper(bool bUseRealColors);
	
	void SolveStage3();
	UINT SerializeCubeStage3Orientation();
	UINT SerializeOrientation(UINT pos1, UINT pos2, UINT pos3, UINT pos4, UINT pos5, UINT pos6, UINT pos7, UINT pos8, UINT pos9);

	void SolveStage4();

private:

	StickerColor CC(CubeSide side, int i, int j);
	void CubeSolver::ApplyCommand(CubeCommand command);
	void ParseCommandString(char *pCommandString, UINT stringLength);
	void CubeSolver::AddRandomMoves(); //temporary

    Cube *pCube;
	CubeCommandList *m_pCubeCommandList;
};
