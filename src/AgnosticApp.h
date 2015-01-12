#pragma once

#include "Cube/CubePlayer.h"
#include "Cube/CubeSolver.h"
#include "Renderer/Renderer.h"

ref class AgnosticApp sealed
{
public:
    AgnosticApp();

    void Initialize(float windowWidth, float windowHeight);

	void Update(float timeTotal, float timeDelta);
    void UpdateForWindowSizeChange();

    void Present();
    void Render();

private:
    Renderer* mRenderer;
	CubePlayer* mCubePlayer;

	glm::mat4 m_viewMatrix;
	glm::mat4 m_projectionMatrix;
};
