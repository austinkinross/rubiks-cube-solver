#pragma once

#include "pch.h"
#include "Renderer.h"

using namespace DirectX;

#include <agile.h>

class Sticker;

// OpenGL ES includes
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

// EGL includes
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <EGL/eglplatform.h>

#include <wrl.h>

class RendererGL : public Renderer
{
public:
    RendererGL();
    ~RendererGL();

    virtual void RenderSticker(Sticker* pSticker, XMFLOAT4X4 *pWorldMatrix, XMFLOAT4X4 *pViewMatrix, XMFLOAT4X4 *pProjectionMatrix);

    virtual void Swap();

    virtual void Clear();

    void UpdateForWindowSizeChange();

protected:
    EGLDisplay mEglDisplay;
    EGLContext mEglContext;
    EGLSurface mEglSurface;
};