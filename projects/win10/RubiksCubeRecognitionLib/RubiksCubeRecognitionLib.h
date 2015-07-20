#pragma once

class Cube;

#include "AgnosticApp.h"

class CubeRecognizer
{
public:
	virtual void ColorNextCubeFace() = 0;
};

CubeRecognizer* createCubeRecognizer(Cube *cube, WindowWrapper *windowWrapper);