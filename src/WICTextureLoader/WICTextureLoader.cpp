//--------------------------------------------------------------------------------------
// File: WICTextureLoader.cpp
//
// Function for loading a WIC image and creating a Direct3D 11 runtime texture for it
// (auto-generating mipmaps if possible)
//
// Note: Assumes application has already called CoInitializeEx
//
// Warning: CreateWICTexture* functions are not thread-safe if given a d3dContext instance for
//          auto-gen mipmap support.
//
// Note these functions are useful for images created as simple 2D textures. For
// more complex resources, DDSTextureLoader is an excellent light-weight runtime loader.
// For a full-featured DDS file reader, writer, and texture processing pipeline see
// the 'Texconv' sample and the 'DirectXTex' library.
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//
// http://go.microsoft.com/fwlink/?LinkId=248926
// http://go.microsoft.com/fwlink/?LinkId=248929
//--------------------------------------------------------------------------------------

// We could load multi-frame images (TIFF/GIF) into a texture array.
// For now, we just load the first frame (note: DirectXTex supports multi-frame images)

#include <dxgiformat.h>
#include <assert.h>

#pragma warning(push)
#pragma warning(disable : 4005)
#include <wincodec.h>
#pragma warning(pop)

#include <memory>

#include "WICTextureLoader.h"

#include <GLES2\/gl2.h>

#if (_WIN32_WINNT >= 0x0602 /*_WIN32_WINNT_WIN8*/) && !defined(DXGI_1_2_FORMATS)
#define DXGI_1_2_FORMATS
#endif

//---------------------------------------------------------------------------------
template<class T> class ScopedObject
{
public:
    explicit ScopedObject(T *p = 0) : _pointer(p) {}
    ~ScopedObject()
    {
        if (_pointer)
        {
            _pointer->Release();
            _pointer = nullptr;
        }
    }

    bool IsNull() const { return (!_pointer); }

    T& operator*() { return *_pointer; }
    T* operator->() { return _pointer; }
    T** operator&() { return &_pointer; }

    void Reset(T *p = 0) { if (_pointer) { _pointer->Release(); } _pointer = p; }

    T* Get() const { return _pointer; }

private:
    ScopedObject(const ScopedObject&);
    ScopedObject& operator=(const ScopedObject&);

    T* _pointer;
};

//-------------------------------------------------------------------------------------
// WIC Pixel Format Translation Data
//-------------------------------------------------------------------------------------
struct WICTranslate
{
    GUID                wic;
    DXGI_FORMAT         format;
};

static WICTranslate g_WICFormats[] =
{
    { GUID_WICPixelFormat128bppRGBAFloat, DXGI_FORMAT_R32G32B32A32_FLOAT },

    { GUID_WICPixelFormat64bppRGBAHalf, DXGI_FORMAT_R16G16B16A16_FLOAT },
    { GUID_WICPixelFormat64bppRGBA, DXGI_FORMAT_R16G16B16A16_UNORM },

    { GUID_WICPixelFormat32bppRGBA, DXGI_FORMAT_R8G8B8A8_UNORM },
    { GUID_WICPixelFormat32bppBGRA, DXGI_FORMAT_B8G8R8A8_UNORM }, // DXGI 1.1
    { GUID_WICPixelFormat32bppBGR, DXGI_FORMAT_B8G8R8X8_UNORM }, // DXGI 1.1

    { GUID_WICPixelFormat32bppRGBA1010102XR, DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM }, // DXGI 1.1
    { GUID_WICPixelFormat32bppRGBA1010102, DXGI_FORMAT_R10G10B10A2_UNORM },
    { GUID_WICPixelFormat32bppRGBE, DXGI_FORMAT_R9G9B9E5_SHAREDEXP },

#ifdef DXGI_1_2_FORMATS

    { GUID_WICPixelFormat16bppBGRA5551, DXGI_FORMAT_B5G5R5A1_UNORM },
    { GUID_WICPixelFormat16bppBGR565, DXGI_FORMAT_B5G6R5_UNORM },

#endif // DXGI_1_2_FORMATS

    { GUID_WICPixelFormat32bppGrayFloat, DXGI_FORMAT_R32_FLOAT },
    { GUID_WICPixelFormat16bppGrayHalf, DXGI_FORMAT_R16_FLOAT },
    { GUID_WICPixelFormat16bppGray, DXGI_FORMAT_R16_UNORM },
    { GUID_WICPixelFormat8bppGray, DXGI_FORMAT_R8_UNORM },

    { GUID_WICPixelFormat8bppAlpha, DXGI_FORMAT_A8_UNORM },

#if (_WIN32_WINNT >= 0x0602 /*_WIN32_WINNT_WIN8*/)
    { GUID_WICPixelFormat96bppRGBFloat, DXGI_FORMAT_R32G32B32_FLOAT },
#endif
};

//-------------------------------------------------------------------------------------
// WIC Pixel Format nearest conversion table
//-------------------------------------------------------------------------------------

struct WICConvert
{
    GUID        source;
    GUID        target;
};

static WICConvert g_WICConvert[] =
{
    // Note target GUID in this conversion table must be one of those directly supported formats (above).

    { GUID_WICPixelFormatBlackWhite, GUID_WICPixelFormat8bppGray }, // DXGI_FORMAT_R8_UNORM

    { GUID_WICPixelFormat1bppIndexed, GUID_WICPixelFormat32bppRGBA }, // DXGI_FORMAT_R8G8B8A8_UNORM 
    { GUID_WICPixelFormat2bppIndexed, GUID_WICPixelFormat32bppRGBA }, // DXGI_FORMAT_R8G8B8A8_UNORM 
    { GUID_WICPixelFormat4bppIndexed, GUID_WICPixelFormat32bppRGBA }, // DXGI_FORMAT_R8G8B8A8_UNORM 
    { GUID_WICPixelFormat8bppIndexed, GUID_WICPixelFormat32bppRGBA }, // DXGI_FORMAT_R8G8B8A8_UNORM 

    { GUID_WICPixelFormat2bppGray, GUID_WICPixelFormat8bppGray }, // DXGI_FORMAT_R8_UNORM 
    { GUID_WICPixelFormat4bppGray, GUID_WICPixelFormat8bppGray }, // DXGI_FORMAT_R8_UNORM 

    { GUID_WICPixelFormat16bppGrayFixedPoint, GUID_WICPixelFormat16bppGrayHalf }, // DXGI_FORMAT_R16_FLOAT 
    { GUID_WICPixelFormat32bppGrayFixedPoint, GUID_WICPixelFormat32bppGrayFloat }, // DXGI_FORMAT_R32_FLOAT 

#ifdef DXGI_1_2_FORMATS

    { GUID_WICPixelFormat16bppBGR555, GUID_WICPixelFormat16bppBGRA5551 }, // DXGI_FORMAT_B5G5R5A1_UNORM

#else

    { GUID_WICPixelFormat16bppBGR555, GUID_WICPixelFormat32bppRGBA }, // DXGI_FORMAT_R8G8B8A8_UNORM
    { GUID_WICPixelFormat16bppBGRA5551, GUID_WICPixelFormat32bppRGBA }, // DXGI_FORMAT_R8G8B8A8_UNORM
    { GUID_WICPixelFormat16bppBGR565, GUID_WICPixelFormat32bppRGBA }, // DXGI_FORMAT_R8G8B8A8_UNORM

#endif // DXGI_1_2_FORMATS

    { GUID_WICPixelFormat32bppBGR101010, GUID_WICPixelFormat32bppRGBA1010102 }, // DXGI_FORMAT_R10G10B10A2_UNORM

    { GUID_WICPixelFormat24bppBGR, GUID_WICPixelFormat32bppRGBA }, // DXGI_FORMAT_R8G8B8A8_UNORM 
    { GUID_WICPixelFormat24bppRGB, GUID_WICPixelFormat32bppRGBA }, // DXGI_FORMAT_R8G8B8A8_UNORM 
    { GUID_WICPixelFormat32bppPBGRA, GUID_WICPixelFormat32bppRGBA }, // DXGI_FORMAT_R8G8B8A8_UNORM 
    { GUID_WICPixelFormat32bppPRGBA, GUID_WICPixelFormat32bppRGBA }, // DXGI_FORMAT_R8G8B8A8_UNORM 

    { GUID_WICPixelFormat48bppRGB, GUID_WICPixelFormat64bppRGBA }, // DXGI_FORMAT_R16G16B16A16_UNORM
    { GUID_WICPixelFormat48bppBGR, GUID_WICPixelFormat64bppRGBA }, // DXGI_FORMAT_R16G16B16A16_UNORM
    { GUID_WICPixelFormat64bppBGRA, GUID_WICPixelFormat64bppRGBA }, // DXGI_FORMAT_R16G16B16A16_UNORM
    { GUID_WICPixelFormat64bppPRGBA, GUID_WICPixelFormat64bppRGBA }, // DXGI_FORMAT_R16G16B16A16_UNORM
    { GUID_WICPixelFormat64bppPBGRA, GUID_WICPixelFormat64bppRGBA }, // DXGI_FORMAT_R16G16B16A16_UNORM

    { GUID_WICPixelFormat48bppRGBFixedPoint, GUID_WICPixelFormat64bppRGBAHalf }, // DXGI_FORMAT_R16G16B16A16_FLOAT 
    { GUID_WICPixelFormat48bppBGRFixedPoint, GUID_WICPixelFormat64bppRGBAHalf }, // DXGI_FORMAT_R16G16B16A16_FLOAT 
    { GUID_WICPixelFormat64bppRGBAFixedPoint, GUID_WICPixelFormat64bppRGBAHalf }, // DXGI_FORMAT_R16G16B16A16_FLOAT 
    { GUID_WICPixelFormat64bppBGRAFixedPoint, GUID_WICPixelFormat64bppRGBAHalf }, // DXGI_FORMAT_R16G16B16A16_FLOAT 
    { GUID_WICPixelFormat64bppRGBFixedPoint, GUID_WICPixelFormat64bppRGBAHalf }, // DXGI_FORMAT_R16G16B16A16_FLOAT 
    { GUID_WICPixelFormat64bppRGBHalf, GUID_WICPixelFormat64bppRGBAHalf }, // DXGI_FORMAT_R16G16B16A16_FLOAT 
    { GUID_WICPixelFormat48bppRGBHalf, GUID_WICPixelFormat64bppRGBAHalf }, // DXGI_FORMAT_R16G16B16A16_FLOAT 

    { GUID_WICPixelFormat96bppRGBFixedPoint, GUID_WICPixelFormat128bppRGBAFloat }, // DXGI_FORMAT_R32G32B32A32_FLOAT 
    { GUID_WICPixelFormat128bppPRGBAFloat, GUID_WICPixelFormat128bppRGBAFloat }, // DXGI_FORMAT_R32G32B32A32_FLOAT 
    { GUID_WICPixelFormat128bppRGBFloat, GUID_WICPixelFormat128bppRGBAFloat }, // DXGI_FORMAT_R32G32B32A32_FLOAT 
    { GUID_WICPixelFormat128bppRGBAFixedPoint, GUID_WICPixelFormat128bppRGBAFloat }, // DXGI_FORMAT_R32G32B32A32_FLOAT 
    { GUID_WICPixelFormat128bppRGBFixedPoint, GUID_WICPixelFormat128bppRGBAFloat }, // DXGI_FORMAT_R32G32B32A32_FLOAT 

    { GUID_WICPixelFormat32bppCMYK, GUID_WICPixelFormat32bppRGBA }, // DXGI_FORMAT_R8G8B8A8_UNORM 
    { GUID_WICPixelFormat64bppCMYK, GUID_WICPixelFormat64bppRGBA }, // DXGI_FORMAT_R16G16B16A16_UNORM
    { GUID_WICPixelFormat40bppCMYKAlpha, GUID_WICPixelFormat64bppRGBA }, // DXGI_FORMAT_R16G16B16A16_UNORM
    { GUID_WICPixelFormat80bppCMYKAlpha, GUID_WICPixelFormat64bppRGBA }, // DXGI_FORMAT_R16G16B16A16_UNORM

#if (_WIN32_WINNT >= 0x0602 /*_WIN32_WINNT_WIN8*/)
    { GUID_WICPixelFormat32bppRGB, GUID_WICPixelFormat32bppRGBA }, // DXGI_FORMAT_R8G8B8A8_UNORM
    { GUID_WICPixelFormat64bppRGB, GUID_WICPixelFormat64bppRGBA }, // DXGI_FORMAT_R16G16B16A16_UNORM
    { GUID_WICPixelFormat64bppPRGBAHalf, GUID_WICPixelFormat64bppRGBAHalf }, // DXGI_FORMAT_R16G16B16A16_FLOAT 
#endif

    // We don't support n-channel formats
};

//--------------------------------------------------------------------------------------
static IWICImagingFactory* _GetWIC()
{
    static IWICImagingFactory* s_Factory = nullptr;

    if (s_Factory)
        return s_Factory;

    HRESULT hr = CoCreateInstance(
        CLSID_WICImagingFactory,
        nullptr,
        CLSCTX_INPROC_SERVER,
        __uuidof(IWICImagingFactory),
        (LPVOID*)&s_Factory
        );

    if (FAILED(hr))
    {
        s_Factory = nullptr;
        return nullptr;
    }

    return s_Factory;
}

//---------------------------------------------------------------------------------
static DXGI_FORMAT _WICToDXGI(const GUID& guid)
{
    for (size_t i = 0; i < _countof(g_WICFormats); ++i)
    {
        if (memcmp(&g_WICFormats[i].wic, &guid, sizeof(GUID)) == 0)
            return g_WICFormats[i].format;
    }

    return DXGI_FORMAT_UNKNOWN;
}

//---------------------------------------------------------------------------------
static size_t _WICBitsPerPixel(REFGUID targetGuid)
{
    IWICImagingFactory* pWIC = _GetWIC();
    if (!pWIC)
        return 0;

    ScopedObject<IWICComponentInfo> cinfo;
    if (FAILED(pWIC->CreateComponentInfo(targetGuid, &cinfo)))
        return 0;

    WICComponentType type;
    if (FAILED(cinfo->GetComponentType(&type)))
        return 0;

    if (type != WICPixelFormat)
        return 0;

    ScopedObject<IWICPixelFormatInfo> pfinfo;
    if (FAILED(cinfo->QueryInterface(__uuidof(IWICPixelFormatInfo), reinterpret_cast<void**>(&pfinfo))))
        return 0;

    UINT bpp;
    if (FAILED(pfinfo->GetBitsPerPixel(&bpp)))
        return 0;

    return bpp;
}

//---------------------------------------------------------------------------------
static HRESULT CreateTextureFromWIC(_In_ IWICBitmapFrameDecode *frame,
                                    GLuint* outTexture,
                                    _In_ size_t maxsize)
{
    UINT width, height;
    HRESULT hr = frame->GetSize(&width, &height);
    if (FAILED(hr))
        return hr;

    assert(width > 0 && height > 0);

    maxsize = 4096;

    assert(maxsize > 0);

    UINT twidth, theight;
    if (width > maxsize || height > maxsize)
    {
        float ar = static_cast<float>(height) / static_cast<float>(width);
        if (width > height)
        {
            twidth = static_cast<UINT>(maxsize);
            theight = static_cast<UINT>(static_cast<float>(maxsize)* ar);
        }
        else
        {
            theight = static_cast<UINT>(maxsize);
            twidth = static_cast<UINT>(static_cast<float>(maxsize) / ar);
        }
        assert(twidth <= maxsize && theight <= maxsize);
    }
    else
    {
        twidth = width;
        theight = height;
    }

    // Determine format
    WICPixelFormatGUID pixelFormat;
    hr = frame->GetPixelFormat(&pixelFormat);
    if (FAILED(hr))
        return hr;

    WICPixelFormatGUID convertGUID;
    memcpy(&convertGUID, &pixelFormat, sizeof(WICPixelFormatGUID));

    size_t bpp = 0;

    DXGI_FORMAT format = _WICToDXGI(pixelFormat);
    if (format == DXGI_FORMAT_UNKNOWN)
    {
        for (size_t i = 0; i < _countof(g_WICConvert); ++i)
        {
            if (memcmp(&g_WICConvert[i].source, &pixelFormat, sizeof(WICPixelFormatGUID)) == 0)
            {
                memcpy(&convertGUID, &g_WICConvert[i].target, sizeof(WICPixelFormatGUID));

                format = _WICToDXGI(g_WICConvert[i].target);
                assert(format != DXGI_FORMAT_UNKNOWN);
                bpp = _WICBitsPerPixel(convertGUID);
                break;
            }
        }

        if (format == DXGI_FORMAT_UNKNOWN)
            return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);
    }
    else
    {
        bpp = _WICBitsPerPixel(pixelFormat);
    }

    if (!bpp)
        return E_FAIL;

    // Allocate temporary memory for image
    size_t rowPitch = (twidth * bpp + 7) / 8;
    size_t imageSize = rowPitch * theight;

    std::unique_ptr<uint8_t[]> temp(new uint8_t[imageSize]);

    // Load image data
    if (memcmp(&convertGUID, &pixelFormat, sizeof(GUID)) == 0
        && twidth == width
        && theight == height)
    {
        // No format conversion or resize needed
        hr = frame->CopyPixels(0, static_cast<UINT>(rowPitch), static_cast<UINT>(imageSize), temp.get());
        if (FAILED(hr))
            return hr;
    }
    else if (twidth != width || theight != height)
    {
        // Resize
        IWICImagingFactory* pWIC = _GetWIC();
        if (!pWIC)
            return E_NOINTERFACE;

        ScopedObject<IWICBitmapScaler> scaler;
        hr = pWIC->CreateBitmapScaler(&scaler);
        if (FAILED(hr))
            return hr;

        hr = scaler->Initialize(frame, twidth, theight, WICBitmapInterpolationModeFant);
        if (FAILED(hr))
            return hr;

        WICPixelFormatGUID pfScaler;
        hr = scaler->GetPixelFormat(&pfScaler);
        if (FAILED(hr))
            return hr;

        if (memcmp(&convertGUID, &pfScaler, sizeof(GUID)) == 0)
        {
            // No format conversion needed
            hr = scaler->CopyPixels(0, static_cast<UINT>(rowPitch), static_cast<UINT>(imageSize), temp.get());
            if (FAILED(hr))
                return hr;
        }
        else
        {
            ScopedObject<IWICFormatConverter> FC;
            hr = pWIC->CreateFormatConverter(&FC);
            if (FAILED(hr))
                return hr;

            hr = FC->Initialize(scaler.Get(), convertGUID, WICBitmapDitherTypeErrorDiffusion, 0, 0, WICBitmapPaletteTypeCustom);
            if (FAILED(hr))
                return hr;

            hr = FC->CopyPixels(0, static_cast<UINT>(rowPitch), static_cast<UINT>(imageSize), temp.get());
            if (FAILED(hr))
                return hr;
        }
    }
    else
    {
        // Format conversion but no resize
        IWICImagingFactory* pWIC = _GetWIC();
        if (!pWIC)
            return E_NOINTERFACE;

        ScopedObject<IWICFormatConverter> FC;
        hr = pWIC->CreateFormatConverter(&FC);
        if (FAILED(hr))
            return hr;

        hr = FC->Initialize(frame, convertGUID, WICBitmapDitherTypeErrorDiffusion, 0, 0, WICBitmapPaletteTypeCustom);
        if (FAILED(hr))
            return hr;

        hr = FC->CopyPixels(0, static_cast<UINT>(rowPitch), static_cast<UINT>(imageSize), temp.get());
        if (FAILED(hr))
            return hr;
    }

    GLuint gennedTexture;
    glGenTextures(1, &gennedTexture);
    glBindTexture(GL_TEXTURE_2D, gennedTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, twidth, theight, 0, GL_RGBA, GL_UNSIGNED_BYTE, temp.get());

    *outTexture = gennedTexture;

    return hr;
}

//--------------------------------------------------------------------------------------
HRESULT CreateWICTextureFromMemory(_In_bytecount_(wicDataSize) const uint8_t* wicData,
                                   _In_ size_t wicDataSize,
                                   GLuint* outTexture,
                                   _In_ size_t maxsize
                                   )
{
    if (!wicData)
    {
        return E_INVALIDARG;
    }

    if (!wicDataSize)
    {
        return E_FAIL;
    }

#ifdef _M_AMD64
    if (wicDataSize > 0xFFFFFFFF)
        return HRESULT_FROM_WIN32(ERROR_FILE_TOO_LARGE);
#endif

    IWICImagingFactory* pWIC = _GetWIC();
    if (!pWIC)
        return E_NOINTERFACE;

    // Create input stream for memory
    ScopedObject<IWICStream> stream;
    HRESULT hr = pWIC->CreateStream(&stream);
    if (FAILED(hr))
        return hr;

    hr = stream->InitializeFromMemory(const_cast<uint8_t*>(wicData), static_cast<DWORD>(wicDataSize));
    if (FAILED(hr))
        return hr;

    // Initialize WIC
    ScopedObject<IWICBitmapDecoder> decoder;
    hr = pWIC->CreateDecoderFromStream(stream.Get(), 0, WICDecodeMetadataCacheOnDemand, &decoder);
    if (FAILED(hr))
        return hr;

    ScopedObject<IWICBitmapFrameDecode> frame;
    hr = decoder->GetFrame(0, &frame);
    if (FAILED(hr))
        return hr;

    hr = CreateTextureFromWIC(frame.Get(), outTexture, maxsize);
    if (FAILED(hr))
        return hr;

    return hr;
}

//--------------------------------------------------------------------------------------
HRESULT CreateWICTextureFromFile(_In_z_ const wchar_t* fileName,
                                 GLuint* outTexture,
                                 _In_ size_t maxsize)
{
    IWICImagingFactory* pWIC = _GetWIC();
    if (!pWIC)
        return E_NOINTERFACE;

    // Initialize WIC
    ScopedObject<IWICBitmapDecoder> decoder;
    HRESULT hr = pWIC->CreateDecoderFromFilename(fileName, 0, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &decoder);
    if (FAILED(hr))
        return hr;

    ScopedObject<IWICBitmapFrameDecode> frame;
    hr = decoder->GetFrame(0, &frame);
    if (FAILED(hr))
        return hr;

    hr = CreateTextureFromWIC(frame.Get(), outTexture, maxsize);
    if (FAILED(hr))
        return hr;

#if defined(_DEBUG) || defined(PROFILE)
    if (*outTexture != 0)
    {
        CHAR strFileA[MAX_PATH];
        WideCharToMultiByte(CP_ACP,
                            WC_NO_BEST_FIT_CHARS,
                            fileName,
                            -1,
                            strFileA,
                            MAX_PATH,
                            nullptr,
                            FALSE
                            );
        const CHAR* pstrName = strrchr(strFileA, '\\');
        if (!pstrName)
        {
            pstrName = strFileA;
        }
        else
        {
            pstrName++;
        }
    }
#endif

    return hr;
}