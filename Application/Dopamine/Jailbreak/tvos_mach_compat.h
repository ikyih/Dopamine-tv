#ifndef DOPAMINE_TVOS_MACH_COMPAT_H
#define DOPAMINE_TVOS_MACH_COMPAT_H

#include <TargetConditionals.h>

#if TARGET_OS_TV
#include <dlfcn.h>
#include <mach/mach.h>
#include <pthread.h>

typedef mach_msg_return_t (*tvos_mach_msg_function_t)(
    mach_msg_header_t *message,
    mach_msg_option_t option,
    mach_msg_size_t send_size,
    mach_msg_size_t receive_size,
    mach_port_t receive_port,
    mach_msg_timeout_t timeout,
    mach_port_t notify_port);

typedef kern_return_t (*tvos_task_get_special_port_function_t)(
    task_name_t task, int which_port, mach_port_t *port);

static tvos_mach_msg_function_t tvos_mach_msg_function;
static tvos_task_get_special_port_function_t tvos_task_get_special_port_function;
static pthread_once_t tvos_mach_msg_once = PTHREAD_ONCE_INIT;

static void tvos_resolve_mach_msg(void) {
    tvos_mach_msg_function = (tvos_mach_msg_function_t)dlsym(RTLD_DEFAULT, "mach_msg");
    tvos_task_get_special_port_function =
        (tvos_task_get_special_port_function_t)dlsym(RTLD_DEFAULT, "task_get_special_port");
}

static inline mach_msg_return_t tvos_mach_msg(
    mach_msg_header_t *message,
    mach_msg_option_t option,
    mach_msg_size_t send_size,
    mach_msg_size_t receive_size,
    mach_port_t receive_port,
    mach_msg_timeout_t timeout,
    mach_port_t notify_port) {
    pthread_once(&tvos_mach_msg_once, tvos_resolve_mach_msg);
    if (tvos_mach_msg_function == NULL) return MACH_SEND_INVALID_DEST;
    return tvos_mach_msg_function(message, option, send_size, receive_size,
                                  receive_port, timeout, notify_port);
}

static inline kern_return_t tvos_task_get_special_port(
    task_name_t task, int which_port, mach_port_t *port) {
    pthread_once(&tvos_mach_msg_once, tvos_resolve_mach_msg);
    if (tvos_task_get_special_port_function == NULL) return KERN_NOT_SUPPORTED;
    return tvos_task_get_special_port_function(task, which_port, port);
}

#define mach_msg tvos_mach_msg
#define task_get_special_port tvos_task_get_special_port
#endif

#endif
