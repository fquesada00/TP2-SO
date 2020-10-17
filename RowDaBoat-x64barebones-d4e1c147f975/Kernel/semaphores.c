// #include <stdint.h>
// #include <stddef.h>
// #include "include/semaphores.h"
// #include "include/scheduler.h"
// #include "include/list.h"
// #include "include/interrupts.h"
// #include "include/standardstring.h"
// #include "include/arrayCircular.h"

// extern uint32_t uintToBase(uint64_t value, char *buffer, uint32_t base);
// extern Header readyHeader;
// extern size_t _xchg(uint64_t pointer, size_t value);
// extern int syscall_write(int fd, const char *buff, size_t bytes);
// extern void unblockProcess(int fd, BlockReason reason);
// extern PCB *getPCB(size_t pid);
// extern void blockCurrent(int fd, BlockReason reason);
// extern void _int20();
// sem_t semaphores[MAX_SEM] = {0};
// extern int unblockProcessByPCB(PCB *pcb);
// void acquire(sem_t *sem)
// {
//     //puts("\naquire\n");
//     while (_xchg(&(sem->lock), 0) != 1)
//         _int20();
// }
// void release(sem_t *sem)
// {
//     //puts("\nrelease\n");
//     _xchg(&(sem->lock), 1);
// }
// void printPid(sem_t *sem)
// {
//     char buff[256] = {0};
//     for (int i = 0; i < MAX_BLOCKED_PID; i++)
//     {
//         uintToBase(sem->blockedPID[i]->PID, buff, 10);
//         puts(buff);
//         putChar('\n');
//     }
//     puts("==============");
//     putChar('\n');
// }

// int addPID(sem_t *sem, PCB *pcb)
// {
//     //printPid(sem);
//     for (int i = sem->idxBlockedPID, j = 0; j < MAX_BLOCKED_PID; i++, j++)
//     {
//         if (i == MAX_BLOCKED_PID)
//         {
//             i = 0;
//         }
//         if (sem->blockedPID[i] == NULL)
//         {
//             sem->blockedPID[i] = pcb;
//             //printPid(sem);

//             return 0;
//         }
//     }
//     return 1;
// }

// PCB *removePID(sem_t *sem)
// {
//     //printPid(sem);
//     if (sem->blockedPID[sem->idxBlockedPID] == NULL)
//         return NULL;
//     PCB *auxPCB = sem->blockedPID[sem->idxBlockedPID];
//     sem->blockedPID[sem->idxBlockedPID] = NULL;

//     if (sem->idxBlockedPID == (MAX_BLOCKED_PID - 1))
//         sem->idxBlockedPID = 0;
//     else
//         sem->idxBlockedPID++;
//     return auxPCB;
// }

// int sem_open(const char *name, size_t value, char created)
// {
//     int flag = 0, idx = 0, i = 0;
//     while (i < MAX_SEM)
//     {
//         if (strcmp(name, semaphores[i].name) == 0)
//         {
//             if (created)
//             {
//                 acquire(&semaphores[i]);
//                 if (strcmp(semaphores[i].name, "") == 0)
//                 {
//                     release(&semaphores[i]);
//                     return 1;
//                 }
//                 semaphores[i].value = value;
//                 //addPCB()
//                 semaphores[i].openPID[semaphores[i].idxOpenedPID++] = readyHeader.current->data.PID;
//                 release(&semaphores[i]);
//             }
//             return 0;
//         }
//         if (!flag && strcmp(semaphores[i].name, "") == 0)
//         {
//             idx = i;
//             flag = 1;
//         }
//         i++;
//     }
//     if (!flag)
//     {
//         return 1;
//     }
//     sem_t s = {0};
//     strcpy(s.name, name);
//     s.value = value;
//     s.lock = 1;
//     s.idxBlockedPID = 0;
//     semaphores[idx] = s;
//     return 0;
// }
// int sem_post(const char *name)
// {
//     int i = 0;
//     while (i < MAX_SEM && strcmp(name, semaphores[i].name) != 0)
//     {
//         i++;
//     }
//     if (i == MAX_SEM)
//     {
//         return 1;
//     }
//     acquire(&semaphores[i]);
//     if (strcmp(semaphores[i].name, "") == 0)
//     {
//         release(&semaphores[i]);
//         return 1;
//     }
//     semaphores[i].value++;
//     PCB *pcb = removePID(&semaphores[i]);
//     if (pcb == NULL)
//     {
//         //return 1;
//         release(&semaphores[i]);
//         return 0;
//     }
//     //unblock(pid)
//     unblockProcessByPCB(pcb);

//     release(&semaphores[i]);
//     return 0;
// }
// int sem_wait(const char *name)
// {
//     int i = 0;
//     while (i < MAX_SEM && strcmp(name, semaphores[i].name) != 0)
//     {
//         i++;
//     }
//     if (i == MAX_SEM)
//     {
//         return 1;
//     }
//     acquire(&semaphores[i]);
//     if (strcmp(semaphores[i].name, "") == 0)
//     {
//         release(&semaphores[i]);
//         return 1;
//     }
//     // if (semaphores[i].value > 0)
//     // {
//     //     semaphores[i].value--;
//     // }
//     // else
//     // {
//     /*puts("\nSoy el proceso ");
//         char buff2[256] = {0};
//         uintToBase(readyHeader.current->data.PID, buff2, 10);
//         puts(buff2);
//         putChar('\n');
//         addPID(&semaphores[i], &readyHeader.current->data);
//         release(&semaphores[i]);
//         int enter = 0;
//         //block current process
//         while(semaphores[i].value == 0){
//             if()
//             release(&semaphores[i]);
//             addPID(&semaphores[i], &readyHeader.current->data);
//             blockCurrent(i, SEM);
//             acquire(&semaphores[i]);
//         }*/
//     // addPID(&semaphores[i], &readyHeader.current->data);
//     // release(&semaphores[i]);
//     // blockCurrent(i, SEM);
//     // do
//     // {
//     //     acquire(&semaphores[i]);
//     //     addPID(&semaphores[i], &readyHeader.current->data);
//     //     release(&semaphores[i]);
//     //     blockCurrent(i, SEM);
//     // } while (semaphores[i].value == 0);
//     if (semaphores[i].value > 0)
//     {
//         PCB *toRem = removePID(&semaphores[i]);
//         if (toRem != NULL)
//         {
//             unblockProcessByPCB(toRem);
//             do
//             {
//                 addPID(&semaphores[i], &readyHeader.current->data);
//                 release(&semaphores[i]);
//                 blockCurrent(i, SEM);
//                 acquire(&semaphores[i]);
//             } while (semaphores[i].value == 0);
//         }
//     }
//     else
//     {
//         do
//         {
//             addPID(&semaphores[i], &readyHeader.current->data);
//             release(&semaphores[i]);
//             blockCurrent(i, SEM);
//             acquire(&semaphores[i]);
//         } while (semaphores[i].value == 0);
//     }
//     // do
//     // {
//     //     addPID(&semaphores[i], &readyHeader.current->data);
//     //     release(&semaphores[i]);
//     //     blockCurrent(i, SEM);
//     //     acquire(&semaphores[i]);
//     // } while (semaphores[i].value == 0);

//     //desbloqueo spinlock -> bloqueo spinlock -> addPid -> bloqueoProceso->desbloque spinlock -> bloqueo spinlock -> resto value (PROBLEMA: el unblock desbloquea ints, entonces puede haber inanicion xq no hice release)
//     semaphores[i].value--;
//     // }
//     /*puts("\nSoy el proceso ");
//     char buff2[256] = {0};
//     uintToBase(readyHeader.current->data.PID, buff2, 10);
//     puts(buff2);
//     putChar('\n');
//     char buff[256] = {0};
//     uintToBase(semaphores[i].value, buff, 10);
//     puts("ESTOY EN WAIT");
//     puts(buff);
//     putChar('\n');*/
//     release(&semaphores[i]);
//     return 0;
// }
// int sem_close(const char *name)
// {
//     int i = 0;
//     while (i < MAX_SEM && strcmp(semaphores[i].name, name) != 0)
//     {
//         i++;
//     }
//     if (i == MAX_SEM)
//     {
//         return 1;
//     }
//     acquire(&semaphores[i]);
//     //TODO BETTER
//     if (strcmp(semaphores[i].name, "") == 0)
//     {
//         release(&semaphores[i]);
//         return 1;
//     }
//     if (isListEmpty(&semaphores[i]))
//         semaphores[i].name[0] = 0;
//     release(&semaphores[i]);
//     return 0;
// }

// /*
// acquire
        
// */
// void sem()
// {
//     int print = 0, headerLenght = 0;
//     char headerBuffer[256] = {0};
//     PCB *pcb;
//     char buffer[256] = {0};
//     for (int i = 0; i < MAX_SEM; i++)
//     {
//         if (strcmp(semaphores[i].name, "") != 0)
//         {
//             if (!print)
//             {
//                 strcpy(buffer, "\n================================================================\n");
//                 syscall_write(1, buffer, strlen(buffer));
//                 strcpy(buffer, "NAME\t\t\t\t\t\t\tVALUE\t\t\tBLOCKED PIDs\n");
//                 int auxHeaderLenght = strlen(buffer);
//                 headerLenght = auxHeaderLenght - 1 - strlen("BLOCKED PIDs");
//                 syscall_write(1, buffer, auxHeaderLenght);
//                 for (int k = 0; k < headerLenght; k++)
//                 {
//                     headerBuffer[k] = " ";
//                 }
//                 print = 1;
//             }
//             strcpy(buffer, semaphores[i].name);
//             syscall_write(1, buffer, strlen(buffer));
//             strcpy(buffer, "\t\t\t\t\t\t");
//             syscall_write(1, buffer, strlen(buffer));
//             syscall_write(1, buffer, uintToBase(semaphores[i].value, buffer, 10));
//             strcpy(buffer, "\t\t\t");
//             syscall_write(1, buffer, strlen(buffer));
//             int k = 0;
//             for (int j = 0; j < MAX_BLOCKED_PID; j++)
//             {
//                 pcb = semaphores[i].blockedPID[j];
//                 if (pcb != NULL)
//                 {
//                     if (k)
//                     {
//                         syscall_write(1, headerBuffer, headerLenght);
//                     }
//                     syscall_write(1, buffer, uintToBase(pcb->PID, buffer, 10));
//                     syscall_write(1, "\n", 1);
//                     k = 1;
//                 }
//             }
//             if (!k)
//             {
//                 syscall_write(1, "\n", 1);
//             }
//         }
//     }
//     if (print)
//     {
//         strcpy(buffer, "\n================================================================\n");
//         syscall_write(1, buffer, strlen(buffer));
//     }
// }
// isListEmpty(sem_t *sem)
// {
//     for (size_t i = 0; i < MAX_BLOCKED_PID; i++)
//     {
//         if (sem->blockedPID[i] != NULL)
//             return 0;
//     }
//     return 1;
// }