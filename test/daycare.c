#include "global.h"
#include "daycare.h"
#include "event_data.h"
#include "malloc.h"
#include "party_menu.h"
#include "regions.h"
#include "test/overworld_script.h"
#include "test/test.h"

// We don't run the StoreSelectedPokemonInDaycare special because it relies on calling the
// party select screen and the GetCursorSelectionMonId function, so we store directly to the struct.
#define STORE_IN_DAYCARE_AND_GET_EGG()                                          \
    StorePokemonInDaycare(&gPlayerParty[0], &gSaveBlock1Ptr->daycare.mons[0]);  \
    StorePokemonInDaycare(&gPlayerParty[0], &gSaveBlock1Ptr->daycare.mons[1]);  \
    RUN_OVERWORLD_SCRIPT( special GiveEggFromDaycare; );

TEST("(Daycare) Pokémon generate Eggs of the lowest member of the evolutionary family")
{
    ASSUME(P_FAMILY_PIKACHU == TRUE);
    ASSUME(P_GEN_2_CROSS_EVOS == TRUE);

    ZeroPlayerPartyMons();
    RUN_OVERWORLD_SCRIPT(
        givemon SPECIES_PIKACHU, 100, gender=MON_MALE;
        givemon SPECIES_PIKACHU, 100, gender=MON_FEMALE;
    );
    STORE_IN_DAYCARE_AND_GET_EGG();

    EXPECT_EQ(GetMonData(&gPlayerParty[0], MON_DATA_SPECIES), SPECIES_PICHU);
}

TEST("(Daycare) Pokémon offspring species is based off the mother's species")
{
    u32 offspring = 0;
    ASSUME(P_FAMILY_PIKACHU == TRUE);
    ASSUME(P_GEN_2_CROSS_EVOS == TRUE);
    ASSUME(P_FAMILY_RIOLU == TRUE);

    ZeroPlayerPartyMons();
    PARAMETRIZE { offspring = SPECIES_RIOLU; RUN_OVERWORLD_SCRIPT(givemon SPECIES_PIKACHU, 100, gender=MON_MALE;   givemon SPECIES_LUCARIO, 100, gender=MON_FEMALE;); }
    PARAMETRIZE { offspring = SPECIES_PICHU; RUN_OVERWORLD_SCRIPT(givemon SPECIES_PIKACHU, 100, gender=MON_FEMALE; givemon SPECIES_LUCARIO, 100, gender=MON_MALE;); }
    STORE_IN_DAYCARE_AND_GET_EGG();

    EXPECT_EQ(GetMonData(&gPlayerParty[0], MON_DATA_SPECIES), offspring);
}

TEST("(Daycare) Pokémon can breed with Ditto if they don't belong to the Ditto or No Eggs Discovered group")
{
    u32 j = 0;
    u32 parentSpecies = 0;

    ZeroPlayerPartyMons();
    for (j = 1; j < NUM_SPECIES; j++)
        PARAMETRIZE { parentSpecies = j; }
    VarSet(VAR_TEMP_C, parentSpecies);
    RUN_OVERWORLD_SCRIPT(
        givemon SPECIES_DITTO, 100; givemon VAR_TEMP_C, 100;
    );
    STORE_IN_DAYCARE_AND_GET_EGG();

    if (gSpeciesInfo[parentSpecies].eggGroups[0] != EGG_GROUP_NO_EGGS_DISCOVERED
     && gSpeciesInfo[parentSpecies].eggGroups[0] != EGG_GROUP_DITTO)
        EXPECT_NE(GetMonData(&gPlayerParty[0], MON_DATA_SPECIES), SPECIES_NONE);
    else
        EXPECT_EQ(GetMonData(&gPlayerParty[0], MON_DATA_SPECIES), SPECIES_NONE);
}

TEST("(Daycare) Shellos' form is always based on the mother's form")
{
    u32 offspring = 0;
    ASSUME(P_FAMILY_MEOWTH == TRUE);
    ASSUME(P_ALOLAN_FORMS == TRUE);
    ASSUME(P_GALARIAN_FORMS == TRUE);

    ZeroPlayerPartyMons();
    PARAMETRIZE { offspring = SPECIES_SHELLOS_WEST; RUN_OVERWORLD_SCRIPT(givemon SPECIES_SHELLOS_EAST, 1, gender=MON_MALE; givemon SPECIES_SHELLOS_WEST, 1, gender=MON_FEMALE;); }
    PARAMETRIZE { offspring = SPECIES_SHELLOS_WEST; RUN_OVERWORLD_SCRIPT(givemon SPECIES_SHELLOS_EAST, 1, gender=MON_MALE, item=ITEM_EVERSTONE; givemon SPECIES_SHELLOS_WEST, 1, gender=MON_FEMALE;); }
    PARAMETRIZE { offspring = SPECIES_SHELLOS_WEST; RUN_OVERWORLD_SCRIPT(givemon SPECIES_SHELLOS_EAST, 1, gender=MON_MALE; givemon SPECIES_SHELLOS_WEST, 1, gender=MON_FEMALE, item=ITEM_EVERSTONE;); }
    PARAMETRIZE { offspring = SPECIES_SHELLOS_EAST; RUN_OVERWORLD_SCRIPT(givemon SPECIES_SHELLOS_WEST, 1, gender=MON_MALE; givemon SPECIES_SHELLOS_EAST, 1, gender=MON_FEMALE;); }
    PARAMETRIZE { offspring = SPECIES_SHELLOS_EAST; RUN_OVERWORLD_SCRIPT(givemon SPECIES_SHELLOS_WEST, 1, gender=MON_MALE, item=ITEM_EVERSTONE; givemon SPECIES_SHELLOS_EAST, 1, gender=MON_FEMALE;); }
    PARAMETRIZE { offspring = SPECIES_SHELLOS_EAST; RUN_OVERWORLD_SCRIPT(givemon SPECIES_SHELLOS_WEST, 1, gender=MON_MALE; givemon SPECIES_SHELLOS_EAST, 1, gender=MON_FEMALE, item=ITEM_EVERSTONE;); }
    STORE_IN_DAYCARE_AND_GET_EGG();

    EXPECT_EQ(GetMonData(&gPlayerParty[0], MON_DATA_SPECIES), offspring);
}

TEST("(Daycare) Pokémon with regional forms give the correct offspring")
{
    u32 offspring = 0, region = 0;
    ASSUME(P_FAMILY_MEOWTH == TRUE);
    ASSUME(P_ALOLAN_FORMS == TRUE);
    ASSUME(P_GALARIAN_FORMS == TRUE);

    ZeroPlayerPartyMons();

    region = GetCurrentRegion();
    if (region == REGION_ALOLA) {
        PARAMETRIZE { offspring = SPECIES_MEOWTH_ALOLA; RUN_OVERWORLD_SCRIPT(givemon SPECIES_MEOWTH,  1, gender=MON_MALE; givemon SPECIES_MEOWTH_ALOLA, 1, gender=MON_FEMALE;); }
        PARAMETRIZE { offspring = SPECIES_MEOWTH_ALOLA; RUN_OVERWORLD_SCRIPT(givemon SPECIES_MEOWTH,  1, gender=MON_MALE; givemon SPECIES_MEOWTH_ALOLA, 1, gender=MON_FEMALE, item=ITEM_EVERSTONE;); }
        PARAMETRIZE { offspring = SPECIES_MEOWTH_ALOLA; RUN_OVERWORLD_SCRIPT(givemon SPECIES_MEOWTH,  1, gender=MON_MALE; givemon SPECIES_MEOWTH_GALAR, 1, gender=MON_FEMALE;); }
        PARAMETRIZE { offspring = SPECIES_MEOWTH_GALAR; RUN_OVERWORLD_SCRIPT(givemon SPECIES_MEOWTH,  1, gender=MON_MALE; givemon SPECIES_MEOWTH_GALAR, 1, gender=MON_FEMALE, item=ITEM_EVERSTONE;); }
        PARAMETRIZE { offspring = SPECIES_MEOWTH_ALOLA; RUN_OVERWORLD_SCRIPT(givemon SPECIES_DIGLETT, 1, gender=MON_MALE; givemon SPECIES_MEOWTH_GALAR, 1, gender=MON_FEMALE;); }
        PARAMETRIZE { offspring = SPECIES_MEOWTH_GALAR; RUN_OVERWORLD_SCRIPT(givemon SPECIES_DIGLETT, 1, gender=MON_MALE; givemon SPECIES_MEOWTH_GALAR, 1, gender=MON_FEMALE, item=ITEM_EVERSTONE;); }
        PARAMETRIZE { offspring = SPECIES_MEOWTH_GALAR; RUN_OVERWORLD_SCRIPT(givemon SPECIES_PERRSERKER, 1, gender=MON_MALE, item=ITEM_EVERSTONE; givemon SPECIES_PERSIAN, 1, gender=MON_FEMALE;); }
        PARAMETRIZE { offspring = SPECIES_MEOWTH;       RUN_OVERWORLD_SCRIPT(givemon SPECIES_PERRSERKER, 1, gender=MON_MALE, item=ITEM_EVERSTONE; givemon SPECIES_PERSIAN, 1, gender=MON_FEMALE, item=ITEM_EVERSTONE;); }
        PARAMETRIZE { offspring = SPECIES_MEOWTH;       RUN_OVERWORLD_SCRIPT(givemon SPECIES_PERSIAN_ALOLA, 1, gender=MON_MALE, item=ITEM_EVERSTONE; givemon SPECIES_PERSIAN, 1, gender=MON_FEMALE, item=ITEM_EVERSTONE;); }
    } else if (region == REGION_GALAR) {
        PARAMETRIZE { offspring = SPECIES_MEOWTH_GALAR; RUN_OVERWORLD_SCRIPT(givemon SPECIES_MEOWTH,  1, gender=MON_MALE; givemon SPECIES_MEOWTH_ALOLA, 1, gender=MON_FEMALE;); }
        PARAMETRIZE { offspring = SPECIES_MEOWTH_ALOLA; RUN_OVERWORLD_SCRIPT(givemon SPECIES_MEOWTH,  1, gender=MON_MALE; givemon SPECIES_MEOWTH_ALOLA, 1, gender=MON_FEMALE, item=ITEM_EVERSTONE;); }
        PARAMETRIZE { offspring = SPECIES_MEOWTH_GALAR; RUN_OVERWORLD_SCRIPT(givemon SPECIES_MEOWTH,  1, gender=MON_MALE; givemon SPECIES_MEOWTH_GALAR, 1, gender=MON_FEMALE;); }
        PARAMETRIZE { offspring = SPECIES_MEOWTH_GALAR; RUN_OVERWORLD_SCRIPT(givemon SPECIES_MEOWTH,  1, gender=MON_MALE; givemon SPECIES_MEOWTH_GALAR, 1, gender=MON_FEMALE, item=ITEM_EVERSTONE;); }
        PARAMETRIZE { offspring = SPECIES_MEOWTH_GALAR; RUN_OVERWORLD_SCRIPT(givemon SPECIES_DIGLETT, 1, gender=MON_MALE; givemon SPECIES_MEOWTH_GALAR, 1, gender=MON_FEMALE;); }
        PARAMETRIZE { offspring = SPECIES_MEOWTH_GALAR; RUN_OVERWORLD_SCRIPT(givemon SPECIES_DIGLETT, 1, gender=MON_MALE; givemon SPECIES_MEOWTH_GALAR, 1, gender=MON_FEMALE, item=ITEM_EVERSTONE;); }
        PARAMETRIZE { offspring = SPECIES_MEOWTH_GALAR; RUN_OVERWORLD_SCRIPT(givemon SPECIES_PERRSERKER, 1, gender=MON_MALE, item=ITEM_EVERSTONE; givemon SPECIES_PERSIAN, 1, gender=MON_FEMALE;); }
        PARAMETRIZE { offspring = SPECIES_MEOWTH;       RUN_OVERWORLD_SCRIPT(givemon SPECIES_PERRSERKER, 1, gender=MON_MALE, item=ITEM_EVERSTONE; givemon SPECIES_PERSIAN, 1, gender=MON_FEMALE, item=ITEM_EVERSTONE;); }
        PARAMETRIZE { offspring = SPECIES_MEOWTH;       RUN_OVERWORLD_SCRIPT(givemon SPECIES_PERSIAN_ALOLA, 1, gender=MON_MALE, item=ITEM_EVERSTONE; givemon SPECIES_PERSIAN, 1, gender=MON_FEMALE, item=ITEM_EVERSTONE;); }
    } else {
        PARAMETRIZE { offspring = SPECIES_MEOWTH;       RUN_OVERWORLD_SCRIPT(givemon SPECIES_MEOWTH,  1, gender=MON_MALE; givemon SPECIES_MEOWTH_ALOLA, 1, gender=MON_FEMALE;); }
        PARAMETRIZE { offspring = SPECIES_MEOWTH_ALOLA; RUN_OVERWORLD_SCRIPT(givemon SPECIES_MEOWTH,  1, gender=MON_MALE; givemon SPECIES_MEOWTH_ALOLA, 1, gender=MON_FEMALE, item=ITEM_EVERSTONE;); }
        PARAMETRIZE { offspring = SPECIES_MEOWTH;       RUN_OVERWORLD_SCRIPT(givemon SPECIES_MEOWTH,  1, gender=MON_MALE; givemon SPECIES_MEOWTH_GALAR, 1, gender=MON_FEMALE;); }
        PARAMETRIZE { offspring = SPECIES_MEOWTH_GALAR; RUN_OVERWORLD_SCRIPT(givemon SPECIES_MEOWTH,  1, gender=MON_MALE; givemon SPECIES_MEOWTH_GALAR, 1, gender=MON_FEMALE, item=ITEM_EVERSTONE;); }
        PARAMETRIZE { offspring = SPECIES_MEOWTH;       RUN_OVERWORLD_SCRIPT(givemon SPECIES_DIGLETT, 1, gender=MON_MALE; givemon SPECIES_MEOWTH_GALAR, 1, gender=MON_FEMALE;); }
        PARAMETRIZE { offspring = SPECIES_MEOWTH_GALAR; RUN_OVERWORLD_SCRIPT(givemon SPECIES_DIGLETT, 1, gender=MON_MALE; givemon SPECIES_MEOWTH_GALAR, 1, gender=MON_FEMALE, item=ITEM_EVERSTONE;); }
        PARAMETRIZE { offspring = SPECIES_MEOWTH_GALAR; RUN_OVERWORLD_SCRIPT(givemon SPECIES_PERRSERKER, 1, gender=MON_MALE, item=ITEM_EVERSTONE; givemon SPECIES_PERSIAN, 1, gender=MON_FEMALE;); }
        PARAMETRIZE { offspring = SPECIES_MEOWTH;       RUN_OVERWORLD_SCRIPT(givemon SPECIES_PERRSERKER, 1, gender=MON_MALE, item=ITEM_EVERSTONE; givemon SPECIES_PERSIAN, 1, gender=MON_FEMALE, item=ITEM_EVERSTONE;); }
        PARAMETRIZE { offspring = SPECIES_MEOWTH;       RUN_OVERWORLD_SCRIPT(givemon SPECIES_PERSIAN_ALOLA, 1, gender=MON_MALE, item=ITEM_EVERSTONE; givemon SPECIES_PERSIAN, 1, gender=MON_FEMALE, item=ITEM_EVERSTONE;); }
    }
    STORE_IN_DAYCARE_AND_GET_EGG();

    EXPECT_EQ(GetMonData(&gPlayerParty[0], MON_DATA_SPECIES), offspring);
}
