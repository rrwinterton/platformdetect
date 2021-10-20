#include <comdef.h>
#include <Wbemidl.h>
#include <tchar.h>
#include <iostream>

#define _WIN32_DCOM
#pragma comment(lib, "wbemuuid.lib")

#include "gpuinfo.h"

//GPUInfo::GPUInfo() (constructor) 
//does all the work then just access member variables as constants
GPUInfo::GPUInfo()
{
    HRESULT hr;
    IWbemClassObject *pclsObj = NULL;
    IEnumWbemClassObject *pEnumerator = NULL;
    IWbemServices *pSvc = NULL;
    IWbemLocator *pLoc = NULL;
    ULONG uReturn = 0;
    int errorCode = 0;
    VARIANT vtProp;

    //initalize all private member variables
    mNumGPUs = 0;
    mGPUName = "";
    mGPUDescription = "";
    mInstalledDisplayDrivers = "";
    mDeviceId = "";
    mVideoProcessor = "";


    //TODO: determine tne number of gpus need to test more than 1 GPU
    errorCode = getNumGPUs(mNumGPUs, mDriverName);
    if (errorCode != GPUINFO_SUCCESS) {
      goto Egress;
    }    
    //initialize COM
    hr = CoInitializeEx(0, COINIT_MULTITHREADED);
    if (FAILED(hr))
    {
        errorCode = GPUINFO_FAILED_COINIT_MULTITHREADED;
        goto Egress;
    }

    //set general COM security
    hr = CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT,
                              RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL);

    if (FAILED(hr))
    {
        errorCode = GPUINFO_FAILED_CoInitializeSecurity;
        goto Egress;
    }

    //initialize locator to WMI
    hr = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER,
                          IID_IWbemLocator, (LPVOID *)&pLoc);

    if (FAILED(hr))
    {
        errorCode = GPUINFO_FAILED_CoCreateInstance;
        goto Egress;
    }

    // connect WMI - connect root\cimv2 namespace
    hr = pLoc->ConnectServer(_bstr_t(L"ROOT\\CIMV2"), NULL, NULL, 0, NULL,
                             0, 0, &pSvc);

    if (FAILED(hr))
    {
        errorCode = GPUINFO_FAILED_ConnectServer;
        goto Egress;
    }

    // set proxy security levels
    hr = CoSetProxyBlanket(pSvc, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE,
                           NULL, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE);

    if (FAILED(hr))
    {
        errorCode = GPUINFO_FAILED_CoSetProxyBlanket;
        goto Egress;
    }

    // get gfx information
    hr = pSvc->ExecQuery(bstr_t("WQL"), bstr_t("SELECT * FROM Win32_VideoController"),
                         WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL, &pEnumerator);

    if (FAILED(hr))
    {
        errorCode = GPUINFO_FAILED_ExecQuery;
        goto Egress;
    }

    while (pEnumerator)
    {
        hr = pEnumerator->Next(WBEM_INFINITE, 1,
                               &pclsObj, &uReturn);

        if (0 == uReturn)
        {
            break;
        }

        //get name property
        vtProp = {0};
        hr = pclsObj->Get(L"Name", 0, &vtProp, 0, 0);
        if (vtProp.bstrVal != nullptr)
        {
            mGPUName = _bstr_t(vtProp.bstrVal, false);
        }
        VariantClear(&vtProp);

        vtProp = { 0 };
        hr = pclsObj->Get(L"PNPDeviceID", 0, &vtProp, 0, 0);
        if (vtProp.bstrVal != nullptr)
        {
            mPNPDeviceID = _bstr_t(vtProp.bstrVal, false);
        }
        VariantClear(&vtProp);


        //get InstalledDisplayDrivers property
        hr = pclsObj->Get(L"InstalledDisplayDrivers", 0, &vtProp, 0, 0);
        if (vtProp.bstrVal != nullptr)
        {
            mInstalledDisplayDrivers = _bstr_t(vtProp.bstrVal, false);
        }
        VariantClear(&vtProp);

        //get Description property
        hr = pclsObj->Get(L"Description", 0, &vtProp, 0, 0);
        if (vtProp.bstrVal != nullptr)
        {
            mGPUDescription = _bstr_t(vtProp.bstrVal, false);
        }
        VariantClear(&vtProp);

        //get DeviceID property
        hr = pclsObj->Get(L"DeviceID", 0, &vtProp, 0, 0);
        if (vtProp.bstrVal != nullptr)
        {
            mDeviceId = _bstr_t(vtProp.bstrVal, false);
        }
        VariantClear(&vtProp);

        //get VideoProcessor property
        hr = pclsObj->Get(L"VideoProcessor", 0, &vtProp, 0, 0);
        if (vtProp.bstrVal != nullptr)
        {
            mVideoProcessor = _bstr_t(vtProp.bstrVal, false);
        }
        VariantClear(&vtProp);

        pclsObj->Release();
    }

Egress:
    switch (errorCode)
    {
    case GPUINFO_SUCCESS:
    case GPUINFO_FAILED_ExecQuery:
    case GPUINFO_FAILED_CoSetProxyBlanket:
        pSvc->Release();
    case GPUINFO_FAILED_ConnectServer:
        pLoc->Release();
    case GPUINFO_FAILED_CoCreateInstance:
        CoUninitialize();
        break;
    //all other GPUINFO fails
    default:
        break;
    }
    return;
}

//GPUInfo::~GPUInfo() (destructor)
GPUInfo::~GPUInfo()
{
}

//GPUInfo::getNumGPUs (private)
#pragma warning( disable : 4244 )
int GPUInfo::getNumGPUs(DWORD &Num, std::string DriverName[])
{
    HKEY keyServ;
    HKEY keyEnum;
    HKEY key;
    HKEY key2;
    LONG lResult;
    int i = 0;
    int count = 0;
    DWORD size = 0;
    DWORD type = 0;
    int errCode = 0;

    //query "SYSTEM\\CurrentControlSet\Services" subkeys
    lResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("SYSTEM\\CurrentControlSet\\Services"), 0, KEY_READ, &keyServ);
    if (ERROR_SUCCESS != lResult) {
        errCode = GPUINFO_FAILED_RegOpenKey1;
        goto Egress;
    }

    //query "SYSTEM\\CurrentControlSet\\Enum" subkeys
    lResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("SYSTEM\\CurrentControlSet\\Enum"), 0, KEY_READ, &keyEnum);
    if (ERROR_SUCCESS != lResult) {
        RegCloseKey(keyServ);
        errCode = GPUINFO_FAILED_RegOpenKey2;
        goto Egress;
    }

    for (;; ++i)
    {
        //TODO: need to test multiple gpus may need sleep(1)
        size = 512;
        TCHAR name[512] = {0};

        //enumerate each subitem
        lResult = RegEnumKeyEx(keyServ, i, name, &size, NULL, NULL, NULL, NULL);

        //if subitems does not exist break;
        if (lResult == ERROR_NO_MORE_ITEMS)
            break;

        //save the value of the field identified by name to key
        lResult = RegOpenKeyEx(keyServ, name, 0, KEY_READ, &key);
        if (lResult != ERROR_SUCCESS)
        {
            RegCloseKey(keyServ);
            errCode = GPUINFO_FAILED_RegOpenKey3;
            goto Egress;
        }

        size = 512;
        //query key is the subkey field name of group
        lResult = RegQueryValueEx(key, TEXT("Group"), 0, &type, (LPBYTE)name, &size);
        if (lResult == ERROR_FILE_NOT_FOUND)
        {
            //key does not exist
            RegCloseKey(key);
            continue;
        };

        //query is not video, the key is not a graphics card driver.
        if (_tcscmp(TEXT("Video"), name) != 0)
        {
            RegCloseKey(key);
            continue; //look for more
        };

        //if continues to execute, information about the graphics card found
        lResult = RegOpenKeyEx(key, TEXT("Enum"), 0, KEY_READ, &key2);
        RegCloseKey(key);
        key = key2;
        size = sizeof(count);
        //query Count field for the number of graphics cards
        lResult = RegQueryValueEx(key, TEXT("Count"), 0, &type, (LPBYTE)&count, &size);

        Num = count; //number of graphics cards found
        if (5 < count)
        {
            goto Egress;
        }
        for (int j = 0; j < count; ++j)
        {
            TCHAR sz[512] = {0};
            TCHAR name[64] = {0};
            wsprintf(name, TEXT("%d"), j);
            size = sizeof(sz);
            lResult = RegQueryValueEx(key, name, 0, &type, (LPBYTE)sz, &size);

            lResult = RegOpenKeyEx(keyEnum, sz, 0, KEY_READ, &key2);
            if (ERROR_SUCCESS)
            {
                RegCloseKey(keyEnum);
                errCode = GPUINFO_FAILED_RegOpenKey4;
                goto Egress;
            }

            size = sizeof(sz);
            lResult = RegQueryValueEx(key2, TEXT("FriendlyName"), 0, &type, (LPBYTE)sz, &size);
            if (lResult == ERROR_FILE_NOT_FOUND)
            {
                size = sizeof(sz);
                lResult = RegQueryValueEx(key2, TEXT("DeviceDesc"), 0, &type, (LPBYTE)sz, &size);
                TCHAR *ptrString = sz;
                DriverName[j] = *ptrString;
            };
            RegCloseKey(key2);
            key2 = NULL;
        };
        RegCloseKey(key);
        key = NULL;
        break;
    }

    //success
    errCode = GPUINFO_SUCCESS;

Egress:
    switch (errCode) {
        case GPUINFO_FAILED_RegOpenKey1:
        case GPUINFO_FAILED_RegOpenKey2:
        case GPUINFO_FAILED_RegOpenKey3:
        case GPUINFO_FAILED_RegOpenKey4:
        default:
            break;
    }
    return errCode;
}
