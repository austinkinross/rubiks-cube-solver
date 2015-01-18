#pragma once

// OpenGL ES includes
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

// EGL includes
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <EGL/eglplatform.h>

#include <agile.h>

#include <fstream>
#include <vector>
#include <wrl.h>

class CubeAnalyzerProto
{
public:
    CubeAnalyzerProto(std::wstring file, int offsetX, int offsetY);
    ~CubeAnalyzerProto();

    void Draw();

private:
    GLuint mCameraTexture;

    const GLint mRenderWidth = 288;
    const GLint mRenderHeight = 512;
    GLuint mRenderTexture;
    GLuint mRenderFramebuffer;

    GLuint mEdgeDetectionProgram;
    GLuint mBlitProgram;

    int mOffsetX;
    int mOffsetY;
};