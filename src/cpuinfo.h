#include <string>

//defines
#define MAX_INTEL_TOP_LEVEL 4

//typedefs
typedef unsigned __int32 uint32_t;
typedef unsigned __int64 uint64_t;

//CPUInfo
class CPUInfo
{
public:
    CPUInfo();
    ~CPUInfo();
    const std::string vendor() const { return mVendorId; }
    const std::string model() const { return mModelName; }
    const int cores() const { return mNumCores; }
    const int logicalCpus() const { return mNumLogCpus; }
    const float cpuFreqInMHz() const { return mCPUMHz; }
    const bool hasHTT() const { return mHasHTT; }
    const bool hasSSE() const { return mHasSSE; }
    const bool hasSSE2() const { return mHasSSE2; }
    const bool hasSSE3() const { return mHasSSE3; }
    const bool hasSSE41() const { return mHasSSE41; }
    const bool hasSSE42() const { return mHasSSE42; }
    const bool hasAVX() const { return mHasAVX; }
    const bool hasAVX2() const { return mHasAVX2; }

private:
    // bit masks for data extractions
    static const uint32_t SSE_BIT = 0x02000000;
    static const uint32_t SSE2_BIT = 0x04000000;
    static const uint32_t SSE3_BIT = 0x00000001;
    static const uint32_t SSE41_BIT = 0x00080000;
    static const uint32_t SSE42_BIT = 0x00100000;
    static const uint32_t AVX_BIT = 0x10000000;
    static const uint32_t AVX2_BIT = 0x00000020;
    static const uint32_t LEVEL_NUM = 0x000000FF;
    static const uint32_t LEVEL_TYPE = 0x0000FF00;
    static const uint32_t LEVEL_CORES = 0x0000FFFF;

    // attributes for cpu
    std::string mVendorId;
    std::string mModelName;
    int mNumSMT;
    int mNumCores;
    int mNumLogCpus;
    float mCPUMHz;
    bool mHasHTT;
    bool mHasSSE;
    bool mHasSSE2;
    bool mHasSSE3;
    bool mHasSSE41;
    bool mHasSSE42;
    bool mHasAVX;
    bool mHasAVX2;
};
