
#define		LOAD_PTR_NC(w)		(0x0000c02c+0x94*w)
#define		LOAD_PTR_EC(w)		(0x0000c09c+0x94*w)
#define		MAX_PTR_NC			(LOAD_PTR_NC(7) + 128)
#define		MAX_PTR_EC			(LOAD_PTR_EC(7) + 48)
#define		MAX_LEVELS			210

#define		NES_PTR_LEVEL_STARTS	0x0001F768

#define		NES_PTR_LEVELS1			0x0001F770
#define		NES_PTR_LEVELS2			0x0001F775

#define		NES_PTR_ENEMY1			0x0001F791
#define		NES_PTR_ENEMY2			0x0001F796
#define		NES_PTR_ENEMY3			0x0001F79B
#define		NES_PTR_ENEMY4			0x0001F7A0
#define		NES_PTR_START			0x00010010
#define		NES_PTR_EOF				0x00014010

extern const LPTSTR		g_szLevelName[];

class CFileLoader
{
	struct
	{
		CString						sFilename;
		std::vector<BYTE>			vFile;
		BOOL						fModified;
	} m_File;

	struct
	{
		BYTE						nLevel[MAX_LEVELS];
		BYTE						nEnemy[MAX_LEVELS];
		BOOL						fLoaded;
	} m_game;

	std::vector<CNesEditor*>		m_vEditors;

	struct
	{
		std::vector<CNesLevel*>			vLevel;
		std::vector<CNesEnemyData*>		vEnemy;
		std::vector<BYTE>				vStarts;
	} m_data;

	struct
	{
		DWORD						ptrLevelStarts;
		DWORD						ptrLevels[2];
		DWORD						ptrEnemies[4];
	} m_ptr;

	void							Flush();

public:
	CFileLoader();
	~CFileLoader();
	BOOL			IsLoaded();
	int				LoadFile(LPCTSTR pszFilename);
	int				SaveFile(LPCTSTR pszFilename = NULL);
	CNesEditor &	getLevelEditor(UINT level);

	int				Read();
	int				Write();

	BOOL			LoadData(PBYTE pArray, DWORD ptr, INT iSizeOfArray);
	BOOL			WriteData(PBYTE pArray, DWORD ptr, INT iSizeOfArray);

	USHORT			Short(DWORD ptr);
	VOID			Short(DWORD ptr, USHORT val);
	BYTE			Byte(DWORD ptr);
	VOID			Byte(DWORD ptr, BYTE val);
	DWORD			Word(DWORD ptr);
	VOID			Word(DWORD ptr, DWORD val);

	BOOL			GetMemoryStatus(DWORD & systemData, DWORD & levelData, DWORD & enemyData, DWORD & occupiedSpace, DWORD & totalSpace);
	BOOL			GetGameSets(std::vector<int> & vLevelData, std::vector<BYTE> & vRoomsLevelsList,
									std::vector<int> & vEnemyData, std::vector<BYTE> & vRoomsEnemiesList);
	BOOL			SetGameSets(std::vector<int> & vLevelData, std::vector<BYTE> & vRoomsLevelsList,
									std::vector<int> & vEnemyData, std::vector<BYTE> & vRoomsEnemiesList);
};

/*
class CFileLoader
{
	BYTE			*pFile;
	BOOL			fLoaded;
	BOOL			fModified;
	DWORD			dwSize;
	BYTE			bZero;
	CNesTiles		m_nTiles;

public:
	struct
	{
		BYTE			nc[64][16];
		BYTE			ec[32][12];

		struct
		{
			DWORD			levelPtr;
			DWORD			enemyPtr;
			DWORD			levelBits;
			DWORD			levelSize;
			DWORD			enemySize;
			DWORD			dwDuplicate;
			DWORD			dwDupEnemies;
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
			} levelFields;
			CNesLevel		vLevel;
			CNesEnemyData	vEnemy;
			CNesEditor		vEditor;
			/ * level bits:
				0 - Horizontal scroll
				1-4 - Reserved
				5-7 - level colors
				8-11 - Reserved
				12-13 - Enemy colors
				14-16 - Type of ground
				17-18 - Level music
				19-23 - Ground
				24-27 - Level length
				28-31 - Object type
			* /
		} level[210];
	} m_gameData;

private:
	BOOL			CheckFile();
	BOOL			ReadFile();
	BOOL			WriteFile();

	PBYTE			MakeLevelList(LPDWORD lpdwSize);
	PBYTE			MakeEnemiesList(LPDWORD lpdwSize);
	DWORD			getEnemyLevelSize(DWORD dwLevel);
	DWORD			getItemLevelSize(DWORD dwLevel);

public:
	CFileLoader();
	~CFileLoader();
	VOID			LoadFile(HANDLE hFile);
	BOOL			SaveFile(HANDLE hFile);
	BOOL			IsLoaded();
	BOOL			IsModified();
	BYTE			levelValue(UINT level, UINT index);
	VOID			levelValue(UINT level, UINT index, BYTE data);
	CNesLevel &		getLevelItem(UINT level);
	CNesEnemyData &	getEnemyItem(UINT level);
	CNesEditor &	getLevelEditor(UINT level);
	BOOL			LoadData(PBYTE pArray, DWORD ptr, INT iSizeOfArray);
	BOOL			WriteData(PBYTE pArray, DWORD ptr, INT iSizeOfArray);
};
*/