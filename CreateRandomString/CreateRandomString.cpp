#include "pch.h"
#include "CreateRandomString.h"
#include <stdio.h>
#include <wincrypt.h>
#include <wrl.h>

using namespace Microsoft::WRL;


class CreateRandomString : public RuntimeClass<RuntimeClassFlags<ClassicCom>, ICreateRandomString>
{
public:
    CreateRandomString()
    {
    }

    STDMETHODIMP CreateString(_In_ int size, _Out_ BYTE* buffer, _Out_ wchar_t* buffOut)
    {
        HCRYPTPROV hCryptProv = 0;
        if (CryptGenRandom(hCryptProv, size, buffer)) 
        {
            LPWSTR buffOut = nullptr;
            DWORD buffSize = 0;

            // Get hex string size
            CryptBinaryToStringW(buffer, size, CRYPT_STRING_HEX, nullptr, &buffSize);
            buffOut = new wchar_t[buffSize];

            // Fill the buffer
            if (CryptBinaryToStringW(buffer, size, CRYPT_STRING_HEX, buffOut, &buffSize)){
                printf("Random string generated\n");
                return S_OK;
            }
            else {
                printf("Error during CryptBinaryToStringW\n");
                exit(1);
            }
        }

        else {
            printf("Error during CryptGenRandom\n");
            exit(1);
        }
    }
};

CoCreatableClass(CreateRandomString);