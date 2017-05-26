
class CPaletteDlg: public CDialog
{
	CFileLoader				& m_File;
	BOOL					m_fAdditional;
	CComboBox				m_cbWorld;
	CTrackBar				m_tbPal;
	USHORT					m_cbPal[16];
	BYTE					m_pPalette[16*8*7];

	struct
	{
		HDC					hDC;
		HDC					hDCBuffer;
		HBITMAP				hBmpBuffer;
		HBITMAP				hOldBmp;
		HIMAGELIST			hIml;
	} m_Buffer;

	VOID					OnInit(LPARAM lParam);
	INT						OnMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & fHandle);
	VOID					OnCommand(USHORT uCmd, USHORT uId, HWND hCtl);
	INT						PSOnApply();
	VOID					LoadItems();
	VOID					SaveItems();
	VOID					SetItems();
	VOID					WriteItems();

	VOID					MakeBuffer();
	VOID					ReleaseBuffer();
	HIMAGELIST				GetImage();

public:
	CPaletteDlg(HINSTANCE hInstance, CFileLoader & fl, BOOL fAdditional = FALSE);
	~CPaletteDlg();
};