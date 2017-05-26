
typedef enum _tagNES_ITEM_TYPE
{
	nitEnemy		= 0,
	nitRegular		= 1,
	nitGround		= 2,
	nitEntrance		= 3,
	nitPointer		= 4
} NES_ITEM_TYPE, *PNES_ITEM_TYPE;

class CFileLoader;
class CNesItem;

class CNesPage: public CNesItemsList<CNesItem>
{
	DWORD		m_nPage;

public:
	VOID		Page(DWORD i);
	DWORD		Page();
	BOOL		InsertObject(CNesItem * pObj);
	BOOL		SortObjects();
	BOOL		SortGroundsHorz();
	BOOL		SortGroundsVert();

	CNesItem *	Item(DWORD i);
};

class CNesObjectsList: public CNesItemsList<NES_LEVEL_ITEM>
{

public:
	BOOL				InsertObject(NES_LEVEL_ITEM * pItem);
	BOOL				InsertSkipper(BOOL fDoubleSkipper);
	PNES_LEVEL_ITEM		InsertNewObject();
	PNES_LEVEL_ITEM		Object(DWORD i);
	BOOL				TrimSkippers();
	~CNesObjectsList();
};

class CNesLayer: public CNesItemsList<CNesPage>
{
	BOOL		m_fHorzLevel;
	DWORD		m_nLayer;
	BOOL		InsertPage(CNesPage * pPage);

public:
	CNesLayer(BOOL fHorz) : m_fHorzLevel( fHorz ) { }
	~CNesLayer();

	BOOL		SortObjects();
	VOID		Layer(DWORD i);
	DWORD		Layer();
	BOOL		SetPagesCount(DWORD c);
	BOOL		InsertObjectToPage(DWORD i, CNesItem * pObject);
	CNesPage*	Page(DWORD i);
};

class CNesLayerArray: public CNesItemsList<CNesLayer>
{

public:
	~CNesLayerArray();
	BOOL		SetLayersCount(DWORD c, BOOL fHorz);
	CNesLayer *	Layer(DWORD i);
};

class CNesObjectsListsArray: public CNesItemsList<CNesObjectsList>
{

public:
	~CNesObjectsListsArray();
	BOOL				SetListsCount(DWORD c);
	CNesObjectsList *	ObjList(DWORD i);
};

class CNesEditor: public CNesItemsList<CNesItem>
{
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

	CNesItem		*pInitialBg;

	CNesLevel &		m_lvl;
	CNesEnemyData & m_enm;
	CFileLoader &	m_file;

	CFileLoader *	m_pFile;

	BOOL			m_fChanged;

	struct
	{
		INT			iWorld;
		INT			iSubLevel;
		DWORD		dwLevel;
	} m_Level;

	struct
	{
		DWORD		dwPages;
		DWORD		dwLayers;
	} m_Counters;

	struct
	{
		DWORD		dwLevelLink;
		DWORD		dwEnemyLink;
	} m_Links;

	BOOL		CheckGroundCollision(UINT pos, INT iLayer, DWORD i = -1);
	BOOL		AssembleHeader();
	BOOL		LoadLevel(CNesLevel & lvl, CNesEnemyData & enm, CFileLoader * file, DWORD dwCurLevel);
	BOOL		MakeObjectsList(DWORD & lpdwCount, std::vector<NES_LEVEL_ITEM> & pItemsList, DWORD & lpdwHeader);
	BOOL		MakeEnemiesList(DWORD & lpdwCount, std::vector<NES_LEVEL_ENEMY> & pItemsList);
	INT			FetchCurWorld();
	VOID		Modified();

public:
	CNesEditor(CNesLevel & lvl, CNesEnemyData & enm, CFileLoader & file, DWORD dwCurLevel);
	~CNesEditor();

	BOOL		RewritePoints();
	DWORD		CountPages();
	DWORD		CountLayers();

	INT			GetCurWorld();
	INT			GetSubLevel();
	DWORD		GetCurLevel();
	BOOL		UpdateCounters();
	CNesItem	LevelItem(UINT i);	
	BYTE		operator[](NES_LEVEL_FIELDS nlf);

	BOOL		Apply();

	BOOL		CheckPagePointers(INT iPage);
	BOOL		InsertObject(INT iLayer, UINT x, UINT y, BYTE regular_id);
	BOOL		InsertEnemy(UINT x, UINT y, BYTE regular_id, BOOL fHidden);
	BOOL		InsertGround(UINT pos, BYTE bGroundSet, BYTE bGroundType, INT iLayer);
	BOOL		InsertPointer(INT iPage, BYTE param1, BYTE param2);
	BOOL		MoveGround(UINT i, UINT pos);
	BOOL		ChangeLayer(UINT i, INT iLayer);
	BOOL		MoveObject(UINT i, UINT x, UINT y);
	BOOL		DeleteObject(UINT i);
	BOOL		MoveEnemy(UINT i, UINT x, UINT y, BOOL fHidden);
	BOOL		InsertEntrance(INT iLayer, UINT x, UINT y, BYTE regular_id, BOOL fFarPointer, BYTE wParam, BYTE lParam);
	BOOL		ChangeEntrancePointer(UINT i, BOOL fFarPointer, BYTE wParam, BYTE lParam);
	BOOL		ChangePointer(UINT i, BYTE wParam, BYTE lParam);
	BOOL		ChangeRegularSize(UINT i, BYTE wSize);
	BOOL		ChangeGround(UINT i, BYTE bSet, BYTE bType);
	BOOL		SetHeaderInfo(NES_LEVEL_FIELDS nlf, BYTE bNewValue);
	BOOL		RemoveAll();
	BOOL		IsEnemyLayerLocked();
	BOOL		IsItemsLayerLocked();
	DWORD		ItemsLayerLink();
	DWORD		EnemiesLayerLink();
	VOID		SetItemsLayerLink(DWORD dwItemsLink);
	VOID		SetEnemiesLayerLink(DWORD dwEnemiesLink);

	BOOL		MakeJarLevel();
	DWORD		GetBgSet(BYTE bBgSet);
	BYTE		GetBgTile(BYTE bBgSet, BYTE bBgType, INT iWorld = -1);
	BYTE		GetObjTile(BYTE bObjectId);
	INT			GetFX(INT iWorld = -1);
	BOOL		GetColFromPalette(COLORREF *pColArray);
	NES_ITEM_DIM	GetMasvDim(int item, NES_ITEM_DIM nid, USHORT & type);
	NES_ITEM_DIM	GetVertDim(int item, NES_ITEM_DIM nid, USHORT & type);
	NES_ITEM_DIM	GetSingDim(int item, NES_ITEM_DIM nid, USHORT & type);
	NES_ITEM_DIM	GetHorzDim(int item, NES_ITEM_DIM nid, USHORT & type);
	NES_ITEM_DIM	GetEntrDim(int item, NES_ITEM_DIM nid, USHORT & type);

	BOOL			GetLocalLevelItems(std::vector<NES_LEVEL_ITEM> & vItems);
	BOOL			GetCurrentLevelItems(std::vector<NES_LEVEL_ITEM> & vItems);
	BOOL			GetLocalEnemyItems(std::vector<NES_LEVEL_ENEMY> & vItems);
	BOOL			GetCurrentEnemyItems(std::vector<NES_LEVEL_ENEMY> & vItems);
	BOOL			Changed();
};

class CNesItem
{
	NES_ITEM_TYPE	m_type;
	POINT			m_pt;
	SIZE			m_sz;
	INT				m_iLayer;
	INT				m_iPage;

	union
	{
		BYTE		pData[4];

		struct
		{
			BYTE	id;
			BYTE	uItemPtr[3];
		} Regular;
		struct
		{
			BYTE	uSets;
			BYTE	uType;
			BYTE	Reserved[2];
		} Ground;
		struct
		{
			BYTE	uEntranceId;
			BYTE	uEntranceParameter[3];
		} Entrance;
		struct
		{
			BYTE	id;
			BYTE	Reserved[3];
		} General;
		struct
		{
			USHORT	uPtrData;
			BYTE	Reserved[2];
		} Pointer;
		struct
		{
			BYTE	id;
			BYTE	Reserved[3];
		} Enemy;
	} m_obj;

	BOOL			SetLayer(INT iLayer);
	BOOL			SetPosition(UINT x, UINT y);
	BOOL			SetPage(INT iPage);
	BOOL			SetEntranceParam(BOOL fFarPointer, BYTE wParam, BYTE lParam);
	BOOL			SetPtr(BYTE wParam, BYTE lParam);

public:
	CNesItem(PNES_LEVEL_ITEM lvl, POINT & pt, INT iLayer, INT iPage);						// regular or entrance
	CNesItem(PNES_LEVEL_ENEMY enm, POINT & pt, INT iPage);									// enemy
	CNesItem(PNES_LEVEL_ITEM lvl, POINT & pt, INT iLayer, INT iPage, BYTE bgType);			// ground
	CNesItem(PNES_LEVEL_ITEM lvl, INT iPage);									// pointer
	BOOL			ChangeBgType(BYTE bgType);

//	friend BOOL CNesEditor::LoadLevel(CNesLevel & lvl, CNesEnemyData & enm, CFileLoader * file, DWORD dwCurLevel);
	friend BOOL CNesEditor::ChangeLayer(UINT i, INT iLayer);
	friend BOOL CNesEditor::MoveObject(UINT i, UINT x, UINT y);
	friend BOOL CNesEditor::MoveEnemy(UINT i, UINT x, UINT y, BOOL fHidden);
	friend BOOL CNesEditor::MoveGround(UINT i, UINT pos);
	friend BOOL CNesEditor::DeleteObject(UINT i);
	friend BOOL CNesEditor::ChangeEntrancePointer(UINT i, BOOL fFarPointer, BYTE wParam, BYTE lParam);
	friend BOOL CNesEditor::ChangePointer(UINT i, BYTE wParam, BYTE lParam);
	friend BOOL CNesEditor::SetHeaderInfo(NES_LEVEL_FIELDS nlf, BYTE bNewValue);
	friend BOOL CNesEditor::RewritePoints();
	friend BOOL CNesEditor::ChangeRegularSize(UINT i, BYTE wSize);
	friend BOOL CNesEditor::ChangeGround(UINT i, BYTE bSet, BYTE bType);
	NES_ITEM_TYPE		Type();
	USHORT				Ground();
	BYTE				Regular();
	BYTE				RegularId();
	BYTE				Enemy();
	POINT				Pos();
	USHORT				Pointer();
	INT					Page();
	INT					Layer();
	BYTE				EntranceParameter(INT i);

};

