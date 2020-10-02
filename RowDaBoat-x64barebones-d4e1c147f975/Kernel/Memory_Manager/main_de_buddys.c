
//f auxs
void printListLevel(size_t l)
{
    a_block *start = headers[l];
    printf("printing %ld level list at %p, continue\n", l, start);
    while (start != NULL)
    {
        printf("%p-", start);
        start = start->nextBlock;
    }
    printf("\nend printing\n");
}

void printHeader(void *pointer)
{
    a_block *p = (a_block *)(pointer - HEADER_SIZE);
    printf("\nLevel: %ld\n", p->level);
}

int main()
{
    FILE * f = fopen("resultados_b3.txt","w+");
    for (int i = 0; i < 100; i++)
    {
        clock_t begin = clock();
        int *baseAddress = malloc(1024 * 1024 * 16);
        size_t size = 1024 * 512;

        void *testers[size];
        for (int i = 0; i < size; i++)
        {
            testers[i] = pMalloc(16, baseAddress);
            //printf("\n%ld\n",getBlockNumber((a_block *)((u_int8_t *)testers[i] - HEADER_SIZE),baseAddress));
        }

        for (int i = 0; i < size; i++)
        {
            pfree(testers[i], baseAddress);
        }
        for (int i = 0; i < LEVELS; i++)
        {
            //printListLevel(i);
        }
        clock_t end = clock();
        double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
        fprintf(f,"%f\n",time_spent);
    }
}