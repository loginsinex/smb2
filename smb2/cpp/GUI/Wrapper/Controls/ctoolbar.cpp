
#include "precomp.h"

CToolbar::CToolbar()
	:
	m_himl( NULL )
{

}

CToolbar::~CToolbar()
{
	if ( m_himl )
		ImageList_Destroy( m_himl );
}

HBITMAP CToolbar::SetImageList(HBITMAP hBitmap, COLORREF clrMask)
{
	if ( m_himl )
	{
		ImageList_Destroy( m_himl );
		m_himl = NULL;
	}

	BITMAP bmp = { 0 };
	if ( GetObject( hBitmap, sizeof(bmp), &bmp ) )
	{
		UINT uFlags = ILC_COLOR;
		switch(bmp.bmBitsPixel)
		{
		case 16: uFlags = ILC_COLOR16; break;
		case 24: uFlags = ILC_COLOR24; break;
		case 32: uFlags = ILC_COLOR32; break;
		}

		m_himl = ImageList_Create( bmp.bmHeight, bmp.bmHeight, uFlags | ILC_MASK, bmp.bmWidth / bmp.bmHeight, 0 );

		if ( m_himl )
		{
			ImageList_AddMasked( m_himl, hBitmap, clrMask );
			cSendMessage( TB_SETIMAGELIST, 0, (LPARAM) m_himl );
			m_szButton.cx = m_szButton.cy = bmp.bmHeight;
		}
	}

	return hBitmap;
}

BOOL CToolbar::AdjustButtonsSize()
{
	return SetButtonsSize(LOWORD(m_szButton.cx), LOWORD(m_szButton.cy));
}

BOOL CToolbar::SetButtonsSize(USHORT cx, USHORT cy)
{
	return cSendMessage(TB_SETBUTTONSIZE, cx, cy);
}

BOOL CToolbar::AddCustomButton(int iBitmap, int idCommand, BYTE fState, BYTE fStyle, DWORD dwData, INT_PTR iString)
{
	return AddCustomButton(-1, iBitmap, idCommand, fState, fStyle, dwData, (INT_PTR) iString);
}

BOOL CToolbar::AddCustomButton(int iBitmap, int idCommand, BYTE fState, BYTE fStyle, DWORD dwData, LPCTSTR iString)
{
	return AddCustomButton(iBitmap, idCommand, fState, fStyle, dwData, (INT_PTR) iString);
}

BOOL CToolbar::AddCustomButton(int iIndex, int iBitmap, int idCommand, BYTE fState, BYTE fStyle, DWORD dwData, INT_PTR iString)
{
	if ( -1 == iIndex )
		iIndex = cSendMessage( TB_BUTTONCOUNT );

	TBBUTTON tb = { 0 };
	tb.iBitmap = iBitmap;
	tb.idCommand = idCommand;
	tb.fsState = fState;
	tb.fsStyle = fStyle;
	tb.dwData = dwData;
	tb.iString = iString;
	return cSendMessage( TB_INSERTBUTTON, iIndex, (LPARAM) &tb );
}

BOOL CToolbar::AddCheck(int iBitmap, int idCommand, LPCTSTR pszText, BOOL fChecked, BOOL fEnabled, LPARAM lParam)
{
	BYTE fStyle = TBSTYLE_CHECK;
	BYTE fState = ( fChecked ? TBSTATE_CHECKED : 0 );
	fState |= ( fEnabled ? TBSTATE_ENABLED : 0 );

	return AddCustomButton(iBitmap, idCommand, fState, fStyle, lParam, pszText);
}

BOOL CToolbar::AddRadio(int iBitmap, int idCommand, LPCTSTR pszText, BOOL fChecked, BOOL fEnabled, LPARAM lParam)
{
	BYTE fStyle = TBSTYLE_CHECKGROUP;
	BYTE fState = ( fChecked ? TBSTATE_CHECKED : 0 );
	fState |= ( fEnabled ? TBSTATE_ENABLED : 0 );

	return AddCustomButton(iBitmap, idCommand, fState, fStyle, lParam, pszText);
}

BOOL CToolbar::AddSeparator()
{
	return AddCustomButton(-1, 0, 0, 0, TBSTYLE_SEP, 0, INT_PTR( 0 ));
}

BOOL CToolbar::AddButton(int iBitmap, int idCommand, LPCTSTR pszText, BOOL fChecked, BOOL fEnabled, LPARAM lParam)
{
	BYTE fStyle = TBSTYLE_BUTTON;
	BYTE fState = ( fChecked ? TBSTATE_PRESSED : 0 );
	fState |= ( fEnabled ? TBSTATE_ENABLED : 0 );

	return AddCustomButton(iBitmap, idCommand, fState, fStyle, lParam, pszText);
}

BOOL CToolbar::AddDropDownButton(int iBitmap, int idCommand, LPCTSTR pszText, BOOL fChecked, BOOL fEnabled, LPARAM lParam)
{
	BYTE fStyle = TBSTYLE_DROPDOWN;
	BYTE fState = ( fChecked ? TBSTATE_PRESSED : 0 );
	fState |= ( fEnabled ? TBSTATE_ENABLED : 0 );

	return AddCustomButton(iBitmap, idCommand, fState, fStyle, lParam, pszText);
}

BOOL CToolbar::Enabled(int idCommand)
{
	return cSendMessage(TB_ISBUTTONENABLED, idCommand);
}

BOOL CToolbar::Checked(int idCommand)
{
	return cSendMessage(TB_ISBUTTONCHECKED, idCommand);
}

void CToolbar::Enabled(int idCommand, BOOL fEnable)
{
	TBBUTTONINFO tbi = { 0 };
	tbi.cbSize = sizeof(tbi);
	tbi.dwMask = TBIF_COMMAND | TBIF_STATE;
	cSendMessage(TB_GETBUTTONINFO, idCommand, (LPARAM) &tbi);
	
	if ( fEnable )
		tbi.fsState |= TBSTATE_ENABLED;
	else
		tbi.fsState &= ~TBSTATE_ENABLED;

	cSendMessage(TB_SETBUTTONINFO, idCommand, (LPARAM) &tbi);
}

void CToolbar::Checked(int idCommand, BOOL fCheck)
{
	TBBUTTONINFO tbi = { 0 };
	tbi.cbSize = sizeof(tbi);
	tbi.dwMask = TBIF_COMMAND | TBIF_STATE;
	cSendMessage(TB_GETBUTTONINFO, idCommand, (LPARAM) &tbi);
	
	if ( fCheck )
		tbi.fsState |= TBSTATE_CHECKED;
	else
		tbi.fsState &= ~TBSTATE_CHECKED;

	cSendMessage(TB_SETBUTTONINFO, idCommand, (LPARAM) &tbi);
}

void CToolbar::EnableAll(BOOL fEnable, ...)
{
	va_list argptr;
	va_start(argptr, fEnable);
	INT * idList = (INT*) argptr;

	for(INT i = 0; idList[i]; ++i)
		Enabled(idList[i], fEnable);

	va_end(argptr);

}

void CToolbar::CheckAll(BOOL fCheck, ...)
{
	va_list argptr;
	va_start(argptr, fCheck);
	INT * idList = (INT*) argptr;

	for(INT i = 0; idList[i]; ++i)
		Checked(idList[i], fCheck);

	va_end(argptr);
}
