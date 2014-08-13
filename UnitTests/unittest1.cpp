#include "stdafx.h"
#include "CppUnitTest.h"

#include "../RubiksCube/Cube.h"
#include "../RubiksCube/CubeSolver.h"
#pragma comment(lib, "../Debug/RubiksCube/RubiksCube.lib")

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace UnitTests
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestInitialCross)
		{
			UINT seed = 2281;

			Cube *pCube = new Cube();
			pCube->Randomize(seed);

			CubeSolver *pCubeSolver = new CubeSolver(pCube);
			pCubeSolver->SolveStage1();


			// Ensure the cross is formed correctly on the bottom face
			Assert::AreEqual((UINT)pCube->bottomFaceStickers[1][1]->GetColor(), (UINT)pCube->bottomFaceStickers[1][0]->GetColor());
			Assert::AreEqual((UINT)pCube->bottomFaceStickers[1][1]->GetColor(), (UINT)pCube->bottomFaceStickers[0][1]->GetColor());
			Assert::AreEqual((UINT)pCube->bottomFaceStickers[1][1]->GetColor(), (UINT)pCube->bottomFaceStickers[1][2]->GetColor());
			Assert::AreEqual((UINT)pCube->bottomFaceStickers[1][1]->GetColor(), (UINT)pCube->bottomFaceStickers[2][1]->GetColor());

			// Ensure the cross is formed of the correct pieces, i.e. the adjacent sides are correct so far
			Assert::AreEqual((UINT)pCube->leftFaceStickers[1][2]->GetColor(), (UINT)pCube->leftFaceStickers[1][1]->GetColor());
			Assert::AreEqual((UINT)pCube->rightFaceStickers[1][2]->GetColor(), (UINT)pCube->rightFaceStickers[1][1]->GetColor());
			Assert::AreEqual((UINT)pCube->frontFaceStickers[1][2]->GetColor(), (UINT)pCube->frontFaceStickers[1][1]->GetColor());
			Assert::AreEqual((UINT)pCube->backFaceStickers[1][2]->GetColor(), (UINT)pCube->backFaceStickers[1][1]->GetColor());

		}

	};
}