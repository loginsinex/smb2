
class CFont
{
	HFONT			m_hFont;

public:
	CFont();
	CFont(INT iHeight, BOOL bBold, BOOL bItalic, BOOL bStriked, BOOL bUnderline, LPTSTR lpszFontFace, UINT fQuality);
	
	void	Create(INT iHeight, BOOL bBold, BOOL bItalic, BOOL bStriked, BOOL bUnderline, LPTSTR lpszFontFace, UINT fQuality);
	void	Release();

	void	LoadFromWindow(HWND hWndFrom, INT iHeight = -1, INT bBold = -1, INT bItalic = -1, INT bStriked = -1, INT bUnderline = -1, LPTSTR lpszFontFace = NULL, UINT fQuality = -1);

	HFONT Font();
	operator HFONT();
	~CFont();
};