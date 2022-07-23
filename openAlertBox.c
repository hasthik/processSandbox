#include "openAlertBox.h"

bool gtk_confirm(char * question) {
    char ans[2];
    printf("[*] %s. [y/N] ", question);
    scanf("%s", ans);
    printf("[*] %s\n", ans[0] == 'y' ? "Allowed!" : "Blocked!");
    return ans[0] == 'y';
}