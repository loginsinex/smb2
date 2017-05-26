
#include "precomp.h"

VOID CLvlDraw::SetScroll()
{
	if ( m_scroll.sz.cx >= m_Bmp.bmLevel.Width() )
	{
		SetWindowLong(m_hWnd, GWL_STYLE, ( GetWindowLong(m_hWnd, GWL_STYLE) & ~WS_HSCROLL));
		m_scroll.scrollX = 0;
	}
	else
	{
		SetWindowLong(m_hWnd, GWL_STYLE, ( GetWindowLong(m_hWnd, GWL_STYLE) | WS_HSCROLL));
		SCROLLINFO si = { 0 };
		si.cbSize = sizeof(si);
		si.fMask = SIF_PAGE | SIF_RANGE;
		__try
		{
			si.nPage = m_scroll.sz.cx / m_dcsz.cx+2;
		}
		__except(EXCEPTION_EXECUTE_HANDLER)
		{
			TCHAR pszStr[MAX_PATH];
			wsprintf(pszStr, TEXT("CX: %d; DCSZ.CX: %d"), m_scroll.sz.cx, m_dcsz.cx);
			// MessageBox(m_hWnd, pszStr, 0, 0);
			_tprintf(TEXT("%s"), pszStr);
		}

		si.nMin = 0; si.nMax = m_Canvas.sz.cx + 1;// si.nMax = m_Bmp.bmLevel.Width() / m_dcsz.cx - m_scroll.sz.cx / m_dcsz.cx + 1;
		SetScrollInfo(m_hWnd, SB_HORZ, &si, TRUE);
	}

	if ( m_scroll.sz.cy >= m_Bmp.bmLevel.Height() )
	{
		m_scroll.scrollY = 0;
		SetWindowLong(m_hWnd, GWL_STYLE, ( GetWindowLong(m_hWnd, GWL_STYLE) & ~WS_VSCROLL));
	}
	else
	{
		SetWindowLong(m_hWnd, GWL_STYLE, ( GetWindowLong(m_hWnd, GWL_STYLE) | WS_VSCROLL));
		SCROLLINFO si = { 0 };
		si.cbSize = sizeof(si);
		si.fMask = SIF_PAGE | SIF_RANGE;
		si.nPage = m_scroll.sz.cy / m_dcsz.cy+2;
		si.nMin = 0; si.nMax = m_Canvas.sz.cy; //m_Bmp.bmLevel.Height() / m_dcsz.cy - m_scroll.sz.cy / m_dcsz.cy + 1;
		SetScrollInfo(m_hWnd, SB_VERT, &si, TRUE);
	}
	SetWindowPos(m_hWnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED | SWP_DRAWFRAME);
}

VOID CLvlDraw::UpdateScroll(UINT uType, USHORT uPos, USHORT uReq)
{
	int maxPosX = 0, maxPosY = 0, pageX = 0, pageY = 0;
	SCROLLINFO si = { 0 };

	if ( WS_HSCROLL == (WS_HSCROLL & GetWindowLong(m_hWnd, GWL_STYLE)) )
	{
		si.cbSize = sizeof(si); si.fMask = SIF_RANGE | SIF_PAGE;
		GetScrollInfo(m_hWnd, SB_HORZ, &si); maxPosX = si.nMax; pageX = si.nPage;
	}
	else if ( WM_HSCROLL == uType )
		return;


	if ( WS_VSCROLL == (WS_VSCROLL & GetWindowLong(m_hWnd, GWL_STYLE)) )
	{
		si.cbSize = sizeof(si); si.fMask = SIF_RANGE | SIF_PAGE;
		GetScrollInfo(m_hWnd, SB_VERT, &si); maxPosY = si.nMax; pageY = si.nPage;
	}
	else if ( WM_VSCROLL == uType )
		return;

	int pos = GetScrollPos(m_hWnd, ( WM_HSCROLL == uType ? SB_HORZ : SB_VERT )), //( WM_HSCROLL == uType ? m_scroll.scrollX : m_scroll.scrollY ),
		maxPos = ( WM_HSCROLL == uType ? maxPosX - pageX + 1: maxPosY - pageY + 1);

	switch(uReq)
	{
	 //case SB_ENDSCROLL: 
	case SB_RIGHT: pos = maxPos ; break;
	case SB_LEFT: pos = 0; break;    // or top
	case SB_LINELEFT: pos--; break;
	case SB_LINERIGHT: pos++; break;
	case SB_PAGELEFT: pos-=16; break;
	case SB_PAGERIGHT: pos+=16; break;
	case SB_THUMBPOSITION: 
	case SB_THUMBTRACK: pos = uPos; break;
	default: return;
	}
	
	if ( pos < 0 ) pos = 0;
	else if ( pos > maxPos ) pos = maxPos;

	SetScrollPos(m_hWnd, ( WM_HSCROLL == uType ? SB_HORZ : SB_VERT ), pos, TRUE);

	if ( WM_HSCROLL == uType )
		m_scroll.scrollX = pos;
	else
		m_scroll.scrollY = pos;

	InvalidateRect(m_hWnd, NULL, TRUE);
}
