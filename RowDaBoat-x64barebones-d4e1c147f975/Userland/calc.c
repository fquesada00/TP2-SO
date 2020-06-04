#include "standardlib.h"
typedef struct Stack
{
    int size;
    char stack[256];
} Stack;
/*+ - * / ( )*/
static char precedenceTable[5][6] = {
    {1, 1, 0, 0, 0, 1}, //+
    {1, 1, 0, 0, 0, 1}, //-
    {1, 1, 1, 1, 0, 1}, //*
    {1, 1, 1, 1, 0, 1}, // /
    {0, 0, 0, 0, 0, 0}  // (
};

char peek(Stack *stack);
int size(Stack *stack);
int is_empty(Stack *stack);
void push(Stack *s, char e);
char pop(Stack *s);
int getPrecedence(char prev, char current);
int getIndex(char op);
double evaluate(char *op, double *numbers, int op_dim, int num_idx);
void calc()
{
    char c;
    double number = 0;
    int decimal = 0;
    int negative = 0;
    Stack s;
    s.size = 0;
    char operators[64];
    double operands[64];
    int last_operand = 0;
    int operator_index = 0, operand_index = 0;
    int start = 0;
    double result=0;
    while (1)
    {
        printf("\nINPUT OPERATION:\n");
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
                    operands[operand_index++] = number;
                    number = 0;
                    decimal = 0;
                    last_operand = 1;
                    start = 0;
                }
                while (!is_empty(&s) && getPrecedence(peek(&s), c))
                {
                    operators[operator_index++] = pop(&s);
                }
                if (c != ')')
                {
                    push(&s, c);
                }
                else if (!is_empty(&s))
                {
                    pop(&s);
                }
                else
                    ; //ERROR
            }
            else
            {
                printf("ERROR");
            }
        }
        while (!is_empty(&s))
        {
            if (peek(&s) != '(')
            {
                operators[operator_index++] = pop(&s);
            }
            else
                ; //ERROR
        }
        result = evaluate(operators,operands,operator_index,operand_index);
        printf("\n%f\n",result);
    }
}
double evaluate(char *op, double *numbers, int op_dim, int num_idx)
{
    int i = 0;
    int j = num_idx - 1;
    double left, right, result;
    result = numbers[j--];
    while (i < op_dim)
    {
        right = result;
        left = numbers[j--];
        switch (op[i++])
        {
        case '+':
            result = left + right;
            break;
        case '-':
            result = left - right;
            break;
        case '*':
            result = left * right;
            break;
        case '/':
            result = left / right;
            break;
        default:
            break;
        }
    }
    return result;
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

char peek(Stack *s)
{
    if (is_empty(s))
        return 'e';
    return s->stack[s->size - 1];
}

int is_empty(Stack *s)
{
    return s->size == 0;
}

char pop(Stack *s)
{
    if (is_empty(s))
        return 'e';
    s->size--;
    return s->stack[s->size];
}
void push(Stack *s, char e)
{
    s->stack[s->size++] = e;
}