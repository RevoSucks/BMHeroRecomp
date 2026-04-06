#include "patches.h"
#include "misc_funcs.h"
#include "stdarg.h"

typedef void* (*PrintCallback)(void*, const char*, size_t);

extern int _Printf(PrintCallback pfn, void* arg, const char* fmt, va_list ap);

void* proutPrintf_2(void* dst, const char* fmt, size_t size) {
    recomp_puts(fmt, size);
    return (void*)1;
}

RECOMP_EXPORT int recomp_printf(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    int ret = _Printf(&proutPrintf_2, NULL, fmt, args);

    va_end(args);

    return ret;
}
