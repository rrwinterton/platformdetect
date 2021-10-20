#include <string>

#define MAX_GPUS 1  //TODO: need to test more than on GPU in system

//error codes
#define GPUINFO_SUCCESS 0
#define GPUINFO_FAILED_COINIT_MULTITHREADED -1
#define GPUINFO_FAILED_CoInitializeSecurity -2
#define GPUINFO_FAILED_CoCreateInstance -3
#define GPUINFO_FAILED_ConnectServer -4
#define GPUINFO_FAILED_CoSetProxyBlanket - 5
#define GPUINFO_FAILED_ExecQuery -6
#define GPUINFO_FAILED_RegOpenKey1 -7
#define GPUINFO_FAILED_RegOpenKey2 -8
#define GPUINFO_FAILED_RegOpenKey3 -9
#define GPUINFO_FAILED_RegOpenKey4 -10

//GPUInfo
class GPUInfo
{
public:
    GPUInfo();
    ~GPUInfo();

    const uint32_t NumGPUs() const { return mNumGPUs; }
    const std::string GPUName() const { return mGPUName; }
    const std::string PNPDeviceID() const { return mPNPDeviceID; }
    const std::string GPUDescription() const { return mGPUDescription; }
    const std::string InstalledDisplayDrivers() const { return mInstalledDisplayDrivers; }
    const std::string DeviceId() const { return mDeviceId; }
    const std::string VideoProcessor() const { return mVideoProcessor; }

private:
    //attributes for gpu
    DWORD mNumGPUs;
    std::string mDriverName[MAX_GPUS];
    int getNumGPUs(DWORD &mNumGPUs, std::string chDriverName[]);
    std::string mGPUName;
    std::string mPNPDeviceID;
    std::string mGPUDescription;
    std::string mInstalledDisplayDrivers;
    std::string mDeviceId;
    std::string mVideoProcessor;
};