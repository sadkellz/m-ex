#ifndef MEX_H_MEX
#define MEX_H_MEX

#include "structs.h"
#include "datatypes.h"
#include "obj.h"

#define DB_FLAG 0

/*** Structs ***/

struct PRIM
{
    int data;
};

struct Translation
{
    float frame;
    float value;
};

struct MEXPlaylist
{
    u16 bgm;
    u16 chance;
};

/*** Functions ***/

void (*MEX_IndexFighterItem)(int fighter_kind, void *itemdata, int item_id) = (void *)0x803d7058;
void (*SpawnMEXEffect)(int effectID, int fighter, int arg1, int arg2, int arg3, int arg4, int arg5) = (void *)0x803d7060;
int (*MEX_GetItemExtID)(GOBJ *gobj, int item_id) = (void *)0x803d7064; // gobj can be fighter or stage
int (*MEX_GetFtItemID)(int ft_kind, int item_id) = (void *)0x803d7088; // gobj can be fighter or stage
int (*MEX_GetGrItemExtID)(int item_id) = (void *)0x803d708C;           // gobj can be fighter or stage
void (*SFX_PlayStageSFX)(int sfx_id) = (void *)0x803d7078;
void *(*calloc)(int size) = (void *)0x803d706C;
PRIM *(*PRIM_NEW)(int vert_count, int params1, int params2) = (void *)0x804DD84C;
void (*PRIM_CLOSE)() = (void *)0x804DD848;
MEXPlaylist *(*MEX_GetPlaylist)() = (void *)0x803d707C;
int (*MEX_GetStageItemExtID)(int item_id) = (void *)0x803d7064;
void (*KirbyStateChange)(GOBJ *fighter, int state, float startFrame, float animSpeed, float animBlend) = 0x803d7080;
void *(*MEX_GetKirbyCpData)(int copy_id) = 0x803d7084;
int (*MEX_GetCopyItemExtID)(int copy_id, int item_id) = 0x803d7088;
#endif