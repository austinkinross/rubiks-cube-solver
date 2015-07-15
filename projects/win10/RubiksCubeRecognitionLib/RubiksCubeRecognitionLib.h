#pragma once

class Cube;

#include "AgnosticApp.h"

class CubeRecognizer
{
public:
	virtual void ColorCubeFace(Cube* cube) = 0;
};

CubeRecognizer* createCubeRecognizer(WindowWrapper *windowWrapper);