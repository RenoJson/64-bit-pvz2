#pragma once

// Useful for filtering logcat
#define LIB_TAG "PvZ2Chair"

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, LIB_TAG, __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, LIB_TAG, __VA_ARGS__))

#define LOG_RETURN_ADDRESS(level) \
LOGI("%p", (uintptr_t)__builtin_return_address(level) - g_libAddress);