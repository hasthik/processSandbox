#include "handleNotification.h"

#define handle(A) case __NR_##A: handle_##A(proc_mem_fd, req, resp);
void watchNotifications(int notification_fd) {
    struct seccomp_notif *req;
    struct seccomp_notif_resp *resp;
    struct seccomp_notif_sizes sizes;
    char path[PATH_MAX];
    int proc_mem_fd;

    if ( seccomp(SECCOMP_GET_NOTIF_SIZES, 0, &sizes) == -1 )
        ee("seccomp");

    req = malloc(sizes.seccomp_notif);
    if (req == NULL)
        ee("malloc");

    resp = malloc(sizes.seccomp_notif_resp);
    if (resp == NULL)
        ee("malloc");
    
    while(true) {
        memset(req, 0,  sizes.seccomp_notif);
        memset(resp, 0,  sizes.seccomp_notif_resp);
        if ( ioctl(notification_fd, SECCOMP_IOCTL_NOTIF_RECV, req) == -1 ) {
            if ( errno == ENONET )
                exit(EXIT_SUCCESS);
            if (errno == EINTR)
                continue;
            ee("ioctl - SECCOMP_IOCTL_NOTIF_RECV");
        }
        snprintf(path, sizeof(path), "/proc/%d/mem", req->pid);

        proc_mem_fd = open(path, O_RDONLY);

        if ( proc_mem_fd < 0 ) {
            ee("open-proc-mem-fd");
        }

        if ( ioctl(notification_fd, SECCOMP_IOCTL_NOTIF_ID_VALID, &req->id) == -1 ) {
            continue;
        }

        switch (req->data.nr)
        {
        handle(mkdir);break;
        handle(mkdirat);break;
        handle(open);break;
        handle(stat);break;
        handle(lstat);break;
        handle(unlink);break;
        handle(unlinkat);break;
        handle(fork);break;
        handle(chmod);break;
        handle(chown);break;
        handle(lchown);break;
        handle(rename);break;
        handle(renameat);break;
        handle(truncate);break;
        handle(symlink);break;
        handle(symlinkat);break;
        handle(execve);break;
        handle(ptrace);break;
        handle(getuid);break;
        handle(getgid);break;
        handle(geteuid);break;
        handle(getegid);break;
        handle(setuid);break;
        handle(setgid);break;
        handle(setresuid);break;
        handle(setresgid);break;
        default:{
            resp->id = req->id;
            resp->error = 0;
            resp->flags = SECCOMP_USER_NOTIF_FLAG_CONTINUE;
            resp->val = 0;
        }
            break;
        }

        if ( ioctl(notification_fd, SECCOMP_IOCTL_NOTIF_ID_VALID, &req->id) == -1 ) {
            continue;
        }


        if ( ioctl(notification_fd, SECCOMP_IOCTL_NOTIF_SEND, resp) == -1) {
            if ( errno == ENONET )
                exit(EXIT_SUCCESS);
            ee("ioctl - SECCOMP_IOCTL_NOTIF_SEND");
        }

        close(proc_mem_fd);
    }
}