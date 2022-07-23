#include "common.h"

int
sendfd(int sockfd, int fd) {
    union {
        char   buf[CMSG_SPACE(sizeof(int))];                        
        struct cmsghdr align;
    } controlMsg;

    struct msghdr msgh;
    msgh.msg_name = NULL;
    msgh.msg_namelen = 0;

    struct iovec iov;
    int data;

    msgh.msg_iov = &iov;
    msgh.msg_iovlen = 1;
    iov.iov_base = &data;
    iov.iov_len = sizeof(int);
    data = 12345;
    msgh.msg_control = controlMsg.buf;
    msgh.msg_controllen = sizeof(controlMsg.buf);
    struct cmsghdr *cmsgp;
    cmsgp = CMSG_FIRSTHDR(&msgh);
    cmsgp->cmsg_level = SOL_SOCKET;
    cmsgp->cmsg_type = SCM_RIGHTS;
    cmsgp->cmsg_len = CMSG_LEN(sizeof(int));
    memcpy(CMSG_DATA(cmsgp), &fd, sizeof(int));
    if (sendmsg(sockfd, &msgh, 0) == -1)
        return -1;

    return 0;
}


int recvfd(int sockfd) {
    struct msghdr msgh;
    struct iovec iov;
    int data, fd;
    ssize_t nr;

    union {
        char   buf[CMSG_SPACE(sizeof(int))];
        struct cmsghdr align;
    } controlMsg;
    struct cmsghdr *cmsgp;
    msgh.msg_name = NULL;
    msgh.msg_namelen = 0;
    msgh.msg_iov = &iov;
    msgh.msg_iovlen = 1;
    iov.iov_base = &data;       
    iov.iov_len = sizeof(int);
    msgh.msg_control = controlMsg.buf;
    msgh.msg_controllen = sizeof(controlMsg.buf);
    nr = recvmsg(sockfd, &msgh, 0);
    if (nr == -1)
        return -1;

    cmsgp = CMSG_FIRSTHDR(&msgh);
    if (cmsgp == NULL ||
            cmsgp->cmsg_len != CMSG_LEN(sizeof(int)) ||
            cmsgp->cmsg_level != SOL_SOCKET ||
            cmsgp->cmsg_type != SCM_RIGHTS) {
        errno = EINVAL;
        return -1;
    }
    memcpy(&fd, CMSG_DATA(cmsgp), sizeof(int));
    return fd;
}