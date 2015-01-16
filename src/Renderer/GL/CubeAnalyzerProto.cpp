#include "CubeAnalyzerProto.h"
#include "ShaderCompilerGL.h"

#define STRING(s) #s

#include <fstream>
#include <vector>
#include <wrl.h>

#include "WICTextureLoader/WICTextureLoader.h"

CubeAnalyzerProto::CubeAnalyzerProto()
{
    CreateWICTextureFromFile(L"cube.jpg", &mProtoTexture);

    // Vertex Shader source
    const std::string vs = STRING
    (
        attribute vec2 aPosition;
        attribute vec2 aTexCoord;
        varying vec2 vTexCoord;
        void main()
        {
            gl_Position = vec4(aPosition, 0.5, 1.0);
            vTexCoord = aTexCoord;
        }
    );

    // Fragment Shader source
    const std::string fs = STRING
    (
        precision mediump float;
        uniform sampler2D sTexture;
        varying vec2 vTexCoord;
        void main()
        {
            gl_FragColor = texture2D(sTexture, vTexCoord);
        }
    );

    mBasicProgram = CompileProgram(vs, fs);

}

CubeAnalyzerProto::~CubeAnalyzerProto()
{
    glDeleteTextures(1, &mProtoTexture);
}

void CubeAnalyzerProto::Draw()
{
    GLfloat vertPos[] =
    {
        -0.8f,  -0.8f,
        -0.8f,   0.0f,
        -0.5f,  -0.8f,
        -0.5f,   0.0f
    };

    GLfloat vertTexCoords[] =
    {
         0.0f, 0.0f,
         0.0f, 1.0f,
         1.0f, 0.0f,
         1.0f, 1.0f
    };

    glUseProgram(mBasicProgram);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, vertPos);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, vertTexCoords);
    glEnableVertexAttribArray(1);

    glBindTexture(GL_TEXTURE_2D, mProtoTexture);
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(0, 0);

    GLboolean cacheDepth;
    glGetBooleanv(GL_DEPTH_TEST, &cacheDepth);
    glDisable(GL_DEPTH_TEST);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    if (!!cacheDepth)
    {
        glEnable(GL_DEPTH_TEST);
    }
}