#include "heuristics.h"
#define ret(A,B) if ( A == -1 ) ee(B);
#define hfp(A,B) handle_file_permissions(A, B, proc_mem_fd, req, res);
#define hfpm(A,B,C) handle_file_permissions_multiple(A, B, C, proc_mem_fd, req, res);
#define hu() handle_uid(req, res)
#define hg() handle_gid(req, res)

static volatile int no_of_execve_calls = 0;

char * getString(int fd) {
    char * path = malloc(2);
    int size = 0;
    char t;
    ret(read(fd, &t, 1), "read");
    while ( t != '\0' ) {
        path[size] = t;
        size++;
        path = realloc(path, size + 1);
        ret(read(fd, &t, 1), "read");
    }
    path[size] = '\0';
    return path;
}

void handle_file_permissions_multiple(int argvalue1, int argvalue2, char * fmt_txt, int proc_mem_fd, struct seccomp_notif * req, struct seccomp_notif_resp * res) {
    int ret_val = lseek(proc_mem_fd, req->data.args[argvalue1],SEEK_SET);
    if ( ret_val == -1 ) {
        ee("lseek");
    }

    char * path_name1 = getString(proc_mem_fd);

    ret_val = lseek(proc_mem_fd, req->data.args[argvalue2],SEEK_SET);
    if ( ret_val == -1 ) {
        ee("lseek");
    }

    char * path_name2 = getString(proc_mem_fd);

    if ( strncmp(path_name1, "/tmp", strlen("/tmp")) == 0 && strncmp(path_name2, "/tmp", strlen("/tmp")) == 0 ) {
        res->id = req->id;
        res->flags = SECCOMP_USER_NOTIF_FLAG_CONTINUE;
        res->val = 0;
    } else {
        char fulltext[ strlen(fmt_txt) + strlen(path_name1) + strlen(path_name2) ];
        sprintf(fulltext, fmt_txt, path_name1, path_name2);
        if ( gtk_confirm(fulltext) == true ) {
            res->id = req->id;
            res->flags = SECCOMP_USER_NOTIF_FLAG_CONTINUE;
            res->val = 0;
        } else {
            res->id = req->id;
            res->flags = 0;
            res->val = -EPERM;
        }
    }
}

void handle_file_permissions(int argvalue, char * fmt_txt, int proc_mem_fd, struct seccomp_notif * req, struct seccomp_notif_resp * res) {
    int ret_val = lseek(proc_mem_fd, req->data.args[argvalue],SEEK_SET);
    if ( ret_val == -1 ) {
        ee("lseek");
    }

    char * path_name = getString(proc_mem_fd);
    if ( strncmp(path_name, "/tmp", strlen("/tmp")) == 0 ) {
        res->id = req->id;
        res->flags = SECCOMP_USER_NOTIF_FLAG_CONTINUE;
        res->val = 0;
    } else {
        char fulltext[ strlen(fmt_txt) + strlen(path_name) ];
        sprintf(fulltext, fmt_txt, path_name);
        if ( gtk_confirm(fulltext) == true ) {
            res->id = req->id;
            res->flags = SECCOMP_USER_NOTIF_FLAG_CONTINUE;
            res->val = 0;
        } else {
            res->id = req->id;
            res->flags = 0;
            res->val = -EPERM;
        }
    }
}

void handle_uid(struct seccomp_notif * req, struct seccomp_notif_resp * res) {
    uid_t ruid, euid, suid;
    getresuid(&ruid, &euid, &suid);
    if( ruid == 0 || euid == 0 || suid == 0 ) {
        res->id = req->id;
        res->error = -EPERM;
        res->flags = 0;
        res->val = 0;
    } else {
        res->id = req->id;
        res->error = 0;
        res->flags = SECCOMP_USER_NOTIF_FLAG_CONTINUE;
        res->val = 0;
    }
}

void handle_gid(struct seccomp_notif * req, struct seccomp_notif_resp * res) {
    gid_t ruid, euid, suid;
    getresgid(&ruid, &euid, &suid);
    if( ruid == 0 || euid == 0 || suid == 0 ) {
        res->id = req->id;
        res->error = -EPERM;
        res->flags = 0;
        res->val = 0;
    } else {
        res->id = req->id;
        res->error = 0;
        res->flags = SECCOMP_USER_NOTIF_FLAG_CONTINUE;
        res->val = 0;
    }
}

void handle_mkdir (
    int proc_mem_fd,
    struct seccomp_notif * req, 
    struct seccomp_notif_resp * res
    ) {
    hfp(0,"The process wants to create the following directory: %s");
}

void handle_mkdirat(int proc_mem_fd, struct seccomp_notif * req, struct seccomp_notif_resp * res) {
    hfp(1,"The process wants to create the following directory: %s");
}

void handle_open(int proc_mem_fd, struct seccomp_notif * req, struct seccomp_notif_resp * res) {
    hfp(0,"The process wants to open the following file: %s");
}

void handle_openat(int proc_mem_fd, struct seccomp_notif * req, struct seccomp_notif_resp * res) {
    hfp(0,"The process wants to open the following file: %s");
}

void handle_stat(int proc_mem_fd, struct seccomp_notif * req, struct seccomp_notif_resp * res) {
    hfp(0,"The process wants to get the file status of the following file: %s");
}
void handle_lstat(int proc_mem_fd, struct seccomp_notif * req, struct seccomp_notif_resp * res) {
    hfp(0,"The process wants to get the file status of the following file: %s");
}
void handle_unlink(int proc_mem_fd, struct seccomp_notif * req, struct seccomp_notif_resp * res) {
    hfp(0,"The process wants to unlink the following symlink: %s");
}
void handle_unlinkat(int proc_mem_fd, struct seccomp_notif * req, struct seccomp_notif_resp * res) {
    hfp(1,"The process wants to unlink the following symlink: %s");
}
void handle_fork(int proc_mem_fd, struct seccomp_notif * req, struct seccomp_notif_resp * res) {
    if ( gtk_confirm("The process is trying to create multiple child processes") == true ) {
        res->id = req->id;
        res->flags = SECCOMP_USER_NOTIF_FLAG_CONTINUE;
        res->val = 0;
    } else {
        res->id = req->id;
        res->flags = 0;
        res->val = -EPERM;
    }
}
void handle_chmod(int proc_mem_fd, struct seccomp_notif * req, struct seccomp_notif_resp * res) {
    hfp(0, "The process is trying to modify permissions of the following file: %s");
}
void handle_chown(int proc_mem_fd, struct seccomp_notif * req, struct seccomp_notif_resp * res) {
    hfp(0, "The process is trying to set the owner of the following file: %s");
}
void handle_lchown(int proc_mem_fd, struct seccomp_notif * req, struct seccomp_notif_resp * res) {
    hfp(0, "The process is trying to set the owner of the following symlink: %s");  
}
void handle_rename(int proc_mem_fd, struct seccomp_notif * req, struct seccomp_notif_resp * res) {
    hfpm(0,1, "The process is trying to rename %s to %s");
}
void handle_renameat(int proc_mem_fd, struct seccomp_notif * req, struct seccomp_notif_resp * res) {
    hfpm(1,3, "The process is trying to rename %s to %s");
}
void handle_truncate(int proc_mem_fd, struct seccomp_notif * req, struct seccomp_notif_resp * res) {
    hfp(0, "The process is trying to truncate the following file: %s");
}
void handle_symlink(int proc_mem_fd, struct seccomp_notif * req, struct seccomp_notif_resp * res) {
    hfpm(0,1, "The process is trying to create a symlink from %s to %s");
}
void handle_symlinkat(int proc_mem_fd, struct seccomp_notif * req, struct seccomp_notif_resp * res) {
    hfpm(1,3, "The process is trying to create a symlink from %s to %s");
}
void handle_execve(int proc_mem_fd, struct seccomp_notif * req, struct seccomp_notif_resp * res) {
        if ( no_of_execve_calls == 0 ) {
            res->id = req->id;
            res->error = 0;
            res->flags = SECCOMP_USER_NOTIF_FLAG_CONTINUE;
            res->val = 0;
            no_of_execve_calls = 1;
        } else {
            if ( gtk_confirm("The process is trying to create multiple child processes") == true ) {
                res->id = req->id;
                res->flags = SECCOMP_USER_NOTIF_FLAG_CONTINUE;
                res->val = 0;
            } else {
                res->id = req->id;
                res->flags = 0;
                res->val = -EPERM;
            }
        }
}

void handle_ptrace(int proc_mem_fd, struct seccomp_notif * req, struct seccomp_notif_resp * res) {
        res->id = req->id;
        res->error = 0;
        res->flags = 0;
        res->val = -EPERM;
}
void handle_getuid(int proc_mem_fd, struct seccomp_notif * req, struct seccomp_notif_resp * res) {
    hu();
}
void handle_getgid(int proc_mem_fd, struct seccomp_notif * req, struct seccomp_notif_resp * res) {
    hg();
}
void handle_geteuid(int proc_mem_fd, struct seccomp_notif * req, struct seccomp_notif_resp * res) {
    hu();
}
void handle_getegid(int proc_mem_fd, struct seccomp_notif * req, struct seccomp_notif_resp * res) {
    hg();
}
void handle_setuid(int proc_mem_fd, struct seccomp_notif * req, struct seccomp_notif_resp * res) {
        res->id = req->id;
        res->error = 0;
        res->flags = 0;
        res->val = -EPERM;
}
void handle_setgid(int proc_mem_fd, struct seccomp_notif * req, struct seccomp_notif_resp * res) {
        res->id = req->id;
        res->error = 0;
        res->flags = 0;
        res->val = -EPERM;
}
void handle_setresuid(int proc_mem_fd, struct seccomp_notif * req, struct seccomp_notif_resp * res) {
        res->id = req->id;
        res->error = 0;
        res->flags = 0;
        res->val = -EPERM;
}
void handle_setresgid(int proc_mem_fd, struct seccomp_notif * req, struct seccomp_notif_resp * res) {
        res->id = req->id;
        res->error = 0;
        res->flags = 0;
        res->val = -EPERM;
}

