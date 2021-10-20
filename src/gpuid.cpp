#include <windows.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <dxgi.h>
#include "gpuid.h"

GPUId::GPUId() {
}

GPUId::~GPUId() {
}

// GPUId::readPCIFile
int GPUId::readPCIFile(std::string fileID, int &numIds, std::vector<uint32_t> &PCIIds)
{
    std::ifstream inFile;
    int deviceCount = 0;
    std::string x;
    std::string fileName;

    fileName = "gen" + fileID + ".txt";

    inFile.open(fileName);
    if (!inFile)
    {
        return -1; // terminate with error
    }

    deviceCount = 0;
    while (inFile >> x)
    {
        PCIIds.push_back(std::stol(x, nullptr, 16));
        deviceCount++;
    }
    inFile.close();
    numIds = deviceCount;
    return 0;
}

int GPUId::findDeviceIds(std::vector<uint32_t> PCIIds, std::vector<uint32_t> &foundPCIIds)
{
    IDXGIAdapter *pAdapter;
    int adapterIndex = 0;
    bool hasIGPU = false;
    bool hasDGPU = false;
    bool isDG1Primary = false;
    HMODULE hDXGI = LoadLibrary(L"dxgi.dll");
    if (hDXGI == NULL)
    {
        return false;
    }

    typedef HRESULT(WINAPI * LPCREATEDXGIFACTORY)(REFIID riid,
                                                  void **ppFactory);

    LPCREATEDXGIFACTORY pCreateDXGIFactory =
        (LPCREATEDXGIFACTORY)GetProcAddress(hDXGI, "CreateDXGIFactory1");
    if (pCreateDXGIFactory == NULL)
    {
        pCreateDXGIFactory = (LPCREATEDXGIFACTORY)GetProcAddress(hDXGI, "CreateDXGIFactory");

        if (pCreateDXGIFactory == NULL)
        {
            FreeLibrary(hDXGI);
            return false;
        }
    }

    IDXGIFactory *pFactory = NULL;
    if (FAILED((*pCreateDXGIFactory)(__uuidof(IDXGIFactory),
                                     (void **)(&pFactory))))
    {
        FreeLibrary(hDXGI);
        return false;
    }


    while (SUCCEEDED(pFactory->EnumAdapters(adapterIndex, &pAdapter)))
    {
        DXGI_ADAPTER_DESC AdapterDesc = {};
        if (SUCCEEDED(pAdapter->GetDesc(&AdapterDesc)))
        {
            if (AdapterDesc.VendorId == 0x8086)
            {
                if (AdapterDesc.DedicatedVideoMemory <= 512 * 1024 * 1024)
                {
                    hasIGPU = true;
                }
                else
                {
                    // TODO: add in discreate card information
                    hasDGPU = true;
                }

                for (auto i = 0; i < (int)PCIIds.size(); i++)
                {
                    if (AdapterDesc.DeviceId == PCIIds[i])
                    {
                        foundPCIIds.push_back(AdapterDesc.DeviceId);
                    }
                }
            }
        }
        adapterIndex++;
        pAdapter->Release();
    }
    pFactory->Release();
    FreeLibrary(hDXGI);
    return hasIGPU && hasDGPU && isDG1Primary;
}
