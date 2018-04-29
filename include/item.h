#ifndef GUARD_ITEM_H
#define GUARD_ITEM_H

enum
{
    ITEMS_POCKET,
    BALLS_POCKET,
    TMHM_POCKET,
    BERRIES_POCKET,
    KEYITEMS_POCKET,
    POCKETS_COUNT
};

typedef void (*ItemUseFunc)(u8);

struct Item
{
    u8 name[ITEM_NAME_LENGTH];
    u16 itemId;
    u16 price;
    u8 holdEffect;
    u8 holdEffectParam;
    const u8 *description;
    u8 importance;
    u8 unk19;
    u8 pocket;
    u8 type;
    ItemUseFunc fieldUseFunc;
    u8 battleUsage;
    ItemUseFunc battleUseFunc;
    u8 secondaryId;
};

struct BagPocket
{
    struct ItemSlot *itemSlots;
    u8 capacity;
};

extern struct BagPocket gBagPockets[];

void GetBerryCountString(u8* dst, const u8* berryName, u32 quantity);
void CopyItemName(u16 itemId, u8 *string);
void CopyItemNameHandlePlural(u16 itemId, u8 *string, u32 quantity);
bool8 IsBagPocketNonEmpty(u8 pocket);
bool8 CheckBagHasItem(u16 itemId, u16 count);
bool8 CheckBagHasSpace(u16 itemId, u16 count);
bool8 AddBagItem(u16 itemId, u16 count);
bool8 RemoveBagItem(u16 itemId, u16 count);
u8 GetPocketByItemId(u16 itemId);
void ClearItemSlots(struct ItemSlot *itemSlots, u8 b);
u8 CountUsedPCItemSlots(void);
bool8 CheckPCHasItem(u16 itemId, u16 count);
bool8 AddPCItem(u16 itemId, u16 count);
void RemovePCItem(u8 index, u16 count);
void SwapRegisteredBike(void);
const struct Item *ItemId_GetItem(u16 itemId);
u16 ItemId_GetId(u16 itemId);
u16 ItemId_GetPrice(u16 itemId);
u8 ItemId_GetHoldEffect(u16 itemId);
u8 ItemId_GetHoldEffectParam(u16 itemId);
const u8 *ItemId_GetDescription(u16 itemId);
bool32 ItemId_CopyDescription(u8 *a, u32 itemId, u32 c);
u8 ItemId_GetImportance(u16 itemId);
u8 ItemId_GetUnknownValue(u16 itemId);
u8 ItemId_GetPocket(u16 itemId);
u8 ItemId_GetType(u16 itemId);
ItemUseFunc ItemId_GetFieldFunc(u16 itemId);
u8 ItemId_GetBattleUsage(u16 itemId);
ItemUseFunc ItemId_GetBattleFunc(u16 itemId);
u8 ItemId_GetSecondaryId(u16 itemId);
u16 itemid_get_market_price(u16 itemId);
u16 BagGetItemIdByPocketPosition(u8 pocketId, u16 pocketPos);
void sub_80D6FB4(struct BagPocket*);
void sub_80D6F64(struct BagPocket*);
void MoveItemSlotInList(struct ItemSlot* itemSlots, u32 a1, u32 a2);
u8 sub_80D6CE4();
bool8 itemid_is_unique(u16 itemId);
void sub_80D6E48(u8, u16);
void sub_80D6E84(void);
void CompactPCItems(void);
void SortBerriesOrTMHMs(struct BagPocket *bagPocket);
void CompactItemsInBagPocket(struct BagPocket *bagPocket);
const u8 *ItemId_GetName(u16 itemId);

#endif // GUARD_ITEM_H
