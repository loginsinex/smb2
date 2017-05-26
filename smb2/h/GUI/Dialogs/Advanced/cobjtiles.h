
class CTilesDlg: public CDialog
{
	INT OnMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & pHandle);

protected:
	CFileLoader				& m_File;
	CTilesDlg(HINSTANCE	hInstance, CFileLoader & fl, LPTSTR lpszRes);
	~CTilesDlg();

	INT						GetFX(INT iWorld);

	virtual VOID			SetTiles(INT i) = 0;
	VOID					LoadTiles();

	struct
	{
		HIMAGELIST				himl[5];
	} m_Image;

};

class CObjTilesDlg: public CTilesDlg
{
	BYTE					m_pTilesArray[252];
	VOID					OnInit(LPARAM lParam);
	VOID					SetTiles(INT i);
	VOID					LoadItems();
	VOID					SaveItems();
	VOID					SetItems();
	VOID					WriteItems();
	VOID					OnCommand(USHORT uCmd, USHORT uId, HWND hCtl);
	INT						PSOnApply();

	CComboBox				m_cbWorld;
	CComboBox				m_cbObj;
	USHORT					m_cbTile[4];

public:
	CObjTilesDlg(HINSTANCE hInstance, CFileLoader & flLoader);
	~CObjTilesDlg();
};