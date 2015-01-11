#pragma once

#include "CubePlayer.h"
#include "CubeSolver.h"
#include "Renderer.h"

ref class AgnosticApplication sealed
{
public:
    AgnosticApplication();

    void Initialize(float windowWidth, float windowHeight);

	void Update(float timeTotal, float timeDelta);
    void UpdateForWindowSizeChange();

    void Present();
    void Render();

private:
    Renderer* mRenderer;
	CubePlayer* mCubePlayer;

	XMFLOAT4X4 m_viewMatrix;
	XMFLOAT4X4 m_projectionMatrix;
};
