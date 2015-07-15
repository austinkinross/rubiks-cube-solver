#pragma once

#include "Cube/CubePlayer.h"
#include "Cube/CubeSolver.h"
#include "Renderer/Renderer.h"

class CubeRecognizer;

struct WindowWrapper
{
	WindowWrapper()
	{
		coreWindow = nullptr;
		swapChainPanel = nullptr;
		cameraPreviewPane = nullptr;
	};

	// Windows Store CoreWindow
	void *coreWindow;

	// Windows Store XAML
	void *swapChainPanel;
	void *cameraPreviewPane;
	void *cameraErrorText;

	// Android?
};

class AgnosticApp
{
public:
    AgnosticApp();

    void Initialize(WindowWrapper *windowWrapper, float windowWidth, float windowHeight);

	void Update(float timeTotal, float timeDelta);
    void UpdateForWindowSizeChange();

    void Present();
    void Render();

	void MakeCurrent();

private:
    Renderer* mRenderer;
	CubePlayer* mCubePlayer;
	CubeRecognizer* mCubeRecognizer;

	glm::mat4 m_viewMatrix;
	glm::mat4 m_projectionMatrix;
};
