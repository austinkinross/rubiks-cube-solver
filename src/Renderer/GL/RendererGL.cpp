#include "RendererGL.h"
#include "ShaderCompilerGL.h"

#define STRING(s) #s

#include "CubeAnalyzerProto.h"

RendererGL::RendererGL()
{
    CreateWindowResources();

    GenerateStickerResources();

    glEnable(GL_DEPTH_TEST);

    mCubeAnalyzerProto1 = new CubeAnalyzerProto(L"cube.jpg", 1, 1);
    mCubeAnalyzerProto2 = new CubeAnalyzerProto(L"cube2.jpg", 4, 1);
    mCubeAnalyzerProto3 = new CubeAnalyzerProto(L"cube3.jpg", 7, 1);

    mCubeAnalyzerProto4 = new CubeAnalyzerProto(L"cube4.jpg", 1, 6);
    mCubeAnalyzerProto5 = new CubeAnalyzerProto(L"cube5.jpg", 4, 6);
    mCubeAnalyzerProto6 = new CubeAnalyzerProto(L"cube6.jpg", 7, 6);

    mCubeAnalyzerProto7 = new CubeAnalyzerProto(L"cube7.jpg", 10, 6);
}

RendererGL::~RendererGL()
{
    if (mStickerProgram != 0)
    {
        glDeleteProgram(mStickerProgram);
        mStickerProgram = 0;
    }

    DestroyWindowResources();
}

void RendererGL::GenerateStickerResources()
{
    // Vertex Shader source
    const std::string vs = STRING
    (
        attribute vec4 vPosition;
        uniform mat4 uModelMatrix;
        uniform mat4 uViewMatrix;
        uniform mat4 uProjectionMatrix;
        uniform vec3 uColor;
        varying vec3 vColor;
        void main()
        {
            gl_Position = vPosition * uModelMatrix * uViewMatrix * uProjectionMatrix;
            vColor = uColor;
        }
    );

    // Fragment Shader source
    const std::string fs = STRING
    (
        precision mediump float;
        varying vec3 vColor;
        void main()
        {
            gl_FragColor = vec4(vColor, 1.0);
        }
    );

    // CompileProgram will throw if it fails, so we don't need to check for success.
    mStickerProgram = CompileProgram(vs, fs);

    mStickerModelUniformPos = glGetUniformLocation(mStickerProgram, "uModelMatrix");
    mStickerViewUniformPos = glGetUniformLocation(mStickerProgram, "uViewMatrix");
    mStickerProjectionUniformPos = glGetUniformLocation(mStickerProgram, "uProjectionMatrix");
    mStickerColorUniformPos = glGetUniformLocation(mStickerProgram, "uColor");
}

void RendererGL::RenderSticker(Sticker* pSticker, glm::mat4 *pWorldMatrix, glm::mat4 *pViewMatrix, glm::mat4 *pProjectionMatrix)
{
    GLfloat vertices[] =
    {
        -0.95f, 3.0f, -0.95f,
        -0.95f, 3.0f, +0.95f,
        +0.95f, 3.0f, -0.95f,
        +0.95f, 3.0f, +0.95f
    };

    glUseProgram(mStickerProgram);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertices);
    glEnableVertexAttribArray(0);

    glUniformMatrix4fv(mStickerModelUniformPos, 1, GL_FALSE, (GLfloat*)pWorldMatrix);
    glUniformMatrix4fv(mStickerViewUniformPos, 1, GL_FALSE, (GLfloat*)pViewMatrix);
    glUniformMatrix4fv(mStickerProjectionUniformPos, 1, GL_FALSE, (GLfloat*)pProjectionMatrix);

    glm::vec4 color = ColorToVec4(pSticker->GetColor());
    glUniform3fv(mStickerColorUniformPos, 1, (GLfloat*)&color);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void RendererGL::Clear()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mCubeAnalyzerProto1->Draw();
    mCubeAnalyzerProto2->Draw();
    mCubeAnalyzerProto3->Draw();
    mCubeAnalyzerProto4->Draw();
    mCubeAnalyzerProto5->Draw();
    mCubeAnalyzerProto6->Draw();
    mCubeAnalyzerProto7->Draw();
}