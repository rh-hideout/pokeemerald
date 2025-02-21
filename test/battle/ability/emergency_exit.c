#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Emergency Exit switches out when taking 50% max-hp damage")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_GOLISOPOD) { Ability(ABILITY_EMERGENCY_EXIT); MaxHP(263); HP(262); };
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_SUPER_FANG); SEND_OUT(opponent, 1); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SUPER_FANG, player);
        HP_BAR(opponent);
        ABILITY_POPUP(opponent, ABILITY_EMERGENCY_EXIT);
    }
}

SINGLE_BATTLE_TEST("Emergency Exit does not switch out when going below 50% max-HP but healed via held item back above the threshold")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET)
        OPPONENT(SPECIES_GOLISOPOD) { Ability(ABILITY_EMERGENCY_EXIT); MaxHP(263); HP(262); Item(ITEM_SITRUS_BERRY); };
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_SUPER_FANG); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SUPER_FANG, player);
        HP_BAR(opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponent);
        NOT ABILITY_POPUP(opponent, ABILITY_EMERGENCY_EXIT);
    }
}

SINGLE_BATTLE_TEST("Emergency Exit switches out when going below 50% max-HP but healing via held item is not enough to go back above the threshold")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET)
        OPPONENT(SPECIES_GOLISOPOD) { Ability(ABILITY_EMERGENCY_EXIT); MaxHP(263); HP(133); Item(ITEM_ORAN_BERRY); };
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_SUPER_FANG); SEND_OUT(opponent, 1); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SUPER_FANG, player);
        HP_BAR(opponent);
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_HELD_ITEM_EFFECT, opponent);
        ABILITY_POPUP(opponent, ABILITY_EMERGENCY_EXIT);
    }
}

SINGLE_BATTLE_TEST("Emergency Exit activates when taking residual damage and falling under 50% max-hp - Burn")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_GOLISOPOD) { Ability(ABILITY_EMERGENCY_EXIT); MaxHP(263); HP(134); Status1(STATUS1_BURN); };
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { SEND_OUT(opponent, 1); }
    } SCENE {
        HP_BAR(opponent);
        ABILITY_POPUP(opponent, ABILITY_EMERGENCY_EXIT);
    }
}

SINGLE_BATTLE_TEST("Emergency Exit activates when taking residual damage and falling under 50% max-hp - Weather")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_GOLISOPOD) { Ability(ABILITY_EMERGENCY_EXIT); MaxHP(263); HP(134); };
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_SANDSTORM); SEND_OUT(opponent, 1); }
    } SCENE {
        HP_BAR(opponent);
        ABILITY_POPUP(opponent, ABILITY_EMERGENCY_EXIT);
    }
}

SINGLE_BATTLE_TEST("Emergency Exit activates when taking residual damage and falling under 50% max-hp - Salt Cure")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_GOLISOPOD) { Ability(ABILITY_EMERGENCY_EXIT); MaxHP(263); HP(160); };
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_SALT_CURE); SEND_OUT(opponent, 1); }
    } SCENE {
        HP_BAR(opponent);
        ABILITY_POPUP(opponent, ABILITY_EMERGENCY_EXIT);
    }
}
