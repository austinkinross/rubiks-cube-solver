#include "pch.h"
#include "SDKRenderer.h"

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace Windows::Foundation;
using namespace Windows::UI::Core;

SDKRenderer::SDKRenderer()
{
}

void SDKRenderer::CreateDeviceResources()
{
	Direct3DBase::CreateDeviceResources();

	// Known to break:  //4741; // 11947; // 1074; //2311;
    UINT seed = 0;

    seed = 5745463;
    seed = 2352242;
    // seed = 4741;
    seed = 4754;

	pCube = new Cube();
	pCube->InitializeModel(m_d3dDevice.Get());
	pCube->Randomize(seed);

	pCubePlayer = new CubePlayer(pCube);


	// pCube->topFaceStickers[1][0]->SetColor(CUBE_ORANGE);
	// pCube->backFaceStickers[2][1]->SetColor(CUBE_ORANGE);
	// Broken list: , 

	// One cross Working: 1786, 1765, 1923, 2281, 1582, 1999, 1876, 1987

	// Complete cross working: 1423, 2281, 1999, 1765, 1923

	// Complete cross broken: 

	Cube *pCubeClone = new Cube();
	pCubeClone->Randomize(seed);
	// pCubeClone->topFaceStickers[1][0]->SetColor(CUBE_ORANGE);
	// pCubeClone->backFaceStickers[2][1]->SetColor(CUBE_ORANGE);

	pCubeSolver = new CubeSolver(pCubeClone);
	pCubeSolver->Solve();

	pCubeSolver->GetCubeCommandList(&pCubeCommandList);

	// Cut things like repetition out of the solution
	pCubeCommandList->Optimize();

	pCubePlayer->UseCommandList(pCubeCommandList);

	UINT l = pCubeCommandList->GetLength();

}

void SDKRenderer::CreateWindowSizeDependentResources()
{
	Direct3DBase::CreateWindowSizeDependentResources();

	float aspectRatio = m_windowBounds.Width / m_windowBounds.Height;
	float fovAngleY = 70.0f * XM_PI / 180.0f;

	XMStoreFloat4x4(&m_projectionMatrix, XMMatrixPerspectiveFovRH(fovAngleY, aspectRatio, 0.01f, 100.0f));

	XMVECTOR eye = XMVectorSet(7.7f, -7.7f, -7.5f, 0.0f);
	XMVECTOR at = XMVectorSet(0.0f, -0.1f, 0.0f, 0.0f);
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	XMStoreFloat4x4(&m_viewMatrix, XMMatrixLookAtRH(eye, at, up));
}

void SDKRenderer::Update(float timeTotal, float timeDelta)
{
	(void) timeDelta; // Unused parameter.

	if (timeTotal >= 7.2f && timeTotal <= 15.2f)
	{
		pCubePlayer->Pause();
	}
	else
	{
		pCubePlayer->Play();
	}

	pCubePlayer->Update(timeTotal, timeDelta);

	XMVECTOR eye = XMVectorSet(7.7f, -7.7f, -7.5f, 0.0f);
	XMVECTOR at = XMVectorSet(0.0f, -0.1f, 0.0f, 0.0f);
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	XMStoreFloat4x4(&m_viewMatrix, XMMatrixMultiply(XMMatrixRotationY(timeTotal / 2.0f), XMMatrixLookAtRH(eye, at, up)));
}

void SDKRenderer::Render()
{
	const float midnightBlue[] = { 0.098f, 0.098f, 0.439f, 1.000f };
	m_d3dContext->ClearRenderTargetView(
		m_renderTargetView.Get(),
		midnightBlue
		);

	m_d3dContext->ClearDepthStencilView(
		m_depthStencilView.Get(),
		D3D11_CLEAR_DEPTH,
		1.0f,
		0
		);

	m_d3dContext->OMSetRenderTargets(
		1,
		m_renderTargetView.GetAddressOf(),
		m_depthStencilView.Get()
		);

	pCube->Draw(m_d3dContext.Get(), &m_viewMatrix, &m_projectionMatrix);



	XMVECTOR eye = XMVectorSet(7.7f, +7.7f, -7.5f, 0.0f);
	XMVECTOR at = XMVectorSet(0.0f, -0.1f, 0.0f, 0.0f);
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	XMStoreFloat4x4(&m_viewMatrix, XMMatrixMultiply(XMMatrixRotationY(0 / 2.0f), XMMatrixLookAtRH(eye, at, up)));

	XMStoreFloat4x4(&m_viewMatrix, XMMatrixMultiply(XMLoadFloat4x4(&m_viewMatrix), XMMatrixTranslation(10, 0, 0)));

	pCube->Draw(m_d3dContext.Get(), &m_viewMatrix, &m_projectionMatrix);

}
