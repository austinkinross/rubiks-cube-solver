#include "pch.h"
#include "CppUnitTest.h"

#include "../RubiksCube/Cube.h"
#include "../RubiksCube/CubeSolver.h"
// #pragma comment(lib, "../Debug/RubiksCube/RubiksCube.lib")

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestLibrary
{

	void VerifyCube(Cube *pCube, bool bVerifyCross, bool bVerifyF2L, bool bVerifyTopOrientation, bool bVerifyTopPermutation);

    TEST_CLASS(CubeTests)
    {
    public:

		//-----------------------------------------------------------------------
		// Test to validate the initial cross being formed on the base. 
		// It randomly generates a cube, uses the CubeSolver to form the cross, and then validates the result. 
		//-----------------------------------------------------------------------
        TEST_METHOD(ValidateInitialCross)
        {
			const UINT numSeedsToTest = 10000;

			for (int i = 0; i < numSeedsToTest; i++)
			{
				UINT seed = 2281 + i;

				Cube *pCube = new Cube();
				pCube->Randomize(seed);

				CubeSolver *pCubeSolver = new CubeSolver(pCube);
				pCubeSolver->PrepareForSolve();
				pCubeSolver->SolveStage1();

				// Ensure there is a cross shape on the bottom face
				VerifyCube(pCube, true, false, false, false);
			}
        }

		//-----------------------------------------------------------------------
		// Test to validate the F2L algorithms
		// It randomly generates a cube, uses the CubeSolver to solve the cross, then the first two layers, then validates the result. 
		//-----------------------------------------------------------------------
        TEST_METHOD(ValidateF2L)
        {
			const UINT numSeedsToTest = 10000;

			for (int i = 0; i < numSeedsToTest; i++)
			{
				UINT seed = 3875 + i;

				Cube *pCube = new Cube();
				pCube->Randomize(seed);

				CubeSolver *pCubeSolver = new CubeSolver(pCube);
				pCubeSolver->PrepareForSolve();
				pCubeSolver->SolveStage1();
				pCubeSolver->SolveStage2();

				// Ensure the bottom face is totally solved
				VerifyCube(pCube, true, true, false, false);
			}
        }

		//-----------------------------------------------------------------------
		// Test to validate the orientation of the top layer
		// It randomly generates a cube, uses the CubeSolver to solve the cross, then the first two layers, then orients the top layer, then validates the result. 
		//-----------------------------------------------------------------------
        TEST_METHOD(ValidateTopOrientation)
        {
			const UINT numSeedsToTest = 10000;

			for (int i = 0; i < numSeedsToTest; i++)
			{
				UINT seed = 3875 + i;

				char s[50];
				sprintf_s(s, "Current seed: %d.\n", seed);

				OutputDebugStringA(s);

				Cube *pCube = new Cube();
				pCube->Randomize(seed);

				CubeSolver *pCubeSolver = new CubeSolver(pCube);
				pCubeSolver->PrepareForSolve();
				pCubeSolver->SolveStage1();
				pCubeSolver->SolveStage2();
				pCubeSolver->SolveStage3();

				// Ensure the bottom face is totally solved
				VerifyCube(pCube, true, true, true, false);
			}
        }

		//-----------------------------------------------------------------------
		// Test to validate the complete solution of the cube!
		// It randomly generates a cube, uses the CubeSolver to solve the cube, then validates the result
		//-----------------------------------------------------------------------
        TEST_METHOD(ValidateTopPermutation)
        {
			const UINT numSeedsToTest = 10000;

			for (int i = 0; i < numSeedsToTest; i++)
			{
				UINT seed = 3877 + i;

				char s[50];
				sprintf_s(s, "Current seed: %d.\n", seed);

				OutputDebugStringA(s);

				Cube *pCube = new Cube();
				pCube->Randomize(seed);

				CubeSolver *pCubeSolver = new CubeSolver(pCube);
				pCubeSolver->PrepareForSolve();
				pCubeSolver->SolveStage1();
				pCubeSolver->SolveStage2();
				pCubeSolver->SolveStage3();
				pCubeSolver->SolveStage4();

				// Ensure the bottom face is totally solved
				VerifyCube(pCube, true, true, true, true);
			}
        }


		//-----------------------------------------------------------------------
		// Test to validate the solution optimiser
		//-----------------------------------------------------------------------
		TEST_METHOD(ValidateOptimiser)
        {
            
            const UINT numSeedsToTest = 1000;
            
            UINT totalNumberOfMoves = 0;

            for (int i = 0; i < numSeedsToTest; i++)
            {
                UINT seed = 3877 + i;

                char s[50];
                sprintf_s(s, "Current seed: %d.\n", seed);
                OutputDebugStringA(s);

                Cube *pCube = new Cube();
                pCube->Randomize(seed);

                CubeSolver *pCubeSolver = new CubeSolver(pCube);
                pCubeSolver->Solve();

                CubeCommandList* pCubeCommandList = NULL;
                pCubeSolver->GetCubeCommandList(&pCubeCommandList);
                pCubeCommandList->Optimize();

                CHAR outputChar[100];
                sprintf_s(&outputChar[0], 100, "Number of moves: %d\n", pCubeCommandList->GetLength());
                OutputDebugStringA(outputChar);

                totalNumberOfMoves += pCubeCommandList->GetLength();

                // Ensure the bottom face is totally solved
                VerifyCube(pCube, true, true, true, true);
            }

            CHAR outputChar[100];
            sprintf_s(&outputChar[0], 100, "Total number of moves: %d\n", totalNumberOfMoves);
            OutputDebugStringA(outputChar);

        }

    };

	void VerifyCube(Cube *pCube, bool bVerifyCross, bool bVerifyF2L, bool bVerifyTopOrientation, bool bVerifyTopPermutation)
	{
		if (bVerifyCross)
		{
			Assert::AreEqual((UINT)pCube->bottomFaceStickers[1][1]->GetColor(), (UINT)pCube->bottomFaceStickers[1][0]->GetColor());
			Assert::AreEqual((UINT)pCube->bottomFaceStickers[1][1]->GetColor(), (UINT)pCube->bottomFaceStickers[0][1]->GetColor());
			Assert::AreEqual((UINT)pCube->bottomFaceStickers[1][1]->GetColor(), (UINT)pCube->bottomFaceStickers[1][2]->GetColor());
			Assert::AreEqual((UINT)pCube->bottomFaceStickers[1][1]->GetColor(), (UINT)pCube->bottomFaceStickers[2][1]->GetColor());

			// Ensure the cross is using the correct pieces, i.e. the adjacent sides are correct 
			Assert::AreEqual((UINT)pCube->leftFaceStickers[1][2]->GetColor(), (UINT)pCube->leftFaceStickers[1][1]->GetColor());
			Assert::AreEqual((UINT)pCube->rightFaceStickers[1][2]->GetColor(), (UINT)pCube->rightFaceStickers[1][1]->GetColor());
			Assert::AreEqual((UINT)pCube->frontFaceStickers[1][2]->GetColor(), (UINT)pCube->frontFaceStickers[1][1]->GetColor());
			Assert::AreEqual((UINT)pCube->backFaceStickers[1][2]->GetColor(), (UINT)pCube->backFaceStickers[1][1]->GetColor());
		}

		if (bVerifyF2L)
		{
			Assert::AreEqual((UINT)pCube->bottomFaceStickers[1][1]->GetColor(), (UINT)pCube->bottomFaceStickers[0][0]->GetColor());
			Assert::AreEqual((UINT)pCube->bottomFaceStickers[1][1]->GetColor(), (UINT)pCube->bottomFaceStickers[0][2]->GetColor());
			Assert::AreEqual((UINT)pCube->bottomFaceStickers[1][1]->GetColor(), (UINT)pCube->bottomFaceStickers[2][0]->GetColor());
			Assert::AreEqual((UINT)pCube->bottomFaceStickers[1][1]->GetColor(), (UINT)pCube->bottomFaceStickers[2][2]->GetColor());

			// Ensure the bottom two layers are solved, too
			Assert::AreEqual((UINT)pCube->leftFaceStickers[0][2]->GetColor(), (UINT)pCube->leftFaceStickers[1][1]->GetColor());
			Assert::AreEqual((UINT)pCube->leftFaceStickers[0][1]->GetColor(), (UINT)pCube->leftFaceStickers[1][1]->GetColor());
			Assert::AreEqual((UINT)pCube->leftFaceStickers[2][2]->GetColor(), (UINT)pCube->leftFaceStickers[1][1]->GetColor());
			Assert::AreEqual((UINT)pCube->leftFaceStickers[2][1]->GetColor(), (UINT)pCube->leftFaceStickers[1][1]->GetColor());

			Assert::AreEqual((UINT)pCube->rightFaceStickers[0][2]->GetColor(), (UINT)pCube->rightFaceStickers[1][1]->GetColor());
			Assert::AreEqual((UINT)pCube->rightFaceStickers[0][1]->GetColor(), (UINT)pCube->rightFaceStickers[1][1]->GetColor());
			Assert::AreEqual((UINT)pCube->rightFaceStickers[2][2]->GetColor(), (UINT)pCube->rightFaceStickers[1][1]->GetColor());
			Assert::AreEqual((UINT)pCube->rightFaceStickers[2][1]->GetColor(), (UINT)pCube->rightFaceStickers[1][1]->GetColor());

			Assert::AreEqual((UINT)pCube->frontFaceStickers[0][2]->GetColor(), (UINT)pCube->frontFaceStickers[1][1]->GetColor());
			Assert::AreEqual((UINT)pCube->frontFaceStickers[0][1]->GetColor(), (UINT)pCube->frontFaceStickers[1][1]->GetColor());
			Assert::AreEqual((UINT)pCube->frontFaceStickers[2][2]->GetColor(), (UINT)pCube->frontFaceStickers[1][1]->GetColor());
			Assert::AreEqual((UINT)pCube->frontFaceStickers[2][1]->GetColor(), (UINT)pCube->frontFaceStickers[1][1]->GetColor());

			Assert::AreEqual((UINT)pCube->backFaceStickers[0][2]->GetColor(), (UINT)pCube->backFaceStickers[1][1]->GetColor());
			Assert::AreEqual((UINT)pCube->backFaceStickers[0][1]->GetColor(), (UINT)pCube->backFaceStickers[1][1]->GetColor());
			Assert::AreEqual((UINT)pCube->backFaceStickers[2][2]->GetColor(), (UINT)pCube->backFaceStickers[1][1]->GetColor());
			Assert::AreEqual((UINT)pCube->backFaceStickers[2][1]->GetColor(), (UINT)pCube->backFaceStickers[1][1]->GetColor());
		}

		if (bVerifyTopOrientation)
		{
			Assert::AreEqual((UINT)pCube->topFaceStickers[1][1]->GetColor(), (UINT)pCube->topFaceStickers[1][0]->GetColor());
			Assert::AreEqual((UINT)pCube->topFaceStickers[1][1]->GetColor(), (UINT)pCube->topFaceStickers[0][1]->GetColor());
			Assert::AreEqual((UINT)pCube->topFaceStickers[1][1]->GetColor(), (UINT)pCube->topFaceStickers[1][2]->GetColor());
			Assert::AreEqual((UINT)pCube->topFaceStickers[1][1]->GetColor(), (UINT)pCube->topFaceStickers[2][1]->GetColor());
			Assert::AreEqual((UINT)pCube->topFaceStickers[1][1]->GetColor(), (UINT)pCube->topFaceStickers[0][0]->GetColor());
			Assert::AreEqual((UINT)pCube->topFaceStickers[1][1]->GetColor(), (UINT)pCube->topFaceStickers[0][2]->GetColor());
			Assert::AreEqual((UINT)pCube->topFaceStickers[1][1]->GetColor(), (UINT)pCube->topFaceStickers[2][0]->GetColor());
			Assert::AreEqual((UINT)pCube->topFaceStickers[1][1]->GetColor(), (UINT)pCube->topFaceStickers[2][2]->GetColor());
		}

		if (bVerifyTopPermutation)
		{
			Assert::AreEqual((UINT)pCube->leftFaceStickers[0][0]->GetColor(), (UINT)pCube->leftFaceStickers[1][1]->GetColor());
			Assert::AreEqual((UINT)pCube->leftFaceStickers[1][0]->GetColor(), (UINT)pCube->leftFaceStickers[1][1]->GetColor());
			Assert::AreEqual((UINT)pCube->leftFaceStickers[2][0]->GetColor(), (UINT)pCube->leftFaceStickers[1][1]->GetColor());

			Assert::AreEqual((UINT)pCube->rightFaceStickers[0][0]->GetColor(), (UINT)pCube->rightFaceStickers[1][1]->GetColor());
			Assert::AreEqual((UINT)pCube->rightFaceStickers[1][0]->GetColor(), (UINT)pCube->rightFaceStickers[1][1]->GetColor());
			Assert::AreEqual((UINT)pCube->rightFaceStickers[2][0]->GetColor(), (UINT)pCube->rightFaceStickers[1][1]->GetColor());

			Assert::AreEqual((UINT)pCube->frontFaceStickers[0][0]->GetColor(), (UINT)pCube->frontFaceStickers[1][1]->GetColor());
			Assert::AreEqual((UINT)pCube->frontFaceStickers[1][0]->GetColor(), (UINT)pCube->frontFaceStickers[1][1]->GetColor());
			Assert::AreEqual((UINT)pCube->frontFaceStickers[2][0]->GetColor(), (UINT)pCube->frontFaceStickers[1][1]->GetColor());

			Assert::AreEqual((UINT)pCube->backFaceStickers[0][0]->GetColor(), (UINT)pCube->backFaceStickers[1][1]->GetColor());
			Assert::AreEqual((UINT)pCube->backFaceStickers[1][0]->GetColor(), (UINT)pCube->backFaceStickers[1][1]->GetColor());
			Assert::AreEqual((UINT)pCube->backFaceStickers[2][0]->GetColor(), (UINT)pCube->backFaceStickers[1][1]->GetColor());
		}
	}
}