
#include "precomp.h"


CNesEditor::CNesEditor(CNesLevel & lvl, CNesEnemyData & enm, CFileLoader & file, DWORD dwCurLevel)
	: CNesItemsList(TRUE), m_lvl( lvl ), m_enm( enm ), m_file( file ), m_pFile( &m_file )
{
	pInitialBg = NULL;
	m_Links.dwEnemyLink = m_Links.dwLevelLink = -1;
	m_Level.dwLevel = 0; m_Level.iWorld = 0;
	m_lvl.AddRef(); m_enm.AddRef();
	LoadLevel(m_lvl, m_enm, m_pFile, dwCurLevel);
}

CNesEditor::~CNesEditor()
{
	m_lvl.Release(); m_enm.Release();
	while(SizeOfItemsList())
	{
		int c = SizeOfItemsList() - 1;
		delete ItemFromList(c);
		DeleteFromItemsList(c);
	}
}

CNesItem CNesEditor::LevelItem(UINT i)
{
	if ( i >= SizeOfItemsList() )
		throw(CString(TEXT("Object subscript is out of range")));

	return *ItemFromList(i);
}

BYTE CNesEditor::operator[](NES_LEVEL_FIELDS index)
{
	switch(index)
	{
	case nlfDirection: return m_levelFields.fDirection; //		= 0,
	case nlfReserved1: return m_levelFields.Reserved1; //		= 1,
	case nlfColor: return m_levelFields.uColor; //			= 2,
	case nlfReserved2: return m_levelFields.Reserved2; //		= 3,
	case nlfEnemyColor: return m_levelFields.uEnemyColor; //		= 4,
	case nlfReserved3: return m_levelFields.Reserved3; //		= 5,
	case nlfGround: return m_levelFields.uGround; //			= 6,
	case nlfLength: return m_levelFields.uLength; //			= 7,
	case nlfObjectType: return m_levelFields.uObjectType; //		= 8,
	case nlfReserved4: return m_levelFields.Reserved4; //		= 9,
	case nlfTypeOfGround: return m_levelFields.uTypeOfGround; //		= 10,
	case nlfReserved5: return m_levelFields.Reserved5; //		= 11,
	case nlfMusic: return m_levelFields.uMusic; //			= 12
	}
	return -1;
}

BOOL CNesEditor::Apply()
{
	std::vector<NES_LEVEL_ITEM> pItems;
	DWORD dwCount = 0, dwLevelHeader = 0;
	std::vector<NES_LEVEL_ENEMY> pEnemies;
	DWORD dwCountEnemies = 0;
	
	BOOL fItemsCreated = MakeObjectsList(dwCount, pItems, dwLevelHeader);
	BOOL fEnemiesCreated = MakeEnemiesList(dwCountEnemies, pEnemies);

	if ( fItemsCreated && fEnemiesCreated )
	{
		m_lvl.LoadLevelData(dwLevelHeader, pItems, dwCount);
		m_enm.LoadEnemyData(pEnemies, dwCountEnemies, (*this)[nlfLength]+1);
		m_fChanged = FALSE;
		return TRUE;
	}
	return FALSE;
}

DWORD CNesEditor::CountPages()
{
	return m_Counters.dwPages;
}

DWORD CNesEditor::CountLayers()
{
	return m_Counters.dwLayers;
}

BOOL CNesEditor::IsEnemyLayerLocked()
{
	return m_enm.IsLinkedLevel();
}

BOOL CNesEditor::IsItemsLayerLocked()
{
	return m_lvl.IsLinkedLevel();
}

DWORD CNesEditor::ItemsLayerLink()
{
	return m_Links.dwLevelLink;
}

DWORD CNesEditor::EnemiesLayerLink()
{
	return m_Links.dwEnemyLink;
}

BOOL CNesEditor::GetLocalLevelItems(std::vector<NES_LEVEL_ITEM> & vItems)
{
	vItems.clear();
	DWORD c = m_lvl.SizeOfItemsList();
	for(DWORD i = 0; i < c; ++i)
		vItems.push_back(m_lvl.LevelData(i));

	return ( vItems.size() > 0 );
}

BOOL CNesEditor::GetCurrentLevelItems(std::vector<NES_LEVEL_ITEM> & vItems)
{
	DWORD d1 = 0, d2 = 0;
	vItems.clear();

	if ( MakeObjectsList(d1, vItems, d2) )
		return ( vItems.size() > 0 );

	return FALSE;
}

BOOL CNesEditor::GetLocalEnemyItems(std::vector<NES_LEVEL_ENEMY> & vItems)
{
	vItems.clear();

	DWORD c = m_enm.SizeOfItemsList();
	for(DWORD i = 0; i < c; ++i)
		vItems.push_back(m_enm.LevelEnemy(i));

	return ( vItems.size() > 0 );
}

BOOL CNesEditor::GetCurrentEnemyItems(std::vector<NES_LEVEL_ENEMY> & vItems)
{
	DWORD d1 = 0, d2 = 0;
	vItems.clear();

	if ( MakeEnemiesList(d1, vItems) )
		return ( vItems.size() > 0 );

	return FALSE;
}

BOOL CNesEditor::Changed()
{
	return m_fChanged;
}

VOID CNesEditor::Modified()
{
	m_fChanged = TRUE;
}