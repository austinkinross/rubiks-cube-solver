#include "pch.h"
#include "Cube_D3D.h"

Cube_D3D::Cube_D3D(ID3D11Device1* pDevice, ID3D11DeviceContext1* pContext) : Cube()
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            leftFaceStickers[i][j] = new Sticker_D3D(pDevice, pContext, CUBE_RED);

            rightFaceStickers[i][j] = new Sticker_D3D(pDevice, pContext, CUBE_ORANGE);

            topFaceStickers[i][j] = new Sticker_D3D(pDevice, pContext, CUBE_GREEN);

            bottomFaceStickers[i][j] = new Sticker_D3D(pDevice, pContext, CUBE_BLUE);

            frontFaceStickers[i][j] = new Sticker_D3D(pDevice, pContext, CUBE_YELLOW);

            backFaceStickers[i][j] = new Sticker_D3D(pDevice, pContext, CUBE_WHITE);
        }
    }

    InitializeSlices();
};