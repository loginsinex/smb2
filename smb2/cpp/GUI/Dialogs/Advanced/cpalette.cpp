
#include "precomp.h"

CPaletteDlg::CPaletteDlg(HINSTANCE hInstance, CFileLoader & fl, BOOL fAdditional) : CDialog(hInstance, MAKEINTRESOURCE(IDD_ADV_PLT)), m_File( fl ), m_fAdditional( fAdditional )
{
	ZeroMemory(&m_Buffer, sizeof(m_Buffer));
	MakeBuffer();
}

CPaletteDlg::~CPaletteDlg()
{
	ReleaseBuffer();
}

VOID CPaletteDlg::MakeBuffer()
{
	if ( m_Buffer.hIml )
		return;

	m_Buffer.hIml = ImageList_Create(256*24, 15, ILC_COLOR24, 256, 1);
	if ( !m_Buffer.hIml )
		return;

	ImageList_SetIconSize(m_Buffer.hIml, 24, 15);

	m_Buffer.hDC = GetDC(GetDesktopWindow());
	if ( !m_Buffer.hDC ) return;
	
	m_Buffer.hDCBuffer = CreateCompatibleDC(m_Buffer.hDC);
	if ( m_Buffer.hDCBuffer )
	{
		m_Buffer.hBmpBuffer = CreateCompatibleBitmap(m_Buffer.hDC, 256*24, 15);
		if ( m_Buffer.hBmpBuffer )
		{
			m_Buffer.hOldBmp = (HBITMAP) SelectObject(m_Buffer.hDCBuffer, m_Buffer.hBmpBuffer);

			// make colors list
			HBRUSH hBrush[64] = { NULL };
			for(int i = 0; i < 64; ++i)
				hBrush[i] = CreateSolidBrush(g_crNesColor[i]);

			for(int i = 0; i < 256; ++i)
			{
				RECT rc = { 24*i, 0, 24*(i+1), 15 };
				FillRect(m_Buffer.hDCBuffer, &rc, hBrush[i%64]);
			}

			ImageList_AddMasked(m_Buffer.hIml, m_Buffer.hBmpBuffer, -1);

			for(int i = 0; i < 64; ++i)
				DeleteObject(hBrush[i]);

			SelectObject(m_Buffer.hDCBuffer, m_Buffer.hOldBmp);
			DeleteObject(m_Buffer.hBmpBuffer);
		}
		DeleteDC(m_Buffer.hDCBuffer);
	}

	ReleaseDC(GetDesktopWindow(), m_Buffer.hDC);

	m_Buffer.hDC = m_Buffer.hDCBuffer = NULL;
	m_Buffer.hBmpBuffer = m_Buffer.hOldBmp = NULL;
}

VOID CPaletteDlg::ReleaseBuffer()
{
	if ( !m_Buffer.hIml )
		return;

	ImageList_Destroy(m_Buffer.hIml);
}

HIMAGELIST CPaletteDlg::GetImage()
{
	return m_Buffer.hIml;
}

INT CPaletteDlg::PSOnApply()
{
	SaveItems();
	return PSNRET_NOERROR;
}

VOID CPaletteDlg::OnInit(LPARAM lParam)
{
	m_cbWorld.AssignWindow(Ctl(IDC_WORLD));
	m_tbPal.AssignWindow(Ctl(IDC_SLIDER1));

	m_cbPal[0] = IDC_TILE0;
	m_cbPal[1] = IDC_TILE1;
	m_cbPal[2] = IDC_TILE2;
	m_cbPal[3] = IDC_TILE3;
	m_cbPal[4] = IDC_TILE4;
	m_cbPal[5] = IDC_TILE5;
	m_cbPal[6] = IDC_TILE6;
	m_cbPal[7] = IDC_TILE7;
	m_cbPal[8] = IDC_TILE8;
	m_cbPal[9] = IDC_TILE9;
	m_cbPal[10] = IDC_TILE10;
	m_cbPal[11] = IDC_TILE11;
	m_cbPal[12] = IDC_TILE12;
	m_cbPal[13] = IDC_TILE13;
	m_cbPal[14] = IDC_TILE14;
	m_cbPal[15] = IDC_TILE15;

	m_tbPal.SetMinMax(0, 7);

	for(INT i = 1; i <=7; ++i)
	{
		CString sWorld; sWorld.Format(TEXT("World %d"), i);
		m_cbWorld.AddString(sWorld.GetBuffer(), i);
	}
	m_cbWorld.SetCurSel(0);

	for(INT i = 0; i < sizeof(m_cbPal)/sizeof(m_cbPal[0]); ++i)
	{
		dSendMessage(m_cbPal[i], CBEM_SETIMAGELIST, 0, (LPARAM) GetImage());

		for(INT k = 0; k < 256; ++k)
		{
			COMBOBOXEXITEM cbei = { 0 };
			cbei.mask = CBEIF_LPARAM | CBEIF_IMAGE | CBEIF_SELECTEDIMAGE | CBEIF_INDENT;
			cbei.iIndent = ( i > 3 ? 1 : 0 );
			cbei.lParam = k;
			cbei.iItem = k;
			cbei.iImage = k;
			cbei.iSelectedImage = k;
			dSendMessage(m_cbPal[i], CBEM_INSERTITEM, 0, (LPARAM) &cbei);
		}
	}

	LoadItems();
	SetItems();
}

INT CPaletteDlg::OnMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & fHandle)
{
	if ( WM_HSCROLL == uMsg && GetWindowLong(HWND(lParam), GWL_ID) == IDC_SLIDER1 )
		SetItems();

	return 0;
}

VOID CPaletteDlg::OnCommand(USHORT uCmd, USHORT uId, HWND hCtl)
{
	if ( CBN_SELCHANGE == uCmd )
	{
		switch(uId)
		{
		case IDC_WORLD:	SetItems(); break;
		default: WriteItems(); break;
		}
	}
}

VOID CPaletteDlg::LoadItems()
{
	for(INT i = 0; i < 7; ++i)
	{
		BYTE bLowPtr = 0, bHiPtr = 0;
		if ( !m_File.LoadData(&bLowPtr, 0x0000c010+i+(m_fAdditional ? 7 : 0), 1) ||
			 !m_File.LoadData(&bHiPtr, 0x0000c01e+i+(m_fAdditional ? 7 : 0), 1)
			)
			continue;

		DWORD ptr = (( bHiPtr << 8 ) | ( bLowPtr )) + 0x4010;
		if ( m_fAdditional )
			m_File.LoadData(&m_pPalette[16*8*i], ptr, 16*8);
		else
			m_File.LoadData(&m_pPalette[16*8*i], ptr, 16*8);
	}
}

VOID CPaletteDlg::SaveItems()
{
	for(INT i = 0; i < 7; ++i)
	{
		BYTE bLowPtr = 0, bHiPtr = 0;
		if ( !m_File.LoadData(&bLowPtr, 0x0000c010+i, 1) ||
			 !m_File.LoadData(&bHiPtr, 0x0000c01e+i, 1)
			)
			continue;

		DWORD ptr = (( bHiPtr << 8 ) | ( bLowPtr )) + 0x4010;
		m_File.WriteData(&m_pPalette[16*8*i], ptr, 16*8);
	}
}

VOID CPaletteDlg::SetItems()
{
	INT iWorld = m_cbWorld.GetParam(m_cbWorld.GetCurSel()) - 1, iPal = m_tbPal.Pos();
	if ( iWorld < 0 || iWorld >= 7 )
		return;

	// load bg colors
	for(INT i = 0; i < 4; ++i)
	{
		SendMessage(
			(HWND) dSendMessage(m_cbPal[i], CBEM_GETCOMBOCONTROL),
			CB_SETCURSEL, m_pPalette[16*8*iWorld+16*iPal+4*i], 0 );
	}

	// load group colors
	for(INT i = 0; i < 4; ++i)
	{
		SendMessage(
			(HWND) dSendMessage(m_cbPal[3*i+4], CBEM_GETCOMBOCONTROL),
			CB_SETCURSEL, m_pPalette[16*8*iWorld+16*iPal+4*i+1], 0 );
		SendMessage(
			(HWND) dSendMessage(m_cbPal[3*i+5], CBEM_GETCOMBOCONTROL),
			CB_SETCURSEL, m_pPalette[16*8*iWorld+16*iPal+4*i+2], 0 );
		SendMessage(
			(HWND) dSendMessage(m_cbPal[3*i+6], CBEM_GETCOMBOCONTROL),
			CB_SETCURSEL, m_pPalette[16*8*iWorld+16*iPal+4*i+3], 0 );
	}
}

VOID CPaletteDlg::WriteItems()
{
	INT iWorld = m_cbWorld.GetParam(m_cbWorld.GetCurSel()) - 1, iPal = m_tbPal.Pos();
	if ( iWorld < 0 || iWorld >= 7 )
		return;

	// Write bg colors
	for(INT i = 0; i < 4; ++i)
	{
		m_pPalette[16*8*iWorld+16*iPal+4*i] = (BYTE) 0xFF & SendMessage(
			(HWND) dSendMessage(m_cbPal[i], CBEM_GETCOMBOCONTROL),
			CB_GETCURSEL, 0, 0 );
	}

	// save group colors
	for(INT i = 0; i < 4; ++i)
	{
		m_pPalette[16*8*iWorld+16*iPal+4*i+1] = (BYTE) 0xFF & SendMessage(
			(HWND) dSendMessage(m_cbPal[3*i+4], CBEM_GETCOMBOCONTROL),
			CB_GETCURSEL, 0, 0 );
		m_pPalette[16*8*iWorld+16*iPal+4*i+2] = (BYTE) 0xFF & SendMessage(
			(HWND) dSendMessage(m_cbPal[3*i+5], CBEM_GETCOMBOCONTROL),
			CB_GETCURSEL, m_pPalette[16*8*iWorld+16*iPal+4*i+2], 0 );
		m_pPalette[16*8*iWorld+16*iPal+4*i+3] = (BYTE) 0xFF & SendMessage(
			(HWND) dSendMessage(m_cbPal[3*i+6], CBEM_GETCOMBOCONTROL),
			CB_GETCURSEL, 0, 0 );
	}
}