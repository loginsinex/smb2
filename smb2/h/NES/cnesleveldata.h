
extern const LPTSTR		g_szItemName[];
extern const LPTSTR		g_szSOName[];

#define		NES_JAR_HEADER			0x18E30101

#define		IS_POINTABLE(obj)		( nliEntrance == (obj)->nli || nliRegular == (obj)->nli )
#define		IS_MVPOINT(obj)			( nliSkipper == (obj)->nli || nliBackToStart == (obj)->nli )
#define		OBJ_SUM(obj1, obj2)		( (obj2)->Object.General.y += (obj1)->Object.General.y )
#define		OBJ_SUB(obj1, obj2)		( (obj2)->Object.General.y -= (obj1)->Object.General.y )
#define		OBJ_RST(obj1)			( (obj1)->Object.General.y = 0 )
#define		OBJ_EQ(obj1, obj2)		( (obj2)->Object.General.y = (obj1)->Object.General.y )

#define		IS_OBJ_LESS(obj1, obj2)	( (obj1)->Object.General.y < (obj2)->Object.General.y )
#define		IS_OBJ_GREAT(obj1, obj2) ( (obj1)->Object.General.y > (obj2)->Object.General.y )
#define		IS_OBJ_EQ(obj1, obj2)	( (obj1)->Object.General.y == (obj2)->Object.General.y )

typedef enum
{
	nliSkipper		= 0,		// 1 byte
	nliBackToStart	= 1,		// 1 byte
	nliRegular		= 2,		// 2 bytes
	nliGroundSet	= 3,		// 2 bytes
	nliGroundType	= 4,		// 2 bytes
	nliPtr			= 5,		// 3 bytes
	nliEntrance		= 6,		// 4 bytes
	nliUnknown		= 7			// 2 bytes?
} NLI_TYPE, *PNLI_TYPE;

typedef enum _tagNES_LEVEL_FIELDS
{
	nlfDirection		= 0,
	nlfReserved1		= 1,
	nlfColor			= 2,
	nlfReserved2		= 3,
	nlfEnemyColor		= 4,
	nlfReserved3		= 5,
	nlfGround			= 6,
	nlfLength			= 7,
	nlfObjectType		= 8,
	nlfReserved4		= 9,
	nlfTypeOfGround		= 10,
	nlfReserved5		= 11,
	nlfMusic			= 12
} NES_LEVEL_FIELDS;

typedef struct _tagNES_LEVEL_ITEM
{
	NLI_TYPE		nli;
	POINT			pt;
	DWORD			Page;

	union
	{
		BYTE		pData[5];
		struct
		{
			BYTE	uType;
			BYTE	Reserved2[4];
		} Skipper;
		struct
		{
			BYTE	Reserved[5];
		} BackToStart;
		struct
		{
			BYTE	x:4;
			BYTE	y:4;
			BYTE	id;
			BYTE	Reserved[2];
		} Regular;
		struct
		{
			BYTE	Reserved;
			BYTE	uSets;
			BYTE	Reserved2[3];
		} GroundSet;
		struct
		{
			BYTE	Reserved;
			BYTE	uType;
			BYTE	Reserved2[3];
		} GroundType;
		struct
		{
			BYTE	Reserved;
			BYTE	uParameter[2];
			BYTE	Reserved2[2];
		} Pointer;
		struct
		{
			BYTE	y:4;
			BYTE	x:4;
			BYTE	uEntranceId;
			BYTE	uEntranceParameter[3];
		} Entrance;
		struct
		{
			BYTE	x:4;
			BYTE	y:4;
			BYTE	id;
			BYTE	Reserved[3];
		} General;
		struct
		{
			BYTE	Reserved[5];
		} Unknown;
	} Object;
	DWORD	SizeOfItem();
} NES_LEVEL_ITEM, *PNES_LEVEL_ITEM;

class CNesLevelContainer: public CNesItemsList<NES_LEVEL_ITEM>
{
	DWORD						m_dwLevelHeader;
	DWORD						m_dwLinkedToLevel;
	DWORD						m_dwLevelPtr;
	DWORD						m_dwLevelSize;

	struct
	{
		BYTE		fDirection:1;
		BYTE		Reserved1:1;
		BYTE		uColor:3;
		BYTE		Reserved2:1;
		BYTE		uEnemyColor:2;
		BYTE		Reserved3:3;
		BYTE		uGround:5;
		BYTE		uLength:4;
		BYTE		uObjectType:4;
		BYTE		Reserved4:2;
		BYTE		uTypeOfGround:3;
		BYTE		Reserved5:1;
		BYTE		uMusic:2;
	} m_levelFields;

	VOID						FlushObjects();
	BOOL						RereadPoints();
	VOID						StoreSizeOfLevel();
	VOID						StoreHeader();

public:
	CNesLevelContainer();
	~CNesLevelContainer();

	VOID		SetLevelPtr(DWORD ptr);
	DWORD		GetLevelPtr();
	BYTE		operator[](NES_LEVEL_FIELDS index);
	VOID		LoadLevel(PBYTE ptr, DWORD dwLinkedTo = -1);
	VOID		LoadLevelData(DWORD dwLevelHeader, std::vector<NES_LEVEL_ITEM> & pItemsList, DWORD dwCount);
	DWORD		SizeOfLevel();
	DWORD		TotalPages();
	BOOL		MakeByteArray(PBYTE * pArray, LPDWORD lpdwCount);
	BOOL		IsLinkedLevel();
	DWORD		LinkLevel();
	const NES_LEVEL_ITEM LevelData(UINT itemIndex);
	DWORD		LevelHeader();
	DWORD		AddRef();
	DWORD		Release();
	BOOL		MakeJarLevel();
};

class CNesLevel: public CNesLevelContainer
{

};
 