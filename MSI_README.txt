Prerequisites:
 - WiX3 installed, with candle.exe and light.exe in %PATH%
 - Hooq build for both debug and release ("nmake all"), and installed into the build directory ("nmake install")

The following files should be copied to the build directory (if you're building in the source directory,
some of these will already be in place):
 - COPYING, COPYING.LGPL, COPYING.GPL, license.rtf
 - hooq.wxs
 - msi.nmake
 - C:\Qt\<VERSION>\bin -> qt
 - QtScriptGenerator\plugins\script -> script
 - hooq-release from "nmake install" of hooq
 - hooq-debug from "nmake install" of hooq
 - Microsoft_VC90_CRT_x86.msm
 - Microsoft_VC90_DebugCRT_x86.msm
 - policy_9_0_Microsoft_VC90_CRT_x86.msm
 - policy_9_0_Microsoft_VC90_DebugCRT_x86.msm

The .msm files are shipped with Visual Studio; they should be in one of the following directories:
- C:\Program Files (x86)\Common Files\Merge Modules
- C:\Program Files\Common Files\Merge Modules

To build:

nmake -f msi.nmake

You might get an error, but if you have 'hooq.msi' at the end, it's worked.