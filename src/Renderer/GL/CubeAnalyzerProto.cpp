#include "CubeAnalyzerProto.h"
#include "ShaderCompilerGL.h"
#include "Helpers.h"

#define STRING(s) #s

#include "WICTextureLoader/WICTextureLoader.h"

CubeAnalyzerProto::CubeAnalyzerProto(std::wstring file, int offsetX, int offsetY)
{
    CreateWICTextureFromFile(file.c_str(), &mCameraTexture);

    glGenTextures(1, &mRenderTexture);
    glBindTexture(GL_TEXTURE_2D, mRenderTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mRenderWidth, mRenderHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenFramebuffers(1, &mRenderFramebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, mRenderFramebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mRenderTexture, 0);
    ThrowIfFalse(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

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
    const std::string edgeDetectionFS = STRING
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

    // Fragment Shader source
    const std::string blitFS = STRING
    (
        precision mediump float;
        uniform sampler2D sTexture;
        uniform highp vec2 uRenderWidth;
        varying vec2 vTexCoord;
        void main()
        {
            gl_FragColor = texture2D(sTexture, vec2((vTexCoord.x) / uRenderWidth.x, (vTexCoord.y) / uRenderWidth.y));
        }
    );


    mEdgeDetectionProgram = CompileProgram(vs, edgeDetectionFS);
    mBlitProgram = CompileProgram(vs, blitFS);

    mOffsetX = offsetX;
    mOffsetY = offsetY;
}

CubeAnalyzerProto::~CubeAnalyzerProto()
{
    glDeleteTextures(1, &mCameraTexture);
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

    glDisable(GL_DEPTH_TEST);

    // Render the camera texture onto the new framebuffer, using the edge-detection shader
    {
        glUseProgram(mEdgeDetectionProgram);

        glBindFramebuffer(GL_FRAMEBUFFER, mRenderFramebuffer);
        glViewport(0, 0, mRenderWidth, mRenderHeight);
        glClear(GL_COLOR_BUFFER_BIT);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, vertPos);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, vertTexCoords);
        glEnableVertexAttribArray(1);

        glBindTexture(GL_TEXTURE_2D, mCameraTexture);
        glActiveTexture(GL_TEXTURE0);
        glUniform1i(0, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glUniform2f(1, 288, 512);

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }

    glEnable(GL_DEPTH_TEST);

    std::vector<uint8> pixels(4 * mRenderWidth * mRenderHeight);
    glReadPixels(0, 0, mRenderWidth, mRenderHeight, GL_RGBA, GL_UNSIGNED_BYTE, &pixels[0]);

    // Find the centers of the cublets

    for (int centerX = 40; centerX < mRenderWidth - 40; centerX += 25)
    {
        for (int centerY = 140; centerY < mRenderHeight - 140; centerY += 25)
        {
            int right = 0;
            int left = 0;
            int top = 0;
            int bottom = 0;

            for (int x = 0; x < mRenderWidth / 6 && centerX + x < mRenderWidth; x++)
            {
                int y = 0;
                uint8 r = pixels[((centerY + y) * mRenderWidth + centerX + x) * 4 + 0];
                uint8 g = pixels[((centerY + y) * mRenderWidth + centerX + x) * 4 + 1];
                uint8 b = pixels[((centerY + y) * mRenderWidth + centerX + x) * 4 + 2];
                uint8 a = pixels[((centerY + y) * mRenderWidth + centerX + x) * 4 + 3];

                if (r + g + b > 150 && right == 0)
                {
                    right = x;
                }
            }

            for (int x = 0; x > -mRenderWidth / 6 && centerX + x > 0; x--)
            {
                int y = 0;
                uint8 r = pixels[((centerY + y) * mRenderWidth + centerX + x) * 4 + 0];
                uint8 g = pixels[((centerY + y) * mRenderWidth + centerX + x) * 4 + 1];
                uint8 b = pixels[((centerY + y) * mRenderWidth + centerX + x) * 4 + 2];
                uint8 a = pixels[((centerY + y) * mRenderWidth + centerX + x) * 4 + 3];

                if (r + g + b > 150 && left == 0)
                {
                    left = x;
                }
            }

            for (int y = 0; y < mRenderWidth / 6 && centerY + y < mRenderHeight; y++)
            {
                int x = 0;
                uint8 r = pixels[((centerY + y) * mRenderWidth + centerX + x) * 4 + 0];
                uint8 g = pixels[((centerY + y) * mRenderWidth + centerX + x) * 4 + 1];
                uint8 b = pixels[((centerY + y) * mRenderWidth + centerX + x) * 4 + 2];
                uint8 a = pixels[((centerY + y) * mRenderWidth + centerX + x) * 4 + 3];

                if (r + g + b > 150 && top == 0)
                {
                    top = y;
                }
            }

            for (int y = 0; y > -mRenderWidth / 6 && centerY + y > 0; y--)
            {
                int x = 0;
                uint8 r = pixels[((centerY + y) * mRenderWidth + centerX + x) * 4 + 0];
                uint8 g = pixels[((centerY + y) * mRenderWidth + centerX + x) * 4 + 1];
                uint8 b = pixels[((centerY + y) * mRenderWidth + centerX + x) * 4 + 2];
                uint8 a = pixels[((centerY + y) * mRenderWidth + centerX + x) * 4 + 3];

                if (r + g + b > 150 && bottom == 0)
                {
                    bottom = y;
                }
            }

            if (top != 0 && bottom != 0 && left != 0 && right != 0)
            {
                if (top - bottom > 30 && right - left > 30)
                {
                    int middleX = (centerX * 2 + right + left) / 2;
                    int middleY = (centerY * 2 + top + bottom) / 2;

                    glEnable(GL_SCISSOR_TEST);
                    glScissor(middleX - 5, middleY - 5, 10, 10);
                    glClearColor(1, 0, 0, 1);
                    glClear(GL_COLOR_BUFFER_BIT);
                    glDisable(GL_SCISSOR_TEST);
                }
            }
        }
    }

    // Draw the edge image to the screen
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glViewport(100 * mOffsetX, 100 * mOffsetY, 288, 512);

        glUseProgram(mBlitProgram);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, vertPos);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, vertTexCoords);
        glEnableVertexAttribArray(1);

        glBindTexture(GL_TEXTURE_2D, mRenderTexture);
        glActiveTexture(GL_TEXTURE0);
        glUniform1i(0, 0);

        glUniform2f(1, mRenderWidth, mRenderHeight);

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        glViewport(oldViewport[0], oldViewport[1], oldViewport[2], oldViewport[3]);
    }
}