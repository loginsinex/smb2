
#include "precomp.h"

CControl::CControl() : m_hWnd( NULL )
{

}

CControl::CControl(HWND hWnd)
{
	m_hWnd = hWnd;
}

CControl::operator HWND()
{
	return m_hWnd;
}

void CControl::AssignControl(HWND hWnd, USHORT uCtlId)
{
	m_hWnd = GetDlgItem(hWnd, uCtlId);

#ifdef UNICODE
	cSendMessage(CCM_SETUNICODEFORMAT, TRUE);
#endif
}

void CControl::AssignWindow(HWND hWnd)
{
	m_hWnd = hWnd;
#ifdef UNICODE
	cSendMessage(CCM_SETUNICODEFORMAT, TRUE);
#endif

#ifdef SETCTLTHEME
	SetWindowTheme(m_hWnd, TEXT("EXPLORER"), NULL);
#endif
}

INT_PTR	CControl::cSendMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return SendMessage(m_hWnd, uMsg, wParam, lParam);
}

HWND CControl::GetWindowHandle()
{
	return m_hWnd;
}

void CControl::Enable(BOOL bEnable)
{
	EnableWindow(m_hWnd, bEnable);
}

BOOL CControl::Enable()
{
	return IsWindowEnabled(m_hWnd);
}

void CControl::Visible(BOOL bVisible)
{
	ShowWindow(m_hWnd, (bVisible ? SW_SHOW : SW_HIDE));
}

BOOL CControl::Visible()
{
	return (GetWindowLong(m_hWnd, GWL_STYLE) & WS_VISIBLE); //IsWindowVisible(m_hWnd);
}
