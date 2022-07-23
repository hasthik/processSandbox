#pragma once
#include "common.h"

#define OFF_SYSCALL     (offsetof(struct seccomp_data, nr  ))
#define OFF_ARCH        (offsetof(struct seccomp_data, arch))

#define LD_SYSCALL                                      \
    BPF_STMT(BPF_LD+BPF_W+BPF_ABS, OFF_SYSCALL)

#define TRACE_SYSCALL(name)                             \
    BPF_JUMP(BPF_JMP+BPF_JEQ+BPF_K, __NR_##name, 0, 1), \
    BPF_STMT(BPF_RET+BPF_K, SECCOMP_RET_USER_NOTIF)

#define ALLOWED                                         \
    BPF_STMT(BPF_RET+BPF_K, SECCOMP_RET_ALLOW)

struct sock_filter filter[] = {
        LD_SYSCALL,
        TRACE_SYSCALL(open),
        TRACE_SYSCALL(openat),
        TRACE_SYSCALL(stat),
        TRACE_SYSCALL(lstat),
        TRACE_SYSCALL(newfstatat),
        TRACE_SYSCALL(mkdir),
        TRACE_SYSCALL(mkdirat),
        TRACE_SYSCALL(rmdir),
        TRACE_SYSCALL(unlink),
        TRACE_SYSCALL(unlinkat),
        TRACE_SYSCALL(access),
        TRACE_SYSCALL(faccessat),
        TRACE_SYSCALL(getdents),
        TRACE_SYSCALL(getcwd),
        TRACE_SYSCALL(chdir),
        TRACE_SYSCALL(creat),
        TRACE_SYSCALL(utimensat),
        TRACE_SYSCALL(utime),
        TRACE_SYSCALL(chmod),
        TRACE_SYSCALL(chown),
        TRACE_SYSCALL(lchown),
        TRACE_SYSCALL(execve),
        TRACE_SYSCALL(truncate),
        TRACE_SYSCALL(rename),
        TRACE_SYSCALL(renameat),
        TRACE_SYSCALL(link),
        TRACE_SYSCALL(symlink),
        TRACE_SYSCALL(readlink),
        TRACE_SYSCALL(mknod),
        TRACE_SYSCALL(readlinkat),
        TRACE_SYSCALL(fchmodat),
        TRACE_SYSCALL(linkat),
        TRACE_SYSCALL(symlinkat),
        TRACE_SYSCALL(mknodat),
        TRACE_SYSCALL(fchownat),
        TRACE_SYSCALL(futimesat),
        TRACE_SYSCALL(utimes),
        TRACE_SYSCALL(uselib),
        TRACE_SYSCALL(statfs),
        TRACE_SYSCALL(acct),
        TRACE_SYSCALL(ptrace),
        TRACE_SYSCALL(setxattr),
        TRACE_SYSCALL(lsetxattr),
        TRACE_SYSCALL(getxattr),
        TRACE_SYSCALL(lgetxattr),
        TRACE_SYSCALL(removexattr),
        TRACE_SYSCALL(lremovexattr),
        TRACE_SYSCALL(listxattr),
        TRACE_SYSCALL(llistxattr),
        TRACE_SYSCALL(socket),
        TRACE_SYSCALL(bind),
        TRACE_SYSCALL(connect),
        TRACE_SYSCALL(getuid),
        TRACE_SYSCALL(getgid),
        TRACE_SYSCALL(geteuid),
        TRACE_SYSCALL(getegid),
        TRACE_SYSCALL(setuid),
        TRACE_SYSCALL(setgid),
        TRACE_SYSCALL(setresuid),
        TRACE_SYSCALL(setregid),
        TRACE_SYSCALL(fchown),
        TRACE_SYSCALL(prctl),
        TRACE_SYSCALL(mprotect),
        TRACE_SYSCALL(mmap),
        TRACE_SYSCALL(mremap),
        TRACE_SYSCALL(brk),
        ALLOWED,
    };