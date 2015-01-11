#pragma once

#include "CubePlayer.h"
#include "CubeSolver.h"

#include <agile.h>

#include "Renderer.h"

// This class renders a simple spinning cube.
ref class AppRenderer sealed
{
public:
	AppRenderer();

    void Initialize(Windows::UI::Core::CoreWindow^ window);

	// Method for updating time-dependent objects.
	void Update(float timeTotal, float timeDelta);
    void UpdateForWindowSizeChange();

    void Present();
    void Render();

private:
    Windows::Foundation::Rect m_windowBounds;
    Platform::Agile<Windows::UI::Core::CoreWindow> m_window;

    Renderer* mRenderer;
	CubePlayer* mCubePlayer;

	XMFLOAT4X4 m_viewMatrix;
	XMFLOAT4X4 m_projectionMatrix;
};
