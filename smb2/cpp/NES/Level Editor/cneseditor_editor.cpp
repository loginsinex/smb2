
#include "precomp.h"

BOOL CNesEditor::CheckPagePointers(INT iPage)
{
	DWORD cItems = SizeOfItemsList();
	for(DWORD i = 0; i < cItems; ++i)
	{
		CNesItem * pItem = (CNesItem*) ItemFromList(i);
		if (( nitEntrance == pItem->Type() || nitPointer == pItem->Type()) && pItem->Page() == iPage )
			return TRUE;
	}

	return FALSE;
}

BOOL CNesEditor::CheckGroundCollision(UINT pos, INT iLayer, DWORD dwItem)
{
	if ( ( m_levelFields.fDirection && (pos> (UINT) 0x10*CountPages() ) )
		|| ( !m_levelFields.fDirection && ( pos > (UINT) 0x0f*CountPages() ) )
		|| iLayer >= (INT) CountLayers() ) 
		return FALSE;

	CNesItem * pItem = (CNesItem*) ItemFromList(dwItem);
	if ( pItem && nitGround != pItem->Type() )
		return FALSE;

	// check for layer collisions
	DWORD cItems = SizeOfItemsList();
	for(INT l = 0; l < iLayer; ++l)
	{
		for(DWORD i = 0; i < cItems; ++i)
		{
			CNesItem * pItem = (CNesItem*) ItemFromList(i);
			if ( nitGround == pItem->Type() )
				if ( i != dwItem && pos >= (UINT) ( m_levelFields.fDirection ? pItem->Pos().x : pItem->Pos().y ) )
					return FALSE;
		}
	}

	// check for collisions for equivalent positions
	for(INT l = iLayer; l < (INT) CountLayers(); ++l)
	{
		for(DWORD i = 0; i < cItems; ++i)
		{
			CNesItem * pItem = (CNesItem*) ItemFromList(i);
			if ( nitGround == pItem->Type() && i != dwItem )
				if ( pos == ( m_levelFields.fDirection ? pItem->Pos().x : pItem->Pos().y ) )
					return FALSE;
		}
	}

	return TRUE;
}

BOOL CNesEditor::InsertObject(INT iLayer, UINT x, UINT y, BYTE regular_id)
{
	NES_LEVEL_ITEM item;
	ZeroMemory(&item, sizeof(item));
	if ( ( m_levelFields.fDirection && (x > (UINT) 0x10*CountPages() || y >= 15 ) )
		|| ( !m_levelFields.fDirection && ( x >= 16 || y > (UINT) 0x0f*CountPages() ) )
		|| iLayer >= (INT) CountLayers() ) 
		return FALSE;

	switch(regular_id)
	{
		case 0x09:
		case 0x0a:
		case 0x0b:
		case 0x13:
		case 0x14:
		case 0x15:
		case 0x1c:
		case 0x1d:
		case 0x1e:
			return FALSE;
	}

	item.nli = nliRegular;
	item.Object.Regular.id = regular_id;
	item.Page = ( m_levelFields.fDirection ? x/0x10 : y / 0x0f );
	POINT pt = { x, y };
	item.pt = pt;
	CNesItem * pItem = new CNesItem(&item, pt, iLayer, item.Page);
	if ( !pItem )
		return FALSE;

	Modified();
	return AppendItemToList(pItem);
}

BOOL CNesEditor::InsertEntrance(INT iLayer, UINT x, UINT y, BYTE regular_id, BOOL fFarPointer, BYTE wParam, BYTE lParam)
{
	NES_LEVEL_ITEM item;
	ZeroMemory(&item, sizeof(item));
	if ( ( m_levelFields.fDirection && (x > (UINT) 0x10*CountPages() || y >= 15 ) )
		|| ( !m_levelFields.fDirection && ( x >= 16 || y > (UINT) 0x0f*CountPages() ) )
		|| iLayer >= (INT) CountLayers() ) 
		return FALSE;


	switch(regular_id)
	{
		case 0x09:
		case 0x0a:
		case 0x0b:
		case 0x13:
		case 0x14:
		case 0x15:
		case 0x1c:
		case 0x1d:
		case 0x1e:
			break;
		default:
			return FALSE;
	}

	item.nli = nliEntrance;
	item.Object.Entrance.uEntranceId = regular_id;
	item.Page = ( m_levelFields.fDirection ? x/0x10 : y / 0x0f );

	if ( CheckPagePointers(item.Page) )
		return FALSE;

	POINT pt = { x, y };
	item.pt = pt;
	if ( fFarPointer )
	{
		item.Object.Entrance.uEntranceParameter[0] = 0xf5;
		item.Object.Entrance.uEntranceParameter[1] = wParam;
		item.Object.Entrance.uEntranceParameter[2] = lParam;
	}
	else
	{
		item.Object.Entrance.uEntranceParameter[0] = wParam;
		item.Object.Entrance.uEntranceParameter[1] = lParam;	
	}

	CNesItem * pItem = new CNesItem(&item, pt, iLayer, item.Page);
	if ( !pItem )
		return FALSE;

	Modified();
	return AppendItemToList(pItem);
}

BOOL CNesEditor::InsertEnemy(UINT x, UINT y, BYTE regular_id, BOOL fHidden)
{
	NES_LEVEL_ENEMY item = { 0 };
	if ( ( m_levelFields.fDirection && (x > (UINT) 0x10*CountPages() || y >= 16 ) )
		|| ( !m_levelFields.fDirection && ( x >= 16 || y > (UINT) 0x10*CountPages() ) )
		) 
		return FALSE;

	if ( fHidden )
		if ( m_levelFields.fDirection )
			x += 10 * 0x10;
		else
			y += 10 * 0x10;

	item.iPage = ( m_levelFields.fDirection ? x : y ) / 0x10;
	item.pt.x = x; item.pt.y = y;
	item.Enemy.Object.id = regular_id;

	CNesItem * pEnemy = new CNesItem(&item, item.pt, item.iPage);
	if ( !pEnemy )
		return FALSE;

	Modified();
	return AppendItemToList(pEnemy);
}

BOOL CNesEditor::InsertGround(UINT pos, BYTE bGroundSet, BYTE bGroundType, INT iLayer)
{
	NES_LEVEL_ITEM item;
	ZeroMemory(&item, sizeof(item));
	if ( ( m_levelFields.fDirection && (pos> (UINT) 0x10*CountPages() ) )
		|| ( !m_levelFields.fDirection && ( pos > (UINT) 0x0f*CountPages() ) )
		|| iLayer >= (INT) CountLayers() ) 
		return FALSE;

	if ( !CheckGroundCollision(pos, iLayer) )
		return FALSE;
	
	item.nli = nliGroundSet;
	item.Object.GroundSet.uSets = bGroundSet;
	item.Page = ( m_levelFields.fDirection ? pos / 0x10 : pos / 0x0f );
	if ( m_levelFields.fDirection )
		item.pt.x = pos ;
	else
		item.pt.y = pos ;

	CNesItem * pItem = new CNesItem( &item, item.pt, iLayer, item.Page, bGroundType);
	if ( !pItem )
		return FALSE;

	Modified();
	return AppendItemToList(pItem);
}

BOOL CNesEditor::InsertPointer(INT iPage, BYTE param1, BYTE param2)
{
	if ( iPage >= (INT) CountPages() || CheckPagePointers(iPage) )
		return FALSE;

	NES_LEVEL_ITEM lvl;
	lvl.nli = nliPtr;
	lvl.Page = iPage;
	lvl.Object.Pointer.Reserved = 0xf5;
	lvl.Object.Pointer.uParameter[0] = param1;
	lvl.Object.Pointer.uParameter[1] = param2;

	CNesItem * pItem = new CNesItem(&lvl, iPage);
	if ( !pItem )
		return FALSE;

	Modified();
	return AppendItemToList(pItem);
}


BOOL CNesEditor::ChangeEntrancePointer(UINT i, BOOL fFarPointer, BYTE wParam, BYTE lParam)
{
	if ( i >= SizeOfItemsList() )
		return FALSE;

	CNesItem * pEntrance = (CNesItem*) ItemFromList(i);
	if ( nitEntrance != pEntrance->Type() )
		return FALSE;

	Modified();
	return pEntrance->SetEntranceParam(fFarPointer, wParam, lParam);
}

BOOL CNesEditor::ChangeLayer(UINT i, INT iLayer)
{
	if ( i >= SizeOfItemsList() || (UINT) iLayer > CountLayers() )
		return FALSE;

	CNesItem * pItem = (CNesItem*) ItemFromList(i);
	if ( nitEnemy == pItem->Type() )
		return FALSE;

	if ( nitGround == pItem->Type() && !CheckGroundCollision( m_levelFields.fDirection ? pItem->Pos().x : pItem->Pos().y, iLayer, i ) )
		return FALSE;

	if ( (UINT) iLayer == CountLayers() ) m_Counters.dwLayers = iLayer+1;

	Modified();
	return pItem->SetLayer(iLayer);
}

BOOL CNesEditor::MoveObject(UINT i, UINT x, UINT y)
{
	if ( ( m_levelFields.fDirection && (x > (UINT) 0x10*CountPages() || y >= 15 ) )
		|| ( !m_levelFields.fDirection && ( x >= 16 || y > (UINT) 0x0f*CountPages() ) )
		) 
		return FALSE;

	CNesItem * pItem = (CNesItem*) ItemFromList(i);

	if ( nitPointer == pItem->Type() || nitEnemy == pItem->Type() )
		return FALSE;

	if ( nitGround == pItem->Type() )
	{
		if ( !CheckGroundCollision( m_levelFields.fDirection ? x : y, pItem->Layer(), i ) )
			return FALSE;

		( m_levelFields.fDirection ? y = 0 : x = 0 );
	}

	if ( pItem->Pos().x == x && pItem->Pos().y == y )
		return FALSE;

	Modified();
	return ( pItem->SetPage( m_levelFields.fDirection ? x/0x10 : y/0x0f ) && pItem->SetPosition(x,y) );
}

BOOL CNesEditor::MoveEnemy(UINT i, UINT x, UINT y, BOOL fHidden)
{
	if ( !fHidden && ( ( m_levelFields.fDirection && (x > (UINT) 0x10*CountPages() || y >= 16 ) )
		|| ( !m_levelFields.fDirection && ( x >= 16 || y > (UINT) 0x10*CountPages() ) )
		) )
		return FALSE;

	CNesItem * pItem = (CNesItem*) ItemFromList(i);

	if ( nitEnemy != pItem->Type() )
		return FALSE;

	if ( fHidden )
		if ( m_levelFields.fDirection )
			x += 10 * 0x10;
		else
			y += 10 * 0x10;

	if ( pItem->Pos().x == x && pItem->Pos().y == y )
		return FALSE;

	Modified();
	return ( pItem->SetPage( ( m_levelFields.fDirection ? x : y )/0x10) && pItem->SetPosition(x,y) );
}

BOOL CNesEditor::ChangePointer(UINT i, BYTE wParam, BYTE lParam)
{
	if ( i >= SizeOfItemsList() )
		return FALSE;

	CNesItem * pItem = (CNesItem*) ItemFromList(i);

	if ( nitPointer != pItem->Type() )
		return FALSE;

	Modified();
	return pItem->SetPtr(wParam, lParam);
}

BOOL CNesEditor::ChangeRegularSize(UINT i, BYTE wSize)
{
	if ( i >= SizeOfItemsList() )
		return FALSE;

	CNesItem * nItem = (CNesItem*) ItemFromList(i);
	if ( nitRegular != nItem->Type() || nItem->Regular() < 0x30 )
		return FALSE;

	wSize &= 0x0f;

	Modified();
	nItem->m_obj.Regular.id = ( 0x10*(nItem->m_obj.Regular.id/0x10) + wSize );
	return TRUE;
}

BOOL CNesEditor::ChangeGround(UINT i, BYTE bSet, BYTE bType)
{
	if ( i >= SizeOfItemsList() )
		return FALSE;

	CNesItem * pItem = (CNesItem*) ItemFromList(i);
	if ( nitGround != pItem->Type() )
		return FALSE;

	Modified();
	pItem->m_obj.Ground.uSets = ( bSet & 0x1f );
	pItem->ChangeBgType(bType);
	return TRUE;
}

BOOL CNesEditor::DeleteObject(UINT i)
{
	if ( i >= SizeOfItemsList() )
		return FALSE;

	delete (CNesItem*) ItemFromList(i);
	Modified();
	return DeleteFromItemsList(i);
}

BOOL CNesEditor::SetHeaderInfo(NES_LEVEL_FIELDS nlf, BYTE bNewValue)
{
	BYTE bOldValue = 0;
	switch(nlf)
	{
	case nlfDirection: bOldValue = m_levelFields.fDirection; m_levelFields.fDirection = ( 1 & bNewValue ); break;
	case nlfMusic: m_levelFields.uMusic = ( 3 & bNewValue ); break;
	case nlfLength: m_levelFields.uLength = ( 15 & bNewValue ); m_Counters.dwPages = m_levelFields.uLength+1; break;
	case nlfGround: m_levelFields.uGround = ( 31 & bNewValue ); break;
	case nlfTypeOfGround: m_levelFields.uTypeOfGround = ( 7 & bNewValue ); break;
	case nlfObjectType: m_levelFields.uObjectType = ( 15 & bNewValue ); break;
	case nlfColor: m_levelFields.uColor = ( 7 & bNewValue ); break;
	case nlfEnemyColor: m_levelFields.uEnemyColor = ( 3 & bNewValue ); break;
	}

	if ( nlfGround == nlf )
		((CNesItem*) ItemFromList(0))->m_obj.Ground.uSets = m_levelFields.uGround;

	if ( nlfTypeOfGround == nlf )
		((CNesItem*) ItemFromList(0))->ChangeBgType(m_levelFields.uTypeOfGround);

	if ( nlfDirection == nlf && bOldValue != m_levelFields.fDirection )
		RewritePoints();

	Modified();
	return TRUE;
}

BOOL CNesEditor::RemoveAll()
{
	if ( SizeOfItemsList() < 1 )
		return FALSE;

	// dont remove first object (contains level ground settings)
	m_Counters.dwLayers = 1;
	while(SizeOfItemsList() > 1)
	{
		delete (CNesItem*) ItemFromList(1);
		DeleteFromItemsList(1);
	}

	Modified();
	return ( 1 == SizeOfItemsList() );
}

INT CNesEditor::GetFX(INT iWorld)
{
	if ( -1 == iWorld ) iWorld = m_Level.iWorld;

	if ( iWorld < 0 || iWorld >= 7 )
		return 0;

	BYTE bLow = 0, bHi = 0;
	m_pFile->LoadData(&bLow, 0x0001fe10+iWorld, 1);
	m_pFile->LoadData(&bHi, 0x0001fe17+iWorld, 1);
	INT fx = ( bLow ) | ( bHi << 8 );
	
	for(INT i = 0; i < 5; ++i)
		if ( g_mWorldInterior[i] == (DWORD) fx )
			return i;

	return 0;
}

INT CNesEditor::GetSubLevel()
{
	return m_Level.iSubLevel;
}

DWORD CNesEditor::GetCurLevel()
{
	return m_Level.dwLevel;
}

INT CNesEditor::FetchCurWorld()
{
	DWORD dwSubLevel = m_Level.dwLevel / 10;

	for(int i = 0; i <= MAX_LEVELS / 30; ++i)
	{
		DWORD dwGameSubLevel = m_pFile->Byte(0x001E022 + i);
		if ( dwSubLevel < dwGameSubLevel )
		{
			if ( !i )
				return -1;

			m_Level.iSubLevel = dwSubLevel - m_pFile->Byte(0x001E022 + i - 1);
			return ( m_Level.iWorld = ( i - 1 ) );
		}
	}

	return -1;
}


INT CNesEditor::GetCurWorld()
{
	return m_Level.iWorld;
}

BOOL CNesEditor::GetColFromPalette(COLORREF * pColArray)
{
	if ( IsBadWritePtr(pColArray, sizeof(COLORREF)*16) )
		return FALSE;

	BYTE bLowPtr = 0, bHiPtr = 0;
	if ( !m_pFile->LoadData(&bLowPtr, 0x0000c010+m_Level.iWorld, 1) ||
		! m_pFile->LoadData(&bHiPtr, 0x0000c01e+m_Level.iWorld, 1)
		)
		return FALSE;

	DWORD ptr = (( bHiPtr << 8 ) | ( bLowPtr )) + 0x4010;
	BYTE bcolArray[16] = { 0 };
	if ( !m_pFile->LoadData(&bcolArray[0], ptr+16*m_levelFields.uColor, 16) )
		return FALSE;

	for(int i = 0; i < 16; ++i )
		pColArray[i] = XCHGCOLORREF( g_crNesColor[bcolArray[i]] );

	return TRUE;
}

