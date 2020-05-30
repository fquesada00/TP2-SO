#include <stdint.h>
#include <stdarg.h>
#define MAX_INT_BYTES 40 // 1 int -> 4 bytes on 64 bit structure

//aca no puse el int fd como 1er parametro -> PREGUNTAR
extern int syswrite(const char * buff, int bytes); 
extern char * numToStr(int);
extern int strlen(char*);

/* return 1 if s iS greater than v
** 0 if s is equal to v
** -1 if s is lower than v */
int strcmp(const char * s, const char * v){
    for( ; s && v ; s++, v++){
        if(*s > *v) return 1;
        else if(*s < *v) return -1;
    }
    if(s && !v) return 1;
    else if(!s && v) return -1;
    return 0;
}

// variable params
void printf(const char * fmt, ...){
    va_list arg_list;
    va_start(arg_list, fmt);
    int q;
    char w;
    double e;
    char * p;
    while(fmt){
        if(*fmt == '%'){
            fmt++;
            if(!fmt) //ERROR
            switch (*fmt)
            {
            case 'd':
                syswrite(numToStr(va_arg(arg_list, int)), MAX_INT_BYTES);
                break;
            case 'c':
                syswrite(va_arg(arg_list, char), 1);
                break;
            //TODO
            case 'f':
                break;
            case 's':
                p = va_arg(arg_list, char *);
                syswrite(p, strlen(p) * sizeof(char));
                break;
            default:
                break;
            }
        }
        //TODO
        else if(*fmt == '\n'){

        }
        //TODO
        else if(*fmt == '\t'){

        }
        //TODO
        else if(*fmt == '\b'){

        }
        else{
            syswrite(fmt, sizeof(char)); //lee de a 1 byte (1 char)
        }
        fmt++;
    }

    va_end(arg_list);
}




void processorInfo(){

}