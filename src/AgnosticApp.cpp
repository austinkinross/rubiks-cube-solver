#include "AgnosticApp.h"
#include "Renderer\D3D\RendererD3D.h"
#include "Renderer\GL\RendererGL.h"
#include "..\projects\Win10\RubiksCubeRecognitionLib\RubiksCubeRecognitionLib.h"

using namespace DirectX;

AgnosticApp::AgnosticApp()
{
	mRenderer = nullptr;
	mCubePlayer = nullptr;
	mCubeRecognizer = nullptr;
}

void AgnosticApp::Initialize(void *window, float windowWidth, float windowHeight)
{
    // Set up the projection matrices for the scene
    float fovAngleY = 70.0f * XM_PI / 180.0f;
    m_projectionMatrix = glm::perspectiveFov(fovAngleY, windowWidth, windowHeight, 0.01f, 100.0f);

    UINT seed = 4741;

	mCubeRecognizer = new CubeRecognizer();

    // One renderer can be shared across all cubes. This uses a D3D11 backend to render the cubes
    mRenderer = new RendererGL(window);

    // Get the list of moves required to solve the cube
    CubeSolver* solver = new CubeSolver();
    CubeCommandList* commandList = NULL;
    solver->GetCube()->Randomize(seed);
    solver->Solve();
    solver->GetCubeCommandList(&commandList);

    // Cut things like repetition out of the solution
    commandList->Optimize();

    // Configure the CubePlayer
    CubePlayerDesc cubePlayerDesc;
    cubePlayerDesc.unfoldCubeAtStart = true;

    // Initialize the CubePlayer, which will play back the moves to solve the cube
    mCubePlayer = new CubePlayer(&cubePlayerDesc);
    mCubePlayer->GetCube()->Randomize(seed);

    mCubePlayer->UseCommandList(commandList);
}

bool tripledSpeed = false;

void AgnosticApp::Update(float timeTotal, float timeDelta)
{
	(void) timeDelta; // Unused parameter.

    if (timeTotal <= 7.2f)
    {
        mCubePlayer->Play();
    }
    else if(timeTotal <= 10.2f)
	{
        mCubePlayer->Pause();
	}
	else
	{
        if (!tripledSpeed)
        {
            // Triple speed
            mCubePlayer->UpdateSolvingSpeed(mCubePlayer->GetDesc().speeds.solvingSpeed / 3);
            tripledSpeed = true;
        }
        mCubePlayer->Play();
	}

    mCubePlayer->Update(timeTotal, timeDelta);

    glm::vec3 eye = glm::vec3(7.7f, -7.7f, -7.5f);
    glm::vec3 at = glm::vec3(0.0f, -0.1f, 0.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    m_viewMatrix = glm::lookAt(eye, at, up) * glm::rotate(glm::mat4(), timeTotal / 2.0f, glm::vec3(0, 1, 0));
}

void AgnosticApp::Render()
{
    mRenderer->Clear();

	mCubeRecognizer->ColorCubeFace(mCubePlayer->GetCube());

    mRenderer->DrawCube(mCubePlayer->GetCube(), &m_viewMatrix, &m_projectionMatrix);

 //   glm::vec3 eye = glm::vec3(7.7f, -7.7f, -7.5f);
 //   glm::vec3 at = glm::vec3(0.0f, -0.1f, 0.0f);
 //   glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

 //   m_viewMatrix = glm::lookAt(eye, at, up) * glm::rotate(glm::mat4(), 0 / 2.0f, glm::vec3(0, 1, 0));
 //   m_viewMatrix *= glm::translate(glm::mat4(), glm::vec3(-5, 0, -5));

	//// XMStoreFloat4x4(&m_viewMatrix, XMMatrixMultiply(XMLoadFloat4x4(&m_viewMatrix), XMMatrixTranslation(10, 0, 0)));

 //   mRenderer->DrawCube(mCubePlayer->GetCube(), &m_viewMatrix, &m_projectionMatrix);
}

// This method is called in the event handler for the SizeChanged event.
void AgnosticApp::UpdateForWindowSizeChange()
{
    mRenderer->UpdateForWindowSizeChange();
}

void AgnosticApp::Present()
{
    mRenderer->Swap();
}