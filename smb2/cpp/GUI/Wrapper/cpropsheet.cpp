
#include "precomp.h"

CPropSheet::CPropSheet(HINSTANCE hInstance)
	: m_hInstance( hInstance ), m_fCreated( FALSE ), m_dwFlags( 0 ), m_hWnd( NULL ), m_fSetToClose( FALSE )
{

}

CPropSheet::~CPropSheet()
{
	m_fCreated = FALSE;
}

void CPropSheet::SetFlag(DWORD dwFlag, BOOL fSet)
{
	if ( fSet )
		m_dwFlags |= dwFlag;
	else
		m_dwFlags &= ~dwFlag;
}

VOID CPropSheet::FApply(BOOL fApply)
{
	SetFlag(PSH_NOAPPLYNOW, !fApply);
}

VOID CPropSheet::FAppendTitle(BOOL fAppendTitle)
{
	SetFlag(PSH_PROPTITLE, fAppendTitle);
}

BOOL CPropSheet::AddSheet(CDialog * pDialog, LPARAM lParam, LPTSTR pszTitle, BOOL fForceCreate)
{
	if ( m_fCreated || !pDialog ) 
		return FALSE;

	PROPSHEETPAGE psPage = { 0 };

	psPage.dwSize = sizeof(PROPSHEETPAGE);
	psPage.dwFlags = ( fForceCreate ? PSP_PREMATURE : 0 ) | ( pszTitle ? PSP_USETITLE : 0 );
	psPage.pszTitle = pszTitle;
	psPage.hInstance = pDialog->GetInstance();
	psPage.pszTemplate = pDialog->m_lpszResource;
	if ( !psPage.pszTemplate )
	{
		psPage.dwFlags |= PSP_DLGINDIRECT;
		pDialog->OnPreInit(pDialog->Dialog());
		psPage.pResource = pDialog->Dialog().Template();
	}

	psPage.pfnDlgProc = CDialog::PropSheetDialogProc;
	psPage.lParam = (LPARAM) pDialog;
	pDialog->m_lUserParam = lParam;
	pDialog->m_pPropSheet = this;

	m_vPSList.push_back(psPage);
	return TRUE;
}

INT_PTR CPropSheet::Show(CDialog * pDialog, LPARAM lParam, LPTSTR pszCaption, DWORD dwFirstPage)
{
	if ( !m_vPSList.size() )
		return 0;
	
	for(vector<PROPSHEETPAGE>::iterator v = m_vPSList.begin(); v < m_vPSList.end(); v++)
	{
		CDialog * pPageDialog = (CDialog*) v->lParam;
		pPageDialog->m_pParent = pDialog;
	}

	PROPSHEETHEADER psHead = { 0 };
	psHead.dwSize = sizeof(psHead);
	psHead.dwFlags =  m_dwFlags | PSH_PROPSHEETPAGE;
	psHead.hwndParent = ( pDialog ? pDialog->GetWindowHandle() : NULL );
	psHead.hInstance = m_hInstance;
	psHead.pszCaption = pszCaption;
	psHead.nStartPage = dwFirstPage;
	psHead.ppsp = &m_vPSList[0];
	psHead.nPages = m_vPSList.size();

	m_fCreated = TRUE;

	INT_PTR ret = PropertySheet(&psHead);

	m_hWnd = NULL;

	return ret;
}

INT_PTR CPropSheet::pSendMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// DON'T USE CLASS VARIABLES BEFORE CHECK 'this' POINTER!!!
	if ( !this )
		return 0;

	if ( !m_fCreated || !m_hWnd )
		return 0;

	return SendMessage(m_hWnd, uMsg, wParam, lParam);
}

HWND CPropSheet::GetWindowHandle()
{
	if ( !this ) 
		return NULL;

	return m_hWnd;
}

CDialog * CPropSheet::WindowToDialog(HWND hWnd)
{
	return (CDialog*) GetWindowLong(hWnd, DWL_USER);
}

HWND CPropSheet::DialogToWindow(CDialog * pDlg)
{
	return ( pDlg ? pDlg->GetWindowHandle() : NULL );
}

// DON'T USE CLASS VARIABLES IN THIS METHODS!!!

VOID CPropSheet::EChanged(BOOL fChanged, CDialog * pCallee)
{
	if ( !pCallee ) 
		return;
	fChanged ? pSendMessage(PSM_CHANGED, (WPARAM) pCallee->GetWindowHandle()) : pSendMessage(PSM_UNCHANGED, (WPARAM) pCallee->GetWindowHandle());
}

VOID CPropSheet::ESetClose()
{
	pSendMessage(PSM_CANCELTOCLOSE);

	if ( this )		// check for valid method call
		this->m_fSetToClose = TRUE;
}

BOOL CPropSheet::EGetClose()
{
	if ( this )		// check for valid method call
		return this->m_fSetToClose;

	return FALSE;
}

HWND CPropSheet::ETabCtl()
{
	return (HWND) pSendMessage(PSM_GETTABCONTROL);
}

VOID CPropSheet::EDoCommand(UINT uButton)
{
	pSendMessage(PSM_PRESSBUTTON, uButton);
}

VOID CPropSheet::ENeedReboot()
{
	pSendMessage(PSM_REBOOTSYSTEM);
}

VOID CPropSheet::ENeedRestart()
{
	pSendMessage(PSM_RESTARTWINDOWS);
}

VOID CPropSheet::ETitle(LPTSTR lpszTitle, BOOL fPropertiesForFlag)
{
	pSendMessage(PSM_SETTITLE, ( fPropertiesForFlag ? PSH_PROPTITLE : 0 ), (LPARAM) lpszTitle);
}

CDialog * CPropSheet::EGetPage(INT iIndex)
{
	return WindowToDialog((HWND) pSendMessage(PSM_INDEXTOHWND, iIndex));
}

INT CPropSheet::EGetIndex(CDialog * pDialog)
{
	if ( !pDialog ) 
		return -1;

	return (INT) pSendMessage(PSM_HWNDTOINDEX, (WPARAM) pDialog->GetWindowHandle());
}

CDialog * CPropSheet::EGetCurrentPage()
{
	return WindowToDialog((HWND) pSendMessage(PSM_GETCURRENTPAGEHWND));
}

INT CPropSheet::EGetCurrentIndex()
{
	CDialog * pDlg = EGetCurrentPage();
	if ( pDlg )
		return EGetIndex(pDlg);

	return -1;
}

VOID CPropSheet::ESetCurrentPage(INT iIndex)
{
	if ( -1 == iIndex )
		return;

	pSendMessage(PSM_SETCURSEL, iIndex);
}

VOID CPropSheet::ESetCurrentPage(CDialog * pDialog)
{
	ESetCurrentPage(EGetIndex(pDialog));
}
