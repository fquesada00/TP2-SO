/*#include <stdint.h>
#include "standardlib.h"
typedef struct charStack
{
    int size;
    char stack[256];
} charStack;
typedef struct doubleStack
{
    int size;
    double stack[64];
} doubleStack;

+ - * / ( )
static char precedenceTable[5][6] = {
    {1, 1, 0, 0, 0, 1}, //+
    {1, 1, 0, 0, 0, 1}, //-
    {1, 1, 1, 1, 0, 1}, //*
    {1, 1, 1, 1, 0, 1}, // /
    {0, 0, 0, 0, 0, 0}  // (
};

char peekChar(charStack *stack);
int sizeChar(charStack *stack);
int is_emptyChar(charStack *stack);
void pushChar(charStack *s, char e);
char popChar(charStack *s);
double peekDouble(doubleStack *stack);
int sizeDouble(doubleStack *stack);
int is_emptyDouble(doubleStack *stack);
void pushDouble(doubleStack *s, double e);
double popDouble(doubleStack *s);
int getPrecedence(char prev, char current);
int getIndex(char op);
double evaluate(char *op, double *numbers, int op_dim, int num_idx);
void calc()
{
    char c;
    double number = 0;
    int decimal = 0;
    int negative = 0;
    charStack s;
    s.size = 0;
    char posfix[64];
    double operands[64];
    int last_operand = 0;
    int operator_index = 0, operand_index = 0;
    int start = 0;
    double result = 0;
    char input[64];
    while (1)
    {
        printf("\nINPUT OPERATION:\n");
        if(scanf("%s=",&input)==1){
            
        }
        while ((c = getchar()) != '=')
        {
            putchar(c);
            if ((c >= '0' && c <= '9') || c == '.' || (c == '-' && !last_operand))
            {
                start = 1;
                if (c == '.')
                {
                    decimal = 1;
                    continue;
                }
                else if (c == '-')
                {
                    negative = 1;
                }
                if (!decimal)
                {
                    number *= 10;
                    number += (c - '0');
                }
                else
                {
                    decimal *= 10;
                    number += ((double)(c - '0')) / ((double)decimal);
                }
            }
            else if (c == '+' || c == '*' || c == '/' || c == '-' || c == '(' || c == ')')
            {
                if (negative)
                {
                    negative = 0;
                    number *= -1;
                }
                if (start)
                {
                    posfix[operator_index++] = 'n';
                    operands[operand_index++] = number;
                    number = 0;
                    decimal = 0;
                    last_operand = 1;
                    start = 0;
                }
                while (!is_emptyChar(&s) && getPrecedence(peekChar(&s), c))
                {
                    posfix[operator_index++] = popChar(&s);
                }
                if (c != ')')
                {
                    pushChar(&s, c);
                }
                else if (!is_emptyChar(&s))
                {
                    popChar(&s);
                }
                else
                    ; //ERROR
            }
            else
            {
                printf("ERROR");
            }
        }
        while (!is_emptyChar(&s))
        {
            if (peekChar(&s) != '(')
            {
                posfix[operator_index++] = popChar(&s);
            }
            else
                ; //ERROR
        }
        result = evaluate(posfix, operands, operator_index, operand_index);
        printf("\n%f\n", result);
        operand_index = 0;
        operator_index = 0;
    }
}
double evaluate(char *op, double *numbers, int op_dim, int num_idx)
{
    doubleStack d;
    int i = 0;
    int j = 0;
    double left, right, result;
    result;
    putchar('\n');
    printf("op dim vale %d",op_dim);
    printf(op);
    while (i < op_dim)
    {
        if (op[i++] == 'n'){
            pushDouble(&d, numbers[j++]);
            printf("pushee");
        }
        else
        {
            if(!is_emptyDouble(&d))
                right=popDouble(&d);
            else; //ERROR
            if(!is_emptyDouble(&d))
                left=popDouble(&d);
            else;//ERROR
            switch (op[i++])
            {
            case '+':
                pushDouble(&d,left + right);
                break;
            case '-':
                pushDouble(&d,left - right);
                break;
            case '*':
                pushDouble(&d,left * right);
                break;
            case '/':
                pushDouble(&d,left / right);
                break;
            default:
                break;
            }
        }
    }
    return popDouble(&d);
}
int getPrecedence(char prev, char current)
{
    int row = getIndex(prev);
    int column = getIndex(current);
    return precedenceTable[row][column];
}
int getIndex(char op)
{
    switch (op)
    {
    case '+':
        return 0;
        break;
    case '-':
        return 1;
    case '*':
        return 2;
    case '/':
        return 3;
    case '(':
        return 4;
    case ')':
        return 5;
    default:
        break;
    }
    return -1;
}

char peekChar(charStack *s)
{
    if (is_emptyChar(s))
        return 'e';
    return s->stack[s->size - 1];
}
double peekDouble(doubleStack *s)
{
    if (is_emptyDouble(s))
        return;
    return s->stack[s->size - 1];
}
int is_emptyChar(charStack *s)
{
    return s->size == 0;
}
int is_emptyDouble(doubleStack *s)
{
    return s->size == 0;
}
char popChar(charStack *s)
{
    if (is_emptyChar(s))
        return 'e';
    (s->size)--;
    return s->stack[s->size];
}
double popDouble(doubleStack *s)
{
    if (is_emptyDouble(s))
        return;
    (s->size)--;
    return s->stack[s->size];
}
void pushChar(charStack *s, char e)
{
    s->stack[(s->size)++] = e;
}
void pushDouble(doubleStack *s, double e)
{
    s->stack[(s->size)++] = e;
}
*/