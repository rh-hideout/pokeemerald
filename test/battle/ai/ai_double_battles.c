#include "global.h"
#include "test/battle.h"
#include "battle_ai_util.h"

AI_DOUBLE_BATTLE_TEST("AI won't use a Weather changing move if partner already chose such move")
{
    u32 j, k;
    static const u16 weatherMoves[] = {MOVE_SUNNY_DAY, MOVE_HAIL, MOVE_RAIN_DANCE, MOVE_SANDSTORM, MOVE_SNOWSCAPE};
    u16 weatherMoveLeft = MOVE_NONE, weatherMoveRight = MOVE_NONE;

    for (j = 0; j < ARRAY_COUNT(weatherMoves); j++)
    {
        for (k = 0; k < ARRAY_COUNT(weatherMoves); k++)
        {
            PARAMETRIZE { weatherMoveLeft = weatherMoves[j]; weatherMoveRight = weatherMoves[k]; }
        }
    }

    GIVEN {
        AI_FLAGS(AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_CHECK_VIABILITY | AI_FLAG_TRY_TO_FAINT);
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET) { Moves(weatherMoveLeft); }
        OPPONENT(SPECIES_WOBBUFFET) { Moves(MOVE_TACKLE, weatherMoveRight); }
    } WHEN {
            TURN {  NOT_EXPECT_MOVE(opponentRight, weatherMoveRight);
                    SCORE_LT_VAL(opponentRight, weatherMoveRight, AI_SCORE_DEFAULT, target:playerLeft);
                    SCORE_LT_VAL(opponentRight, weatherMoveRight, AI_SCORE_DEFAULT, target:playerRight);
                    SCORE_LT_VAL(opponentRight, weatherMoveRight, AI_SCORE_DEFAULT, target:opponentLeft);
                 }
    }
}

AI_DOUBLE_BATTLE_TEST("AI will not use Helping Hand if partner does not have any damage moves")
{
    u16 move1 = MOVE_NONE, move2 = MOVE_NONE, move3 = MOVE_NONE, move4 = MOVE_NONE;

    PARAMETRIZE { move1 = MOVE_LEER; move2 = MOVE_TOXIC; }
    PARAMETRIZE { move1 = MOVE_HELPING_HAND; move2 = MOVE_PROTECT; }
    PARAMETRIZE { move1 = MOVE_ACUPRESSURE; move2 = MOVE_DOUBLE_TEAM; move3 = MOVE_TOXIC; move4 = MOVE_PROTECT; }

    GIVEN {
        AI_FLAGS(AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_CHECK_VIABILITY | AI_FLAG_TRY_TO_FAINT);
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET) { Moves(MOVE_HELPING_HAND, MOVE_TACKLE); }
        OPPONENT(SPECIES_WOBBUFFET) { Moves(move1, move2, move3, move4); }
    } WHEN {
            TURN {  NOT_EXPECT_MOVE(opponentLeft, MOVE_HELPING_HAND);
                    SCORE_LT_VAL(opponentLeft, MOVE_HELPING_HAND, AI_SCORE_DEFAULT, target:playerLeft);
                    SCORE_LT_VAL(opponentLeft, MOVE_HELPING_HAND, AI_SCORE_DEFAULT, target:playerRight);
                    SCORE_LT_VAL(opponentLeft, MOVE_HELPING_HAND, AI_SCORE_DEFAULT, target:opponentLeft);
                 }
    } SCENE {
        NOT MESSAGE("The opposing Wobbuffet used Helping Hand!");
    }
}

AI_DOUBLE_BATTLE_TEST("AI will not use a status move if partner already chose Helping Hand")
{
    s32 j;
    u32 statusMove = MOVE_NONE;

    for (j = MOVE_NONE + 1; j < MOVES_COUNT; j++)
    {
        if (GetMoveCategory(j) == DAMAGE_CATEGORY_STATUS) {
            PARAMETRIZE { statusMove = j; }
        }
    }

    GIVEN {
        AI_FLAGS(AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_CHECK_VIABILITY | AI_FLAG_TRY_TO_FAINT);
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET) { Moves(MOVE_HELPING_HAND); }
        OPPONENT(SPECIES_WOBBUFFET) { Moves(MOVE_TACKLE, statusMove); }
    } WHEN {
            TURN {  NOT_EXPECT_MOVE(opponentRight, statusMove);
                    SCORE_LT_VAL(opponentRight, statusMove, AI_SCORE_DEFAULT, target:playerLeft);
                    SCORE_LT_VAL(opponentRight, statusMove, AI_SCORE_DEFAULT, target:playerRight);
                    SCORE_LT_VAL(opponentRight, statusMove, AI_SCORE_DEFAULT, target:opponentLeft);
                 }
    } SCENE {
        MESSAGE("The opposing Wobbuffet used Helping Hand!");
    }
}

AI_DOUBLE_BATTLE_TEST("AI without any flags chooses moves at random - doubles")
{
    GIVEN {
        AI_FLAGS(0);
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_NIDOQUEEN) { Moves(MOVE_SPLASH, MOVE_EXPLOSION, MOVE_RAGE, MOVE_HELPING_HAND); }
        OPPONENT(SPECIES_NIDOQUEEN) { Moves(MOVE_SPLASH, MOVE_EXPLOSION, MOVE_RAGE, MOVE_HELPING_HAND); }
    } WHEN {
            TURN { EXPECT_MOVES(opponentLeft, MOVE_SPLASH, MOVE_EXPLOSION, MOVE_RAGE, MOVE_HELPING_HAND);
                   EXPECT_MOVES(opponentRight, MOVE_SPLASH, MOVE_EXPLOSION, MOVE_RAGE, MOVE_HELPING_HAND);
                   SCORE_EQ_VAL(opponentLeft, MOVE_SPLASH, AI_SCORE_DEFAULT, target:playerLeft);
                   SCORE_EQ_VAL(opponentLeft, MOVE_EXPLOSION, AI_SCORE_DEFAULT, target:playerLeft);
                   SCORE_EQ_VAL(opponentLeft, MOVE_RAGE, AI_SCORE_DEFAULT, target:playerLeft);
                   SCORE_EQ_VAL(opponentLeft, MOVE_HELPING_HAND, AI_SCORE_DEFAULT, target:playerLeft);
                   SCORE_EQ_VAL(opponentRight, MOVE_SPLASH, AI_SCORE_DEFAULT, target:playerLeft);
                   SCORE_EQ_VAL(opponentRight, MOVE_EXPLOSION, AI_SCORE_DEFAULT, target:playerLeft);
                   SCORE_EQ_VAL(opponentRight, MOVE_RAGE, AI_SCORE_DEFAULT, target:playerLeft);
                   SCORE_EQ_VAL(opponentRight, MOVE_HELPING_HAND, AI_SCORE_DEFAULT, target:playerLeft);
                }
    }
}

AI_DOUBLE_BATTLE_TEST("AI will not choose Earthquake if it damages the partner without a positive effect")
{
    u32 species;

    PARAMETRIZE { species = SPECIES_CHARIZARD; }
    PARAMETRIZE { species = SPECIES_CHARMANDER; }
    PARAMETRIZE { species = SPECIES_CHIKORITA; }

    GIVEN {
        ASSUME(GetMoveTarget(MOVE_EARTHQUAKE) == MOVE_TARGET_FOES_AND_ALLY);
        AI_FLAGS(AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_CHECK_VIABILITY | AI_FLAG_TRY_TO_FAINT);
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_PHANPY) { Moves(MOVE_EARTHQUAKE, MOVE_TACKLE); }
        OPPONENT(species) { Moves(MOVE_CELEBRATE); }
    } WHEN {
        if (species == SPECIES_CHARIZARD)
            TURN { EXPECT_MOVE(opponentLeft, MOVE_EARTHQUAKE); }
        else
            TURN { EXPECT_MOVE(opponentLeft, MOVE_TACKLE, target: playerLeft); }
    }
}

AI_DOUBLE_BATTLE_TEST("AI will choose Bulldoze if it triggers its ally's Defiant but will not KO the ally")
{
    TO_DO;

    u32 species, ability, level;    

    PARAMETRIZE { species = SPECIES_KINGAMBIT; ability = ABILITY_DEFIANT;  level = 100; }
    PARAMETRIZE { species = SPECIES_PAWNIARD;  ability = ABILITY_PRESSURE; level = 1; }
    PARAMETRIZE { species = SPECIES_PAWNIARD;  ability = ABILITY_DEFIANT;  level = 1; }

    GIVEN {
        ASSUME(GetMoveTarget(MOVE_EARTHQUAKE) == MOVE_TARGET_FOES_AND_ALLY);
        AI_FLAGS(AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_CHECK_VIABILITY | AI_FLAG_TRY_TO_FAINT);
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_PHANPY) { Moves(MOVE_BULLDOZE, MOVE_TACKLE); }
        OPPONENT(species) { Moves(MOVE_CELEBRATE); Level(level); Ability(ability); }
    } WHEN {
        if ((species == SPECIES_KINGAMBIT) && (ability == ABILITY_DEFIANT))
            TURN { EXPECT_MOVE(opponentLeft, MOVE_BULLDOZE); }
        else
            TURN { EXPECT_MOVE(opponentLeft, MOVE_TACKLE, target: playerLeft); }
    }
}

AI_DOUBLE_BATTLE_TEST("AI will choose Beat Up on an ally with Justified if it will not KO.")
{
    TO_DO;

    u32 ability, level;

    PARAMETRIZE { ability = ABILITY_FLASH_FIRE; level = 100; }
    PARAMETRIZE { ability = ABILITY_JUSTIFIED; level = 100; }
    PARAMETRIZE { ability = ABILITY_JUSTIFIED; level = 1; }

    GIVEN {
        AI_FLAGS(AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_CHECK_VIABILITY | AI_FLAG_TRY_TO_FAINT);
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_CLEFABLE);
        OPPONENT(SPECIES_SNEASEL) { Moves(MOVE_BEAT_UP); }
        OPPONENT(SPECIES_GROWLITHE) { Moves(MOVE_CELEBRATE); Level(level); Ability(ability); }
    } WHEN {
        if ((level == 100) && (ability == ABILITY_JUSTIFIED))
            TURN { EXPECT_MOVE(opponentLeft, MOVE_BEAT_UP, target: opponentRight); }
        else
            TURN { EXPECT_MOVE(opponentLeft, MOVE_BEAT_UP, target: playerLeft); }
    }
}

AI_DOUBLE_BATTLE_TEST("AI will choose Earthquake if partner is not alive")
{
    GIVEN {
        ASSUME(GetMoveTarget(MOVE_EARTHQUAKE) == MOVE_TARGET_FOES_AND_ALLY);
        AI_FLAGS(AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_CHECK_VIABILITY | AI_FLAG_TRY_TO_FAINT);
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET) { Moves(MOVE_EARTHQUAKE, MOVE_TACKLE); }
        OPPONENT(SPECIES_PIKACHU) { HP(1); Moves(MOVE_CELEBRATE); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_TACKLE, target: opponentRight); }
        TURN { EXPECT_MOVE(opponentLeft, MOVE_EARTHQUAKE); }
    }
}

AI_DOUBLE_BATTLE_TEST("AI will choose Earthquake if it kill an opposing mon and does 1/3 of damage to AI")
{
    GIVEN {
        ASSUME(GetMoveTarget(MOVE_EARTHQUAKE) == MOVE_TARGET_FOES_AND_ALLY);
        AI_FLAGS(AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_CHECK_VIABILITY | AI_FLAG_TRY_TO_FAINT);
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WOBBUFFET) { HP(1); }
        OPPONENT(SPECIES_WOBBUFFET) { Moves(MOVE_EARTHQUAKE, MOVE_TACKLE); }
        OPPONENT(SPECIES_PARAS) { Moves(MOVE_CELEBRATE); }
    } WHEN {
        TURN { EXPECT_MOVE(opponentLeft, MOVE_EARTHQUAKE); }
    }
}

AI_DOUBLE_BATTLE_TEST("AI will the see a corresponding absorbing ability on partner to one of its moves")
{
    u32 ability;
    PARAMETRIZE { ability = ABILITY_LIGHTNING_ROD; }
    PARAMETRIZE { ability = ABILITY_STATIC; }

    GIVEN {
        ASSUME(GetMoveTarget(MOVE_DISCHARGE) == MOVE_TARGET_FOES_AND_ALLY);
        AI_FLAGS(AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_CHECK_VIABILITY | AI_FLAG_TRY_TO_FAINT);
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET) { Moves(MOVE_DISCHARGE, MOVE_TACKLE); }
        OPPONENT(SPECIES_PIKACHU) { HP(1); Ability(ability); Moves(MOVE_CELEBRATE); }
    } WHEN {
        if (ability == ABILITY_LIGHTNING_ROD)
            TURN { EXPECT_MOVE(opponentLeft, MOVE_DISCHARGE); }
        else
            TURN { EXPECT_MOVE(opponentLeft, MOVE_TACKLE); }
    }
}

AI_DOUBLE_BATTLE_TEST("AI recognizes Volt Absorb received from Trace")
{
    KNOWN_FAILING; // MGriffin's PR that switched two turn charging moves in AI tests broke this test, waiting on a fix
    GIVEN {
        AI_FLAGS(AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_CHECK_VIABILITY | AI_FLAG_TRY_TO_FAINT);
        PLAYER(SPECIES_MAGNETON);
        PLAYER(SPECIES_GARDEVOIR) { Ability(ABILITY_TRACE); }
        OPPONENT(SPECIES_JOLTEON) { Ability(ABILITY_VOLT_ABSORB); Moves(MOVE_THUNDER_WAVE, MOVE_THUNDERSHOCK, MOVE_WATER_GUN); }
        OPPONENT(SPECIES_JOLTEON) { Ability(ABILITY_VOLT_ABSORB); Moves(MOVE_THUNDER_WAVE, MOVE_THUNDERSHOCK, MOVE_WATER_GUN); }
    } WHEN {
        TURN { NOT_EXPECT_MOVE(opponentLeft, MOVE_THUNDERSHOCK); NOT_EXPECT_MOVE(opponentLeft, MOVE_THUNDER_WAVE); NOT_EXPECT_MOVE(opponentRight, MOVE_THUNDER_WAVE); }
    } THEN {
        EXPECT(gBattleResources->aiData->abilities[B_POSITION_PLAYER_RIGHT] == ABILITY_VOLT_ABSORB);
    }
}

AI_DOUBLE_BATTLE_TEST("AI activates its ally's Weakness Policy with spread moves if it will not KO")
{
    TO_DO;
    
    u32 holdItem, currentHP;

    PARAMETRIZE { holdItem = ITEM_NONE; currentHP = 500; }
    PARAMETRIZE { holdItem = ITEM_WEAKNESS_POLICY; currentHP = 500; }
    PARAMETRIZE { holdItem = ITEM_WEAKNESS_POLICY; currentHP = 1; }

    GIVEN {
        ASSUME(GetMoveTarget(MOVE_EARTHQUAKE) == MOVE_TARGET_FOES_AND_ALLY);
        ASSUME(GetMoveTarget(MOVE_STOMPING_TANTRUM) == MOVE_TARGET_SELECTED);
        AI_FLAGS(AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_CHECK_VIABILITY | AI_FLAG_TRY_TO_FAINT);
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_PHANPY) { Moves(MOVE_EARTHQUAKE, MOVE_STOMPING_TANTRUM); }
        OPPONENT(SPECIES_INCINEROAR) { Moves(MOVE_CELEBRATE); Item(holdItem); HP(currentHP); }
    } WHEN {
        if ((holdItem = ITEM_WEAKNESS_POLICY) && (currentHP == 500))
            TURN { EXPECT_MOVE(opponentLeft, MOVE_EARTHQUAKE); }
        else
            TURN { EXPECT_MOVE(opponentLeft, MOVE_STOMPING_TANTRUM, target: playerLeft); }
    }
}

AI_DOUBLE_BATTLE_TEST("AI will not use Trick Room if its ally is fast")
{
    TO_DO;

    u32 species;

    PARAMETRIZE { species = SPECIES_SLOWBRO; }
    PARAMETRIZE { species = SPECIES_REGIELEKI; }
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_TRICK_ROOM) == EFFECT_TRICK_ROOM);
        AI_FLAGS(AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_CHECK_VIABILITY | AI_FLAG_TRY_TO_FAINT);
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET) { Moves(MOVE_TRICK_ROOM, MOVE_TACKLE); }
        OPPONENT(species) { Moves(MOVE_CELEBRATE); }
    } WHEN {
        if (species == SPECIES_REGIELEKI)
            TURN { EXPECT_MOVE(opponentLeft, MOVE_TACKLE); }
        else
            TURN { EXPECT_MOVE(opponentLeft, MOVE_TRICK_ROOM); }
    }
}

AI_DOUBLE_BATTLE_TEST("AI knows that giving its ally immunity would let it use a spread move")
{
    TO_DO;
    ASSUME(GetMoveTarget(MOVE_SURF) == MOVE_TARGET_FOES_AND_ALLY);

    u32 ability, move;

    PARAMETRIZE { ability = ABILITY_LEVITATE;       move = MOVE_EARTHQUAKE; }
    PARAMETRIZE { ability = ABILITY_EARTH_EATER;    move = MOVE_EARTHQUAKE; }
    PARAMETRIZE { ability = ABILITY_VOLT_ABSORB;    move = MOVE_DISCHARGE; }
    PARAMETRIZE { ability = ABILITY_MOTOR_DRIVE;    move = MOVE_DISCHARGE; }
    PARAMETRIZE { ability = ABILITY_LIGHTNING_ROD;  move = MOVE_DISCHARGE; }
    PARAMETRIZE { ability = ABILITY_FLASH_FIRE;     move = MOVE_LAVA_PLUME; }
    PARAMETRIZE { ability = ABILITY_WATER_ABSORB;   move = MOVE_SURF; }
    GIVEN {
        AI_FLAGS(AI_FLAG_CHECK_BAD_MOVE | AI_FLAG_CHECK_VIABILITY | AI_FLAG_TRY_TO_FAINT);
        PLAYER(SPECIES_WOBBUFFET);
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_CLAYDOL) { Ability(ability); Moves(MOVE_SKILL_SWAP, move); }
        OPPONENT(SPECIES_WOBBUFFET) { Moves(MOVE_CELEBRATE);  }
    } WHEN {
        TURN { EXPECT_MOVE(opponentLeft, MOVE_SKILL_SWAP, target: opponentRight); }
        TURN { EXPECT_MOVE(opponentLeft, move); }
    }
}

