// nativearch.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <stdio.h>
#include <windows.h>

enum {
	WINARCH_unknown,
	WINARCH_x86,
	WINARCH_x64,
	WINARCH_Itanium,
	WINARCH_Win10ARM64, // 不能运行x64和ARM64EC
	WINARCH_ARM64
};

int GetNativeArch(void)
{
	typedef BOOL(WINAPI* PfnIsWow64Process2)(HANDLE hProcess, USHORT* pProcessMachine, USHORT* pNativeMachine);
	HMODULE hkernel32 = GetModuleHandleW(L"kernel32");
	PfnIsWow64Process2 pfnIsWow64Process2 = (PfnIsWow64Process2)GetProcAddress(hkernel32, "IsWow64Process2");
	USHORT wow64 = 0, native = 0;
	SYSTEM_INFO si = { 0 };
	if (pfnIsWow64Process2)
	{
		pfnIsWow64Process2(GetCurrentProcess(), &wow64, &native);
		switch (native)
		{
		case IMAGE_FILE_MACHINE_I386: return WINARCH_x86;
		case IMAGE_FILE_MACHINE_AMD64: return WINARCH_x64;
		case IMAGE_FILE_MACHINE_ARM64:
			if (GetProcAddress(hkernel32, "GetMachineTypeAttributes") != NULL)
				return WINARCH_ARM64;
			else
				return WINARCH_Win10ARM64;
		default: return WINARCH_unknown;
		}
	}
	else
	{
		GetNativeSystemInfo(&si);
		switch (si.wProcessorArchitecture)
		{
		case PROCESSOR_ARCHITECTURE_INTEL: return WINARCH_x86;
		case PROCESSOR_ARCHITECTURE_AMD64: return WINARCH_x64;
		case PROCESSOR_ARCHITECTURE_IA64: return WINARCH_Itanium;
		default: return WINARCH_unknown;
		}
	}
}

int main()
{
	int nativearch = GetNativeArch();
	switch (nativearch)
	{
	case WINARCH_x86: printf("Windows x86 32位 支持x86程序\n"); break;
	case WINARCH_x64: printf("Windows x64 64位 支持x86、x64程序\n"); break;
	case WINARCH_Itanium: printf("Windows Itanium 64位 支持x86、Itanium程序\n"); break;
	case WINARCH_Win10ARM64: printf("Windows 10 ARM64 64位 支持x86、ARMNT、ARM64程序\n"); break;
	case WINARCH_ARM64: printf("Windows 11+ ARM64 64位 支持x86、x64、ARMNT、ARM64、ARM64EC（x64兼容的ARM64）、ARM64X（ARM64+ARM64EC复合）程序\n"); break;
	default: printf("未知Windows平台\n"); break;
	}
	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
