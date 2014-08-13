#pragma once

enum StickerColor
{
	CUBE_RED		= 1 << 0,
	CUBE_WHITE		= 1 << 1,
	CUBE_ORANGE		= 1 << 2,
	CUBE_BLUE		= 1 << 3,
	CUBE_GREEN		= 1 << 4,
	CUBE_YELLOW		= 1 << 5,
	CUBE_BLACK		= 1 << 6,
	CUBE_PINK		= 1 << 7,
	CUBE_ANY		= (1 << 8) - 1
};

enum TwistType
{
	TwistTypeIdentity,
	TwistType90,
	TwistType180,
	TwistType270,
};

enum CubeCommand
{
	CubeCommandLeft,
	CubeCommandLeftPrime,
	CubeCommandRight,
	CubeCommandRightPrime,
	CubeCommandTop,
	CubeCommandTopPrime,
	CubeCommandBottom,
	CubeCommandBottomPrime,
	CubeCommandFront,
	CubeCommandFrontPrime,
	CubeCommandBack,
	CubeCommandBackPrime,
	CubeRotateY,
};

CubeCommand OppositeCommand(CubeCommand command);

DirectX::XMFLOAT4 ColorToXMFLOAT4(StickerColor c);

DirectX::XMFLOAT4X4 XMMATRIXToXMFLOAT4X4(DirectX::XMMATRIX *pInput);

DirectX::XMFLOAT3 AddXM3(DirectX::XMFLOAT3 input1, DirectX::XMFLOAT3 input2);

DirectX::XMFLOAT3 MultXM3(DirectX::XMFLOAT3 input1, float input2);

bool IsPrimeCubeCommand(CubeCommand com);