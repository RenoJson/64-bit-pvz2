#pragma once
#include <unistd.h>
#include <cstdlib>
#include <stdio.h>

extern uintptr_t g_libAddress;
// Get base address of a library (.so) loaded in memory.
uintptr_t GetLibraryAddress(const char* libName);
// Get actual offset of address inside libPVZ2.so
uintptr_t getActualOffset(uintptr_t offset);
// Get offset within libPVZ2.so from a running program
uintptr_t GetOriginalOffset(uintptr_t actualOffset);
// Get a ptr to a member of an object. You can use it to receive or set a member without a need to recreate the layout, especially if the origin is unknown.
uintptr_t* GetMemberPtr(void* obj, uintptr_t memberOffset);
// Hook a function in libPVZ2
void PVZ2HookFunction(uintptr_t offset, void* replace, void** result);
// Copy vftable from memory
void* CopyVFTable(uintptr_t vftableAddr, int64_t numVFuncs);
// Copy vftable from memory with destination
void CopyVFTable(void* dest, intptr_t vftableAddr, int64_t numVFuncsToCopy);
// Patch a virtual function without the need to hook it. Patched function will remain overriden only in its vftable
void PatchVFTable(void* vftable, void* funcAddr, int index);
// Create a deriven vftable. Often used in custom types
void* CreateChildVFTable(int64_t vFuncsCount, intptr_t parentVftable, int64_t nuMVFuncsToCopy);
// Set an RtClass to the vftable in a ctor
void SetVFTable(void* obj, uintptr_t newVftablePtr);
// Get a virtual function from vftable
void* GetVirtualFunc(void* obj, int index);
//Modify bytes in libPVZ2.so
bool ReplaceBytes(uintptr_t offset, const void* data, size_t size);
// Call a virtual function from libPVZ2.so. The vfunc index should be correct (ex: it usually should be divided by 4 in 32 bit and 8 in 64 bit)
template<typename R, typename... Args>
R CallVirtualFunc(void* obj, int index, Args... args)
{
    void* func = GetVirtualFunc(obj, index);
    auto castedFunc = reinterpret_cast<R(*)(Args...)>(func);
    return castedFunc(args...);
}

// Call a function from libPVZ2.so. It supports multiple parameters, so it's very flexible and shortens the time of decompiling functions
template<typename R, typename... Args>
R CallFunc(uintptr_t funcOffset, Args... args)
{
    typedef R(*func)(Args...);
    func castedFunc = (func)getActualOffset(funcOffset);
    return castedFunc(args...);
}