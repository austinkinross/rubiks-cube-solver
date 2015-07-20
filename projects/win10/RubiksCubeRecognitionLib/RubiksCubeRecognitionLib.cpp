
#include "RubiksCubeRecognitionLib.h"

#include "Cube/Cube.h"
#include "Cube/CubeCommandList.h"

using namespace RubiksCubeRecognitionWinRT;
using namespace Windows::UI::Xaml::Controls;

class CubeRecognizerWinRT : public CubeRecognizer
{
public:
	CubeRecognizerWinRT(Cube *cube, WindowWrapper *windowWrapper);

	void ColorNextCubeFace() override;

private:
	CSharpRecognizer^ mCSharpRecognizer;
	Cube *mCube;
	int currentColoringCubeFace;
};

CubeRecognizer* createCubeRecognizer(Cube *cube, WindowWrapper *windowWrapper)
{
	return new CubeRecognizerWinRT(cube, windowWrapper);
}

CubeRecognizerWinRT::CubeRecognizerWinRT(Cube *cube, WindowWrapper *windowWrapper)
{
	mCube = cube;
	CaptureElement^ captureElement = reinterpret_cast<CaptureElement^>(windowWrapper->cameraPreviewPane);
	TextBlock^ progressText = reinterpret_cast<TextBlock^>(windowWrapper->cameraErrorText);
	mCSharpRecognizer = ref new CSharpRecognizer(captureElement, progressText);
	currentColoringCubeFace = 0;
}

void CubeRecognizerWinRT::ColorNextCubeFace()
{
	Platform::Array<Windows::UI::Color>^ recognizerColors;
	recognizerColors = mCSharpRecognizer->getLastColorArray();

	for (int x = 0; x < 3; x++)
	{
		for (int y = 0; y < 3; y++)
		{
			Windows::UI::Color color = recognizerColors[x * 3 + y];

			switch (currentColoringCubeFace)
			{
			case 0:
				mCube->topFaceStickers[2 - y][x]->SetRGBColor(((float)color.R / 255.0f), ((float)color.G / 255.0f), ((float)color.B / 255.0f));
				break;
			case 1:
				mCube->rightFaceStickers[x][y]->SetRGBColor(((float)color.R / 255.0f), ((float)color.G / 255.0f), ((float)color.B / 255.0f));
				break;
			case 2:
				mCube->bottomFaceStickers[2 - x][2 - y]->SetRGBColor(((float)color.R / 255.0f), ((float)color.G / 255.0f), ((float)color.B / 255.0f));
				break;
			case 3:
				mCube->frontFaceStickers[x][y]->SetRGBColor(((float)color.R / 255.0f), ((float)color.G / 255.0f), ((float)color.B / 255.0f));
				break;
			case 4:
				mCube->leftFaceStickers[x][y]->SetRGBColor(((float)color.R / 255.0f), ((float)color.G / 255.0f), ((float)color.B / 255.0f));
				break;
			case 5:
				mCube->backFaceStickers[x][y]->SetRGBColor(((float)color.R / 255.0f), ((float)color.G / 255.0f), ((float)color.B / 255.0f));
				break;
			}

		}
	}

	currentColoringCubeFace++;

}