#include "global.h"
#include "test/battle.h"

ASSUMPTIONS
{
    ASSUME(gItemsInfo[ITEM_LANSAT_BERRY].holdEffect == HOLD_EFFECT_CRITICAL_UP);
    ASSUME(gMovesInfo[MOVE_DRAGON_RAGE].effect == EFFECT_FIXED_DAMAGE_ARG);
    ASSUME(gMovesInfo[MOVE_DRAGON_RAGE].argument == 40);
}

SINGLE_BATTLE_TEST("Lansat Berry raises the holder's critical-hit-ratio by two stages when HP drops to 1/4 or below")
{
    u32 move;

    PARAMETRIZE { move = MOVE_TACKLE; }
    PARAMETRIZE { move = MOVE_DRAGON_RAGE; }

    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(160); HP(80); Item(ITEM_LANSAT_BERRY); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, move); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, move, opponent);
        if (move == MOVE_TACKLE) {
            NONE_OF {
                ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, player);
                MESSAGE("Wobbuffet used the Lansat Berry to get pumped!");
            }
        } else {
            ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, player);
            MESSAGE("Wobbuffet used the Lansat Berry to get pumped!");
        }
    }
}

SINGLE_BATTLE_TEST("Lansat Berry raises the holder's critical-hit-ratio by two stages when HP drops to 1/2 or below")
{
    GIVEN {
        PLAYER(SPECIES_BELLSPROUT) { MaxHP(80); HP(80); Ability(ABILITY_GLUTTONY); Item(ITEM_LANSAT_BERRY); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, MOVE_DRAGON_RAGE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_DRAGON_RAGE, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, player);
        MESSAGE("Bellsprout used the Lansat Berry to get pumped!");
    }
}

SINGLE_BATTLE_TEST("Lansat Berry raises the holder's critical-hit-ratio by 2 stages")
{
    u32 genConfig, chance;
    PARAMETRIZE { genConfig = GEN_2; chance = 4; }
    PARAMETRIZE { genConfig = GEN_6; chance = 2; }
    PASSES_RANDOMLY(1, chance, RNG_CRITICAL_HIT);
    GIVEN {
        WITH_CONFIG(GEN_CONFIG_CRIT_CHANCE, genConfig);
        ASSUME(gMovesInfo[MOVE_TACKLE].criticalHitStage == 0);
        PLAYER(SPECIES_WOBBUFFET) { MaxHP(160); HP(80); Item(ITEM_LANSAT_BERRY); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, MOVE_DRAGON_RAGE); MOVE(player, MOVE_TACKLE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_DRAGON_RAGE, opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, player);
        MESSAGE("Wobbuffet used the Lansat Berry to get pumped!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TACKLE, player);
        MESSAGE("A critical hit!");
    }
}
