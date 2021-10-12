
#include <algorithm>
#include <intrin.h>

#include "cpuinfo.h"

//cpuid instruction wrapper for intrinsic
class CPUID
{
    uint32_t regs[4];

public:
    explicit CPUID(unsigned funcId, unsigned subFuncId)
    {
        __cpuidex((int *)regs, (int)funcId, (int)subFuncId);
    }

    const uint32_t &eax() const { return regs[0]; }
    const uint32_t &ebx() const { return regs[1]; }
    const uint32_t &ecx() const { return regs[2]; }
    const uint32_t &edx() const { return regs[3]; }
};

//CPUInfo::CPUInfo constructor
//does all the work then just access member variables
CPUInfo::CPUInfo()
{
    mVendorId = "";
    mModelName = "";
    mNumSMT = 0;
    mNumCores = 0;
    mNumLogCpus = 0;
    mCPUMHz = 0;
    mHasHTT = false;
    mHasSSE = false;
    mHasSSE2 = false;
    mHasSSE3 = false;
    mHasSSE41 = false;
    mHasSSE42 = false;
    mHasAVX = false;
    mHasAVX2 = false;

    //vendor eax=0
    CPUID cpuID0(0, 0);
    uint32_t LC = cpuID0.eax();
    mVendorId += std::string((const char *)&cpuID0.ebx(), 4);
    mVendorId += std::string((const char *)&cpuID0.edx(), 4);
    mVendorId += std::string((const char *)&cpuID0.ecx(), 4);

    //SIMD instructions eax=1
    CPUID cpuID1(1, 0);
    mHasSSE = cpuID1.edx() & SSE_BIT;
    mHasSSE2 = cpuID1.edx() & SSE2_BIT;
    mHasSSE3 = cpuID1.ecx() & SSE3_BIT;
    mHasSSE41 = cpuID1.ecx() & SSE41_BIT;
    mHasSSE42 = cpuID1.ecx() & SSE41_BIT;
    mHasAVX = cpuID1.ecx() & AVX_BIT;

    //AVX2 and HTT instructions eax=7
    CPUID cpuID7(7, 0);
    mHasAVX2 = cpuID7.ebx() & AVX2_BIT;
    if (cpuID1.edx() & AVX_BIT)
    {
        mHasHTT = true;
    }
    else
    {
        mHasHTT = false;
    }

    std::string VenderId = mVendorId;
    for_each(VenderId.begin(), VenderId.end(), [](char &in)
             { in = ::toupper(in); });
    //number of cores
    if (VenderId.find("INTEL") != std::string::npos)
    {
        if (LC >= 11)
        {
            for (int level = 0; level < MAX_INTEL_TOP_LEVEL; ++level)
            {
                CPUID cpuID4(0x0B, level);
                uint32_t currLevel = (LEVEL_TYPE & cpuID4.ecx()) >> 8;
                switch (currLevel)
                {
                case 0x01:
                    mNumSMT = LEVEL_CORES & cpuID4.ebx();
                    break;
                case 0x02:
                    mNumLogCpus = LEVEL_CORES & cpuID4.ebx();
                    break;
                default:
                    break;
                }
            }
            mNumCores = mNumLogCpus / mNumSMT;
        }
        else
        {
            if (LC >= 1)
            {
                mNumLogCpus = (cpuID1.ebx() >> 16) & 0xFF;
                if (LC >= 4)
                {
                    mNumCores = 1 + (CPUID(4, 0).eax() >> 26) & 0x3F;
                }
            }
            if (mHasHTT)
            {
                if (!(mNumCores > 1))
                {
                    mNumCores = 1;
                    mNumLogCpus = (mNumLogCpus >= 2 ? mNumLogCpus : 2);
                }
            }
            else
            {
                mNumCores = mNumLogCpus = 1;
            }
        }

        //processor brand string
        for (int i = 0x80000002; i < 0x80000005; ++i)
        {
            CPUID cpuID(i, 0);
            mModelName += std::string((const char *)&cpuID.eax(), 4);
            mModelName += std::string((const char *)&cpuID.ebx(), 4);
            mModelName += std::string((const char *)&cpuID.ecx(), 4);
            mModelName += std::string((const char *)&cpuID.edx(), 4);
        }
    }
    else
    {
        mModelName = "non Intel";
    }
}

//CPUInfo::~CPUInfo destructor
CPUInfo::~CPUInfo()
{
}
