#ifndef _STRING_LIB_H
#define _STRING_LIB_H
#include <stdint.h>
int strcmp(const char*, const char*);
int strlen(char * s);
int strcpy(char * des, const char * src);
int doubleToString(double num, char *buffer);
int uintToBase(uint64_t value, char *buffer, uint32_t base);
int myAtoi(char *str);
int strcat(char* destination, const char* source);
#endif