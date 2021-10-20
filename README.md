# Platform Detect (platformdetect)
Detects CPU ISA and GPU information

This project can be used to show how to detect specific ISA capabilities from the GPU as well as the CPUID string as well as GPU and Graphics Driver Information.

The following is the executed 64 bit binary output when running the unit test "platformdetect.exe".  The top of the output is the PCI ID that is for the platform presented below in the debug information and unit testing.  

![alt output](platformdetect.png)

# File Descriptions
README.md - this file <br>
platformdetect.vcxproj.filters - msvs (2019) file filters <br>
platformdetect.vcxproj - msvs (2019) project file <br>
platformdetect.png - example output <br>
./x64/Release/gen11.txt - file containing some of the gen 11 PCI ID's (TODO: rrw this needs to be completed) <br>
./x64/Release/platformdetect.exe - windows 64 bit example binary <br>
./src/cpuinfo.cpp - debug cpu information collection implementation <br>
./src/cpuinfo.h - debug cpu information collection class <br>
./src/gpuid.cpp - gpu pci id collection implementation <br>
./src/gpuid.h - gpu pci id collection class <br>
./src/gpuinfo.cpp - debug gpu information collection implementation <br>
./src/gpuinfo.h - debug gpu information collection class <br>
./src/platformdetect.cpp - unit test example code <br>