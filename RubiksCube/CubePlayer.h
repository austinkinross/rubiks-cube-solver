#pragma once

#include "pch.h"
#include "Cube.h"
#include "CubeCommandList.h"

const float fTimePerMove = 0.3f;

class CubePlayer
{
public:
	CubePlayer();
    ~CubePlayer();

	void Update(float timeTotal, float timeDelta);
	void UseCommandList(CubeCommandList *pCommandList);

	void Play();
	void Pause();

    Cube* GetCube() { return pCube; };

private:
	void Reset();

    Cube *pCube;
	CubeCommandList *pCommandList;

	UINT uiCurrentCommandPos;
	float fCurrentCommandProportion;
	bool bPaused;

};
