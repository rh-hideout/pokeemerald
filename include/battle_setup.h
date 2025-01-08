#ifndef GUARD_BATTLE_SETUP_H
#define GUARD_BATTLE_SETUP_H

#include "gym_leader_rematch.h"

#define REMATCHES_COUNT 5
#define MAX_STACK_SIZE 20

struct RematchTrainer
{
    u16 trainerIds[REMATCHES_COUNT];
    u16 mapGroup;
    u16 mapNum;
};

/*
the layout of the first byte can be confusing here
isDoubleBattle is the least lsb. msb is in the padding.
*/
typedef union PACKED TrainerBattleParameter
{
    struct PACKED _TrainerBattleParameter
    {
        u8 isDoubleBattle:1;
        u8 isRematch:1;
        u8 playMusicA:1;
        u8 playMusicB:1;
        u8 padding:4;
        u8 objEventLocalIdA;
        u16 battleOpponentA;
        u8* introTextA;
        u8* defeatTextA;
        u8* battleScriptRetAddrA;
        u8 objEventLocalIdB;
        u16 battleOpponentB;
        u8* introTextB;
        u8* defeatTextB;
        u8* battleScriptRetAddrB;
        u8* victoryText;
        u8* cannotBattleText;
    } params;
    u8 data[sizeof(struct _TrainerBattleParameter)];
} TrainerBattleParameter;

/*
    to use with old multi battle system
    used instead of TRAINER_BATTLE_SET_TRAINER_A/B modes of trainerbattle macro
*/
typedef union PACKED MultiTrainerBattleParameter
{
    struct PACKED _MultiTrainerBattleParameter
    {
        u16 battleOpponentA;
        u8* defeatTextA;
        u16 battleOpponentB;
        u8* defeatTextB;
    } params;
    u8 data[sizeof(struct _MultiTrainerBattleParameter)];
} MultiTrainerBattleParameter;

#define DebugPrintTrainerParams(battleParameter) DebugPrintfLevel(MGBA_LOG_DEBUG, "\nisDouble: %d\nplayMusicA: %d\nplayMusicB: %d\nisRematch: %d\npadding: %d\nlocalIdA: %d\ntrainerA: %d\nintroA: %x\ndefeatA: %x\neventA: %x\nlocalIdB: %d\ntrainerB: %d\nintroB: %x\ndefeatB: %x\neventB: %x\nvictory: %x\nnotBattle:%x\n", \
        battleParameter->params.isDoubleBattle, battleParameter->params.playMusicA, battleParameter->params.playMusicB, battleParameter->params.isRematch, battleParameter->params.padding, \
        battleParameter->params.objEventLocalIdA, battleParameter->params.battleOpponentA, battleParameter->params.introTextA, battleParameter->params.defeatTextA, battleParameter->params.battleScriptRetAddrA, \
        battleParameter->params.objEventLocalIdB, battleParameter->params.battleOpponentB, battleParameter->params.introTextB, battleParameter->params.defeatTextB, battleParameter->params.battleScriptRetAddrB, \
        battleParameter->params.victoryText, battleParameter->params.cannotBattleText)

typedef struct TrainerBattleScriptStack
{
    const u8* stack[MAX_STACK_SIZE];
    s8 stackPtr;
} TrainerBattleScriptStack;

#define TRAINER_BATTLE_PARAM gTrainerBattleParameter.params

extern const struct RematchTrainer gRematchTable[REMATCH_TABLE_ENTRIES];

extern TrainerBattleParameter gTrainerBattleParameter;
extern bool16 gHideCancelOptionOnPartySelect;
extern u16 gPartnerTrainerId;

void BattleSetup_StartWildBattle(void);
void BattleSetup_StartDoubleWildBattle(void);
void BattleSetup_StartBattlePikeWildBattle(void);
void BattleSetup_StartRoamerBattle(void);
void StartWallyTutorialBattle(void);
void BattleSetup_StartScriptedWildBattle(void);
void BattleSetup_StartScriptedDoubleWildBattle(void);
void BattleSetup_StartLatiBattle(void);
void BattleSetup_StartLegendaryBattle(void);
void StartGroudonKyogreBattle(void);
void StartRegiBattle(void);
u8 BattleSetup_GetTerrainId(void);
u8 GetWildBattleTransition(void);
u8 GetTrainerBattleTransition(void);
u8 GetSpecialBattleTransition(s32 id);
void ChooseStarter(void);
void ResetTrainerOpponentIds(void);
void SetMapVarsToTrainerA(void);
void SetMapVarsToTrainerB(void);
const u8 *BattleSetup_ConfigureTrainerBattle(const u8 *data, PtrStack *scrStack, bool32 isApproaching);
void ConfigureAndSetUpOneTrainerBattle(u8 trainerObjEventId, const u8 *trainerScript);
void ConfigureTwoTrainersBattle(u8 trainerObjEventId, const u8 *trainerScript);
void SetUpTwoTrainersBattle(void);
bool32 GetTrainerFlagFromScriptPointer(const u8 *data);
void SetTrainerFacingDirection(void);
bool8 GetTrainerFlag(void);
bool8 HasTrainerBeenFought(u16 trainerId);
void SetTrainerFlag(u16 trainerId);
void ClearTrainerFlag(u16 trainerId);
void BattleSetup_StartTrainerBattle(void);
void BattleSetup_StartRematchBattle(void);
void ShowTrainerIntroSpeech(void);
const u8 *BattleSetup_GetScriptAddrAfterBattle(void);
void BattleSetup_SetScriptAddrAfterBattle(const u8* ptr);
const u8 *BattleSetup_GetTrainerPostBattleScript(void);
void ShowTrainerCantBattleSpeech(void);
void PlayTrainerEncounterMusic(void);
const u8 *GetTrainerALoseText(void);
const u8 *GetTrainerBLoseText(void);
const u8 *GetTrainerWonSpeech(void);
void UpdateRematchIfDefeated(s32 rematchTableId);
void IncrementRematchStepCounter(void);
void TryUpdateRandomTrainerRematches(u16 mapGroup, u16 mapNum);
bool32 DoesSomeoneWantRematchIn(u16 mapGroup, u16 mapNum);
bool32 IsRematchTrainerIn(u16 mapGroup, u16 mapNum);
u16 GetLastBeatenRematchTrainerId(u16 trainerId);
bool8 ShouldTryRematchBattle(void);
bool8 IsTrainerReadyForRematch(void);
void ShouldTryGetTrainerScript(void);
u16 CountBattledRematchTeams(u16 trainerId);
void TrainerBattleLoadArgs(const u8* data);
void TrainerBattleLoadArgsTrainerA(const u8* data);
void TrainerBattleLoadArgsTrainerB(const u8* data);
void TrainerBattleLoadArgsSecondTrainer(const u8* data);
void MultiTrainerBattleLoadArgs(const u8* data);
void InitTrainerBattleVariables(void);

void DoStandardWildBattle_Debug(void);
void BattleSetup_StartTrainerBattle_Debug(void);
s32 TrainerIdToRematchTableId(const struct RematchTrainer *table, u16 trainerId);
s32 FirstBattleTrainerIdToRematchTableId(const struct RematchTrainer *table, u16 trainerId);
u16 GetRematchTrainerIdFromTable(const struct RematchTrainer *table, u16 firstBattleTrainerId);
const u8* BattleSetup_ConfigureFacilityTrainerBattle(u8 type, const u8* scriptEndPtr);

#endif // GUARD_BATTLE_SETUP_H
