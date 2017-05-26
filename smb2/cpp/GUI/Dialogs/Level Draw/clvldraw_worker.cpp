
#include "precomp.h"


BOOL CLvlDraw::DrawLevelEx(DWORD dwCurLevel, CNesEditor * ed)
{
	if ( !ed || ( ed && -1 == ed->GetCurWorld() ) )
	{
		m_pvEditor = NULL;
		m_dcsz.cx = m_dcsz.cy = 0;
		DestroyCanvas();
		m_Bmp.bmBuffer.Create(1, 1);
		m_Bmp.bmLevel.Create(1, 1);
		m_Bmp.bmSpecial.Create(1, 1);
		OnResize();

		return FALSE;
	}

	UINT
		uLevelLen = (*ed)[nlfLength]+1,
		fHorizontal = (*ed)[nlfDirection],
		uColor = (*ed)[nlfColor];


	m_pvEditor = ed;
	
	m_Canvas.vHilite.clear();

	m_Catch.dwCatchedObject = 0;
	m_Catch.fCatchEnemies = FALSE;
	m_Catch.fObjectCatched = FALSE;
	m_Catch.ptDelta.x = m_Catch.ptDelta.y = 0;

	INT fx = 0, gfx = ed->GetFX();
	switch(dwCurLevel/30)
	{
	case 0: fx = 3; break;
	case 1: fx = 2; break;
	case 2: fx = 3; break;
	case 3: fx = 1; break;
	case 4: fx = 3; break;
	case 5: fx = 2; break;
	case 6: fx = 0; break;
	default: fx = 0; break;
	}

	m_Canvas.eColor = 3 - (*ed)[nlfEnemyColor];
	AllocCanvas(( fHorizontal ? 0x10*uLevelLen : 0x10 ) , ( fHorizontal ? 0x0f : 0x0f*uLevelLen ), fx);
	UINT cItems = (*ed).SizeOfItemsList();

	// draw bg only
	for(UINT i = 0; i < cItems; ++i)
	{
		if ( (*ed).LevelItem(i).Type() == nitGround )
			DrawObjectEx(i, fx, (*ed)[nlfObjectType], (*ed).LevelItem(i));
	}

	// draw objects
	for(UINT i = 0; i < cItems; ++i)
	{
		if ( (*ed).LevelItem(i).Type() != nitGround )
			DrawObjectEx(i, fx, (*ed)[nlfObjectType], (*ed).LevelItem(i));
	}
	
	InitSize(0, ( fHorizontal ? uLevelLen*0x10 : 0x10 ) , ( fHorizontal ? 0x0f : uLevelLen*0x0f ), 16, 16);
	UseGamma(uColor, fx+4, gfx+10);
	UseEnemyMask(8, 3);
	DrawCanvas();
	RedrawBuffer();
	InvalidateRect(m_hWnd, NULL, TRUE);
	return TRUE;
}

BOOL CLvlDraw::RedrawLevel(BOOL fEraseEnemies)
{
	if ( !m_pvEditor || !m_Canvas.sz.cx || !m_Canvas.sz.cy )
		return FALSE;

	UINT cItems = m_pvEditor->SizeOfItemsList();

	EraseItemsCanvas();

	if ( fEraseEnemies )
		EraseEnemyCanvas();

	std::vector<DWORD> pGrounds;
	pGrounds.insert(pGrounds.end(), m_pvEditor->SizeOfItemsList(), 0);
	UINT wGround = 0;
	// draw bg only
	for(UINT i = 0; i < cItems; ++i)
	{
		if ( m_pvEditor->LevelItem(i).Type() == nitGround )
			pGrounds[wGround++] = i;
	}

	// sort grounds by x nor y
	BOOL fSortInProgress = FALSE, fHorizontal = (*m_pvEditor)[nlfDirection];
	do
	{
		fSortInProgress = FALSE;
		for(DWORD i = 0; i < wGround - 1; ++i)
		{
			CNesItem pGr1 = m_pvEditor->LevelItem(pGrounds[i]);
			CNesItem pGr2 = m_pvEditor->LevelItem(pGrounds[i+1]);
			if ( ( fHorizontal && pGr1.Pos().x > pGr2.Pos().x )
				|| ( !fHorizontal && pGr1.Pos().y > pGr2.Pos().y ) )
			{
				DWORD myswap = pGrounds[i]; pGrounds[i] = pGrounds[i+1]; pGrounds[i+1] = myswap;
				fSortInProgress = TRUE;
			}
		}
	} while(fSortInProgress);

	for(DWORD i = 0; i < wGround; ++i)
		DrawObjectEx((USHORT) pGrounds[i], (USHORT) m_Canvas.fx, (*m_pvEditor)[nlfObjectType], m_pvEditor->LevelItem(pGrounds[i]));


	// calculates layers
	DWORD cLayers = m_pvEditor->CountLayers();

	std::vector<DWORD> lpdwLayer; lpdwLayer.insert(lpdwLayer.begin(), cLayers, 0);
	std::vector<USHORT> lpdwLayerObj; lpdwLayerObj.insert(lpdwLayerObj.begin(), cItems * cLayers, 0);

	for(UINT i = 0; i < cItems; ++i)
	{
		DWORD layer = m_pvEditor->LevelItem(i).Layer();
		lpdwLayerObj[cItems*layer+lpdwLayer[layer]++] = i;
	}
	
	// draw objects by layers
	for(DWORD l = 0; l < cLayers; ++l)
	{
		for(DWORD i = 0; i < lpdwLayer[l]; ++i)
			if ( m_pvEditor->LevelItem(lpdwLayerObj[l*cItems+i]).Type() != nitGround )
				DrawObjectEx(lpdwLayerObj[l*cItems+i], (USHORT) m_Canvas.fx, (*m_pvEditor)[nlfObjectType], (*m_pvEditor).LevelItem(lpdwLayerObj[l*cItems+i]));
	}
	
	m_Bmp.bmLevel.FillRect(0, 0, m_Bmp.bmLevel.Width(), m_Bmp.bmLevel.Height(), m_crBg, 0, 0);
	m_Bmp.bmSpecial.FillRect(0, 0, m_Bmp.bmSpecial.Width(), m_Bmp.bmSpecial.Height(), 0, 0, 0);

	BOOL fResult = DrawCanvas();
	RedrawBuffer(FALSE);
	InvalidateRect(m_hWnd, NULL, TRUE);
	return fResult;
}

BOOL CLvlDraw::DrawObjectEx(USHORT item, USHORT fx, USHORT uObjectType, CNesItem & x)
{
	m_vDim = (PNES_ITEM_DIM) g_mItemDim;

	switch(x.Type())
	{
	case nitPointer:
		{
			NES_GITEM nPtrItem = { 0 };
			POINT pos = { (*m_pvEditor)[nlfDirection] ? 0x10*x.Page() : 0, (*m_pvEditor)[nlfDirection] ? 0 : 0x0f*x.Page() };
			nPtrItem.Item.Item.id = item;
			SetSpecialCanvasItem(pos.x, pos.y, &nPtrItem);
			break; // skip
		}
	case nitEnemy:
		{
			UINT dx = 0, dy = 0;

			NES_GITEM nitem = { 0 };
			nitem.Item.Item.id = item;
			if ( ( m_pvEditor->GetCurLevel() % 10 ) == 4 )
				if ( (*m_pvEditor)[nlfDirection] )
					dx = 0x10 * 10;
				else
					dy = 0x10 * 10;

			if ( dx || dy )
				SetCanvasEnemyItem(&nitem, x.Pos().x - dx, x.Pos().y - dy, TRUE);
			
			SetCanvasEnemyItem(&nitem, x.Pos().x, x.Pos().y);
			break; 
		}
	case nitEntrance: return DrawSpecialObjectEx(item, fx, x); break;
	case nitGround: return DrawGroundEx(item, fx, x); break;
	case nitRegular:
		{
			switch(x.Regular())
			{
			case 6:
			case 7:
			case 8:
			case 12:
			case 13:
			case 15:
			case 22:
				return DrawVertObjectEx(item, fx, x, g_mLayerLimiting[x.Regular()]);

			case 18:
				return DrawVertObjectEx(item, fx, x, g_mLayerLimiting[x.Regular()], TRUE); 
			
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 17:
//			case 28:		-- why castle entrance here ?!
//			case 29:
			case 32:
			case 33:
			case 34:
			case 35:
			case 36:
			case 37:
			case 38:
			case 39:
			case 40:
			case 41:
			case 42:
			case 43:
			case 44:
			case 45:
			case 46:
			case 47:
				return DrawSingleObjectEx(item, fx, x);

			case 24:
			case 25:
				return DrawMasvObjectEx(item, fx, x, g_mLayerLimiting[x.Regular()]);

			case 14:
			case 16:
			case 23:
			case 30:
			case 31:
				return DrawSpecialObjectEx(item, fx, x);

			default:
				{
					m_vDim = (PNES_ITEM_DIM) &g_msItemDim[13*16*fx+13*uObjectType];
					BYTE vid = (x.Regular()-0x30)/0x10;
					switch(vid)
					{
					case 0: 
					case 1:
					case 2:
					case 3:
					case 4:
						return DrawHorzGrObjectEx(item, x);
					case 5:
					case 6:
					case 7:
						return DrawVertGrObjectEx(item, x);
					case 10:
					case 11:
						return DrawHorzObjectEx(item, fx, x);
					case 12:
					case 9:
					case 8:
						return DrawMasvObjectEx(item, fx, x, g_mLayerLimiting[vid+0x30]);
					}
				}
			}
			break;
		}
	}
	return FALSE;
}

BOOL CLvlDraw::DrawSingleObjectEx(USHORT item, USHORT fx, CNesItem & x)
{
	NES_GITEM pItem = { 0 };
	NES_ITEM_DIM vDim = m_pvEditor->GetSingDim(x.Regular(), m_vDim[x.Regular()], pItem.Item.Item.type);
	pItem.Item.Item.id = (USHORT) item;
	pItem.idTile = vDim.topleft;
	SetCanvasItem(&pItem, x.Pos().x, x.Pos().y);
	return FALSE;
}

BOOL CLvlDraw::DrawHorzObjectEx(USHORT item, USHORT fx, CNesItem & x)
{
	POINT pos = x.Pos();
	BYTE id = ( x.Regular() >= 0x30 ? (x.Regular()-0x30)/0x10 : x.Regular() ), size = ( x.Regular()-0x30 ) & 0x0F;

	NES_GITEM pItem = { 0 };
	NES_ITEM_DIM vDim = m_pvEditor->GetHorzDim(id, m_vDim[id], pItem.Item.Item.type);
	pItem.idTile = vDim.topleft;
	pItem.Item.Item.id = item;
	SetCanvasItem(&pItem, pos.x, pos.y);
	
	// size of item 
	pItem.idTile = vDim.topright;
	SetCanvasItem(&pItem, pos.x+size, pos.y);

	pItem.idTile = vDim.middle;
	for(BYTE i=1; i<size; ++i)
		SetCanvasItem(&pItem, pos.x+i, pos.y);
	
	return TRUE;
}

BOOL CLvlDraw::DrawVertObjectEx(USHORT item, USHORT fx, CNesItem & x, BOOL fLimitByLayer, BOOL fDirection)
{
	INT iLayer = x.Layer();
	POINT pos = x.Pos();
	BYTE id = x.Regular(), size = ( id >= 0x30 ? ( id & 0x0F ) : (BYTE) (0x0f*((pos.y+0x0f)/0x0f) - pos.y - 1) );
	NES_GITEM pItem = { 0 };
	id = ( id > 0x30 ? (id-0x30)/0x10 : id );

	NES_ITEM_DIM vDim = m_pvEditor->GetVertDim(id, m_vDim[id], pItem.Item.Item.type);

	if ( !fDirection )
	{
		BOOL fIsVine = FALSE;

		switch(id)
		{
		case 0x0C:
		case 0x0D:
		case 0x0F:
			{
				size += 0x0F;
				fIsVine = TRUE;
				break;
			}
		}

		pItem.idTile = vDim.topleft;
		pItem.Item.Item.id = item;
		SetCanvasItem(&pItem, pos.x, pos.y);
	
		pItem.idTile = vDim.middle;
		for(BYTE i=1; i<size; ++i)
		{
			int y = pos.y + ( fDirection ? -1 : 1 )*i;
			NES_GITEM ng = { 0 };
			GetCanvasItem(pos.x, y, &ng);

			if ( iLayer > 0 && fLimitByLayer && IsCanvasLineContainLayerLimit(pos.x, pos.y+i, 1, iLayer) ||
				( fIsVine && ng.fVisible && ng.idTile != 0x40 && ng.Item.Item.type ) ) 
				return TRUE;

			SetCanvasItem(&pItem, pos.x, y);
		}
		// size of item 
		pItem.idTile = vDim.bottomleft;
		SetCanvasItem(&pItem, pos.x, pos.y+size);
	}
	else
	{
		pItem.idTile = vDim.bottomleft;
		pItem.Item.Item.id = item;
		SetCanvasItem(&pItem, pos.x, pos.y);

		int minY = max(pos.y - 0x0f, 0);
		int yStart = pos.y - 1;

		if ( 0x12 == id && !(*m_pvEditor)[nlfDirection] && minY > 0x0 )
			minY = ( ( 1 + minY / 0x0F ) * 0x0F );

		pItem.idTile = vDim.middle;
		for(int cy = pos.y - 1; cy > minY; --cy)
		{
			NES_GITEM ng = { 0 };
			GetCanvasItem(pos.x, cy, &ng);

			if ( iLayer > 0 && fLimitByLayer && IsCanvasLineContainLayerLimit(pos.x, cy, 1, iLayer) ||
				ng.fVisible && ng.idTile != 0x40 && ng.Item.Item.type )
				return TRUE;



			SetCanvasItem(&pItem, pos.x, cy);
		}

		pItem.idTile = vDim.topleft;
		pItem.Item.Item.id = item;
		SetCanvasItem(&pItem, pos.x, minY);
	}
	return TRUE;
}

BOOL CLvlDraw::DrawHorzGrObjectEx(USHORT item, CNesItem & x)
{
	POINT pos = x.Pos();
	BYTE id = ( x.Regular() >= 0x30 ? (x.Regular()-0x30)/0x10 : x.Regular() ), size = ( x.Regular()-0x30 ) & 0x0F;

	NES_GITEM pItem = { 0 };
	pItem.idTile = m_pvEditor->GetObjTile(id);
	pItem.Item.Item.id = item;
	pItem.Item.Item.type = 4;

	for(BYTE i=0; i<=size; ++i)
		SetCanvasItem(&pItem, pos.x+i, pos.y);
	
	return TRUE;
}

BOOL CLvlDraw::DrawVertGrObjectEx(USHORT item, CNesItem & x, BOOL fLimitByLayer)
{
	INT iLayer = x.Layer();
	POINT pos = x.Pos();
	BYTE id = x.Regular(), size = ( id >= 0x30 ? ( id & 0x0F ) : (BYTE) (0x0f*((pos.y+0x0f)/0x0f) - pos.y) );
	NES_GITEM pItem = { 0 };
	id = ( id > 0x30 ? (id-0x30)/0x10 : id );

	pItem.Item.Item.id = item;
	pItem.Item.Item.type = 4;
	pItem.idTile = m_pvEditor->GetObjTile(id);
	
	for(BYTE i=1; i<=size; ++i)
	{
		if ( iLayer > 0 && fLimitByLayer && IsCanvasLineContainLayerLimit(pos.x, pos.y+i, 1, iLayer) )
			return TRUE;

		SetCanvasItem(&pItem, pos.x, pos.y + i);
	}
	
	// except for 0x06 object. Up of this object has 0x9f tile!
	if ( 0x06 == id )
		pItem.idTile = 0x9f;

	SetCanvasItem(&pItem, pos.x, pos.y);

	return TRUE;
}

BOOL CLvlDraw::DrawMasvObjectEx(USHORT item, USHORT fx, CNesItem & x, BOOL fLimitByLayer)
{
	BYTE sizeX = ( x.Regular() >= 0x30 ? ((x.Regular()-0x30) & 0x0F) : 0x5 ), sizeY = 0x0E;
	INT iLayer = x.Layer();

	NES_GITEM pItem = { 0 };
	POINT pos = x.Pos();
	BYTE idRegular = ( x.Regular() >= 0x30 ? (x.Regular()-0x30) / 0x10 : x.Regular() );
	pItem.Item.Item.type = 0;
	NES_ITEM_DIM vDim = m_pvEditor->GetMasvDim(idRegular, m_vDim[idRegular], pItem.Item.Item.type);
	pItem.Item.Item.id = item;
	

	pItem.idTile = vDim.topright;
	SetCanvasItem(&pItem, pos.x+sizeX, pos.y);
	pItem.idTile = vDim.topleft;
	SetCanvasItem(&pItem, pos.x, pos.y);
	pItem.idTile = vDim.top;
	for(BYTE cx=1; cx<sizeX;++cx)
		SetCanvasItem(&pItem, pos.x+cx, pos.y);
	
	for(BYTE cy=1; cy<=sizeY;++cy)
	{
		// scan this line for minor layer
		if ( iLayer > 0 && 
			fLimitByLayer && 
			IsCanvasLineContainLayerLimit(pos.x, pos.y + cy, sizeX, iLayer) 
			)
				break;

		for(BYTE cx=0;cx<=sizeX;++cx)
		{
			pItem.idTile = ( 0 == cx ? vDim.left : ( cx < sizeX ? vDim.middle : vDim.right ) );
			SetCanvasItem(&pItem, pos.x+cx, pos.y+cy);
		}
	}

	return TRUE;
}

BOOL CLvlDraw::DrawSpecialObjectEx(USHORT item, USHORT fx, CNesItem & x)
{
	NES_GITEM pItem = { 0 };
	POINT pos = x.Pos();
	if ( nitEntrance == x.Type() )
	{
		switch(x.Regular())
		{
		case 9:		// doors
		case 10:
		case 11:
		case 28:
		case 29:
			{
				pItem.Item.Item.type = 0;
				NES_ITEM_DIM vDim = 
					m_pvEditor->GetEntrDim(x.Regular(), m_vDim[x.Regular()], pItem.Item.Item.type);

				pItem.Item.Item.id = item;				
				pItem.idTile = vDim.topleft;
				SetCanvasItem(&pItem, pos.x, pos.y);
				pItem.idTile = vDim.bottomleft;				
				SetCanvasItem(&pItem, pos.x, pos.y+1);
				return TRUE;
			}
		case 19:	// light entrance left
			{
				pItem.Item.Item.type = 0;
				NES_ITEM_DIM vDim = 
					m_pvEditor->GetEntrDim(x.Regular(), m_vDim[x.Regular()], pItem.Item.Item.type);

				pItem.Item.Item.id = item;
				pItem.idTile = vDim.topleft;
				SetCanvasItem(&pItem, pos.x, pos.y);
				pItem.idTile = vDim.bottomleft;				
				SetCanvasItem(&pItem, pos.x, pos.y+1);
				pItem.idTile = vDim.top;
				SetCanvasItem(&pItem, pos.x+1, pos.y);
				pItem.idTile = vDim.middle;				
				SetCanvasItem(&pItem, pos.x+1, pos.y+1);
				pItem.idTile = vDim.bottomright;				
				SetCanvasItem(&pItem, pos.x+2, pos.y+1);
				return TRUE;
			}
		case 20:
			{
				pItem.Item.Item.type = 0;
				NES_ITEM_DIM vDim = 
					m_pvEditor->GetEntrDim(x.Regular(), m_vDim[x.Regular()], pItem.Item.Item.type);

				pItem.Item.Item.id = item;
				pItem.idTile = vDim.bottomleft;				
				SetCanvasItem(&pItem, pos.x-2, pos.y+1);
				pItem.idTile = vDim.top;
				SetCanvasItem(&pItem, pos.x-1, pos.y);
				pItem.idTile = vDim.middle;				
				SetCanvasItem(&pItem, pos.x-1, pos.y+1);
				pItem.idTile = vDim.topright;
				SetCanvasItem(&pItem, pos.x, pos.y);
				pItem.idTile = vDim.bottomright;				
				SetCanvasItem(&pItem, pos.x, pos.y+1);
				break;
			}
		case 21:
		case 30:	// desert entrance
			{
				return DrawDesertEntrance(item, fx, x);
			}
		}
	}
	else
	{
		switch(x.Regular())
		{
		case 14: // star back
			return DrawStarBg(item, fx, x);

		case 16: //cloud
			{
				pItem.Item.Item.type = 0;
				NES_ITEM_DIM vDim = 
					m_pvEditor->GetSingDim(x.Regular(), m_vDim[x.Regular()], pItem.Item.Item.type);

				pItem.Item.Item.id = item;				
				pItem.idTile = vDim.topleft;
				SetCanvasItem(&pItem, x.Pos().x, x.Pos().y);
				pItem.idTile = vDim.topright;
				SetCanvasItem(&pItem, x.Pos().x+1, x.Pos().y);
				break;
			}
		case 23:	// pyramid
			return DrawPyramidEx(item, fx, x);

		case 31:	// red background
			return DrawRedBg(item, fx, x);
		
		default:
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CLvlDraw::DrawPyramidEx(USHORT item, USHORT fx, CNesItem & x)
{
	NES_GITEM pItem = { 0 };
	pItem.idTile = 0xFB;
	pItem.Item.Item.id = item;
	pItem.Item.Item.type = 0;
	SetCanvasItem(&pItem, x.Pos().x, x.Pos().y);
	return TRUE;
}

BOOL CLvlDraw::DrawDesertEntrance(USHORT item, USHORT fx, CNesItem & x)
{
	NES_GITEM pItem = { 0 };
	pItem.idTile = 0xFC;
	pItem.Item.Item.id = item;
	pItem.Item.Item.type = 0;
	SetCanvasItem(&pItem, x.Pos().x, x.Pos().y);
	return FALSE;
}

BOOL CLvlDraw::DrawRedBg(USHORT item, USHORT fx, CNesItem & x)
{
	NES_GITEM pItem = { 0 };
	pItem.idTile = 0xFD;
	pItem.Item.Item.id = item;
	pItem.Item.Item.type = 0;
	SetCanvasItem(&pItem, x.Pos().x, x.Pos().y);
	return FALSE;
}

BOOL CLvlDraw::DrawStarBg(USHORT item, USHORT fx, CNesItem & x)
{
	NES_GITEM pItem = { 0 };
	pItem.idTile = 0xFE;
	pItem.Item.Item.id = item;
	pItem.Item.Item.type = 0;
	SetCanvasItem(&pItem, x.Pos().x, x.Pos().y);
	return FALSE;
}



BOOL CLvlDraw::DrawGroundEx(USHORT item, USHORT fx, CNesItem & x)
{
	if ( nitGround != x.Type() )
		return FALSE;


	POINT pos = x.Pos();
	USHORT uGround = x.Ground();
	BYTE uType = (( uGround >> 8 ) & 0xFF) & 0x7, uSet = ( uGround & 0x1F );

	// dont draw marker for level bg definition
	if ( item > 0 )
	{
		NES_GITEM nGroundItem = { 0 };
		nGroundItem.Item.Item.id = item;
		SetSpecialCanvasItem(pos.x, pos.y, &nGroundItem);
	}

	if ( (*m_pvEditor)[nlfDirection] )
	{
		DWORD dwGroundSet = m_pvEditor->GetBgSet(uSet); // g_mGroundSet[uSet];
		if ( 0x1f == uSet && 0 == item ) // level bg for 0x1f are skipped
			return TRUE;

		NES_GROUND_DIM ngdType = g_mGroundType[uType+fx*8];

		for(INT cx = pos.x; cx < m_Canvas.sz.cx; ++cx)
		{
			INT bit = 8*sizeof(dwGroundSet) - 2;
			for(INT cy=0;cy<m_Canvas.sz.cy;++cy)
			{
				NES_GITEM pItem = { 0 };
				pItem.Item.Item.id = item;
				BYTE bitset = ( dwGroundSet >> bit) & 0x03;
//				bitset = ( bitset >= 1 && bitset < 3 ? 3 - bitset : bitset );
				pItem.Item.Item.type = bitset; 
				
				if ( !bitset )
					SetCanvasNullItem(cx, cy);
				else
				{
					pItem.idTile = m_pvEditor->GetBgTile(bitset, uType); // ngdType.gr.gr[bitset];
					SetCanvasItem(&pItem, cx, cy);
				}
				bit-=2;
			}
		}
	}
	else
	{
		DWORD dwGroundSet = m_pvEditor->GetBgSet(uSet); //g_mvGroundSet[uSet];
		if ( 0x1f == uSet && 0 == item ) // level bg for 0x1f are skipped
			return TRUE;

		NES_GROUND_DIM ngdType = g_mvGroundType[uType+fx*8];

		for(INT cy = pos.y; cy <= m_Canvas.sz.cy; ++cy)
		{
			INT bit = 8*sizeof(dwGroundSet) - 2;
			for(INT cx=0;cx<m_Canvas.sz.cx;++cx)
			{
				NES_GITEM pItem = { 0 };
				pItem.Item.Item.id = item;
				BYTE bitset = ( dwGroundSet >> bit ) & 0x03;
				//bitset = ( bitset >= 1 && bitset < 3 ? 3 - bitset : bitset );
//				bitset = ( bitset < 1 ? 0 : 4 - bitset );// 3 : ( bitset == 1 ? 1 : bitset ) );// 2 ? bitset : 5 - bitset );
//				bitset = ( bitset >= 1 && bitset < 3 ? 3 - bitset : bitset );
				pItem.Item.Item.type = bitset; 
			
				if ( !bitset )
					SetCanvasNullItem(cx, cy);
				else
				{
					pItem.idTile = m_pvEditor->GetBgTile(bitset, uType); // ngdType.gr.gr[bitset];
					SetCanvasItem(&pItem, cx, cy);
				}
				bit-=2;
			}
		}
	}
	return TRUE;
}