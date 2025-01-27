#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Lucky Punch increases critical hit ratio by 2 stages for Chansey")
{
    u32 genConfig, chance;
    PARAMETRIZE { genConfig = GEN_2; chance = 4; }
    PARAMETRIZE { genConfig = GEN_6; chance = 2; }
    PASSES_RANDOMLY(1, chance, RNG_CRITICAL_HIT);
    GIVEN {
        WITH_CONFIG(GEN_CONFIG_CRIT_CHANCE, genConfig);
        ASSUME(gItemsInfo[ITEM_LUCKY_PUNCH].holdEffect == HOLD_EFFECT_LUCKY_PUNCH);
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_CHANSEY) { Item(ITEM_LUCKY_PUNCH); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_TACKLE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TACKLE, opponent);
        MESSAGE("A critical hit!");
    }
}
