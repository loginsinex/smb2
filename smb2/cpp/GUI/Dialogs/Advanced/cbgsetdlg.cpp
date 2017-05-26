
#include "precomp.h"

CBgSetHorzDlg::CBgSetHorzDlg(HINSTANCE hInstance, CFileLoader & fl, BOOL fHorizontal) : CTilesDlg(hInstance, fl, MAKEINTRESOURCE(IDD_ADV_HORZ_SET)) , m_fHorizontal( fHorizontal )
{

}

VOID CBgSetHorzDlg::SetTiles(INT iFX)
{
	dSendMessage(IDC_LIST1, LVM_SETIMAGELIST, LVSIL_SMALL, (LPARAM) m_Image.himl[iFX]);
	dSendMessage(IDC_TILE0, CBEM_SETIMAGELIST,0, (LPARAM) m_Image.himl[iFX]);
}

VOID CBgSetHorzDlg::OnInit(LPARAM lParam)
{
	m_lvTiles.AssignWindow(Ctl(IDC_LIST1));
	m_tbSet.AssignWindow(Ctl(IDC_SLIDER1));
	m_cbWorld.AssignWindow(Ctl(IDC_WORLD));

	m_lvTiles.AddColumn(TEXT(""), 100);
	m_lvTiles.AddColumn(TEXT(""), 100);
	m_lvTiles.ExtendedStyle(LVS_EX_FULLROWSELECT);

	m_tbSet.SetMinMax(0, 31 - ( m_fHorizontal ? 1 : 0 ));

	for(INT i = 1; i <=7; ++i)
	{
		CString sWorld; sWorld.Format(TEXT("World %d"), i);
		m_cbWorld.AddString(sWorld.GetBuffer(), i);
	}

	for(INT i = 0; i < 16; ++i)
	{
		INT id = m_lvTiles.AddItem(i, MAKELONG(i+1, 0));
		m_lvTiles.SetItem(id, 1, i);
	}

	COMBOBOXEXITEM cbei = { 0 };
	cbei.mask = CBEIF_TEXT | CBEIF_LPARAM | CBEIF_IMAGE | CBEIF_SELECTEDIMAGE;
	cbei.iImage = 0;
	cbei.iSelectedImage = 0;

	cbei.pszText = TEXT("Space"); cbei.iItem = cbei.lParam = 0;
	dSendMessage(IDC_TILE0, CBEM_INSERTITEM, 0, (LPARAM) &cbei);
	cbei.pszText = TEXT("Block 1"); cbei.iItem = cbei.lParam = 1;
	dSendMessage(IDC_TILE0, CBEM_INSERTITEM, 0, (LPARAM) &cbei);
	cbei.pszText = TEXT("Block 2"); cbei.iItem = cbei.lParam = 2;
	dSendMessage(IDC_TILE0, CBEM_INSERTITEM, 0, (LPARAM) &cbei);
	cbei.pszText = TEXT("Block 3"); cbei.iItem = cbei.lParam = 3;
	dSendMessage(IDC_TILE0, CBEM_INSERTITEM, 0, (LPARAM) &cbei);

	m_cbWorld.SetCurSel(0);
	LoadItems();
	SetTiles(0);
	SetItems();
}

VOID CBgSetHorzDlg::OnCommand(USHORT uCmd, USHORT uId, HWND hWnd)
{
	if ( CBN_SELCHANGE == uCmd && IDC_TILE0 == uId )
	{
		USHORT bg = (USHORT) ( 0xFFFF & SendMessage((HWND) dSendMessage(IDC_TILE0, CBEM_GETCOMBOCONTROL), CB_GETCURSEL, 0, 0) );

		INT id = m_lvTiles.CurSel();
		if ( id >= 0 )
		{
			INT param = m_lvTiles.Param(id);
			m_lvTiles.Param(id, MAKELONG(LOWORD(param), bg));

			CString s;
			s.Format( bg < 1 ? TEXT("Space") : TEXT("Block %d"), bg );
			m_lvTiles.SetItem(id, 0, id+1);
			m_lvTiles.SetItem(id, 1, s.GetBuffer());
			WriteItems();
		}

	}
}


VOID CBgSetHorzDlg::LoadItems()
{
	m_File.LoadData((PBYTE) &m_dwBgSet[0], 0x0000d210 + ( m_fHorizontal ? 0 : 0x80 - 4), 128);
}

VOID CBgSetHorzDlg::SaveItems()
{
	m_File.WriteData((PBYTE) &m_dwBgSet[0], 0x0000d210 + ( m_fHorizontal ? 0 : 0x80 - 4), 128 - ( m_fHorizontal ? 4 : 0 ));
}

VOID CBgSetHorzDlg::SetItems()
{
	SetTiles(GetFX(m_cbWorld.GetParam(m_cbWorld.GetCurSel())-1));

	INT k = m_tbSet.Pos();
	if ( k < 0 || k >= 32 )
		return ;

	DWORD dwBgSet = SWAP_DWORD(m_dwBgSet[k]);
	for(INT i = 0; i < 16; ++i)
	{
		BYTE bg = 3 & (dwBgSet >> (30-2*i));
		m_lvTiles.SetItem(i, 0, i+1);
		m_lvTiles.Param(i, MAKELONG(i, bg));
		CString s;
		s.Format( bg < 1 ? TEXT("Space") : TEXT("Block %d"), bg );
		m_lvTiles.SetItem(i, 1, s.GetBuffer());
	}
}

VOID CBgSetHorzDlg::WriteItems()
{
	DWORD dwBgSet = 0;
	INT k = m_tbSet.Pos();
	if ( k < 0 || k >= 32 )
		return ;

	for(INT i = 0; i < 16; ++i)
	{
		USHORT bg = HIWORD(m_lvTiles.Param(i)); // 3 & (dwBgSet >> (30-2*i));
		dwBgSet <<= 2;
		dwBgSet |= (bg & 3);
	}

	m_dwBgSet[k] = SWAP_DWORD(dwBgSet);

}

INT_PTR CBgSetHorzDlg::OnNotify(LPNMHDR lpnm)
{
	if ( IDC_SLIDER1 == lpnm->idFrom && WM_HSCROLL == lpnm->code )
	{
		if ( m_tbPos != m_tbSet.Pos() )
		{
			m_tbPos = m_tbSet.Pos();
			SetItems();
		}
	}
	else if ( IDC_LIST1 == lpnm->idFrom )
	{
		if ( LVN_ITEMCHANGED == lpnm->code )
		{
			USHORT bg = HIWORD(m_lvTiles.Param(m_lvTiles.CurSel()));
			SendMessage((HWND) dSendMessage(IDC_TILE0, CBEM_GETCOMBOCONTROL), CB_SETCURSEL, bg, 0);
		}
	}
	return 0;
}

INT CBgSetHorzDlg::PSOnApply()
{
	SaveItems();
	return PSNRET_NOERROR;
}
