
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
	Class1^ class1;
};

CubeRecognizer* createCubeRecognizer(WindowWrapper *windowWrapper)
{
	return new CubeRecognizerWinRT(windowWrapper);
}

CubeRecognizerWinRT::CubeRecognizerWinRT(WindowWrapper *windowWrapper)
{
	CaptureElement^ captureElement = reinterpret_cast<CaptureElement^>(windowWrapper->cameraPreviewPane);
	class1 = ref new Class1(captureElement);
}

void CubeRecognizerWinRT::ColorCubeFace(Cube* cube)
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			
			bool b = class1->getBool();

			if (b)
			{
				cube->frontFaceStickers[i][j]->SetColor(StickerColor::CUBE_PINK);
			}
		}
	}
}