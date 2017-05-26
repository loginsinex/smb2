
extern const LPTSTR g_pszEnemies[];

typedef struct _tagNES_LEVEL_ENEMY
{
	union
	{
		BYTE		pData[2];
		struct
		{
			BYTE	id;
			BYTE	y:4;
			BYTE	x:4;
		} Object;
	} Enemy;
	POINT			pt;
	INT				iPage;
} NES_LEVEL_ENEMY, *PNES_LEVEL_ENEMY;

class CNesEnemyData: public CNesItemsList<NES_LEVEL_ENEMY>
{
	DWORD			m_dwPages;
	DWORD			m_dwLinkedToLevel;
	DWORD			m_dwSizeOfEnemies;
	
	VOID			StoreSizeOfEnemies();
	BOOL			FlushObjects();
	BOOL			RereadPoints();
	DWORD			ItemsFromPage(INT iPage, PNES_LEVEL_ENEMY nItems, DWORD dwCount);

public:
	CNesEnemyData();
	~CNesEnemyData();

	BOOL			LoadData(PBYTE ptr, int size, DWORD dwLinkedLevel = -1);
	BOOL			LoadEnemyData(std::vector<NES_LEVEL_ENEMY> & pItemsList, DWORD dwCount, DWORD dwPagesCount);
	NES_LEVEL_ENEMY	LevelEnemy(DWORD index);
	DWORD			SizeOfEnemies();
	DWORD			CountPages();
	BOOL			MakeByteArray(PBYTE * pArray, LPDWORD lpdwCount);
	BOOL			IsLinkedLevel();
	DWORD			LinkLevel();
	DWORD			AddRef();
	DWORD			Release();

};
