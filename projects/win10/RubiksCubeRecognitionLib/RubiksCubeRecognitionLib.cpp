
#include "RubiksCubeRecognitionLib.h"

#include "Cube/Cube.h"
#include "Cube/CubeCommandList.h"

using namespace RubiksCubeRecognitionWinRT;
using namespace Windows::UI::Xaml::Controls;

class CubeRecognizerWinRT : public CubeRecognizer
{
public:
	CubeRecognizerWinRT(WindowWrapper *windowWrapper);

	void ColorCubeFace(Cube* cube) override;

private:
	CSharpRecognizer^ mCSharpRecognizer;
};

CubeRecognizer* createCubeRecognizer(WindowWrapper *windowWrapper)
{
	return new CubeRecognizerWinRT(windowWrapper);
}

CubeRecognizerWinRT::CubeRecognizerWinRT(WindowWrapper *windowWrapper)
{
	CaptureElement^ captureElement = reinterpret_cast<CaptureElement^>(windowWrapper->cameraPreviewPane);
	TextBlock^ progressText = reinterpret_cast<TextBlock^>(windowWrapper->cameraErrorText);
	mCSharpRecognizer = ref new CSharpRecognizer(captureElement, progressText);
}

void CubeRecognizerWinRT::ColorCubeFace(Cube* cube)
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			bool b = mCSharpRecognizer->getBool();

			if (b)
			{
				cube->frontFaceStickers[i][j]->SetColor(StickerColor::CUBE_PINK);
			}
		}
	}
}