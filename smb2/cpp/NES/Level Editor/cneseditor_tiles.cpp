
#include "precomp.h"

DWORD CNesEditor::GetBgSet(BYTE bBgSet)
{
	if ( bBgSet >= 32 )
		return 0;

	BOOL fHorizontal = m_levelFields.fDirection;
	DWORD bgSet = 0;
	m_pFile->LoadData((PBYTE) &bgSet, 0x0000d210 + ( fHorizontal ? 0 : 0x7c ) + 4*bBgSet, 4);
	return SWAP_DWORD(bgSet);
}

BYTE CNesEditor::GetBgTile(BYTE bBgSet, BYTE bBgType, INT iWorld)
{
	if ( -1 == iWorld ) iWorld = m_Level.iWorld;
	if ( iWorld < 0 || iWorld >= 7 || bBgSet >= 4 || bBgType >= 8 )
		return -1;

	BOOL fHorizontal = m_levelFields.fDirection;
	BYTE bTile = 0;

	// Load pointer for horizontal level data
	BYTE bLowPtr = 0, bHiPtr = 0;
	USHORT ptr = 0;
	m_pFile->LoadData(&bLowPtr, ( fHorizontal ? 0x0000c438 : 0x0000c43f ) + iWorld , 1);
	m_pFile->LoadData(&bHiPtr, ( fHorizontal ? 0x0000c446 : 0x0000c44d ) + iWorld, 1);
		
	// make pointer
	ptr = 0x4010 + (((USHORT) bLowPtr) | (((USHORT) bHiPtr) << 8));
		
	// load bg data
	if ( ptr > 0 ) 
	{
		m_pFile->LoadData(&bTile, ptr+4*bBgType+bBgSet, 1);
		return bTile;
	}

	return -1;
}

BYTE CNesEditor::GetObjTile(BYTE bObjectId)
{
	if ( bObjectId < 0 || bObjectId > 8 )
		return 0;

	BYTE bLowPtr = 0, bHiPtr = 0, bTile = 0, 
		bObjectTypePtr = ( bObjectId >= 7 ? m_levelFields.uObjectType / 4 : m_levelFields.uObjectType % 4 );
	USHORT ptr = 0;

	if ( !m_pFile->LoadData(&bLowPtr, 0x0000c911+m_Level.iWorld, 1) ||
		!m_pFile->LoadData(&bHiPtr, 0x0000c918+m_Level.iWorld, 1) )
		return FALSE;

	ptr = (( bLowPtr ) | ( bHiPtr << 8 )) + 0x00004010;
	if ( m_pFile->LoadData(&bTile, ptr+4*bObjectId+bObjectTypePtr, 1) )
		return bTile;

	return FALSE;
}

NES_ITEM_DIM CNesEditor::GetMasvDim(int item, NES_ITEM_DIM nid, USHORT & type)
{
	BYTE bLevel = 0;
	BOOL fCastleLevel = FALSE;

	switch(item)
	{
	case 0x18:
	case 0x19:
		{
			m_pFile->LoadData(&bLevel, 0xCD7F, 1);
			fCastleLevel = ( bLevel == LOBYTE(LOWORD(m_Level.iWorld)) );
			if ( !fCastleLevel )
				m_pFile->LoadData(&nid.topleft, 0x000CD5A + ( item - 0x18 ), 1);
			else
				m_pFile->LoadData(&nid.topleft, 0x000CD5C + ( item - 0x18 ), 1);

			nid.top = nid.topright = 
				nid.left = nid.middle = nid.right = 
				nid.topleft;
			break;
		}
	case 0x09:	// warning! This line and below cases have NOT real regular id!
		{
			m_pFile->LoadData(&bLevel, 0xCBC7, 1);
			fCastleLevel = ( bLevel == LOBYTE(LOWORD(m_Level.iWorld)) );
			if ( fCastleLevel )
			{
				m_pFile->LoadData(&nid.topleft, 0x000D06C, 1);
				m_pFile->LoadData(&nid.top, 0x000D070, 1);
				m_pFile->LoadData(&nid.topright, 0x000D074, 1);
				m_pFile->LoadData(&nid.left, 0x000D06D, 1);
				m_pFile->LoadData(&nid.middle, 0x000D071, 1);
				m_pFile->LoadData(&nid.right, 0x000D075, 1);
				nid.bottomleft = nid.left;
				nid.bottomright = nid.right;
			}
			else
			{
				m_pFile->LoadData(&nid.topleft, 0x000CBBA, 1);
				m_pFile->LoadData(&nid.top, 0x000CBBB, 1);
				m_pFile->LoadData(&nid.topright, 0x000CBBC, 1);
				m_pFile->LoadData(&nid.left, 0x000CBBD, 1);
				m_pFile->LoadData(&nid.middle, 0x000CBBE, 1);
				m_pFile->LoadData(&nid.right, 0x000CBBF, 1);
				nid.bottomleft = nid.left;
				nid.bottomright = nid.right;
			}
			break;
		}
	case 0x08:
		{
			m_pFile->LoadData(&bLevel, 0xCECC, 1);
			fCastleLevel = ( bLevel == LOBYTE(LOWORD(m_Level.iWorld)) );
			if ( fCastleLevel )
			{
				m_pFile->LoadData(&nid.topleft, 0x000D128, 1);
				nid.top = nid.topright = nid.topleft;
				nid.left = nid.right = nid.middle = nid.topleft;
			}
			else
			{
				m_pFile->LoadData(&nid.topleft, 0x000CE92, 1);
				m_pFile->LoadData(&nid.top, 0x000CE97, 1);
				m_pFile->LoadData(&nid.topright, 0x000CE9C, 1);
				m_pFile->LoadData(&nid.left, 0x000CE93, 1);
				m_pFile->LoadData(&nid.middle, 0x000CE98, 1);
				m_pFile->LoadData(&nid.right, 0x000CE9D, 1);
				nid.bottomleft = nid.bottomright = 0x11;
			}

			break;
		}
	case 0x0C:
		{
			m_pFile->LoadData(&bLevel, 0xCDA9, 1);
			fCastleLevel = ( bLevel == LOBYTE(LOWORD(m_Level.iWorld)) ); 
			// really NOT castle level. Level #04
			if ( fCastleLevel )
			{
				m_pFile->LoadData(&nid.top, 0x000CF24, 1);
				nid.topleft = nid.topright = nid.middle = nid.top;
				nid.left = nid.right = nid.middle;
			}
			else
			{
				m_pFile->LoadData(&nid.top, 0x000CDA3, 1);
				m_pFile->LoadData(&nid.middle, 0x000CDA4, 1);
				nid.topleft = nid.topright = nid.top;
				nid.left = nid.right = nid.middle;
				break;
			}
			break;
		}
	default: return nid;
	}

	type = 4;

	return nid;
}

NES_ITEM_DIM CNesEditor::GetVertDim(int item, NES_ITEM_DIM nid, USHORT & type)
{
	struct
	{
		DWORD		top;
		DWORD		middle;
		DWORD		bottom;
	} ptr = { 0 };

	switch(item)
	{
	case 0x06: 
	case 0x07: 
	case 0x08: 
		{
			ptr.top = 0x000CCD2 + (item - 0x06);
			ptr.middle = 0x000CCED;
			ptr.bottom = 0x000CCF9;
			break;
		}
	case 0x0C:
		{
			ptr.top = 0x000CD07;
			ptr.middle = 0x000CD14;
			ptr.bottom = ptr.middle;
			break;
		}
	case 0x0D:
		{
			ptr.top = 0x000CD14;
			ptr.middle = 0x000CD14;
			ptr.bottom = ptr.middle;
			break;
		}
	case 0x12:
		{
			ptr.bottom = 0x000CD26;
			ptr.middle = ptr.top = 0x000CD39;
			break;
		}
	case 0x16:
		{
			ptr.top = 0x000CC63;
			ptr.bottom = ptr.middle = 0x00CC65;
			break;
		}
	case 0x0F:
		{
			ptr.bottom = ptr.top = 0x000D0D5 + m_Level.iWorld;
			ptr.middle = 0x000D0DC + m_Level.iWorld;
			break;
		}
	default: return nid;
	}

	type = 4;
	m_pFile->LoadData(&nid.topleft, ptr.top, 1);
	m_pFile->LoadData(&nid.middle, ptr.middle, 1);
	if ( ptr.bottom == ptr.top )
		nid.bottomleft = nid.topleft;
	else
		m_pFile->LoadData(&nid.bottomleft, ptr.bottom, 1);

	return nid;
}

NES_ITEM_DIM CNesEditor::GetSingDim(int item, NES_ITEM_DIM nid, USHORT & type)
{
	DWORD ptr = 0;

	switch(item)
	{
	case 0x00: 
	case 0x01: 
	case 0x02: 
	case 0x03: 
	case 0x04:
	case 0x05:
		{
			DWORD simpleItemPtr = 0x000CB50;
			BYTE bLevel = 0;
			m_pFile->LoadData(&bLevel, 0x000CB72, 1);
			ptr = 
				( bLevel == LOBYTE(LOWORD(m_Level.iWorld)) ? 
					0x000CB57 : 
					0x000CB50 
				) + item;
			break;
		}
	case 0x10:							// big cloud
		{
			ptr = 0x000CCC2;
			m_pFile->LoadData(&nid.topright, 0x000CCC7, 1);	// small cheat for double tiles
			break;
		}
	case 0x11: ptr = 0x000CCCE; break;	// single cloud

	case 0x20:							// herbs and other simple objects
	case 0x21:
	case 0x22:
	case 0x23:
	case 0x24:
	case 0x25:
	case 0x26:
	case 0x27:
	case 0x28:
	case 0x29:
	case 0x2a:
	// case 0x2b:						// 1st space mushroom
	case 0x2c:
	// case 0x2d:						// 2nd space mushroom
	case 0x2e:
	case 0x2f: ptr = 0x000CD40 + ( item & 0x0F ); break;
	default: return nid;
	}

	type = 4;
	m_pFile->LoadData(&nid.topleft, ptr, 1);
	return nid;
}

NES_ITEM_DIM CNesEditor::GetHorzDim(int item, NES_ITEM_DIM nid, USHORT & type)
{
	switch(item)
	{
	case 0x0A:
	case 0x0B:
		{
			BYTE bLevel = 0;
			m_pFile->LoadData(&bLevel, 0x000CB8B, 1);
			if ( bLevel == LOBYTE(LOWORD(m_Level.iWorld)) )
			{
				m_pFile->LoadData(&nid.topleft, 0x000CF5A + ( item - 0x0A ), 1);
				m_pFile->LoadData(&nid.middle, 0x000CF5D + ( item - 0x0A ), 1);
				m_pFile->LoadData(&nid.topright, 0x000CF5F + ( item - 0x0A ), 1);
			}
			else
			{
				m_pFile->LoadData(&nid.topleft, 0x000CB81 + ( item - 0x0A ), 1);
				m_pFile->LoadData(&nid.middle, 0x000CB83 + ( item - 0x0A ), 1);
				m_pFile->LoadData(&nid.topright, 0x000CB85 + ( item - 0x0A ), 1);
			}
			break;
		}
	default: return nid;
	}

	type = 4;
	return nid;
}

NES_ITEM_DIM CNesEditor::GetEntrDim(int item, NES_ITEM_DIM nid, USHORT & type)
{
	switch(item)
	{
	case 9:		// doors
	case 10:
	case 11:
	case 28:
	case 29:
		{
			item = ( item > 16 ? item - 16 : item );
			m_pFile->LoadData(&nid.topleft, 0x000D026 + ( item - 9 ), 1);
			m_pFile->LoadData(&nid.bottomleft, 0x000D02B + ( item - 9 ), 1);
			break;
		}
	case 19:	// light entrance left
		{
			BYTE bLevel = 0;
			m_pFile->LoadData(&bLevel, 0x000CA75, 1);
			if ( bLevel == LOBYTE(LOWORD(m_Level.iWorld)) )
			{
				nid.topright = 0xFF;
				m_pFile->LoadData(&nid.topleft, 0x000CAB9, 1);
				m_pFile->LoadData(&nid.bottomleft, 0x000CAB9, 1);
				m_pFile->LoadData(&nid.top, 0x000CACE, 1);
				m_pFile->LoadData(&nid.bottomright, 0x000CACE, 1);
				m_pFile->LoadData(&nid.middle, 0x000CAC5, 1);
			}
			else
			{
				nid.topright = 0x00;
				m_pFile->LoadData(&nid.topleft, 0x000CA7E, 1);
				m_pFile->LoadData(&nid.bottomleft, 0x000CA8D, 1);
				m_pFile->LoadData(&nid.top, 0x000CA83, 1);
				m_pFile->LoadData(&nid.bottomright, 0x000CA97, 1);
				m_pFile->LoadData(&nid.middle, 0x000CA92, 1);
			}
			break;
		}
	case 20:	// light entrance right
		{
			m_pFile->LoadData(&nid.topright, 0x000CAE3, 1);
			m_pFile->LoadData(&nid.bottomright, 0x000CAF3, 1);
			m_pFile->LoadData(&nid.top, 0x000CAE8, 1);
			m_pFile->LoadData(&nid.bottomleft, 0x000CAFD, 1);
			m_pFile->LoadData(&nid.middle, 0x000CAF8, 1);			
			break;
		}
	default: return nid;
	}

	type = 4;
	return nid;
}