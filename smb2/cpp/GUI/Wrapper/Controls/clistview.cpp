
#include "precomp.h"

// --- listview

INT CListView::Count()
{
	return (INT) cSendMessage(LVM_GETITEMCOUNT, 0, 0);
}

INT CListView::AddItem(DWORD dwValue, LPARAM lParam)
{
	TCHAR pszText[32];
	wsprintf(pszText, TEXT("%d"), dwValue);
	return AddItem(pszText, lParam);
}

INT CListView::AddItem(LPCTSTR pszText, LPARAM lParam)
{
	INT iItems = (INT) cSendMessage(LVM_GETITEMCOUNT, 0, 0);
	return InsertItem(iItems, pszText, lParam);
}

INT CListView::InsertItem(INT iAfterItem, LPCTSTR pszText, LPARAM lParam)
{
	LV_ITEM lvi = { 0 };
	lvi.mask = LVIF_TEXT | LVIF_PARAM;
	lvi.pszText = LPTSTR(pszText);
	lvi.lParam = lParam;
	lvi.iItem = iAfterItem;
	return (INT) cSendMessage(LVM_INSERTITEM, 0, LPARAM(&lvi));
}

void CListView::CurSel(INT iItem)
{
	LV_ITEM lvi = { 0 };
	lvi.mask = LVIF_STATE;
	lvi.state = LVIS_FOCUSED | LVIS_SELECTED;
	lvi.stateMask = -1;
	lvi.iItem = iItem;
	cSendMessage(LVM_SETITEM, 0, LPARAM(&lvi));
}

INT CListView::CurSel()
{
	return (INT) cSendMessage(LVM_GETNEXTITEM, -1, MAKELPARAM(LVIS_FOCUSED | LVIS_SELECTED, 0));
}

void CListView::DeleteItem(INT iItem)
{
	cSendMessage(LVM_DELETEITEM, iItem, 0);
}

void CListView::ResetContent()
{
	cSendMessage(LVM_DELETEALLITEMS, 0, 0);
}

void CListView::SetItem(INT iItem, INT iSubItem, LPCTSTR pszText)
{
	LV_ITEM lvi = { 0 };
	lvi.mask = LVIF_TEXT;
	lvi.iItem = iItem;
	lvi.iSubItem = iSubItem;
	lvi.pszText = LPTSTR(pszText);
	cSendMessage(LVM_SETITEM, 0, LPARAM(&lvi));
}

void CListView::SetItem(INT iItem, INT iSubItem, DWORD dwValue)
{
	TCHAR pszText[32];
	wsprintf(pszText, TEXT("%d"), dwValue);
	SetItem(iItem, iSubItem, pszText);
}

void CListView::Param(INT iItem, LPARAM lParam)
{
	LV_ITEM lvi = { 0 };
	lvi.mask = LVIF_PARAM;
	lvi.iItem = iItem;
	lvi.lParam = lParam;
	cSendMessage(LVM_SETITEM, 0, LPARAM(&lvi));
}

LPARAM CListView::Param(INT iItem)
{
	LV_ITEM lvi = { 0 };
	lvi.mask = LVIF_PARAM;
	lvi.iItem = iItem;
	cSendMessage(LVM_GETITEM, 0, LPARAM(&lvi));
	return lvi.lParam;
}

void CListView::AddColumn(LPCTSTR pszText, INT cx)
{
	//HDM_GETITEMCOUNT
	if ( !m_hHeader ) 
		m_hHeader = (HWND) cSendMessage(LVM_GETHEADER); // FindWindowEx(GetWindowHandle(), 0, TEXT("SysHeader32"), NULL);

	HWND hHeader = m_hHeader;
	if ( !hHeader ) return;

	INT c = (INT) SendMessage(hHeader, HDM_GETITEMCOUNT, 0, 0);
	if ( cx == -1 )
	{
		RECT rc = { 0 };
		GetClientRect(GetWindowHandle(), &rc);
		
		INT icx = 0;
		for(INT i = 0; i < c; ++i)
		{
			HD_ITEM hdi = { 0 };
			hdi.mask = HDI_WIDTH;
			SendMessage(hHeader, HDM_GETITEM, i, LPARAM(&hdi));
			icx += hdi.cxy;
		}

		if ( icx >= rc.right ) cx = 100;
		else cx = rc.right - icx - GetSystemMetrics(SM_CXVSCROLL);
	}
	else if ( !cx )
	{
		HDC hDC = GetDC(hHeader);
		RECT rc = { 0 };
		DrawText(hDC, pszText, lstrlen(pszText), &rc, DT_CALCRECT);
		ReleaseDC(hHeader, hDC);
		cx = rc.right + 2*GetSystemMetrics(SM_CXFIXEDFRAME);
	}

	LV_COLUMN lvc = { 0 };
	lvc.mask = LVCF_TEXT | LVCF_WIDTH;
	lvc.cx = cx;
	lvc.pszText = LPTSTR(pszText);
	cSendMessage(LVM_INSERTCOLUMN, c, LPARAM(&lvc));
}

void CListView::ExtendedStyle(DWORD dwStyle)
{
	cSendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, -1, dwStyle);
}

DWORD CListView::ExtendedStyle()
{
	return (DWORD) cSendMessage(LVM_GETEXTENDEDLISTVIEWSTYLE, 0, 0);
}

INT CListView::State(INT iItem)
{
	return (INT) cSendMessage(LVM_GETITEMSTATE, iItem, -1);
}

void CListView::State(INT iItem, INT iState)
{
	LV_ITEM lvi = { 0 };
	lvi.mask = LVIF_STATE;
	lvi.iItem = iItem;
	lvi.state = iState;
	lvi.stateMask = -1;
	cSendMessage(LVM_SETITEM, 0, LPARAM(&lvi));
}

BOOL CListView::Selected(INT iItem)
{
	return ( LVIS_SELECTED == ( State(iItem) & LVIS_SELECTED ) );
}

void CListView::Selected(INT iItem, BOOL fSelected)
{
	State(iItem, 
		fSelected
		?
		( State(iItem) | LVIS_SELECTED )
		:
		( State(iItem) & ~LVIS_SELECTED )
	);
}

BOOL CListView::Check(INT iItem)
{
	LV_ITEM lvi = { 0 };
	lvi.iItem = iItem;
	lvi.stateMask = (3 << 12);
	lvi.mask = LVIF_STATE;
	cSendMessage(LVM_GETITEM, 0, (LPARAM) &lvi);
	return ( 2 == ((lvi.state >> 12) & 3));
}

void CListView::Check(INT iItem, BOOL fCheck)
{
	LV_ITEM lvi = { 0 };
	lvi.iItem = iItem;
	lvi.stateMask = (3 << 12);
	lvi.state = ( (fCheck ? 2 : 1) << 12 );
	lvi.mask = LVIF_STATE;
	cSendMessage(LVM_SETITEM, 0, (LPARAM) &lvi);
}

BOOL CListView::ItemRect(INT iItem, INT code, LPRECT lprc)
{
	lprc->left = code;
	BOOL ret = (BOOL) cSendMessage(LVM_GETITEMRECT, iItem, LPARAM(lprc));
	lprc->left = 0;
	return ret;
}

INT CListView::ColumnWidth(INT iColumn)
{
	return (INT) cSendMessage(LVM_GETCOLUMNWIDTH, iColumn, 0);
}

void CListView::ScrollToVisible()
{
	INT c = CurSel();
	if ( -1 == c ) return;
	cSendMessage(LVM_ENSUREVISIBLE, c, FALSE);
}

BOOL CListView::GetItemText(INT iItem, INT iSubItem, CString & text)
{
	BOOL result = FALSE;

	LV_ITEM lvi;
	lvi.mask = LVIF_TEXT;
	lvi.iItem = iItem;
	lvi.iSubItem = iSubItem;
	lvi.cchTextMax = 8192;
	lvi.pszText = LPTSTR(text.GetBuffer(8192));
	if ( cSendMessage(LVM_GETITEM, 0, LPARAM(&lvi)) )
		result = TRUE;

	text.ReleaseBuffer();
	return result;
}

CString CListView::GetItemText(INT iItem, INT iSubItem)
{
	CString sText;
	if ( GetItemText(iItem, iSubItem, sText) )
		return sText;

	return TEXT("");
}

