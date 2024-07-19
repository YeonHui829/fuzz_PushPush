#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>




int main(){
    char * ptr;
    char a [255] = "0";
    int z;
    int c = sscanf(a, "%d", &z);
    printf("a = %d\n", c);
    printf("%d", z);
    ptr = strtok(a, ":");

    // printf("%s", ptr);
    // ptr = strtok(NULL, ":");
    // printf("%s", ptr);

    
}