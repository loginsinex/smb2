
#include "precomp.h"

VOID CLvlDraw::AllocCanvas(UINT cx, UINT cy, INT fx)
{
	if ( !cx || !cy )
		return;

	if ( m_Canvas.pItems )
		delete m_Canvas.pItems;

	if ( m_Canvas.pEnemyItems )
		delete m_Canvas.pEnemyItems;

	if ( m_Canvas.pSpecial )
		delete m_Canvas.pSpecial;

	m_Canvas.pEnemyItems = new NES_GITEM[cx*cy];
	m_Canvas.pItems = new NES_GITEM[cx*cy];
	m_Canvas.pSpecial = new NES_GITEM[ (*m_pvEditor)[nlfDirection] ? cx*3 : 3*cy ];

	if ( !m_Canvas.pSpecial )
	{
		if ( m_Canvas.pEnemyItems ) delete m_Canvas.pEnemyItems;
		if ( m_Canvas.pItems ) delete m_Canvas.pItems;
		m_Canvas.sz.cx = m_Canvas.sz.cy = m_Canvas.szSpecial.cx = m_Canvas.sz.cy = 0;
		return;
	}

	if ( m_Canvas.pItems )
	{
		m_Canvas.sz.cx = cx;
		m_Canvas.sz.cy = cy;
		ZeroMemory(m_Canvas.pItems, cx*cy*sizeof(m_Canvas.pItems[0]));
		m_Canvas.fx = fx;
	}
	else
		m_Canvas.sz.cx = m_Canvas.sz.cy = m_Canvas.szSpecial.cx = m_Canvas.sz.cy = 0;

	if ( !m_Canvas.pItems && m_Canvas.pEnemyItems )
	{
		m_Canvas.sz.cx = m_Canvas.sz.cy = m_Canvas.szSpecial.cx = m_Canvas.sz.cy = 0;
		delete m_Canvas.pEnemyItems;
		m_Canvas.pEnemyItems = NULL;
		return;
	}
	else if ( m_Canvas.pItems && !m_Canvas.pEnemyItems )
	{
		m_Canvas.sz.cx = m_Canvas.sz.cy = m_Canvas.szSpecial.cx = m_Canvas.sz.cy = 0;
		delete m_Canvas.pItems;
		m_Canvas.pItems = NULL;
		return;
	}

	if ( m_Canvas.pSpecial )
	{
		m_Canvas.szSpecial.cx = ( (*m_pvEditor)[nlfDirection] ? cx : 3 );
		m_Canvas.szSpecial.cy = ( (*m_pvEditor)[nlfDirection] ? 3 : cy );
	}
}

VOID CLvlDraw::DestroyCanvas()
{
	if ( m_Canvas.pItems )
		delete m_Canvas.pItems;

	m_Canvas.pItems = NULL;
	m_Canvas.sz.cx = m_Canvas.sz.cy = 0;
	m_Canvas.fx = 0;


	if ( m_Canvas.pEnemyItems )
		delete m_Canvas.pEnemyItems;

	if ( m_Canvas.pSpecial )
		delete m_Canvas.pSpecial;

	m_Canvas.pItems = m_Canvas.pEnemyItems = m_Canvas.pSpecial = NULL;
	m_Canvas.szSpecial.cx = m_Canvas.szSpecial.cy = 0;
}

BOOL CLvlDraw::SetCanvasEnemyItem(PNES_GITEM pItem, UINT x, UINT y, BOOL fHiddenEnemy)
{
	if ( x >= (UINT) m_Canvas.sz.cx || y >= (UINT) m_Canvas.sz.cy )
		return FALSE;

	CNesItem nItem = m_pvEditor->LevelItem( pItem->Item.Item.id );

	if ( nitEnemy != nItem.Type() )
		return FALSE;

	BYTE enemy = nItem.Enemy(), cx = g_mEnemyDim[enemy].szxy & 0x0f, cy = (g_mEnemyDim[enemy].szxy >> 4) & 0x0f;
	if ( 0xff == g_mEnemyDim[enemy].szxy || enemy >= 0x80 )
		return FALSE;

	for(BYTE iy = 0; iy < cy; ++iy)
	{
		for(BYTE ix = 0; ix < cx; ++ix)
		{
			if ( (x+ix) >= (UINT) m_Canvas.sz.cx || (y+iy) >= (UINT) m_Canvas.sz.cy )
				continue;

			UINT c = (x+ix)+m_Canvas.sz.cx*(y+iy);
			m_Canvas.pEnemyItems[c].fVisible = TRUE;
			m_Canvas.pEnemyItems[c].idTile = g_mEnemyDim[enemy].xy + (ix | (iy << 4));
			m_Canvas.pEnemyItems[c].Item.Item.id = pItem->Item.Item.id;
			m_Canvas.pEnemyItems[c].Item.Item.type = 0;
			m_Canvas.pEnemyItems[c].fHidden = fHiddenEnemy;
		}
	}

	return TRUE;
}

BOOL CLvlDraw::GetCanvasEnemyItem(PNES_GITEM pItem, UINT x, UINT y)
{
	if ( x >= (UINT) m_Canvas.sz.cx || y >= (UINT) m_Canvas.sz.cy )
		return FALSE;

	(*pItem) = m_Canvas.pEnemyItems[x+m_Canvas.sz.cx*y];
	return TRUE;
}

BOOL CLvlDraw::EraseItemsCanvas()
{
	if ( !m_Canvas.pItems )
		return FALSE;

	ZeroMemory(m_Canvas.pItems, m_Canvas.sz.cx * m_Canvas.sz.cy * sizeof(m_Canvas.pItems[0]));

	if ( m_Canvas.pSpecial )
		ZeroMemory(m_Canvas.pSpecial, m_Canvas.szSpecial.cx * m_Canvas.szSpecial.cy * sizeof(m_Canvas.pSpecial[0]));

	return TRUE;
}

BOOL CLvlDraw::EraseEnemyCanvas()
{
	if ( !m_Canvas.pEnemyItems )
		return FALSE;

	ZeroMemory(m_Canvas.pEnemyItems, m_Canvas.sz.cx * m_Canvas.sz.cy * sizeof(m_Canvas.pEnemyItems[0]));
	return TRUE;
}

BOOL CLvlDraw::SetCanvasItem(PNES_GITEM pItem, UINT x, UINT y)
{
	if ( x >= (UINT) m_Canvas.sz.cx || y >= (UINT) m_Canvas.sz.cy )
		return FALSE;

	int c = x+m_Canvas.sz.cx*y;

	
	if ( m_Canvas.pItems[c].fVisible )
	{
		// check priority
		USHORT id = m_Canvas.pItems[c].Item.Item.id, bg = ( 3 & m_Canvas.pItems[c].Item.Item.type );
		USHORT myid = pItem->Item.Item.id, mybg = ( 3 & pItem->Item.Item.type );
		BYTE regular_id = 0, myregular_id = 0, bg_id = 0, mybg_id = 0;
		if ( !bg ) { regular_id = m_pvEditor->LevelItem(id).Regular(); regular_id = CONVERT_REGULAR(regular_id); }
		else bg_id = ( m_pvEditor->LevelItem(id).Ground() >> 8 ) & 0xff;
		if ( !mybg ) { myregular_id = m_pvEditor->LevelItem(myid).Regular(); myregular_id = CONVERT_REGULAR(myregular_id); }
		else mybg_id = ( m_pvEditor->LevelItem(myid).Ground() >> 8 ) & 0xff;

		if ( !bg && mybg && BG_PRIOR(regular_id, mybg, m_Canvas.fx, mybg_id, (*m_pvEditor)[nlfDirection]) ) //  !g_mPriorityList[regular_id].bg.Tile.Tile[bg] )
			return FALSE;		// dont draw background over front [0] object

		if ( bg && !mybg && BG_PRIOR(myregular_id, bg, m_Canvas.fx, bg_id, (*m_pvEditor)[nlfDirection]) ) // g_mPriorityList[myregular_id].bg.Tile.Tile[bg] )
			return FALSE;		// dont draw back [1] object over background

		if ( !bg && !mybg && g_mPriorityList[regular_id].bPriority < g_mPriorityList[myregular_id].bPriority )
			return FALSE;		// dont draw back [1] object over front [0] object
	}

	m_Canvas.pItems[c] = (*pItem);
	m_Canvas.pItems[c].fVisible = TRUE;
	return TRUE;
}

BOOL CLvlDraw::SetSpecialCanvasItem(UINT x, UINT y, PNES_GITEM nItem)
{
	CNesItem npItem = m_pvEditor->LevelItem(nItem->Item.Item.id);
	if ( npItem.Type() != nitGround && npItem.Type() != nitPointer )
		return FALSE;

	switch(npItem.Type())
	{
	case nitGround:
		{
			if ( (*m_pvEditor)[nlfDirection] )
			{
				if ( npItem.Pos().x >= m_Canvas.szSpecial.cx ) return FALSE;
				PNES_GITEM pItem = &m_Canvas.pSpecial[npItem.Pos().x+1*m_Canvas.szSpecial.cx];
				pItem->fVisible = TRUE;
				pItem->idTile = 0xf0;
				pItem->Item.Item.id = nItem->Item.Item.id;
			}
			else
			{
				if ( npItem.Pos().y >= m_Canvas.szSpecial.cy ) return FALSE;
				PNES_GITEM pItem = &m_Canvas.pSpecial[npItem.Pos().y*m_Canvas.szSpecial.cx+1];
				pItem->fVisible = TRUE;
				pItem->idTile = 0xf0;
				pItem->Item.Item.id = nItem->Item.Item.id;
			}
			break;
		}
	case nitPointer:
		{
			if ( (*m_pvEditor)[nlfDirection] )
			{
				if ( (INT) x >= m_Canvas.szSpecial.cx ) return FALSE;
				PNES_GITEM pItem = &m_Canvas.pSpecial[x+2*m_Canvas.szSpecial.cx];
				pItem->fVisible = TRUE;
				pItem->idTile = 0xf5;
				pItem->Item.Item.id = nItem->Item.Item.id;
			}
			else
			{
				if ( (INT) y >= m_Canvas.szSpecial.cy ) return FALSE;
				PNES_GITEM pItem = &m_Canvas.pSpecial[y*m_Canvas.szSpecial.cx+2];
				pItem->fVisible = TRUE;
				pItem->idTile = 0xf5;
				pItem->Item.Item.id = nItem->Item.Item.id;
			}
			break;
		}
	}
	return TRUE;
}

BOOL CLvlDraw::IsCanvasLineContainLayerLimit(UINT x, UINT y, UINT cx, INT iLayer)
{
	if ( (INT) x >= m_Canvas.sz.cx || (INT) y >= m_Canvas.sz.cy )
		return FALSE;

	if ( iLayer < 1 ) 
		return FALSE;

	UINT rx = min(x+cx, (UINT) m_Canvas.sz.cx);
	for(UINT ix=x;ix<rx;++ix)
	{
		UINT c = ix+y*m_Canvas.sz.cx;
		if ( m_Canvas.pItems[c].fVisible && 0 == ( 3 & m_Canvas.pItems[c].Item.Item.type ) )
		{
			CNesItem nItem = m_pvEditor->LevelItem(m_Canvas.pItems[c].Item.Item.id);
			BYTE regular = nItem.Regular();
			regular = CONVERT_REGULAR(regular);

			if ( nItem.Layer() < iLayer && g_mLayerLimiter[regular] )
				return TRUE;
		}
	}
	return FALSE;
}

BOOL CLvlDraw::SetCanvasNullItem(UINT x, UINT y)
{
	if ( (INT) x >= m_Canvas.sz.cx || (INT) y >= m_Canvas.sz.cy )
		return FALSE;

	int c = x+m_Canvas.sz.cx*y;
	ZeroMemory(&m_Canvas.pItems[c], sizeof(m_Canvas.pItems[c]));
	return TRUE;
}

BOOL CLvlDraw::IsCanvasItemUsed(UINT x, UINT y)
{
	if ( (INT) x >= m_Canvas.sz.cx || (INT) y >= m_Canvas.sz.cy )
		return FALSE;

	return m_Canvas.pItems[x+m_Canvas.sz.cx*y].fVisible;
}

BOOL CLvlDraw::GetCanvasItem(UINT x, UINT y, PNES_GITEM nItem)
{
	if ( (INT) x >= m_Canvas.sz.cx || (INT) y >= m_Canvas.sz.cy )
		return FALSE;

	
	*nItem = m_Canvas.pItems[x+m_Canvas.sz.cx*y];
	return TRUE;
}
	
BOOL CLvlDraw::GetSpecialCanvasItem(UINT x, UINT y, PNES_GITEM nItem)
{
	(*m_pvEditor)[nlfDirection] ? y -= 0x0f : x -= 0x10;
	if ( (INT) x >= m_Canvas.szSpecial.cx || (INT) y >= m_Canvas.szSpecial.cy )
		return FALSE;

	UINT c = x+y*m_Canvas.szSpecial.cx;

	if ( (*m_pvEditor)[nlfDirection] && y < 1 ||
		 !(*m_pvEditor)[nlfDirection] && x < 1 )
			return FALSE;

	if ( !m_Canvas.pSpecial[c].fVisible )
		return FALSE;

	*nItem = m_Canvas.pSpecial[c];
	return TRUE;
}

BOOL CLvlDraw::DrawSpecialCanvas()
{
	if ( !m_Canvas.pSpecial )
		return FALSE;

	for(INT cy=0; cy<m_Canvas.szSpecial.cy; ++cy)
	{
		for(INT cx=0; cx<m_Canvas.szSpecial.cx; ++cx)
		{
			UINT c = cx+cy*m_Canvas.szSpecial.cx;
			if ( m_Canvas.pSpecial[c].fVisible )
			{
				BYTE tileX = m_Canvas.pSpecial[c].idTile & 0x0F, tileY = ( m_Canvas.pSpecial[c].idTile >> 4 ) & 0x0F;
				DrawSpecialGamma(0, 0, tileX, tileY, 1, 1, cx, cy, ( m_Catch.dwCatchedObject > 0 && m_Catch.dwCatchedObject == m_Canvas.pSpecial[c].Item.Item.id ));
			}
		}
	}

	if ( (*m_pvEditor)[nlfDirection] )
	{
		for(INT cx = 0; cx < m_Canvas.szSpecial.cx; cx += 0x10)
			m_Bmp.bmSpecial.FillRect(cx*m_dcsz.cx, 0, 2, 0x10, RGB(255, 255, 255), 0, 0);
	}
	else
	{
		for(INT cy = 0; cy < m_Canvas.szSpecial.cy; cy += 0x0f)
			m_Bmp.bmSpecial.FillRect(0, cy*m_dcsz.cy, 0x10, 2, RGB(255, 255, 255), 0, 0);
	}
	return TRUE;
}

BOOL CLvlDraw::DrawCanvas()
{
	BOOL fHilitePresent = ( int(m_Canvas.vHilite.size()) >= m_Canvas.sz.cx * m_Canvas.sz.cy );

	for(INT cy=0; cy<m_Canvas.sz.cy;++cy)
	{
		for(INT cx=0; cx<m_Canvas.sz.cx;++cx)
		{
			UINT c = cx+cy*m_Canvas.sz.cx;
			BOOL fHilite = ( fHilitePresent ? m_Canvas.vHilite[ c ] : FALSE );

			if ( m_Canvas.pItems[c].fVisible )
			{
				BYTE tileX = m_Canvas.pItems[c].idTile & 0x0F, tileY = ( m_Canvas.pItems[c].idTile >> 4 ) & 0x0F;
				

				if ( m_Canvas.pItems[c].Item.Item.type )
				{
					DrawGrGamma(m_Canvas.pItems[c].idTile, cx, cy, ( m_Catch.dwCatchedObject > 0 && m_Catch.dwCatchedObject == m_Canvas.pItems[c].Item.Item.id ), fHilite);
				}
				else
				{
					DrawGamma(0, 0, tileX, tileY, 1, 1, cx, cy, ( m_Catch.dwCatchedObject > 0 && m_Catch.dwCatchedObject == m_Canvas.pItems[c].Item.Item.id ), fHilite);
				}
			}
			else if ( fHilite )
				DrawGamma(0, 0, 5, 7, 1, 1, cx, cy, 0, fHilite);

			if ( m_Canvas.pEnemyItems[c].fVisible )
			{
				BYTE tileX = m_Canvas.pEnemyItems[c].idTile & 0x0F, tileY = ( m_Canvas.pEnemyItems[c].idTile >> 4 ) & 0x0F;
				Draw(m_Canvas.eColor, tileX, tileY, 1, 1, cx, cy, ( m_Catch.dwCatchedObject > 0 && m_Catch.dwCatchedObject == m_Canvas.pEnemyItems[c].Item.Item.id ));
			}
		}
	}

	// if ( fHilitePresent )
	//	m_Canvas.vHilite.clear();

	return DrawSpecialCanvas();
}