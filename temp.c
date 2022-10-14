#include <stdio.h>
#include <string.h>

int main(){
    char cmd[10];
    scanf("%s", cmd);
    printf("%d\n", strcmp(cmd, "hi"));
    printf("%d", cmd == "hi");
}