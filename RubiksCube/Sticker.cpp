#include "pch.h"
#include "Sticker.h"

//-------------------------------------------------------------------------------------------------------------------------------------------------------
// A "Sticker" represents a single face of a single cublet.
// It has several matrices associated with it:
//		1) Its own world matrix that determines where it is positioned in relation to the rest of the cube.
//		2) The rotation matrices of the various slices that a sticker may be in. (Note: only one of these should be non-identity at a time)
//		3) Its parent cube's world matrix
// The sticker's final world matrix is the product of these matrices.
//-------------------------------------------------------------------------------------------------------------------------------------------------------

Sticker::Sticker(StickerColor color)
{
	this->color = color;
	pRotation1 = NULL;
	pRotation2 = NULL;
	pRotation3 = NULL;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------
// This sets up the sticker's constant buffer before it is drawn
//-------------------------------------------------------------------------------------------------------------------------------------------------------
void Sticker::ConfigureShaderMatrices(XMFLOAT4X4 *pViewMatrix, XMFLOAT4X4 *pProjectionMatrix)
{
	XMFLOAT4X4 cumulativeWorld = worldMatrix;

	// cumulativeWorld = worldMatrix, then rotation1, then rotation2, then rotation3, then cubeWorld

	if (pRotation1 != NULL)
	{
		XMStoreFloat4x4(&cumulativeWorld, XMMatrixMultiply(XMLoadFloat4x4(&cumulativeWorld), XMLoadFloat4x4(pRotation1)));
	}

	if (pRotation2 != NULL)
	{
		XMStoreFloat4x4(&cumulativeWorld, XMMatrixMultiply(XMLoadFloat4x4(&cumulativeWorld), XMLoadFloat4x4(pRotation2)));
	}

	if (pRotation3 != NULL)
	{
		XMStoreFloat4x4(&cumulativeWorld, XMMatrixMultiply(XMLoadFloat4x4(&cumulativeWorld), XMLoadFloat4x4(pRotation3)));
	}

	XMStoreFloat4x4(&cumulativeWorld, XMMatrixMultiply(XMLoadFloat4x4(&cumulativeWorld), XMLoadFloat4x4(pCubeWorld)));

	// Set final values. Note, the matrices must be transposed for the shaders!
	XMStoreFloat4x4(&m_constantBufferData.model, XMMatrixTranspose((XMLoadFloat4x4(&cumulativeWorld))));
	XMStoreFloat4x4(&m_constantBufferData.view, XMMatrixTranspose(XMLoadFloat4x4(pViewMatrix)));
	XMStoreFloat4x4(&m_constantBufferData.projection, XMMatrixTranspose(XMLoadFloat4x4(pProjectionMatrix)));

}

void Sticker::AttachRotationMatrix(XMFLOAT4X4 *pRotationMatrix)
{
	if (pRotation1 == NULL)
	{
		pRotation1 = pRotationMatrix;
		return;
	}
	if (pRotation2 == NULL)
	{
		pRotation2 = pRotationMatrix;
		return;
	}
	if (pRotation3 == NULL)
	{
		pRotation3 = pRotationMatrix;
		return;
	}
	assert(0);
}

void Sticker::SetColor(StickerColor color)
{
	m_constantBufferData.color = ColorToXMFLOAT4(color);
	this->color = color;
}

StickerColor Sticker::GetColor()
{
	return this->color;
}

void Sticker::InitializeModel(ID3D11Device1 *pDevice, XMFLOAT4X4 *pCubeWorld, XMFLOAT4X4 sideRotation, int pos1, int pos2)
{
	XMStoreFloat4x4(&worldMatrix,  XMMatrixMultiply(XMMatrixTranslation(2.0f * pos1 - 2.0f, 0.0f, 2.0f * pos2 - 2.0f), XMLoadFloat4x4(&sideRotation)));

	this->pCubeWorld = pCubeWorld;

	m_constantBufferData.color = ColorToXMFLOAT4(color);


	auto loadVSTask = DX::ReadDataAsync("SimpleVertexShader.cso");
	auto loadPSTask = DX::ReadDataAsync("SimplePixelShader.cso");

	auto createVSTask = loadVSTask.then([pDevice, this](Platform::Array<byte>^ fileData) {
		DX::ThrowIfFailed(
			pDevice->CreateVertexShader(
 				fileData->Data,
				fileData->Length,
				nullptr,
				&m_vertexShader
				)
			);

		const D3D11_INPUT_ELEMENT_DESC vertexDesc[] = 
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		DX::ThrowIfFailed(
			pDevice->CreateInputLayout(
				vertexDesc,
				ARRAYSIZE(vertexDesc),
				fileData->Data,
				fileData->Length,
				&m_inputLayout
				)
			);
	});


	auto createPSTask = loadPSTask.then([pDevice, this](Platform::Array<byte>^ fileData) {
		DX::ThrowIfFailed(
			pDevice->CreatePixelShader(
				fileData->Data,
				fileData->Length,
				nullptr,
				&m_pixelShader
				)
			);

		CD3D11_BUFFER_DESC constantBufferDesc(sizeof(SideCubeConstantBuffer), D3D11_BIND_CONSTANT_BUFFER);
		DX::ThrowIfFailed(
			pDevice->CreateBuffer(
				&constantBufferDesc,
				nullptr,
				&m_constantBuffer
				)
			);
	});

	auto createCubeTask = (createPSTask && createVSTask).then([pDevice, this] () {
		VertexPositionColor cubeVertices[] = 
		{
			{XMFLOAT3(-0.95f, 3.0f, -0.95f), XMFLOAT3(0,0,0)},
			{XMFLOAT3(-0.95f, 3.0f, +0.95f), XMFLOAT3(0,0,0)},
			{XMFLOAT3(+0.95f, 3.0f, -0.95f), XMFLOAT3(0,0,0)},
			{XMFLOAT3(+0.95f, 3.0f, +0.95f), XMFLOAT3(0,0,0)},

			// Cheap way to draw some black back faces
	/*		{XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(0,0,0)},
			{XMFLOAT3(-1.0f, 1.0f, +1.0f), XMFLOAT3(0,0,0)},
			{XMFLOAT3(+1.0f, 1.0f, -1.0f), XMFLOAT3(0,0,0)},
			{XMFLOAT3(+1.0f, 1.0f, +1.0f), XMFLOAT3(0,0,0)},
			{AddXM3(AddXM3(cubeCenter, MultXM3(dir1, (float)pos1 - 1.0f)), MultXM3(dir2, (float)pos2 - 1.0f)), color},
			{AddXM3(AddXM3(cubeCenter, MultXM3(dir1, (float)pos1 - 1.0f)), MultXM3(dir2, (float)pos2 + 1.0f)), color},
			{AddXM3(AddXM3(cubeCenter, MultXM3(dir1, (float)pos1 + 1.0f)), MultXM3(dir2, (float)pos2 - 1.0f)), color},
			{AddXM3(AddXM3(cubeCenter, MultXM3(dir1, (float)pos1 + 1.0f)), MultXM3(dir2, (float)pos2 + 1.0f)), color}, */

		};

		D3D11_SUBRESOURCE_DATA vertexBufferData = {0};
		vertexBufferData.pSysMem = cubeVertices;
		vertexBufferData.SysMemPitch = 0;
		vertexBufferData.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC vertexBufferDesc(sizeof(cubeVertices), D3D11_BIND_VERTEX_BUFFER);
		DX::ThrowIfFailed(
			pDevice->CreateBuffer(
				&vertexBufferDesc,
				&vertexBufferData,
				&m_vertexBuffer
				)
			);

		unsigned short cubeIndices[] = 
		{
			// Front colored side
			0,2,1, 
			1,2,3,

			// Black back side
		//	4,5,6,
		//	5,7,6,
		};

		m_indexCount = ARRAYSIZE(cubeIndices);

		D3D11_SUBRESOURCE_DATA indexBufferData = {0};
		indexBufferData.pSysMem = cubeIndices;
		indexBufferData.SysMemPitch = 0;
		indexBufferData.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC indexBufferDesc(sizeof(cubeIndices), D3D11_BIND_INDEX_BUFFER);
		DX::ThrowIfFailed(
			pDevice->CreateBuffer(
				&indexBufferDesc,
				&indexBufferData,
				&m_indexBuffer
				)
			);
	});

	createCubeTask.then([this] () {
		m_loadingComplete = true;
	});

}

void Sticker::Draw(ID3D11DeviceContext *pContext, XMFLOAT4X4 *pViewMatrix, XMFLOAT4X4 *pProjectionMatrix)
{
	// Only draw the cube once it is loaded (loading is asynchronous).
	if (!m_loadingComplete)
	{
		return;
	}

	ConfigureShaderMatrices(pViewMatrix, pProjectionMatrix);

	pContext->UpdateSubresource(
		m_constantBuffer.Get(),
		0,
		NULL,
		&m_constantBufferData,
		0,
		0
		);

	UINT stride = sizeof(VertexPositionColor);
	UINT offset = 0;
	pContext->IASetVertexBuffers(
		0,
		1,
		m_vertexBuffer.GetAddressOf(),
		&stride,
		&offset
		);

	pContext->IASetIndexBuffer(
		m_indexBuffer.Get(),
		DXGI_FORMAT_R16_UINT,
		0
		);

	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	pContext->IASetInputLayout(m_inputLayout.Get());

	pContext->VSSetShader(
		m_vertexShader.Get(),
		nullptr,
		0
		);

	pContext->VSSetConstantBuffers(
		0,
		1,
		m_constantBuffer.GetAddressOf()
		); 

	pContext->PSSetShader(
		m_pixelShader.Get(),
		nullptr,
		0
		);

	pContext->DrawIndexed(
		m_indexCount,
		0,
		0
		);
}
