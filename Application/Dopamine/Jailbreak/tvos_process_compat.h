#ifndef DOPAMINE_TVOS_PROCESS_COMPAT_H
#define DOPAMINE_TVOS_PROCESS_COMPAT_H

#include <TargetConditionals.h>

#if TARGET_OS_TV
#include <mach/mach.h>
#include <spawn.h>
#include <stdint.h>
#include <sys/types.h>

int tvos_posix_spawn(
    pid_t *pid, const char *path,
    const posix_spawn_file_actions_t *fileActions,
    const posix_spawnattr_t *attr, char *const argv[], char *const envp[]);
int tvos_posix_spawnattr_init(posix_spawnattr_t *attr);
int tvos_posix_spawnattr_destroy(posix_spawnattr_t *attr);
int tvos_posix_spawnattr_setflags(const posix_spawnattr_t *attr, short flags);
int tvos_posix_spawn_file_actions_init(posix_spawn_file_actions_t *fileActions);
int tvos_posix_spawn_file_actions_adddup2(
    posix_spawn_file_actions_t *fileActions, int fileDescriptor, int newFileDescriptor);
int tvos_posix_spawn_file_actions_destroy(posix_spawn_file_actions_t *fileActions);

int tvos_posix_spawnattr_set_persona_np(
    const posix_spawnattr_t *attr, uid_t persona, uint32_t flags);
int tvos_posix_spawnattr_set_persona_uid_np(
    const posix_spawnattr_t *attr, uid_t persona);
int tvos_posix_spawnattr_set_persona_gid_np(
    const posix_spawnattr_t *attr, gid_t persona);
int tvos_posix_spawnattr_set_uid_np(
    const posix_spawnattr_t *attr, uid_t uid);
int tvos_posix_spawnattr_set_gid_np(
    const posix_spawnattr_t *attr, gid_t gid);

int posix_spawnattr_set_registered_ports_np(
    posix_spawnattr_t *attr, mach_port_t ports[], uint32_t count);

#define posix_spawn tvos_posix_spawn
#define posix_spawnattr_init tvos_posix_spawnattr_init
#define posix_spawnattr_destroy tvos_posix_spawnattr_destroy
#define posix_spawnattr_setflags tvos_posix_spawnattr_setflags
#define posix_spawn_file_actions_init tvos_posix_spawn_file_actions_init
#define posix_spawn_file_actions_adddup2 tvos_posix_spawn_file_actions_adddup2
#define posix_spawn_file_actions_destroy tvos_posix_spawn_file_actions_destroy
#define posix_spawnattr_set_persona_np tvos_posix_spawnattr_set_persona_np
#define posix_spawnattr_set_persona_uid_np tvos_posix_spawnattr_set_persona_uid_np
#define posix_spawnattr_set_persona_gid_np tvos_posix_spawnattr_set_persona_gid_np
#define posix_spawnattr_set_uid_np tvos_posix_spawnattr_set_uid_np
#define posix_spawnattr_set_gid_np tvos_posix_spawnattr_set_gid_np
#endif

#endif
