
#define LVIS_CURSEL			(LVIS_SELECTED | LVIS_FOCUSED)
#define DBF_CONTROL			TEXT("DBF")

typedef struct _tagLISTVIEW_PARAM
{
	WNDPROC				pfnWndProc;
	HBITMAP				hBitmap;
	PVOID				pvBits;
	HDC					hDC;
	RECT				rc;

	HDC					hDesktopDC;
	HBITMAP				hOldBmp;

	BOOL				fIsListView;
	RECT				header_rc;
	HWND				hHeader;
} LISTVIEW_PARAM, *LPLISTVIEW_PARAM;

void InitListView();
void InitDDControl(LPCTSTR pszClassName);