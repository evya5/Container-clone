#ifndef EX5_MYCONTAINER_H
#define EX5_MYCONTAINER_H
#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <sched.h>
#include <cstdlib>
#include <cstring>
#include <wait.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <fstream>
#include "ErrHandler.h"

#define STACK 8192
#define DIR_PERMISSIONS 0755
#define WRITE_MODE "w"

#define FS_BASE_DIR "/"
#define PATH_TO_FS "/sys/fs"
#define PATH_TO_CGROUP "/sys/fs/cgroup"
#define PATH_TO_PIDS "/sys/fs/cgroup/pids"
#define PATH_TO_PROC "/proc"
#define PROC_FOLDER_NAME "proc"

#define NOTIFY_ON_RELEASE_FLAG "1"

#define PIDS_MAX_FILENAME "/pids.max"
#define CGROUP_PROCS_FILENAMME "/cgroup.procs"
#define NOTIFY_ON_RELEASE_FILENAME "/notify_on_release"

#define MKDIR_FUNC "mkdir()"
#define CHROOT_FUNC "chroot()"
#define CHDIR_FUNC "chdir()"
#define SETHOSTNAME_FUNC "sethostname()"
#define MOUNT_FUNC "mount()"
#define UMOUNT_FUNC "umount()"
#define WAIT_FUNC "wait()"
#define CLONE_FUNC "clone()"
#define FOPEN_FUNC "fopen()"
#define REMOVE_FUNC "remove()"
#define CHMOD_FUNC "chmod()"

int main(int argc, char* argv[]);

#endif