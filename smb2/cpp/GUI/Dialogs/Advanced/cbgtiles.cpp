
#include "precomp.h"

CBgTilesDlg::CBgTilesDlg(HINSTANCE hInstance, CFileLoader & flLoader) : CTilesDlg(hInstance, flLoader, MAKEINTRESOURCE(IDD_ADV_BG)) // CDialog(hInstance, MAKEINTRESOURCE(IDD_ADV_BG)), m_File( flLoader )
{

}

CBgTilesDlg::~CBgTilesDlg()
{

}

VOID CBgTilesDlg::SetTiles(INT i)
{
	dSendMessage(m_cbTile[1], CBEM_SETIMAGELIST, 0, (LPARAM) m_Image.himl[i]);
	dSendMessage(m_cbTile[2], CBEM_SETIMAGELIST, 0, (LPARAM) m_Image.himl[i]);
	dSendMessage(m_cbTile[3], CBEM_SETIMAGELIST, 0, (LPARAM) m_Image.himl[i]);
}

VOID CBgTilesDlg::OnInit(LPARAM lParam)
{
	m_cbWorld.AssignWindow(Ctl(IDC_WORLD));
	m_tbBgType.AssignWindow(Ctl(IDC_SLIDER1));
	m_cbTile[0] = IDC_TILE0;
	m_cbTile[1] = IDC_TILE1;
	m_cbTile[2] = IDC_TILE2;
	m_cbTile[3] = IDC_TILE3;

	LoadTiles();
	SetTiles(0);

	m_tbBgType.SetMinMax(0, 7);
	for(INT i = 1; i <=7; ++i)
	{
		CString sWorld; sWorld.Format(TEXT("World %d"), i);
		m_cbWorld.AddString(sWorld.GetBuffer(), i);
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

	m_uTbPos = 0;
	m_cbWorld.SetCurSel(0);
	m_tbBgType.Pos(m_uTbPos);
	LoadItems();
	SetItems();

}

INT_PTR CBgTilesDlg::OnNotify(LPNMHDR lpnm)
{
	if ( IDC_SLIDER1 == lpnm->idFrom && WM_HSCROLL == lpnm->code ) // lpnm->hwndFrom == m_tbBgType.GetWindowHandle() )
	{
		m_tbBgType.Pos();
		SetItems();
	}
	else
	{
		switch(lpnm->idFrom)
		{
		case IDC_TILE1:
		case IDC_TILE2:
		case IDC_TILE3:
			{
				if ( CBEN_ENDEDIT == lpnm->code )
				{
					WriteItems();
				}
				break;
			}
		}
	}
	return 0;
}

VOID CBgTilesDlg::OnButton(USHORT uId)
{
	if ( IDC_DIRECTION == uId )
		SetItems();
}

VOID CBgTilesDlg::OnCommand(USHORT uCmd, USHORT uId, HWND hCtl)
{
	if ( IDC_WORLD == uId && CBN_SELCHANGE == uCmd )
		SetItems();
	else if ( CBN_SELCHANGE == uCmd && uId >= IDC_TILE1 && uId <= IDC_TILE3 )
	{
		WriteItems();
	}
}

VOID CBgTilesDlg::LoadItems()
{
	for(INT i=0; i<7; ++i)
	{
		// Load pointer for horizontal level data
		BYTE bLowPtr = 0, bHiPtr = 0;
		USHORT ptr = 0;
		m_File.LoadData(&bLowPtr, 0x0000c438+i, 1);
		m_File.LoadData(&bHiPtr, 0x0000c446+i, 1);
		
		// make pointer
		ptr = 0x4010 + (((USHORT) bLowPtr) | (((USHORT) bHiPtr) << 8));
		
		// load bg data
		if ( ptr > 0 ) 
			m_File.LoadData(&m_pTilesArray[64*i], ptr, 32);

		// Load pointer for vertical level data
		m_File.LoadData(&bLowPtr, 0x0000c43f+i, 1);
		m_File.LoadData(&bHiPtr, 0x0000c44d+i, 1);
		
		// make pointer
		ptr = 0x4010 + (((USHORT) bLowPtr) | (((USHORT) bHiPtr) << 8));
		
		// load bg data
		if ( ptr > 0 ) 
			m_File.LoadData(&m_pTilesArray[64*i+32], ptr, 32);
	}
}

VOID CBgTilesDlg::SetItems()
{
	INT iWorld = m_cbWorld.GetParam( m_cbWorld.GetCurSel() ) - 1, iType = m_tbBgType.Pos();
	BOOL fVertical = ( BST_CHECKED == dSendMessage(IDC_DIRECTION, BM_GETCHECK) ? FALSE : TRUE );

	INT ptr = iWorld*64 + ( fVertical ? 0 : 32 ) + 4*iType;
	if ( ptr < 0 || ptr >= sizeof(m_pTilesArray) )
		return;

	SetTiles(GetFX(iWorld));

	for(INT i = 0; i < 4; ++i)
	{
		SendMessage(
			(HWND) dSendMessage(m_cbTile[i], CBEM_GETCOMBOCONTROL), CB_SETCURSEL, m_pTilesArray[ptr+i], 0
			);
	}
}

VOID CBgTilesDlg::SaveItems()
{
	for(INT i=0; i<7; ++i)
	{
		// Load pointer for horizontal level data
		BYTE bLowPtr = 0, bHiPtr = 0;
		USHORT ptr = 0;
		m_File.LoadData(&bLowPtr, 0x0000c438+i, 1);
		m_File.LoadData(&bHiPtr, 0x0000c446+i, 1);
		
		// make pointer
		ptr = 0x4010 + (((USHORT) bLowPtr) | (((USHORT) bHiPtr) << 8));
		
		// load bg data
		if ( ptr > 0 ) 
			m_File.WriteData(&m_pTilesArray[64*i], ptr, 32);

		// Load pointer for vertical level data
		m_File.LoadData(&bLowPtr, 0x0000c43f+i, 1);
		m_File.LoadData(&bHiPtr, 0x0000c44d+i, 1);
		
		// make pointer
		ptr = 0x4010 + (((USHORT) bLowPtr) | (((USHORT) bHiPtr) << 8));
		
		// load bg data
		if ( ptr > 0 ) 
			m_File.WriteData(&m_pTilesArray[64*i+32], ptr, 32);
	}
}
VOID CBgTilesDlg::WriteItems()
{
	INT iWorld = m_cbWorld.GetParam( m_cbWorld.GetCurSel() ) - 1, iType = m_tbBgType.Pos();
	BOOL fVertical = ( BST_CHECKED == dSendMessage(IDC_DIRECTION, BM_GETCHECK) ? FALSE : TRUE );

	INT ptr = iWorld*64 + ( fVertical ? 0 : 32 ) + 4*iType;
	if ( ptr < 0 || ptr >= sizeof(m_pTilesArray) )
		return;

	for(INT i = 0; i < 4; ++i)
	{
		INT id = SendMessage(
			(HWND) dSendMessage(m_cbTile[i], CBEM_GETCOMBOCONTROL), CB_GETCURSEL, 0, 0);

		COMBOBOXEXITEM cbei = { 0 };
		cbei.mask = CBEIF_LPARAM;
		cbei.iItem = id;
		dSendMessage(m_cbTile[i], CBEM_GETITEM, 0, (LPARAM) &cbei);
		
		m_pTilesArray[ptr+i] = (BYTE) ( cbei.lParam & 0xFF );
	}
}

INT CBgTilesDlg::PSOnApply()
{
//	m_File.WriteData(&m_pTilesArray[0], 0x0000c454, sizeof(m_pTilesArray));
	SaveItems();
	return PSNRET_NOERROR;
}
