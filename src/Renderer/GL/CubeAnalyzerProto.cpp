#include "CubeAnalyzerProto.h"
#include "ShaderCompilerGL.h"

#define STRING(s) #s



#include "WICTextureLoader/WICTextureLoader.h"

CubeAnalyzerProto::CubeAnalyzerProto(std::wstring file, int offsetX, int offsetY)
{
    CreateWICTextureFromFile(file.c_str(), &mProtoTexture);

    // Vertex Shader source
    const std::string vs = STRING
    (
        attribute vec2 aPosition;
        attribute vec2 aTexCoord;
        varying vec2 vTexCoord;
        uniform vec2 uRenderWidth;
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
        uniform highp vec2 uRenderWidth;
        varying vec2 vTexCoord;
        void main()
        {
            vec4 up =   texture2D(sTexture, vec2((vTexCoord.x) / uRenderWidth.x, (vTexCoord.y - 5.0) / uRenderWidth.y));
            vec4 down = texture2D(sTexture, vec2((vTexCoord.x) / uRenderWidth.x, (vTexCoord.y + 5.0) / uRenderWidth.y));
            vec4 left = texture2D(sTexture, vec2((vTexCoord.x - 5.0) / uRenderWidth.x, (vTexCoord.y) / uRenderWidth.y));
            vec4 right = texture2D(sTexture, vec2((vTexCoord.x + 5.0) / uRenderWidth.x, (vTexCoord.y) / uRenderWidth.y));

            vec4 com = 4.0 * texture2D(sTexture, vec2(vTexCoord.x / uRenderWidth.x, vTexCoord.y / uRenderWidth.y)) - up - down - left - right;
            float combineFloat = com.x * com.x + com.y * com.y + com.z * com.z + com.w * com.w;

            gl_FragColor = vec4(combineFloat);
        }
    );

    mBasicProgram = CompileProgram(vs, fs);

    mOffsetX = offsetX;
    mOffsetY = offsetY;
}

CubeAnalyzerProto::~CubeAnalyzerProto()
{
    glDeleteTextures(1, &mProtoTexture);
}

void CubeAnalyzerProto::Draw()
{
    float oldViewport[4];
    glGetFloatv(GL_VIEWPORT, &oldViewport[0]);

    GLfloat vertPos[] =
    {
        -1.0f,  -1.0f,
        -1.0f,   1.0f,
         1.0f,  -1.0f,
         1.0f,   1.0f
    };

    GLfloat vertTexCoords[] =
    {
         0.0f, 0.0f,
         0.0f, 512.0f,
         288.0f, 0.0f,
         288.0f, 512
    };

    glUseProgram(mBasicProgram);

    glViewport(100 * mOffsetX, 100 * mOffsetY, 288, 512);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, vertPos);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, vertTexCoords);
    glEnableVertexAttribArray(1);

    glBindTexture(GL_TEXTURE_2D, mProtoTexture);
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(0, 0);

    glUniform2f(1, 288, 512);

    GLboolean cacheDepth;
    glGetBooleanv(GL_DEPTH_TEST, &cacheDepth);
    glDisable(GL_DEPTH_TEST);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    if (!!cacheDepth)
    {
        glEnable(GL_DEPTH_TEST);
    }

    glViewport(oldViewport[0], oldViewport[1], oldViewport[2], oldViewport[3]);
}