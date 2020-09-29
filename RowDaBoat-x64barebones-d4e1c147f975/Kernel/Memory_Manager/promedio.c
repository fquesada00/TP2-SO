#include <stdio.h>
#include <stdlib.h>

int main(int argc, char ** argv){
    double total = 0;
    int count = 0;
    char *c;
    FILE * f = fopen(argv[1],"r");
    char buffer[256] = {0};
    while(fgets(buffer, 256, f) != NULL){
        double ans = strtod(buffer, &c);
        total += ans;
        count++;
    }
    printf("Promedio de %d = %f\n", count, total/count);
}