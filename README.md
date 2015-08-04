C++ Rubik's Cube Solver
==================
Personal project. Updated in bursts every few months or so. Not endorsed by any official "Rubik's Cube" company or product.

About
=====
Work-in-progress C++ Rubik's Cube solver. 

The user holds a Rubik's Cube up to a webcam. The app then analyzes the Rubik's Cube's colors, recreates a 3d model of the cube, and then solves it on-screen.

Long-term aim is to recreate and dramatically improve my old XNA project (click to watch video):

[![Alt text for your video](http://img.youtube.com/vi/rxeDnIynUOo/0.jpg)](https://www.youtube.com/watch?v=rxeDnIynUOo)

Current progress:
=====
- Rubik's Cube solving library complete.
 - Currently requires approximately 80 moves (on average) to solve a rubik's cube.
 - Extensive unit tests available to verify correctness of the library
- Basic Rubik's Cube renderer complete
 - Available in DirectX and OpenGL ES (using [ANGLE](https://github.com/MSOpenTech/angle) on Windows)
- Compiles cross-platform
 - Windows 8.1, Windows 10 and Android projects available
 - _(Android projects have currently rotten a bit; will require refreshing soon)_
- Webcam Rubik's Cube detection nearly complete on Windows 10
 - Uses new camera and [Win2D](https://github.com/microsoft/win2d) features to detect Rubik's Cube in webcam feed, and and find locations of cublets' faces
 - Populating 3d cube's colors with colors detected from webcam feed is still work-in-progress
- Webcam detection currently unavailable on Windows 8.1 and Android
