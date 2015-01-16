#pragma once

// OpenGL ES includes
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

// EGL includes
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <EGL/eglplatform.h>

#include <agile.h>

#include "Renderer\Renderer.h"

#include "CubeAnalyzerProto.h"

class Sticker;

class RendererGL : public Renderer
{
public:
    RendererGL();
    ~RendererGL();

    virtual void RenderSticker(Sticker* pSticker, glm::mat4 *pWorldMatrix, glm::mat4 *pViewMatrix, glm::mat4 *pProjectionMatrix);

    virtual void Swap();

    virtual void Clear();

    void UpdateForWindowSizeChange();

    void GenerateStickerResources();

protected:
    void CreateWindowResources();
    void DestroyWindowResources();

#ifdef WIN32
    EGLDisplay mEglDisplay;
    EGLContext mEglContext;
    EGLSurface mEglSurface;
#endif

    GLuint mStickerProgram;
    GLint mStickerModelUniformPos;
    GLint mStickerViewUniformPos;
    GLint mStickerProjectionUniformPos;
    GLint mStickerColorUniformPos;

    CubeAnalyzerProto* mCubeAnalyzerProto1;
    CubeAnalyzerProto* mCubeAnalyzerProto2;
    CubeAnalyzerProto* mCubeAnalyzerProto3;
    CubeAnalyzerProto* mCubeAnalyzerProto4;
    CubeAnalyzerProto* mCubeAnalyzerProto5;
    CubeAnalyzerProto* mCubeAnalyzerProto6;
    CubeAnalyzerProto* mCubeAnalyzerProto7;
};