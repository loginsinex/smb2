
class CRebarWindow: public CControl
{

public:
	INT				InsertBand(REBARBANDINFO * lpBand);
	INT				Count();
	INT				InsertBand(HWND hWnd, LPARAM lParam = 0, BOOL fBreak = FALSE, BOOL fMovable = TRUE, BOOL fTopAlign = FALSE, LPCTSTR pszText = NULL, int cx = -1);
	INT				Width(int idBand);
	void			Width(int idBand, int cx);
};