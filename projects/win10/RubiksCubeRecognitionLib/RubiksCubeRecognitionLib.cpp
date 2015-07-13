
#include "RubiksCubeRecognitionLib.h"

#include "Cube/Cube.h"
#include "Cube/CubeCommandList.h"

using namespace RubiksCubeRecognitionWinRT;

void CubeRecognizer::ColorCubeFace(Cube* cube)
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			Class1^ class1 = ref new Class1;
			bool b = class1->getBool();

			if (b)
			{
				cube->frontFaceStickers[i][j]->SetColor(StickerColor::CUBE_PINK);
			}
		}
	}
}