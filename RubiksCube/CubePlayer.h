#pragma once

#include "pch.h"
#include "Cube.h"
#include "CubeCommandList.h"

const float fTimePerMove = 0.3f;

class CubePlayer
{
public:
	CubePlayer(Cube *pCube);
	void Update(float timeTotal, float timeDelta);
	void UseCommandList(CubeCommandList *pCommandList);

	void Play();
	void Pause();

private:
	void Reset();

	Cube *pPlaybackCube;
	CubeCommandList *pCommandList;

	UINT uiCurrentCommandPos;
	float fCurrentCommandProportion;
	bool bPaused;

};
