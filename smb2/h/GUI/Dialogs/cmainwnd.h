
class CMainWnd: public CDialog
{
	CFileLoader		m_File;
	UINT			m_uCurLevel;

	CLevelDlg		dlvl;
	CLvlOptionsDlg	dlvloptions;

	struct
	{
		CTreeView	tv;
		CTabControl	tc;
		CToolbar	tb;
	} m_ctl;

	void			OnCommand(USHORT uCmd, USHORT uId, HWND hCtl);
	void			OnInit(LPARAM lParam);
	void			OnButton(USHORT uId);
	void			OnSize(UINT fwSizeType, SHORT cx, SHORT cy);
	void			OnSizing(UINT fwSizeType, LPRECT lprc);
	INT_PTR			OnNotify(LPNMHDR lpnm);
	void			AlignPage();
	void			UpdatePages();
	VOID			ShowStat();
	VOID			ListLevels();

public:
	CMainWnd(HINSTANCE hInstance);
	UINT			GetCurLevel();

	VOID			SetPage(INT iIndex);
	BOOL			UnlockLevel();
	BOOL			UnlockEnemy();
	void			SelectLevel(USHORT uPtr, BYTE pos);
};