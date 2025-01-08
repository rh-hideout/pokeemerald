#ifndef GUARD_SCRIPT_H
#define GUARD_SCRIPT_H

struct ScriptContext;

typedef bool8 (*ScrCmdFunc)(struct ScriptContext *);
typedef u8 Script[];

struct ScriptContext
{
    u8 stackDepth;
    u8 mode;
    u8 comparisonResult;
    u8 (*nativePtr)(void);
    const u8 *scriptPtr;
    const u8 *stack[20];
    ScrCmdFunc *cmdTable;
    ScrCmdFunc *cmdTableEnd;
    u32 data[4];
};

#define ScriptReadByte(ctx) (*(ctx->scriptPtr++))

#define DebugPrintScriptStack \
do { \
    u8 i; \
    DebugPrintfLevel(MGBA_LOG_DEBUG, "_______ScriptStack________"); \
    for (i = ctx->stackDepth; i > 0; i--) { \
        DebugPrintfLevel(MGBA_LOG_DEBUG, "%d: %x", i-1, ctx->stack[i-1]); \
    } \
    DebugPrintfLevel(MGBA_LOG_DEBUG, "_______StackFloor_________"); \
} while(0);

#define DebugPrintGlobalScriptStack \
do { \
    u8 i; \
    DebugPrintfLevel(MGBA_LOG_DEBUG, "_______GlobalScriptStack________"); \
    for (i = sGlobalScriptContext.stackDepth; i > 0; i--) { \
        DebugPrintfLevel(MGBA_LOG_DEBUG, "%d: %x", i-1, sGlobalScriptContext.stack[i-1]); \
    } \
    DebugPrintfLevel(MGBA_LOG_DEBUG, "_______GlobalStackFloor_________"); \
} while(0);

void InitScriptContext(struct ScriptContext *ctx, void *cmdTable, void *cmdTableEnd);
u8 SetupBytecodeScript(struct ScriptContext *ctx, const u8 *ptr);
void SetupNativeScript(struct ScriptContext *ctx, bool8 (*ptr)(void));
void StopScript(struct ScriptContext *ctx);
bool8 RunScriptCommand(struct ScriptContext *ctx);
bool8 ScriptPush(struct ScriptContext *ctx, const u8 *ptr);
const u8 *ScriptPop(struct ScriptContext *ctx);
void ScriptJump(struct ScriptContext *ctx, const u8 *ptr);
void ScriptCall(struct ScriptContext *ctx, const u8 *ptr);
void ScriptReturn(struct ScriptContext *ctx);
u16 ScriptReadHalfword(struct ScriptContext *ctx);
u32 ScriptReadWord(struct ScriptContext *ctx);
u32 ScriptPeekWord(struct ScriptContext *ctx);
void LockPlayerFieldControls(void);
void UnlockPlayerFieldControls(void);
bool8 ArePlayerFieldControlsLocked(void);
void ScriptContext_Init(void);
bool8 ScriptContext_IsEnabled(void);
bool8 ScriptContext_RunScript(void);
void ScriptContext_SetupScript(const u8 *ptr);
void ScriptContext_Stop(void);
void ScriptContext_Enable(void);
bool32 ScriptContext_PushFromStack(PtrStack *stack);
void RunScriptImmediately(const u8 *ptr);
u8 *MapHeaderGetScriptTable(u8 tag);
void MapHeaderRunScriptType(u8 tag);
u8 *MapHeaderCheckScriptTable(u8 tag);
void RunOnLoadMapScript(void);
void RunOnTransitionMapScript(void);
void RunOnResumeMapScript(void);
void RunOnReturnToFieldMapScript(void);
void RunOnDiveWarpMapScript(void);
bool8 TryRunOnFrameMapScript(void);
void TryRunOnWarpIntoMapScript(void);
u32 CalculateRamScriptChecksum(void);
void ClearRamScript(void);
bool8 InitRamScript(const u8 *script, u16 scriptSize, u8 mapGroup, u8 mapNum, u8 objectId);
const u8 *GetRamScript(u8 objectId, const u8 *script);
bool32 ValidateSavedRamScript(void);
u8 *GetSavedRamScriptIfValid(void);
void InitRamScript_NoObjectEvent(u8 *script, u16 scriptSize);

// srccmd.h
void SetMovingNpcId(u16 npcId);

extern u8 gMsgIsSignPost;
extern u8 gMsgBoxIsCancelable;

#endif // GUARD_SCRIPT_H
