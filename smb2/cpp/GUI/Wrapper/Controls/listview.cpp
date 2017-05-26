
#include <precomp.h>

LPLISTVIEW_PARAM InitializeListViewInstance(HWND hwnd);
void UninitializeListViewInstance(HWND hwnd);
WNDPROC		pfnLvProc;

LRESULT CALLBACK Listview_WindowProc(

    HWND hwnd,	// handle of window
    UINT uMsg,	// message identifier
    WPARAM wParam,	// first message parameter
    LPARAM lParam 	// second message parameter
   );

void InitListView()
{
	WNDCLASS wc = { 0 };
	if ( GetClassInfo(GetModuleHandle(NULL), TEXT("SysListView32"), &wc) )
	{
		pfnLvProc = wc.lpfnWndProc;
		wc.lpfnWndProc = Listview_WindowProc;
		wc.lpszClassName = TEXT("DBFListView32");
		wc.cbWndExtra = 20;
		RegisterClass(&wc);
	}
}

void InitDDControl(LPCTSTR pszClassName)
{
	WNDCLASS wc = { 0 };
	if ( GetClassInfo(GetModuleHandle(NULL), pszClassName, &wc) )
	{
		TCHAR pszNewClassName[MAX_PATH] = { 0 };
		lstrcpy(pszNewClassName, DBF_CONTROL);
		lstrcat(pszNewClassName, pszClassName);
		wc.lpfnWndProc = Listview_WindowProc;
		wc.lpszClassName = pszNewClassName;
		wc.cbWndExtra = 20;
		RegisterClass(&wc);
	}
}

LRESULT CALLBACK Listview_WindowProc(

    HWND hwnd,	// handle of window
    UINT uMsg,	// message identifier
    WPARAM wParam,	// first message parameter
    LPARAM lParam 	// second message parameter
   )
{
	LPLISTVIEW_PARAM lv = LPLISTVIEW_PARAM(GetWindowLong(hwnd, 16));
	WNDPROC pwndProc = (WNDPROC) GetWindowLong(hwnd, 12);
	switch(uMsg)
	{
	case WM_NCCREATE:
		{
			lv = InitializeListViewInstance(hwnd);
			TCHAR pszClassName[MAX_PATH] = { 0 };
			GetClassName(hwnd, pszClassName, MAX_PATH);
			if ( lstrcmpi(pszClassName, TEXT("DBFListView32")) )
			{
				int preflen = sizeof(DBF_CONTROL) / sizeof(TCHAR) - 1, chr = 0;
				while(chr <= preflen && pszClassName[chr] == DBF_CONTROL[chr] )
					chr++;

				if ( chr == preflen )
				{
					WNDCLASS wc = { 0 };
					if ( GetClassInfo( (HINSTANCE) GetWindowLong(hwnd, GWL_HINSTANCE), &pszClassName[chr], &wc ) )
					{
						SetWindowLong(hwnd, 12, (LONG) wc.lpfnWndProc);
						lv->pfnWndProc = wc.lpfnWndProc;
						lv->fIsListView = FALSE;
						pwndProc = wc.lpfnWndProc;
					}
					else
						return FALSE;
				}
				else
					return FALSE;
			}

			SetRect(&lv->rc,
						LPCREATESTRUCT(lParam)->x,
						LPCREATESTRUCT(lParam)->y,
						LPCREATESTRUCT(lParam)->cx,
						LPCREATESTRUCT(lParam)->cy
						);
			lv->hHeader = NULL;
			break;
		}
	case WM_ERASEBKGND:
		{
			if ( lv->fIsListView )
			{
				if ( !lv->hHeader ) lv->hHeader = (HWND) lv->pfnWndProc(hwnd, LVM_GETHEADER, 0, 0);
				GetClientRect(lv->hHeader, &lv->header_rc);
				return CallWindowProc(lv->pfnWndProc, hwnd, uMsg, WPARAM(lv->hDC), lParam);
			}
			else
				return pwndProc(hwnd, uMsg, WPARAM(lv->hDC), lParam);

			break;
		}
	case WM_SIZING:
	case WM_SIZE:
		{
			GetClientRect(hwnd, &lv->rc);
			if ( lv->fIsListView )
			{
				if ( !lv->hHeader ) lv->hHeader = (HWND) lv->pfnWndProc(hwnd, LVM_GETHEADER, 0, 0);
				GetClientRect(lv->hHeader, &lv->header_rc);
			}
			break;
		}
	case WM_PRINT:
		{
			WNDPROC pwnd = ( lv->fIsListView ? lv->pfnWndProc : pwndProc );
//			pwnd(hwnd, WM_ERASEBKGND, (WPARAM) lv->hDC, 0);
//			pwnd(hwnd, WM_PAINT, (WPARAM) lv->hDC, 0);
			pwnd(hwnd, WM_PRINT, (WPARAM) lv->hDC, lParam);

			if ( !PBYTE(lv->pvBits)[0] )
				FillRect((HDC) wParam, &lv->rc, GetSysColorBrush(COLOR_WINDOW));
			else
				BitBlt((HDC) wParam, 0, 0, lv->rc.right, lv->rc.bottom, lv->hDC, 0, 0, SRCCOPY);
			return 0;
		}
	case WM_PAINT:
		{
			PAINTSTRUCT ps = { 0 };
			HDC hdc = BeginPaint(hwnd, &ps);
			if ( lv->fIsListView )
				CallWindowProc(lv->pfnWndProc, hwnd, uMsg, WPARAM(lv->hDC), lParam);
			else
				pwndProc(hwnd, uMsg, WPARAM(lv->hDC), lParam);
			
			if ( lv->fIsListView )
				BitBlt(hdc, 0, lv->header_rc.bottom, lv->rc.right, lv->rc.bottom - lv->header_rc.bottom, lv->hDC, 0, lv->header_rc.bottom, SRCCOPY);
			else
				BitBlt(hdc, 0, 0, lv->rc.right, lv->rc.bottom, lv->hDC, 0, 0, SRCCOPY);

			EndPaint(hwnd, &ps);
			return 0;
		}
	case WM_NCDESTROY:
		{
			LRESULT ret = ( lv->fIsListView ? lv->pfnWndProc(hwnd, uMsg, wParam, lParam) : pwndProc(hwnd, uMsg, wParam, lParam) );
			UninitializeListViewInstance(hwnd);
			return ret;
		}
	}

	if ( lv->fIsListView && lv->pfnWndProc )
		return lv->pfnWndProc(hwnd, uMsg, wParam, lParam);
	else if ( !lv->fIsListView && pwndProc )
		return pwndProc(hwnd, uMsg, wParam, lParam);

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

LPLISTVIEW_PARAM InitializeListViewInstance(HWND hwnd)
{
	LPLISTVIEW_PARAM lv = new LISTVIEW_PARAM;
	
	lv->fIsListView = TRUE;
	lv->pfnWndProc = pfnLvProc;
	lv->hDesktopDC = GetDC(NULL);
	lv->hDC = CreateCompatibleDC(lv->hDesktopDC);

	SIZE sz = { GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) };

	ULONG uSize = sz.cx * sz.cy * sizeof(WRGBQUAD);
	BITMAPINFO bmi = { 0 };
	bmi.bmiHeader.biSize = sizeof(bmi);
	bmi.bmiHeader.biWidth = sz.cx;
	bmi.bmiHeader.biHeight = -sz.cy;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biSizeImage = uSize;

	WRGBQUAD * pv = NULL;

	lv->hBitmap = CreateDIBSection( lv->hDC, &bmi, DIB_RGB_COLORS, (PVOID*) &lv->pvBits, NULL, 0 );
	//	CreateCompatibleBitmap(lv->hDesktopDC, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));

	lv->hOldBmp = HBITMAP(SelectObject(lv->hDC, lv->hBitmap));

	SetWindowLongPtr(hwnd, 16, LPARAM(lv));
	return lv;
}

void UninitializeListViewInstance(HWND hwnd)
{
	LPLISTVIEW_PARAM lv = LPLISTVIEW_PARAM(GetWindowLong(hwnd, 16));

	SelectObject(lv->hDC, lv->hOldBmp);
	DeleteObject(lv->hBitmap);
	DeleteDC(lv->hDC);
	ReleaseDC(NULL, lv->hDesktopDC);
	delete lv;
}