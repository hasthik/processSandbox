#pragma once

#include "common.h"
#include "openAlertBox.h"

void handle_file_permissions(int argvalue, char * fmt_txt, int proc_mem_fd, struct seccomp_notif * req, struct seccomp_notif_resp * res);
void handle_file_permissions_multiple(int argvalue1, int argvalue2, char * fmt_txt, int proc_mem_fd, struct seccomp_notif * req, struct seccomp_notif_resp * res);
void handle_mkdir(int proc_mem_fd, struct seccomp_notif * req, struct seccomp_notif_resp * res);
void handle_mkdirat(int proc_mem_fd, struct seccomp_notif * req, struct seccomp_notif_resp * res);
void handle_open(int proc_mem_fd, struct seccomp_notif * req, struct seccomp_notif_resp * res);
void handle_openat(int proc_mem_fd, struct seccomp_notif * req, struct seccomp_notif_resp * res);
void handle_stat(int proc_mem_fd, struct seccomp_notif * req, struct seccomp_notif_resp * res);
void handle_lstat(int proc_mem_fd, struct seccomp_notif * req, struct seccomp_notif_resp * res);
void handle_unlink(int proc_mem_fd, struct seccomp_notif * req, struct seccomp_notif_resp * res);
void handle_unlinkat(int proc_mem_fd, struct seccomp_notif * req, struct seccomp_notif_resp * res);
void handle_fork(int proc_mem_fd, struct seccomp_notif * req, struct seccomp_notif_resp * res);
void handle_chmod(int proc_mem_fd, struct seccomp_notif * req, struct seccomp_notif_resp * res);
void handle_chown(int proc_mem_fd, struct seccomp_notif * req, struct seccomp_notif_resp * res);
void handle_lchown(int proc_mem_fd, struct seccomp_notif * req, struct seccomp_notif_resp * res);
void handle_rename(int proc_mem_fd, struct seccomp_notif * req, struct seccomp_notif_resp * res);
void handle_renameat(int proc_mem_fd, struct seccomp_notif * req, struct seccomp_notif_resp * res);
void handle_truncate(int proc_mem_fd, struct seccomp_notif * req, struct seccomp_notif_resp * res);
void handle_symlink(int proc_mem_fd, struct seccomp_notif * req, struct seccomp_notif_resp * res);
void handle_symlinkat(int proc_mem_fd, struct seccomp_notif * req, struct seccomp_notif_resp * res);
void handle_execve(int proc_mem_fd, struct seccomp_notif * req, struct seccomp_notif_resp * res);
void handle_ptrace(int proc_mem_fd, struct seccomp_notif * req, struct seccomp_notif_resp * res);
void handle_getuid(int proc_mem_fd, struct seccomp_notif * req, struct seccomp_notif_resp * res);
void handle_getgid(int proc_mem_fd, struct seccomp_notif * req, struct seccomp_notif_resp * res);
void handle_geteuid(int proc_mem_fd, struct seccomp_notif * req, struct seccomp_notif_resp * res);
void handle_getegid(int proc_mem_fd, struct seccomp_notif * req, struct seccomp_notif_resp * res);
void handle_setuid(int proc_mem_fd, struct seccomp_notif * req, struct seccomp_notif_resp * res);
void handle_setgid(int proc_mem_fd, struct seccomp_notif * req, struct seccomp_notif_resp * res);
void handle_setresuid(int proc_mem_fd, struct seccomp_notif * req, struct seccomp_notif_resp * res);
void handle_setresgid(int proc_mem_fd, struct seccomp_notif * req, struct seccomp_notif_resp * res);

