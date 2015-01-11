#include "pch.h"
#include "AgnosticApplication.h"
#include "RendererD3D.h"
#include "RendererGL.h"

using namespace DirectX;

AgnosticApplication::AgnosticApplication()
{

}

void AgnosticApplication::Initialize(float windowWidth, float windowHeight)
{
    // Set up the projection matrices for the scene
    float aspectRatio = (float)windowWidth / (float)windowHeight;
    float fovAngleY = 70.0f * XM_PI / 180.0f;
    XMStoreFloat4x4(&m_projectionMatrix, XMMatrixPerspectiveFovRH(fovAngleY, aspectRatio, 0.01f, 100.0f));

    UINT seed = 4741;

    // One renderer can be shared across all cubes. This uses a D3D11 backend to render the cubes
    mRenderer = new RendererGL();

    // Get the list of moves required to solve the cube
    CubeSolver* solver = new CubeSolver();
    CubeCommandList* commandList = NULL;
    solver->GetCube()->Randomize(seed);
    solver->Solve();
    solver->GetCubeCommandList(&commandList);

    // Cut things like repetition out of the solution
    commandList->Optimize();

    // Initialize the CubePlayer, which will play back the moves to solve the cube
    mCubePlayer = new CubePlayer();
    mCubePlayer->GetCube()->Randomize(seed);

    mCubePlayer->UseCommandList(commandList);
}

void AgnosticApplication::Update(float timeTotal, float timeDelta)
{
	(void) timeDelta; // Unused parameter.

	if (timeTotal >= 7.2f && timeTotal <= 15.2f)
	{
        mCubePlayer->Pause();
	}
	else
	{
        mCubePlayer->Play();
	}

    mCubePlayer->Update(timeTotal, timeDelta);

	XMVECTOR eye = XMVectorSet(7.7f, -7.7f, -7.5f, 0.0f);
	XMVECTOR at = XMVectorSet(0.0f, -0.1f, 0.0f, 0.0f);
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	XMStoreFloat4x4(&m_viewMatrix, XMMatrixMultiply(XMMatrixRotationY(timeTotal / 2.0f), XMMatrixLookAtRH(eye, at, up)));
}

void AgnosticApplication::Render()
{
    mRenderer->Clear();

    mCubePlayer->GetCube()->Draw(mRenderer, &m_viewMatrix, &m_projectionMatrix);

	XMVECTOR eye = XMVectorSet(7.7f, +7.7f, -7.5f, 0.0f);
	XMVECTOR at = XMVectorSet(0.0f, -0.1f, 0.0f, 0.0f);
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	XMStoreFloat4x4(&m_viewMatrix, XMMatrixMultiply(XMMatrixRotationY(0 / 2.0f), XMMatrixLookAtRH(eye, at, up)));

	XMStoreFloat4x4(&m_viewMatrix, XMMatrixMultiply(XMLoadFloat4x4(&m_viewMatrix), XMMatrixTranslation(10, 0, 0)));

    mCubePlayer->GetCube()->Draw(mRenderer, &m_viewMatrix, &m_projectionMatrix);
}

// This method is called in the event handler for the SizeChanged event.
void AgnosticApplication::UpdateForWindowSizeChange()
{
    mRenderer->UpdateForWindowSizeChange();
}

void AgnosticApplication::Present()
{
    mRenderer->Swap();
}