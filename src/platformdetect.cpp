//platformdetect.cpp
//this is a console test application for cpuinfo and gpuinfo
//look for TODO: in testing

//includes
#include <windows.h>
#include <dxgi.h>

//cpuinfo and gpuinfo includes
#include <vector>
#include <iostream>
#include <fstream>
#include <string>

#include "gpuid.h"
#include "cpuinfo.h"
#include "gpuinfo.h"

using namespace std; //for cout debug only

//main for platform detect
int main(int argc, char *argv[])
{
    GPUId cGPUId;
    CPUInfo cCPUInfo;
    GPUInfo cGPUInfo;
    int numberOfGPUs;
    std::vector<uint32_t> PCIIds;
    std::vector<uint32_t> foundGenPCIIds;
    int numIds;
    uint32_t numGenGfx;

    //TODO: rrw this code needs to be tested for multiple gpu platforms
    //begin code to be used as sample code
    std::cout << "Gen 10 Detection" << endl;
    cGPUId.readPCIFile("10", numIds, PCIIds);
    if (0 == PCIIds.size()) {
        cout << "No PCI ID file." << endl;
    }
    else {
        numberOfGPUs = cGPUInfo.NumGPUs();
        cGPUId.findDeviceIds(PCIIds, foundGenPCIIds);
    }

    numGenGfx = (uint32_t) foundGenPCIIds.size();
    if (0 != foundGenPCIIds.size()) {
        numIds = 1;
        cout << "Using Gen 10 Graphics" << endl;
        for (auto itr = foundGenPCIIds.begin(); itr < foundGenPCIIds.end(); itr++) {
            cout << numIds << ":  " << std::hex << *itr << endl;
        }
        cout << endl;
    }
    else {
        cout << "No Gen 10 Graphics Detected" << endl;
    }
    PCIIds.clear();
    foundGenPCIIds.clear();
    
    std::cout << "Gen 11 Detection" << endl;
    cGPUId.readPCIFile("11", numIds, PCIIds);
    if (0 == PCIIds.size()) {
        cout << "No PCI ID file." << endl;
    }
    numberOfGPUs = cGPUInfo.NumGPUs();
    cGPUId.findDeviceIds(PCIIds, foundGenPCIIds);

    numGenGfx = (uint32_t) foundGenPCIIds.size();
    if (0 != foundGenPCIIds.size()) {
        numIds = 1;
        cout << "Using Gen 11 Graphics" << endl;
        for (auto itr = foundGenPCIIds.begin(); itr < foundGenPCIIds.end(); itr++) {
            cout << numIds << ":  " << std::hex << *itr << endl;
        }
        cout << endl;
    }
    else {
        cout << "No Gen 11 Graphics Detected" << endl;
    }

    PCIIds.clear();
    foundGenPCIIds.clear();

    //end code to be used as sample code

    //debugging unit test information
    //print out cpu information
    cout << "*** Below Information is to be used for debug only ***" << endl;
    cout << "(CPU Info)" << endl;
    cout << "CPU vendor: " << cCPUInfo.vendor() << endl;
    cout << "CPU string: " << cCPUInfo.model() << endl;
    cout << "number of cores: " << cCPUInfo.cores() << endl;
    cout << "number of logical cores: " << cCPUInfo.logicalCpus() << endl;
    cout << "HT Technology: " << cCPUInfo.hasHTT() << endl;
    cout << "SSE: " << cCPUInfo.hasSSE() << endl;
    cout << "SSE2: " << cCPUInfo.hasSSE2() << endl;
    cout << "SSE3: " << cCPUInfo.hasSSE3() << endl;
    cout << "SSE41: " << cCPUInfo.hasSSE41() << endl;
    cout << "SSE42: " << cCPUInfo.hasSSE42() << endl;
    cout << "AVX: " << cCPUInfo.hasAVX() << endl;
    cout << "AVX2: " << cCPUInfo.hasAVX2() << endl;
    cout << endl;

    //print out gpu information
    cout << "(GPU Info)" << endl;
    //TODO: haven't tested multiple gpu's yet
    if (1 == cGPUInfo.NumGPUs())
    {
        //        for (int i = 0; i < numGPU; i++) {
        //            cout << i << ". Driver Names = " << DriverNames[i] << endl;
        //        }
        cout << "Number of gpus: " << cGPUInfo.NumGPUs() << endl;
        //        cout << "Driver Name " << cGPUInfo.DriverNames[0] << endl;
        cout << "GPU Name: " << cGPUInfo.GPUName() << endl;
        cout << "PNP Device ID: " << cGPUInfo.PNPDeviceID() << endl;
        cout << "GPU Description: " << cGPUInfo.GPUDescription() << endl;
        cout << "GPU InstalledDisplayDrivers: " << cGPUInfo.InstalledDisplayDrivers() << endl;
        cout << "GPU DeviceId: " << cGPUInfo.DeviceId() << endl;
        cout << "Video Processor String: " << cGPUInfo.VideoProcessor() << endl;
    }
    cout << endl;

    return GPUINFO_SUCCESS;
}
