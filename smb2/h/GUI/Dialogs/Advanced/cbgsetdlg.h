
class CBgSetHorzDlg: public CTilesDlg
{
	CListView		m_lvTiles;
	CTrackBar		m_tbSet;
	CComboBox		m_cbWorld;

	DWORD			m_dwBgSet[32];
	DWORD			m_tbPos;
	BOOL			m_fHorizontal;

	VOID			SetTiles(INT iWorld);
	VOID			OnInit(LPARAM lParam);
	INT_PTR			OnNotify(LPNMHDR lpnm);
	VOID			OnCommand(USHORT uCmd, USHORT uId, HWND hWnd);
	INT				PSOnApply();

	VOID			LoadItems();
	VOID			SaveItems();
	VOID			SetItems();
	VOID			WriteItems();
	
public:
	CBgSetHorzDlg(HINSTANCE hInstance, CFileLoader & fl, BOOL fHorizontal);

};
