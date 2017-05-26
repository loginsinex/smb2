
#include "precomp.h"

INT CTabControl::ItemsCount() { return (INT) cSendMessage(TCM_GETITEMCOUNT, 0, 0); }

void CTabControl::AddItem(LPCTSTR pszText, LPARAM lParam)
{
	TC_ITEM tci = { 0 };
	tci.mask = TCIF_TEXT | TCIF_PARAM;
	tci.pszText = LPTSTR(pszText);
	tci.lParam = lParam;
	cSendMessage(TCM_INSERTITEM, ItemsCount(), LPARAM(&tci));
}

void CTabControl::SetCurSel(INT i)
{
	cSendMessage(TCM_SETCURSEL, i, 0);
}

INT CTabControl::GetCurSel()
{
	return (INT) cSendMessage(TCM_GETCURSEL, 0, 0);
}

void CTabControl::Adjust(HWND hWnd)
{
	RECT rc = { 0 };
	cSendMessage(TCM_ADJUSTRECT, 0, LPARAM(&rc));
	
	HWND hParent = GetParent(GetWindowHandle());

	RECT rc_p, rc_t;
	GetClientRect(hParent, &rc_p);
	GetClientRect(GetWindowHandle(), &rc_t);

	ClientToScreen(hParent, LPPOINT(&rc_p));
	ClientToScreen(GetWindowHandle(), LPPOINT(&rc_t));

	rc_t.left -= rc_p.left;
	rc_t.top -= rc_p.top;
	
	SetWindowPos(hWnd, GetWindowHandle(), rc_t.left+rc.left, rc_t.top+rc.top, rc_t.right+rc.right-rc.left, rc_t.bottom+rc.bottom-rc.top, SWP_FRAMECHANGED);
}

void CTabControl::DeferAdjust(HDWP hDwp, HWND hWnd)
{
	RECT rc = { 0 };
	cSendMessage(TCM_ADJUSTRECT, 0, LPARAM(&rc));
	
	HWND hParent = GetParent(GetWindowHandle());

	RECT rc_p, rc_t;
	GetClientRect(hParent, &rc_p);
	GetClientRect(GetWindowHandle(), &rc_t);

	ClientToScreen(hParent, LPPOINT(&rc_p));
	ClientToScreen(GetWindowHandle(), LPPOINT(&rc_t));

	rc_t.left -= rc_p.left;
	rc_t.top -= rc_p.top;
	
	DeferWindowPos(hDwp, hWnd, GetWindowHandle(), rc_t.left+rc.left, rc_t.top+rc.top, rc_t.right+rc.right-rc.left, rc_t.bottom+rc.bottom-rc.top, SWP_FRAMECHANGED);
}

void CTabControl::Param(int i, LPARAM lParam)
{
	TC_ITEM tci = { 0 };
	tci.mask = TCIF_PARAM;
	tci.lParam = lParam;
	cSendMessage(TCM_SETITEM, i, (LPARAM) &tci);
}

LPARAM CTabControl::Param(int i)
{
	TC_ITEM tci = { 0 };
	tci.mask = TCIF_PARAM;
	cSendMessage(TCM_GETITEM, i, (LPARAM) &tci);
	return tci.lParam;
}

void CTabControl::SetText(int i, LPCTSTR pszText)
{
	TC_ITEM tci = { 0 };
	tci.mask = TCIF_TEXT;
	tci.pszText = LPTSTR(pszText);
	cSendMessage(TCM_SETITEM, i, (LPARAM) &tci);
}