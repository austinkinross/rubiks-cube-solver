#pragma once

// OpenGL ES includes
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

// EGL includes
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <EGL/eglplatform.h>

#include <agile.h>

class CubeAnalyzerProto
{
public:
    CubeAnalyzerProto();
    ~CubeAnalyzerProto();

    void Draw();

private:
    GLuint mProtoTexture;
    GLuint mBasicProgram;
};