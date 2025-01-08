#include "global.h"
#include "stack.h"
#include "malloc.h"

void PtrStackInit(PtrStack *s)
{
    memset(s->stack, 0, STACK_SIZE * sizeof(void*));
    s->stackPtr = -1;
}

void* PtrStackPop(PtrStack *s)
{
    if (s->stackPtr < 0) 
        return NULL;

    return s->stack[s->stackPtr--];
}

bool32 PtrStackPush(PtrStack *s, void* ptr)
{
    if (s->stackPtr + 1 >= STACK_SIZE)
        return FALSE;

    s->stack[++(s->stackPtr)] = ptr;
    return TRUE;
}

const u8* PtrStackPopU8(PtrStack *s)
{
    return (const u8*)PtrStackPop(s);
}

bool32 PtrStackPushU8(PtrStack *s, const u8* ptr)
{
    return PtrStackPush(s, (void*)ptr);
}
