#include "MemUtils.h"

#include <And64InlineHook.hpp>
#include <sys/mman.h>

extern "C" void __clear_cache(void* beg, void* end);

uintptr_t g_libAddress = NULL;

uintptr_t GetLibraryAddress(const char* libName) {
    FILE* fp = fopen("/proc/self/maps", "rt");
    if (fp == nullptr) {
        perror("fopen");
        return 0;
    }

    uintptr_t addr = 0;
    char line[1024];

    while (fgets(line, sizeof(line), fp) != nullptr) {
        if (strstr(line, libName) != nullptr) {
            addr = strtoul(line, nullptr, 16);
            break;
        }
    }

    fclose(fp);
    return addr;
}


uintptr_t getActualOffset(uintptr_t offset)
{
    if (g_libAddress == 0)
    {
        g_libAddress = GetLibraryAddress("libPVZ2.so");
    }
    return g_libAddress + offset;
}

uintptr_t GetOriginalOffset(uintptr_t actualOffset) {
    if (g_libAddress == 0)
    {
        g_libAddress = GetLibraryAddress("libPVZ2.so");
    }
    return actualOffset - g_libAddress;
}

struct BacktraceState {
    void** current;
    void** end;
};

static _Unwind_Reason_Code unwindCallback(struct _Unwind_Context* context, void* arg) {
    BacktraceState* state = static_cast<BacktraceState*>(arg);
    uintptr_t pc = _Unwind_GetIP(context);

    if (pc) {
        if (state->current == state->end) {
            return _URC_END_OF_STACK;
        }
        else {
            *state->current++ = reinterpret_cast<void*>(pc);
        }
    }
    return _URC_NO_REASON;
}

void LogCallStack(int maxDepth) {
    void* buffer[30];
    int actualDepth = (maxDepth > 30) ? 30 : maxDepth;

    BacktraceState state = { buffer, buffer + actualDepth };
    _Unwind_Backtrace(unwindCallback, &state);

    int count = state.current - buffer; 

    LOGI("[TRACE] ====== BAT DAU TRUY VET CALL STACK (%d LOP) ======", count);

    for (int i = 0; i < count; ++i) {
        uintptr_t actualAddr = (uintptr_t)buffer[i];

        uintptr_t idaOffset = GetOriginalOffset(actualAddr);

        Dl_info info;
        const char* libName = "Unknown";
        if (dladdr((void*)actualAddr, &info) && info.dli_fname) {
            libName = info.dli_fname;
        }

        LOGI("[TRACE] Lop [%d]: Offset IDA = 0x%lX | (Lib: %s)", i, idaOffset, libName);
    }
    LOGI("[TRACE] ==================================================");
}

uintptr_t* GetMemberPtr(void* obj, uintptr_t memberOffset)
{
    uintptr_t member = *(uintptr_t*)(*(int64_t*)obj + memberOffset);
    return &member;
}

void PVZ2HookFunction(uintptr_t offset, void* replace, void** result)
{
    A64HookFunction((void*)getActualOffset(offset), replace, result);
}

void* CopyVFTable(uintptr_t vftableAddr, int64_t numVFuncs)
{
    int64_t size = numVFuncs * sizeof(int64_t);
    void* vftableCopy = malloc(size);
    memcpy(vftableCopy, (const void*)vftableAddr, size);
    return vftableCopy;
}

void CopyVFTable(void* dest, intptr_t vftableAddr, int64_t numVFuncsToCopy)
{
    int64_t size = numVFuncsToCopy * sizeof(int64_t);
    memcpy(dest, (const void*)vftableAddr, size);
}

void PatchVFTable(void* vftable, void* funcAddr, int index)
{
    ((reinterpret_cast<void**>(vftable))[index]) = funcAddr;
}

void* CreateChildVFTable(int64_t vFuncsCount, intptr_t parentVftable, int64_t nuMVFuncsToCopy)
{
    int64_t size = vFuncsCount * sizeof(int64_t);
    void* childVftable = malloc(size);
    CopyVFTable(childVftable, parentVftable, nuMVFuncsToCopy);

    return childVftable;
}

void SetVFTable(void* obj, uintptr_t newVftablePtr)
{
    *reinterpret_cast<uintptr_t*>(obj) = newVftablePtr;
}

void* GetVirtualFunc(void* obj, int index)
{
    void** vtable = *reinterpret_cast<void***>(obj);
    return vtable[index];
}

static void BarrierICache(void* addr, size_t size)
{
    __clear_cache(reinterpret_cast<char*>(addr), reinterpret_cast<char*>(addr) + size);
#if defined(__aarch64__) || defined(__arm__)
    __asm__ __volatile__("dmb ish" ::: "memory");
    __asm__ __volatile__("isb" ::: "memory");
#endif
}


static bool ReplaceBytesViaRemap(uintptr_t addr, const void* data, size_t size)
{
    size_t pagesize = static_cast<size_t>(sysconf(_SC_PAGESIZE));
    uintptr_t pageStart = addr & ~(pagesize - 1);
    size_t pageOff = static_cast<size_t>(addr - pageStart);
    if (pageOff + size > pagesize) {
        __android_log_print(ANDROID_LOG_ERROR, LIB_TAG,
            "ReplaceBytesViaRemap: patch crosses page boundary");
        return false;
    }

    void* copy = mmap(nullptr, pagesize, PROT_READ | PROT_WRITE,
        MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (copy == MAP_FAILED) {
        __android_log_print(ANDROID_LOG_ERROR, LIB_TAG,
            "ReplaceBytesViaRemap: mmap copy failed errno=%d", errno);
        return false;
    }

    memcpy(copy, reinterpret_cast<void*>(pageStart), pagesize);
    memcpy(reinterpret_cast<char*>(copy) + pageOff, data, size);

    if (mprotect(copy, pagesize, PROT_READ | PROT_EXEC) != 0) {
        if (mprotect(copy, pagesize, PROT_READ | PROT_WRITE | PROT_EXEC) != 0) {
            __android_log_print(ANDROID_LOG_ERROR, LIB_TAG,
                "ReplaceBytesViaRemap: mprotect RX/RWX failed errno=%d", errno);
            munmap(copy, pagesize);
            return false;
        }
    }

    void* remapped = mremap(copy, pagesize, pagesize,
        MREMAP_MAYMOVE | MREMAP_FIXED,
        reinterpret_cast<void*>(pageStart));
    if (remapped == MAP_FAILED) {
        __android_log_print(ANDROID_LOG_ERROR, LIB_TAG,
            "ReplaceBytesViaRemap: mremap FIXED failed errno=%d", errno);
        munmap(copy, pagesize);
        return false;
    }

    BarrierICache(reinterpret_cast<void*>(addr), size);
    __android_log_print(ANDROID_LOG_INFO, LIB_TAG,
        "ReplaceBytesViaRemap: OK page @%p",
        reinterpret_cast<void*>(pageStart));
    return true;
}

bool ReplaceBytes(uintptr_t offset, const void* data, size_t size)
{
    uintptr_t addr = getActualOffset(offset);

    if (ReplaceBytesViaRemap(addr, data, size)) {
        return true;
    }

    __android_log_print(ANDROID_LOG_ERROR, LIB_TAG,
        "ReplaceBytes: all strategies failed errno=%d", errno);
    return false;
}