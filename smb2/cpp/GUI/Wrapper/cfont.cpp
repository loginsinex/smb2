#include "precomp.h"

CFont::CFont() : m_hFont( NULL )
{

}

CFont::CFont(INT iHeight, BOOL bBold, BOOL bItalic, BOOL bStriked, BOOL bUnderline, LPTSTR lpszFontFace, UINT fQuality) : m_hFont( NULL )
{
	Create(iHeight, bBold, bItalic, bStriked, bUnderline, lpszFontFace, fQuality);
}

void CFont::LoadFromWindow(HWND hWndFrom, INT iHeight, INT bBold, INT bItalic, INT bStriked, INT bUnderline, LPTSTR lpszFontFace, UINT fQuality)
{
	if ( !IsWindow(hWndFrom) ) return;

	HDC hWindowDC = GetWindowDC(hWndFrom);
	HGDIOBJ holdFn = SelectObject(hWindowDC, (HFONT) SendMessage(hWndFrom, WM_GETFONT, 0, 0));
	TEXTMETRIC tm = { 0 };
	GetTextMetrics(hWindowDC, &tm);
	
	TCHAR pszFontFace[MAX_PATH+1];
	GetTextFace(hWindowDC, MAX_PATH, pszFontFace);

	if ( -1 == iHeight ) iHeight = tm.tmHeight;
	if ( -1 == bBold ) bBold = ( 0 != tm.tmWeight );
	if ( -1 == bItalic ) bItalic = tm.tmItalic;
	if ( -1 == bStriked ) bStriked = tm.tmStruckOut;
	if ( -1 == bUnderline ) bUnderline = tm.tmUnderlined;
	if ( -1 == fQuality ) fQuality = 0;
	if ( NULL == lpszFontFace ) lpszFontFace = pszFontFace;

	SelectObject(hWindowDC, holdFn);
	ReleaseDC(hWndFrom, hWindowDC);

	Create(iHeight, bBold, bItalic, bStriked, bUnderline, lpszFontFace, fQuality);
}

void CFont::Create(INT iHeight, BOOL bBold, BOOL bItalic, BOOL bStriked, BOOL bUnderline, LPTSTR lpszFontFace, UINT fQuality)
{
	if ( m_hFont ) return;
	m_hFont = CreateFont(iHeight, 0, 0, 0, ( bBold ? FW_BLACK : 0 ), bItalic, bUnderline, bStriked, ANSI_CHARSET, 0, 0, fQuality, 0, lpszFontFace);
}

void CFont::Release()
{
	if ( m_hFont ) DeleteObject(m_hFont);
	m_hFont = NULL;
}

HFONT CFont::Font()
{
	return m_hFont;
}

CFont::operator HFONT()
{
	return m_hFont;
}

CFont::~CFont()
{
	if ( m_hFont ) DeleteObject(m_hFont);
	m_hFont = NULL;
}
