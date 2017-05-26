
#define		SPINVAL(x)		( dSendMessage((x), UDM_GETPOS) )
#define		SPINSET(x, a)	( dSendMessage((x), UDM_SETPOS, 0, MAKELONG((a), 0)) )
#define		SPINRANGE(x, a, b)	( dSendMessage((x), UDM_SETRANGE, 0, MAKELONG((a), (b))) )
#define		SPINERR(x)		( 0 != ( HIWORD(SPINVAL(x) ) ) )

class CPropSheet;

typedef enum _tagCTLBRUSH
{
	ctlbNone		= 0,
	ctlbBtn			= 1,
	ctlbDlg			= 2,
	ctlbEdit		= 3,
	ctlbList		= 4,
	ctlbScroll		= 5,
	ctlbStatic		= 6,
	ctlbMbox		= 7
} CTLBRUSH;

void			CreateNewDialogClass(HINSTANCE hInstance, LPCTSTR pszNewClass);

#define SHELL_FONT		L"MS Shell Dlg"
#define MAX_FIELD_TEXT	300

#ifndef DS_SHELLFONT 
#define DS_SHELLFONT (DS_SETFONT | DS_FIXEDSYS)
#endif

typedef enum _tagDLG_ITEM_PRECLASS
{
	cCustom		= 0x0000,
	cButton		= 0x0080,
	cEdit		= 0x0081,
	cStatic		= 0x0082,
	cListbox	= 0x0083,
	cScrollbar	= 0x0084,
	cCombobox	= 0x0085
} DLG_ITEM_PRECLASS, *PDLG_ITEM_PRECLASS;

typedef struct _tagDLG_TPL
{
	WORD dlgVer;
	WORD signature;
	DWORD helpID;
	DWORD exStyle;
	DWORD style;
	WORD cDlgItems;
	short x;
	short y;
	short cx;
	short cy;
	struct
	{
		WCHAR pszMenu[MAX_FIELD_TEXT];
		USHORT uId;
	} menuId;

	struct
	{
		WCHAR pszClassName[MAX_FIELD_TEXT];
		USHORT atomId;
	} dlgClass;

	WCHAR pszTitle[MAX_FIELD_TEXT];
	WORD pointsize;
	WORD weight;
	BYTE italic;
	BYTE charset;
	WCHAR typeface[MAX_FIELD_TEXT];
} DLG_TPL, *PDLG_TPL;

typedef struct _tagDLG_ITEM
{
    DWORD helpID;
    DWORD exStyle;
    DWORD style;
    short x;
    short y;
    short cx;
    short cy;
    DWORD id;
    struct
	{
		WCHAR windowClass[MAX_FIELD_TEXT];
		DLG_ITEM_PRECLASS	cPreClass;
	} cls;
    WCHAR title[MAX_PATH];
    WORD extraCount;
	PBYTE	pvExtraData;
} DLG_ITEM, *PDLG_ITEM;


class CDialogTemplate
{
	PBYTE					m_pTemplate;
	BOOL					m_fTemplateChanged;
	DLG_TPL					m_dlgTpl;
	std::vector<DLG_ITEM>	m_vdlgItems;
	int						StrCopy(LPWSTR pszDest, LPCTSTR pszSrc, int len = 0);
	
	INT						GetDlgItemTemplate(DLG_ITEM & di, PBYTE pBuffer);
	PBYTE					AddData(PVOID pData, int dataLen, PBYTE pBuffer);
	PBYTE					AddDataAligned(PVOID pData, int dataLen, PBYTE pBuffer);
	PBYTE					AddDataAligned4(PVOID pData, int dataLen, PBYTE pBuffer);

public:
	CDialogTemplate();
	~CDialogTemplate();

	void				SetDlgTitle(LPCTSTR pszTitle);
	void				SetDlgStyle(DWORD style, DWORD styleEx);
	void				SetDlgPos(int x, int y, int cx, int cy);
	void				SetDlgFont(LPCTSTR pszFont, WORD size, WORD weight, BOOL fItalic, BYTE bCharset);
	void				SetDlgMenu(LPCTSTR pszMenu);
	void				SetDlgExtra(DWORD helpId);
	void				SetDlgClass(LPCTSTR pszClass);
	void				SetDlgClass(WORD atomId);

	void				AddStyle(DWORD dwStyle);
	void				AddExStyle(DWORD dwStyle);

	
	int					AddItem(USHORT uId, DLG_ITEM_PRECLASS cls, LPCTSTR pszCaption,
								int x, int y, int cx, int cy,
								DWORD style = 0, DWORD exStyle = 0, DWORD helpId = 0,
								PVOID pvExtraData = NULL, int extraLen = 0);

	int					AddItem(USHORT uId, LPCTSTR pszClass, LPCTSTR pszCaption,
								int x, int y, int cx, int cy,
								DWORD style = 0, DWORD exStyle = 0, DWORD helpId = 0,
								PVOID pvExtraData = NULL, int extraLen = 0);
	
	int					AddPreItem(USHORT uId, DLG_ITEM_PRECLASS cls, LPCTSTR pszCaption,
								int x, int y, int cx, int cy, DWORD style = 0, 
								BOOL fVisible = TRUE, BOOL fEnable = TRUE, BOOL fTabStop = TRUE);

	int					AddPreItem(USHORT uId, LPCTSTR pszClass, LPCTSTR pszCaption,
								int x, int y, int cx, int cy, DWORD style = 0,
								BOOL fVisible = TRUE, BOOL fEnable = TRUE, BOOL fTabStop = TRUE);

	LPCDLGTEMPLATE		Template();
};


class CDialog
{
	HINSTANCE				m_hInstance;
	LPTSTR					m_lpszResource;
	HWND					m_hWnd;
	LPARAM					m_lUserParam;
	CDialog					*m_pParent;
	CPropSheet				*m_pPropSheet;
	vector<CSizeAnchor*>	m_vAncList;
	CString					m_sResStr;
	TCHAR					m_pszMaxPath[MAX_PATH + 1];
	std::map<USHORT, HBRUSH>		m_mBrushList;
	BOOL					m_fNeedPostQuit;
	BOOL					m_fDenyCalls;
	BOOL					m_fWindowDestroyed;
	BOOL					m_fShowModeless;
	CDialogTemplate			m_dlgTemplate;

	struct
	{
		HBRUSH				hBtnBrush;
		HBRUSH				hDlgBrush;
		HBRUSH				hEditBrush;
		HBRUSH				hListBoxBrush;
		HBRUSH				hScrollBrush;
		HBRUSH				hStaticBrush;
		HBRUSH				hMsgBox;
	} m_vBrushes;

	static			INT_PTR CALLBACK PropSheetDialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static			INT_PTR CALLBACK DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	VOID			InitAnchors();
	VOID			InternalOnSize(UINT fwSizeType, SHORT cx, SHORT cy);
	VOID			InternalOnSizing(UINT fwSizeType, LPRECT lprc);
	INT_PTR			InternalOnNotify(LPNMHDR lpnm);
	INT_PTR			InternalOnPSNotify(UINT code, LPPSHNOTIFY lpnm);
	void			InternalOnCommand(USHORT uCmd, USHORT uId, HWND hCtl);
	HBRUSH			InternalOnColorDialog(HDC hDCDialog, HWND hDlg, UINT wmCtl);
	void			InternalOnDestroy();
	INT				MsgBox(const TCHAR * lpszText, DWORD dwMsgBoxFlags);
	CDialogTemplate & Dialog();

protected:
	CDwmApi			m_dwm;
	CDialog(HINSTANCE hInstance, LPTSTR lpszResource);
	CDialog(HINSTANCE hInstance);
	virtual			~CDialog();

	void			MainWindow();
	void			Close(UINT uRet);
	void			SetCtlFont(USHORT uId, CFont & cFont);
	void			SetCtlText(USHORT uId, LPCTSTR lpszText);
	CString			GetCtlText(USHORT uId);
	void			SetBgBrush(HBRUSH hBrush, CTLBRUSH ctlb);
	HBRUSH			GetBgBrush(CTLBRUSH ctlb);
	void			SetBgSysColor(UINT uSysColor, CTLBRUSH ctlb);
	void			SetBgDialogColor(COLORREF clr);
	void			SetBgCtlColor(COLORREF clr, USHORT uId);
	BOOL			GetRect(USHORT uId, LPRECT lprc, BOOL fPos = FALSE);
	CString			MB_GetString(UINT wButton);
	virtual			void OnPreInit(CDialogTemplate & t);
	virtual			void OnInit(LPARAM lParam);
	virtual			BOOL OnClose(INT & iRetCode);
	virtual			void OnDestroy();
	virtual			INT_PTR OnNotify(LPNMHDR lpnm);
	virtual			void OnCommand(USHORT uCmd, USHORT uId, HWND hCtl);
	virtual			void OnButton(USHORT uId);
	virtual			void OnOK();
	virtual			void OnCancel();
	virtual			void OnPaint(HDC hDC, LPPAINTSTRUCT lpps);
	virtual			void OnSize(UINT fwSizeType, SHORT cx, SHORT cy);
	virtual			void OnSizing(UINT fwSizeType, LPRECT lprc);
	virtual			INT OnMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & pHandle);
	virtual			HBRUSH OnColorDialog(HDC hDCDialog, HWND hDlg, CTLBRUSH ctlb);
	virtual			INT PSOnApply();
	virtual			void PSOnHelp();
	virtual			BOOL PSOnKillActive();
	virtual			BOOL PSOnQueryCancel();
	virtual			void PSOnRest();
	virtual			INT PSOnSetActive();
	virtual			void OnThemeChanged();
	virtual			void OnSysColorChanged();

public:
	HWND			Create(CDialog * pParent = NULL, LPARAM lUserParam = 0);
	INT				Show(CDialog * pParent = NULL, LPARAM lUserParam = 0);
	HWND			GetWindowHandle();
	HWND			Ctl(USHORT uCtlId);
	HINSTANCE		GetInstance();
	CDialog			*Parent();
	CPropSheet		*PS();
	INT_PTR			dSendMessage(USHORT uCtlId, UINT uMsg, WPARAM wParam = 0, LPARAM lParam = NULL);
	INT				DoLoop(HWND hWndAccel = NULL, HACCEL hAccel = NULL);
	operator		HWND();
	virtual			HWND GetCustomMenu();

// MessageBoxes
	void			ShowMessageF(const TCHAR * tpl, ...);
	void			ShowMessage(const TCHAR * lpszText);
	BOOL			Confirm(const TCHAR * lpszText, BOOL bDefaultNo = FALSE);
	void			ShowWarning(const TCHAR * lpszText);
	void			ShowError(const TCHAR * lpszText);
	void			ShowErrorF(const TCHAR * lpszText, ...);
	CString			OpenFile(LPCTSTR pszDefault, ...);
	CString			SaveFile(LPCTSTR pszDefault, ...);
	VOID			AddAnchor(UINT uId, UINT uAnchorType);
	VOID			AddCustomAnchor(UINT uId, AnchorType left, AnchorType top, AnchorType right, AnchorType bottom);
	VOID			UpdateAnchor(USHORT uId);
	LPTSTR			GetString(UINT uRes);
	CString			GetStr(UINT uRes);
	CString			Title();
	void			Title(CString sTitle);
	VOID			Visible(BOOL fVisible);
	BOOL			Visible();
	VOID			Visible(USHORT uId, BOOL fVisible);
	BOOL			Visible(USHORT uId);
	VOID			Enable(BOOL bEnableWindow);
	BOOL			Enable();
	VOID			Enable(USHORT uId, BOOL bEnableCtl);
	BOOL			Enable(USHORT uId);
	VOID			EnableAll(BOOL bEnable, ...);
	VOID			ShowAll(BOOL bEnable, ...);
	VOID			CheckAll(BOOL bCheck, ...);
	UINT			Checked(USHORT uId);
	BOOL			IsWindowDestroyed();
	friend			CPropSheet;
};