#define EOF -1
extern syswrite(int fd, char * buff, int size);
int filter(int argc, char **argv)
{
    int idx = 0;
    char c;
    char writeB[256] = {0};
    while((c = getchar()) != EOF){
        if(c != 'a' && c != 'e' && c != 'i' && c != 'o' && c != 'u'){
            writeB[idx++] = c;
            if(c == '\n'){
                writeB[idx++] = '\0';
                syswrite(1,writeB,idx);
                idx = 0;
            }
        }
    }
    syswrite(1,writeB,idx);
}