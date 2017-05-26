
#include "precomp.h"

void InitLevelDraw()
{
	WNDCLASS wc = { 0 };
	wc.cbWndExtra = 8;
	wc.hbrBackground = (HBRUSH) ( COLOR_WINDOW + 1 );
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpfnWndProc = CLvlDraw::ControlProc;
	wc.lpszClassName = ZLVLDRAW_CLASS;
	wc.style = CS_DBLCLKS;

	RegisterClass(&wc);
}


LRESULT CALLBACK CLvlDraw::ControlProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CLvlDraw * This = (CLvlDraw*) GetWindowLong(hWnd, 4);

	switch(uMsg)
	{
	case WM_NCCREATE:
		{
			This = new CLvlDraw((LPCREATESTRUCT) lParam, hWnd);
			SetWindowLong(hWnd, 4, (LONG) This);
			return 1;
		}
	case WM_GETDLGCODE: return (DLGC_WANTARROWS | DLGC_WANTCHARS);
	case WM_SETFONT:
		{
			This->m_hGlobalFont = HFONT(wParam);
			if ( lParam )
				InvalidateRect(hWnd, NULL, TRUE);
			break;
		}
	case WM_GETFONT:
		{
			return LRESULT(This->m_hGlobalFont);
		}
	case WM_SIZE:
	case WM_SIZING:
		{
			This->OnResize();
			break;
		}
	case WM_VSCROLL:
	case WM_HSCROLL:
		{
			This->UpdateScroll(uMsg, HIWORD(wParam), LOWORD(wParam));
			break;
		}
	case WM_SETFOCUS: 
		{
			break;
		}
	case 0x1133: return GetWindowLong(hWnd, 4);
	case WM_MOUSEWHEEL:
		{
			SHORT delta = ((SHORT)HIWORD(wParam))/WHEEL_DELTA;
			for(int i=0;i<abs(delta);++i)
				This->UpdateScroll(WM_VSCROLL, 0, delta > 0 ? SB_LINEUP : SB_LINEDOWN );
			break;
		}
	case WM_LBUTTONDOWN:
		{
			SetFocus(hWnd);
			This->OnMouseDown(wParam, LOWORD(lParam), HIWORD(lParam));
			break;
		}
	case WM_LBUTTONUP:
		{
			This->OnMouseUp(wParam, LOWORD(lParam), HIWORD(lParam));
			break;
		}
	case WM_CONTEXTMENU:
		{
			POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) }; 
			if ( pt.x != -1 && pt.y != -1 )
			{
				ScreenToClient(hWnd, &pt);
				This->OnMouseDown(0, LOWORD(pt.x), LOWORD(pt.y));
				This->OnMouseUp(0, LOWORD(pt.x), LOWORD(pt.y));
			}

			This->OnContextMenu();
			break;
		}
	case WM_KEYDOWN:
		{
			This->OnKeyDown(wParam, ( lParam >> 16 ) & 0x00FFL );
			break;
		}
	case WM_ERASEBKGND:
		{
			This->OnErase((HDC) wParam);
			break;
		}
	case WM_MOUSEMOVE:
		{
			This->OnMouseMove(wParam, LOWORD(lParam), HIWORD(lParam));
			break;
		}
	case WM_CHAR:
		{
			break;
		}
	case WM_SETCURSOR:
		{
			SetCursor(This->m_cursor.hCurrent);
			break;
		}
	case WM_NCDESTROY:
		{
			delete This;
			break;
		}
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return 0;
}

CLvlDraw::CLvlDraw(LPCREATESTRUCT lpcs, HWND hWnd) : m_pvEditor( NULL )
{
	m_hWnd = hWnd;
	m_hParentWnd = lpcs->hwndParent;
	m_hDC = GetDC(m_hWnd);
	m_Bmp.bmBuffer.Create(lpcs->cx, lpcs->cy);
	m_Bmp.bmLevel.Create(1, 1);
	m_Bmp.bmSpecial.Create(1, 1);
	m_vDim = NULL;
	m_Canvas.pItems = m_Canvas.pEnemyItems = m_Canvas.pSpecial = NULL;
	m_Canvas.sz.cx = m_Canvas.sz.cy = 0;
	m_Canvas.fx = 0;
	m_Canvas.szSpecial.cx = m_Canvas.szSpecial.cy = 0;
	ZeroMemory(&m_Catch, sizeof(m_Catch));

	m_Canvas.fDisableEditor = ( lpcs->style & 1 );
	m_cursor.hCurrent = m_cursor.hArrow = LoadCursor(NULL, IDC_ARROW);
	m_cursor.hMove = LoadCursor(NULL, IDC_SIZEALL);
	// get font chars from parent
	m_Font.LoadFromWindow(lpcs->hwndParent, -1, -1, -1, -1, -1, TEXT("Arial"));
	// SendMessage(m_hWnd, WM_SETFONT, (WPARAM) m_Font.Font(), (LPARAM) TRUE);
	m_hGlobalFont = m_Font.Font();
	//SendMessage(m_hWnd, WM_GETFONT, 
	//	SendMessage(m_hParentWnd, WM_GETFONT, 0, 0), TRUE);
}

CLvlDraw::~CLvlDraw()
{
	ReleaseDC(m_hWnd, m_hDC);
}

BOOL CLvlDraw::SetCatchCanvas(BOOL fEnemiesCanvas, BOOL fJarEnemies)
{
	BOOL fRet = m_Catch.fCatchEnemies;
	m_Catch.fCatchEnemies = ( fEnemiesCanvas && !fJarEnemies );
	m_Catch.fCatchJarEnemies = ( !fEnemiesCanvas && fJarEnemies );
	m_Catch.dwCatchedObject = 0;
	m_Catch.fObjectCatched = FALSE;
	RedrawLevel();
	return fRet;
}

void CLvlDraw::SetCursorMode(BOOL fMove)
{
	if ( fMove )
		m_cursor.hCurrent = m_cursor.hMove;
	else
		m_cursor.hCurrent = m_cursor.hArrow;

	SetCursor(m_cursor.hCurrent);
}

VOID CLvlDraw::SelectPos(int page, BYTE pos)
{
	m_Canvas.vHilite.clear();
	m_Canvas.vHilite.insert(m_Canvas.vHilite.end(), m_Canvas.sz.cx * m_Canvas.sz.cy, FALSE);

	if ( (*m_pvEditor)[nlfDirection] )
	{
		UpdateScroll(WM_HSCROLL, 0x10 * page, SB_THUMBPOSITION);
		
		if ( pos < 16)
		{
			int x = page * 0x10 + int(pos), cx = m_Canvas.sz.cx;
			if ( x + cx * 14 < int(m_Canvas.vHilite.size()) )
				for(int y = 0; y < 15; ++y)
					m_Canvas.vHilite[x + cx * y] = TRUE;
			else
				_tprintf(TEXT("WARNING: page/pos is out of range: %d/%d\n"), page, pos);
		}
		else
		{
			int sx = page * 0x10, cx = m_Canvas.sz.cx;
			if ( 0x0f + cx * 14 < int(m_Canvas.vHilite.size()) )
				for(int y = 0; y < 15; ++y)
					for(int x = sx; x < sx + 0x10; ++x)
						m_Canvas.vHilite[x + cx * y] = TRUE;
			else
				_tprintf(TEXT("WARNING: page/pos is out of range: %d/%d\n"), page, pos);
		}
	}
	else
	{
		UpdateScroll(WM_VSCROLL, 0x0F * page, SB_THUMBPOSITION);

		if ( pos < 16 )
		{
			int x = int(pos), cx = m_Canvas.sz.cx, sy = page * 0x0f;
			if ( x + cx * sy < int(m_Canvas.vHilite.size()) )
				for(int y = sy; y < 15 + sy; ++y)
					m_Canvas.vHilite[x + cx * y] = TRUE;
			else
				_tprintf(TEXT("WARNING: page/pos is out of range: %d/%d\n"), page, pos);
		}
		else
		{
			int cx = m_Canvas.sz.cx, sy = page * 0x0f;
			if ( 0x0f + cx * sy < int(m_Canvas.vHilite.size()) )
				for(int y = sy; y < 15 + sy; ++y)
					for(int x = 0; x < 16; ++x)
						m_Canvas.vHilite[x + cx * y] = TRUE;
			else
				_tprintf(TEXT("WARNING: page/pos is out of range: %d/%d\n"), page, pos);
		}

	}

	RedrawLevel(FALSE);
}

VOID CLvlDraw::OnResize()
{
	RECT rc = { 0 };
	GetClientRect(m_hWnd, &rc);
	m_scroll.sz.cx = rc.right;
	m_scroll.sz.cy = rc.bottom;
	SetScroll();
	InvalidateRect(m_hWnd, NULL, TRUE);

	if ( m_Bmp.bmBuffer.Width() < rc.right || m_Bmp.bmBuffer.Height() < rc.bottom )
	{
		m_Bmp.bmBuffer.Resize(max(rc.right, m_Bmp.bmBuffer.Width()) , max(rc.bottom, m_Bmp.bmBuffer.Height()));
		RedrawBuffer();
	}

}

VOID CLvlDraw::OnMouseDown(UINT uCtl, SHORT x, SHORT y)
{
	if ( m_Catch.fObjectCatched || m_Canvas.fDisableEditor )
		return;

	if ( 0 == m_Canvas.sz.cx || 0 == m_Canvas.sz.cy )
		return;
	
	x /= (SHORT) m_dcsz.cx; y /= (SHORT) m_dcsz.cy;

	x += m_scroll.scrollX; y += m_scroll.scrollY;


	NES_GITEM pItem = { 0 };
	if ( (( !( m_Catch.fCatchEnemies || m_Catch.fCatchJarEnemies ) && ( GetCanvasItem(x,y,&pItem) || GetSpecialCanvasItem(x,y,&pItem) ) ) 
		|| ( ( m_Catch.fCatchEnemies || m_Catch.fCatchJarEnemies ) && GetCanvasEnemyItem(&pItem, x, y)) ) &&
		pItem.Item.Item.id < m_pvEditor->SizeOfItemsList() )
	{
		CNesItem nItem = m_pvEditor->LevelItem(pItem.Item.Item.id);

		if ( 0 != pItem.Item.Item.id && !( !pItem.fHidden && m_Catch.fCatchJarEnemies ) && !( pItem.fHidden && m_Catch.fCatchEnemies) )
		{
			m_Catch.fObjectCatched = TRUE;
			m_Catch.dwCatchedObject = pItem.Item.Item.id;
			POINT itemPt = nItem.Pos();

			if ( pItem.fHidden )
			{
				if ( itemPt.x >= 10 * 0x10 )
					itemPt.x -= 10 * 0x10;

				if ( itemPt.y >= 10 * 0x10 )
					itemPt.y -= 10 * 0x10;
			}

			m_Catch.ptDelta.x = x - (USHORT) itemPt.x;
			m_Catch.ptDelta.y = y - (USHORT) itemPt.y;

			if ( nItem.Type() != nitEntrance && nItem.Type() != nitPointer )
				m_Canvas.vHilite.clear();

			RedrawLevel(FALSE);
			DoNotify(LDN_ITEMSELECTED, pItem.Item.Item.id, x, y);
		}
		else
		{
			m_Catch.fObjectCatched = FALSE;
			m_Catch.dwCatchedObject = 0;
			SetCursorMode(FALSE);
			DoNotify(LDN_ITEMSELECTED, 0, 0, 0);
			RedrawLevel(FALSE);
		}
	}
	else
	{
		m_Canvas.vHilite.clear();
		m_Catch.dwCatchedObject = 0;
		m_Catch.fObjectCatched = FALSE;
		RedrawLevel(FALSE);
		SetCursorMode(FALSE);
		DoNotify(LDN_ITEMSELECTED, 0, 0, 0);
	}
}

VOID CLvlDraw::OnMouseUp(UINT uCtl, SHORT x, SHORT y)
{
	if ( !m_Catch.fObjectCatched || m_Canvas.fDisableEditor )
		return;

	SetCursorMode(FALSE);

	m_Catch.fObjectCatched = FALSE;

	if ( 0 == m_Canvas.sz.cx || 0 == m_Canvas.sz.cy )
		return;
	
	x /= (SHORT) m_dcsz.cx; y /= (SHORT) m_dcsz.cy;

	x += m_scroll.scrollX; y += m_scroll.scrollY;

	if ( x >= m_Canvas.sz.cx || y >= m_Canvas.sz.cy )
		return;

	INT cx = x - SHORT(LOWORD(m_Catch.ptDelta.x)), cy = y - SHORT(LOWORD(m_Catch.ptDelta.y));
	
	NES_GITEM pItem = { 0 };

	if ( ( m_Catch.fCatchJarEnemies || m_Catch.fCatchEnemies ) ? m_pvEditor->MoveEnemy(m_Catch.dwCatchedObject, cx, cy, m_Catch.fCatchJarEnemies) : m_pvEditor->MoveObject(m_Catch.dwCatchedObject, cx, cy) )
		RedrawLevel( m_Catch.fCatchEnemies || m_Catch.fCatchJarEnemies );
}

VOID CLvlDraw::OnMouseMove(UINT uCtl, SHORT x, SHORT y)
{
	if ( 0 == m_Canvas.sz.cx || 0 == m_Canvas.sz.cy || m_Canvas.fDisableEditor )
		return;
	
	x /= (SHORT) m_dcsz.cx; y /= (SHORT) m_dcsz.cy;

	x += m_scroll.scrollX; y += m_scroll.scrollY;


	if ( m_Catch.fObjectCatched )
	{
		INT cx = x - SHORT(LOWORD(m_Catch.ptDelta.x)), cy = y - SHORT(LOWORD(m_Catch.ptDelta.y));

		SetCursorMode(TRUE);
	
		if ( ( m_Catch.fCatchEnemies || m_Catch.fCatchJarEnemies ) ? m_pvEditor->MoveEnemy(m_Catch.dwCatchedObject, cx, cy, m_Catch.fCatchJarEnemies) : m_pvEditor->MoveObject(m_Catch.dwCatchedObject, cx, cy) )
		{
			DoNotify(LDN_ITEMSELECTED, m_Catch.dwCatchedObject, cx, cy);
			RedrawLevel( m_Catch.fCatchEnemies || m_Catch.fCatchJarEnemies );
		}
	}

	NMLVLDRWHT nm = { 0 };
	nm.nm.idFrom = GetWindowLong(m_hWnd, GWL_ID);
	nm.nm.hwndFrom = m_hWnd;
	nm.nm.code = LDN_HITEST;
	nm.pos.x = x; nm.pos.y = y;
	NES_GITEM nItem = { 0 };

	if ( ( GetCanvasItem(x, y, &nItem) || GetSpecialCanvasItem(x, y, &nItem) || nItem.Item.Item.id )
		&& nItem.Item.Item.id < m_pvEditor->SizeOfItemsList() )
		nm.iItem = ( nItem.Item.Item.id > 0 ? nItem.Item.Item.id : 0xFFFF );
	else
		nm.iItem = 0xFFFF;

	SendMessage(GetParent(m_hWnd), WM_NOTIFY, 0, (LPARAM) &nm);

	nm.nm.code = LDN_HITESTENEMY;
	if ( GetCanvasEnemyItem(&nItem, x, y) ) //&& 0 != nItem.Item.Item.id )
	{
		nm.iItem = ( nItem.Item.Item.id > 0 ? nItem.Item.Item.id : 0xFFFF );
	}
	else
		nm.iItem = 0xFFFF;
	
	SendMessage(GetParent(m_hWnd), WM_NOTIFY, 0, (LPARAM) &nm);
}



VOID CLvlDraw::OnKeyDown(UINT vKey, USHORT uScanCode)
{
	switch(uScanCode)
	{
	case 0x4b:		   // right
		{
			UpdateScroll(WM_HSCROLL, 0, SB_LINELEFT);
			break;
		}
	case 0x4d:		   // left
		{
			UpdateScroll(WM_HSCROLL, 0, SB_LINERIGHT);
			break;
		}
	case 0x50:		   // down
		{
			UpdateScroll(WM_VSCROLL, 0, SB_LINERIGHT);
			break;
		}
	case 0x48:		   // up
		{
			UpdateScroll(WM_VSCROLL, 0, SB_LINELEFT);
			break;
		}
	case 0x51: // pg dn
		{
			UpdateScroll(WM_VSCROLL, 0, SB_PAGEDOWN);
			UpdateScroll(WM_HSCROLL, 0, SB_PAGERIGHT);
			break;
		}
	case 0x49: // pg up
		{
			UpdateScroll(WM_VSCROLL, 0, SB_PAGEUP);
			UpdateScroll(WM_HSCROLL, 0, SB_PAGELEFT);
			break;
		}
	case 0x47: // home
		{
			UpdateScroll(WM_VSCROLL, 0, SB_TOP);
			UpdateScroll(WM_HSCROLL, 0, SB_LEFT);
			break;
		}
	case 0x4F: // end
		{
			UpdateScroll(WM_VSCROLL, 0, SB_BOTTOM);
			UpdateScroll(WM_HSCROLL, 0, SB_RIGHT);
			break;
		}
	case 74: // gray '-'
	case 12: // '-'
		{
			OnChangeBg(-1);
			break;
		}
	case 78: // gray '+'
	case 13: // '+'
		{
			OnChangeBg(+1);
			break;
		}
	case 83:
		{
			OnDeleteItem();
			break;
		}
	case VK_TAB:
		{
			OnNextItem(GetKeyState(VK_SHIFT) & 0x8000);
			break;
		}
	}
}

void CLvlDraw::OnNextItem(BOOL fPrevItem)
{
	if ( m_Catch.dwCatchedObject < 1 )
		return;

	if ( DoNotify(LDN_NEXTITEM, m_Catch.dwCatchedObject, fPrevItem, fPrevItem) )
		RedrawLevel(TRUE);
}

VOID CLvlDraw::OnDeleteItem()
{
	if ( m_Catch.dwCatchedObject < 1 )
		return;

	if ( DoNotify(LDN_DELETE, m_Catch.dwCatchedObject, 0, 0) )
	{
		m_Catch.dwCatchedObject = 0;
		m_Catch.fObjectCatched = FALSE;
		RedrawLevel(TRUE);
	}
}

VOID CLvlDraw::OnChangeBg(int code)
{
	if ( m_Catch.dwCatchedObject < 1 )
		return;

	if ( DoNotify(LDN_CHANGEBG, m_Catch.dwCatchedObject, code, !!(GetKeyState(VK_SHIFT) & 0x8000)) )
	{
		RedrawLevel(TRUE);
		DoNotify(LDN_ITEMSELECTED, m_Catch.dwCatchedObject, 0, 0);
	}
}

VOID CLvlDraw::OnErase(HDC hDC)
{
	RECT rc = { 0 };
	GetClientRect(m_hWnd, &rc);

	m_Bmp.bmBuffer.RenderToDevice(m_hDC, 0, 0, rc.right, rc.bottom, m_scroll.scrollX*m_dcsz.cx, m_Bmp.bmBuffer.Height() - rc.bottom - m_scroll.scrollY*m_dcsz.cx);
}


VOID CLvlDraw::OnContextMenu()
{
	if ( !m_pvEditor )
		return;

	BOOL fEditEnable = ( m_Catch.dwCatchedObject != 0 ),
		fDeleteEnable = ( m_Catch.dwCatchedObject != 0 ),
		fLayerChangable = FALSE, fEntrance = FALSE;

	if ( fEditEnable )
	{
		CNesItem nItem = m_pvEditor->LevelItem(m_Catch.dwCatchedObject);
		switch(nItem.Type())
		{
		case nitRegular: fEditEnable = ( nItem.Regular() >= 0x30 ); break;
		case nitEntrance:
		case nitPointer:
			fEntrance = TRUE;
		case nitGround:
			fEditEnable = TRUE;
			break;
		default:
			fEditEnable = FALSE;
			break;
		}
		fLayerChangable = ( nitEnemy != nItem.Type() );
	}

	HMENU hPopup = CreatePopupMenu();
	AppendMenu(hPopup, 0, 0x10, TEXT("Insert object..."));
	if ( 4 == ( m_pvEditor->GetCurLevel() % 10 ) )
	{
		AppendMenu(hPopup, 0, 0x18, TEXT("Insert jar enemy..."));
		AppendMenu(hPopup, 0, 0x19, TEXT("Make level as jar"));
	}

	AppendMenu(hPopup, ( fEditEnable ? 0 : MFS_DISABLED | MFS_GRAYED ), 0x11, TEXT("Edit object...\t\t[Shift]+/-"));
	AppendMenu(hPopup, ( fDeleteEnable ? 0 : MFS_DISABLED | MFS_GRAYED ), 0x12, TEXT("Delete object\t\tDel"));
	AppendMenu(hPopup, ( fLayerChangable ? 0 : MFS_DISABLED | MFS_GRAYED ), 0x13, TEXT("Change layer..."));
	if ( fEntrance )
		AppendMenu(hPopup, 0, 0x14, TEXT("Jump to pointer"));

	AppendMenu(hPopup, MFT_SEPARATOR, 0xff, NULL);
	AppendMenu(hPopup, 0, 0x17, TEXT("Edit level information..."));

	POINT pt; GetCursorPos(&pt);
	SetFocus(m_hWnd);

	INT id = TrackPopupMenu(hPopup, TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RIGHTBUTTON | TPM_LEFTBUTTON | TPM_RETURNCMD, pt.x, pt.y, 0, m_hWnd, NULL);
	
	ScreenToClient(m_hWnd, &pt);
	pt.x /= m_dcsz.cx; pt.y /= m_dcsz.cy;
	pt.x += m_scroll.scrollX; pt.y += m_scroll.scrollY;
	pt.x = max(0,pt.x); pt.x = min(pt.x,m_Canvas.sz.cx);
	pt.y = max(0,pt.y); pt.y = min(pt.y,m_Canvas.sz.cy);

	BOOL fNeedRedraw = FALSE;
	switch(id)
	{
	case 0x10: fNeedRedraw = DoNotify(LDN_INSERT, 0, pt.x, pt.y); break;
	case 0x11: fNeedRedraw = DoNotify(LDN_EDIT, m_Catch.dwCatchedObject, pt.x, pt.y); break;
	case 0x12: fNeedRedraw = DoNotify(LDN_DELETE, m_Catch.dwCatchedObject, pt.x, pt.y); break;
	case 0x13: fNeedRedraw = DoNotify(LDN_CHANGELAYER, m_Catch.dwCatchedObject, pt.x, pt.y); break;
	case 0x14: fNeedRedraw = DoNotify(LDN_JUMPTOPOINTER, m_Catch.dwCatchedObject, 0, 0); break;
	case 0x17: fNeedRedraw = DoNotify(LDN_LEVELDATA, 0, 0, 0); break;
	case 0x18: fNeedRedraw = DoNotify(LDN_INSERTJARENEMY, 0, pt.x, pt.y); break;
	case 0x19: fNeedRedraw = DoNotify(LDN_MAKEJAR, 0, 0, 0); break;
	}

	if ( fNeedRedraw )
	{
		DoNotify(LDN_ITEMSELECTED, m_Catch.dwCatchedObject, 0, 0);

		RedrawLevel(TRUE);
	}
	else
		InvalidateRect(m_hWnd, NULL, TRUE);

	DestroyMenu(hPopup);
}

INT_PTR CLvlDraw::DoNotify(UINT uCode, DWORD i, UINT x, UINT y)
{
	NMLVLDRWHT nmlvl = { 0 };
	nmlvl.nm.code = uCode;
	nmlvl.nm.idFrom = GetWindowLong(m_hWnd, GWL_ID);
	nmlvl.nm.hwndFrom = m_hWnd;
	nmlvl.pos.x = x;
	nmlvl.pos.y = y;
	nmlvl.iItem = (USHORT) i;
	return SendMessage(m_hParentWnd, WM_NOTIFY, 0, (LPARAM) &nmlvl);
}

UINT CLvlDraw::GetSelectedItem()
{
	return m_Catch.dwCatchedObject;
}

VOID CLvlDraw::RequestRedraw(BOOL fWithEnemies)
{
	RedrawLevel(fWithEnemies);
}