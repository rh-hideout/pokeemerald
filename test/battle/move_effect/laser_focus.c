#include "global.h"
#include "test/battle.h"

ASSUMPTIONS
{
    ASSUME(gMovesInfo[MOVE_LASER_FOCUS].effect == EFFECT_LASER_FOCUS);
}

SINGLE_BATTLE_TEST("Laser Focus causes the user's move used on the next turn to result in a Critical Hit")
{
    PASSES_RANDOMLY(1, 1, RNG_CRITICAL_HIT);
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_LASER_FOCUS); }
        TURN { MOVE(player, MOVE_TACKLE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_LASER_FOCUS, player);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TACKLE, player);
        MESSAGE("A critical hit!");
    }
}
