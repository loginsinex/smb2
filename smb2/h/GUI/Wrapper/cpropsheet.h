
class CPropSheet
{
	HINSTANCE				m_hInstance;
	HWND					m_hWnd;				// this field will set by CDialog::PropSheetDialogProc !!!
	BOOL					m_fCreated;
	BOOL					m_fSetToClose;
	vector<PROPSHEETPAGE>	m_vPSList;
	DWORD					m_dwFlags;
	CDialog	*				m_pParent;

	void					SetFlag(DWORD dwFlag, BOOL fSet);
	INT_PTR					pSendMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0);
	CDialog *				WindowToDialog(HWND hWnd);
	HWND					DialogToWindow(CDialog * pDialog);
	friend INT_PTR CALLBACK CDialog::PropSheetDialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

public:

	CPropSheet(HINSTANCE hInstance);
	~CPropSheet();

	BOOL			AddSheet(CDialog * pDialog, LPARAM lParam = 0, LPTSTR pszTitle = NULL, BOOL fForceCreate = FALSE);
	INT_PTR			Show(CDialog * pParent, LPARAM lParam = 0, LPTSTR pszCaption = NULL, DWORD dwFirstPage = 0);
	CDialog *		GetParent();
	HWND			GetWindowHandle();

	VOID			FApply(BOOL fApply);
	VOID			FAppendTitle(BOOL fAppendTitle);

	VOID			EChanged(BOOL fChanged, CDialog * pCallee);
	VOID			ESetClose();
	BOOL			EGetClose();
	HWND			ETabCtl();
	VOID			EDoCommand(UINT uButton);
	VOID			ENeedReboot();
	VOID			ENeedRestart();
	VOID			ETitle(LPTSTR lpszTitle, BOOL fPropertiesForFlag = FALSE);
	CDialog *		EGetPage(INT iIndex);
	INT				EGetIndex(CDialog * pDialog);
	CDialog *		EGetCurrentPage();
	INT				EGetCurrentIndex();
	VOID			ESetCurrentPage(INT iIndex);
	VOID			ESetCurrentPage(CDialog * pDialog);
};