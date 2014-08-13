#pragma once

#include "pch.h"
#include "Direct3DBase.h"
#include "CubeHelperFunctions.h"

using namespace DirectX;

struct SideCubeConstantBuffer
{
	DirectX::XMFLOAT4X4 model;
	DirectX::XMFLOAT4X4 view;
	DirectX::XMFLOAT4X4 projection;
	DirectX::XMFLOAT4 color;
};

struct VertexPositionColor
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 color;
};

class Sticker 
{

public:
	Sticker();
	Sticker(StickerColor color);
	void Sticker::InitializeModel(ID3D11Device1 *pDevice, XMFLOAT4X4 *pCubeWorld, XMFLOAT4X4 sideRotation, int pos1, int pos2);

	void Draw(ID3D11DeviceContext *pContext, XMFLOAT4X4 *pViewMatrix, XMFLOAT4X4 *pProjectionMatrix);
	void Sticker::AttachRotationMatrix(XMFLOAT4X4 *pRotationMatrix);
	void Sticker::SetColor(StickerColor color);
	EXPORTFORTEST StickerColor Sticker::GetColor();

	bool m_loadingComplete;

	

private:

	void Sticker::ConfigureShaderMatrices(XMFLOAT4X4 *pViewMatrix, XMFLOAT4X4 *pProjectionMatrix);


	StickerColor color;

	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_constantBuffer;

	uint32 m_indexCount;
	SideCubeConstantBuffer m_constantBufferData;

	XMFLOAT4X4* pCubeProjection;
	XMFLOAT4X4* pCubeView;
	XMFLOAT4X4* pCubeWorld;

	XMFLOAT4X4* pRotation1;
	XMFLOAT4X4* pRotation2;
	XMFLOAT4X4* pRotation3;

	XMFLOAT4X4 worldMatrix;
	
};