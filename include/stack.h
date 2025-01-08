#ifndef GUARD_STACK_H
#define GUARD_STACK_H

#define STACK_SIZE 0xFF

typedef struct PtrStack
{
    void* stack[STACK_SIZE];
    s32 stackPtr;
} PtrStack;

#define DebugPrintStack(s) \
do { \
    s16 i; \
    DebugPrintfLevel(MGBA_LOG_DEBUG, "_______StackTop______");  \
    for (i = s->stackPtr; i > -1; i--) {  \
        DebugPrintfLevel(MGBA_LOG_DEBUG, "%d: %x", i, s->stack[i]);  \
    }   \
    DebugPrintfLevel(MGBA_LOG_DEBUG, "_______StackFloor______");  \
} while(0) 

void PtrStackInit(PtrStack* stack);
void* PtrStackPop(PtrStack* stack);
bool32 PtrStackPush(PtrStack* stack, void* ptr);
const u8* PtrStackPopU8(PtrStack *stack);
bool32 PtrStackPushU8(PtrStack *stack, const u8* ptr);

#endif // GUARD_STACK_H
