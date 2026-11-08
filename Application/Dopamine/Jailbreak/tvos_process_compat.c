#include "tvos_process_compat.h"

#if TARGET_OS_TV
#include <dlfcn.h>
#include <errno.h>
#include <pthread.h>

typedef int (*set_persona_function_t)(
    const posix_spawnattr_t *, uid_t, uint32_t);
typedef int (*set_persona_id_function_t)(
    const posix_spawnattr_t *, uid_t);
typedef int (*set_persona_gid_function_t)(
    const posix_spawnattr_t *, gid_t);
typedef int (*set_uid_function_t)(
    const posix_spawnattr_t *, uid_t);
typedef int (*set_gid_function_t)(
    const posix_spawnattr_t *, gid_t);
typedef int (*spawn_function_t)(
    pid_t *, const char *, const posix_spawn_file_actions_t *,
    const posix_spawnattr_t *, char *const [], char *const []);
typedef int (*spawnattr_init_function_t)(posix_spawnattr_t *);
typedef int (*spawnattr_destroy_function_t)(posix_spawnattr_t *);
typedef int (*spawnattr_setflags_function_t)(const posix_spawnattr_t *, short);
typedef int (*spawn_file_actions_init_function_t)(posix_spawn_file_actions_t *);
typedef int (*spawn_file_actions_adddup2_function_t)(posix_spawn_file_actions_t *, int, int);
typedef int (*spawn_file_actions_destroy_function_t)(posix_spawn_file_actions_t *);
typedef int (*set_registered_ports_function_t)(
    posix_spawnattr_t *, mach_port_t [], uint32_t);

static spawn_function_t spawn_function;
static spawnattr_init_function_t spawnattr_init_function;
static spawnattr_destroy_function_t spawnattr_destroy_function;
static spawnattr_setflags_function_t spawnattr_setflags_function;
static spawn_file_actions_init_function_t spawn_file_actions_init_function;
static spawn_file_actions_adddup2_function_t spawn_file_actions_adddup2_function;
static spawn_file_actions_destroy_function_t spawn_file_actions_destroy_function;
static set_persona_function_t set_persona_function;
static set_persona_id_function_t set_persona_uid_function;
static set_persona_gid_function_t set_persona_gid_function;
static set_uid_function_t set_uid_function;
static set_gid_function_t set_gid_function;
static set_registered_ports_function_t set_registered_ports_function;
static pthread_once_t resolve_spawn_functions_once = PTHREAD_ONCE_INIT;

static void resolve_spawn_functions(void) {
    spawn_function = (spawn_function_t)dlsym(RTLD_DEFAULT, "posix_spawn");
    spawnattr_init_function = (spawnattr_init_function_t)dlsym(
        RTLD_DEFAULT, "posix_spawnattr_init");
    spawnattr_destroy_function = (spawnattr_destroy_function_t)dlsym(
        RTLD_DEFAULT, "posix_spawnattr_destroy");
    spawnattr_setflags_function = (spawnattr_setflags_function_t)dlsym(
        RTLD_DEFAULT, "posix_spawnattr_setflags");
    spawn_file_actions_init_function = (spawn_file_actions_init_function_t)dlsym(
        RTLD_DEFAULT, "posix_spawn_file_actions_init");
    spawn_file_actions_adddup2_function = (spawn_file_actions_adddup2_function_t)dlsym(
        RTLD_DEFAULT, "posix_spawn_file_actions_adddup2");
    spawn_file_actions_destroy_function = (spawn_file_actions_destroy_function_t)dlsym(
        RTLD_DEFAULT, "posix_spawn_file_actions_destroy");
    set_persona_function = (set_persona_function_t)dlsym(
        RTLD_DEFAULT, "posix_spawnattr_set_persona_np");
    set_persona_uid_function = (set_persona_id_function_t)dlsym(
        RTLD_DEFAULT, "posix_spawnattr_set_persona_uid_np");
    set_persona_gid_function = (set_persona_gid_function_t)dlsym(
        RTLD_DEFAULT, "posix_spawnattr_set_persona_gid_np");
    set_uid_function = (set_uid_function_t)dlsym(
        RTLD_DEFAULT, "posix_spawnattr_set_uid_np");
    set_gid_function = (set_gid_function_t)dlsym(
        RTLD_DEFAULT, "posix_spawnattr_set_gid_np");
    set_registered_ports_function = (set_registered_ports_function_t)dlsym(
        RTLD_NEXT, "posix_spawnattr_set_registered_ports_np");
}

int tvos_posix_spawn(
    pid_t *pid, const char *path,
    const posix_spawn_file_actions_t *fileActions,
    const posix_spawnattr_t *attr, char *const argv[], char *const envp[]) {
    pthread_once(&resolve_spawn_functions_once, resolve_spawn_functions);
    return spawn_function
        ? spawn_function(pid, path, fileActions, attr, argv, envp)
        : ENOTSUP;
}

int tvos_posix_spawnattr_init(posix_spawnattr_t *attr) {
    pthread_once(&resolve_spawn_functions_once, resolve_spawn_functions);
    return spawnattr_init_function ? spawnattr_init_function(attr) : ENOTSUP;
}

int tvos_posix_spawnattr_destroy(posix_spawnattr_t *attr) {
    pthread_once(&resolve_spawn_functions_once, resolve_spawn_functions);
    return spawnattr_destroy_function ? spawnattr_destroy_function(attr) : ENOTSUP;
}

int tvos_posix_spawnattr_setflags(const posix_spawnattr_t *attr, short flags) {
    pthread_once(&resolve_spawn_functions_once, resolve_spawn_functions);
    return spawnattr_setflags_function
        ? spawnattr_setflags_function(attr, flags)
        : ENOTSUP;
}

int tvos_posix_spawn_file_actions_init(posix_spawn_file_actions_t *fileActions) {
    pthread_once(&resolve_spawn_functions_once, resolve_spawn_functions);
    return spawn_file_actions_init_function
        ? spawn_file_actions_init_function(fileActions)
        : ENOTSUP;
}

int tvos_posix_spawn_file_actions_adddup2(
    posix_spawn_file_actions_t *fileActions, int fileDescriptor, int newFileDescriptor) {
    pthread_once(&resolve_spawn_functions_once, resolve_spawn_functions);
    return spawn_file_actions_adddup2_function
        ? spawn_file_actions_adddup2_function(fileActions, fileDescriptor, newFileDescriptor)
        : ENOTSUP;
}

int tvos_posix_spawn_file_actions_destroy(posix_spawn_file_actions_t *fileActions) {
    pthread_once(&resolve_spawn_functions_once, resolve_spawn_functions);
    return spawn_file_actions_destroy_function
        ? spawn_file_actions_destroy_function(fileActions)
        : ENOTSUP;
}

int tvos_posix_spawnattr_set_persona_np(
    const posix_spawnattr_t *attr, uid_t persona, uint32_t flags) {
    pthread_once(&resolve_spawn_functions_once, resolve_spawn_functions);
    return set_persona_function ? set_persona_function(attr, persona, flags) : ENOTSUP;
}

int tvos_posix_spawnattr_set_persona_uid_np(
    const posix_spawnattr_t *attr, uid_t persona) {
    pthread_once(&resolve_spawn_functions_once, resolve_spawn_functions);
    return set_persona_uid_function ? set_persona_uid_function(attr, persona) : ENOTSUP;
}

int tvos_posix_spawnattr_set_persona_gid_np(
    const posix_spawnattr_t *attr, gid_t persona) {
    pthread_once(&resolve_spawn_functions_once, resolve_spawn_functions);
    return set_persona_gid_function ? set_persona_gid_function(attr, persona) : ENOTSUP;
}

int tvos_posix_spawnattr_set_uid_np(
    const posix_spawnattr_t *attr, uid_t uid) {
    pthread_once(&resolve_spawn_functions_once, resolve_spawn_functions);
    return set_uid_function ? set_uid_function(attr, uid) : ENOTSUP;
}

int tvos_posix_spawnattr_set_gid_np(
    const posix_spawnattr_t *attr, gid_t gid) {
    pthread_once(&resolve_spawn_functions_once, resolve_spawn_functions);
    return set_gid_function ? set_gid_function(attr, gid) : ENOTSUP;
}

int posix_spawnattr_set_registered_ports_np(
    posix_spawnattr_t *attr, mach_port_t ports[], uint32_t count) {
    pthread_once(&resolve_spawn_functions_once, resolve_spawn_functions);
    return set_registered_ports_function
        ? set_registered_ports_function(attr, ports, count)
        : ENOTSUP;
}
#endif
