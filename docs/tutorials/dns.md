## Day/Night system FAQ

### Q: How do I mark certain colors in a palette as light-blended?
A: Create a `.pla` file in the same folder as the `.pal` with the same name.

In this file you can enter color indices [0,15]
on separate lines to mark those colors as being light-blended, i.e:

`06.pla:`
```
# A comment
0 # if color 0 is listed, uses it to blend with instead of the default!
1
9
10
```

### Q: How do I limit what species can be used as followers?
You may use the following configs in `include/config/overworld.h`
```c
#define OW_FOLLOWERS_ALLOWED_SPECIES (0)
#define OW_FOLLOWERS_ALLOWED_MET_LVL (0)
#define OW_FOLLOWERS_ALLOWED_MET_LOC (0)
```
Examples:
```c
// Yellow Pikachu:
#define OW_FOLLOWERS_ALLOWED_SPECIES (SPECIES_PIKACHU)
#define OW_FOLLOWERS_ALLOWED_MET_LVL (0)
#define OW_FOLLOWERS_ALLOWED_MET_LOC (MAPSEC_PALLET_TOWN)
// Hoenn Starter:
#define OW_FOLLOWERS_ALLOWED_SPECIES (0)
#define OW_FOLLOWERS_ALLOWED_MET_LVL (5)
#define OW_FOLLOWERS_ALLOWED_MET_LOC (MAPSEC_ROUTE_101)
// Species set in VAR_XXXX:
#define OW_FOLLOWERS_ALLOWED_SPECIES (VAR_XXXX)
#define OW_FOLLOWERS_ALLOWED_MET_LVL (0)
#define OW_FOLLOWERS_ALLOWED_MET_LOC (0)
```
