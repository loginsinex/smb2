
#include "precomp.h"	

BOOL CNesEditor::LoadLevel(CNesLevel & lvl, CNesEnemyData & enm, CFileLoader * file, DWORD dwCurLevel)
{
	m_pFile = file;
	m_Level.dwLevel = dwCurLevel;
	m_Level.iSubLevel = -1;
	m_Level.iWorld = FetchCurWorld();
	// load level items
	UINT cItems = lvl.SizeOfItemsList();
	BYTE uGroundType = lvl[nlfTypeOfGround], uGroundSet = lvl[nlfGround];
	UINT iDeltaX = 0, iDeltaY = 0, iLastGrSetX = 0, iLastBgSet = 0, iLayer = 0, iLastPageX = 0;
	m_Links.dwEnemyLink = enm.LinkLevel();
	m_Links.dwLevelLink = lvl.LinkLevel();

	m_levelFields.fDirection = lvl[nlfDirection]; //		= 0,
	m_levelFields.Reserved1 = lvl[nlfReserved1]; //		= 1,
	m_levelFields.uColor = lvl[nlfColor]; //			= 2,
	m_levelFields.Reserved2 = lvl[nlfReserved2]; //		= 3,
	m_levelFields.uEnemyColor = lvl[nlfEnemyColor]; //		= 4,
	m_levelFields.Reserved3 = lvl[nlfReserved3]; //		= 5,
	m_levelFields.uGround = lvl[nlfGround]; //			= 6,
	m_levelFields.uLength = lvl[nlfLength]; //			= 7,
	m_levelFields.uObjectType = lvl[nlfObjectType]; //		= 8,
	m_levelFields.Reserved4 = lvl[nlfReserved4]; //		= 9,
	m_levelFields.uTypeOfGround = lvl[nlfTypeOfGround]; //		= 10,
	m_levelFields.Reserved5 = lvl[nlfReserved5]; //		= 11,
	m_levelFields.uMusic = lvl[nlfMusic]; //			= 12

	NES_LEVEL_ITEM nItem;
	ZeroMemory(&nItem, sizeof(nItem));
	nItem.nli = nliGroundSet;
	nItem.Object.GroundSet.uSets = m_levelFields.uGround;
	nItem.Page = 0;
	POINT pos = { 0 , 0 };

	pInitialBg = new CNesItem(&nItem, pos, 0, 0, m_levelFields.uTypeOfGround);
	AppendItemToList(pInitialBg);

	for(UINT i=0; i < cItems; ++i)
	{
		CNesItem * pItem = NULL;
		NES_LEVEL_ITEM x = lvl.LevelData(i);
		switch(x.nli)
		{
		case nliEntrance:
		case nliRegular:
			{
				iDeltaY += x.Object.General.y;
				if ( iDeltaY >= 0x0F )
				{
					iDeltaX += 0x10;
					iDeltaY = ( iDeltaY + 1 ) % 16;
				}

				POINT p = { m_levelFields.fDirection ? iDeltaX + x.Object.General.x : x.Object.General.x , m_levelFields.fDirection ? iDeltaY : iDeltaY + 0x0f*(iDeltaX/0x10) };
				pItem = new CNesItem(&x, p, iLayer, iDeltaX / 0x10);
				break;
			}
		case nliBackToStart:
			{
				iDeltaX = iDeltaY = 0;
				iLayer++;
				break;
			}
		case nliSkipper:
			{
				iDeltaY = 0;
				iDeltaX += 0x10*( (x.Object.Skipper.uType & 0x0F) - 1 );
				break;
			}
		case nliGroundSet:
			{
				iLastBgSet = SizeOfItemsList();
				if ( m_levelFields.fDirection )
				{
					UINT cx = iDeltaX + 8*( x.Object.GroundSet.Reserved & 0x0F );
					cx += x.Object.GroundSet.uSets/0x20;
					if ( cx <= iLastGrSetX ) cx = iLastGrSetX + 1;
					POINT pt = { cx, 0 };
					iLastGrSetX = cx;
					pItem = new CNesItem(&x, pt, iLayer, iDeltaX / 0x10, uGroundType);
				}
				else
				{
					UINT cy = 0x0f*(iDeltaX/0x10) + 8*( x.Object.GroundSet.Reserved & 0x0F );
					cy += x.Object.GroundSet.uSets/0x20;
					if ( cy <= iLastGrSetX ) cy = iLastGrSetX + 1;
					POINT pt = { 0, cy };
					iLastGrSetX = cy;
					pItem = new CNesItem(&x, pt, iLayer, iDeltaX / 0x10, uGroundType);
				}
				break;
			}
		case nliGroundType:
			{
				uGroundType = x.Object.GroundType.uType;
				ItemFromList(iLastBgSet)->ChangeBgType(uGroundType);
				break;
			}
		case nliPtr:
			{
				pItem = new CNesItem(&x, iDeltaX/0x10);
				break;
			}
		}

		if ( iDeltaX > iLastPageX ) iLastPageX = iDeltaX;

		if ( pItem )
			AppendItemToList(pItem);
	}

	m_Counters.dwPages = 1 + max(iLastPageX / 0x10, m_levelFields.uLength);
	m_Counters.dwLayers = 1 + iLayer;

	// load level enemies
	for(UINT i=0; i<enm.SizeOfItemsList(); ++i)
	{ 
		NES_LEVEL_ENEMY Item = enm.LevelEnemy(i);
		Item.iPage = Item.pt.x / 0x10;
		if ( !m_levelFields.fDirection )
		{
			Item.pt.y += 0x10*(Item.pt.x/0x10);
			Item.pt.x %= 0x10;
		}
		CNesItem * pItem = new CNesItem(&Item, Item.pt, Item.iPage);
		AppendItemToList(pItem);
	}

	return (SizeOfItemsList() > 0);
}

VOID CNesEditor::SetItemsLayerLink(DWORD dwItemsLink)
{
	m_Links.dwLevelLink = dwItemsLink;
}

VOID CNesEditor::SetEnemiesLayerLink(DWORD dwEnemiesLink)
{
	m_Links.dwEnemyLink = dwEnemiesLink;
}

BOOL CNesEditor::UpdateCounters()
{
	m_Level.iWorld = FetchCurWorld();
	return ( m_Level.iWorld != -1 );
}