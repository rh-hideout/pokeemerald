#include "global.h"
#include "test/battle.h"

ASSUMPTIONS
{
    ASSUME(GetMoveType(MOVE_TACKLE) == TYPE_NORMAL);
    ASSUME(GetMovePower(MOVE_TACKLE) > 0);
}

SINGLE_BATTLE_TEST("Refrigerate turns a Normal-type move into a Ice-type move")
{
    GIVEN {
        PLAYER(SPECIES_MEGANIUM);
        OPPONENT(SPECIES_AMAURA) { Ability(ABILITY_REFRIGERATE); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_TACKLE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TACKLE, opponent);
        MESSAGE("It's super effective!");
    }
}

SINGLE_BATTLE_TEST("Refrigerate boosts power of affected moves by 20% (Gen7+) or 30% (Gen1-6)", s16 damage)
{
    u32 ability, genConfig;
    PARAMETRIZE { ability = ABILITY_SNOW_WARNING;   genConfig = GEN_7; }
    PARAMETRIZE { ability = ABILITY_SNOW_WARNING;   genConfig = GEN_6; }
    PARAMETRIZE { ability = ABILITY_REFRIGERATE;    genConfig = GEN_7; }
    PARAMETRIZE { ability = ABILITY_REFRIGERATE;    genConfig = GEN_6; }

    GIVEN {
        WITH_CONFIG(GEN_CONFIG_ATE_MULTIPLIER, genConfig);
        PLAYER(SPECIES_AMAURA) { Ability(ability); Moves(MOVE_TACKLE); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_TACKLE); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        if (genConfig >= GEN_7)
            EXPECT_MUL_EQ(results[0].damage, Q_4_12(1.8), results[2].damage); // STAB + ate
        else
            EXPECT_MUL_EQ(results[1].damage, Q_4_12(1.95), results[3].damage); // STAB + ate
    }
}

// Gen 6-7
SINGLE_BATTLE_TEST("Refrigerate overrides Electrify (Gen6-7)", s16 damage)
{
    u32 ability, genConfig;
    PARAMETRIZE { ability = ABILITY_SNOW_WARNING;   genConfig = GEN_7; }
    PARAMETRIZE { ability = ABILITY_SNOW_WARNING;   genConfig = GEN_6; }
    PARAMETRIZE { ability = ABILITY_REFRIGERATE;    genConfig = GEN_7; }
    PARAMETRIZE { ability = ABILITY_REFRIGERATE;    genConfig = GEN_6; }
    
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_ELECTRIFY) == EFFECT_ELECTRIFY);
        WITH_CONFIG(GEN_CONFIG_ATE_MULTIPLIER, genConfig);
        WITH_CONFIG(GEN_CONFIG_ATE_OVERRIDE, genConfig);
        PLAYER(SPECIES_AMAURA) { Ability(ability); Moves(MOVE_TACKLE); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, MOVE_ELECTRIFY); MOVE(player, MOVE_TACKLE); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        if (genConfig >= GEN_7)
            EXPECT_MUL_EQ(results[0].damage, Q_4_12(1.8), results[2].damage); // STAB + ate
        else
            EXPECT_MUL_EQ(results[1].damage, Q_4_12(1.95), results[3].damage); // STAB + ate
    }
}

SINGLE_BATTLE_TEST("Refrigerate overrides Ion Deluge (Gen6-7)", s16 damage)
{
    u32 ability, genConfig;
    PARAMETRIZE { ability = ABILITY_SNOW_WARNING;   genConfig = GEN_7; }
    PARAMETRIZE { ability = ABILITY_SNOW_WARNING;   genConfig = GEN_6; }
    PARAMETRIZE { ability = ABILITY_REFRIGERATE;    genConfig = GEN_7; }
    PARAMETRIZE { ability = ABILITY_REFRIGERATE;    genConfig = GEN_6; }
    
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_ION_DELUGE) == EFFECT_ION_DELUGE);
        WITH_CONFIG(GEN_CONFIG_ATE_MULTIPLIER, genConfig);
        WITH_CONFIG(GEN_CONFIG_ATE_OVERRIDE, genConfig);
        PLAYER(SPECIES_AMAURA) { Ability(ability); Moves(MOVE_TACKLE); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, MOVE_ION_DELUGE); MOVE(player, MOVE_TACKLE); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        if (genConfig >= GEN_7)
            EXPECT_MUL_EQ(results[0].damage, Q_4_12(1.8), results[2].damage); // STAB + ate
        else
            EXPECT_MUL_EQ(results[1].damage, Q_4_12(1.95), results[3].damage); // STAB + ate
    }
}

// Gen 8+
SINGLE_BATTLE_TEST("Refrigerate doesn't override Electrify (Gen8+)", s16 damage)
{
    // Bulbapedia doesn't list this effect, so it assumes it behaves like Pixilate.
    u32 ability, genConfig;
    PARAMETRIZE { ability = ABILITY_SNOW_WARNING;   genConfig = GEN_7; }
    PARAMETRIZE { ability = ABILITY_SNOW_WARNING;   genConfig = GEN_8; }
    PARAMETRIZE { ability = ABILITY_REFRIGERATE;    genConfig = GEN_7; }
    PARAMETRIZE { ability = ABILITY_REFRIGERATE;    genConfig = GEN_8; }
    
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_ELECTRIFY) == EFFECT_ELECTRIFY);
        WITH_CONFIG(GEN_CONFIG_ATE_MULTIPLIER, genConfig);
        WITH_CONFIG(GEN_CONFIG_ATE_OVERRIDE, genConfig);
        PLAYER(SPECIES_AMAURA) { Ability(ability); Moves(MOVE_TACKLE); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, MOVE_ELECTRIFY); MOVE(player, MOVE_TACKLE); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        if (genConfig >= GEN_7)
            EXPECT_MUL_EQ(results[0].damage, Q_4_12(1.8), results[2].damage); // STAB + ate
        else
            EXPECT_EQ(results[1].damage, results[3].damage);
    }
} 

SINGLE_BATTLE_TEST("Refrigerate doesn't override Ion Deluge (Gen8+)", s16 damage)
{
    // Ion Deluge doesn't exist in Gen 8+, but we probably could assume it behaves similar to under Electrify. TODO: Test by hacking SV.
    u32 ability, genConfig;
    PARAMETRIZE { ability = ABILITY_SNOW_WARNING;   genConfig = GEN_7; }
    PARAMETRIZE { ability = ABILITY_SNOW_WARNING;   genConfig = GEN_8; }
    PARAMETRIZE { ability = ABILITY_REFRIGERATE;    genConfig = GEN_7; }
    PARAMETRIZE { ability = ABILITY_REFRIGERATE;    genConfig = GEN_8; }
    
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_ION_DELUGE) == EFFECT_ION_DELUGE);
        WITH_CONFIG(GEN_CONFIG_ATE_MULTIPLIER, genConfig);
        WITH_CONFIG(GEN_CONFIG_ATE_OVERRIDE, genConfig);
        PLAYER(SPECIES_AMAURA) { Ability(ability); Moves(MOVE_TACKLE); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, EFFECT_ION_DELUGE); MOVE(player, MOVE_TACKLE); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        if (genConfig >= GEN_7)
            EXPECT_MUL_EQ(results[0].damage, Q_4_12(1.8), results[2].damage); // STAB + ate
        else
            EXPECT_EQ(results[1].damage, results[3].damage);
    }
}

SINGLE_BATTLE_TEST("Refrigerate doesn't affect Weather Ball's type", s16 damage)
{
    u16 move, ability;
    PARAMETRIZE { move = MOVE_CELEBRATE; ability = ABILITY_SNOW_WARNING; }
    PARAMETRIZE { move = MOVE_SUNNY_DAY; ability = ABILITY_SNOW_WARNING; }
    PARAMETRIZE { move = MOVE_CELEBRATE; ability = ABILITY_REFRIGERATE; }
    PARAMETRIZE { move = MOVE_SUNNY_DAY; ability = ABILITY_REFRIGERATE; }
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_WEATHER_BALL) == EFFECT_WEATHER_BALL);
        ASSUME(gSpeciesInfo[SPECIES_PINSIR].types[0] == TYPE_BUG);
        PLAYER(SPECIES_AMAURA) { Ability(ability); }
        OPPONENT(SPECIES_PINSIR);
    } WHEN {
        TURN { MOVE(player, move); }
        TURN { MOVE(player, MOVE_WEATHER_BALL); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
        if (move == MOVE_SUNNY_DAY)
            MESSAGE("It's super effective!");
    } FINALLY {
        EXPECT_MUL_EQ(results[0].damage, Q_4_12(2.0), results[1].damage); // double base power + type effectiveness + sun 50% boost vs hail no type effectiveness
        EXPECT_MUL_EQ(results[2].damage, Q_4_12(6.0), results[3].damage); // double base power + type effectiveness + sun 50% boost
        EXPECT_MUL_EQ(results[2].damage, Q_4_12(3.0), results[0].damage); // type effectiveness + sun 50% boost, both have double base power
        EXPECT_EQ(results[1].damage, results[3].damage);
    }
}

SINGLE_BATTLE_TEST("Refrigerate doesn't affect Natural Gift's type")
{
    u16 ability;
    PARAMETRIZE { ability = ABILITY_SNOW_WARNING; }
    PARAMETRIZE { ability = ABILITY_REFRIGERATE; }
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_NATURAL_GIFT) == EFFECT_NATURAL_GIFT);
        ASSUME(gNaturalGiftTable[ITEM_TO_BERRY(ITEM_ORAN_BERRY)].type == TYPE_POISON);
        ASSUME(gSpeciesInfo[SPECIES_BELDUM].types[0] == TYPE_STEEL);
        PLAYER(SPECIES_AMAURA) { Ability(ability); Item(ITEM_ORAN_BERRY); }
        OPPONENT(SPECIES_BELDUM);
    } WHEN {
        TURN { MOVE(player, MOVE_NATURAL_GIFT); }
    } SCENE {
        NOT { ANIMATION(ANIM_TYPE_MOVE, MOVE_NATURAL_GIFT, player); }
        MESSAGE("It doesn't affect the opposing Beldum…");
    }
}


TO_DO_BATTLE_TEST("Refrigerate doesn't affect Tera Starstorm's type");
TO_DO_BATTLE_TEST("Refrigerate doesn't affect Natural Gift's type");
TO_DO_BATTLE_TEST("Refrigerate doesn't affect Max Strike's type");
TO_DO_BATTLE_TEST("Refrigerate doesn't affect Weather Ball's type");
TO_DO_BATTLE_TEST("Refrigerate doesn't affect Hidden Power's type");
TO_DO_BATTLE_TEST("Refrigerate doesn't affect Judgment/Techno Blast/Multi-Attack's type");
TO_DO_BATTLE_TEST("Refrigerate doesn't affect Terrain Pulse's type");
TO_DO_BATTLE_TEST("Refrigerate doesn't affect damaging Z-Move types");
TO_DO_BATTLE_TEST("(DYNAMAX) Refrigerate turns Max Strike into Max Hailstorm when not used by Gigantamax Lapras");
//TO_DO_BATTLE_TEST("(DYNAMAX) Refrigerate doesn't turn Max Strike into Max Hailstorm when used by Gigantamax Lapras, instead becoming G-Max Resonance"); // Marked in Bulbapedia as "needs research", so this assumes that it behaves like Pixilate.
