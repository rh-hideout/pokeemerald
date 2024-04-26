#ifndef GUARD_BATTLE_DYNAMAX_H
#define GUARD_BATTLE_DYNAMAX_H

#define DYNAMAX_TURNS_COUNT	3

enum MaxMoveEffect
{
	MAX_EFFECT_NONE,
	MAX_EFFECT_RAISE_TEAM_ATTACK,
	MAX_EFFECT_RAISE_TEAM_DEFENSE,
	MAX_EFFECT_RAISE_TEAM_SPEED,
	MAX_EFFECT_RAISE_TEAM_SP_ATK,
	MAX_EFFECT_RAISE_TEAM_SP_DEF,
	MAX_EFFECT_LOWER_ATTACK,
	MAX_EFFECT_LOWER_DEFENSE,
	MAX_EFFECT_LOWER_SPEED,
	MAX_EFFECT_LOWER_SP_ATK,
	MAX_EFFECT_LOWER_SP_DEF,
	MAX_EFFECT_SUN,
	MAX_EFFECT_RAIN,
	MAX_EFFECT_SANDSTORM,
	MAX_EFFECT_HAIL,
	MAX_EFFECT_MISTY_TERRAIN,
	MAX_EFFECT_GRASSY_TERRAIN,
	MAX_EFFECT_ELECTRIC_TERRAIN,
	MAX_EFFECT_PSYCHIC_TERRAIN,
	MAX_EFFECT_VINE_LASH,
	MAX_EFFECT_WILDFIRE,
	MAX_EFFECT_CANNONADE,
	MAX_EFFECT_EFFECT_SPORE_FOES,
	MAX_EFFECT_PARALYZE_FOES,
	MAX_EFFECT_CONFUSE_FOES_PAY_DAY,
	MAX_EFFECT_CRIT_PLUS,
	MAX_EFFECT_MEAN_LOOK,
	MAX_EFFECT_AURORA_VEIL,
	MAX_EFFECT_INFATUATE_FOES,
	MAX_EFFECT_RECYCLE_BERRIES,
	MAX_EFFECT_POISON_FOES,
	MAX_EFFECT_STEALTH_ROCK,
	MAX_EFFECT_DEFOG,
	MAX_EFFECT_POISON_PARALYZE_FOES,
	MAX_EFFECT_HEAL_TEAM,
	MAX_EFFECT_SPITE,
	MAX_EFFECT_GRAVITY,
	MAX_EFFECT_VOLCALITH,
	MAX_EFFECT_SANDBLAST_FOES,
	MAX_EFFECT_YAWN_FOE,
	MAX_EFFECT_LOWER_EVASIVENESS_FOES,
	MAX_EFFECT_AROMATHERAPY,
	MAX_EFFECT_CONFUSE_FOES,
	MAX_EFFECT_STEELSURGE,
	MAX_EFFECT_TORMENT_FOES,
	MAX_EFFECT_LOWER_SPEED_2_FOES,
	MAX_EFFECT_FIRE_SPIN_FOES,
	MAX_EFFECT_FIXED_POWER,
	MAX_EFFECT_BYPASS_PROTECT,
};

bool32 CanDynamax(u32 battler);
bool32 IsGigantamaxed(u32 battler);
void ApplyDynamaxHPMultiplier(u32 battler, struct Pokemon* mon);
void ActivateDynamax(u32 battler);
u16 GetNonDynamaxHP(u32 battler);
u16 GetNonDynamaxMaxHP(u32 battler);
void UndoDynamax(u32 battler);
bool32 IsMoveBlockedByMaxGuard(u32 move);
bool32 IsMoveBlockedByDynamax(u32 move);

bool32 ShouldUseMaxMove(u32 battler, u32 baseMove);
u16 GetMaxMove(u32 battler, u32 baseMove);
u8 GetMaxMovePower(u32 move);
bool32 IsMaxMove(u32 move);
void ChooseDamageNonTypesString(u8 type);

void BS_UpdateDynamax(void);
void BS_SetMaxMoveEffect(void);
void BS_SetSteelsurge(void);
void BS_TrySetStatus1(void);
void BS_TrySetStatus2(void);
void BS_DamageNonTypes(void);
void BS_HealOneSixth(void);
void BS_TryRecycleBerry(void);
void BS_JumpIfDynamaxed(void);

#endif
