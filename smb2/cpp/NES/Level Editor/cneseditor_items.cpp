
#include "precomp.h"

CNesItem::CNesItem(PNES_LEVEL_ITEM lvl, POINT & pt, INT iLayer, INT iPage)
{
	if ( nliEntrance != lvl->nli && nliRegular != lvl->nli )
		throw CString(TEXT("Invalid object inserted for editor"));

	m_iPage = iPage;
	m_pt = pt;
	m_iLayer = iLayer;
	m_obj.pData[0] = lvl->Object.pData[1]; 
	m_obj.pData[1] = lvl->Object.pData[2]; 
	m_obj.pData[2] = lvl->Object.pData[3]; 
	m_obj.pData[3] = lvl->Object.pData[4]; 

	switch(lvl->nli)
	{
	case nliEntrance:
		{
			m_type = nitEntrance; 
			break;
		}
	case nliRegular:
		{
			m_type = nitRegular;
			break;
		}
	}

}

CNesItem::CNesItem(PNES_LEVEL_ENEMY enm, POINT & pt, INT iPage)
{
	m_type = nitEnemy;
	m_pt = pt;
	m_obj.Enemy.id = enm->Enemy.Object.id;
	m_iPage = iPage;
}

CNesItem::CNesItem(PNES_LEVEL_ITEM lvl, POINT & pt, INT iLayer, INT iPage, BYTE bgType)
{
	if ( nliGroundSet != lvl->nli )
		throw(TEXT("Invalid ground type inserted for editor"));

	m_iPage = iPage;
	m_type = nitGround;
	m_iLayer = iLayer;
	m_obj.Ground.uSets = lvl->Object.GroundSet.uSets;
	m_obj.Ground.uType = bgType;
	m_pt = pt;
}

CNesItem::CNesItem(PNES_LEVEL_ITEM x, INT iPage)
{
	m_type = nitPointer;
	m_iPage = iPage;
	m_obj.Pointer.uPtrData = x->Object.Pointer.uParameter[0] | (x->Object.Pointer.uParameter[1] << 8);
}

BOOL CNesItem::ChangeBgType(BYTE bgType)
{
	if ( nitGround != m_type )
		return FALSE;

	m_obj.Ground.uType = ( 0x07 & bgType );
	return TRUE;
}

NES_ITEM_TYPE CNesItem::Type() { return m_type; }
USHORT CNesItem::Ground() { if ( nitGround != m_type ) return 0; return m_obj.pData[0] | (m_obj.pData[1] << 8); }
BYTE CNesItem::Regular() { if ( nitEntrance != m_type && nitRegular != m_type ) return 0x0; return m_obj.General.id; }
BYTE CNesItem::RegularId() { if ( nitEntrance != m_type && nitRegular != m_type ) return 0x0; return ( m_obj.General.id < 48 ? m_obj.General.id : 48 + (m_obj.General.id - 48) / 16 ); }
BYTE CNesItem::Enemy()  { if ( nitEnemy != m_type ) return 0x0; return m_obj.Enemy.id & 0x7f; }
POINT CNesItem::Pos() { return m_pt; }
USHORT CNesItem::Pointer() { if ( nitPointer != m_type ) return -1; return m_obj.Pointer.uPtrData; }
INT CNesItem::Page() { return m_iPage; }
INT CNesItem::Layer() { return m_iLayer; }
BYTE CNesItem::EntranceParameter(INT i) { if ( nitEntrance != m_type || i < 0 || i > 3 ) return 0; return m_obj.Entrance.uEntranceParameter[i]; }
BOOL CNesItem::SetLayer(INT iLayer) { if ( nitEnemy == m_type ) return FALSE; m_iLayer = iLayer; return TRUE; }
BOOL CNesItem::SetPosition(UINT x, UINT y) { if ( nitPointer == m_type ) return FALSE; m_pt.x = x; m_pt.y = y; return TRUE; }
BOOL CNesItem::SetPage(INT iPage) { m_iPage = iPage; return TRUE; }
BOOL CNesItem::SetEntranceParam(BOOL fFarPointer, BYTE wParam, BYTE lParam)
{
	if ( nitEntrance != m_type )
		return FALSE;
	if ( fFarPointer )
	{
		m_obj.Entrance.uEntranceParameter[0] = 0xf5;
		m_obj.Entrance.uEntranceParameter[1] = wParam;
		m_obj.Entrance.uEntranceParameter[2] = lParam;
	}
	else
	{
		m_obj.Entrance.uEntranceParameter[0] = wParam;
		m_obj.Entrance.uEntranceParameter[1] = lParam;
	}
	return TRUE;
}
BOOL CNesItem::SetPtr(BYTE wParam, BYTE lParam) { if ( nitPointer != m_type ) return FALSE; m_obj.Pointer.uPtrData = (wParam | ( lParam << 8 )); return TRUE; }
