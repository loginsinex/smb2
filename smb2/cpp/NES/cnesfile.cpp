
#include "precomp.h"

const LPTSTR g_szLevelName[] =
{
 TEXT("1-1, beginning"),
 TEXT("1-1, upwards part"),
 TEXT("1-1, main part"),
 TEXT("1-1, underground (top)"),
 TEXT("1-1, underground (bottom)"),
 TEXT("1-1, boss area"),
 TEXT("1-1, unused (1)"),
 TEXT("1-1, unused (2)"),
 TEXT("1-1, unused (3)"),
 TEXT("1-1, unused (4)"),
 TEXT("1-2, main part"),
 TEXT("1-2, underground"),
 TEXT("1-2, boss area"),
 TEXT("Jar 1, found in 1-2"),
 TEXT("Jar 2, found in 1-2"),
 TEXT("1-2, unused (1)"),
 TEXT("1-2, unused (2)"),
 TEXT("1-2, unused (3)"),
 TEXT("1-2, unused (4)"),
 TEXT("1-2, unused (5)"),
 TEXT("1-3, main part"),
 TEXT("1-3, key room"),
 TEXT("1-3, indoors"),
 TEXT("1-3, way to the boss"),
 TEXT("1-3, boss room"),
 TEXT("1-3, unused (1)"),
 TEXT("1-3, unused (2)"),
 TEXT("1-3, unused (3)"),
 TEXT("1-3, unused (4)"),
 TEXT("1-3, unused (5)"),
 TEXT("2-1, main part"),
 TEXT("2-1, underground"),
 TEXT("2-1, boss area"),
 TEXT("2-1, unused (1)"),
 TEXT("Jar 3, found in 2-1"),
 TEXT("2-1, unused (2)"),
 TEXT("2-1, unused (3)"),
 TEXT("2-1, unused (4)"),
 TEXT("2-1, unused (5)"),
 TEXT("2-1, unused (6)"),
 TEXT("2-2, beginning"),
 TEXT("2-2, main part"),
 TEXT("2-2, underground bonus"),
 TEXT("2-2, underground"),
 TEXT("Jar 4, found in 2-2"),
 TEXT("2-2, boss area"),
 TEXT("2-2, unused (1)"),
 TEXT("2-2, unused (2)"),
 TEXT("2-2, unused (3)"),
 TEXT("2-2, unused (4)"),
 TEXT("2-3, beginning"),
 TEXT("2-3, main part"),
 TEXT("2-3, bonus room"),
 TEXT("2-3, underground part"),
 TEXT("Jar 5, found in 2-3"),
 TEXT("2-3, way to the boss"),
 TEXT("2-3, boss room"),
 TEXT("2-3, key room"),
 TEXT("2-3, unused (1)"),
 TEXT("2-3, unused (2)"),
 TEXT("3-1, beginning"),
 TEXT("3-1, main part"),
 TEXT("3-1, bonus room"),
 TEXT("3-1, sky part"),
 TEXT("3-1, boss area"),
 TEXT("3-1, unused (1)"),
 TEXT("3-1, unused (2)"),
 TEXT("3-1, unused (3)"),
 TEXT("3-1, unused (4)"),
 TEXT("3-1, unused (5)"),
 TEXT("3-2, main part"),
 TEXT("3-2, underground"),
 TEXT("3-2, boss area"),
 TEXT("3-2, unused (1)"),
 TEXT("3-2, unused (2)"),
 TEXT("3-2, unused (3)"),
 TEXT("3-2, unused (4)"),
 TEXT("3-2, unused (5)"),
 TEXT("3-2, unused (6)"),
 TEXT("3-2, unused (7)"),
 TEXT("3-3, beginning"),
 TEXT("3-3, outside part"),
 TEXT("3-3, indoors part 1"),
 TEXT("3-3, indoors part 2"),
 TEXT("3-3, indoors part 3"),
 TEXT("3-3, indoors part 4"),
 TEXT("3-3, key room"),
 TEXT("3-3, way to the boss"),
 TEXT("3-3, boss room"),
 TEXT("3-3, unused (1)"),
 TEXT("4-1, main part"),
 TEXT("4-1, ending"),
 TEXT("4-1, unused (1)"),
 TEXT("4-1, unused (2)"),
 TEXT("4-1, unused (3)"),
 TEXT("4-1, unused (4)"),
 TEXT("4-1, unused (5)"),
 TEXT("4-1, unused (6)"),
 TEXT("4-1, unused (7)"),
 TEXT("4-1, unused (8)"),
 TEXT("4-2, beginning"),
 TEXT("4-2, part 1"),
 TEXT("4-2, part 2"),
 TEXT("4-2, way to the boss"),
 TEXT("4-2, boss area"),
 TEXT("4-2, unused (1)"),
 TEXT("4-2, unused (2)"),
 TEXT("4-2, unused (3)"),
 TEXT("4-2, unused (4)"),
 TEXT("4-2, unused (5)"),
 TEXT("4-3, beginning"),
 TEXT("4-3, outside part 1"),
 TEXT("4-3, indoors part 1"),
 TEXT("4-3, indoors part 2"),
 TEXT("4-3, outside part 2"),
 TEXT("4-3, key room"),
 TEXT("4-3, way to the boss"),
 TEXT("4-3, boss room"),
 TEXT("4-3, unused (1)"),
 TEXT("4-3, unused (2)"),
 TEXT("5-1, beginning"),
 TEXT("5-1, underground"),
 TEXT("5-1, boss area"),
 TEXT("5-1, unused (1)"),
 TEXT("5-1, unused (2)"),
 TEXT("5-1, unused (3)"),
 TEXT("5-1, unused (4)"),
 TEXT("5-1, unused (5)"),
 TEXT("5-1, unused (6)"),
 TEXT("5-1, unused (7)"),
 TEXT("5-2, beginning"),
 TEXT("5-2, part 1"),
 TEXT("5-2, part 2"),
 TEXT("5-2, part 3"),
 TEXT("Jar 6, found in 5-2"),
 TEXT("5-2, boss area"),
 TEXT("5-2, unused (1)"),
 TEXT("5-2, unused (2)"),
 TEXT("5-2, unused (3)"),
 TEXT("5-2, unused (4)"),
 TEXT("5-3, beginning"),
 TEXT("5-3, main part"),
 TEXT("5-3, underground 1"),
 TEXT("5-3, underground 2"),
 TEXT("5-3, way to the boss"),
 TEXT("5-3, boss room"),
 TEXT("5-3, unused (1)"),
 TEXT("5-3, unused (2)"),
 TEXT("5-3, unused (3)"),
 TEXT("5-3, unused (4)"),
 TEXT("6-1, main part"),
 TEXT("6-1, underground"),
 TEXT("6-1, boss area"),
 TEXT("Jar 7, found in 6-1"),
 TEXT("Jar 8, found in 6-1"),
 TEXT("Jar 9, found in 6-1"),
 TEXT("Jar 10, found in 6-1"),
 TEXT("6-1, unused (1)"),
 TEXT("6-1, unused (2)"),
 TEXT("6-1, unused (3)"),
 TEXT("6-2, beginning"),
 TEXT("6-2, main part"),
 TEXT("6-2, boss area"),
 TEXT("6-2, unused (1)"),
 TEXT("6-2, unused (2)"),
 TEXT("6-2, unused (3)"),
 TEXT("6-2, unused (4)"),
 TEXT("6-2, unused (5)"),
 TEXT("6-2, unused (6)"),
 TEXT("6-2, unused (7)"),
 TEXT("6-3, beginning"),
 TEXT("6-3, main part"),
 TEXT("6-3, underground"),
 TEXT("6-3, upwards part"),
 TEXT("6-3, boss room entrance"),
 TEXT("6-3, boss area"),
 TEXT("6-3, boss room"),
 TEXT("6-3, unused (1)"),
 TEXT("6-3, unused (2)"),
 TEXT("6-3, unused (3)"),
 TEXT("7-1, beginning"),
 TEXT("7-1, part 1"),
 TEXT("7-1, part 2"),
 TEXT("7-1, part 3"),
 TEXT("7-1, boss area"),
 TEXT("7-1, bonus room"),
 TEXT("7-1, unused (1)"),
 TEXT("7-1, unused (2)"),
 TEXT("7-1, unused (3)"),
 TEXT("7-1, unused (4)"),
 TEXT("7-2, beginning"),
 TEXT("7-2, indoors part 1"),
 TEXT("7-2, indoors part 2"),
 TEXT("7-2, indoors part 3"),
 TEXT("7-2, indoors part 4"),
 TEXT("7-2, Wart's room"),
 TEXT("7-2, indoors part 5"),
 TEXT("7-2, indoors part 6"),
 TEXT("7-2, indoors part 7"),
 TEXT("7-2, outside part"),
 TEXT("7-3, unused (1)"),
 TEXT("7-3, unused (2)"),
 TEXT("7-3, unused (3)"),
 TEXT("7-3, unused (4)"),
 TEXT("7-3, unused (5)"),
 TEXT("7-3, unused (6)"),
 TEXT("7-3, unused (7)"),
 TEXT("7-3, unused (8)"),
 TEXT("7-3, unused (9)"),
 TEXT("7-3, unused (10)"),
};


CFileLoader::CFileLoader()
{
	m_File.fModified = FALSE;
	Flush();
}

CFileLoader::~CFileLoader()
{
	Flush();
}

void CFileLoader::Flush()
{
	ZeroMemory(&m_ptr, sizeof(m_ptr));
	m_data.vEnemy.clear();
	m_data.vLevel.clear();
	ZeroMemory(&m_game, sizeof(m_game));

	int c = m_data.vLevel.size();
	for(int i = 0; i < c; ++i)
		delete m_data.vLevel[i];

	m_data.vLevel.clear();

	c = m_data.vEnemy.size();
	for(int i = 0; i < c; ++i)
		delete m_data.vEnemy[i];

	m_data.vEnemy.clear();

	c = m_vEditors.size();
	for(int i = 0; i < c; ++i)
		delete m_vEditors[i];

	m_vEditors.clear();

	m_data.vStarts.clear();
}

BOOL CFileLoader::IsLoaded()
{
	return ( m_File.vFile.size() > 0 ) && m_game.fLoaded;
}

int CFileLoader::LoadFile(LPCTSTR pszFilename)
{
	if ( !pszFilename )
		return ERROR_FILE_NOT_FOUND;

	HANDLE hFile = CreateFile(pszFilename, 
		GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);

	if ( INVALID_HANDLE_VALUE == hFile )
		return GetLastError();

	int err = 0;
	LARGE_INTEGER sz = { 0 };

	if ( GetFileSizeEx(hFile, &sz) && sz.QuadPart >= 262160 && sz.QuadPart < 284000 )
	{
		DWORD w = { 0 };
		m_File.vFile.clear();
		m_File.vFile.insert(m_File.vFile.end(), sz.LowPart, 0);

		if ( !ReadFile(hFile, &m_File.vFile[0], sz.LowPart, &w, NULL) )
			err = GetLastError();
		else if ( w != sz.LowPart )
			err = ERROR_READ_FAULT;
	}
	else
		err = ERROR_FILE_TOO_LARGE;

	CloseHandle(hFile);

	if ( !err ) 
		m_File.sFilename = pszFilename;
	else
		m_File.vFile.clear();

	return err;
}

int CFileLoader::SaveFile(LPCTSTR pszFilename)
{
	if ( !m_File.vFile.size() )
		return ERROR_FILE_NOT_FOUND;

	if ( !pszFilename )
		pszFilename = m_File.sFilename;

	HANDLE hFile = CreateFile(pszFilename, 
		GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, 0, NULL);

	if ( INVALID_HANDLE_VALUE == hFile )
		return GetLastError();

	int err = 0;
	DWORD w = 0;

	if ( !WriteFile(hFile, &m_File.vFile[0], m_File.vFile.size(), &w, NULL) )
		err = GetLastError();
	else if ( w != m_File.vFile.size() )
		err = ERROR_WRITE_FAULT;

	if ( !err ) 
		m_File.sFilename = pszFilename;
	
	CloseHandle(hFile);

	return err;
}

int CFileLoader::Read()
{
	if ( !m_File.vFile.size() )
		return ERROR_FILE_NOT_FOUND;

	Flush();

	const DWORD DELTA_PTR = 0x8010;
	int err = 0;
	std::map<DWORD, DWORD> mLevels, mEnemies;

	// get main pointers
	m_ptr.ptrLevelStarts = Short(NES_PTR_LEVEL_STARTS);
	m_ptr.ptrLevels[0] = Short(NES_PTR_LEVELS1);
	m_ptr.ptrLevels[1] = Short(NES_PTR_LEVELS2);
	m_ptr.ptrEnemies[0] = Short(NES_PTR_ENEMY1);
	m_ptr.ptrEnemies[1] = Short(NES_PTR_ENEMY2);
	m_ptr.ptrEnemies[2] = Short(NES_PTR_ENEMY3);
	m_ptr.ptrEnemies[3] = Short(NES_PTR_ENEMY4);

	// load levels
	if ( m_ptr.ptrLevels[1] - m_ptr.ptrLevels[0] != MAX_LEVELS 
		|| m_ptr.ptrLevelStarts + DELTA_PTR + MAX_LEVELS / 10 > m_File.vFile.size() )
	{
		Flush();
		return ERROR_FILE_CORRUPT;
	}

	m_data.vStarts.insert(m_data.vStarts.end(), 
		m_File.vFile.begin() + (m_ptr.ptrLevelStarts + DELTA_PTR), 
		m_File.vFile.begin() + (m_ptr.ptrLevelStarts + DELTA_PTR + MAX_LEVELS / 10));

	for(int i = 0; i < MAX_LEVELS; ++i)
	{
		DWORD levelPtr = 
			MAKEWORD(
				m_File.vFile[i + m_ptr.ptrLevels[0] + DELTA_PTR],
				m_File.vFile[i + m_ptr.ptrLevels[1] + DELTA_PTR]
				);

		if ( levelPtr + DELTA_PTR >= m_File.vFile.size() )
		{
			err = ERROR_FILE_CORRUPT;
			break;
		}

		if ( mLevels.find(levelPtr) == mLevels.end() )
		{
			CNesLevel * plv = new CNesLevel();
			plv->LoadLevel(&m_File.vFile[levelPtr + DELTA_PTR]);
			m_data.vLevel.push_back(plv);
			mLevels.insert(std::make_pair(levelPtr, m_data.vLevel.size()));
		}

		m_game.nLevel[i] = LOBYTE(LOWORD(mLevels.find(levelPtr)->second));
	}

	DWORD ptrsList[MAX_LEVELS] = { 0 };

	for(int lv = 0; lv < MAX_LEVELS / 10; ++lv)
	{
		DWORD levelPtrHi = 
			MAKEWORD(
				m_File.vFile[lv + m_ptr.ptrEnemies[1] + DELTA_PTR],
				m_File.vFile[lv + m_ptr.ptrEnemies[0] + DELTA_PTR]
			),
			  levelPtrLo = 
			MAKEWORD(
				m_File.vFile[lv + m_ptr.ptrEnemies[3] + DELTA_PTR],
				m_File.vFile[lv + m_ptr.ptrEnemies[2] + DELTA_PTR]
			);

		if ( levelPtrHi + DELTA_PTR >= m_File.vFile.size() || levelPtrLo + DELTA_PTR >= m_File.vFile.size() )
		{
			err = ERROR_FILE_CORRUPT;
			break;
		}

		for(int i = 0; i < 10; ++i)
		{
			DWORD levelPtr = 
				MAKEWORD(
					m_File.vFile[i + levelPtrLo + DELTA_PTR],
					m_File.vFile[i + levelPtrHi + DELTA_PTR]
				);

			if ( levelPtr + DELTA_PTR >= m_File.vFile.size() )
				continue;

			if ( mEnemies.find(levelPtr) == mEnemies.end() )
			{
				CNesEnemyData * ped = new CNesEnemyData;
				m_data.vEnemy.push_back(ped);
				mEnemies.insert(std::make_pair(levelPtr, m_data.vEnemy.size()));
			}

			m_game.nEnemy[10 * lv + i] = LOBYTE(LOWORD(mEnemies[levelPtr]));
		}
	}

	if ( !err )
	{
		// due to enemies data has no special marker for end data, 
		// we shall calculate sizes manualy, and load enemies after 'for'
		// also note that std::map is ALREADY sorted by the key value,
		// so sizes of enemies data will be actually calculated between neighbors arrays
		for(std::map<DWORD, DWORD>::iterator m = mEnemies.begin(); m != mEnemies.end(); ++m)
		{
			std::map<DWORD, DWORD>::iterator next = m;
			next++;

			if ( next != mEnemies.end() )
			{
				m_data.vEnemy[m->second - 1]->LoadData( &m_File.vFile[m->first + DELTA_PTR], 
					next->first - m->first);
			}
			else
			{
				// try to find 'FF' marker, or EOF data
				DWORD lastPtr = m->first + DELTA_PTR, eofPtr = min(NES_PTR_EOF, m_File.vFile.size());
				while(lastPtr < eofPtr && 0xFF != m_File.vFile[lastPtr])
					lastPtr++;

				m_data.vEnemy[m->second - 1]->LoadData( &m_File.vFile[m->first + DELTA_PTR],
					( lastPtr - DELTA_PTR ) - m->first - 1);
			}
		}

		// now load editors
		for(int i = 0; i < MAX_LEVELS; ++i)
		{
			CNesEditor * pEditor = new CNesEditor(
				*m_data.vLevel[m_game.nLevel[i] - 1], *m_data.vEnemy[m_game.nEnemy[i] - 1],
				*this, i
				);
			m_vEditors.push_back(pEditor);
		}

		m_game.fLoaded = TRUE;
	}
	else if ( err )
		Flush();

	return err;
}

int CFileLoader::Write()
{
	if ( !IsLoaded() )
		return ERROR_FILE_NOT_FOUND;

	if ( !m_data.vStarts.size() || !m_data.vEnemy.size() || !m_data.vLevel.size() )
		return ERROR_NOINTERFACE;

	int err = 0;
	std::vector<BYTE> file = m_File.vFile;

	DWORD systemData = 0, levelData = 0, enemyData = 0, occupiedSpace = 0, totalSpace = 0;
	
	// first, apply modifications to levels
	for(int i = 0; i < MAX_LEVELS; ++i)
		if ( m_vEditors[i]->Changed() )
			m_vEditors[i]->Apply();

	if ( !GetMemoryStatus(systemData, levelData, enemyData, occupiedSpace, totalSpace) || occupiedSpace >= totalSpace 
		|| file.size() < NES_PTR_EOF )
		return ERROR_NOT_ENOUGH_MEMORY;

	const DWORD DELTA_PTR = 0x8010;

	std::vector<DWORD> vLevels, vEnemies;

	m_ptr.ptrLevelStarts = NES_PTR_START - DELTA_PTR;
	CopyMemory(&file[NES_PTR_START], &m_data.vStarts[0], m_data.vStarts.size());

	m_ptr.ptrLevels[0] = m_ptr.ptrLevelStarts + m_data.vStarts.size();
	m_ptr.ptrLevels[1] = m_ptr.ptrLevels[0] + MAX_LEVELS;

	// write levels banks
	DWORD ptr = m_ptr.ptrLevels[1] + MAX_LEVELS;
	int c = m_data.vLevel.size();
	for(int i = 0; i < c; ++i)
	{
		vLevels.push_back(ptr);

		PBYTE pArray = NULL;
		DWORD dwSize = 0;
		if ( m_data.vLevel[i]->MakeByteArray(&pArray, &dwSize) &&
			ptr + dwSize + DELTA_PTR < NES_PTR_EOF )
		{
			CopyMemory(&file[ptr + DELTA_PTR], pArray, dwSize);
			ptr += dwSize;
			delete[] pArray;
		}
		else
			return ERROR_NOT_ENOUGH_MEMORY;
	}

	// write level pointers
	for(int i = 0; i < MAX_LEVELS; ++i)
	{
		file[m_ptr.ptrLevels[1] + DELTA_PTR + i] = HIBYTE(LOWORD(vLevels[m_game.nLevel[i] - 1]));
		file[m_ptr.ptrLevels[0] + DELTA_PTR + i] = LOBYTE(LOWORD(vLevels[m_game.nLevel[i] - 1]));
	}

	// now update place of enemies data
	// note, that we already checked, that we have enough space to store all data

	// enemies pointers arrays
	DWORD dwEnmPtrsSize = MAX_LEVELS * 2 + ( MAX_LEVELS / 10 ) * 4;
	DWORD eptr = max(ptr, m_ptr.ptrEnemies[0]);

	if ( eptr + dwEnmPtrsSize + enemyData + DELTA_PTR > NES_PTR_EOF )	// move data backward
		eptr = NES_PTR_EOF - DELTA_PTR - enemyData - dwEnmPtrsSize;
	
	if ( eptr < ptr ) // cant rewrite level data, but it shall be always false
		return ERROR_NOT_ENOUGH_MEMORY;
	
	if ( ptr < eptr )	// fill free space with 'ff'
		for(DWORD p = ptr + DELTA_PTR; p < eptr + DELTA_PTR; ++p)
			file[p] = 0xFF;
	
	m_ptr.ptrEnemies[0] = eptr;
	m_ptr.ptrEnemies[1] = m_ptr.ptrEnemies[0] + MAX_LEVELS / 10;
	m_ptr.ptrEnemies[2] = m_ptr.ptrEnemies[1] + MAX_LEVELS / 10;
	m_ptr.ptrEnemies[3] = m_ptr.ptrEnemies[2] + MAX_LEVELS / 10;
	eptr = m_ptr.ptrEnemies[3] + MAX_LEVELS / 10;
	ptr = eptr + MAX_LEVELS * 2;
	// now 'eptr' points to start of array ptrs, 
	// and 'ptr' points to data
	
	c = m_data.vEnemy.size();
	for(int i = 0; i < c; ++i)
	{
		vEnemies.push_back(ptr);
		
		PBYTE pArray = NULL;
		DWORD dwSize = 0;
		if ( m_data.vEnemy[i]->MakeByteArray(&pArray, &dwSize) &&
			ptr + dwSize < NES_PTR_EOF )
		{
			CopyMemory(&file[ptr + DELTA_PTR], pArray, dwSize);
			delete[] pArray;

			ptr += dwSize;
		}
		else
			return ERROR_NOT_ENOUGH_MEMORY;
	}

	for(; ptr < NES_PTR_EOF - DELTA_PTR; ++ptr)
		file[ptr + DELTA_PTR] = 0xFF;

	// write enemies pointers
	for(int lv = 0; lv < MAX_LEVELS / 10; ++lv)
	{
		DWORD levelPtrHi = eptr + 20 * lv, levelPtrLo = eptr + 20 * lv + 10;
		file[m_ptr.ptrEnemies[0] + lv + DELTA_PTR] = HIBYTE(LOWORD(levelPtrHi));
		file[m_ptr.ptrEnemies[1] + lv + DELTA_PTR] = LOBYTE(LOWORD(levelPtrHi));
		file[m_ptr.ptrEnemies[2] + lv + DELTA_PTR] = HIBYTE(LOWORD(levelPtrLo));
		file[m_ptr.ptrEnemies[3] + lv + DELTA_PTR] = LOBYTE(LOWORD(levelPtrLo));

		for(int i = 0; i < 10; ++i)
		{
			file[levelPtrHi + DELTA_PTR + i] = HIBYTE(LOWORD(vEnemies[m_game.nEnemy[10 * lv + i] - 1]));
			file[levelPtrLo + DELTA_PTR + i] = LOBYTE(LOWORD(vEnemies[m_game.nEnemy[10 * lv + i] - 1]));
		}
	}

	// write from copy to a file array
	m_File.vFile = file;

	// write main pointers

	Short(NES_PTR_LEVEL_STARTS, LOWORD(m_ptr.ptrLevelStarts));
	Short(NES_PTR_LEVELS1, LOWORD(m_ptr.ptrLevels[0]));
	Short(NES_PTR_LEVELS2, LOWORD(m_ptr.ptrLevels[1]));
	Short(NES_PTR_ENEMY1, LOWORD(m_ptr.ptrEnemies[0]));
	Short(NES_PTR_ENEMY2, LOWORD(m_ptr.ptrEnemies[1]));
	Short(NES_PTR_ENEMY3, LOWORD(m_ptr.ptrEnemies[2]));
	Short(NES_PTR_ENEMY4, LOWORD(m_ptr.ptrEnemies[3]));

	return err;
}

CNesEditor & CFileLoader::getLevelEditor(UINT level)
{
	if ( !m_File.vFile.size() || level >= MAX_LEVELS )
		throw CString(TEXT("No such level"));

	return *m_vEditors[level];
}

BOOL CFileLoader::LoadData(PBYTE pArray, DWORD ptr, INT iSizeOfArray)
{
	if ( !m_File.vFile.size() || IsBadWritePtr(pArray, iSizeOfArray) 
			|| ptr + DWORD(iSizeOfArray) > m_File.vFile.size() )
		return FALSE;

	CopyMemory(pArray, &m_File.vFile[ptr], iSizeOfArray);
	return TRUE;
}

BOOL CFileLoader::WriteData(PBYTE pArray, DWORD ptr, INT iSizeOfArray)
{
	if ( !m_File.vFile.size() || IsBadReadPtr(pArray, iSizeOfArray) 
			|| ptr + DWORD(iSizeOfArray) > m_File.vFile.size() )
		return FALSE;

	CopyMemory(&m_File.vFile[ptr], pArray, iSizeOfArray);
	return TRUE;
}

BOOL CFileLoader::GetMemoryStatus(DWORD & systemData, DWORD & levelData, DWORD & enemyData, DWORD & occupiedSpace, DWORD & totalSpace)
{
	if ( !IsLoaded() )
		return FALSE;

	const DWORD DELTA_PTR = 0x8010;

	totalSpace = ( NES_PTR_EOF - NES_PTR_START );
	systemData = 
		MAX_LEVELS * 2 + ( MAX_LEVELS / 10 ) * 4 +			// enemies ptr array + 2 enemies ptrs to ptrs array
		MAX_LEVELS * 2 +									// levels ptr array
		MAX_LEVELS / 10;									// levels starts ptrs

	levelData = 0;
	int c = m_data.vLevel.size();
	for(int i = 0; i < c; ++i)
		levelData += m_data.vLevel[i]->SizeOfLevel() + 1;

	enemyData = 0;
	c = m_data.vEnemy.size();
	for(int i = 0; i < c; ++i)
		enemyData += m_data.vEnemy[i]->SizeOfEnemies();

	occupiedSpace = systemData + levelData + enemyData;

	return TRUE;
}

USHORT CFileLoader::Short(DWORD ptr)
{
	if ( !m_File.vFile.size() || ptr + sizeof(USHORT) > m_File.vFile.size() )
		return 0;

	return *((PUSHORT) &m_File.vFile[ptr]);
}

VOID CFileLoader::Short(DWORD ptr, USHORT val)
{
	if ( !m_File.vFile.size() || ptr + sizeof(USHORT) > m_File.vFile.size() )
		return;

	*((PUSHORT) &m_File.vFile[ptr]) = val;
}

BYTE CFileLoader::Byte(DWORD ptr)
{
	if ( !m_File.vFile.size() || ptr >= m_File.vFile.size() )
		return 0;

	return m_File.vFile[ptr];
}

VOID CFileLoader::Byte(DWORD ptr, BYTE val)
{
	if ( !m_File.vFile.size() || ptr >= m_File.vFile.size() )
		return;

	m_File.vFile[ptr] = val;
}

DWORD CFileLoader::Word(DWORD ptr)
{
	if ( !m_File.vFile.size() || ptr + sizeof(DWORD) > m_File.vFile.size() )
		return 0;

	return *((PDWORD) &m_File.vFile[ptr]);
}

VOID CFileLoader::Word(DWORD ptr, DWORD val)
{
	if ( !m_File.vFile.size() || ptr + sizeof(DWORD) > m_File.vFile.size() )
		return;

	*((PDWORD) &m_File.vFile[ptr]) = val;
}

BOOL CFileLoader::GetGameSets(std::vector<int> & vLevelData, std::vector<BYTE> & vRoomsLevelsList,
									std::vector<int> & vEnemyData, std::vector<BYTE> & vRoomsEnemiesList)
{
	if ( !IsLoaded() )
		return FALSE;

	vLevelData.clear();
	vEnemyData.clear();
	vRoomsLevelsList.clear();
	vRoomsEnemiesList.clear();

	int c = m_data.vLevel.size();
	for(int i = 0; i < c; ++i)
		vLevelData.push_back(i);

	c = m_data.vEnemy.size();
	for(int i = 0; i < c; ++i)
		vEnemyData.push_back(i);

	vRoomsLevelsList.insert(vRoomsLevelsList.end(), MAX_LEVELS, 0);
	vRoomsEnemiesList.insert(vRoomsEnemiesList.end(), MAX_LEVELS, 0);
	CopyMemory(&vRoomsLevelsList[0], m_game.nLevel, MAX_LEVELS);
	CopyMemory(&vRoomsEnemiesList[0], m_game.nEnemy, MAX_LEVELS);

	return TRUE;
}

BOOL CFileLoader::SetGameSets(std::vector<int> & vLevelData, std::vector<BYTE> & vRoomsLevelsList,
									std::vector<int> & vEnemyData, std::vector<BYTE> & vRoomsEnemiesList)
{

	if ( vLevelData.size() < m_data.vLevel.size() ||
		vEnemyData.size() < m_data.vEnemy.size() ||
		vRoomsLevelsList.size() != MAX_LEVELS ||
		vRoomsEnemiesList.size() != MAX_LEVELS )
		return FALSE;

	std::vector<CNesLevel*>			vLevel;
	std::vector<CNesEnemyData*>		vEnemy;
	BYTE							nLevel[MAX_LEVELS];
	BYTE							nEnemy[MAX_LEVELS];

	// create new links list for rooms 
	int c = vLevelData.size(), my = m_data.vLevel.size();
	for(int i = 0; i < c; ++i)
	{
		std::vector<BYTE> vLinks;
		for(int k = 0; k < MAX_LEVELS; ++k)
			if ( vRoomsLevelsList[k] == ( i + 1 ) )
				vLinks.push_back(k);

		if ( i < my )
		{
			if ( !vLinks.size() )
			{
				delete m_data.vLevel[i];
				m_data.vLevel[i] = NULL;
			}
			else
			{
				vLevel.push_back(m_data.vLevel[i]);
				int new_id = vLevel.size() - 1;
				int cLinks = vLinks.size();
				for(int k = 0; k < cLinks; ++k)
					nLevel[vLinks[k]] = new_id + 1;
			}
		}
		else if ( vLinks.size() > 0 )
		{
			CNesLevel * lv = new CNesLevel;

			vLevel.push_back(lv);

			int new_id = vLevel.size() - 1;
			int cLinks = vLinks.size();
			for(int k = 0; k < cLinks; ++k)
				nLevel[vLinks[k]] = new_id + 1;

		}
	}

	// create new links list for enemies
	c = vEnemyData.size(), my = m_data.vEnemy.size();
	for(int i = 0; i < c; ++i)
	{
		std::vector<BYTE> vLinks;
		for(int k = 0; k < MAX_LEVELS; ++k)
			if ( vRoomsEnemiesList[k] == ( i + 1 ) )
				vLinks.push_back(k);

		if ( i < my )
		{
			if ( !vLinks.size() )
			{
				delete m_data.vEnemy[i];
				m_data.vEnemy[i] = NULL;
			}
			else
			{
				vEnemy.push_back(m_data.vEnemy[i]);
				int new_id = vEnemy.size() - 1;
				int cLinks = vLinks.size();
				for(int k = 0; k < cLinks; ++k)
					nEnemy[vLinks[k]] = new_id + 1;
			}
		}
		else if ( vLinks.size() > 0 )
		{
			CNesEnemyData * en = new CNesEnemyData;
			vEnemy.push_back(en);

			int new_id = vEnemy.size() - 1;
			int cLinks = vLinks.size();
			for(int k = 0; k < cLinks; ++k)
				nEnemy[vLinks[k]] = new_id + 1;
		}
	}


	// erase all editors and create new
	m_data.vLevel = vLevel;
	m_data.vEnemy = vEnemy;
	CopyMemory(m_game.nLevel, nLevel, sizeof(nLevel));
	CopyMemory(m_game.nEnemy, nEnemy, sizeof(nEnemy));

	c = m_vEditors.size();
	for(int i = 0; i < c; ++i)
	{
		if ( m_vEditors[i]->Changed() )
			m_vEditors[i]->Apply();

		delete m_vEditors[i];
	}

	m_vEditors.clear();

	for(int i = 0; i < MAX_LEVELS; ++i)
	{
		CNesEditor * ed = new CNesEditor(*vLevel[nLevel[i] - 1], *vEnemy[nEnemy[i] - 1], *this, i);
		m_vEditors.push_back(ed);
	}

	return TRUE;
}
