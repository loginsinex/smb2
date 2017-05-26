

typedef struct _tagNES_ITEM_DIM
{
	BYTE		topleft;
	BYTE		top;
	BYTE		topright;
	BYTE		left;
	BYTE		right;
	BYTE		middle;
	BYTE		bottomleft;
	BYTE		bottomright;
} NES_ITEM_DIM, *PNES_ITEM_DIM;

typedef struct _tagNES_ENEMY_DIM
{
	BYTE		xy;
	BYTE		szxy;
} NES_ENEMY_DIM, *PNES_ENEMY_DIM;

typedef union _tagNES_GROUND_DIM
{
	DWORD		dwGround;
	struct
	{
		BYTE		gr[4];
	} gr;
} NES_GROUND_DIM, *PNES_GROUND_DIM;

typedef struct _tagNES_GROUND_BITSET
{
	BYTE		uGroundType;
	DWORD		uGroundSet;
} NES_GROUND_BITSET, *PNES_GROUND_BITSET;


typedef struct _tagNES_PRIORITY_LIST
{
	BYTE		bgPriority;
	BYTE		bObjId;			// regular id
	BYTE		bPriority;		// priority. 0 - higher, 5 - lower
} NES_PRIORITY_LIST, *PNES_PRIORITY_LIST;

extern const NES_ITEM_DIM g_mItemDim[];
extern const NES_ITEM_DIM g_msItemDim[];
extern const NES_ENEMY_DIM g_mEnemyDim[];
extern const DWORD g_mGroundSet[];
extern const DWORD g_mvGroundSet[];
extern const NES_GROUND_DIM g_mGroundType[];
extern const NES_GROUND_DIM g_mvGroundType[];
extern const NES_PRIORITY_LIST g_mPriorityList[];
extern const NES_GROUND_DIM g_mbgPriority[];
extern const NES_GROUND_DIM g_mvbgPriority[];
extern const BYTE g_mLayerLimiter[];
extern const BYTE g_mLayerLimiting[];
extern const USHORT g_mWorldInterior[];
extern const COLORREF g_crNesColor[];