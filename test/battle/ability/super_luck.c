#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Super Luck increases the critical hit ratio by 1 stage")
{
    u32 genConfig = 0;
    for (u32 j = GEN_2; j < GEN_LATEST + 1; j++)
        PARAMETRIZE { genConfig = j; }
    PASSES_RANDOMLY(1, 8, RNG_CRITICAL_HIT);
    GIVEN {
        WITH_CONFIG(GEN_CONFIG_CRIT_CHANCE, genConfig);
        PLAYER(SPECIES_TOGEPI) { Ability(ABILITY_SUPER_LUCK); };
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_TACKLE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TACKLE, player);
        MESSAGE("A critical hit!");
    }
}

TO_DO_BATTLE_TEST("Super Luck increases the chances of wild Pokémon holding items (Gen8+)");
