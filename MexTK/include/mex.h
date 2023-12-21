#ifndef MEX_H_MEX
#define MEX_H_MEX

#include "structs.h"
#include "datatypes.h"
#include "obj.h"

#define DB_FLAG 0

enum MEX_GETDATA
{
    MXDT_FTINTNUM,
    MXDT_FTEXTNUM,
    MXDT_FTICONNUM,
    MXDT_FTICONDATA,
    MXDT_GRINTNUM,
    MXDT_GREXTNUM,
    MXDT_GRICONNUM,
    MXDT_GRICONDATA,
    MXDT_FTCOSTUMEARCHIVE,
    MXDT_GRDESC,         // gets GrDesc struct
    MXDT_GREXTLOOKUP,    // gets first stage external ID from internal ID
    MXDT_GRNAME,         // returns stage name char*
    MXDT_FTNAME,         // returns fighter name char*, indexed by external ID
    MXDT_FTDAT,          // returns pointer to fighter file struct, indexed by internal ID
    MXDT_FTKINDDESC,     // returns pointer to ftkind desc array, indexed by external ID
    MXDT_FTEMBLEMLOOKUP, // returns pointer to ftkind desc array, indexed by external ID
    MXDT_MEXDATA,        //
    MXDT_PRELOADHEAP,    // returns pointer to PreloadHeap
};

typedef enum SSMKind
{
    SSMKIND_CHAR,
    SSMKIND_STAGE,
} SSMKind;

/*** Structs ***/

struct Stc_icns
{
    u16 reserved_num;
    u16 stride;
    MatAnimJointDesc *matanimjoint;
    int custom_stock_num;
    void *custom_stock_descs;
};

struct MEXPlaylistEntry
{
    u16 bgm;
    u16 chance;
};

struct MEXPlaylist
{
    int entry_count;
    MEXPlaylistEntry *entries;
};

struct MexCostumeDesc // this is the symbol in the file
{

    void *costume_vis_lookup; // 0x0
    void *costume_mat_lookup; // 0x4
    int accessory_num;        // 0x8
    struct MexCostumeDescAccessory
    {
        JOBJDesc *joint_desc;
        int bone_index;
        int dynamic_num;
        DynamicsDesc **dynamic_desc;
        int ftparts_num;
        FtPartsVis *ftparts;
        int dynamic_hit_num;
        DynamicsHitDesc **dynamic_hit_desc;
        AnimJointDesc *animjointdesc;
        MatAnimJointDesc *matanimjointdesc;
        FtScript *script;
    } **accessories;
};

typedef struct MexCostumeRuntime
{
    JOBJDesc *joint_desc;
    MatAnimJointDesc *matanim_desc;
    int x08;
    int x0C;
    int x10;
    HSD_Archive *archive;
} MexCostumeRuntime;

typedef struct MexCostumeRuntimeDesc
{
    MexCostumeRuntime *runtimes;
    int count;
} MexCostumeRuntimeDesc;
typedef struct MEXFunctionTable
{
    int index;       // 0x0
    int code_offset; // 0x4
} MEXFunctionTable;

typedef struct MEXFunction
{
    u8 *code;                           // 0x0
    int *instruction_reloc_table;       // 0x4
    int instruction_reloc_table_num;    // 0x8
    MEXFunctionTable *func_reloc_table; // 0xC
    int func_reloc_table_num;           // 0x10
    int code_size;                      // 0x14
    int debug_symbol_num;               // 0x18
    void *debug_symbol;                 // 0x1c
} MEXFunction;

typedef struct MexMetaData
{
    u8 v_major;
    u8 v_minor;
    short flags;
    int internal_id_count;
    int external_id_count;
    int css_icon_count;
    int internal_stage_count;
    int external_stage_count;
    int sss_icon_count;
    int ssm_count;
    int bgm_count;
    int effect_count;
    int bootup_scene;
    int last_major;
    int last_minor;
    int trophy_count;
    int trophy_sd_offset;
} MexMetaData;

typedef struct MexMenuData
{
    int *params;
    MnSlChrIcon *css_icons;
    void *sss_icons;
    void *sss_bitfields;
} MexMenuData;

typedef struct MexMusicTable
{
    char **filenames;
    MEXPlaylistEntry *menu_playlist;
    int menu_playlist_count;
    char **labels;

} MexMusicTable;

typedef struct MexStageSound
{
    char ssmid;
    char reverb;
    char x03;
} MexStageSound;

typedef struct MexStageTable
{
    void *id_table;
    MexStageSound *sound_table;
    void *collision_table;
    void *item_table;
    void *name_table;
    void *playlist_table;
} MexStageTable;

typedef struct MexData
{
    MexMetaData *metadata;
    MexMenuData *menu;
    void *fighter;
    void *fighter_function;
    void *ssm;
    MexMusicTable *music;
    void *effect;
    void *item;
    void *kirby;
    void *kirby_function;
    MexStageTable *stage;
    void *stage_function;
    void *scene;
    void *misc;
} MexData;

/*** Functions ***/
HSD_Archive *MEX_LoadRelArchive(char *file, void *functions, char *symbol);
void MEX_IndexFighterItem(int fighter_kind, ItemDesc *itemdesc, int item_id);
// void SpawnMEXEffect(int effectID, int fighter, int arg1, int arg2, int arg3, int arg4, int arg5);
float Mex_GetStockIconFrame(int internal_id, int costume_id);
int MEX_GetFtItemID(GOBJ *f, int item_id);
int MEX_GetGrItemID(int item_id);
int MEX_GetSSMID(SSMKind ssm_kind, int kind); // ssm_kind, 0 = fighter, 1 = stage | kind is the c_kind / gr_kind
void MEX_RelocRelArchive(void *xFunction);
int SFX_PlayStageSFX(int sfx_id); // use index relative to the ssm (start at 0)
void *calloc(int size);
MEXPlaylist *MEX_GetPlaylist();
// void KirbyStateChange(GOBJ *fighter, int state, float startFrame, float animSpeed, float animBlend);
void KirbyStateChange(float anim_start_frame, float anim_rate, float anim_blend, GOBJ *f, int state_id, int flags, GOBJ *alt_state_source);
void *MEX_GetKirbyCpData(int copy_id);
void *MEX_GetData(int index);

static void MEX_InitRELDAT(HSD_Archive *archive, char *symbol_name, int *return_func_array)
{
    MEXFunction *mex_function = Archive_GetPublicAddress(archive, symbol_name);

    // reloc instructions
    MEX_RelocRelArchive(mex_function);

    // reloc function pointers
    for (int i = 0; i < mex_function->func_reloc_table_num; i++)
    {
        MEXFunctionTable *this_func = &mex_function->func_reloc_table[i];
        return_func_array[i] = &mex_function->code[this_func->code_offset];
    }
}
static int MEX_PlayStageSoundRaw(int sfxid, int volume, int panning)
{
    // get soundbank id
    MexData *md = MEX_GetData(MXDT_MEXDATA);
    int internal_id = Stage_ExternalToInternal(Stage_GetExternalID());
    int bank = md->stage->sound_table[internal_id].ssmid * 10000;

    // play sound
    return SFX_PlayRaw(bank + sfxid, volume, panning, 0, 6);
}

#endif