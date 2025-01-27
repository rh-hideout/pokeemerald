#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Dire Hit increases a battler's critical hit chance by 2 stages")
{
    u32 genConfig, chance;
    PARAMETRIZE { genConfig = GEN_2; chance = 4; }
    PARAMETRIZE { genConfig = GEN_6; chance = 2; }
    PASSES_RANDOMLY(1, chance, RNG_CRITICAL_HIT);
    GIVEN {
        WITH_CONFIG(GEN_CONFIG_CRIT_CHANCE, genConfig);
        ASSUME(gItemsInfo[ITEM_DIRE_HIT].battleUsage == EFFECT_ITEM_SET_FOCUS_ENERGY);
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { USE_ITEM(player, ITEM_DIRE_HIT, partyIndex: 0); }
        TURN { MOVE(player, MOVE_SCRATCH); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_FOCUS_ENERGY, player);
        MESSAGE("Wobbuffet used the Dire Hit to get pumped!");
        MESSAGE("Wobbuffet used Scratch!");
        MESSAGE("A critical hit!");
    }
}
