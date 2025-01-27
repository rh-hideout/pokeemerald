#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Leek increases critical hit ratio by 2 stages for the Farfetch'd Family")
{
    u32 species,genConfig, chance;
    PARAMETRIZE { genConfig = GEN_2; chance = 4; species = SPECIES_FARFETCHD; }
    PARAMETRIZE { genConfig = GEN_6; chance = 2; species = SPECIES_FARFETCHD; }
    PARAMETRIZE { genConfig = GEN_2; chance = 4; species = SPECIES_FARFETCHD_GALAR; }
    PARAMETRIZE { genConfig = GEN_6; chance = 2; species = SPECIES_FARFETCHD_GALAR; }
    PARAMETRIZE { genConfig = GEN_2; chance = 4; species = SPECIES_SIRFETCHD; }
    PARAMETRIZE { genConfig = GEN_6; chance = 2; species = SPECIES_SIRFETCHD; }
    PASSES_RANDOMLY(1, chance, RNG_CRITICAL_HIT);
    GIVEN {
        WITH_CONFIG(GEN_CONFIG_CRIT_CHANCE, genConfig);
        ASSUME(gItemsInfo[ITEM_LEEK].holdEffect == HOLD_EFFECT_LEEK);
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(species) { Item(ITEM_LEEK); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_TACKLE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TACKLE, opponent);
        MESSAGE("A critical hit!");
    }
}
