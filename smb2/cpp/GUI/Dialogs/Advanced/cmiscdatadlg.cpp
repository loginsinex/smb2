
#include "precomp.h"

CMiscDataDlg::CMiscDataDlg(HINSTANCE hInstance, CFileLoader & fl)
	: CDialog(hInstance, MAKEINTRESOURCE(IDD_ADV_MISC)), m_File(fl), m_nmCurSel( 0 )
{
	
#define	NADDATA(n, ptr, desc)	{ NES_MISC_DATA nmdata = { TEXT( desc ), ( ptr ), 0 }; \
								m_mData[n] = nmdata; \
								}
/*
 adr(1) = &H5013
 adr(2) = &H500F
 adr(3) = &H65EA
 adr(4) = &H68C2
 adr(5) = &H6D03
 adr(6) = &H61C6
 adr(7) = &H7255
 adr(8) = &H560E
 adr(9) = &H4C84
 adr(10) = &H6E49
 adr(11) = &H526E
 adr(12) = &H4B87
 adr(13) = &H4BBE
 adr(14) = &H4D5E
 adr(15) = &H52ED
 adr(16) = &H6460
 adr(17) = &H6CC3
*/

/*
 adr(1) = &H39D0
 adr(2) = &H456A
 adr(3) = &H10E6
 adr(4) = &H10EE
 adr(5) = &H10F2
 adr(6) = &H1079
 adr(7) = &H107A
 adr(8) = &H107B
 adr(9) = &H107C
 adr(10) = &H107D
 adr(11) = &H107E
 adr(12) = &H107F
 adr(13) = &H1080
 adr(14) = &H1081
 adr(15) = &H1082
*/
	NADDATA( 1, 0x005017, "Birdo's weapon [1]");
	NADDATA( 2, 0x005013, "Birdo's weapon [2]");
	NADDATA( 3, 0x0065EA, "Mouser's weapon");
	NADDATA( 4, 0x0068C2, "Triclyde's weapon");
	// NADDATA( 5, 0x006D03, "Fryguy's weapon");
	NADDATA( 6, 0x0061C6, "Clawglip's weapon");
	NADDATA( 7, 0x00725C, "Wart's weapon");
	NADDATA( 8, 0x005612, "Snifit's weapon");
	NADDATA( 9, 0x004C88, "Panser's weapon");
	NADDATA(10, 0x006E50, "Autobomb's weapon");
	NADDATA(11, 0x005272, "Albatoss weapon");
	NADDATA(12, 0x004B8B, "Albatoss attack");
	NADDATA(13, 0x004BC2, "Beezo attack");
	NADDATA(14, 0x004D62, "Bob-omb jars");
	NADDATA(15, 0x0052F1, "Shyguy jars");
	NADDATA(16, 0x006460, "Pidgit's carpet");
	NADDATA(17, 0x006CC3, "Small fryguys");

	NADDATA(18, 0x0039D0, "Star");
	NADDATA(19, 0x00456A, "Heart");
	NADDATA(20, 0x0010E6, "Stopwatch herbs");
	// NADDATA(21, 0x0010EE, "Number of herbs");
	NADDATA(22, 0x0010F2, "Stopwatch");
	NADDATA(23, 0x001079, "Coin herb");
	NADDATA(24, 0x00107A, "Fresh herb");
	NADDATA(25, 0x00107B, "Small herb");
	NADDATA(26, 0x00107C, "Rocket herb");
	NADDATA(27, 0x00107D, "Turtle shell herb");
	NADDATA(28, 0x00107E, "Bomb herb");
	NADDATA(29, 0x00107F, "Potion herb");
	NADDATA(30, 0x001080, "1-UP herb");
	NADDATA(31, 0x001081, "POW herb");
	NADDATA(32, 0x001082, "Bob-obm herb");

#undef NADDATA
}

VOID CMiscDataDlg::OnInit(LPARAM lParam)
{
	m_lv.AssignWindow(Ctl(IDC_LIST1));
	m_lv.ExtendedStyle(LVS_EX_FULLROWSELECT);
	m_lv.AddColumn(TEXT("Description"), 125);
	m_lv.AddColumn(TEXT("Value"));

	m_cb.AssignWindow(Ctl(IDC_COMBO1));

	m_cb.cSendMessage(WM_SETFONT,
		m_lv.cSendMessage(WM_GETFONT), 1);

	for(int i = 0; i < 127; ++i)
	{
		if ( i >= 75 && i <= 91 )
			continue;

		CString s;
		s.Format(TEXT("%s"), g_pszEnemies[i]);
		m_cb.AddString(s, i);
	}

	LoadItems();

	for(std::map<NES_MISC_DATA_ENUM, NES_MISC_DATA>::iterator m = m_mData.begin();
		m != m_mData.end(); ++m)
	{
		int id = m_lv.AddItem(m->second.sDescription, m->first);
		m_lv.SetItem(id, 1, g_pszEnemies[m->second.value]);
	}

	m_lv.cSendMessage(LVM_SORTITEMS, (WPARAM) this, (LPARAM) CMiscDataDlg::CompareFunc);
}

VOID CMiscDataDlg::LoadItems()
{
	for(std::map<NES_MISC_DATA_ENUM, NES_MISC_DATA>::iterator m = m_mData.begin();
		m != m_mData.end(); ++m)
		m->second.value = m_File.Byte(m->second.ptr) & 0x7F;
}

VOID CMiscDataDlg::SaveItems()
{
	for(std::map<NES_MISC_DATA_ENUM, NES_MISC_DATA>::iterator m = m_mData.begin();
		m != m_mData.end(); ++m)
		m_File.Byte(m->second.ptr, m->second.value);
}

INT CMiscDataDlg::PSOnApply()
{
	m_lv.CurSel(-1);
	SaveItems();
	return PSNRET_NOERROR;
}


int CALLBACK CMiscDataDlg::CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	return ((CMiscDataDlg*) lParamSort)->LVSort(NES_MISC_DATA_ENUM (lParam1), NES_MISC_DATA_ENUM (lParam2));
}

int CMiscDataDlg::LVSort(NES_MISC_DATA_ENUM lv1, NES_MISC_DATA_ENUM lv2)
{
	if ( m_mData.find(lv1) == m_mData.end() || m_mData.find(lv2) == m_mData.end() )
		return 0;

	return lstrcmp(m_mData[lv1].sDescription, m_mData[lv2].sDescription);
}

INT_PTR CMiscDataDlg::OnNotify(LPNMHDR lpnm)
{
	if ( IDC_LIST1 == lpnm->idFrom )
	{
		switch(lpnm->code)
		{
		case LVN_ITEMCHANGED:
			{
				int id = m_lv.CurSel();
				if ( id >= 0 )
				{
					NES_MISC_DATA_ENUM nmde = m_lv.Param(id);
					m_nmCurSel = nmde;
					m_cb.Enable(TRUE);					
					int c = m_cb.cSendMessage(CB_GETCOUNT);
					for(int i = 0; i < c; ++i)
					{
						if ( m_cb.GetParam(i) == m_mData[nmde].value )
						{
							m_cb.SetCurSel(i);
							break;
						}
					}
					
				}
				else
				{
					m_nmCurSel = 0;
					m_cb.Enable(FALSE);
					m_cb.SetCurSel(-1);
				}		
				break;
			}
		}
	}

	return 0;
}

VOID CMiscDataDlg::OnCommand(USHORT uCmd, USHORT uId, HWND hCtl)
{
	if ( CBN_SELENDOK == uCmd && IDC_COMBO1 == uId )
	{
		if ( m_nmCurSel > 0 && m_cb.GetCurSel() >= 0 )
		{
			DWORD id = m_cb.GetParam( m_cb.GetCurSel() );
			m_mData[m_nmCurSel].value = LOBYTE(LOWORD(id));
			
			int c = m_lv.Count();
			for(int i = 0; i < c; ++i)
			{
				if ( m_nmCurSel == m_lv.Param(i) )
				{
					m_lv.SetItem(i, 1, g_pszEnemies[id]);
					break;
				}
			}
		}
	}
}