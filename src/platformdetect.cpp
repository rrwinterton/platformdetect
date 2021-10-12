//platformdetect.cpp
//this is a console test application for cpuinfo and gpuinfo
//look for TODO: in testing

//includes
#include <windows.h>
#include <iostream>

//cpuinfo and gpuinfo includes
#include "cpuinfo.h"
#include "gpuinfo.h"

using namespace std; //for cout

//main for platform detect
int main(int argc, char *argv[])
{
    CPUInfo cCPUInfo;
    GPUInfo cGPUInfo;

    //print out cpu information
    cout << "*** CPU Info ***" << endl;
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
    cout << "*** GPU Info ***" << endl;
    //TODO: haven't tested multiple gpu's yet
    if (1 == cGPUInfo.NumGPUs())
    {
        //        for (int i = 0; i < numGPU; i++) {
        //            cout << i << ". Driver Names = " << DriverNames[i] << endl;
        //        }
        cout << "Number of gpus: " << cGPUInfo.NumGPUs() << endl;
        //        cout << "Driver Name " << cGPUInfo.DriverNames[0] << endl;
        cout << "GPU Name: " << cGPUInfo.GPUName() << endl;
        cout << "GPU Description: " << cGPUInfo.GPUDescription() << endl;
        cout << "GPU InstalledDisplayDrivers: " << cGPUInfo.InstalledDisplayDrivers() << endl;
        cout << "GPU DeviceId: " << cGPUInfo.DeviceId() << endl;
        cout << "Video Processor String: " << cGPUInfo.VideoProcessor() << endl;
    }
    cout << endl;

    return GPUINFO_SUCCESS;
}
