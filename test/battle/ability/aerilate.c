#include "global.h"
#include "test/battle.h"

ASSUMPTIONS
{
    ASSUME(GetMoveType(MOVE_TACKLE) == TYPE_NORMAL);
    ASSUME(GetMovePower(MOVE_TACKLE) > 0);
}

SINGLE_BATTLE_TEST("Aerilate turns a Normal-type move into Flying-type move")
{
    GIVEN {
        PLAYER(SPECIES_MEGANIUM);
        OPPONENT(SPECIES_SALAMENCE) { Item(ITEM_SALAMENCITE); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_TACKLE, gimmick: GIMMICK_MEGA); }
    } SCENE {
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_MEGA_EVOLUTION, opponent);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TACKLE, opponent);
        MESSAGE("It's super effective!");
    }
}

SINGLE_BATTLE_TEST("Aerilate can not turn certain moves into Flying type moves")
{
    u32 move;
    PARAMETRIZE { move = MOVE_WEATHER_BALL; }
    // PARAMETRIZE { move = MOVE_NATURAL_GIFT; } TODO: handle this case via Skill Swap
    PARAMETRIZE { move = MOVE_JUDGMENT; }
    PARAMETRIZE { move = MOVE_TECHNO_BLAST; }
    PARAMETRIZE { move = MOVE_REVELATION_DANCE; }
    PARAMETRIZE { move = MOVE_MULTI_ATTACK; }
    PARAMETRIZE { move = MOVE_TERRAIN_PULSE; }
    GIVEN {
        PLAYER(SPECIES_MEGANIUM);
        OPPONENT(SPECIES_SALAMENCE) { Item(ITEM_SALAMENCITE); }
    } WHEN {
        TURN { MOVE(opponent, move, gimmick: GIMMICK_MEGA); }
    } SCENE {
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_MEGA_EVOLUTION, opponent);
        ANIMATION(ANIM_TYPE_MOVE, move, opponent);
        NONE_OF {
            MESSAGE("It's super effective!");
        }
    }
}

SINGLE_BATTLE_TEST("Aerilate boosts power of affected moves by 20% (Gen7+) or 30% (Gen1-6)", s16 damage)
{
    u32 ability, genConfig;
    PARAMETRIZE { ability = ABILITY_HYPER_CUTTER;   genConfig = GEN_7; }
    PARAMETRIZE { ability = ABILITY_HYPER_CUTTER;   genConfig = GEN_6; }
    PARAMETRIZE { ability = ABILITY_AERILATE;       genConfig = GEN_7; }
    PARAMETRIZE { ability = ABILITY_AERILATE;       genConfig = GEN_6; }

    GIVEN {
        WITH_CONFIG(GEN_CONFIG_ATE_MULTIPLIER, genConfig);
        PLAYER(SPECIES_PINSIR) { Ability(ability); Moves(MOVE_TACKLE); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_TACKLE); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        if (genConfig >= GEN_7)
            EXPECT_MUL_EQ(results[0].damage, Q_4_12(1.2), results[2].damage); // No STAB
        else
            EXPECT_MUL_EQ(results[1].damage, Q_4_12(1.3), results[3].damage); // No STAB
    }
}

// Gen 6-7
SINGLE_BATTLE_TEST("Aerilate overrides Electrify (Gen6-7)", s16 damage)
{
    u32 ability, genConfig;
    PARAMETRIZE { ability = ABILITY_HYPER_CUTTER;   genConfig = GEN_7; }
    PARAMETRIZE { ability = ABILITY_HYPER_CUTTER;   genConfig = GEN_6; }
    PARAMETRIZE { ability = ABILITY_AERILATE;       genConfig = GEN_7; }
    PARAMETRIZE { ability = ABILITY_AERILATE;       genConfig = GEN_6; }

    GIVEN {
        ASSUME(GetMoveEffect(MOVE_ELECTRIFY) == EFFECT_ELECTRIFY);
        WITH_CONFIG(GEN_CONFIG_ATE_MULTIPLIER, genConfig);
        WITH_CONFIG(GEN_CONFIG_ATE_OVERRIDE, genConfig);
        PLAYER(SPECIES_PINSIR) { Ability(ability); Moves(MOVE_TACKLE); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, MOVE_ELECTRIFY); MOVE(player, MOVE_TACKLE); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        if (genConfig >= GEN_7)
            EXPECT_MUL_EQ(results[0].damage, Q_4_12(1.2), results[2].damage); // No STAB
        else
            EXPECT_MUL_EQ(results[1].damage, Q_4_12(1.3), results[3].damage); // No STAB
    }
}

SINGLE_BATTLE_TEST("Aerilate overrides Ion Deluge (Gen6-7)", s16 damage)
{
    u32 ability, genConfig;
    PARAMETRIZE { ability = ABILITY_HYPER_CUTTER;   genConfig = GEN_7; }
    PARAMETRIZE { ability = ABILITY_HYPER_CUTTER;   genConfig = GEN_6; }
    PARAMETRIZE { ability = ABILITY_AERILATE;       genConfig = GEN_7; }
    PARAMETRIZE { ability = ABILITY_AERILATE;       genConfig = GEN_6; }

    GIVEN {
        ASSUME(GetMoveEffect(MOVE_ION_DELUGE) == EFFECT_ION_DELUGE);
        WITH_CONFIG(GEN_CONFIG_ATE_MULTIPLIER, genConfig);
        WITH_CONFIG(GEN_CONFIG_ATE_OVERRIDE, genConfig);
        PLAYER(SPECIES_PINSIR) { Ability(ability); Moves(MOVE_TACKLE); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, MOVE_ION_DELUGE); MOVE(player, MOVE_TACKLE); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        if (genConfig >= GEN_7)
            EXPECT_MUL_EQ(results[0].damage, Q_4_12(1.2), results[2].damage); // No STAB
        else
            EXPECT_MUL_EQ(results[1].damage, Q_4_12(1.3), results[3].damage); // No STAB
    }
}

// Gen 8+
SINGLE_BATTLE_TEST("Aerilate doesn't override Electrify (Gen8+)", s16 damage)
{
    // No mon with Aerilate exists in Gen8+, but probably behaves similar to Pixilate, which does.
    u32 ability, genConfig;
    PARAMETRIZE { ability = ABILITY_HYPER_CUTTER;   genConfig = GEN_7; }
    PARAMETRIZE { ability = ABILITY_HYPER_CUTTER;   genConfig = GEN_8; }
    PARAMETRIZE { ability = ABILITY_AERILATE;       genConfig = GEN_7; }
    PARAMETRIZE { ability = ABILITY_AERILATE;       genConfig = GEN_8; }

    GIVEN {
        ASSUME(GetMoveEffect(MOVE_ELECTRIFY) == EFFECT_ELECTRIFY);
        WITH_CONFIG(GEN_CONFIG_ATE_MULTIPLIER, genConfig);
        WITH_CONFIG(GEN_CONFIG_ATE_OVERRIDE, genConfig);
        PLAYER(SPECIES_PINSIR) { Ability(ability); Moves(MOVE_TACKLE); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, MOVE_ELECTRIFY); MOVE(player, MOVE_TACKLE); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        if (genConfig >= GEN_7)
            EXPECT_MUL_EQ(results[0].damage, Q_4_12(1.2), results[2].damage); // No STAB
        else
            EXPECT_EQ(results[1].damage, results[3].damage);
    }
}

SINGLE_BATTLE_TEST("Aerilate doesn't override Ion Deluge (Gen8+)", s16 damage)
{
    // Ion Deluge doesn't exist in Gen 8+, but we probably could assume it behaves similar to under Electrify. TODO: Test by hacking SV.
    u32 ability, genConfig;
    PARAMETRIZE { ability = ABILITY_HYPER_CUTTER;   genConfig = GEN_7; }
    PARAMETRIZE { ability = ABILITY_HYPER_CUTTER;   genConfig = GEN_8; }
    PARAMETRIZE { ability = ABILITY_AERILATE;       genConfig = GEN_7; }
    PARAMETRIZE { ability = ABILITY_AERILATE;       genConfig = GEN_8; }

    GIVEN {
        ASSUME(GetMoveEffect(MOVE_ION_DELUGE) == EFFECT_ION_DELUGE);
        WITH_CONFIG(GEN_CONFIG_ATE_MULTIPLIER, genConfig);
        WITH_CONFIG(GEN_CONFIG_ATE_OVERRIDE, genConfig);
        PLAYER(SPECIES_PINSIR) { Ability(ability); Moves(MOVE_TACKLE); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, MOVE_ION_DELUGE); MOVE(player, MOVE_TACKLE); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        if (genConfig >= GEN_7)
            EXPECT_MUL_EQ(results[0].damage, Q_4_12(1.2), results[2].damage); // No STAB
        else
            EXPECT_EQ(results[1].damage,  results[3].damage);
    }
}

SINGLE_BATTLE_TEST("Aerilate doesn't affect Weather Ball's type", s16 damage)
{
    u16 move, ability;
    PARAMETRIZE { move = MOVE_CELEBRATE; ability = ABILITY_HYPER_CUTTER; }
    PARAMETRIZE { move = MOVE_SUNNY_DAY; ability = ABILITY_HYPER_CUTTER; }
    PARAMETRIZE { move = MOVE_CELEBRATE; ability = ABILITY_AERILATE; }
    PARAMETRIZE { move = MOVE_SUNNY_DAY; ability = ABILITY_AERILATE; }
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_WEATHER_BALL) == EFFECT_WEATHER_BALL);
        ASSUME(GetMoveType(MOVE_WEATHER_BALL) == TYPE_NORMAL);
        ASSUME(gSpeciesInfo[SPECIES_PINSIR].types[0] == TYPE_BUG);
        PLAYER(SPECIES_PINSIR) { Ability(ability); }
        OPPONENT(SPECIES_PINSIR);
    } WHEN {
        TURN { MOVE(player, move); }
        TURN { MOVE(player, MOVE_WEATHER_BALL); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
        if (move == MOVE_SUNNY_DAY)
            MESSAGE("It's super effective!");
    } FINALLY {
        EXPECT_MUL_EQ(results[0].damage, Q_4_12(6.0), results[1].damage); // double base power + type effectiveness + sun 50% boost
        EXPECT_MUL_EQ(results[2].damage, Q_4_12(6.0), results[3].damage);
        EXPECT_EQ(results[0].damage, results[2].damage);
        EXPECT_EQ(results[1].damage, results[3].damage);
    }
}

SINGLE_BATTLE_TEST("Aerilate doesn't affect Natural Gift's type")
{
    u16 ability;
    PARAMETRIZE { ability = ABILITY_HYPER_CUTTER; }
    PARAMETRIZE { ability = ABILITY_AERILATE; }
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_NATURAL_GIFT) == EFFECT_NATURAL_GIFT);
        ASSUME(gNaturalGiftTable[ITEM_TO_BERRY(ITEM_ORAN_BERRY)].type == TYPE_POISON);
        ASSUME(gSpeciesInfo[SPECIES_BELDUM].types[0] == TYPE_STEEL);
        PLAYER(SPECIES_PINSIR) { Ability(ability); Item(ITEM_ORAN_BERRY); }
        OPPONENT(SPECIES_BELDUM);
    } WHEN {
        TURN { MOVE(player, MOVE_NATURAL_GIFT); }
    } SCENE {
        NOT { ANIMATION(ANIM_TYPE_MOVE, MOVE_NATURAL_GIFT, player); }
        MESSAGE("It doesn't affect the opposing Beldum…");
    }
}

SINGLE_BATTLE_TEST("Aerilate doesn't affect Judgment / Techno Blast / Multi-Attack's type")
{
    u16 move, item;
    PARAMETRIZE { move = MOVE_JUDGMENT; item = ITEM_ZAP_PLATE; }
    PARAMETRIZE { move = MOVE_TECHNO_BLAST; item = ITEM_SHOCK_DRIVE; }
    PARAMETRIZE { move = MOVE_MULTI_ATTACK; item = ITEM_ELECTRIC_MEMORY; }
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_JUDGMENT) == EFFECT_CHANGE_TYPE_ON_ITEM);
        ASSUME(GetMoveEffect(MOVE_TECHNO_BLAST) == EFFECT_CHANGE_TYPE_ON_ITEM);
        ASSUME(GetMoveEffect(MOVE_MULTI_ATTACK) == EFFECT_CHANGE_TYPE_ON_ITEM);
        ASSUME(gItemsInfo[ITEM_ZAP_PLATE].holdEffect == HOLD_EFFECT_PLATE);
        ASSUME(gItemsInfo[ITEM_ZAP_PLATE].secondaryId == TYPE_ELECTRIC);
        ASSUME(gItemsInfo[ITEM_SHOCK_DRIVE].holdEffect == HOLD_EFFECT_DRIVE);
        ASSUME(gItemsInfo[ITEM_SHOCK_DRIVE].secondaryId == TYPE_ELECTRIC);
        ASSUME(gItemsInfo[ITEM_ELECTRIC_MEMORY].holdEffect == HOLD_EFFECT_MEMORY);
        ASSUME(gItemsInfo[ITEM_ELECTRIC_MEMORY].secondaryId == TYPE_ELECTRIC);
        ASSUME(gSpeciesInfo[SPECIES_DIGLETT].types[0] == TYPE_GROUND);
        PLAYER(SPECIES_PINSIR) { Ability(ABILITY_AERILATE); Item(item); }
        OPPONENT(SPECIES_DIGLETT);
    } WHEN {
        TURN { MOVE(player, move); }
    } SCENE {
        NOT { ANIMATION(ANIM_TYPE_MOVE, move, player); }
        MESSAGE("It doesn't affect the opposing Diglett…");
    }
}

SINGLE_BATTLE_TEST("Aerilate doesn't affect Hidden Power's type")
{
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_HIDDEN_POWER) == EFFECT_HIDDEN_POWER);
        ASSUME(gTypesInfo[TYPE_ELECTRIC].isHiddenPowerType == TRUE);
        ASSUME(gSpeciesInfo[SPECIES_DIGLETT].types[0] == TYPE_GROUND);
        PLAYER(SPECIES_PINSIR) { Ability(ABILITY_AERILATE); HPIV(31); AttackIV(31); DefenseIV(31); SpAttackIV(30); SpDefenseIV(31); SpeedIV(31); } // HP Electric
        OPPONENT(SPECIES_DIGLETT);
    } WHEN {
        TURN { MOVE(player, MOVE_HIDDEN_POWER); }
    } SCENE {
        NOT { ANIMATION(ANIM_TYPE_MOVE, MOVE_HIDDEN_POWER, player); }
        MESSAGE("It doesn't affect the opposing Diglett…");
    }
}

TO_DO_BATTLE_TEST("Aerilate doesn't affect Tera Starstorm's type");
TO_DO_BATTLE_TEST("Aerilate doesn't affect Max Strike's type");
TO_DO_BATTLE_TEST("Aerilate doesn't affect Terrain Pulse's type");
TO_DO_BATTLE_TEST("Aerilate doesn't affect damaging Z-Move types");
TO_DO_BATTLE_TEST("(DYNAMAX) Aerilate turns Max Strike into Max Airstream"); // All other -ate abilities do this, so interpolating this as no Aerilate mon is available in a Dynamax game
