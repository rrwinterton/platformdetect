// GPUInfo
#include <windows.h>
#include <string>
#include <vector>

//typedefs
typedef HRESULT(WINAPI * LPCREATEDXGIFACTORY)(REFIID riid, void **ppFactory);

// class GPUId
class GPUId
{
public:
    GPUId();
    ~GPUId();
    int readPCIFile(std::string fileID, int &numIds, std::vector<uint32_t> &PCIIds);
    int findDeviceIds(std::vector<uint32_t> PCIIds, std::vector<uint32_t> &foundPCIIds);

private:
};