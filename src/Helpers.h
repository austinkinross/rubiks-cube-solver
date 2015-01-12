#pragma once

#ifdef WIN32

#include <wrl/client.h>

inline void ThrowIfFailed(HRESULT hr)
{
    if (FAILED(hr))
    {
        // Set a breakpoint on this line to catch Win32 API errors.
        throw Platform::Exception::CreateException(hr);
    }
}

#endif