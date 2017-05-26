
#include "precomp.h"

CWarpsDlg::CWarpsDlg(HINSTANCE hInstance, CFileLoader & fl)
	: CDialog(hInstance, MAKEINTRESOURCE(IDD_ADV_WARP)), m_File( fl )
{

}


VOID CWarpsDlg::OnInit(LPARAM lParam)
{
	m_cbWarps[0].AssignWindow(Ctl(IDC_COMBO1));
	m_cbWarps[1].AssignWindow(Ctl(IDC_COMBO2));
	m_cbWarps[2].AssignWindow(Ctl(IDC_COMBO3));
	m_cbWarps[3].AssignWindow(Ctl(IDC_COMBO4));
	m_cbWarps[4].AssignWindow(Ctl(IDC_COMBO5));
	m_cbWarps[5].AssignWindow(Ctl(IDC_COMBO6));
	m_cbWarps[6].AssignWindow(Ctl(IDC_COMBO7));

	LoadItems();

	for(int i = 0; i < 7; ++i)
	{
		CString s;
		s.Format(TEXT("World %d"), i + 1);
		for(int k = 0; k < 7; ++k)
		{
			int id = m_cbWarps[k].AddString(s, i);
			if ( m_pData[k] == i )
				m_cbWarps[k].SetCurSel(id);
		}
	}
}

VOID CWarpsDlg::LoadItems()
{
	m_File.LoadData(m_pData, 0x1f66a, 7);
}

VOID CWarpsDlg::SaveItems()
{
	m_File.WriteData(m_pData, 0x1f66a, 7);
}

VOID CWarpsDlg::WriteItems()
{
	for(int i = 0; i < 7; ++i)
	{
		int id = m_cbWarps[i].GetCurSel();
		if ( id >= 0 )
		{
			id = m_cbWarps[i].GetParam(id);
			if ( id >= 0 && id < 7 )
				m_pData[i] = id;
		}
	}
}

INT CWarpsDlg::PSOnApply()
{
	WriteItems();
	SaveItems();
	return PSNRET_NOERROR;
}