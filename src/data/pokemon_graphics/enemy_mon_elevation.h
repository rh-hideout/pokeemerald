#include "constants/species.h"

// This determines how much higher above the usual position the enemy Pokémon
// is during battle. Species that float or fly have nonzero values.
const u8 gEnemyMonElevation[] =
{
	[SPECIES_BUTTERFREE] = 10,
	[SPECIES_BEEDRILL] = 9,
	[SPECIES_PIDGEOT] = 5,
	[SPECIES_FEAROW] = 6,
	[SPECIES_ZUBAT] = 16,
	[SPECIES_GOLBAT] = 10,
	[SPECIES_VENOMOTH] = 16,
	[SPECIES_GEODUDE] = 16,
	[SPECIES_MAGNEMITE] = 17,
	[SPECIES_MAGNETON] = 9,
	[SPECIES_GASTLY] = 10,
	[SPECIES_HAUNTER] = 11,
	[SPECIES_VOLTORB] = 11,
	[SPECIES_ELECTRODE] = 12,
	[SPECIES_KOFFING] = 14,
	[SPECIES_WEEZING] = 6,
	[SPECIES_AERODACTYL] = 9,
	[SPECIES_ZAPDOS] = 8,
	[SPECIES_MEW] = 11,
	[SPECIES_LEDIAN] = 10,
	[SPECIES_CROBAT] = 9,
	[SPECIES_HOPPIP] = 15,
	[SPECIES_SKIPLOOM] = 15,
	[SPECIES_JUMPLUFF] = 9,
	[SPECIES_YANMA] = 13,
	[SPECIES_MURKROW] = 12,
	[SPECIES_MISDREAVUS] = 12,
	[SPECIES_UNOWN] = 8,
	[SPECIES_GLIGAR] = 8,
	[SPECIES_LUGIA] = 6,
	[SPECIES_HO_OH] = 6,
	[SPECIES_CELEBI] = 15,
	[SPECIES_BEAUTIFLY] = 10,
	[SPECIES_DUSTOX] = 12,
	[SPECIES_NINJASK] = 10,
	[SPECIES_SHEDINJA] = 8,
	[SPECIES_WINGULL] = 15,
	[SPECIES_PELIPPER] = 8,
	[SPECIES_MASQUERAIN] = 10,
	[SPECIES_BALTOY] = 1,
	[SPECIES_CLAYDOL] = 10,
	[SPECIES_FLYGON] = 7,
	[SPECIES_GLALIE] = 12,
	[SPECIES_LUNATONE] = 13,
	[SPECIES_SOLROCK] = 4,
	[SPECIES_SWABLU] = 12,
	[SPECIES_ALTARIA] = 8,
	[SPECIES_DUSKULL] = 13,
	[SPECIES_SHUPPET] = 14,
	[SPECIES_BANETTE] = 8,
	[SPECIES_CASTFORM] = 16,
	[SPECIES_BELDUM] = 8,
	[SPECIES_RAYQUAZA] = 6,
	[SPECIES_LATIAS] = 12,
	[SPECIES_LATIOS] = 6,
	[SPECIES_JIRACHI] = 14,
	[SPECIES_CHIMECHO] = 16,
	[SPECIES_MOTHIM] = 10,
	[SPECIES_COMBEE] = 8,
	[SPECIES_VESPIQUEN] = 4,
	[SPECIES_DRIFLOON] = 5,
	[SPECIES_DRIFBLIM] = 7,
	[SPECIES_MISMAGIUS] = 3,
	[SPECIES_BRONZONG] = 4,
	[SPECIES_MAGNEZONE] = 4,
	[SPECIES_TOGEKISS] = 14,
	[SPECIES_YANMEGA] = 6,
	[SPECIES_GLISCOR] = 6,
	[SPECIES_ROTOM] = 10,
	[SPECIES_UXIE] = 6,
	[SPECIES_MESPRIT] = 6,
	[SPECIES_AZELF] = 6,
	[SPECIES_CRESSELIA] = 3,
	[SPECIES_DARKRAI] = 4,
};

