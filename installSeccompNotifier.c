#include "installSeccompNotifier.h"
#include "bpf.h"

int installNotifyFilter(void) {

    struct sock_fprog bpf_prog = {
        .len = (unsigned short) (sizeof(filter) / sizeof(filter[0])),
        .filter = filter,
    };

    int notify_fd = seccomp(SECCOMP_SET_MODE_FILTER,
                           SECCOMP_FILTER_FLAG_NEW_LISTENER, &bpf_prog);
    if (notify_fd < 0)
        ee("seccomp-install-notify-filter");

    return notify_fd;
}