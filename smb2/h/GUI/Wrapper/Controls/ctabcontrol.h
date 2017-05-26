
class CTabControl: public CControl
{
public:
	void		AddItem(LPCTSTR pszText, LPARAM lParam = 0);
	void		Param(int i, LPARAM lParam);
	LPARAM		Param(int i);
	INT			GetCurSel();
	void		SetCurSel(INT i);
	INT			ItemsCount();
	void		Adjust(HWND hWnd);
	void		DeferAdjust(HDWP hDwp, HWND hWnd);
	void		SetText(int i, LPCTSTR pszText);
};
