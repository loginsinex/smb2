
#include "precomp.h"

CWrldItrDlg::CWrldItrDlg(HINSTANCE hInstance, CFileLoader & fl) : CDialog(hInstance, MAKEINTRESOURCE(IDD_ADV_INTERIOR)), m_File( fl )
{

}

VOID CWrldItrDlg::OnInit(LPARAM lParam)
{
	m_cbIterior[0].AssignWindow(Ctl(IDC_COMBO1));
	m_cbIterior[1].AssignWindow(Ctl(IDC_COMBO2));
	m_cbIterior[2].AssignWindow(Ctl(IDC_COMBO3));
	m_cbIterior[3].AssignWindow(Ctl(IDC_COMBO4));
	m_cbIterior[4].AssignWindow(Ctl(IDC_COMBO5));
	m_cbIterior[5].AssignWindow(Ctl(IDC_COMBO6));
	m_cbIterior[6].AssignWindow(Ctl(IDC_COMBO7));

	for(INT i = 0; i < 7; ++i)
	{
		m_cbIterior[i].AddString(TEXT("Night"), g_mWorldInterior[0]);
		m_cbIterior[i].AddString(TEXT("Day"), g_mWorldInterior[1]);
		m_cbIterior[i].AddString(TEXT("Desert"), g_mWorldInterior[2]);
		m_cbIterior[i].AddString(TEXT("Winter"), g_mWorldInterior[3]);
		m_cbIterior[i].AddString(TEXT("Castle"), g_mWorldInterior[4]);
	}

	LoadItems();
	SetItems();
}

VOID CWrldItrDlg::LoadItems()
{
	for(INT i = 0; i < 7; ++i)
	{
		BYTE bLow = 0, bHi = 0;
		m_File.LoadData(&bLow, 0x0001fe10+i, 1);
		m_File.LoadData(&bHi, 0x0001fe17+i, 1);
		m_pData[i] = ( bLow ) | ( bHi << 8 );
	}
}

VOID CWrldItrDlg::SaveItems()
{
	for(INT i = 0; i < 7; ++i)
	{
		BYTE bLow = m_pData[i] & 0xFF, bHi = ( m_pData[i] >> 8 ) & 0xFF;		
		m_File.WriteData(&bLow, 0x0001fe10+i, 1);
		m_File.WriteData(&bHi, 0x0001fe17+i, 1);
	}
}

VOID CWrldItrDlg::SetItems()
{
	for(INT i = 0; i < 7; ++i)
	{
		for(INT k = 0; k < 5; ++k)
		{
			if ( m_cbIterior[i].GetParam(k) == m_pData[i] ) 
			{
				m_cbIterior[i].SetCurSel(k);
				break;
			}
		}
	}
}

VOID CWrldItrDlg::WriteItems()
{
	for(INT i = 0; i < 7; ++i)
	{
		if ( -1 != m_cbIterior[i].GetCurSel() )
			m_pData[i] = (USHORT) (0xFFFF & m_cbIterior[i].GetParam(m_cbIterior[i].GetCurSel()));
	}
}

INT CWrldItrDlg::PSOnApply()
{
	WriteItems();
	SaveItems();
	return PSNRET_NOERROR;
}
