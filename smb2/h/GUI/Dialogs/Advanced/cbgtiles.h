
class CBgTilesDlg: public CTilesDlg //public CDialog
{
	BYTE					m_pTilesArray[448];
	VOID					OnInit(LPARAM lParam);
	VOID					SetTiles(INT i);
	VOID					LoadItems();
	VOID					SaveItems();
	VOID					SetItems();
	VOID					WriteItems();
	VOID					OnButton(USHORT uId);
	INT_PTR					OnNotify(LPNMHDR lpnm);
	VOID					OnCommand(USHORT uCmd, USHORT uId, HWND hCtl);
	INT						PSOnApply();

	CComboBox				m_cbWorld;
	CTrackBar				m_tbBgType;
	USHORT					m_cbTile[5];
	DWORD					m_uTbPos;
	
	//struct
	//{
	//	HIMAGELIST				himl[5];
	//} m_Image;

public:
	CBgTilesDlg(HINSTANCE hInstance, CFileLoader & flLoader);
	~CBgTilesDlg();
};