
#include "precomp.h"

// 0x1f542 + enemyId - properties of hits
// 0x1f5d0 + enemyId - hit area?
// 0x1f589 + enemyId - height of enemy?

const LPTSTR g_pszEnemies[] =
{
 /*	0	*/ TEXT("Heart"),
 /*	1	*/ TEXT("Shyguy (red)"),
 /*	2	*/ TEXT("Tweeter"),
 /*	3	*/ TEXT("Shyguy (pink)"),
 /*	4	*/ TEXT("Porcupo"),
 /*	5	*/ TEXT("Snifit (red)"),
 /*	6	*/ TEXT("Snifit (grey)"),
 /*	7	*/ TEXT("Snifit (pink)"),
 /*	8	*/ TEXT("Ostro"),
 /*	9	*/ TEXT("Bob-omb"),
 /*	10	*/ TEXT("Albatoss (with Bob-omb)"),
 /*	11	*/ TEXT("Albatoss, goes right"),
 /*	12	*/ TEXT("Albatoss, goes left"),
 /*	13	*/ TEXT("Ninji (running)"),
 /*	14	*/ TEXT("Ninji (jumping)"),
 /*	15	*/ TEXT("Beezo (red, swooping)"),
 /*	16	*/ TEXT("Beezo (grey, flies straight)"),
 /*	17	*/ TEXT("Wart's bubble"),
 /*	18	*/ TEXT("Pidgit (with carpet)"),
 /*	19	*/ TEXT("Trouter"),
 /*	20	*/ TEXT("Hoopstar"),
 /*	21	*/ TEXT("Shyguy, comes out of jars"),
 /*	22	*/ TEXT("Bob-omb, comes out of jars"),
 /*	23	*/ TEXT("Phanto"),
 /*	24	*/ TEXT("Cobrat (in jar)"),
 /*	25	*/ TEXT("Cobrat (in sand)"),
 /*	26	*/ TEXT("Pokey"),
 /*	27	*/ TEXT("Stray bullet"),
 /*	28	*/ TEXT("Birdo"),
 /*	29	*/ TEXT("Mouser"),
 /*	30	*/ TEXT("Stray egg"),
 /*	31	*/ TEXT("Triclyde"),
 /*	32	*/ TEXT("Stray fireball"),
 /*	33	*/ TEXT("Clawglip"),
 /*	34	*/ TEXT("Stray rock"),
 /*	35	*/ TEXT("Panser (red)"),
 /*	36	*/ TEXT("Panser (pink)"),
 /*	37	*/ TEXT("Panser (grey)"),
 /*	38	*/ TEXT("Autobomb"),
 /*	39	*/ TEXT("Stray autobomb fireball"),
 /*	40	*/ TEXT("Whale spout"),
 /*	41	*/ TEXT("Flurry"),
 /*	42	*/ TEXT("Fryguy"),
 /*	43	*/ TEXT("Little fryguy"),
 /*	44	*/ TEXT("Wart"),
 /*	45	*/ TEXT("Hawkmouth (alive)"),
 /*	46	*/ TEXT("Spark (1)"),
 /*	47	*/ TEXT("Spark (2)"),
 /*	48	*/ TEXT("Spark (3)"),
 /*	49	*/ TEXT("Spark (4)"),
 /*	50	*/ TEXT("Small vegetable"),
 /*	51	*/ TEXT("Fresh vegetable"),
 /*	52	*/ TEXT("Other vegetable (used by vegetable thrower)"),
 /*	53	*/ TEXT("Turtle shell"),
 /*	54	*/ TEXT("Coin"),
 /*	55	*/ TEXT("Bomb (explodes)"),
 /*	56	*/ TEXT("Rocket (weird)"),
 /*	57	*/ TEXT("Mushroom block (weird)"),
 /*	58	*/ TEXT("POW block"),
 /*	59	*/ TEXT("Rolling red beam"),
 /*	60	*/ TEXT("Door, leads to sub-space"),
 /*	61	*/ TEXT("Key"),
 /*	62	*/ TEXT("Potion"),
 /*	63	*/ TEXT("Mushroom"),
 /*	64	*/ TEXT("1-UP mushroom"),
 /*	65	*/ TEXT("Pidgit's carpet"),
 /*	66	*/ TEXT("Hawkmouth (faces right)"),
 /*	67	*/ TEXT("Hawkmouth (faces left)"),
 /*	68	*/ TEXT("Crystal ball"),
 /*	69	*/ TEXT("Star"),
 /*	70	*/ TEXT("Stopwatch"),
 /*	71	*/ TEXT("Albatoss attack (with Bob-ombs)"),
 /*	72	*/ TEXT("Beezo attack"),
 /*	73	*/ TEXT("Stop albatoss/beezo attack"),
 /*	74	*/ TEXT("Vegetable throw (used in Wart's room)"),
 /*	75	*/ TEXT("Not used"),
 /*	76	*/ TEXT("Not used"),
 /*	77	*/ TEXT("Not used"),
 /*	78	*/ TEXT("Not used"),
 /*	79	*/ TEXT("Not used"),
 /*	80	*/ TEXT("Not used"),
 /*	81	*/ TEXT("Not used"),
 /*	82	*/ TEXT("Not used"),
 /*	83	*/ TEXT("Not used"),
 /*	84	*/ TEXT("Not used"),
 /*	85	*/ TEXT("Not used"),
 /*	86	*/ TEXT("Not used"),
 /*	87	*/ TEXT("Not used"),
 /*	88	*/ TEXT("Not used"),
 /*	89	*/ TEXT("Not used"),
 /*	90	*/ TEXT("Not used"),
 /*	91	*/ TEXT("Not used"),
 /*	92	*/ TEXT("Birdo (level boss)"),
 /*	93	*/ TEXT("Mouser (level boss)"),
 /*	94	*/ TEXT("Stray egg"),
 /*	95	*/ TEXT("Triclyde (level boss)"),
 /*	96	*/ TEXT("Stray fireball (level boss)"),
 /*	97	*/ TEXT("Clawglip (level boss)"),
 /*	98	*/ TEXT("Stray rock (level boss)"),
 /*	99	*/ TEXT("Panser (red) (level boss)"),
 /*	100	*/ TEXT("Panser (pink) (level boss)"),
 /*	101	*/ TEXT("Panser (grey) (level boss)"),
 /*	102	*/ TEXT("Autobomb (level boss)"),
 /*	103	*/ TEXT("Stray autobomb fireball (level boss)"),
 /*	104	*/ TEXT("Whale spout"),
 /*	105	*/ TEXT("Flurry (level boss)"),
 /*	106	*/ TEXT("Fryguy (level boss)"),
 /*	107	*/ TEXT("Little fryguy (level boss)"),
 /*	108	*/ TEXT("Wart (level boss)"),
 /*	109	*/ TEXT("Hawkmouth (level boss)"),
 /*	110	*/ TEXT("Spark (1) (level boss)"),
 /*	111	*/ TEXT("Spark (2) (level boss)"),
 /*	112	*/ TEXT("Spark (3) (level boss)"),
 /*	113	*/ TEXT("Spark (4) (level boss)"),
 /*	114	*/ TEXT("Small vegetable"),
 /*	115	*/ TEXT("Fresh vegetable"),
 /*	116	*/ TEXT("Other vegetable"),
 /*	117	*/ TEXT("Turtle shell (level boss)"),
 /*	118	*/ TEXT("Coin"),
 /*	119	*/ TEXT("Bomb (explodes) (level boss)"),
 /*	120	*/ TEXT("Rocket (weird)"),
 /*	121	*/ TEXT("Mushroom block (weird)"),
 /*	122	*/ TEXT("POW block"),
 /*	123	*/ TEXT("Rolling red beam"),
 /*	124	*/ TEXT("Door, leads to sub-space"),
 /*	125	*/ TEXT("Key"),
 /*	126	*/ TEXT("Potion"),
 /*	127	*/ TEXT("Mushroom")
};

CNesEnemyData::CNesEnemyData()
	: m_dwSizeOfEnemies( 0 )
{
	m_dwPages = 0;
	m_dwLinkedToLevel = 0;
}

CNesEnemyData::~CNesEnemyData()
{
	FlushObjects();
}

BOOL CNesEnemyData::FlushObjects()
{
	while(SizeOfItemsList() > 0)
	{
		delete (PNES_LEVEL_ENEMY) ItemFromList(0);
		DeleteFromItemsList(0);
	}
	m_dwPages = 0;

	StoreSizeOfEnemies();

	return TRUE;
}

BOOL CNesEnemyData::RereadPoints()
{
	DWORD cItems = SizeOfItemsList();

	for(DWORD i = 0; i < cItems; ++i)
	{
		PNES_LEVEL_ENEMY pEnemy = (PNES_LEVEL_ENEMY) ItemFromList(i);
		pEnemy->pt.y = pEnemy->Enemy.Object.y;
		pEnemy->pt.x = 0x10*pEnemy->iPage + pEnemy->Enemy.Object.x;
	}

	return TRUE;
}

BOOL CNesEnemyData::LoadData(PBYTE ptr, int size, DWORD dwLinkedLevel)
{
	FlushObjects();
	int i = 0, usize = 0, page = 0;
	do
	{
		usize = ptr[i++];
		usize--;
		for(int k=0;k<usize;k+=2)
		{
			PNES_LEVEL_ENEMY pItem = new NES_LEVEL_ENEMY;
			pItem->Enemy.pData[0] = ptr[i++];
			pItem->Enemy.pData[1] = ptr[i++];
			pItem->pt.x = 0x10*page + (DWORD) pItem->Enemy.Object.x;
			pItem->pt.y = pItem->Enemy.Object.y;
			pItem->iPage = page;
			AppendItemToList(pItem);
		}
		page++;
	} while(i < size); //page < size);

	m_dwPages = page;

	StoreSizeOfEnemies();

	return (page > 0);

}

BOOL CNesEnemyData::LoadEnemyData(std::vector<NES_LEVEL_ENEMY> & pItemsList, DWORD dwCount, DWORD dwPagesCount)
{
	FlushObjects();

	for(DWORD i = 0; i < dwCount; ++i)
	{
		PNES_LEVEL_ENEMY pEnemy = new NES_LEVEL_ENEMY;
		*pEnemy = pItemsList[i];
		if ( m_dwPages < (DWORD) pItemsList[i].iPage ) m_dwPages = pItemsList[i].iPage;
		AppendItemToList(pEnemy);
	}
	RereadPoints();

	m_dwPages++;
	if ( m_dwPages < dwPagesCount ) m_dwPages = dwPagesCount;

	StoreSizeOfEnemies();

	return ( dwCount > 0 );
}

NES_LEVEL_ENEMY CNesEnemyData::LevelEnemy(DWORD index)
{
	if ( index >= SizeOfItemsList() )
		throw CString("Subscript is out of range");

	return NES_LEVEL_ENEMY(*(PNES_LEVEL_ENEMY) ItemFromList(index));
}


DWORD CNesEnemyData::SizeOfEnemies()
{
	return m_dwSizeOfEnemies;
}

VOID CNesEnemyData::StoreSizeOfEnemies()
{
	m_dwSizeOfEnemies = 1;

	if ( !m_dwPages )
		return;

	m_dwSizeOfEnemies = SizeOfItemsList()*2 + (m_dwPages-1) + 1;
}

DWORD CNesEnemyData::CountPages()
{
	return m_dwPages;
}

DWORD CNesEnemyData::ItemsFromPage(INT iPage, PNES_LEVEL_ENEMY nItems, DWORD dwCount)
{
	DWORD cItems = SizeOfItemsList(), wObject = 0;
	for(DWORD i = 0; i < cItems && wObject < dwCount; ++i)
	{
		PNES_LEVEL_ENEMY pItem = (PNES_LEVEL_ENEMY) ItemFromList(i);
		if ( pItem->iPage == iPage )
			nItems[wObject++] = *pItem;
	}
	return wObject;
}

BOOL CNesEnemyData::MakeByteArray(PBYTE * pArray, LPDWORD lpdwCount)
{
	if ( CountPages() < 1 )
		return FALSE;

	*lpdwCount = SizeOfEnemies();
	*pArray = new BYTE[*lpdwCount];
	if ( !(*pArray) ) return FALSE;

	NES_LEVEL_ENEMY nItems[128];
	DWORD cPages = CountPages(), index = 0;
	for(DWORD i = 0; i < cPages; ++i)
	{
		DWORD cItems = ItemsFromPage(i, &nItems[0], 128);
		(*pArray)[index++] = (BYTE) (1+2*cItems);
		
		for(DWORD k = 0; k < cItems; ++k)
		{
			(*pArray)[index++] = nItems[k].Enemy.pData[0];
			(*pArray)[index++] = nItems[k].Enemy.pData[1];
		}
	}

	return ( index > 0 );
}

DWORD CNesEnemyData::LinkLevel()
{
	return m_dwLinkedToLevel;
}

BOOL CNesEnemyData::IsLinkedLevel()
{
	return ( m_dwLinkedToLevel > 1 );
}

DWORD CNesEnemyData::AddRef()
{
	return ++m_dwLinkedToLevel;
}

DWORD CNesEnemyData::Release()
{
	return --m_dwLinkedToLevel;
}
