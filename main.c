#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    const char *str_from = "Hello There";
    char *str_to = (char *)malloc(sizeof(char) * (strlen(str_from) + 1));
    memcpy(str_to, str_from, strlen(str_from) + 1);
    if(strlen(str_to) == strlen(str_from))
    {
        printf("Hello\n");
    }
    return 0;
}
