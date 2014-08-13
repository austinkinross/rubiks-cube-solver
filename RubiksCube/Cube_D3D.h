#pragma once

#include "pch.h"
#include "Cube.h"
#include "Sticker_D3D.h"
#include "Slice.h"
#include "CubeHelperFunctions.h"

class Cube_D3D : public Cube
{
public:
    EXPORTFORTEST Cube_D3D(ID3D11Device1* pDevice, ID3D11DeviceContext1* pContext);
};
