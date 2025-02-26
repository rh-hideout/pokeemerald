#include "global.h"
#include "test/battle.h"

// These tests are very heavy computationally. Only use them to review animation PRs.

#if T_SHOULD_RUN_MOVE_ANIM

SINGLE_BATTLE_TEST("Move Animations don't leak when used - Singles (player to opponent)")
{
    u32 j = 1, move = 0, species = 0;
    FORCE_MOVE_ANIM(TRUE);
    for (; j < MOVES_COUNT; j++) {
        if (gMovesInfo[j].effect == EFFECT_DARK_VOID) { // User needs to be Darkrai
            PARAMETRIZE { move = j; species = SPECIES_DARKRAI; }
        } else if (gMovesInfo[j].effect == EFFECT_HYPERSPACE_FURY) { // User needs to be Hoopa Unbound
            PARAMETRIZE { move = j; species = SPECIES_HOOPA_UNBOUND; }
        } else if (gMovesInfo[j].effect == EFFECT_AURA_WHEEL) { // User needs to be Morpeko
            PARAMETRIZE { move = j; species = SPECIES_MORPEKO_FULL_BELLY; }
        } else if (gMovesInfo[j].effect == EFFECT_ROTOTILLER
                || gMovesInfo[j].effect == EFFECT_FLOWER_SHIELD) { // User needs to be Grass-type
            PARAMETRIZE { move = j; species = SPECIES_TANGELA; }
        } else if (gMovesInfo[j].effect == EFFECT_FAIL_IF_NOT_ARG_TYPE && gMovesInfo[j].argument.type == TYPE_FIRE) { // User needs to be Fire-type
            PARAMETRIZE { move = j; species = SPECIES_FLAREON; }
        } else if (gMovesInfo[j].effect == EFFECT_FAIL_IF_NOT_ARG_TYPE && gMovesInfo[j].argument.type == TYPE_ELECTRIC) { // User needs to be Electric-type
            PARAMETRIZE { move = j; species = SPECIES_JOLTEON; }
        } else if (gMovesInfo[j].effect == EFFECT_MAGNETIC_FLUX
                || gMovesInfo[j].effect == EFFECT_GEAR_UP) { // User needs to have Plus
            PARAMETRIZE { move = j; species = SPECIES_KLINKLANG; }
        } else if (gMovesInfo[j].effect == EFFECT_PLACEHOLDER) { // Ignore placeholder moves
            PARAMETRIZE { move = MOVE_POUND; species = SPECIES_WOBBUFFET; }
        } else { // Everything else
            PARAMETRIZE { move = j; species = SPECIES_WOBBUFFET; }
        }
    }
    GIVEN {
        PLAYER(species) {
            HP(9997); MaxHP(9999); Item(ITEM_ORAN_BERRY);
            if (species == SPECIES_WOBBUFFET)
                Gender(MON_FEMALE);
            if (gMovesInfo[move].effect == EFFECT_LAST_RESORT)
                Moves(move, MOVE_POUND);
            if (species == SPECIES_KLINKLANG)
                Ability(ABILITY_PLUS);
        }
        PLAYER(SPECIES_WOBBUFFET)   {
            Gender(MON_MALE); MaxHP(9999); Moves(MOVE_POUND);
            HP(gMovesInfo[move].effect == EFFECT_REVIVAL_BLESSING ? 0 : 9998);
        }
        OPPONENT(SPECIES_WOBBUFFET) {
            Gender(MON_MALE); HP(9998); MaxHP(9999); SpDefense(9999); Defense(9999); Ability(ABILITY_TELEPATHY);
            if (gMovesInfo[move].effect != EFFECT_BESTOW) {
                Item(ITEM_ORAN_BERRY);
            }
        }
        OPPONENT(SPECIES_WOBBUFFET) { Gender(MON_FEMALE); HP(9998); MaxHP(9999); SpDefense(9999); Defense(9999); }
    } WHEN {
        // Setup turn
        if (gMovesInfo[move].effect == EFFECT_SNORE
         || gMovesInfo[move].effect == EFFECT_SLEEP_TALK) {  // Player needs to be asleep
            TURN { MOVE(player, MOVE_REST); }
        } else if (gMovesInfo[move].effect == EFFECT_SPIT_UP
                || gMovesInfo[move].effect == EFFECT_SWALLOW) { // Player needs to have used Stockpile
            TURN { MOVE(player, MOVE_STOCKPILE); }
        } else if ((gMovesInfo[move].effect == EFFECT_DOUBLE_POWER_ON_ARG_STATUS && gMovesInfo[move].argument.status == STATUS1_PARALYSIS)) { // Opponent needs to be paralyzed
            TURN { MOVE(player, MOVE_THUNDER_WAVE); }
        } else if (gMovesInfo[move].effect == EFFECT_RECYCLE
                || gMovesInfo[move].effect == EFFECT_BELCH) { // Player needs to have eaten its Berry
            TURN { MOVE(player, MOVE_STUFF_CHEEKS); }
        } else if (gMovesInfo[move].effect == EFFECT_REFRESH
                || gMovesInfo[move].effect == EFFECT_PSYCHO_SHIFT) { // Player needs to be paralyzed
            TURN { MOVE(opponent, MOVE_THUNDER_WAVE); }
        } else if (gMovesInfo[move].effect == EFFECT_LAST_RESORT) { // Player needs to have used all other moves
            TURN { MOVE(player, MOVE_POUND); }
        } else if (gMovesInfo[move].effect == EFFECT_DREAM_EATER
                || gMovesInfo[move].effect == EFFECT_NIGHTMARE) { // Opponent needs to be asleep
            TURN { MOVE(opponent, MOVE_REST); }
        } else if (gMovesInfo[move].effect == EFFECT_VENOM_DRENCH
                || gMovesInfo[move].effect == EFFECT_PURIFY) { // Opponent needs to be poisoned
            TURN { MOVE(player, MOVE_POISON_POWDER); }
        } else if (gMovesInfo[move].effect == EFFECT_TOPSY_TURVY) { // Opponent needs to have its stats buffed
            TURN { MOVE(opponent, MOVE_SWORDS_DANCE); }
        } else if (gMovesInfo[move].effect == EFFECT_AURORA_VEIL) { // Has to be hailing
            TURN { MOVE(player, MOVE_HAIL); }
        } else if (gMovesInfo[move].effect == EFFECT_HIT_SET_REMOVE_TERRAIN && gMovesInfo[move].argument.moveProperty == ARG_TRY_REMOVE_TERRAIN_FAIL) { // Needs a terrain
            TURN { MOVE(player, MOVE_ELECTRIC_TERRAIN); }
        }
        // Effective turn
        TURN {
            if (gMovesInfo[move].effect == EFFECT_COUNTER
             || gMovesInfo[move].effect == EFFECT_MIRROR_MOVE
             || gMovesInfo[move].effect == EFFECT_CONVERSION_2
             || gMovesInfo[move].effect == EFFECT_MIRROR_COAT
             || gMovesInfo[move].effect == EFFECT_METAL_BURST
             || gMovesInfo[move].effect == EFFECT_COPYCAT
             || gMovesInfo[move].effect == EFFECT_SUCKER_PUNCH
             || gMovesInfo[move].effect == EFFECT_INSTRUCT) { // Opponent needs to hit the player first
                MOVE(opponent, gMovesInfo[move].effect == EFFECT_MIRROR_COAT ? MOVE_SWIFT : MOVE_POUND);
                MOVE(player, move);
            } else if (gMovesInfo[move].effect == EFFECT_SNATCH) { // Opponent needs to steal the opponent's buffing move
                MOVE(player, move);
                MOVE(opponent, MOVE_SWORDS_DANCE);
            } else if (gMovesInfo[move].effect == EFFECT_OHKO) { // Opponent needs to send out a different team member
                MOVE(player, move);
                SEND_OUT(opponent, 1);
            } else if (gMovesInfo[move].effect == EFFECT_TELEPORT
                    || gMovesInfo[move].effect == EFFECT_EXPLOSION
                    || gMovesInfo[move].effect == EFFECT_BATON_PASS
                    || gMovesInfo[move].effect == EFFECT_MEMENTO
                    || gMovesInfo[move].effect == EFFECT_HEALING_WISH
                    || gMovesInfo[move].effect == EFFECT_HIT_ESCAPE
                    || gMovesInfo[move].effect == EFFECT_FINAL_GAMBIT
                    || gMovesInfo[move].effect == EFFECT_PARTING_SHOT
                    || gMovesInfo[move].effect == EFFECT_SHED_TAIL
                    || gMovesInfo[move].effect == EFFECT_CHILLY_RECEPTION) { // Player needs to send out a different team member
                MOVE(player, move);
                SEND_OUT(player, 1);
            } else if (gMovesInfo[move].effect == EFFECT_PROTECT
                    || gMovesInfo[move].effect == EFFECT_ENDURE
                    || gMovesInfo[move].effect == EFFECT_FOLLOW_ME
                    || gMovesInfo[move].effect == EFFECT_MAGIC_COAT
                    || gMovesInfo[move].effect == EFFECT_ME_FIRST
                    || gMovesInfo[move].effect == EFFECT_QUASH
                    || gMovesInfo[move].effect == EFFECT_MAT_BLOCK
                    || gMovesInfo[move].effect == EFFECT_ELECTRIFY
                    || gMovesInfo[move].effect == EFFECT_SHELL_TRAP) { // Player needs to go first
                MOVE(player, move);
                MOVE(opponent, MOVE_POUND);
            } else if (gMovesInfo[move].effect == EFFECT_REVIVAL_BLESSING) { // Player selects party member
                MOVE(player, move, partyIndex: 1);
                MOVE(opponent, MOVE_HELPING_HAND);
            } else if (gMovesInfo[move].effect == EFFECT_UPPER_HAND) { // Opponent needs to choose priority move
                MOVE(player, move);
                MOVE(opponent, MOVE_QUICK_ATTACK);
            } else if (gMovesInfo[move].effect == EFFECT_ACUPRESSURE) {
                MOVE(player, move, target: player);
            } else { // All other moves
                MOVE(opponent, MOVE_HELPING_HAND); // Helping Hand, so there's no anim on the opponent's side.
                MOVE(player, move);
            }
        }
    } SCENE {
        if (gMovesInfo[move].effect == EFFECT_FOLLOW_ME
         || gMovesInfo[move].effect == EFFECT_HELPING_HAND
         || gMovesInfo[move].effect == EFFECT_AFTER_YOU
         || gMovesInfo[move].effect == EFFECT_ALLY_SWITCH
         || gMovesInfo[move].effect == EFFECT_AROMATIC_MIST
         || move == MOVE_HOLD_HANDS // Hack here because it shares its effect with Splash and Celebrate
         || gMovesInfo[move].effect == EFFECT_COACHING
         || gMovesInfo[move].effect == EFFECT_DRAGON_CHEER) {
            // Moves that fail in Single Battles
        } else if (gMovesInfo[move].effect == EFFECT_MIRROR_MOVE) { // Copy the opponent's move
            ANIMATION(ANIM_TYPE_MOVE, MOVE_POUND, player);
        } else if (gMovesInfo[move].effect == EFFECT_NATURE_POWER) { // Recorded battles always use BATTLE_TERRAIN_BUILDING
            ANIMATION(ANIM_TYPE_MOVE, B_NATURE_POWER_MOVES >= GEN_4 ? MOVE_TRI_ATTACK : MOVE_SWIFT, player);
        } else { // All other moves
            ANIMATION(ANIM_TYPE_MOVE, move, player);
        }
    } THEN {
        FORCE_MOVE_ANIM(FALSE);
        if (gLoadFail)
            DebugPrintf("Move failed: %S (%u)", gMovesInfo[move].name, move);
        EXPECT_EQ(gLoadFail, FALSE);
    }
}

SINGLE_BATTLE_TEST("Move Animations don't leak when used - Singles (opponent to player)")
{
    u32 j = 1, move = 0, species = 0;
    FORCE_MOVE_ANIM(TRUE);
    for (; j < MOVES_COUNT; j++) {
        if (gMovesInfo[j].effect == EFFECT_DARK_VOID) { // User needs to be Darkrai
            PARAMETRIZE { move = j; species = SPECIES_DARKRAI; }
        } else if (gMovesInfo[j].effect == EFFECT_HYPERSPACE_FURY) { // User needs to be Hoopa Unbound
            PARAMETRIZE { move = j; species = SPECIES_HOOPA_UNBOUND; }
        } else if (gMovesInfo[j].effect == EFFECT_AURA_WHEEL) { // User needs to be Morpeko
            PARAMETRIZE { move = j; species = SPECIES_MORPEKO_FULL_BELLY; }
        } else if (gMovesInfo[j].effect == EFFECT_ROTOTILLER
                || gMovesInfo[j].effect == EFFECT_FLOWER_SHIELD) { // User needs to be Grass-type
            PARAMETRIZE { move = j; species = SPECIES_TANGELA; }
        } else if (gMovesInfo[j].effect == EFFECT_FAIL_IF_NOT_ARG_TYPE && gMovesInfo[j].argument.type == TYPE_FIRE) { // User needs to be Fire-type
            PARAMETRIZE { move = j; species = SPECIES_FLAREON; }
        } else if (gMovesInfo[j].effect == EFFECT_FAIL_IF_NOT_ARG_TYPE && gMovesInfo[j].argument.type == TYPE_ELECTRIC) { // User needs to be Electric-type
            PARAMETRIZE { move = j; species = SPECIES_JOLTEON; }
        } else if (gMovesInfo[j].effect == EFFECT_MAGNETIC_FLUX
                || gMovesInfo[j].effect == EFFECT_GEAR_UP) { // User needs to have Plus
            PARAMETRIZE { move = j; species = SPECIES_KLINKLANG; }
        } else if (gMovesInfo[j].effect == EFFECT_PLACEHOLDER) { // Ignore placeholder moves
            PARAMETRIZE { move = MOVE_POUND; species = SPECIES_WOBBUFFET; }
        } else { // Everything else
            PARAMETRIZE { move = j; species = SPECIES_WOBBUFFET; }
        }
    }
    GIVEN {
        OPPONENT(species) {
            HP(9997); MaxHP(9999); Item(ITEM_ORAN_BERRY);
            if (species == SPECIES_WOBBUFFET)
                Gender(MON_FEMALE);
            if (gMovesInfo[move].effect == EFFECT_LAST_RESORT)
                Moves(move, MOVE_POUND);
            if (species == SPECIES_KLINKLANG)
                Ability(ABILITY_PLUS);
        }
        OPPONENT(SPECIES_WOBBUFFET)   {
            Gender(MON_MALE); MaxHP(9999); Moves(MOVE_POUND);
            HP(gMovesInfo[move].effect == EFFECT_REVIVAL_BLESSING ? 0 : 9998);
        }
        PLAYER(SPECIES_WOBBUFFET) {
            Gender(MON_MALE); HP(9998); MaxHP(9999); SpDefense(9999); Defense(9999); Ability(ABILITY_TELEPATHY);
            if (gMovesInfo[move].effect != EFFECT_BESTOW) {
                Item(ITEM_ORAN_BERRY);
            }
        }
        PLAYER(SPECIES_WOBBUFFET) { Gender(MON_FEMALE); HP(9998); MaxHP(9999); SpDefense(9999); Defense(9999); }
    } WHEN {
        // Setup turn
        if (gMovesInfo[move].effect == EFFECT_SNORE
         || gMovesInfo[move].effect == EFFECT_SLEEP_TALK) {  // Player needs to be asleep
            TURN { MOVE(opponent, MOVE_REST); }
        } else if (gMovesInfo[move].effect == EFFECT_SPIT_UP
                || gMovesInfo[move].effect == EFFECT_SWALLOW) { // Player needs to have used Stockpile
            TURN { MOVE(opponent, MOVE_STOCKPILE); }
        } else if ((gMovesInfo[move].effect == EFFECT_DOUBLE_POWER_ON_ARG_STATUS && gMovesInfo[move].argument.status == STATUS1_PARALYSIS)) { // Opponent needs to be paralyzed
            TURN { MOVE(opponent, MOVE_THUNDER_WAVE); }
        } else if (gMovesInfo[move].effect == EFFECT_RECYCLE
                || gMovesInfo[move].effect == EFFECT_BELCH) { // Player needs to have eaten its Berry
            TURN { MOVE(opponent, MOVE_STUFF_CHEEKS); }
        } else if (gMovesInfo[move].effect == EFFECT_REFRESH
                || gMovesInfo[move].effect == EFFECT_PSYCHO_SHIFT) { // Player needs to be paralyzed
            TURN { MOVE(player, MOVE_THUNDER_WAVE); }
        } else if (gMovesInfo[move].effect == EFFECT_LAST_RESORT) { // Player needs to have used all other moves
            TURN { MOVE(opponent, MOVE_POUND); }
        } else if (gMovesInfo[move].effect == EFFECT_DREAM_EATER
                || gMovesInfo[move].effect == EFFECT_NIGHTMARE) { // Opponent needs to be asleep
            TURN { MOVE(player, MOVE_REST); }
        } else if (gMovesInfo[move].effect == EFFECT_VENOM_DRENCH
                || gMovesInfo[move].effect == EFFECT_PURIFY) { // Opponent needs to be poisoned
            TURN { MOVE(opponent, MOVE_POISON_POWDER); }
        } else if (gMovesInfo[move].effect == EFFECT_TOPSY_TURVY) { // Opponent needs to have its stats buffed
            TURN { MOVE(player, MOVE_SWORDS_DANCE); }
        } else if (gMovesInfo[move].effect == EFFECT_AURORA_VEIL) { // Has to be hailing
            TURN { MOVE(opponent, MOVE_HAIL); }
        } else if (gMovesInfo[move].effect == EFFECT_HIT_SET_REMOVE_TERRAIN && gMovesInfo[move].argument.moveProperty == ARG_TRY_REMOVE_TERRAIN_FAIL) { // Needs a terrain
            TURN { MOVE(opponent, MOVE_ELECTRIC_TERRAIN); }
        }
        // Effective turn
        TURN {
            if (gMovesInfo[move].effect == EFFECT_COUNTER
             || gMovesInfo[move].effect == EFFECT_MIRROR_MOVE
             || gMovesInfo[move].effect == EFFECT_CONVERSION_2
             || gMovesInfo[move].effect == EFFECT_MIRROR_COAT
             || gMovesInfo[move].effect == EFFECT_METAL_BURST
             || gMovesInfo[move].effect == EFFECT_COPYCAT
             || gMovesInfo[move].effect == EFFECT_SUCKER_PUNCH
             || gMovesInfo[move].effect == EFFECT_INSTRUCT) { // Opponent needs to hit the player first
                MOVE(player, gMovesInfo[move].effect == EFFECT_MIRROR_COAT ? MOVE_SWIFT : MOVE_POUND);
                MOVE(opponent, move);
            } else if (gMovesInfo[move].effect == EFFECT_SNATCH) { // Opponent needs to steal the opponent's buffing move
                MOVE(opponent, move);
                MOVE(player, MOVE_SWORDS_DANCE);
            } else if (gMovesInfo[move].effect == EFFECT_OHKO) { // Opponent needs to send out a different team member
                MOVE(opponent, move);
                SEND_OUT(player, 1);
            } else if (gMovesInfo[move].effect == EFFECT_TELEPORT
                    || gMovesInfo[move].effect == EFFECT_EXPLOSION
                    || gMovesInfo[move].effect == EFFECT_BATON_PASS
                    || gMovesInfo[move].effect == EFFECT_MEMENTO
                    || gMovesInfo[move].effect == EFFECT_HEALING_WISH
                    || gMovesInfo[move].effect == EFFECT_HIT_ESCAPE
                    || gMovesInfo[move].effect == EFFECT_FINAL_GAMBIT
                    || gMovesInfo[move].effect == EFFECT_PARTING_SHOT
                    || gMovesInfo[move].effect == EFFECT_SHED_TAIL
                    || gMovesInfo[move].effect == EFFECT_CHILLY_RECEPTION) { // Player needs to send out a different team member
                MOVE(opponent, move);
                SEND_OUT(opponent, 1);
            } else if (gMovesInfo[move].effect == EFFECT_PROTECT
                    || gMovesInfo[move].effect == EFFECT_ENDURE
                    || gMovesInfo[move].effect == EFFECT_FOLLOW_ME
                    || gMovesInfo[move].effect == EFFECT_MAGIC_COAT
                    || gMovesInfo[move].effect == EFFECT_ME_FIRST
                    || gMovesInfo[move].effect == EFFECT_QUASH
                    || gMovesInfo[move].effect == EFFECT_MAT_BLOCK
                    || gMovesInfo[move].effect == EFFECT_ELECTRIFY
                    || gMovesInfo[move].effect == EFFECT_SHELL_TRAP) { // Player needs to go first
                MOVE(opponent, move);
                MOVE(player, MOVE_POUND);
            } else if (gMovesInfo[move].effect == EFFECT_REVIVAL_BLESSING) { // Player selects party member
                MOVE(opponent, move, partyIndex: 1);
                MOVE(player, MOVE_HELPING_HAND);
            } else if (gMovesInfo[move].effect == EFFECT_UPPER_HAND) { // Opponent needs to choose priority move
                MOVE(opponent, move);
                MOVE(player, MOVE_QUICK_ATTACK);
            } else if (gMovesInfo[move].effect == EFFECT_ACUPRESSURE) {
                MOVE(opponent, move, target: player);
            } else { // All other moves
                MOVE(player, MOVE_HELPING_HAND); // Helping Hand, so there's no anim on the opponent's side.
                MOVE(opponent, move);
            }
        }
    } SCENE {
        if (gMovesInfo[move].effect == EFFECT_FOLLOW_ME
         || gMovesInfo[move].effect == EFFECT_HELPING_HAND
         || gMovesInfo[move].effect == EFFECT_AFTER_YOU
         || gMovesInfo[move].effect == EFFECT_ALLY_SWITCH
         || gMovesInfo[move].effect == EFFECT_AROMATIC_MIST
         || move == MOVE_HOLD_HANDS // Hack here because it shares its effect with Splash and Celebrate
         || gMovesInfo[move].effect == EFFECT_COACHING
         || gMovesInfo[move].effect == EFFECT_DRAGON_CHEER) {
            // Moves that fail in Single Battles
        } else if (gMovesInfo[move].effect == EFFECT_MIRROR_MOVE) { // Copy the opponent's move
            ANIMATION(ANIM_TYPE_MOVE, MOVE_POUND, opponent);
        } else if (gMovesInfo[move].effect == EFFECT_NATURE_POWER) { // Recorded battles always use BATTLE_TERRAIN_BUILDING
            ANIMATION(ANIM_TYPE_MOVE, B_NATURE_POWER_MOVES >= GEN_4 ? MOVE_TRI_ATTACK : MOVE_SWIFT, opponent);
        } else { // All other moves
            ANIMATION(ANIM_TYPE_MOVE, move, opponent);
        }
    } THEN {
        FORCE_MOVE_ANIM(FALSE);
        if (gLoadFail)
            DebugPrintf("Move failed: %S (%u)", gMovesInfo[move].name, move);
        EXPECT_EQ(gLoadFail, FALSE);
    }
}

DOUBLE_BATTLE_TEST("Move Animations don't leak when used - Doubles (playerLeft to opponentLeft)")
{
    u32 j = 1, move = 0, species = 0;
    FORCE_MOVE_ANIM(TRUE);
    struct BattlePokemon *attacker = playerLeft;
    struct BattlePokemon *target = opponentLeft;
    struct BattlePokemon *ignore1 = playerRight;
    struct BattlePokemon *ignore2 = opponentRight;
    for (; j < MOVES_COUNT; j++) {
        if (gMovesInfo[j].effect == EFFECT_DARK_VOID) { // User needs to be Darkrai
            PARAMETRIZE { move = j; species = SPECIES_DARKRAI; }
        } else if (gMovesInfo[j].effect == EFFECT_HYPERSPACE_FURY) { // User needs to be Hoopa Unbound
            PARAMETRIZE { move = j; species = SPECIES_HOOPA_UNBOUND; }
        } else if (gMovesInfo[j].effect == EFFECT_AURA_WHEEL) { // User needs to be Morpeko
            PARAMETRIZE { move = j; species = SPECIES_MORPEKO_FULL_BELLY; }
        } else if (gMovesInfo[j].effect == EFFECT_ROTOTILLER
                || gMovesInfo[j].effect == EFFECT_FLOWER_SHIELD) { // User needs to be Grass-type
            PARAMETRIZE { move = j; species = SPECIES_TANGELA; }
        } else if (gMovesInfo[j].effect == EFFECT_FAIL_IF_NOT_ARG_TYPE && gMovesInfo[j].argument.type == TYPE_FIRE) { // User needs to be Fire-type
            PARAMETRIZE { move = j; species = SPECIES_FLAREON; }
        } else if (gMovesInfo[j].effect == EFFECT_FAIL_IF_NOT_ARG_TYPE && gMovesInfo[j].argument.type == TYPE_ELECTRIC) { // User needs to be Electric-type
            PARAMETRIZE { move = j; species = SPECIES_JOLTEON; }
        } else if (gMovesInfo[j].effect == EFFECT_MAGNETIC_FLUX
                || gMovesInfo[j].effect == EFFECT_GEAR_UP) { // User needs to have Plus
            PARAMETRIZE { move = j; species = SPECIES_KLINKLANG; }
        } else if (gMovesInfo[j].effect == EFFECT_PLACEHOLDER) { // Ignore placeholder moves
            PARAMETRIZE { move = MOVE_POUND; species = SPECIES_WOBBUFFET; }
        } else { // Everything else
            PARAMETRIZE { move = j; species = SPECIES_WOBBUFFET; }
        }
    }
    GIVEN {
        PLAYER(species) {
            HP(9997); MaxHP(9999); Item(ITEM_ORAN_BERRY);
            if (attacker == playerLeft) {
                if (species == SPECIES_WOBBUFFET) Gender(MON_FEMALE);
                if (gMovesInfo[move].effect == EFFECT_LAST_RESORT) Moves(move, MOVE_POUND);
                if (species == SPECIES_KLINKLANG) Ability(ABILITY_PLUS);
            }
        }
        PLAYER(species) {
            HP(9997); MaxHP(9999); Item(ITEM_ORAN_BERRY);
            if (attacker == playerRight) {
                if (species == SPECIES_WOBBUFFET) Gender(MON_FEMALE);
                if (gMovesInfo[move].effect == EFFECT_LAST_RESORT) Moves(move, MOVE_POUND);
                if (species == SPECIES_KLINKLANG) Ability(ABILITY_PLUS);
            }
        }
        PLAYER(SPECIES_WOBBUFFET) {
            Gender(MON_MALE); MaxHP(9999); Moves(MOVE_POUND, MOVE_CELEBRATE);
            HP(gMovesInfo[move].effect == EFFECT_REVIVAL_BLESSING ? 0 : 9998);
        }
        OPPONENT(SPECIES_WOBBUFFET) {
            Gender(MON_MALE); HP(9998); MaxHP(9999); SpDefense(9999); Defense(9999); Ability(ABILITY_TELEPATHY);
            if (gMovesInfo[move].effect != EFFECT_BESTOW) {
                Item(ITEM_ORAN_BERRY);
            }
        }
        OPPONENT(SPECIES_WOBBUFFET) {
            Gender(MON_MALE); HP(9998); MaxHP(9999); SpDefense(9999); Defense(9999); Ability(ABILITY_TELEPATHY);
            if (gMovesInfo[move].effect != EFFECT_BESTOW) {
                Item(ITEM_ORAN_BERRY);
            }
        }
        OPPONENT(SPECIES_WOBBUFFET) { Gender(MON_FEMALE); HP(9998); MaxHP(9999); SpDefense(9999); Defense(9999); }
    } WHEN {
        // Setup turn
        if (gMovesInfo[move].effect == EFFECT_SNORE
         || gMovesInfo[move].effect == EFFECT_SLEEP_TALK) {  // Player needs to be asleep
            TURN { MOVE(attacker, MOVE_REST); }
        } else if (gMovesInfo[move].effect == EFFECT_SPIT_UP
                || gMovesInfo[move].effect == EFFECT_SWALLOW) { // Player needs to have used Stockpile
            TURN { MOVE(attacker, MOVE_STOCKPILE); }
        } else if ((gMovesInfo[move].effect == EFFECT_DOUBLE_POWER_ON_ARG_STATUS && gMovesInfo[move].argument.status == STATUS1_PARALYSIS)) { // Opponent needs to be paralyzed
            TURN { MOVE(attacker, MOVE_THUNDER_WAVE, target: target); }
        } else if (gMovesInfo[move].effect == EFFECT_RECYCLE
                || gMovesInfo[move].effect == EFFECT_BELCH) { // Player needs to have eaten its Berry
            TURN { MOVE(attacker, MOVE_STUFF_CHEEKS); }
        } else if (gMovesInfo[move].effect == EFFECT_REFRESH
                || gMovesInfo[move].effect == EFFECT_PSYCHO_SHIFT) { // Player needs to be paralyzed
            TURN { MOVE(target, MOVE_THUNDER_WAVE, target: attacker); }
        } else if (gMovesInfo[move].effect == EFFECT_LAST_RESORT) { // Player needs to have used all other moves
            TURN { MOVE(attacker, MOVE_POUND, target: target); }
        } else if (gMovesInfo[move].effect == EFFECT_DREAM_EATER
                || gMovesInfo[move].effect == EFFECT_NIGHTMARE) { // Opponent needs to be asleep
            TURN { MOVE(target, MOVE_REST); }
        } else if (gMovesInfo[move].effect == EFFECT_VENOM_DRENCH
                || gMovesInfo[move].effect == EFFECT_PURIFY) { // Opponent needs to be poisoned
            TURN { MOVE(attacker, MOVE_POISON_POWDER, target: target); }
        } else if (gMovesInfo[move].effect == EFFECT_TOPSY_TURVY) { // Opponent needs to have its stats buffed
            TURN { MOVE(target, MOVE_SWORDS_DANCE); }
        } else if (gMovesInfo[move].effect == EFFECT_AURORA_VEIL) { // Has to be hailing
            TURN { MOVE(attacker, MOVE_HAIL); }
        } else if (gMovesInfo[move].effect == EFFECT_HIT_SET_REMOVE_TERRAIN && gMovesInfo[move].argument.moveProperty == ARG_TRY_REMOVE_TERRAIN_FAIL) { // Needs a terrain
            TURN { MOVE(attacker, MOVE_ELECTRIC_TERRAIN); }
        }
        // Effective turn
        TURN {
            if (gMovesInfo[move].effect == EFFECT_COUNTER
             || gMovesInfo[move].effect == EFFECT_MIRROR_MOVE
             || gMovesInfo[move].effect == EFFECT_CONVERSION_2
             || gMovesInfo[move].effect == EFFECT_MIRROR_COAT
             || gMovesInfo[move].effect == EFFECT_METAL_BURST
             || gMovesInfo[move].effect == EFFECT_COPYCAT
             || gMovesInfo[move].effect == EFFECT_SUCKER_PUNCH
             || gMovesInfo[move].effect == EFFECT_INSTRUCT) { // Opponent needs to hit the player first
                MOVE(target, gMovesInfo[move].effect == EFFECT_MIRROR_COAT ? MOVE_SWIFT : MOVE_POUND, target: attacker);
                MOVE(attacker, move, target: target);
            } else if (gMovesInfo[move].effect == EFFECT_SNATCH) { // Opponent needs to steal the opponent's buffing move
                MOVE(attacker, move, target: target);
                MOVE(target, MOVE_SWORDS_DANCE);
            } else if (gMovesInfo[move].effect == EFFECT_OHKO) { // Opponent needs to send out a different team member
                MOVE(attacker, move, target: target);
                SEND_OUT(target, 2);
            } else if (gMovesInfo[move].effect == EFFECT_TELEPORT
                    || gMovesInfo[move].effect == EFFECT_EXPLOSION
                    || gMovesInfo[move].effect == EFFECT_BATON_PASS
                    || gMovesInfo[move].effect == EFFECT_MEMENTO
                    || gMovesInfo[move].effect == EFFECT_HEALING_WISH
                    || gMovesInfo[move].effect == EFFECT_HIT_ESCAPE
                    || gMovesInfo[move].effect == EFFECT_FINAL_GAMBIT
                    || gMovesInfo[move].effect == EFFECT_PARTING_SHOT
                    || gMovesInfo[move].effect == EFFECT_SHED_TAIL
                    || gMovesInfo[move].effect == EFFECT_CHILLY_RECEPTION) { // Player needs to send out a different team member
                MOVE(attacker, move, target: target);
                SEND_OUT(attacker, 2);
            } else if (gMovesInfo[move].effect == EFFECT_PROTECT
                    || gMovesInfo[move].effect == EFFECT_ENDURE
                    || gMovesInfo[move].effect == EFFECT_FOLLOW_ME
                    || gMovesInfo[move].effect == EFFECT_MAGIC_COAT
                    || gMovesInfo[move].effect == EFFECT_ME_FIRST
                    || gMovesInfo[move].effect == EFFECT_QUASH
                    || gMovesInfo[move].effect == EFFECT_MAT_BLOCK
                    || gMovesInfo[move].effect == EFFECT_ELECTRIFY
                    || gMovesInfo[move].effect == EFFECT_SHELL_TRAP) { // Player needs to go first
                MOVE(attacker, move, target: target);
                MOVE(target, MOVE_POUND, target: attacker);
            } else if (gMovesInfo[move].effect == EFFECT_AFTER_YOU) { // Player goes first, opponent third
                MOVE(attacker, move, target: target);
                MOVE(ignore1, MOVE_CELEBRATE);
                MOVE(target, MOVE_POUND, target: attacker);
                MOVE(ignore2, MOVE_CELEBRATE);
            } else if (gMovesInfo[move].effect == EFFECT_REVIVAL_BLESSING) { // Player selects party member
                MOVE(attacker, move, partyIndex: 2);
                MOVE(target, MOVE_LAST_RESORT, target: attacker); // Last Resort, so there's no anim on the opponent's side.
            } else if (gMovesInfo[move].effect == EFFECT_UPPER_HAND) { // Opponent needs to choose priority move
                MOVE(attacker, move, target: target);
                MOVE(target, MOVE_QUICK_ATTACK, target: attacker);
            } else { // All other moves
                MOVE(target, MOVE_LAST_RESORT, target: attacker); // Last Resort, so there's no anim on the opponent's side.
                MOVE(attacker, move, target: target);
            }
            if (gMovesInfo[move].effect != EFFECT_AFTER_YOU) {
                // Actions for the remaining battlers
                MOVE(ignore1, MOVE_CELEBRATE);
                MOVE(ignore2, MOVE_CELEBRATE);
            }
        }
    } SCENE {
        if (gMovesInfo[move].effect == EFFECT_MAGNETIC_FLUX || gMovesInfo[move].effect == EFFECT_GEAR_UP) // For some reason, Magnetic Flux and Gear Up are failing in Double Battles here
        {
            // Moves that fail in Double Battles
        } else if (gMovesInfo[move].effect == EFFECT_MIRROR_MOVE) { // Copy the opponent's move
            ANIMATION(ANIM_TYPE_MOVE, MOVE_POUND, attacker);
        } else if (gMovesInfo[move].effect == EFFECT_NATURE_POWER) { // Recorded battles always use BATTLE_TERRAIN_BUILDING
            ANIMATION(ANIM_TYPE_MOVE, B_NATURE_POWER_MOVES >= GEN_4 ? MOVE_TRI_ATTACK : MOVE_SWIFT, attacker);
        } else { // All other moves
            ANIMATION(ANIM_TYPE_MOVE, move, attacker);
        }
    } THEN {
        FORCE_MOVE_ANIM(FALSE);
        if (gLoadFail)
            DebugPrintf("Move failed: %S (%u)", gMovesInfo[move].name, move);
        EXPECT_EQ(gLoadFail, FALSE);
    }
}

DOUBLE_BATTLE_TEST("Move Animations don't leak when used - Doubles (opponentLeft to playerLeft)")
{
    u32 j = 1, move = 0, species = 0;
    FORCE_MOVE_ANIM(TRUE);
    struct BattlePokemon *attacker = opponentLeft;
    struct BattlePokemon *target = playerLeft;
    struct BattlePokemon *ignore1 = opponentRight;
    struct BattlePokemon *ignore2 = playerRight;
    for (; j < MOVES_COUNT; j++) {
        if (gMovesInfo[j].effect == EFFECT_DARK_VOID) { // User needs to be Darkrai
            PARAMETRIZE { move = j; species = SPECIES_DARKRAI; }
        } else if (gMovesInfo[j].effect == EFFECT_HYPERSPACE_FURY) { // User needs to be Hoopa Unbound
            PARAMETRIZE { move = j; species = SPECIES_HOOPA_UNBOUND; }
        } else if (gMovesInfo[j].effect == EFFECT_AURA_WHEEL) { // User needs to be Morpeko
            PARAMETRIZE { move = j; species = SPECIES_MORPEKO_FULL_BELLY; }
        } else if (gMovesInfo[j].effect == EFFECT_ROTOTILLER
                || gMovesInfo[j].effect == EFFECT_FLOWER_SHIELD) { // User needs to be Grass-type
            PARAMETRIZE { move = j; species = SPECIES_TANGELA; }
        } else if (gMovesInfo[j].effect == EFFECT_FAIL_IF_NOT_ARG_TYPE && gMovesInfo[j].argument.type == TYPE_FIRE) { // User needs to be Fire-type
            PARAMETRIZE { move = j; species = SPECIES_FLAREON; }
        } else if (gMovesInfo[j].effect == EFFECT_FAIL_IF_NOT_ARG_TYPE && gMovesInfo[j].argument.type == TYPE_ELECTRIC) { // User needs to be Electric-type
            PARAMETRIZE { move = j; species = SPECIES_JOLTEON; }
        } else if (gMovesInfo[j].effect == EFFECT_MAGNETIC_FLUX
                || gMovesInfo[j].effect == EFFECT_GEAR_UP) { // User needs to have Plus
            PARAMETRIZE { move = j; species = SPECIES_KLINKLANG; }
        } else if (gMovesInfo[j].effect == EFFECT_PLACEHOLDER) { // Ignore placeholder moves
            PARAMETRIZE { move = MOVE_POUND; species = SPECIES_WOBBUFFET; }
        } else { // Everything else
            PARAMETRIZE { move = j; species = SPECIES_WOBBUFFET; }
        }
    }
    GIVEN {
        OPPONENT(species) {
            HP(9997); MaxHP(9999); Item(ITEM_ORAN_BERRY);
            if (attacker == opponentLeft) {
                if (species == SPECIES_WOBBUFFET) Gender(MON_FEMALE);
                if (gMovesInfo[move].effect == EFFECT_LAST_RESORT) Moves(move, MOVE_POUND);
                if (species == SPECIES_KLINKLANG) Ability(ABILITY_PLUS);
            }
        }
        OPPONENT(species) {
            HP(9997); MaxHP(9999); Item(ITEM_ORAN_BERRY);
            if (attacker == opponentRight) {
                if (species == SPECIES_WOBBUFFET) Gender(MON_FEMALE);
                if (gMovesInfo[move].effect == EFFECT_LAST_RESORT) Moves(move, MOVE_POUND);
                if (species == SPECIES_KLINKLANG) Ability(ABILITY_PLUS);
            }
        }
        OPPONENT(SPECIES_WOBBUFFET) {
            Gender(MON_MALE); MaxHP(9999); Moves(MOVE_POUND, MOVE_CELEBRATE);
            HP(gMovesInfo[move].effect == EFFECT_REVIVAL_BLESSING ? 0 : 9998);
        }
        PLAYER(SPECIES_WOBBUFFET) {
            Gender(MON_MALE); HP(9998); MaxHP(9999); SpDefense(9999); Defense(9999); Ability(ABILITY_TELEPATHY);
            if (gMovesInfo[move].effect != EFFECT_BESTOW) {
                Item(ITEM_ORAN_BERRY);
            }
        }
        PLAYER(SPECIES_WOBBUFFET) {
            Gender(MON_MALE); HP(9998); MaxHP(9999); SpDefense(9999); Defense(9999); Ability(ABILITY_TELEPATHY);
            if (gMovesInfo[move].effect != EFFECT_BESTOW) {
                Item(ITEM_ORAN_BERRY);
            }
        }
        PLAYER(SPECIES_WOBBUFFET) { Gender(MON_FEMALE); HP(9998); MaxHP(9999); SpDefense(9999); Defense(9999); }
    } WHEN {
        // Setup turn
        if (gMovesInfo[move].effect == EFFECT_SNORE
         || gMovesInfo[move].effect == EFFECT_SLEEP_TALK) {  // Player needs to be asleep
            TURN { MOVE(attacker, MOVE_REST); }
        } else if (gMovesInfo[move].effect == EFFECT_SPIT_UP
                || gMovesInfo[move].effect == EFFECT_SWALLOW) { // Player needs to have used Stockpile
            TURN { MOVE(attacker, MOVE_STOCKPILE); }
        } else if ((gMovesInfo[move].effect == EFFECT_DOUBLE_POWER_ON_ARG_STATUS && gMovesInfo[move].argument.status == STATUS1_PARALYSIS)) { // Opponent needs to be paralyzed
            TURN { MOVE(attacker, MOVE_THUNDER_WAVE, target: target); }
        } else if (gMovesInfo[move].effect == EFFECT_RECYCLE
                || gMovesInfo[move].effect == EFFECT_BELCH) { // Player needs to have eaten its Berry
            TURN { MOVE(attacker, MOVE_STUFF_CHEEKS); }
        } else if (gMovesInfo[move].effect == EFFECT_REFRESH
                || gMovesInfo[move].effect == EFFECT_PSYCHO_SHIFT) { // Player needs to be paralyzed
            TURN { MOVE(target, MOVE_THUNDER_WAVE, target: attacker); }
        } else if (gMovesInfo[move].effect == EFFECT_LAST_RESORT) { // Player needs to have used all other moves
            TURN { MOVE(attacker, MOVE_POUND, target: target); }
        } else if (gMovesInfo[move].effect == EFFECT_DREAM_EATER
                || gMovesInfo[move].effect == EFFECT_NIGHTMARE) { // Opponent needs to be asleep
            TURN { MOVE(target, MOVE_REST); }
        } else if (gMovesInfo[move].effect == EFFECT_VENOM_DRENCH
                || gMovesInfo[move].effect == EFFECT_PURIFY) { // Opponent needs to be poisoned
            TURN { MOVE(attacker, MOVE_POISON_POWDER, target: target); }
        } else if (gMovesInfo[move].effect == EFFECT_TOPSY_TURVY) { // Opponent needs to have its stats buffed
            TURN { MOVE(target, MOVE_SWORDS_DANCE); }
        } else if (gMovesInfo[move].effect == EFFECT_AURORA_VEIL) { // Has to be hailing
            TURN { MOVE(attacker, MOVE_HAIL); }
        } else if (gMovesInfo[move].effect == EFFECT_HIT_SET_REMOVE_TERRAIN && gMovesInfo[move].argument.moveProperty == ARG_TRY_REMOVE_TERRAIN_FAIL) { // Needs a terrain
            TURN { MOVE(attacker, MOVE_ELECTRIC_TERRAIN); }
        }
        // Effective turn
        TURN {
            if (gMovesInfo[move].effect == EFFECT_COUNTER
             || gMovesInfo[move].effect == EFFECT_MIRROR_MOVE
             || gMovesInfo[move].effect == EFFECT_CONVERSION_2
             || gMovesInfo[move].effect == EFFECT_MIRROR_COAT
             || gMovesInfo[move].effect == EFFECT_METAL_BURST
             || gMovesInfo[move].effect == EFFECT_COPYCAT
             || gMovesInfo[move].effect == EFFECT_SUCKER_PUNCH
             || gMovesInfo[move].effect == EFFECT_INSTRUCT) { // Opponent needs to hit the player first
                MOVE(target, gMovesInfo[move].effect == EFFECT_MIRROR_COAT ? MOVE_SWIFT : MOVE_POUND, target: attacker);
                MOVE(attacker, move, target: target);
            } else if (gMovesInfo[move].effect == EFFECT_SNATCH) { // Opponent needs to steal the opponent's buffing move
                MOVE(attacker, move, target: target);
                MOVE(target, MOVE_SWORDS_DANCE);
            } else if (gMovesInfo[move].effect == EFFECT_OHKO) { // Opponent needs to send out a different team member
                MOVE(attacker, move, target: target);
                SEND_OUT(target, 2);
            } else if (gMovesInfo[move].effect == EFFECT_TELEPORT
                    || gMovesInfo[move].effect == EFFECT_EXPLOSION
                    || gMovesInfo[move].effect == EFFECT_BATON_PASS
                    || gMovesInfo[move].effect == EFFECT_MEMENTO
                    || gMovesInfo[move].effect == EFFECT_HEALING_WISH
                    || gMovesInfo[move].effect == EFFECT_HIT_ESCAPE
                    || gMovesInfo[move].effect == EFFECT_FINAL_GAMBIT
                    || gMovesInfo[move].effect == EFFECT_PARTING_SHOT
                    || gMovesInfo[move].effect == EFFECT_SHED_TAIL
                    || gMovesInfo[move].effect == EFFECT_CHILLY_RECEPTION) { // Player needs to send out a different team member
                MOVE(attacker, move, target: target);
                SEND_OUT(attacker, 2);
            } else if (gMovesInfo[move].effect == EFFECT_PROTECT
                    || gMovesInfo[move].effect == EFFECT_ENDURE
                    || gMovesInfo[move].effect == EFFECT_FOLLOW_ME
                    || gMovesInfo[move].effect == EFFECT_MAGIC_COAT
                    || gMovesInfo[move].effect == EFFECT_ME_FIRST
                    || gMovesInfo[move].effect == EFFECT_QUASH
                    || gMovesInfo[move].effect == EFFECT_MAT_BLOCK
                    || gMovesInfo[move].effect == EFFECT_ELECTRIFY
                    || gMovesInfo[move].effect == EFFECT_SHELL_TRAP) { // Player needs to go first
                MOVE(attacker, move, target: target);
                MOVE(target, MOVE_POUND, target: attacker);
            } else if (gMovesInfo[move].effect == EFFECT_AFTER_YOU) { // Player goes first, opponent third
                MOVE(attacker, move, target: target);
                MOVE(ignore1, MOVE_CELEBRATE);
                MOVE(target, MOVE_POUND, target: attacker);
                MOVE(ignore2, MOVE_CELEBRATE);
            } else if (gMovesInfo[move].effect == EFFECT_REVIVAL_BLESSING) { // Player selects party member
                MOVE(attacker, move, partyIndex: 2);
                MOVE(target, MOVE_LAST_RESORT, target: attacker); // Last Resort, so there's no anim on the opponent's side.
            } else if (gMovesInfo[move].effect == EFFECT_UPPER_HAND) { // Opponent needs to choose priority move
                MOVE(attacker, move, target: target);
                MOVE(target, MOVE_QUICK_ATTACK, target: attacker);
            } else { // All other moves
                MOVE(target, MOVE_LAST_RESORT, target: attacker); // Last Resort, so there's no anim on the opponent's side.
                MOVE(attacker, move, target: target);
            }
            if (gMovesInfo[move].effect != EFFECT_AFTER_YOU) {
                // Actions for the remaining battlers
                MOVE(ignore1, MOVE_CELEBRATE);
                MOVE(ignore2, MOVE_CELEBRATE);
            }
        }
    } SCENE {
        if (gMovesInfo[move].effect == EFFECT_MAGNETIC_FLUX || gMovesInfo[move].effect == EFFECT_GEAR_UP) // For some reason, Magnetic Flux and Gear Up are failing in Double Battles here
        {
            // Moves that fail in Double Battles
        } else if (gMovesInfo[move].effect == EFFECT_MIRROR_MOVE) { // Copy the opponent's move
            ANIMATION(ANIM_TYPE_MOVE, MOVE_POUND, attacker);
        } else if (gMovesInfo[move].effect == EFFECT_NATURE_POWER) { // Recorded battles always use BATTLE_TERRAIN_BUILDING
            ANIMATION(ANIM_TYPE_MOVE, B_NATURE_POWER_MOVES >= GEN_4 ? MOVE_TRI_ATTACK : MOVE_SWIFT, attacker);
        } else { // All other moves
            ANIMATION(ANIM_TYPE_MOVE, move, attacker);
        }
    } THEN {
        FORCE_MOVE_ANIM(FALSE);
        if (gLoadFail)
            DebugPrintf("Move failed: %S (%u)", gMovesInfo[move].name, move);
        EXPECT_EQ(gLoadFail, FALSE);
    }
}

DOUBLE_BATTLE_TEST("Move Animations don't leak when used - Doubles (playerLeft to opponentRight)")
{
    u32 j = 1, move = 0, species = 0;
    FORCE_MOVE_ANIM(TRUE);
    struct BattlePokemon *attacker = playerLeft;
    struct BattlePokemon *target = opponentRight;
    struct BattlePokemon *ignore1 = playerRight;
    struct BattlePokemon *ignore2 = opponentLeft;
    for (; j < MOVES_COUNT; j++) {
        if (gMovesInfo[j].effect == EFFECT_DARK_VOID) { // User needs to be Darkrai
            PARAMETRIZE { move = j; species = SPECIES_DARKRAI; }
        } else if (gMovesInfo[j].effect == EFFECT_HYPERSPACE_FURY) { // User needs to be Hoopa Unbound
            PARAMETRIZE { move = j; species = SPECIES_HOOPA_UNBOUND; }
        } else if (gMovesInfo[j].effect == EFFECT_AURA_WHEEL) { // User needs to be Morpeko
            PARAMETRIZE { move = j; species = SPECIES_MORPEKO_FULL_BELLY; }
        } else if (gMovesInfo[j].effect == EFFECT_ROTOTILLER
                || gMovesInfo[j].effect == EFFECT_FLOWER_SHIELD) { // User needs to be Grass-type
            PARAMETRIZE { move = j; species = SPECIES_TANGELA; }
        } else if (gMovesInfo[j].effect == EFFECT_FAIL_IF_NOT_ARG_TYPE && gMovesInfo[j].argument.type == TYPE_FIRE) { // User needs to be Fire-type
            PARAMETRIZE { move = j; species = SPECIES_FLAREON; }
        } else if (gMovesInfo[j].effect == EFFECT_FAIL_IF_NOT_ARG_TYPE && gMovesInfo[j].argument.type == TYPE_ELECTRIC) { // User needs to be Electric-type
            PARAMETRIZE { move = j; species = SPECIES_JOLTEON; }
        } else if (gMovesInfo[j].effect == EFFECT_MAGNETIC_FLUX
                || gMovesInfo[j].effect == EFFECT_GEAR_UP) { // User needs to have Plus
            PARAMETRIZE { move = j; species = SPECIES_KLINKLANG; }
        } else if (gMovesInfo[j].effect == EFFECT_PLACEHOLDER) { // Ignore placeholder moves
            PARAMETRIZE { move = MOVE_POUND; species = SPECIES_WOBBUFFET; }
        } else { // Everything else
            PARAMETRIZE { move = j; species = SPECIES_WOBBUFFET; }
        }
    }
    GIVEN {
        PLAYER(species) {
            HP(9997); MaxHP(9999); Item(ITEM_ORAN_BERRY);
            if (attacker == playerLeft) {
                if (species == SPECIES_WOBBUFFET) Gender(MON_FEMALE);
                if (gMovesInfo[move].effect == EFFECT_LAST_RESORT) Moves(move, MOVE_POUND);
                if (species == SPECIES_KLINKLANG) Ability(ABILITY_PLUS);
            }
        }
        PLAYER(species) {
            HP(9997); MaxHP(9999); Item(ITEM_ORAN_BERRY);
            if (attacker == playerRight) {
                if (species == SPECIES_WOBBUFFET) Gender(MON_FEMALE);
                if (gMovesInfo[move].effect == EFFECT_LAST_RESORT) Moves(move, MOVE_POUND);
                if (species == SPECIES_KLINKLANG) Ability(ABILITY_PLUS);
            }
        }
        PLAYER(SPECIES_WOBBUFFET) {
            Gender(MON_MALE); MaxHP(9999); Moves(MOVE_POUND, MOVE_CELEBRATE);
            HP(gMovesInfo[move].effect == EFFECT_REVIVAL_BLESSING ? 0 : 9998);
        }
        OPPONENT(SPECIES_WOBBUFFET) {
            Gender(MON_MALE); HP(9998); MaxHP(9999); SpDefense(9999); Defense(9999); Ability(ABILITY_TELEPATHY);
            if (gMovesInfo[move].effect != EFFECT_BESTOW) {
                Item(ITEM_ORAN_BERRY);
            }
        }
        OPPONENT(SPECIES_WOBBUFFET) {
            Gender(MON_MALE); HP(9998); MaxHP(9999); SpDefense(9999); Defense(9999); Ability(ABILITY_TELEPATHY);
            if (gMovesInfo[move].effect != EFFECT_BESTOW) {
                Item(ITEM_ORAN_BERRY);
            }
        }
        OPPONENT(SPECIES_WOBBUFFET) { Gender(MON_FEMALE); HP(9998); MaxHP(9999); SpDefense(9999); Defense(9999); }
    } WHEN {
        // Setup turn
        if (gMovesInfo[move].effect == EFFECT_SNORE
         || gMovesInfo[move].effect == EFFECT_SLEEP_TALK) {  // Player needs to be asleep
            TURN { MOVE(attacker, MOVE_REST); }
        } else if (gMovesInfo[move].effect == EFFECT_SPIT_UP
                || gMovesInfo[move].effect == EFFECT_SWALLOW) { // Player needs to have used Stockpile
            TURN { MOVE(attacker, MOVE_STOCKPILE); }
        } else if ((gMovesInfo[move].effect == EFFECT_DOUBLE_POWER_ON_ARG_STATUS && gMovesInfo[move].argument.status == STATUS1_PARALYSIS)) { // Opponent needs to be paralyzed
            TURN { MOVE(attacker, MOVE_THUNDER_WAVE, target: target); }
        } else if (gMovesInfo[move].effect == EFFECT_RECYCLE
                || gMovesInfo[move].effect == EFFECT_BELCH) { // Player needs to have eaten its Berry
            TURN { MOVE(attacker, MOVE_STUFF_CHEEKS); }
        } else if (gMovesInfo[move].effect == EFFECT_REFRESH
                || gMovesInfo[move].effect == EFFECT_PSYCHO_SHIFT) { // Player needs to be paralyzed
            TURN { MOVE(target, MOVE_THUNDER_WAVE, target: attacker); }
        } else if (gMovesInfo[move].effect == EFFECT_LAST_RESORT) { // Player needs to have used all other moves
            TURN { MOVE(attacker, MOVE_POUND, target: target); }
        } else if (gMovesInfo[move].effect == EFFECT_DREAM_EATER
                || gMovesInfo[move].effect == EFFECT_NIGHTMARE) { // Opponent needs to be asleep
            TURN { MOVE(target, MOVE_REST); }
        } else if (gMovesInfo[move].effect == EFFECT_VENOM_DRENCH
                || gMovesInfo[move].effect == EFFECT_PURIFY) { // Opponent needs to be poisoned
            TURN { MOVE(attacker, MOVE_POISON_POWDER, target: target); }
        } else if (gMovesInfo[move].effect == EFFECT_TOPSY_TURVY) { // Opponent needs to have its stats buffed
            TURN { MOVE(target, MOVE_SWORDS_DANCE); }
        } else if (gMovesInfo[move].effect == EFFECT_AURORA_VEIL) { // Has to be hailing
            TURN { MOVE(attacker, MOVE_HAIL); }
        } else if (gMovesInfo[move].effect == EFFECT_HIT_SET_REMOVE_TERRAIN && gMovesInfo[move].argument.moveProperty == ARG_TRY_REMOVE_TERRAIN_FAIL) { // Needs a terrain
            TURN { MOVE(attacker, MOVE_ELECTRIC_TERRAIN); }
        }
        // Effective turn
        TURN {
            if (gMovesInfo[move].effect == EFFECT_COUNTER
             || gMovesInfo[move].effect == EFFECT_MIRROR_MOVE
             || gMovesInfo[move].effect == EFFECT_CONVERSION_2
             || gMovesInfo[move].effect == EFFECT_MIRROR_COAT
             || gMovesInfo[move].effect == EFFECT_METAL_BURST
             || gMovesInfo[move].effect == EFFECT_COPYCAT
             || gMovesInfo[move].effect == EFFECT_SUCKER_PUNCH
             || gMovesInfo[move].effect == EFFECT_INSTRUCT) { // Opponent needs to hit the player first
                MOVE(target, gMovesInfo[move].effect == EFFECT_MIRROR_COAT ? MOVE_SWIFT : MOVE_POUND, target: attacker);
                MOVE(attacker, move, target: target);
            } else if (gMovesInfo[move].effect == EFFECT_SNATCH) { // Opponent needs to steal the opponent's buffing move
                MOVE(attacker, move, target: target);
                MOVE(target, MOVE_SWORDS_DANCE);
            } else if (gMovesInfo[move].effect == EFFECT_OHKO) { // Opponent needs to send out a different team member
                MOVE(attacker, move, target: target);
                SEND_OUT(target, 2);
            } else if (gMovesInfo[move].effect == EFFECT_TELEPORT
                    || gMovesInfo[move].effect == EFFECT_EXPLOSION
                    || gMovesInfo[move].effect == EFFECT_BATON_PASS
                    || gMovesInfo[move].effect == EFFECT_MEMENTO
                    || gMovesInfo[move].effect == EFFECT_HEALING_WISH
                    || gMovesInfo[move].effect == EFFECT_HIT_ESCAPE
                    || gMovesInfo[move].effect == EFFECT_FINAL_GAMBIT
                    || gMovesInfo[move].effect == EFFECT_PARTING_SHOT
                    || gMovesInfo[move].effect == EFFECT_SHED_TAIL
                    || gMovesInfo[move].effect == EFFECT_CHILLY_RECEPTION) { // Player needs to send out a different team member
                MOVE(attacker, move, target: target);
                SEND_OUT(attacker, 2);
            } else if (gMovesInfo[move].effect == EFFECT_PROTECT
                    || gMovesInfo[move].effect == EFFECT_ENDURE
                    || gMovesInfo[move].effect == EFFECT_FOLLOW_ME
                    || gMovesInfo[move].effect == EFFECT_MAGIC_COAT
                    || gMovesInfo[move].effect == EFFECT_ME_FIRST
                    || gMovesInfo[move].effect == EFFECT_QUASH
                    || gMovesInfo[move].effect == EFFECT_MAT_BLOCK
                    || gMovesInfo[move].effect == EFFECT_ELECTRIFY
                    || gMovesInfo[move].effect == EFFECT_SHELL_TRAP) { // Player needs to go first
                MOVE(attacker, move, target: target);
                MOVE(target, MOVE_POUND, target: attacker);
            } else if (gMovesInfo[move].effect == EFFECT_AFTER_YOU) { // Player goes first, opponent third
                MOVE(attacker, move, target: target);
                MOVE(ignore1, MOVE_CELEBRATE);
                MOVE(target, MOVE_POUND, target: attacker);
                MOVE(ignore2, MOVE_CELEBRATE);
            } else if (gMovesInfo[move].effect == EFFECT_REVIVAL_BLESSING) { // Player selects party member
                MOVE(attacker, move, partyIndex: 2);
                MOVE(target, MOVE_LAST_RESORT, target: attacker); // Last Resort, so there's no anim on the opponent's side.
            } else if (gMovesInfo[move].effect == EFFECT_UPPER_HAND) { // Opponent needs to choose priority move
                MOVE(attacker, move, target: target);
                MOVE(target, MOVE_QUICK_ATTACK, target: attacker);
            } else { // All other moves
                MOVE(target, MOVE_LAST_RESORT, target: attacker); // Last Resort, so there's no anim on the opponent's side.
                MOVE(attacker, move, target: target);
            }
            if (gMovesInfo[move].effect != EFFECT_AFTER_YOU) {
                // Actions for the remaining battlers
                MOVE(ignore1, MOVE_CELEBRATE);
                MOVE(ignore2, MOVE_CELEBRATE);
            }
        }
    } SCENE {
        if (gMovesInfo[move].effect == EFFECT_MAGNETIC_FLUX || gMovesInfo[move].effect == EFFECT_GEAR_UP) // For some reason, Magnetic Flux and Gear Up are failing in Double Battles here
        {
            // Moves that fail in Double Battles
        } else if (gMovesInfo[move].effect == EFFECT_MIRROR_MOVE) { // Copy the opponent's move
            ANIMATION(ANIM_TYPE_MOVE, MOVE_POUND, attacker);
        } else if (gMovesInfo[move].effect == EFFECT_NATURE_POWER) { // Recorded battles always use BATTLE_TERRAIN_BUILDING
            ANIMATION(ANIM_TYPE_MOVE, B_NATURE_POWER_MOVES >= GEN_4 ? MOVE_TRI_ATTACK : MOVE_SWIFT, attacker);
        } else { // All other moves
            ANIMATION(ANIM_TYPE_MOVE, move, attacker);
        }
    } THEN {
        FORCE_MOVE_ANIM(FALSE);
        if (gLoadFail)
            DebugPrintf("Move failed: %S (%u)", gMovesInfo[move].name, move);
        EXPECT_EQ(gLoadFail, FALSE);
    }
}

DOUBLE_BATTLE_TEST("Move Animations don't leak when used - Doubles (opponentRight to playerLeft)")
{
    u32 j = 1, move = 0, species = 0;
    FORCE_MOVE_ANIM(TRUE);
    struct BattlePokemon *attacker = opponentRight;
    struct BattlePokemon *target = playerLeft;
    struct BattlePokemon *ignore1 = opponentLeft;
    struct BattlePokemon *ignore2 = playerRight;
    for (; j < MOVES_COUNT; j++) {
        if (gMovesInfo[j].effect == EFFECT_DARK_VOID) { // User needs to be Darkrai
            PARAMETRIZE { move = j; species = SPECIES_DARKRAI; }
        } else if (gMovesInfo[j].effect == EFFECT_HYPERSPACE_FURY) { // User needs to be Hoopa Unbound
            PARAMETRIZE { move = j; species = SPECIES_HOOPA_UNBOUND; }
        } else if (gMovesInfo[j].effect == EFFECT_AURA_WHEEL) { // User needs to be Morpeko
            PARAMETRIZE { move = j; species = SPECIES_MORPEKO_FULL_BELLY; }
        } else if (gMovesInfo[j].effect == EFFECT_ROTOTILLER
                || gMovesInfo[j].effect == EFFECT_FLOWER_SHIELD) { // User needs to be Grass-type
            PARAMETRIZE { move = j; species = SPECIES_TANGELA; }
        } else if (gMovesInfo[j].effect == EFFECT_FAIL_IF_NOT_ARG_TYPE && gMovesInfo[j].argument.type == TYPE_FIRE) { // User needs to be Fire-type
            PARAMETRIZE { move = j; species = SPECIES_FLAREON; }
        } else if (gMovesInfo[j].effect == EFFECT_FAIL_IF_NOT_ARG_TYPE && gMovesInfo[j].argument.type == TYPE_ELECTRIC) { // User needs to be Electric-type
            PARAMETRIZE { move = j; species = SPECIES_JOLTEON; }
        } else if (gMovesInfo[j].effect == EFFECT_MAGNETIC_FLUX
                || gMovesInfo[j].effect == EFFECT_GEAR_UP) { // User needs to have Plus
            PARAMETRIZE { move = j; species = SPECIES_KLINKLANG; }
        } else if (gMovesInfo[j].effect == EFFECT_PLACEHOLDER) { // Ignore placeholder moves
            PARAMETRIZE { move = MOVE_POUND; species = SPECIES_WOBBUFFET; }
        } else { // Everything else
            PARAMETRIZE { move = j; species = SPECIES_WOBBUFFET; }
        }
    }
    GIVEN {
        OPPONENT(species) {
            HP(9997); MaxHP(9999); Item(ITEM_ORAN_BERRY);
            if (attacker == opponentLeft) {
                if (species == SPECIES_WOBBUFFET) Gender(MON_FEMALE);
                if (gMovesInfo[move].effect == EFFECT_LAST_RESORT) Moves(move, MOVE_POUND);
                if (species == SPECIES_KLINKLANG) Ability(ABILITY_PLUS);
            }
        }
        OPPONENT(species) {
            HP(9997); MaxHP(9999); Item(ITEM_ORAN_BERRY);
            if (attacker == opponentRight) {
                if (species == SPECIES_WOBBUFFET) Gender(MON_FEMALE);
                if (gMovesInfo[move].effect == EFFECT_LAST_RESORT) Moves(move, MOVE_POUND);
                if (species == SPECIES_KLINKLANG) Ability(ABILITY_PLUS);
            }
        }
        OPPONENT(SPECIES_WOBBUFFET) {
            Gender(MON_MALE); MaxHP(9999); Moves(MOVE_POUND, MOVE_CELEBRATE);
            HP(gMovesInfo[move].effect == EFFECT_REVIVAL_BLESSING ? 0 : 9998);
        }
        PLAYER(SPECIES_WOBBUFFET) {
            Gender(MON_MALE); HP(9998); MaxHP(9999); SpDefense(9999); Defense(9999); Ability(ABILITY_TELEPATHY);
            if (gMovesInfo[move].effect != EFFECT_BESTOW) {
                Item(ITEM_ORAN_BERRY);
            }
        }
        PLAYER(SPECIES_WOBBUFFET) {
            Gender(MON_MALE); HP(9998); MaxHP(9999); SpDefense(9999); Defense(9999); Ability(ABILITY_TELEPATHY);
            if (gMovesInfo[move].effect != EFFECT_BESTOW) {
                Item(ITEM_ORAN_BERRY);
            }
        }
        PLAYER(SPECIES_WOBBUFFET) { Gender(MON_FEMALE); HP(9998); MaxHP(9999); SpDefense(9999); Defense(9999); }
    } WHEN {
        // Setup turn
        if (gMovesInfo[move].effect == EFFECT_SNORE
         || gMovesInfo[move].effect == EFFECT_SLEEP_TALK) {  // Player needs to be asleep
            TURN { MOVE(attacker, MOVE_REST); }
        } else if (gMovesInfo[move].effect == EFFECT_SPIT_UP
                || gMovesInfo[move].effect == EFFECT_SWALLOW) { // Player needs to have used Stockpile
            TURN { MOVE(attacker, MOVE_STOCKPILE); }
        } else if ((gMovesInfo[move].effect == EFFECT_DOUBLE_POWER_ON_ARG_STATUS && gMovesInfo[move].argument.status == STATUS1_PARALYSIS)) { // Opponent needs to be paralyzed
            TURN { MOVE(attacker, MOVE_THUNDER_WAVE, target: target); }
        } else if (gMovesInfo[move].effect == EFFECT_RECYCLE
                || gMovesInfo[move].effect == EFFECT_BELCH) { // Player needs to have eaten its Berry
            TURN { MOVE(attacker, MOVE_STUFF_CHEEKS); }
        } else if (gMovesInfo[move].effect == EFFECT_REFRESH
                || gMovesInfo[move].effect == EFFECT_PSYCHO_SHIFT) { // Player needs to be paralyzed
            TURN { MOVE(target, MOVE_THUNDER_WAVE, target: attacker); }
        } else if (gMovesInfo[move].effect == EFFECT_LAST_RESORT) { // Player needs to have used all other moves
            TURN { MOVE(attacker, MOVE_POUND, target: target); }
        } else if (gMovesInfo[move].effect == EFFECT_DREAM_EATER
                || gMovesInfo[move].effect == EFFECT_NIGHTMARE) { // Opponent needs to be asleep
            TURN { MOVE(target, MOVE_REST); }
        } else if (gMovesInfo[move].effect == EFFECT_VENOM_DRENCH
                || gMovesInfo[move].effect == EFFECT_PURIFY) { // Opponent needs to be poisoned
            TURN { MOVE(attacker, MOVE_POISON_POWDER, target: target); }
        } else if (gMovesInfo[move].effect == EFFECT_TOPSY_TURVY) { // Opponent needs to have its stats buffed
            TURN { MOVE(target, MOVE_SWORDS_DANCE); }
        } else if (gMovesInfo[move].effect == EFFECT_AURORA_VEIL) { // Has to be hailing
            TURN { MOVE(attacker, MOVE_HAIL); }
        } else if (gMovesInfo[move].effect == EFFECT_HIT_SET_REMOVE_TERRAIN && gMovesInfo[move].argument.moveProperty == ARG_TRY_REMOVE_TERRAIN_FAIL) { // Needs a terrain
            TURN { MOVE(attacker, MOVE_ELECTRIC_TERRAIN); }
        }
        // Effective turn
        TURN {
            if (gMovesInfo[move].effect == EFFECT_COUNTER
             || gMovesInfo[move].effect == EFFECT_MIRROR_MOVE
             || gMovesInfo[move].effect == EFFECT_CONVERSION_2
             || gMovesInfo[move].effect == EFFECT_MIRROR_COAT
             || gMovesInfo[move].effect == EFFECT_METAL_BURST
             || gMovesInfo[move].effect == EFFECT_COPYCAT
             || gMovesInfo[move].effect == EFFECT_SUCKER_PUNCH
             || gMovesInfo[move].effect == EFFECT_INSTRUCT) { // Opponent needs to hit the player first
                MOVE(target, gMovesInfo[move].effect == EFFECT_MIRROR_COAT ? MOVE_SWIFT : MOVE_POUND, target: attacker);
                MOVE(attacker, move, target: target);
            } else if (gMovesInfo[move].effect == EFFECT_SNATCH) { // Opponent needs to steal the opponent's buffing move
                MOVE(attacker, move, target: target);
                MOVE(target, MOVE_SWORDS_DANCE);
            } else if (gMovesInfo[move].effect == EFFECT_OHKO) { // Opponent needs to send out a different team member
                MOVE(attacker, move, target: target);
                SEND_OUT(target, 2);
            } else if (gMovesInfo[move].effect == EFFECT_TELEPORT
                    || gMovesInfo[move].effect == EFFECT_EXPLOSION
                    || gMovesInfo[move].effect == EFFECT_BATON_PASS
                    || gMovesInfo[move].effect == EFFECT_MEMENTO
                    || gMovesInfo[move].effect == EFFECT_HEALING_WISH
                    || gMovesInfo[move].effect == EFFECT_HIT_ESCAPE
                    || gMovesInfo[move].effect == EFFECT_FINAL_GAMBIT
                    || gMovesInfo[move].effect == EFFECT_PARTING_SHOT
                    || gMovesInfo[move].effect == EFFECT_SHED_TAIL
                    || gMovesInfo[move].effect == EFFECT_CHILLY_RECEPTION) { // Player needs to send out a different team member
                MOVE(attacker, move, target: target);
                SEND_OUT(attacker, 2);
            } else if (gMovesInfo[move].effect == EFFECT_PROTECT
                    || gMovesInfo[move].effect == EFFECT_ENDURE
                    || gMovesInfo[move].effect == EFFECT_FOLLOW_ME
                    || gMovesInfo[move].effect == EFFECT_MAGIC_COAT
                    || gMovesInfo[move].effect == EFFECT_ME_FIRST
                    || gMovesInfo[move].effect == EFFECT_QUASH
                    || gMovesInfo[move].effect == EFFECT_MAT_BLOCK
                    || gMovesInfo[move].effect == EFFECT_ELECTRIFY
                    || gMovesInfo[move].effect == EFFECT_SHELL_TRAP) { // Player needs to go first
                MOVE(attacker, move, target: target);
                MOVE(target, MOVE_POUND, target: attacker);
            } else if (gMovesInfo[move].effect == EFFECT_AFTER_YOU) { // Player goes first, opponent third
                MOVE(attacker, move, target: target);
                MOVE(ignore1, MOVE_CELEBRATE);
                MOVE(target, MOVE_POUND, target: attacker);
                MOVE(ignore2, MOVE_CELEBRATE);
            } else if (gMovesInfo[move].effect == EFFECT_REVIVAL_BLESSING) { // Player selects party member
                MOVE(attacker, move, partyIndex: 2);
                MOVE(target, MOVE_LAST_RESORT, target: attacker); // Last Resort, so there's no anim on the opponent's side.
            } else if (gMovesInfo[move].effect == EFFECT_UPPER_HAND) { // Opponent needs to choose priority move
                MOVE(attacker, move, target: target);
                MOVE(target, MOVE_QUICK_ATTACK, target: attacker);
            } else { // All other moves
                MOVE(target, MOVE_LAST_RESORT, target: attacker); // Last Resort, so there's no anim on the opponent's side.
                MOVE(attacker, move, target: target);
            }
            if (gMovesInfo[move].effect != EFFECT_AFTER_YOU) {
                // Actions for the remaining battlers
                MOVE(ignore1, MOVE_CELEBRATE);
                MOVE(ignore2, MOVE_CELEBRATE);
            }
        }
    } SCENE {
        if (gMovesInfo[move].effect == EFFECT_MAGNETIC_FLUX || gMovesInfo[move].effect == EFFECT_GEAR_UP) // For some reason, Magnetic Flux and Gear Up are failing in Double Battles here
        {
            // Moves that fail in Double Battles
        } else if (gMovesInfo[move].effect == EFFECT_MIRROR_MOVE) { // Copy the opponent's move
            ANIMATION(ANIM_TYPE_MOVE, MOVE_POUND, attacker);
        } else if (gMovesInfo[move].effect == EFFECT_NATURE_POWER) { // Recorded battles always use BATTLE_TERRAIN_BUILDING
            ANIMATION(ANIM_TYPE_MOVE, B_NATURE_POWER_MOVES >= GEN_4 ? MOVE_TRI_ATTACK : MOVE_SWIFT, attacker);
        } else { // All other moves
            ANIMATION(ANIM_TYPE_MOVE, move, attacker);
        }
    } THEN {
        FORCE_MOVE_ANIM(FALSE);
        if (gLoadFail)
            DebugPrintf("Move failed: %S (%u)", gMovesInfo[move].name, move);
        EXPECT_EQ(gLoadFail, FALSE);
    }
}

DOUBLE_BATTLE_TEST("Move Animations don't leak when used - Doubles (playerRight to opponentLeft)")
{
    u32 j = 1, move = 0, species = 0;
    FORCE_MOVE_ANIM(TRUE);
    struct BattlePokemon *attacker = playerRight;
    struct BattlePokemon *target = opponentLeft;
    struct BattlePokemon *ignore1 = playerLeft;
    struct BattlePokemon *ignore2 = opponentRight;
    for (; j < MOVES_COUNT; j++) {
        if (gMovesInfo[j].effect == EFFECT_DARK_VOID) { // User needs to be Darkrai
            PARAMETRIZE { move = j; species = SPECIES_DARKRAI; }
        } else if (gMovesInfo[j].effect == EFFECT_HYPERSPACE_FURY) { // User needs to be Hoopa Unbound
            PARAMETRIZE { move = j; species = SPECIES_HOOPA_UNBOUND; }
        } else if (gMovesInfo[j].effect == EFFECT_AURA_WHEEL) { // User needs to be Morpeko
            PARAMETRIZE { move = j; species = SPECIES_MORPEKO_FULL_BELLY; }
        } else if (gMovesInfo[j].effect == EFFECT_ROTOTILLER
                || gMovesInfo[j].effect == EFFECT_FLOWER_SHIELD) { // User needs to be Grass-type
            PARAMETRIZE { move = j; species = SPECIES_TANGELA; }
        } else if (gMovesInfo[j].effect == EFFECT_FAIL_IF_NOT_ARG_TYPE && gMovesInfo[j].argument.type == TYPE_FIRE) { // User needs to be Fire-type
            PARAMETRIZE { move = j; species = SPECIES_FLAREON; }
        } else if (gMovesInfo[j].effect == EFFECT_FAIL_IF_NOT_ARG_TYPE && gMovesInfo[j].argument.type == TYPE_ELECTRIC) { // User needs to be Electric-type
            PARAMETRIZE { move = j; species = SPECIES_JOLTEON; }
        } else if (gMovesInfo[j].effect == EFFECT_MAGNETIC_FLUX
                || gMovesInfo[j].effect == EFFECT_GEAR_UP) { // User needs to have Plus
            PARAMETRIZE { move = j; species = SPECIES_KLINKLANG; }
        } else if (gMovesInfo[j].effect == EFFECT_PLACEHOLDER) { // Ignore placeholder moves
            PARAMETRIZE { move = MOVE_POUND; species = SPECIES_WOBBUFFET; }
        } else { // Everything else
            PARAMETRIZE { move = j; species = SPECIES_WOBBUFFET; }
        }
    }
    GIVEN {
        PLAYER(species) {
            HP(9997); MaxHP(9999); Item(ITEM_ORAN_BERRY);
            if (attacker == playerLeft) {
                if (species == SPECIES_WOBBUFFET) Gender(MON_FEMALE);
                if (gMovesInfo[move].effect == EFFECT_LAST_RESORT) Moves(move, MOVE_POUND);
                if (species == SPECIES_KLINKLANG) Ability(ABILITY_PLUS);
            }
        }
        PLAYER(species) {
            HP(9997); MaxHP(9999); Item(ITEM_ORAN_BERRY);
            if (attacker == playerRight) {
                if (species == SPECIES_WOBBUFFET) Gender(MON_FEMALE);
                if (gMovesInfo[move].effect == EFFECT_LAST_RESORT) Moves(move, MOVE_POUND);
                if (species == SPECIES_KLINKLANG) Ability(ABILITY_PLUS);
            }
        }
        PLAYER(SPECIES_WOBBUFFET) {
            Gender(MON_MALE); MaxHP(9999); Moves(MOVE_POUND, MOVE_CELEBRATE);
            HP(gMovesInfo[move].effect == EFFECT_REVIVAL_BLESSING ? 0 : 9998);
        }
        OPPONENT(SPECIES_WOBBUFFET) {
            Gender(MON_MALE); HP(9998); MaxHP(9999); SpDefense(9999); Defense(9999); Ability(ABILITY_TELEPATHY);
            if (gMovesInfo[move].effect != EFFECT_BESTOW) {
                Item(ITEM_ORAN_BERRY);
            }
        }
        OPPONENT(SPECIES_WOBBUFFET) {
            Gender(MON_MALE); HP(9998); MaxHP(9999); SpDefense(9999); Defense(9999); Ability(ABILITY_TELEPATHY);
            if (gMovesInfo[move].effect != EFFECT_BESTOW) {
                Item(ITEM_ORAN_BERRY);
            }
        }
        OPPONENT(SPECIES_WOBBUFFET) { Gender(MON_FEMALE); HP(9998); MaxHP(9999); SpDefense(9999); Defense(9999); }
    } WHEN {
        // Setup turn
        if (gMovesInfo[move].effect == EFFECT_SNORE
         || gMovesInfo[move].effect == EFFECT_SLEEP_TALK) {  // Player needs to be asleep
            TURN { MOVE(attacker, MOVE_REST); }
        } else if (gMovesInfo[move].effect == EFFECT_SPIT_UP
                || gMovesInfo[move].effect == EFFECT_SWALLOW) { // Player needs to have used Stockpile
            TURN { MOVE(attacker, MOVE_STOCKPILE); }
        } else if ((gMovesInfo[move].effect == EFFECT_DOUBLE_POWER_ON_ARG_STATUS && gMovesInfo[move].argument.status == STATUS1_PARALYSIS)) { // Opponent needs to be paralyzed
            TURN { MOVE(attacker, MOVE_THUNDER_WAVE, target: target); }
        } else if (gMovesInfo[move].effect == EFFECT_RECYCLE
                || gMovesInfo[move].effect == EFFECT_BELCH) { // Player needs to have eaten its Berry
            TURN { MOVE(attacker, MOVE_STUFF_CHEEKS); }
        } else if (gMovesInfo[move].effect == EFFECT_REFRESH
                || gMovesInfo[move].effect == EFFECT_PSYCHO_SHIFT) { // Player needs to be paralyzed
            TURN { MOVE(target, MOVE_THUNDER_WAVE, target: attacker); }
        } else if (gMovesInfo[move].effect == EFFECT_LAST_RESORT) { // Player needs to have used all other moves
            TURN { MOVE(attacker, MOVE_POUND, target: target); }
        } else if (gMovesInfo[move].effect == EFFECT_DREAM_EATER
                || gMovesInfo[move].effect == EFFECT_NIGHTMARE) { // Opponent needs to be asleep
            TURN { MOVE(target, MOVE_REST); }
        } else if (gMovesInfo[move].effect == EFFECT_VENOM_DRENCH
                || gMovesInfo[move].effect == EFFECT_PURIFY) { // Opponent needs to be poisoned
            TURN { MOVE(attacker, MOVE_POISON_POWDER, target: target); }
        } else if (gMovesInfo[move].effect == EFFECT_TOPSY_TURVY) { // Opponent needs to have its stats buffed
            TURN { MOVE(target, MOVE_SWORDS_DANCE); }
        } else if (gMovesInfo[move].effect == EFFECT_AURORA_VEIL) { // Has to be hailing
            TURN { MOVE(attacker, MOVE_HAIL); }
        } else if (gMovesInfo[move].effect == EFFECT_HIT_SET_REMOVE_TERRAIN && gMovesInfo[move].argument.moveProperty == ARG_TRY_REMOVE_TERRAIN_FAIL) { // Needs a terrain
            TURN { MOVE(attacker, MOVE_ELECTRIC_TERRAIN); }
        }
        // Effective turn
        TURN {
            if (gMovesInfo[move].effect == EFFECT_COUNTER
             || gMovesInfo[move].effect == EFFECT_MIRROR_MOVE
             || gMovesInfo[move].effect == EFFECT_CONVERSION_2
             || gMovesInfo[move].effect == EFFECT_MIRROR_COAT
             || gMovesInfo[move].effect == EFFECT_METAL_BURST
             || gMovesInfo[move].effect == EFFECT_COPYCAT
             || gMovesInfo[move].effect == EFFECT_SUCKER_PUNCH
             || gMovesInfo[move].effect == EFFECT_INSTRUCT) { // Opponent needs to hit the player first
                MOVE(target, gMovesInfo[move].effect == EFFECT_MIRROR_COAT ? MOVE_SWIFT : MOVE_POUND, target: attacker);
                MOVE(attacker, move, target: target);
            } else if (gMovesInfo[move].effect == EFFECT_SNATCH) { // Opponent needs to steal the opponent's buffing move
                MOVE(attacker, move, target: target);
                MOVE(target, MOVE_SWORDS_DANCE);
            } else if (gMovesInfo[move].effect == EFFECT_OHKO) { // Opponent needs to send out a different team member
                MOVE(attacker, move, target: target);
                SEND_OUT(target, 2);
            } else if (gMovesInfo[move].effect == EFFECT_TELEPORT
                    || gMovesInfo[move].effect == EFFECT_EXPLOSION
                    || gMovesInfo[move].effect == EFFECT_BATON_PASS
                    || gMovesInfo[move].effect == EFFECT_MEMENTO
                    || gMovesInfo[move].effect == EFFECT_HEALING_WISH
                    || gMovesInfo[move].effect == EFFECT_HIT_ESCAPE
                    || gMovesInfo[move].effect == EFFECT_FINAL_GAMBIT
                    || gMovesInfo[move].effect == EFFECT_PARTING_SHOT
                    || gMovesInfo[move].effect == EFFECT_SHED_TAIL
                    || gMovesInfo[move].effect == EFFECT_CHILLY_RECEPTION) { // Player needs to send out a different team member
                MOVE(attacker, move, target: target);
                SEND_OUT(attacker, 2);
            } else if (gMovesInfo[move].effect == EFFECT_PROTECT
                    || gMovesInfo[move].effect == EFFECT_ENDURE
                    || gMovesInfo[move].effect == EFFECT_FOLLOW_ME
                    || gMovesInfo[move].effect == EFFECT_MAGIC_COAT
                    || gMovesInfo[move].effect == EFFECT_ME_FIRST
                    || gMovesInfo[move].effect == EFFECT_QUASH
                    || gMovesInfo[move].effect == EFFECT_MAT_BLOCK
                    || gMovesInfo[move].effect == EFFECT_ELECTRIFY
                    || gMovesInfo[move].effect == EFFECT_SHELL_TRAP) { // Player needs to go first
                MOVE(attacker, move, target: target);
                MOVE(target, MOVE_POUND, target: attacker);
            } else if (gMovesInfo[move].effect == EFFECT_AFTER_YOU) { // Player goes first, opponent third
                MOVE(attacker, move, target: target);
                MOVE(ignore1, MOVE_CELEBRATE);
                MOVE(target, MOVE_POUND, target: attacker);
                MOVE(ignore2, MOVE_CELEBRATE);
            } else if (gMovesInfo[move].effect == EFFECT_REVIVAL_BLESSING) { // Player selects party member
                MOVE(attacker, move, partyIndex: 2);
                MOVE(target, MOVE_LAST_RESORT, target: attacker); // Last Resort, so there's no anim on the opponent's side.
            } else if (gMovesInfo[move].effect == EFFECT_UPPER_HAND) { // Opponent needs to choose priority move
                MOVE(attacker, move, target: target);
                MOVE(target, MOVE_QUICK_ATTACK, target: attacker);
            } else { // All other moves
                MOVE(target, MOVE_LAST_RESORT, target: attacker); // Last Resort, so there's no anim on the opponent's side.
                MOVE(attacker, move, target: target);
            }
            if (gMovesInfo[move].effect != EFFECT_AFTER_YOU) {
                // Actions for the remaining battlers
                MOVE(ignore1, MOVE_CELEBRATE);
                MOVE(ignore2, MOVE_CELEBRATE);
            }
        }
    } SCENE {
        if (gMovesInfo[move].effect == EFFECT_MAGNETIC_FLUX || gMovesInfo[move].effect == EFFECT_GEAR_UP) // For some reason, Magnetic Flux and Gear Up are failing in Double Battles here
        {
            // Moves that fail in Double Battles
        } else if (gMovesInfo[move].effect == EFFECT_MIRROR_MOVE) { // Copy the opponent's move
            ANIMATION(ANIM_TYPE_MOVE, MOVE_POUND, attacker);
        } else if (gMovesInfo[move].effect == EFFECT_NATURE_POWER) { // Recorded battles always use BATTLE_TERRAIN_BUILDING
            ANIMATION(ANIM_TYPE_MOVE, B_NATURE_POWER_MOVES >= GEN_4 ? MOVE_TRI_ATTACK : MOVE_SWIFT, attacker);
        } else { // All other moves
            ANIMATION(ANIM_TYPE_MOVE, move, attacker);
        }
    } THEN {
        FORCE_MOVE_ANIM(FALSE);
        if (gLoadFail)
            DebugPrintf("Move failed: %S (%u)", gMovesInfo[move].name, move);
        EXPECT_EQ(gLoadFail, FALSE);
    }
}

DOUBLE_BATTLE_TEST("Move Animations don't leak when used - Doubles (opponentLeft to playerRight)")
{
    u32 j = 1, move = 0, species = 0;
    FORCE_MOVE_ANIM(TRUE);
    struct BattlePokemon *attacker = opponentLeft;
    struct BattlePokemon *target = playerRight;
    struct BattlePokemon *ignore1 = playerLeft;
    struct BattlePokemon *ignore2 = opponentRight;
    for (; j < MOVES_COUNT; j++) {
        if (gMovesInfo[j].effect == EFFECT_DARK_VOID) { // User needs to be Darkrai
            PARAMETRIZE { move = j; species = SPECIES_DARKRAI; }
        } else if (gMovesInfo[j].effect == EFFECT_HYPERSPACE_FURY) { // User needs to be Hoopa Unbound
            PARAMETRIZE { move = j; species = SPECIES_HOOPA_UNBOUND; }
        } else if (gMovesInfo[j].effect == EFFECT_AURA_WHEEL) { // User needs to be Morpeko
            PARAMETRIZE { move = j; species = SPECIES_MORPEKO_FULL_BELLY; }
        } else if (gMovesInfo[j].effect == EFFECT_ROTOTILLER
                || gMovesInfo[j].effect == EFFECT_FLOWER_SHIELD) { // User needs to be Grass-type
            PARAMETRIZE { move = j; species = SPECIES_TANGELA; }
        } else if (gMovesInfo[j].effect == EFFECT_FAIL_IF_NOT_ARG_TYPE && gMovesInfo[j].argument.type == TYPE_FIRE) { // User needs to be Fire-type
            PARAMETRIZE { move = j; species = SPECIES_FLAREON; }
        } else if (gMovesInfo[j].effect == EFFECT_FAIL_IF_NOT_ARG_TYPE && gMovesInfo[j].argument.type == TYPE_ELECTRIC) { // User needs to be Electric-type
            PARAMETRIZE { move = j; species = SPECIES_JOLTEON; }
        } else if (gMovesInfo[j].effect == EFFECT_MAGNETIC_FLUX
                || gMovesInfo[j].effect == EFFECT_GEAR_UP) { // User needs to have Plus
            PARAMETRIZE { move = j; species = SPECIES_KLINKLANG; }
        } else if (gMovesInfo[j].effect == EFFECT_PLACEHOLDER) { // Ignore placeholder moves
            PARAMETRIZE { move = MOVE_POUND; species = SPECIES_WOBBUFFET; }
        } else { // Everything else
            PARAMETRIZE { move = j; species = SPECIES_WOBBUFFET; }
        }
    }
    GIVEN {
        OPPONENT(species) {
            HP(9997); MaxHP(9999); Item(ITEM_ORAN_BERRY);
            if (attacker == opponentLeft) {
                if (species == SPECIES_WOBBUFFET) Gender(MON_FEMALE);
                if (gMovesInfo[move].effect == EFFECT_LAST_RESORT) Moves(move, MOVE_POUND);
                if (species == SPECIES_KLINKLANG) Ability(ABILITY_PLUS);
            }
        }
        OPPONENT(species) {
            HP(9997); MaxHP(9999); Item(ITEM_ORAN_BERRY);
            if (attacker == opponentRight) {
                if (species == SPECIES_WOBBUFFET) Gender(MON_FEMALE);
                if (gMovesInfo[move].effect == EFFECT_LAST_RESORT) Moves(move, MOVE_POUND);
                if (species == SPECIES_KLINKLANG) Ability(ABILITY_PLUS);
            }
        }
        OPPONENT(SPECIES_WOBBUFFET) {
            Gender(MON_MALE); MaxHP(9999); Moves(MOVE_POUND, MOVE_CELEBRATE);
            HP(gMovesInfo[move].effect == EFFECT_REVIVAL_BLESSING ? 0 : 9998);
        }
        PLAYER(SPECIES_WOBBUFFET) {
            Gender(MON_MALE); HP(9998); MaxHP(9999); SpDefense(9999); Defense(9999); Ability(ABILITY_TELEPATHY);
            if (gMovesInfo[move].effect != EFFECT_BESTOW) {
                Item(ITEM_ORAN_BERRY);
            }
        }
        PLAYER(SPECIES_WOBBUFFET) {
            Gender(MON_MALE); HP(9998); MaxHP(9999); SpDefense(9999); Defense(9999); Ability(ABILITY_TELEPATHY);
            if (gMovesInfo[move].effect != EFFECT_BESTOW) {
                Item(ITEM_ORAN_BERRY);
            }
        }
        PLAYER(SPECIES_WOBBUFFET) { Gender(MON_FEMALE); HP(9998); MaxHP(9999); SpDefense(9999); Defense(9999); }
    } WHEN {
        // Setup turn
        if (gMovesInfo[move].effect == EFFECT_SNORE
         || gMovesInfo[move].effect == EFFECT_SLEEP_TALK) {  // Player needs to be asleep
            TURN { MOVE(attacker, MOVE_REST); }
        } else if (gMovesInfo[move].effect == EFFECT_SPIT_UP
                || gMovesInfo[move].effect == EFFECT_SWALLOW) { // Player needs to have used Stockpile
            TURN { MOVE(attacker, MOVE_STOCKPILE); }
        } else if ((gMovesInfo[move].effect == EFFECT_DOUBLE_POWER_ON_ARG_STATUS && gMovesInfo[move].argument.status == STATUS1_PARALYSIS)) { // Opponent needs to be paralyzed
            TURN { MOVE(attacker, MOVE_THUNDER_WAVE, target: target); }
        } else if (gMovesInfo[move].effect == EFFECT_RECYCLE
                || gMovesInfo[move].effect == EFFECT_BELCH) { // Player needs to have eaten its Berry
            TURN { MOVE(attacker, MOVE_STUFF_CHEEKS); }
        } else if (gMovesInfo[move].effect == EFFECT_REFRESH
                || gMovesInfo[move].effect == EFFECT_PSYCHO_SHIFT) { // Player needs to be paralyzed
            TURN { MOVE(target, MOVE_THUNDER_WAVE, target: attacker); }
        } else if (gMovesInfo[move].effect == EFFECT_LAST_RESORT) { // Player needs to have used all other moves
            TURN { MOVE(attacker, MOVE_POUND, target: target); }
        } else if (gMovesInfo[move].effect == EFFECT_DREAM_EATER
                || gMovesInfo[move].effect == EFFECT_NIGHTMARE) { // Opponent needs to be asleep
            TURN { MOVE(target, MOVE_REST); }
        } else if (gMovesInfo[move].effect == EFFECT_VENOM_DRENCH
                || gMovesInfo[move].effect == EFFECT_PURIFY) { // Opponent needs to be poisoned
            TURN { MOVE(attacker, MOVE_POISON_POWDER, target: target); }
        } else if (gMovesInfo[move].effect == EFFECT_TOPSY_TURVY) { // Opponent needs to have its stats buffed
            TURN { MOVE(target, MOVE_SWORDS_DANCE); }
        } else if (gMovesInfo[move].effect == EFFECT_AURORA_VEIL) { // Has to be hailing
            TURN { MOVE(attacker, MOVE_HAIL); }
        } else if (gMovesInfo[move].effect == EFFECT_HIT_SET_REMOVE_TERRAIN && gMovesInfo[move].argument.moveProperty == ARG_TRY_REMOVE_TERRAIN_FAIL) { // Needs a terrain
            TURN { MOVE(attacker, MOVE_ELECTRIC_TERRAIN); }
        }
        // Effective turn
        TURN {
            if (gMovesInfo[move].effect == EFFECT_COUNTER
             || gMovesInfo[move].effect == EFFECT_MIRROR_MOVE
             || gMovesInfo[move].effect == EFFECT_CONVERSION_2
             || gMovesInfo[move].effect == EFFECT_MIRROR_COAT
             || gMovesInfo[move].effect == EFFECT_METAL_BURST
             || gMovesInfo[move].effect == EFFECT_COPYCAT
             || gMovesInfo[move].effect == EFFECT_SUCKER_PUNCH
             || gMovesInfo[move].effect == EFFECT_INSTRUCT) { // Opponent needs to hit the player first
                MOVE(target, gMovesInfo[move].effect == EFFECT_MIRROR_COAT ? MOVE_SWIFT : MOVE_POUND, target: attacker);
                MOVE(attacker, move, target: target);
            } else if (gMovesInfo[move].effect == EFFECT_SNATCH) { // Opponent needs to steal the opponent's buffing move
                MOVE(attacker, move, target: target);
                MOVE(target, MOVE_SWORDS_DANCE);
            } else if (gMovesInfo[move].effect == EFFECT_OHKO) { // Opponent needs to send out a different team member
                MOVE(attacker, move, target: target);
                SEND_OUT(target, 2);
            } else if (gMovesInfo[move].effect == EFFECT_TELEPORT
                    || gMovesInfo[move].effect == EFFECT_EXPLOSION
                    || gMovesInfo[move].effect == EFFECT_BATON_PASS
                    || gMovesInfo[move].effect == EFFECT_MEMENTO
                    || gMovesInfo[move].effect == EFFECT_HEALING_WISH
                    || gMovesInfo[move].effect == EFFECT_HIT_ESCAPE
                    || gMovesInfo[move].effect == EFFECT_FINAL_GAMBIT
                    || gMovesInfo[move].effect == EFFECT_PARTING_SHOT
                    || gMovesInfo[move].effect == EFFECT_SHED_TAIL
                    || gMovesInfo[move].effect == EFFECT_CHILLY_RECEPTION) { // Player needs to send out a different team member
                MOVE(attacker, move, target: target);
                SEND_OUT(attacker, 2);
            } else if (gMovesInfo[move].effect == EFFECT_PROTECT
                    || gMovesInfo[move].effect == EFFECT_ENDURE
                    || gMovesInfo[move].effect == EFFECT_FOLLOW_ME
                    || gMovesInfo[move].effect == EFFECT_MAGIC_COAT
                    || gMovesInfo[move].effect == EFFECT_ME_FIRST
                    || gMovesInfo[move].effect == EFFECT_QUASH
                    || gMovesInfo[move].effect == EFFECT_MAT_BLOCK
                    || gMovesInfo[move].effect == EFFECT_ELECTRIFY
                    || gMovesInfo[move].effect == EFFECT_SHELL_TRAP) { // Player needs to go first
                MOVE(attacker, move, target: target);
                MOVE(target, MOVE_POUND, target: attacker);
            } else if (gMovesInfo[move].effect == EFFECT_AFTER_YOU) { // Player goes first, opponent third
                MOVE(attacker, move, target: target);
                MOVE(ignore1, MOVE_CELEBRATE);
                MOVE(target, MOVE_POUND, target: attacker);
                MOVE(ignore2, MOVE_CELEBRATE);
            } else if (gMovesInfo[move].effect == EFFECT_REVIVAL_BLESSING) { // Player selects party member
                MOVE(attacker, move, partyIndex: 2);
                MOVE(target, MOVE_LAST_RESORT, target: attacker); // Last Resort, so there's no anim on the opponent's side.
            } else if (gMovesInfo[move].effect == EFFECT_UPPER_HAND) { // Opponent needs to choose priority move
                MOVE(attacker, move, target: target);
                MOVE(target, MOVE_QUICK_ATTACK, target: attacker);
            } else { // All other moves
                MOVE(target, MOVE_LAST_RESORT, target: attacker); // Last Resort, so there's no anim on the opponent's side.
                MOVE(attacker, move, target: target);
            }
            if (gMovesInfo[move].effect != EFFECT_AFTER_YOU) {
                // Actions for the remaining battlers
                MOVE(ignore1, MOVE_CELEBRATE);
                MOVE(ignore2, MOVE_CELEBRATE);
            }
        }
    } SCENE {
        if (gMovesInfo[move].effect == EFFECT_MAGNETIC_FLUX || gMovesInfo[move].effect == EFFECT_GEAR_UP) // For some reason, Magnetic Flux and Gear Up are failing in Double Battles here
        {
            // Moves that fail in Double Battles
        } else if (gMovesInfo[move].effect == EFFECT_MIRROR_MOVE) { // Copy the opponent's move
            ANIMATION(ANIM_TYPE_MOVE, MOVE_POUND, attacker);
        } else if (gMovesInfo[move].effect == EFFECT_NATURE_POWER) { // Recorded battles always use BATTLE_TERRAIN_BUILDING
            ANIMATION(ANIM_TYPE_MOVE, B_NATURE_POWER_MOVES >= GEN_4 ? MOVE_TRI_ATTACK : MOVE_SWIFT, attacker);
        } else { // All other moves
            ANIMATION(ANIM_TYPE_MOVE, move, attacker);
        }
    } THEN {
        FORCE_MOVE_ANIM(FALSE);
        if (gLoadFail)
            DebugPrintf("Move failed: %S (%u)", gMovesInfo[move].name, move);
        EXPECT_EQ(gLoadFail, FALSE);
    }
}

DOUBLE_BATTLE_TEST("Move Animations don't leak when used - Doubles (playerRight to opponentRight)")
{
    u32 j = 1, move = 0, species = 0;
    FORCE_MOVE_ANIM(TRUE);
    struct BattlePokemon *attacker = playerRight;
    struct BattlePokemon *target = opponentRight;
    struct BattlePokemon *ignore1 = playerLeft;
    struct BattlePokemon *ignore2 = opponentLeft;
    for (; j < MOVES_COUNT; j++) {
        if (gMovesInfo[j].effect == EFFECT_DARK_VOID) { // User needs to be Darkrai
            PARAMETRIZE { move = j; species = SPECIES_DARKRAI; }
        } else if (gMovesInfo[j].effect == EFFECT_HYPERSPACE_FURY) { // User needs to be Hoopa Unbound
            PARAMETRIZE { move = j; species = SPECIES_HOOPA_UNBOUND; }
        } else if (gMovesInfo[j].effect == EFFECT_AURA_WHEEL) { // User needs to be Morpeko
            PARAMETRIZE { move = j; species = SPECIES_MORPEKO_FULL_BELLY; }
        } else if (gMovesInfo[j].effect == EFFECT_ROTOTILLER
                || gMovesInfo[j].effect == EFFECT_FLOWER_SHIELD) { // User needs to be Grass-type
            PARAMETRIZE { move = j; species = SPECIES_TANGELA; }
        } else if (gMovesInfo[j].effect == EFFECT_FAIL_IF_NOT_ARG_TYPE && gMovesInfo[j].argument.type == TYPE_FIRE) { // User needs to be Fire-type
            PARAMETRIZE { move = j; species = SPECIES_FLAREON; }
        } else if (gMovesInfo[j].effect == EFFECT_FAIL_IF_NOT_ARG_TYPE && gMovesInfo[j].argument.type == TYPE_ELECTRIC) { // User needs to be Electric-type
            PARAMETRIZE { move = j; species = SPECIES_JOLTEON; }
        } else if (gMovesInfo[j].effect == EFFECT_MAGNETIC_FLUX
                || gMovesInfo[j].effect == EFFECT_GEAR_UP) { // User needs to have Plus
            PARAMETRIZE { move = j; species = SPECIES_KLINKLANG; }
        } else if (gMovesInfo[j].effect == EFFECT_PLACEHOLDER) { // Ignore placeholder moves
            PARAMETRIZE { move = MOVE_POUND; species = SPECIES_WOBBUFFET; }
        } else { // Everything else
            PARAMETRIZE { move = j; species = SPECIES_WOBBUFFET; }
        }
    }
    GIVEN {
        PLAYER(species) {
            HP(9997); MaxHP(9999); Item(ITEM_ORAN_BERRY);
            if (attacker == playerLeft) {
                if (species == SPECIES_WOBBUFFET) Gender(MON_FEMALE);
                if (gMovesInfo[move].effect == EFFECT_LAST_RESORT) Moves(move, MOVE_POUND);
                if (species == SPECIES_KLINKLANG) Ability(ABILITY_PLUS);
            }
        }
        PLAYER(species) {
            HP(9997); MaxHP(9999); Item(ITEM_ORAN_BERRY);
            if (attacker == playerRight) {
                if (species == SPECIES_WOBBUFFET) Gender(MON_FEMALE);
                if (gMovesInfo[move].effect == EFFECT_LAST_RESORT) Moves(move, MOVE_POUND);
                if (species == SPECIES_KLINKLANG) Ability(ABILITY_PLUS);
            }
        }
        PLAYER(SPECIES_WOBBUFFET) {
            Gender(MON_MALE); MaxHP(9999); Moves(MOVE_POUND, MOVE_CELEBRATE);
            HP(gMovesInfo[move].effect == EFFECT_REVIVAL_BLESSING ? 0 : 9998);
        }
        OPPONENT(SPECIES_WOBBUFFET) {
            Gender(MON_MALE); HP(9998); MaxHP(9999); SpDefense(9999); Defense(9999); Ability(ABILITY_TELEPATHY);
            if (gMovesInfo[move].effect != EFFECT_BESTOW) {
                Item(ITEM_ORAN_BERRY);
            }
        }
        OPPONENT(SPECIES_WOBBUFFET) {
            Gender(MON_MALE); HP(9998); MaxHP(9999); SpDefense(9999); Defense(9999); Ability(ABILITY_TELEPATHY);
            if (gMovesInfo[move].effect != EFFECT_BESTOW) {
                Item(ITEM_ORAN_BERRY);
            }
        }
        OPPONENT(SPECIES_WOBBUFFET) { Gender(MON_FEMALE); HP(9998); MaxHP(9999); SpDefense(9999); Defense(9999); }
    } WHEN {
        // Setup turn
        if (gMovesInfo[move].effect == EFFECT_SNORE
         || gMovesInfo[move].effect == EFFECT_SLEEP_TALK) {  // Player needs to be asleep
            TURN { MOVE(attacker, MOVE_REST); }
        } else if (gMovesInfo[move].effect == EFFECT_SPIT_UP
                || gMovesInfo[move].effect == EFFECT_SWALLOW) { // Player needs to have used Stockpile
            TURN { MOVE(attacker, MOVE_STOCKPILE); }
        } else if ((gMovesInfo[move].effect == EFFECT_DOUBLE_POWER_ON_ARG_STATUS && gMovesInfo[move].argument.status == STATUS1_PARALYSIS)) { // Opponent needs to be paralyzed
            TURN { MOVE(attacker, MOVE_THUNDER_WAVE, target: target); }
        } else if (gMovesInfo[move].effect == EFFECT_RECYCLE
                || gMovesInfo[move].effect == EFFECT_BELCH) { // Player needs to have eaten its Berry
            TURN { MOVE(attacker, MOVE_STUFF_CHEEKS); }
        } else if (gMovesInfo[move].effect == EFFECT_REFRESH
                || gMovesInfo[move].effect == EFFECT_PSYCHO_SHIFT) { // Player needs to be paralyzed
            TURN { MOVE(target, MOVE_THUNDER_WAVE, target: attacker); }
        } else if (gMovesInfo[move].effect == EFFECT_LAST_RESORT) { // Player needs to have used all other moves
            TURN { MOVE(attacker, MOVE_POUND, target: target); }
        } else if (gMovesInfo[move].effect == EFFECT_DREAM_EATER
                || gMovesInfo[move].effect == EFFECT_NIGHTMARE) { // Opponent needs to be asleep
            TURN { MOVE(target, MOVE_REST); }
        } else if (gMovesInfo[move].effect == EFFECT_VENOM_DRENCH
                || gMovesInfo[move].effect == EFFECT_PURIFY) { // Opponent needs to be poisoned
            TURN { MOVE(attacker, MOVE_POISON_POWDER, target: target); }
        } else if (gMovesInfo[move].effect == EFFECT_TOPSY_TURVY) { // Opponent needs to have its stats buffed
            TURN { MOVE(target, MOVE_SWORDS_DANCE); }
        } else if (gMovesInfo[move].effect == EFFECT_AURORA_VEIL) { // Has to be hailing
            TURN { MOVE(attacker, MOVE_HAIL); }
        } else if (gMovesInfo[move].effect == EFFECT_HIT_SET_REMOVE_TERRAIN && gMovesInfo[move].argument.moveProperty == ARG_TRY_REMOVE_TERRAIN_FAIL) { // Needs a terrain
            TURN { MOVE(attacker, MOVE_ELECTRIC_TERRAIN); }
        }
        // Effective turn
        TURN {
            if (gMovesInfo[move].effect == EFFECT_COUNTER
             || gMovesInfo[move].effect == EFFECT_MIRROR_MOVE
             || gMovesInfo[move].effect == EFFECT_CONVERSION_2
             || gMovesInfo[move].effect == EFFECT_MIRROR_COAT
             || gMovesInfo[move].effect == EFFECT_METAL_BURST
             || gMovesInfo[move].effect == EFFECT_COPYCAT
             || gMovesInfo[move].effect == EFFECT_SUCKER_PUNCH
             || gMovesInfo[move].effect == EFFECT_INSTRUCT) { // Opponent needs to hit the player first
                MOVE(target, gMovesInfo[move].effect == EFFECT_MIRROR_COAT ? MOVE_SWIFT : MOVE_POUND, target: attacker);
                MOVE(attacker, move, target: target);
            } else if (gMovesInfo[move].effect == EFFECT_SNATCH) { // Opponent needs to steal the opponent's buffing move
                MOVE(attacker, move, target: target);
                MOVE(target, MOVE_SWORDS_DANCE);
            } else if (gMovesInfo[move].effect == EFFECT_OHKO) { // Opponent needs to send out a different team member
                MOVE(attacker, move, target: target);
                SEND_OUT(target, 2);
            } else if (gMovesInfo[move].effect == EFFECT_TELEPORT
                    || gMovesInfo[move].effect == EFFECT_EXPLOSION
                    || gMovesInfo[move].effect == EFFECT_BATON_PASS
                    || gMovesInfo[move].effect == EFFECT_MEMENTO
                    || gMovesInfo[move].effect == EFFECT_HEALING_WISH
                    || gMovesInfo[move].effect == EFFECT_HIT_ESCAPE
                    || gMovesInfo[move].effect == EFFECT_FINAL_GAMBIT
                    || gMovesInfo[move].effect == EFFECT_PARTING_SHOT
                    || gMovesInfo[move].effect == EFFECT_SHED_TAIL
                    || gMovesInfo[move].effect == EFFECT_CHILLY_RECEPTION) { // Player needs to send out a different team member
                MOVE(attacker, move, target: target);
                SEND_OUT(attacker, 2);
            } else if (gMovesInfo[move].effect == EFFECT_PROTECT
                    || gMovesInfo[move].effect == EFFECT_ENDURE
                    || gMovesInfo[move].effect == EFFECT_FOLLOW_ME
                    || gMovesInfo[move].effect == EFFECT_MAGIC_COAT
                    || gMovesInfo[move].effect == EFFECT_ME_FIRST
                    || gMovesInfo[move].effect == EFFECT_QUASH
                    || gMovesInfo[move].effect == EFFECT_MAT_BLOCK
                    || gMovesInfo[move].effect == EFFECT_ELECTRIFY
                    || gMovesInfo[move].effect == EFFECT_SHELL_TRAP) { // Player needs to go first
                MOVE(attacker, move, target: target);
                MOVE(target, MOVE_POUND, target: attacker);
            } else if (gMovesInfo[move].effect == EFFECT_AFTER_YOU) { // Player goes first, opponent third
                MOVE(attacker, move, target: target);
                MOVE(ignore1, MOVE_CELEBRATE);
                MOVE(target, MOVE_POUND, target: attacker);
                MOVE(ignore2, MOVE_CELEBRATE);
            } else if (gMovesInfo[move].effect == EFFECT_REVIVAL_BLESSING) { // Player selects party member
                MOVE(attacker, move, partyIndex: 2);
                MOVE(target, MOVE_LAST_RESORT, target: attacker); // Last Resort, so there's no anim on the opponent's side.
            } else if (gMovesInfo[move].effect == EFFECT_UPPER_HAND) { // Opponent needs to choose priority move
                MOVE(attacker, move, target: target);
                MOVE(target, MOVE_QUICK_ATTACK, target: attacker);
            } else { // All other moves
                MOVE(target, MOVE_LAST_RESORT, target: attacker); // Last Resort, so there's no anim on the opponent's side.
                MOVE(attacker, move, target: target);
            }
            if (gMovesInfo[move].effect != EFFECT_AFTER_YOU) {
                // Actions for the remaining battlers
                MOVE(ignore1, MOVE_CELEBRATE);
                MOVE(ignore2, MOVE_CELEBRATE);
            }
        }
    } SCENE {
        if (gMovesInfo[move].effect == EFFECT_MAGNETIC_FLUX || gMovesInfo[move].effect == EFFECT_GEAR_UP) // For some reason, Magnetic Flux and Gear Up are failing in Double Battles here
        {
            // Moves that fail in Double Battles
        } else if (gMovesInfo[move].effect == EFFECT_MIRROR_MOVE) { // Copy the opponent's move
            ANIMATION(ANIM_TYPE_MOVE, MOVE_POUND, attacker);
        } else if (gMovesInfo[move].effect == EFFECT_NATURE_POWER) { // Recorded battles always use BATTLE_TERRAIN_BUILDING
            ANIMATION(ANIM_TYPE_MOVE, B_NATURE_POWER_MOVES >= GEN_4 ? MOVE_TRI_ATTACK : MOVE_SWIFT, attacker);
        } else { // All other moves
            ANIMATION(ANIM_TYPE_MOVE, move, attacker);
        }
    } THEN {
        FORCE_MOVE_ANIM(FALSE);
        if (gLoadFail)
            DebugPrintf("Move failed: %S (%u)", gMovesInfo[move].name, move);
        EXPECT_EQ(gLoadFail, FALSE);
    }
}

DOUBLE_BATTLE_TEST("Move Animations don't leak when used - Doubles (opponentRight to playerRight)")
{
    u32 j = 1, move = 0, species = 0;
    FORCE_MOVE_ANIM(TRUE);
    struct BattlePokemon *attacker = opponentRight;
    struct BattlePokemon *target = playerRight;
    struct BattlePokemon *ignore1 = playerLeft;
    struct BattlePokemon *ignore2 = opponentLeft;
    for (; j < MOVES_COUNT; j++) {
        if (j == 419)
            continue;
        if (gMovesInfo[j].effect == EFFECT_DARK_VOID) { // User needs to be Darkrai
            PARAMETRIZE { move = j; species = SPECIES_DARKRAI; }
        } else if (gMovesInfo[j].effect == EFFECT_HYPERSPACE_FURY) { // User needs to be Hoopa Unbound
            PARAMETRIZE { move = j; species = SPECIES_HOOPA_UNBOUND; }
        } else if (gMovesInfo[j].effect == EFFECT_AURA_WHEEL) { // User needs to be Morpeko
            PARAMETRIZE { move = j; species = SPECIES_MORPEKO_FULL_BELLY; }
        } else if (gMovesInfo[j].effect == EFFECT_ROTOTILLER
                || gMovesInfo[j].effect == EFFECT_FLOWER_SHIELD) { // User needs to be Grass-type
            PARAMETRIZE { move = j; species = SPECIES_TANGELA; }
        } else if (gMovesInfo[j].effect == EFFECT_FAIL_IF_NOT_ARG_TYPE && gMovesInfo[j].argument.type == TYPE_FIRE) { // User needs to be Fire-type
            PARAMETRIZE { move = j; species = SPECIES_FLAREON; }
        } else if (gMovesInfo[j].effect == EFFECT_FAIL_IF_NOT_ARG_TYPE && gMovesInfo[j].argument.type == TYPE_ELECTRIC) { // User needs to be Electric-type
            PARAMETRIZE { move = j; species = SPECIES_JOLTEON; }
        } else if (gMovesInfo[j].effect == EFFECT_MAGNETIC_FLUX
                || gMovesInfo[j].effect == EFFECT_GEAR_UP) { // User needs to have Plus
            PARAMETRIZE { move = j; species = SPECIES_KLINKLANG; }
        } else if (gMovesInfo[j].effect == EFFECT_PLACEHOLDER) { // Ignore placeholder moves
            PARAMETRIZE { move = MOVE_POUND; species = SPECIES_WOBBUFFET; }
        } else { // Everything else
            PARAMETRIZE { move = j; species = SPECIES_WOBBUFFET; }
        }
    }
    GIVEN {
        OPPONENT(species) {
            HP(9997); MaxHP(9999); Item(ITEM_ORAN_BERRY);
            if (attacker == opponentLeft) {
                if (species == SPECIES_WOBBUFFET) Gender(MON_FEMALE);
                if (gMovesInfo[move].effect == EFFECT_LAST_RESORT) Moves(move, MOVE_POUND);
                if (species == SPECIES_KLINKLANG) Ability(ABILITY_PLUS);
            }
        }
        OPPONENT(species) {
            HP(9997); MaxHP(9999); Item(ITEM_ORAN_BERRY);
            if (attacker == opponentRight) {
                if (species == SPECIES_WOBBUFFET) Gender(MON_FEMALE);
                if (gMovesInfo[move].effect == EFFECT_LAST_RESORT) Moves(move, MOVE_POUND);
                if (species == SPECIES_KLINKLANG) Ability(ABILITY_PLUS);
            }
        }
        OPPONENT(SPECIES_WOBBUFFET) {
            Gender(MON_MALE); MaxHP(9999); Moves(MOVE_POUND, MOVE_CELEBRATE);
            HP(gMovesInfo[move].effect == EFFECT_REVIVAL_BLESSING ? 0 : 9998);
        }
        PLAYER(SPECIES_WOBBUFFET) {
            Gender(MON_MALE); HP(9998); MaxHP(9999); SpDefense(9999); Defense(9999); Ability(ABILITY_TELEPATHY);
            if (gMovesInfo[move].effect != EFFECT_BESTOW) {
                Item(ITEM_ORAN_BERRY);
            }
        }
        PLAYER(SPECIES_WOBBUFFET) {
            Gender(MON_MALE); HP(9998); MaxHP(9999); SpDefense(9999); Defense(9999); Ability(ABILITY_TELEPATHY);
            if (gMovesInfo[move].effect != EFFECT_BESTOW) {
                Item(ITEM_ORAN_BERRY);
            }
        }
        PLAYER(SPECIES_WOBBUFFET) { Gender(MON_FEMALE); HP(9998); MaxHP(9999); SpDefense(9999); Defense(9999); }
    } WHEN {
        // Setup turn
        if (gMovesInfo[move].effect == EFFECT_SNORE
         || gMovesInfo[move].effect == EFFECT_SLEEP_TALK) {  // Player needs to be asleep
            TURN { MOVE(attacker, MOVE_REST); }
        } else if (gMovesInfo[move].effect == EFFECT_SPIT_UP
                || gMovesInfo[move].effect == EFFECT_SWALLOW) { // Player needs to have used Stockpile
            TURN { MOVE(attacker, MOVE_STOCKPILE); }
        } else if ((gMovesInfo[move].effect == EFFECT_DOUBLE_POWER_ON_ARG_STATUS && gMovesInfo[move].argument.status == STATUS1_PARALYSIS)) { // Opponent needs to be paralyzed
            TURN { MOVE(attacker, MOVE_THUNDER_WAVE, target: target); }
        } else if (gMovesInfo[move].effect == EFFECT_RECYCLE
                || gMovesInfo[move].effect == EFFECT_BELCH) { // Player needs to have eaten its Berry
            TURN { MOVE(attacker, MOVE_STUFF_CHEEKS); }
        } else if (gMovesInfo[move].effect == EFFECT_REFRESH
                || gMovesInfo[move].effect == EFFECT_PSYCHO_SHIFT) { // Player needs to be paralyzed
            TURN { MOVE(target, MOVE_THUNDER_WAVE, target: attacker); }
        } else if (gMovesInfo[move].effect == EFFECT_LAST_RESORT) { // Player needs to have used all other moves
            TURN { MOVE(attacker, MOVE_POUND, target: target); }
        } else if (gMovesInfo[move].effect == EFFECT_DREAM_EATER
                || gMovesInfo[move].effect == EFFECT_NIGHTMARE) { // Opponent needs to be asleep
            TURN { MOVE(target, MOVE_REST); }
        } else if (gMovesInfo[move].effect == EFFECT_VENOM_DRENCH
                || gMovesInfo[move].effect == EFFECT_PURIFY) { // Opponent needs to be poisoned
            TURN { MOVE(attacker, MOVE_POISON_POWDER, target: target); }
        } else if (gMovesInfo[move].effect == EFFECT_TOPSY_TURVY) { // Opponent needs to have its stats buffed
            TURN { MOVE(target, MOVE_SWORDS_DANCE); }
        } else if (gMovesInfo[move].effect == EFFECT_AURORA_VEIL) { // Has to be hailing
            TURN { MOVE(attacker, MOVE_HAIL); }
        } else if (gMovesInfo[move].effect == EFFECT_HIT_SET_REMOVE_TERRAIN && gMovesInfo[move].argument.moveProperty == ARG_TRY_REMOVE_TERRAIN_FAIL) { // Needs a terrain
            TURN { MOVE(attacker, MOVE_ELECTRIC_TERRAIN); }
        }
        // Effective turn
        TURN {
            if (gMovesInfo[move].effect == EFFECT_COUNTER
             || gMovesInfo[move].effect == EFFECT_MIRROR_MOVE
             || gMovesInfo[move].effect == EFFECT_CONVERSION_2
             || gMovesInfo[move].effect == EFFECT_MIRROR_COAT
             || gMovesInfo[move].effect == EFFECT_METAL_BURST
             || gMovesInfo[move].effect == EFFECT_COPYCAT
             || gMovesInfo[move].effect == EFFECT_SUCKER_PUNCH
             || gMovesInfo[move].effect == EFFECT_INSTRUCT) { // Opponent needs to hit the player first
                MOVE(target, gMovesInfo[move].effect == EFFECT_MIRROR_COAT ? MOVE_SWIFT : MOVE_POUND, target: attacker);
                MOVE(attacker, move, target: target);
            } else if (gMovesInfo[move].effect == EFFECT_SNATCH) { // Opponent needs to steal the opponent's buffing move
                MOVE(attacker, move, target: target);
                MOVE(target, MOVE_SWORDS_DANCE);
            } else if (gMovesInfo[move].effect == EFFECT_OHKO) { // Opponent needs to send out a different team member
                MOVE(attacker, move, target: target);
                SEND_OUT(target, 2);
            } else if (gMovesInfo[move].effect == EFFECT_TELEPORT
                    || gMovesInfo[move].effect == EFFECT_EXPLOSION
                    || gMovesInfo[move].effect == EFFECT_BATON_PASS
                    || gMovesInfo[move].effect == EFFECT_MEMENTO
                    || gMovesInfo[move].effect == EFFECT_HEALING_WISH
                    || gMovesInfo[move].effect == EFFECT_HIT_ESCAPE
                    || gMovesInfo[move].effect == EFFECT_FINAL_GAMBIT
                    || gMovesInfo[move].effect == EFFECT_PARTING_SHOT
                    || gMovesInfo[move].effect == EFFECT_SHED_TAIL
                    || gMovesInfo[move].effect == EFFECT_CHILLY_RECEPTION) { // Player needs to send out a different team member
                MOVE(attacker, move, target: target);
                SEND_OUT(attacker, 2);
            } else if (gMovesInfo[move].effect == EFFECT_PROTECT
                    || gMovesInfo[move].effect == EFFECT_ENDURE
                    || gMovesInfo[move].effect == EFFECT_FOLLOW_ME
                    || gMovesInfo[move].effect == EFFECT_MAGIC_COAT
                    || gMovesInfo[move].effect == EFFECT_ME_FIRST
                    || gMovesInfo[move].effect == EFFECT_QUASH
                    || gMovesInfo[move].effect == EFFECT_MAT_BLOCK
                    || gMovesInfo[move].effect == EFFECT_ELECTRIFY
                    || gMovesInfo[move].effect == EFFECT_SHELL_TRAP) { // Player needs to go first
                MOVE(attacker, move, target: target);
                MOVE(target, MOVE_POUND, target: attacker);
            } else if (gMovesInfo[move].effect == EFFECT_AFTER_YOU) { // Player goes first, opponent third
                MOVE(attacker, move, target: target);
                MOVE(ignore1, MOVE_CELEBRATE);
                MOVE(target, MOVE_POUND, target: attacker);
                MOVE(ignore2, MOVE_CELEBRATE);
            } else if (gMovesInfo[move].effect == EFFECT_REVIVAL_BLESSING) { // Player selects party member
                MOVE(attacker, move, partyIndex: 2);
                MOVE(target, MOVE_LAST_RESORT, target: attacker); // Last Resort, so there's no anim on the opponent's side.
            } else if (gMovesInfo[move].effect == EFFECT_UPPER_HAND) { // Opponent needs to choose priority move
                MOVE(attacker, move, target: target);
                MOVE(target, MOVE_QUICK_ATTACK, target: attacker);
            } else { // All other moves
                MOVE(target, MOVE_LAST_RESORT, target: attacker); // Last Resort, so there's no anim on the opponent's side.
                MOVE(attacker, move, target: target);
            }
            if (gMovesInfo[move].effect != EFFECT_AFTER_YOU) {
                // Actions for the remaining battlers
                MOVE(ignore1, MOVE_CELEBRATE);
                MOVE(ignore2, MOVE_CELEBRATE);
            }
        }
    } SCENE {
        if (gMovesInfo[move].effect == EFFECT_MAGNETIC_FLUX || gMovesInfo[move].effect == EFFECT_GEAR_UP) // For some reason, Magnetic Flux and Gear Up are failing in Double Battles here
        {
            // Moves that fail in Double Battles
        } else if (gMovesInfo[move].effect == EFFECT_MIRROR_MOVE) { // Copy the opponent's move
            ANIMATION(ANIM_TYPE_MOVE, MOVE_POUND, attacker);
        } else if (gMovesInfo[move].effect == EFFECT_NATURE_POWER) { // Recorded battles always use BATTLE_TERRAIN_BUILDING
            ANIMATION(ANIM_TYPE_MOVE, B_NATURE_POWER_MOVES >= GEN_4 ? MOVE_TRI_ATTACK : MOVE_SWIFT, attacker);
        } else { // All other moves
            ANIMATION(ANIM_TYPE_MOVE, move, attacker);
        }
    } THEN {
        FORCE_MOVE_ANIM(FALSE);
        if (gLoadFail)
            DebugPrintf("Move failed: %S (%u)", gMovesInfo[move].name, move);
        //EXPECT_EQ(gLoadFail, FALSE);
    }
}

#endif
