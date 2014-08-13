#include "pch.h"
#include "Cube_NULL.h"

Cube_NULL::Cube_NULL() : Cube()
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            leftFaceStickers[i][j] = new Sticker_NULL(CUBE_RED);

            rightFaceStickers[i][j] = new Sticker_NULL(CUBE_ORANGE);

            topFaceStickers[i][j] = new Sticker_NULL(CUBE_GREEN);

            bottomFaceStickers[i][j] = new Sticker_NULL(CUBE_BLUE);

            frontFaceStickers[i][j] = new Sticker_NULL(CUBE_YELLOW);

            backFaceStickers[i][j] = new Sticker_NULL(CUBE_WHITE);
        }
    }

    InitializeSlices();
};