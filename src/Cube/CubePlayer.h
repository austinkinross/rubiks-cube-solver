#pragma once

#include "pch.h"
#include "Cube.h"
#include "CubeCommandList.h"

struct CubePlayerSettings
{
    CubePlayerSettings()
    {
        UnfoldCubeAtStart = false;
        SolvingSpeed = 0.3f;
        FoldingSpeed = 0.005f;
    }

    bool UnfoldCubeAtStart;

    float FoldingSpeed;
    float SolvingSpeed;
};


class CubePlayer
{
public:
    CubePlayer(CubePlayerSettings* settings);
    ~CubePlayer();

	void Update(float timeTotal, float timeDelta);
	void UseCommandList(CubeCommandList *pCommandList);

	void Play();
	void Pause();

    Cube* GetCube() { return mCube; };

private:

    enum PlaybackState
    {
        PLAYBACK_STATE_FOLDING = 1,
        PLAYBACK_STATE_SOLVING = 2
    };

	void Reset();

    Cube* mCube;
	CubeCommandList* mCubeCommandList;

    PlaybackState mPlaybackState;

    // Folding state members
    float mFoldingAngle;

    // Solving state members
	unsigned int uiCurrentCommandPos;
	float fCurrentCommandProportion;
	bool bPaused;

    CubePlayerSettings mSettings;
};
