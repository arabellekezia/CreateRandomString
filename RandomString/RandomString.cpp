#include "..\CreateRandomString\pch.h" // Use stdafx.h in Visual Studio 2017 and earlier

#include "..\CreateRandomString\CreateRandomString.h"
#include "..\CreateRandomString\CreateRandomString_i.c"

#include <wrl.h>

using namespace Microsoft::WRL;

// Prints an error string for the provided source code line and HRESULT
// value and returns the HRESULT value as an int.
int PrintError(unsigned int line, HRESULT hr)
{
    wprintf_s(L"ERROR: Line:%d HRESULT: 0x%X\n", line, hr);
    return hr;
}

int wmain()
{
    HRESULT hr = 0;

    // RAII wrapper for managing the lifetime of the COM library.
    class CoInitializeWrapper
    {
        HRESULT _hr;
    public:
        CoInitializeWrapper(DWORD flags)
        {
            _hr = CoInitializeEx(nullptr, flags);
        }
        ~CoInitializeWrapper()
        {
            if (SUCCEEDED(_hr))

            {
                CoUninitialize();
            }
        }
        operator HRESULT()
        {
            return _hr;
        }

    };

    // Initialize the COM library.
    CoInitializeWrapper initialize(COINIT_APARTMENTTHREADED);
    if (FAILED(initialize))
    {
        return PrintError(__LINE__, initialize);
    }

    ComPtr<ICreateRandomString> rand; // Interface to COM component.

    hr = CoCreateInstance(CLSID_CreateRandomString, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(rand.GetAddressOf()));
    if (SUCCEEDED(hr))
    {
        BYTE* data = nullptr;
        wchar_t* out = nullptr;
        hr = rand->CreateString(16, data, out);
        if (FAILED(hr))
        {
            return PrintError(__LINE__, hr);
        }
        wprintf_s(out);
    }
    else
    {
        // Object creation failed. Print a message.
        return PrintError(__LINE__, hr);
    }

    return 0;
}