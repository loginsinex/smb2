
#include "precomp.h"

CTilesDlg::CTilesDlg(HINSTANCE hInstance, CFileLoader & fl, LPTSTR lpszRes) : CDialog(hInstance, lpszRes) , m_File(fl)
{
	ZeroMemory(&m_Image, sizeof(m_Image));
//	LoadTiles();
}

CTilesDlg::~CTilesDlg()
{
	for(int i=0; i < 5; ++i)
		if ( m_Image.himl[i] )
			ImageList_Destroy(m_Image.himl[i]);
}

INT CTilesDlg::OnMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & pHandle)
{
	if ( WM_HSCROLL == uMsg || WM_VSCROLL == uMsg )
	{
		NMHDR nm = { 0 };
		nm.code = uMsg;
		nm.hwndFrom = HWND(lParam);
		nm.idFrom = GetWindowLong(nm.hwndFrom, GWL_ID);
		
		OnNotify(&nm);
	}

	return 0;
}

INT CTilesDlg::GetFX(INT iWorld)
{
	if ( iWorld < 0 || iWorld >= 7 )
		return 0;

	BYTE bLow = 0, bHi = 0;
	m_File.LoadData(&bLow, 0x0001fe10+iWorld, 1);
	m_File.LoadData(&bHi, 0x0001fe17+iWorld, 1);
	INT fx = ( bLow ) | ( bHi << 8 );
	
	for(INT i = 0; i < 5; ++i)
		if ( g_mWorldInterior[i] == (DWORD) fx )
			return i;

	return 0;
}

VOID CTilesDlg::LoadTiles()
{
	for(int i=IDB_BITMAP16; i<=IDB_BITMAP20;++i)
	{
		HBITMAP hBmp = LoadBitmap(g_hResInstance, MAKEINTRESOURCE(i));
		BITMAP bmp={ 0 };
		if ( GetObject(hBmp, sizeof(bmp), &bmp) )
		{
			m_Image.himl[i-IDB_BITMAP16] = ImageList_Create(bmp.bmHeight, bmp.bmHeight, ILC_COLOR16 | ILC_MASK, 256, 1); //  LoadBitmap(g_hResInstance, MAKEINTRESOURCE(IDB_BITMAP11), 16, 1, RGB(0,0,0));
			ImageList_AddMasked(m_Image.himl[i-IDB_BITMAP16], hBmp, RGB(0,0,0));
			DeleteObject(hBmp);
		}
	}
}


CObjTilesDlg::CObjTilesDlg(HINSTANCE hInstance, CFileLoader & fl) : CTilesDlg(hInstance, fl, MAKEINTRESOURCE(IDD_ADV_OBJ))
{

}

CObjTilesDlg::~CObjTilesDlg()
{

}


VOID CObjTilesDlg::OnInit(LPARAM lParam)
{
	m_cbWorld.AssignWindow(Ctl(IDC_WORLD));
	m_cbObj.AssignWindow(Ctl(IDC_COMBO1));
	m_cbTile[0] = IDC_TILE0;
	m_cbTile[1] = IDC_TILE1;
	m_cbTile[2] = IDC_TILE2;
	m_cbTile[3] = IDC_TILE3;

	LoadTiles();
	SetTiles(0);

	for(INT i = 1; i <=7; ++i)
	{
		CString sWorld; sWorld.Format(TEXT("World %d"), i);
		m_cbWorld.AddString(sWorld.GetBuffer(), i);
	}
	for(INT i = 0; i < 8; ++i)
	{
		m_cbObj.AddString(g_szItemName[0x30+i], i);
	}

	for(INT i = 0; i < 256; ++i)
	{
		CString sByte; sByte.Format(TEXT("0x%02X"), i);
		COMBOBOXEXITEM cbei = { 0 };
		cbei.mask = CBEIF_TEXT | CBEIF_LPARAM | CBEIF_IMAGE | CBEIF_SELECTEDIMAGE;
		cbei.pszText = sByte.GetBuffer();
		cbei.lParam = i;
		cbei.iItem = i;
		cbei.iImage = i;
		cbei.iSelectedImage = i;
		for(INT k = 0; k < 4; ++k)
			dSendMessage(m_cbTile[k], CBEM_INSERTITEM, 0, (LPARAM) &cbei);
	}

	m_cbWorld.SetCurSel(0);
	m_cbObj.SetCurSel(0);
	LoadItems();
	SetItems();
}

VOID CObjTilesDlg::OnCommand(USHORT uCmd, USHORT uId, HWND hCtl)
{
	if ( CBN_SELCHANGE == uCmd && (IDC_WORLD == uId || IDC_COMBO1 == uId) )
		SetItems();
	else if ( CBN_SELCHANGE == uCmd )
		WriteItems();
}

INT CObjTilesDlg::PSOnApply()
{
	SaveItems();
	return PSNRET_NOERROR;
}

VOID CObjTilesDlg::LoadItems()
{
	for(INT i = 0; i < 7; ++i)
	{
		BYTE bLowPtr = 0, bHiPtr = 0;
		USHORT ptr = 0;

		m_File.LoadData(&bLowPtr, 0x0000c911+i, 1);
		m_File.LoadData(&bHiPtr, 0x0000c918+i, 1);
		ptr = (( bLowPtr ) | ( bHiPtr << 8 )) + 0x00004010;
		m_File.LoadData(&m_pTilesArray[36*i], ptr, 36);
	}
}

VOID CObjTilesDlg::SaveItems()
{
	for(INT i = 0; i < 7; ++i)
	{
		BYTE bLowPtr = 0, bHiPtr = 0;
		USHORT ptr = 0;

		m_File.LoadData(&bLowPtr, 0x0000c911+i, 1);
		m_File.LoadData(&bHiPtr, 0x0000c918+i, 1);
		ptr = (( bLowPtr ) | ( bHiPtr << 8 )) + 0x00004010;
		m_File.WriteData(&m_pTilesArray[36*i], ptr, 36);
	}
}

VOID CObjTilesDlg::SetItems()
{
	INT iWorld = m_cbWorld.GetParam(m_cbWorld.GetCurSel())-1;
	INT ptr = 36*(m_cbWorld.GetParam(m_cbWorld.GetCurSel())-1) + 4*(m_cbObj.GetParam(m_cbObj.GetCurSel()));
	if ( ptr < 0 || ptr >= sizeof(m_pTilesArray) )
		return;

	SetTiles(GetFX(iWorld));

	for(INT k = 0; k < 4; ++k)
	{
		SendMessage(
			(HWND) dSendMessage(m_cbTile[k], CBEM_GETCOMBOCONTROL), CB_SETCURSEL, m_pTilesArray[ptr+k], 0
			);
	}
}

VOID CObjTilesDlg::WriteItems()
{
	INT ptr = 36*(m_cbWorld.GetParam(m_cbWorld.GetCurSel())-1) + 4*(m_cbObj.GetParam(m_cbObj.GetCurSel()));
	if ( ptr < 0 || ptr >= sizeof(m_pTilesArray) )
		return;

	for(INT k = 0; k < 4; ++k)
	{
		INT id = SendMessage(
			(HWND) dSendMessage(m_cbTile[k], CBEM_GETCOMBOCONTROL), CB_GETCURSEL, 0, 0
			);

		COMBOBOXEXITEM cbei = { 0 };
		cbei.mask = CBEIF_LPARAM;
		cbei.iItem = id;
		dSendMessage(m_cbTile[k], CBEM_GETITEM, 0, (LPARAM) &cbei);
		
		m_pTilesArray[ptr+k] = (BYTE) ( cbei.lParam & 0xFF );
	}
}

VOID CObjTilesDlg::SetTiles(INT i)
{
	dSendMessage(m_cbTile[0], CBEM_SETIMAGELIST, 0, (LPARAM) m_Image.himl[i]);
	dSendMessage(m_cbTile[1], CBEM_SETIMAGELIST, 0, (LPARAM) m_Image.himl[i]);
	dSendMessage(m_cbTile[2], CBEM_SETIMAGELIST, 0, (LPARAM) m_Image.himl[i]);
	dSendMessage(m_cbTile[3], CBEM_SETIMAGELIST, 0, (LPARAM) m_Image.himl[i]);
}
