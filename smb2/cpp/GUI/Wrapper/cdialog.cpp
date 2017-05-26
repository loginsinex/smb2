
#include "precomp.h"

void CreateNewDialogClass(HINSTANCE hInst, LPCTSTR pszNewClass)
{
	WNDCLASS wc = { 0 };
	wc.cbWndExtra = DLGWINDOWEXTRA;
	wc.lpfnWndProc = DefDlgProc;
	wc.hInstance = hInst;
	wc.lpszClassName = pszNewClass;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = GetSysColorBrush(COLOR_WINDOW);
	RegisterClass(&wc);
}

CDialog::CDialog(HINSTANCE hInstance)
	: m_hInstance(hInstance), m_lpszResource( NULL ),
	m_hWnd( NULL ), m_lUserParam( 0 ), m_pPropSheet( NULL ), m_pParent( NULL ),
	m_fNeedPostQuit( FALSE ),
	m_fDenyCalls( TRUE ),
	m_fWindowDestroyed( TRUE ), m_fShowModeless( FALSE )
{
	m_vBrushes.hBtnBrush = 
		m_vBrushes.hDlgBrush = 
		m_vBrushes.hEditBrush = 
		m_vBrushes.hListBoxBrush = 
		m_vBrushes.hScrollBrush = 
		m_vBrushes.hStaticBrush = 
		m_vBrushes.hMsgBox =
		(HBRUSH) -1;

	m_fDenyCalls = FALSE;
}

CDialog::CDialog(HINSTANCE hInstance, LPTSTR lpszResource)
	: m_hWnd( NULL ), m_lUserParam( 0 ), m_pPropSheet( NULL ), m_pParent( NULL ),
	m_fNeedPostQuit( FALSE ),
	m_fDenyCalls( TRUE ),
	m_fWindowDestroyed( TRUE ), m_fShowModeless( FALSE )
{
	m_hInstance = hInstance;
	m_lpszResource = lpszResource;

	m_vBrushes.hBtnBrush = 
		m_vBrushes.hDlgBrush = 
		m_vBrushes.hEditBrush = 
		m_vBrushes.hListBoxBrush = 
		m_vBrushes.hScrollBrush = 
		m_vBrushes.hStaticBrush = 
		m_vBrushes.hMsgBox =
		(HBRUSH) -1;

	m_fDenyCalls = FALSE;
}

CDialog::~CDialog()
{
	m_fDenyCalls = TRUE;
	if ( m_hWnd ) DestroyWindow(m_hWnd);

	for(vector<CSizeAnchor*>::iterator v=m_vAncList.begin();v<m_vAncList.end();v++)
		delete (*v);

	m_vAncList.clear();

	for(map<USHORT, HBRUSH>::iterator br=m_mBrushList.begin();br!=m_mBrushList.end();br++)
		DeleteObject(br->second);

	m_mBrushList.clear();

	m_hWnd = NULL;
	m_hInstance = NULL;
	m_lpszResource = NULL;
	m_lUserParam = 0;
	m_fDenyCalls = FALSE;
}

//BOOL CALLBACK CDialog::PropSheetDialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
INT_PTR CALLBACK CDialog::PropSheetDialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_INITDIALOG:
		{
			LPPROPSHEETPAGE lppsp = (LPPROPSHEETPAGE) lParam;
			CDialog * This = (CDialog*) lppsp->lParam;
			if ( This->m_pPropSheet )
				This->m_pPropSheet->m_hWnd = GetParent(hWnd);

			return CDialog::DialogProc(hWnd, uMsg, wParam, lppsp->lParam);
		}
	}

	return CDialog::DialogProc(hWnd, uMsg, wParam, lParam);
}

INT_PTR CALLBACK CDialog::DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CDialog * This = (CDialog*) GetWindowLong(hWnd, DWL_USER);

#ifdef UNICODE
	if ( WM_NOTIFYFORMAT == uMsg )
		return NFR_UNICODE;
#endif

	if ( This )
	{
		if ( !This->m_fDenyCalls )
		{
			BOOL fHandle = FALSE;
			INT ret = This->OnMessage(uMsg, wParam, lParam, fHandle);
			if ( fHandle )
				return ret;
		}
		else			// we're in constructor or destructor
		{
			if ( WM_DESTROY == uMsg )
				This->InternalOnDestroy();
			else if ( WM_NCDESTROY == uMsg )
			{
				This->m_hWnd = NULL;
				This->m_pParent = NULL;
				This->m_lUserParam = 0;
				This->m_pPropSheet = NULL;
			}

			return DefWindowProc(hWnd, uMsg, wParam, lParam);
		}

	}
	else if ( !This && WM_INITDIALOG != uMsg )
		return DefWindowProc(hWnd, uMsg, wParam, lParam);

	switch(uMsg)
	{
	case WM_INITDIALOG:
		{
			This = (CDialog*) lParam;
			This->m_fWindowDestroyed = FALSE;
			SetWindowLong(hWnd, DWL_USER, lParam);
			This->m_hWnd = hWnd;
			This->InitAnchors();
			if ( This->m_pParent )
			{
				SendMessage(*This, WM_SETICON, ICON_BIG, SendMessage(*This->m_pParent, WM_GETICON, ICON_BIG, 0));
				SendMessage(*This, WM_SETICON, ICON_SMALL, SendMessage(*This->m_pParent, WM_GETICON, ICON_SMALL, 0));
			}

			This->OnInit(This->m_lUserParam);
			break;
		}
	case WM_THEMECHANGED:
		{
			This->OnThemeChanged();
			break;
		}
	case WM_SYSCOLORCHANGE:
		{
			This->OnSysColorChanged();
			break;
		}
	case WM_SIZE: 
		{
			This->InternalOnSize(wParam, LOWORD(lParam), HIWORD(lParam));
			break;
		}
	case WM_PAINT:
		{
			PAINTSTRUCT ps = { 0 };
			HDC hDC = BeginPaint(hWnd, &ps);
			This->OnPaint(hDC, &ps);
			EndPaint(hWnd, &ps);
			return 0;
		}
	case WM_SIZING:
		{
			This->InternalOnSizing(wParam, LPRECT(lParam));
			break;
		}
	case WM_COMMAND:
		{
			This->InternalOnCommand(HIWORD(wParam), LOWORD(wParam), (HWND) lParam);
			break;
		}
	case WM_CLOSE:
		{
			INT iRetCode = 0;
			if ( This->OnClose(iRetCode) )
			{
				
				// EndDialog(hWnd, iRetCode);
				This->Close(iRetCode);
				if ( This->m_fNeedPostQuit )
					PostQuitMessage( 0 );
			}
			break;
		}
	case WM_NOTIFY:
		{
			INT_PTR iReturn = This->InternalOnNotify((LPNMHDR) lParam);
			SetWindowLong(hWnd, DWL_MSGRESULT, iReturn);
			return iReturn;
		}
	case WM_CTLCOLORBTN:
	case WM_CTLCOLORDLG:
	case WM_CTLCOLOREDIT:
	case WM_CTLCOLORLISTBOX:
	case WM_CTLCOLORSCROLLBAR:
	case WM_CTLCOLORSTATIC:
	case WM_CTLCOLORMSGBOX:
		{
			HBRUSH hBrush = This->InternalOnColorDialog((HDC) wParam, (HWND) lParam, uMsg);
			if ( (HBRUSH) -1 != hBrush ) return (LRESULT) hBrush;
			return NULL;
		}
	case WM_DESTROY:
		{
			This->InternalOnDestroy();
			break;
		}
	case WM_NCDESTROY:
		{
			This->m_hWnd = NULL;
			This->m_pParent = NULL;
			This->m_lUserParam = 0;
			This->m_pPropSheet = NULL;
			break;
		}
	default:
		{
			return FALSE;
		}
	}

	return TRUE;
}

HWND CDialog::Create(CDialog * pParent, LPARAM lParam)
{
	if ( m_hWnd ) return m_hWnd;

	m_lUserParam = lParam;
	m_pParent = pParent;
	m_fShowModeless = TRUE;
	m_pPropSheet = NULL;

	if ( m_lpszResource )
		return
			CreateDialogParam(
					m_hInstance,
					m_lpszResource,
					( pParent ? pParent->GetWindowHandle() : NULL ),
					CDialog::DialogProc,
					(LPARAM) this
			);
	else
	{
		OnPreInit(Dialog());
		return
			CreateDialogIndirectParam(
					m_hInstance,
					Dialog().Template(),
					( pParent ? pParent->GetWindowHandle() : NULL ),
					CDialog::DialogProc,
					(LPARAM) this
			);
	}

}

INT CDialog::Show(CDialog * pParent, LPARAM lParam)
{
	if ( m_hWnd ) return -1;

	m_fShowModeless = FALSE;
	m_lUserParam = lParam;
	m_pParent = pParent;
	m_pPropSheet = NULL;

	if ( m_lpszResource )
		return
			DialogBoxParam(
					m_hInstance,
					m_lpszResource,
					( pParent ? pParent->GetWindowHandle() : NULL ),
					CDialog::DialogProc,
					(LPARAM) this
		);
	else
	{
		OnPreInit(Dialog());
		return
			DialogBoxIndirectParam(
					m_hInstance,
					Dialog().Template(),
					( pParent ? pParent->GetWindowHandle() : NULL ),
					CDialog::DialogProc,
					(LPARAM) this
		);
	}

}

void CDialog::MainWindow()
{
	m_fNeedPostQuit = TRUE;
}

void CDialog::Close(UINT uRet)
{
	if ( m_fShowModeless )
		::DestroyWindow(m_hWnd);
	else
		::EndDialog(m_hWnd, uRet);

	if ( m_fNeedPostQuit )
		PostQuitMessage( uRet );
}

HWND CDialog::GetWindowHandle()
{
	return m_hWnd;
}

HWND CDialog::Ctl(USHORT uCtlId)
{
	return GetDlgItem(m_hWnd, uCtlId);
}

HINSTANCE CDialog::GetInstance()
{
	return m_hInstance;
}

CDialog * CDialog::Parent()
{
	return m_pParent;
}

CPropSheet * CDialog::PS()
{
	return m_pPropSheet;
}

void CDialog::SetCtlFont(USHORT uId, CFont & cFont)
{
	dSendMessage(uId, WM_SETFONT, (WPARAM) cFont.Font(), MAKELONG(TRUE, 0));
}

void CDialog::SetCtlText(USHORT uId, LPCTSTR lpszText)
{
	dSendMessage(uId, WM_SETTEXT, NULL, (LPARAM) lpszText);
}

CString CDialog::GetCtlText(USHORT uId)
{
	INT len = dSendMessage(uId, WM_GETTEXTLENGTH);
	if ( len > 0 )
	{
		CString s;
		s.GetBufferSetLength(len + 1);
		dSendMessage(uId, WM_GETTEXT, len + 1, (LPARAM) s.GetBuffer());
		s.ReleaseBuffer();
		return CString( s );
	}

	return CString(TEXT(""));
}

VOID CDialog::Enable(BOOL bEnableWindow)
{
	EnableWindow(m_hWnd, bEnableWindow);
}

BOOL CDialog::Enable()
{
	return IsWindowEnabled(m_hWnd);
}

VOID CDialog::Enable(USHORT uId, BOOL bEnableCtl)
{
	EnableWindow(Ctl(uId), bEnableCtl);
}

BOOL CDialog::Enable(USHORT uId)
{
	return IsWindowEnabled(Ctl(uId));
}

VOID CDialog::EnableAll(BOOL bEnable, ...)
{
	va_list argptr;
	va_start(argptr, bEnable);
	INT * idList = (INT*) argptr;

	for(INT i = 0; idList[i]; ++i)
		Enable(idList[i], bEnable);

	va_end(argptr);
}

VOID CDialog::ShowAll(BOOL bVisible, ...)
{
	va_list argptr;
	va_start(argptr, bVisible);
	INT * idList = (INT*) argptr;

	for(INT i = 0; idList[i]; ++i)
		Visible(idList[i], bVisible);

	va_end(argptr);
}

VOID CDialog::CheckAll(BOOL bCheck, ...)
{
	va_list argptr;
	va_start(argptr, bCheck);
	INT * idList = (INT*) argptr;

	for(INT i = 0; idList[i]; ++i)
		dSendMessage(idList[i], BM_SETCHECK, ( bCheck ? BST_CHECKED : BST_UNCHECKED ));

	va_end(argptr);
}

UINT CDialog::Checked(USHORT uId)
{
	return (UINT) dSendMessage(uId, BM_GETCHECK);
}

VOID CDialog::SetBgBrush(HBRUSH hBrush, CTLBRUSH ctlb)
{
	switch(ctlb)
	{
	case ctlbDlg: m_vBrushes.hDlgBrush = hBrush; break;
	case ctlbBtn: m_vBrushes.hBtnBrush = hBrush; break;
	case ctlbEdit: m_vBrushes.hEditBrush= hBrush; break;
	case ctlbList: m_vBrushes.hListBoxBrush = hBrush; break;
	case ctlbScroll: m_vBrushes.hScrollBrush = hBrush; break;
	case ctlbStatic: m_vBrushes.hStaticBrush = hBrush; break;
	case ctlbMbox: m_vBrushes.hMsgBox = hBrush; break;
	}
}

void CDialog::SetBgSysColor(UINT uSysColor, CTLBRUSH ctlb)
{
	SetBgBrush(GetSysColorBrush(uSysColor), ctlb);
}

HBRUSH CDialog::GetBgBrush(CTLBRUSH ctlb)
{
	HBRUSH h;
	switch(ctlb)
	{
	case ctlbDlg: h = m_vBrushes.hDlgBrush; break;
	case ctlbBtn: h = m_vBrushes.hBtnBrush; break;
	case ctlbEdit: h = m_vBrushes.hEditBrush; break;
	case ctlbList: h = m_vBrushes.hListBoxBrush; break;
	case ctlbScroll: h = m_vBrushes.hScrollBrush; break;
	case ctlbStatic: h = m_vBrushes.hStaticBrush; break;
	case ctlbMbox: h = m_vBrushes.hMsgBox; break;
	default: h = (HBRUSH) -1; break;
	}

	return h;
}

void CDialog::SetBgDialogColor(COLORREF clr)
{
	HBRUSH hBrush = CreateSolidBrush(clr);
	if ( !hBrush ) return;

	map<USHORT, HBRUSH>::iterator br = m_mBrushList.find(0);
	if ( m_mBrushList.end() != br )
	{
		DeleteObject(br->second);
	}
	else
		m_mBrushList[0] = hBrush;
}

void CDialog::SetBgCtlColor(COLORREF clr, USHORT uCtl)
{
	if ( 0 == uCtl )
		return;			// must use SetBgDialogColor

	HBRUSH hBrush = CreateSolidBrush(clr);
	if ( !hBrush ) return;

	map<USHORT, HBRUSH>::iterator br = m_mBrushList.find(uCtl);
	if ( m_mBrushList.end() != br )
	{
		DeleteObject(br->second);
	}
	
	m_mBrushList[uCtl] = hBrush;
	InvalidateRect( Ctl(uCtl), NULL, TRUE );
}

INT_PTR CDialog::dSendMessage(USHORT uCtlId, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return SendDlgItemMessage(m_hWnd, uCtlId, uMsg, wParam, lParam);
}

INT CDialog::MsgBox(const TCHAR * lpszText, DWORD dwMsgBoxFlags)
{
	static BOOL fIsWin6x = ( LOWORD(GetVersion()) >= 6 );
	static HRESULT (WINAPI * pTaskDialog)(HWND,HINSTANCE,PCWSTR,PCWSTR,PCWSTR,TASKDIALOG_COMMON_BUTTON_FLAGS,PCWSTR,int *)
		=
		(HRESULT (WINAPI * )(HWND,HINSTANCE,PCWSTR,PCWSTR,PCWSTR,TASKDIALOG_COMMON_BUTTON_FLAGS,PCWSTR,int *)) 
			GetProcAddress(GetModuleHandle(TEXT("COMCTL32")), "TaskDialog");

	HWND hTitle = m_hWnd;
	TCHAR pszTitle[MAX_PATH+1];
	do
	{
		GetWindowText(hTitle, pszTitle, MAX_PATH);
		hTitle = GetParent(hTitle);
	} while( !lstrlen(pszTitle) && NULL != hTitle );

	int ret = IDOK, fDoDefault = ( !fIsWin6x || (NULL == pTaskDialog) );

	if ( !fDoDefault )
	{
		DWORD dwButtons;
		LPTSTR dwIcon;
		switch(dwMsgBoxFlags & MB_TYPEMASK)
		{
		case MB_OK: dwButtons = TDCBF_OK_BUTTON; break;
		case MB_OKCANCEL: dwButtons = TDCBF_OK_BUTTON | TDCBF_CANCEL_BUTTON; break;
		case MB_YESNO:  dwButtons = TDCBF_YES_BUTTON | TDCBF_NO_BUTTON; break;
		case MB_YESNOCANCEL:  dwButtons = TDCBF_YES_BUTTON | TDCBF_NO_BUTTON | TDCBF_CANCEL_BUTTON; break;
		default: fDoDefault = TRUE; break;
		}

		switch(dwMsgBoxFlags & MB_ICONMASK)
		{
		case MB_ICONERROR: dwIcon = TD_ERROR_ICON; break;
		case MB_ICONWARNING: dwIcon = TD_WARNING_ICON; break;
		case MB_ICONQUESTION:
		case MB_ICONINFORMATION: dwIcon = TD_INFORMATION_ICON; break;
		default: dwIcon = NULL; break;
		}

		if ( !fDoDefault && !FAILED(pTaskDialog(m_hWnd, m_hInstance, pszTitle, NULL, lpszText, dwButtons, dwIcon, &ret ) ) ) 
			return ret;
	}
		
	return MessageBox(m_hWnd, lpszText, pszTitle, dwMsgBoxFlags);
}

CString CDialog::OpenFile(LPCTSTR pszDefaultFile, ...)
{
	TCHAR pszFile[MAX_PATH] = { 0 };
	va_list argptr;
	va_start(argptr, pszDefaultFile);
	LPTSTR * szList = (LPTSTR*) argptr;

	DWORD len = 0;
	for(INT i = 0; szList[i]; ++i)
		len += lstrlen(szList[i])+1;

	LPTSTR pszExtList = new TCHAR[len+2];
	ZeroMemory(pszExtList, sizeof(TCHAR)*(len+2));

	len = 0;
	for(INT i = 0; szList[i]; ++i)
	{
		lstrcpy(pszExtList+len, szList[i]);
		len += lstrlen(szList[i])+1;
	}
	va_end(argptr);

	OPENFILENAME ofn = { 0 };

	ofn.lStructSize = sizeof(ofn);
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR | OFN_PATHMUSTEXIST;
	ofn.hInstance = m_hInstance;
	ofn.hwndOwner = GetWindowHandle();
	ofn.lpstrFile = pszFile;
	ofn.lpstrFilter = pszExtList;
	ofn.nMaxFile = MAX_PATH;
	
	lstrcpyn(pszFile, pszDefaultFile, MAX_PATH);
	
	BOOL fResult = GetOpenFileName(&ofn);
	delete pszExtList;

	if ( fResult )
		return CString(pszFile);
	
	return CString(TEXT(""));
}

CString CDialog::SaveFile(LPCTSTR pszDefaultFile, ...)
{
	TCHAR pszFile[MAX_PATH] = { 0 };
	va_list argptr;
	va_start(argptr, pszDefaultFile);
	LPTSTR * szList = (LPTSTR*) argptr;

	DWORD len = 0;
	for(INT i = 0; szList[i]; ++i)
		len += lstrlen(szList[i])+1;

	LPTSTR pszExtList = new TCHAR[len+1];
	ZeroMemory(pszExtList, len+1);

	len = 0;
	for(INT i = 0; szList[i]; ++i)
	{
		lstrcpy(pszExtList+len, szList[i]);
		len += lstrlen(szList[i])+1;
	}
	va_end(argptr);

	OPENFILENAME ofn = { 0 };

	ofn.lStructSize = sizeof(ofn);
	ofn.Flags = OFN_EXPLORER | OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY | OFN_NOCHANGEDIR | OFN_PATHMUSTEXIST;
	ofn.hInstance = m_hInstance;
	ofn.hwndOwner = GetWindowHandle();
	ofn.lpstrFile = pszFile;
	ofn.lpstrFilter = pszExtList;
	ofn.nMaxFile = MAX_PATH;
	
	lstrcpyn(pszFile, pszDefaultFile, MAX_PATH);
	
	BOOL fResult = GetSaveFileName(&ofn);
	delete pszExtList;

	if ( fResult )
		return CString(pszFile);
	
	return CString(TEXT(""));
}

void CDialog::ShowMessage(const TCHAR * lpszText)
{
	MsgBox(lpszText, MB_ICONINFORMATION);
}

BOOL CDialog::Confirm(const TCHAR * lpszText, BOOL bDefaultNo)
{
	TCHAR pszTitle[MAX_PATH+1];
	GetWindowText(m_hWnd, pszTitle, MAX_PATH);
	return ( IDYES == MsgBox(lpszText, MB_ICONQUESTION | MB_YESNO | ( bDefaultNo ? MB_DEFBUTTON2 : MB_DEFBUTTON1 )) );
}

void CDialog::ShowErrorF(const TCHAR * lpszText, ...)
{
	va_list argptr;
	va_start(argptr, lpszText);

	CString str;
	str.FormatV(lpszText, argptr);
	ShowError(str);
	va_end(argptr);
}

void CDialog::ShowMessageF(const TCHAR * lpszText, ...)
{
	va_list argptr;
	va_start(argptr, lpszText);

	CString str;
	str.FormatV(lpszText, argptr);
	ShowMessage(str);
	va_end(argptr);
}

void CDialog::ShowWarning(const TCHAR * lpszText)
{
	MsgBox(lpszText, MB_ICONWARNING);
}

void CDialog::ShowError(const TCHAR * lpszText)
{
	MsgBox(lpszText, MB_ICONERROR);
}

LPTSTR CDialog::GetString(UINT uResource)
{
	::LoadString(GetInstance(), uResource, m_pszMaxPath, MAX_PATH);
	return m_pszMaxPath;
}

CString CDialog::GetStr(UINT uResource)
{
	TCHAR pszText[MAX_PATH] = { 0 };
	::LoadString(GetInstance(), uResource, pszText, MAX_PATH);
	return CString(pszText);
}

CString CDialog::Title()
{
	int len = GetWindowTextLength(m_hWnd) + 1;
	CString str;
	GetWindowText(m_hWnd, str.GetBuffer(len), len);
	str.ReleaseBuffer();
	return CString(str);
}

void CDialog::Title(CString sTitle)
{
	SetWindowText(m_hWnd, sTitle);
}

VOID CDialog::AddAnchor(UINT uId, UINT uAnchorType)
{
	switch(uAnchorType)
	{
	case ANCHOR_LEFT_TOP: AddCustomAnchor(uId, AnchorHardLinkLeftTop, AnchorHardLinkLeftTop, AnchorHardLinkLeftTop, AnchorHardLinkLeftTop); break;
	case ANCHOR_LEFT_BOTTOM: AddCustomAnchor(uId, AnchorHardLinkLeftTop, AnchorHardLinkRightBottom, AnchorHardLinkLeftTop, AnchorHardLinkRightBottom); break;
	case ANCHOR_RIGHT_TOP: AddCustomAnchor(uId, AnchorHardLinkRightBottom, AnchorHardLinkLeftTop, AnchorHardLinkRightBottom, AnchorHardLinkLeftTop); break;
	case ANCHOR_RIGHT_BOTTOM: AddCustomAnchor(uId, AnchorHardLinkRightBottom, AnchorHardLinkRightBottom, AnchorHardLinkRightBottom, AnchorHardLinkRightBottom); break;
	case ANCHOR_LEFT: AddCustomAnchor(uId, AnchorHardLinkLeftTop, AnchorHardLinkLeftTop, AnchorHardLinkLeftTop, AnchorHardLinkRightBottom); break;
	case ANCHOR_RIGHT: AddCustomAnchor(uId, AnchorHardLinkRightBottom, AnchorHardLinkLeftTop, AnchorHardLinkRightBottom, AnchorHardLinkRightBottom); break;
	case ANCHOR_TOP: AddCustomAnchor(uId, AnchorHardLinkLeftTop, AnchorHardLinkLeftTop, AnchorHardLinkRightBottom, AnchorHardLinkLeftTop); break;
	case ANCHOR_BOTTOM: AddCustomAnchor(uId, AnchorHardLinkLeftTop, AnchorHardLinkRightBottom, AnchorHardLinkRightBottom, AnchorHardLinkRightBottom); break;
	case ANCHOR_FULL: AddCustomAnchor(uId, AnchorHardLinkLeftTop, AnchorHardLinkLeftTop, AnchorHardLinkRightBottom, AnchorHardLinkRightBottom); break;
	}
}

VOID CDialog::AddCustomAnchor(UINT uId, AnchorType left, AnchorType top, AnchorType right, AnchorType bottom)
{
	CSizeAnchor * anchor = new CSizeAnchor(uId, left, top, right, bottom);
	m_vAncList.push_back(anchor);
}

VOID CDialog::InitAnchors()
{
	for(vector<CSizeAnchor*>::iterator v=m_vAncList.begin(); v < m_vAncList.end(); v++)
		(*v)->Initialize(m_hWnd);
}

VOID CDialog::UpdateAnchor(USHORT uId)
{
	for(vector<CSizeAnchor*>::iterator v=m_vAncList.begin(); v < m_vAncList.end(); v++)
		if ( uId == (*v)->ControlId() )
			(*v)->Initialize(m_hWnd);
}

VOID CDialog::InternalOnSize(UINT fwSizeType, SHORT cx, SHORT cy)
{
	
	HDWP hdwp = BeginDeferWindowPos(m_vAncList.size());
	vector<CSizeAnchor*>::iterator v;

	for(v=m_vAncList.begin(); v < m_vAncList.end(); v++)
		(*v)->ResizeByDefer(hdwp);
	EndDeferWindowPos(hdwp);

	OnSize(fwSizeType, cx, cy);
}

VOID CDialog::InternalOnSizing(UINT fwSizeType, LPRECT lprc)
{
	HDWP hdwp = BeginDeferWindowPos(m_vAncList.size());
	vector<CSizeAnchor*>::iterator v;

	for(v=m_vAncList.begin(); v < m_vAncList.end(); v++)
		(*v)->ResizeByDefer(hdwp);
	EndDeferWindowPos(hdwp);
	
	OnSizing(fwSizeType, lprc);

//	CDialog::InternalOnSize(fwSizeType, (SHORT) (0xFFFF & lprc->right) , (SHORT) (0xFFFF & lprc->bottom) );	
}

void CDialog::OnPreInit(CDialogTemplate & t)
{

}

void CDialog::OnInit(LPARAM lParam)
{
	return;
}

void CDialog::InternalOnCommand(USHORT uCmd, USHORT uId, HWND hWnd)
{
	if ( BN_CLICKED != uCmd )
		OnCommand(uCmd, uId, hWnd);
	else
	{
		switch(uId)
		{
		case IDOK: OnOK(); break;
		case IDCANCEL: OnCancel(); break;
		default: OnButton(uId); break;
		}
	}
}

void CDialog::OnCommand(USHORT uCmd, USHORT uId, HWND hWnd)
{
	return;
}

void CDialog::OnButton(USHORT uId)
{
	return;
}

void CDialog::OnOK()
{
	return;
}

void CDialog::OnCancel()
{
	Close(0);

	return;
}

void CDialog::OnSize(UINT fwSizeType, SHORT cx, SHORT cy)
{
	return;
}

void CDialog::OnSizing(UINT fwSizeType, LPRECT lprc)
{
	return;
}

INT_PTR CDialog::InternalOnNotify(LPNMHDR lpnm)
{
	switch(lpnm->code)
	{
	case PSN_APPLY:
	case PSN_HELP:
	case PSN_KILLACTIVE:
	case PSN_QUERYCANCEL:
	case PSN_QUERYINITIALFOCUS:
	case PSN_RESET:
	case PSN_SETACTIVE:
		return InternalOnPSNotify(lpnm->code, (LPPSHNOTIFY) lpnm);
	}

	return OnNotify(lpnm);
}

INT_PTR CDialog::InternalOnPSNotify(UINT code, LPPSHNOTIFY lpnm)
{
	switch(code)
	{
	case PSN_APPLY: return PSOnApply();
	case PSN_HELP: PSOnHelp(); return PSNRET_NOERROR;
	case PSN_KILLACTIVE: return PSOnKillActive();
	case PSN_QUERYCANCEL: return PSOnQueryCancel();
	case PSN_RESET: PSOnRest(); return PSNRET_NOERROR;
	case PSN_SETACTIVE: return PSOnSetActive();
	default: return PSNRET_NOERROR;
	}
}

INT_PTR CDialog::OnNotify(LPNMHDR lpnm)
{
	return 0;
}

INT CDialog::PSOnApply()
{
	return PSNRET_NOERROR;
}

void CDialog::PSOnHelp()
{
	return;
}

BOOL CDialog::PSOnKillActive()
{
	return FALSE;
}

BOOL CDialog::PSOnQueryCancel()
{
	return FALSE;
}

void CDialog::PSOnRest()
{
	return;
}

INT CDialog::PSOnSetActive()
{
	return 0;
}

INT CDialog::OnMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & fHandle)
{
	fHandle = FALSE;
	return 0;
}

INT CDialog::DoLoop(HWND hWndAccel, HACCEL hAccel)
{
	if ( !m_hWnd ) 
		return -1;

	MSG msg = { 0 };
	INT res = 0;
	do
	{
		res = GetMessage(&msg, NULL, 0, 0);

		if ( !res || -1 == res )
			break;

		BOOL fAccel = ( hAccel ? TranslateAccelerator(hWndAccel ? hWndAccel : *this, hAccel, &msg) : FALSE );

		if ( !fAccel && !IsDialogMessage(*this, &msg) )
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if ( 0 == msg.message || WM_CLOSE == msg.message || WM_DESTROY == msg.message || !m_hWnd )
		{
			if ( !m_hWnd || ( m_hWnd == msg.hwnd ) )
				break;
		}

	} while(-1 != res && 0 != res);

	return res;
}

BOOL CDialog::OnClose(INT & iRetCode)
{
	iRetCode = 0;
	return ( !m_pPropSheet );
}

void CDialog::OnPaint( HDC hDC , LPPAINTSTRUCT lpps )
{
	return;
}

void CDialog::OnThemeChanged()
{
	return;
}

void CDialog::OnSysColorChanged()
{
	return;
}


void CDialog::InternalOnDestroy()
{
	if ( !m_fDenyCalls )
		OnDestroy();
	else
	{
		if ( m_hWnd )
		{
			TCHAR pszClass[MAX_PATH] = { 0 };
			GetClassName(m_hWnd, pszClass, MAX_PATH);
			TCHAR psz[100] = { 0 };;
			SendMessage(m_hWnd, WM_GETTEXT, 100, (LPARAM) psz);

			_tprintf(TEXT("WARNING: handler '%s::\"%s\"::OnDestroy() was not called!\n"), pszClass, psz);
		}
	}
}

void CDialog::OnDestroy()
{
	return;
}

HBRUSH CDialog::InternalOnColorDialog(HDC hDC, HWND hWnd, UINT wmCtl)
{
	CTLBRUSH ctlb = ctlbNone;
	switch(wmCtl)
	{
	case WM_CTLCOLORDLG: ctlb = ctlbDlg; break;
	case WM_CTLCOLORBTN: ctlb = ctlbBtn; break;
	case WM_CTLCOLOREDIT: ctlb = ctlbEdit; break;
	case WM_CTLCOLORLISTBOX: ctlb = ctlbList; break;
	case WM_CTLCOLORSCROLLBAR: ctlb = ctlbScroll; break;
	case WM_CTLCOLORSTATIC: ctlb = ctlbStatic; break;
	case WM_CTLCOLORMSGBOX: ctlb = ctlbMbox; break;
	default: return (HBRUSH) -1;
	}

	map<USHORT, HBRUSH>::iterator br = m_mBrushList.find(0);
	if ( hWnd == GetWindowHandle() && br != m_mBrushList.end() )
	{
		return br->second;
	}
	else
	{
		USHORT uCtl = (USHORT) (0xFFFF & GetWindowLong(hWnd, GWL_ID));
		br = m_mBrushList.find(uCtl);
		if ( 0 != uCtl && br != m_mBrushList.end() )
			return br->second;
	}

	HBRUSH hBrush = GetBgBrush(ctlb);
	if ( (HBRUSH) -1 == hBrush )
		return OnColorDialog(hDC, hWnd, ctlb);
	else
		return hBrush;
}

HBRUSH CDialog::OnColorDialog(HDC hDC, HWND hWnd, CTLBRUSH ctlb)
{
	return (HBRUSH) -1;
}

VOID CDialog::Visible(BOOL fVisible)
{
	ShowWindow(m_hWnd, ( fVisible ? SW_SHOW : SW_HIDE ) );
}

BOOL CDialog::Visible()
{
	return IsWindowVisible(m_hWnd);
}

VOID CDialog::Visible(USHORT uId, BOOL fVisible)
{
	ShowWindow(Ctl(uId), ( fVisible ? SW_SHOW : SW_HIDE ) );
}

BOOL CDialog::Visible(USHORT uId)
{
	return IsWindowVisible(Ctl(uId));
}

CDialog::operator HWND()
{
	return m_hWnd;
}

BOOL CDialog::IsWindowDestroyed()
{
	return m_fWindowDestroyed;
}

HWND CDialog::GetCustomMenu()
{
	return NULL;
}

CDialogTemplate & CDialog::Dialog()
{
	return m_dlgTemplate;
}

BOOL CDialog::GetRect(USHORT uId, LPRECT lprc, BOOL fPos)
{
	BOOL fResult = GetWindowRect(Ctl(uId), lprc);

	if ( !fPos )
	{
		lprc->right -= lprc->left;
		lprc->bottom -= lprc->top;
		lprc->left = lprc->top = 0;
	}
	else
	{
		ScreenToClient(*this, LPPOINT(lprc));
		ScreenToClient(*this, LPPOINT(lprc) + 1);
	}

	return fResult;
}

CString CDialog::MB_GetString(UINT wButton)
{
	LPCWSTR (WINAPI* mbgs)(UINT);

	*((PVOID*) &mbgs) = GetProcAddress(GetModuleHandle(TEXT("user32")), "MB_GetString");

	if ( mbgs )
		return mbgs(wButton);
	else
		return TEXT("");
}

// --- dialog template creator

CDialogTemplate::CDialogTemplate()
{
	ZeroMemory(&m_dlgTpl, sizeof(m_dlgTpl));

	m_dlgTpl.dlgVer = 1;
	m_dlgTpl.signature = 0xFFFF;
	m_dlgTpl.helpID = 0;
	m_dlgTpl.exStyle = 0;
	m_dlgTpl.style = DS_FIXEDSYS | DS_SHELLFONT | WS_POPUP | WS_CAPTION | WS_SYSMENU;
	m_dlgTpl.cDlgItems = 0;
	m_dlgTpl.x = 0;
	m_dlgTpl.y = 0;
	m_dlgTpl.cx = 200;
	m_dlgTpl.cy = 150;
	m_dlgTpl.menuId.uId = 0;
	m_dlgTpl.dlgClass.atomId = 0;
	m_dlgTpl.dlgClass.pszClassName[0] = 0;
	m_dlgTpl.pszTitle[0] = 0;
	m_dlgTpl.pointsize = 8;
	m_dlgTpl.weight = 0;
	m_dlgTpl.italic = 0;
	m_dlgTpl.charset = DEFAULT_CHARSET;
	
	lstrcpyW(m_dlgTpl.typeface, SHELL_FONT);

	m_pTemplate = NULL;
	m_fTemplateChanged = TRUE;
}

CDialogTemplate::~CDialogTemplate()
{
	while(m_vdlgItems.size() > 0)
	{
		DLG_ITEM & di = *m_vdlgItems.begin();

		if ( di.pvExtraData )
			delete[] di.pvExtraData;

		di.pvExtraData = NULL;
		m_vdlgItems.erase(m_vdlgItems.begin());
	}

	if ( m_pTemplate )
		delete[] m_pTemplate;
}

int CDialogTemplate::StrCopy(LPWSTR pszDest, LPCTSTR pszSrc, int len)
{
	if ( !len )
		len = lstrlen(pszSrc);

#ifdef UNICODE
	return lstrlen(lstrcpyn(pszDest, pszSrc, len));
#else
	return MultiByteToWideChar(CP_ACP, 0, pszSrc, lstrlen(pszSrc), pszDest, len); 
#endif

	return 0;
}


void CDialogTemplate::SetDlgTitle(LPCTSTR pszTitle)
{
	StrCopy(m_dlgTpl.pszTitle, pszTitle, MAX_FIELD_TEXT);
	m_fTemplateChanged = TRUE;
}

void CDialogTemplate::SetDlgStyle(DWORD style, DWORD styleEx)
{
	m_dlgTpl.style = style;
	m_dlgTpl.exStyle = styleEx;
	m_fTemplateChanged = TRUE;
}

void CDialogTemplate::AddStyle(DWORD dwStyle)
{
	m_dlgTpl.style |= dwStyle;
	m_fTemplateChanged = TRUE;
}

void CDialogTemplate::AddExStyle(DWORD dwStyle)
{
	m_dlgTpl.exStyle |= dwStyle;
	m_fTemplateChanged = TRUE;
}


void CDialogTemplate::SetDlgPos(int x, int y, int cx, int cy)
{
	m_dlgTpl.x = LOWORD(x);
	m_dlgTpl.y = LOWORD(y);
	m_dlgTpl.cx = LOWORD(cx);
	m_dlgTpl.cy = LOWORD(cy);
	m_fTemplateChanged = TRUE;
}

void CDialogTemplate::SetDlgFont(LPCTSTR pszFont, WORD size, WORD weight, BOOL fItalic, BYTE bCharset)
{
	StrCopy(m_dlgTpl.typeface, pszFont, MAX_FIELD_TEXT);
	m_dlgTpl.pointsize = size;
	m_dlgTpl.weight = weight;
	m_dlgTpl.italic = ( fItalic ? TRUE : FALSE );
	m_dlgTpl.charset = bCharset;
	m_dlgTpl.style |= DS_SETFONT;
	m_fTemplateChanged = TRUE;
}

void CDialogTemplate::SetDlgExtra(DWORD helpId)
{
	m_dlgTpl.helpID = helpId;
	m_fTemplateChanged = TRUE;
}

void CDialogTemplate::SetDlgClass(LPCTSTR pszClass)
{
	StrCopy(m_dlgTpl.dlgClass.pszClassName, pszClass, MAX_FIELD_TEXT);
	m_fTemplateChanged = TRUE;
}

void CDialogTemplate::SetDlgClass(WORD atomId)
{
	m_dlgTpl.dlgClass.atomId = atomId;
	m_fTemplateChanged = TRUE;
}

int CDialogTemplate::AddItem(USHORT uId, DLG_ITEM_PRECLASS cls, LPCTSTR pszCaption,
								int x, int y, int cx, int cy,
								DWORD style, DWORD exStyle, DWORD helpId,
								PVOID pvExtraData, int extraLen)
{
	if ( extraLen > 0 && IsBadReadPtr(pvExtraData, extraLen) )
		return -1;

	if ( cls == cCustom )
		return -1;

	DLG_ITEM di = { 0 };

	if ( x < 0 )
		x = int(m_dlgTpl.cx) - (-x);

	if ( y < 0 )
		y = int(m_dlgTpl.cy) - (-y);

	if ( cx < 0 )
		cx = int(m_dlgTpl.cx) - x - (-cx);
	
	if ( cy < 0 )
		cy = int(m_dlgTpl.cy) - y - (-cy);

    di.helpID = helpId;
    di.exStyle = exStyle;
    di.style = style | WS_CHILD;
    di.x = LOWORD(x);
    di.y = LOWORD(y);
    di.cx = LOWORD(cx);
    di.cy = LOWORD(cy);
    di.id = uId;
	di.cls.cPreClass = cls;
   
	if ( pszCaption )
		StrCopy(di.title, pszCaption, MAX_FIELD_TEXT);
    
	if ( extraLen > 0 )
	{
		di.extraCount = LOWORD(extraLen);
		di.pvExtraData = new BYTE[di.extraCount];
		CopyMemory(di.pvExtraData, pvExtraData, USHORT(di.extraCount));
	}

	m_vdlgItems.push_back(di);
	m_fTemplateChanged = TRUE;
	return (m_vdlgItems.size() - 1);
}


int CDialogTemplate::AddItem(USHORT uId, LPCTSTR pszClass, LPCTSTR pszCaption,
								int x, int y, int cx, int cy,
								DWORD style, DWORD exStyle, DWORD helpId,
								PVOID pvExtraData, int extraLen)
{
	if ( extraLen > 0 && IsBadReadPtr(pvExtraData, extraLen) )
		return -1;

	if ( !pszClass )
		return -1;

	DLG_ITEM di = { 0 };

	if ( x < 0 )
		x = int(m_dlgTpl.cx) - (-x);

	if ( y < 0 )
		y = int(m_dlgTpl.cy) - (-y);

	if ( cx < 0 )
		cx = int(m_dlgTpl.cx) - x - (-cx);
	
	if ( cy < 0 )
		cy = int(m_dlgTpl.cy) - y - (-cy);

    di.helpID = helpId;
    di.exStyle = exStyle;
    di.style = style | WS_CHILD;
    di.x = LOWORD(x);
    di.y = LOWORD(y);
    di.cx = LOWORD(cx);
    di.cy = LOWORD(cy);
    di.id = uId;
	StrCopy(di.cls.windowClass, pszClass, MAX_FIELD_TEXT);
   
	if ( pszCaption )
		StrCopy(di.title, pszCaption, MAX_FIELD_TEXT);
    
	if ( extraLen > 0 )
	{
		di.extraCount = LOWORD(extraLen);
		di.pvExtraData = new BYTE[di.extraCount];
		CopyMemory(di.pvExtraData, pvExtraData, USHORT(di.extraCount));
	}

	m_vdlgItems.push_back(di);
	m_fTemplateChanged = TRUE;
	return (m_vdlgItems.size() - 1);
}

	
int CDialogTemplate::AddPreItem(USHORT uId, DLG_ITEM_PRECLASS cls, LPCTSTR pszCaption,
								int x, int y, int cx, int cy, DWORD style,
								BOOL fVisible, BOOL fEnable, BOOL fTabStop)
{
	style |= 
		( fVisible ? WS_VISIBLE : 0 ) |
		( fEnable ? 0 : WS_DISABLED ) |
		( fTabStop ? WS_TABSTOP : 0 );

	return AddItem(uId, cls, pszCaption, x, y, cx, cy, style, 0, NULL, 0);
	
}


int CDialogTemplate::AddPreItem(USHORT uId, LPCTSTR pszClass, LPCTSTR pszCaption,
								int x, int y, int cx, int cy, DWORD style,
								BOOL fVisible, BOOL fEnable, BOOL fTabStop)
{
	style |= 
		( fVisible ? WS_VISIBLE : 0 ) |
		( fEnable ? 0 : WS_DISABLED ) |
		( fTabStop ? WS_TABSTOP : 0 );

	return AddItem(uId, pszClass, pszCaption, x, y, cx, cy, style, 0, NULL, 0);
}

LPCDLGTEMPLATE CDialogTemplate::Template()
{
	if ( m_pTemplate && !m_fTemplateChanged )
		return (LPCDLGTEMPLATE) m_pTemplate;

	
	if ( m_pTemplate )
		delete[] m_pTemplate;

	m_pTemplate = NULL;


	INT size = 0;
	WORD zero = 0;
	m_dlgTpl.cDlgItems = m_vdlgItems.size();

	size =
		sizeof(m_dlgTpl.dlgVer) +
		sizeof(m_dlgTpl.signature) +
		sizeof(m_dlgTpl.helpID) +
		sizeof(m_dlgTpl.exStyle) +
		sizeof(m_dlgTpl.style) +
		sizeof(m_dlgTpl.cDlgItems) +
		sizeof(m_dlgTpl.x) +
		sizeof(m_dlgTpl.y) +
		sizeof(m_dlgTpl.cx) +
		sizeof(m_dlgTpl.cy) +
		( m_dlgTpl.menuId.pszMenu[0] ? sizeof(WCHAR) * (1 + lstrlenW(m_dlgTpl.menuId.pszMenu)) : sizeof(WORD) ) +
		( m_dlgTpl.dlgClass.pszClassName[0] ? sizeof(WCHAR) * (1 + lstrlenW(m_dlgTpl.dlgClass.pszClassName)) : sizeof(WORD) ) +
		sizeof(WCHAR) * (1 + lstrlenW(m_dlgTpl.pszTitle)) +
		sizeof(m_dlgTpl.pointsize) +
		sizeof(m_dlgTpl.weight) +
		sizeof(m_dlgTpl.italic) +
		sizeof(m_dlgTpl.charset) +
		( ( m_dlgTpl.style & DS_SETFONT ) == DS_SETFONT ? sizeof(WCHAR) * (1 + lstrlenW(m_dlgTpl.typeface)) : sizeof(WCHAR) ) +
		sizeof(DWORD);
		
	for(std::vector<DLG_ITEM>::iterator v = m_vdlgItems.begin(); v < m_vdlgItems.end(); ++v)
		size += GetDlgItemTemplate(*v, 0) + sizeof(DWORD);

	m_pTemplate = new BYTE[size + 1];
	ZeroMemory(m_pTemplate, size + 1);
	PBYTE pBuffer = m_pTemplate;

#define ADDATA(x)		pBuffer = AddData(&(x), sizeof(x), pBuffer)
	ADDATA(m_dlgTpl.dlgVer);
	ADDATA(m_dlgTpl.signature);
	ADDATA(m_dlgTpl.helpID);
	ADDATA(m_dlgTpl.exStyle);
	ADDATA(m_dlgTpl.style);
	ADDATA(m_dlgTpl.cDlgItems);
	ADDATA(m_dlgTpl.x);
	ADDATA(m_dlgTpl.y);
	ADDATA(m_dlgTpl.cx);
	ADDATA(m_dlgTpl.cy);

	if ( m_dlgTpl.menuId.pszMenu[0] )
		pBuffer = AddDataAligned(m_dlgTpl.menuId.pszMenu, sizeof(WCHAR) * (1 + lstrlenW(m_dlgTpl.menuId.pszMenu)), pBuffer);
	else
		ADDATA(m_dlgTpl.menuId.uId);

	if ( m_dlgTpl.dlgClass.pszClassName[0] )
		pBuffer = AddDataAligned(m_dlgTpl.dlgClass.pszClassName, sizeof(WCHAR) * (1 + lstrlenW(m_dlgTpl.dlgClass.pszClassName)), pBuffer);
	else
		ADDATA(m_dlgTpl.dlgClass.atomId);

	pBuffer = AddDataAligned(m_dlgTpl.pszTitle, sizeof(WCHAR) * (1 + lstrlenW(m_dlgTpl.pszTitle)), pBuffer);

	ADDATA(m_dlgTpl.pointsize);
	ADDATA(m_dlgTpl.weight);
	ADDATA(m_dlgTpl.italic);
	ADDATA(m_dlgTpl.charset);
	
	if ( ( m_dlgTpl.style & DS_SETFONT ) == DS_SETFONT )
		pBuffer = AddDataAligned(m_dlgTpl.typeface, sizeof(WCHAR) * (1 + lstrlenW(m_dlgTpl.typeface)), pBuffer);
	else
		ADDATA(zero);

#undef ADDATA

	for(std::vector<DLG_ITEM>::iterator vItem = m_vdlgItems.begin(); vItem < m_vdlgItems.end(); ++vItem)
	{
		INT_PTR p = 4 - ( INT_PTR(pBuffer) % sizeof(DWORD) );	// align to dword
		if ( p < 4 )
			pBuffer += p;

		pBuffer += GetDlgItemTemplate(*vItem, pBuffer);
	}


	m_fTemplateChanged = FALSE;
	return LPCDLGTEMPLATE(m_pTemplate);
}

PBYTE CDialogTemplate::AddData(PVOID pData, int dataLen, PBYTE pBuffer)
{
	CopyMemory(pBuffer, pData, dataLen);
	return ( pBuffer + dataLen );
}

PBYTE CDialogTemplate::AddDataAligned(PVOID pData, int dataLen, PBYTE pBuffer)
{
	pBuffer += ( INT_PTR(pBuffer) % sizeof(WORD) );
	CopyMemory(pBuffer, pData, dataLen);
	return ( pBuffer + dataLen );
}

PBYTE CDialogTemplate::AddDataAligned4(PVOID pData, int dataLen, PBYTE pBuffer)
{
	INT_PTR p = ( INT_PTR(pBuffer) % sizeof(DWORD) );
	
	if ( p )
		pBuffer += ( 4 - p );

	CopyMemory(pBuffer, pData, dataLen);
	return ( pBuffer + dataLen );
}


INT CDialogTemplate::GetDlgItemTemplate(DLG_ITEM & di, PBYTE pBuffer)
{
	INT size = 
		sizeof(DWORD) +			// helpID
		sizeof(DWORD) +			// exStyle
		sizeof(DWORD) +			// style
		sizeof(short) +			// x
		sizeof(short) +			// y
		sizeof(short) +			// cx
		sizeof(short) +			// cy
		sizeof(DWORD) +			// id
		( di.cls.cPreClass == cCustom ? sizeof(WCHAR) * ( 1 + lstrlenW(di.cls.windowClass) ): sizeof(DWORD) ) +  // class
		sizeof(WCHAR) * (1 + lstrlenW(di.title) ) +		// title
		sizeof(WORD) +			// extraCount
		di.extraCount + 
		sizeof(WORD) + sizeof(DWORD);

	if ( di.cls.cPreClass == cCustom && !di.cls.windowClass[0] )
		return 0;

	if ( !pBuffer ) 
		return size;


#define ADDATA(x)		pBuffer = AddData(&(x), sizeof(x), pBuffer)

	PBYTE pStart = pBuffer;
	WORD zero = 0, neg = -1;
	WORD cls = WORD(di.cls.cPreClass);
	ADDATA(di.helpID);
	ADDATA(di.exStyle);
	ADDATA(di.style);
	ADDATA(di.x); 
	ADDATA(di.y);
	ADDATA(di.cx);
	ADDATA(di.cy);
	ADDATA(di.id);

	if ( di.cls.cPreClass != cCustom )
	{
		ADDATA(neg);
		ADDATA(cls);
	}
	else
		pBuffer = AddDataAligned(di.cls.windowClass, sizeof(WCHAR) * (lstrlenW(di.cls.windowClass) + 1), pBuffer);

	if ( di.title[0] )
		pBuffer = AddDataAligned(di.title, sizeof(WCHAR) * (lstrlenW(di.title) + 1), pBuffer);
	else
		ADDATA(zero);

	ADDATA(di.extraCount);
	
	if ( di.extraCount > 0 )
		pBuffer = AddDataAligned4(di.pvExtraData, di.extraCount, pBuffer);

#undef ADDATA

	return ( pBuffer - pStart );
}