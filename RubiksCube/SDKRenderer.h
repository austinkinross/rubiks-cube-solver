#pragma once

#include "Direct3DBase.h"
#include "CubePlayer.h"
#include "CubeSolver.h"

#include <agile.h>

// This class renders a simple spinning cube.
ref class SDKRenderer sealed : public Direct3DBase
{
public:
	SDKRenderer();

	// Direct3DBase methods.
	virtual void CreateDeviceResources() override;
	virtual void CreateWindowSizeDependentResources() override;
	virtual void Render() override;
	
	// Method for updating time-dependent objects.
	void Update(float timeTotal, float timeDelta);

private:

	Cube* pCube;
	CubePlayer* pCubePlayer;
	CubeCommandList* pCubeCommandList;
	CubeSolver *pCubeSolver;

	XMFLOAT4X4 m_viewMatrix;
	XMFLOAT4X4 m_projectionMatrix;
};
