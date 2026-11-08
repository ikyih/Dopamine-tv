#ifndef DOPAMINE_TVOS_IOKIT_COMPAT_H
#define DOPAMINE_TVOS_IOKIT_COMPAT_H

#include <CoreFoundation/CoreFoundation.h>
#include <dlfcn.h>
#include <pthread.h>
#include <stdint.h>

typedef uint32_t tvos_io_registry_entry_t;

typedef tvos_io_registry_entry_t (*tvos_io_registry_entry_from_path_t)(
    uint32_t master_port, const char *path);
typedef CFTypeRef (*tvos_io_registry_entry_create_property_t)(
    tvos_io_registry_entry_t entry, CFStringRef key,
    CFAllocatorRef allocator, uint32_t options);
typedef int (*tvos_io_object_release_t)(tvos_io_registry_entry_t object);

static void *tvos_iokit_handle;
static pthread_once_t tvos_iokit_once = PTHREAD_ONCE_INIT;

static inline void tvos_iokit_load_framework(void)
{
    tvos_iokit_handle = dlopen(
        "/System/Library/Frameworks/IOKit.framework/IOKit", RTLD_LAZY | RTLD_LOCAL);
}

static inline void *tvos_iokit_framework_handle(void)
{
    pthread_once(&tvos_iokit_once, tvos_iokit_load_framework);
    return tvos_iokit_handle;
}

static inline void *tvos_iokit_symbol(const char *name)
{
    void *symbol = dlsym(RTLD_DEFAULT, name);
    if (symbol != NULL) return symbol;
    void *handle = tvos_iokit_framework_handle();
    return handle != NULL ? dlsym(handle, name) : NULL;
}

static inline tvos_io_registry_entry_t tvos_IORegistryEntryFromPath(
    uint32_t master_port, const char *path) {
    tvos_io_registry_entry_from_path_t function =
        (tvos_io_registry_entry_from_path_t)tvos_iokit_symbol("IORegistryEntryFromPath");
    return function ? function(master_port, path) : 0;
}

static inline CFTypeRef tvos_IORegistryEntryCreateCFProperty(
    tvos_io_registry_entry_t entry, CFStringRef key,
    CFAllocatorRef allocator, uint32_t options) {
    tvos_io_registry_entry_create_property_t function =
        (tvos_io_registry_entry_create_property_t)tvos_iokit_symbol(
            "IORegistryEntryCreateCFProperty");
    return function ? function(entry, key, allocator, options) : NULL;
}

static inline void tvos_IOObjectRelease(tvos_io_registry_entry_t object) {
    tvos_io_object_release_t function =
        (tvos_io_object_release_t)tvos_iokit_symbol("IOObjectRelease");
    if (function) (void)function(object);
}

#endif
