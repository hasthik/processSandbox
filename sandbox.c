#include "common.h"
#include "installSeccompNotifier.h"
#include "handleNotification.h"
#include "socketComm.h"

void help() {
    printf("USAGE: sandbox [name_of_file] [args] ...");
}

pid_t createChild(int socket_fd_pair[], char * argv[], int argc) {
    pid_t child_pid;
    int notification_fd;
    child_pid = fork();
    if ( child_pid < 0 ) {
        ee("fork");
    }

    if ( child_pid != 0 ) {
        return child_pid;
    }

    printf("[*] Starting target process: %d\n", (pid_t)getpid());

    if ( prctl(PR_SET_NO_NEW_PRIVS, 1, 0, 0, 0) )
        ee("prctl");
    
    notification_fd = installNotifyFilter();
    
    if (sendfd(socket_fd_pair[0], notification_fd) == -1) {
        ee("sendfd");
    }

    if ( close(notification_fd) < 0 ) {
        ee("close");
    }

    printf("[*] Done setting up\n");

    return execvp(argv[0], &argv[0]);
}

pid_t createController(int socket_pair[2]) {
    pid_t controller_pid = fork();
    if ( controller_pid < 0 )
        ee("fork");

    if ( controller_pid != 0 ) {
        return controller_pid;
    }

    int notification_fd = recvfd(socket_pair[1]);
    if ( notification_fd < 0 )
        ee("recvfd");

    printf("[*] Ready to setup listener\n");

    watchNotifications(notification_fd);

    exit(EXIT_SUCCESS);
}

int main(int argc, char * argv[]) {
    gtk_init (&argc, &argv);
    if ( argc < 2 ) {
        help();
        exit(EXIT_FAILURE);
    }

    setbuf(stdout, NULL);

    int socket_fd_pair[2];
    pid_t pid_of_sandboxed_fork;
    if ( socketpair(AF_UNIX, SOCK_STREAM, 0, socket_fd_pair) == -1 ) {
        ee("socketpair");
    }

    pid_of_sandboxed_fork = createChild(socket_fd_pair, &argv[1], argc - 1);
    createController(socket_fd_pair);
    waitpid( pid_of_sandboxed_fork, NULL, 0);
    printf("[*] Process exited!\n");
}
