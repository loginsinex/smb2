
#include "precomp.h"

CLvlOptionsDlg::CLvlOptionsDlg(CFileLoader & fl, HINSTANCE hInstance) : CDialog(hInstance, MAKEINTRESOURCE(IDD_LEVELOPTIONS)), m_File(fl)
{
	AddAnchor(IDC_ST1, ANCHOR_TOP);
	AddAnchor(IDC_ST2, ANCHOR_TOP);
	AddAnchor(IDC_ST3, ANCHOR_TOP);
	AddAnchor(IDC_ST4, ANCHOR_TOP);
	AddAnchor(IDC_ST5, ANCHOR_TOP);

	AddAnchor(IDC_DIRHOR, ANCHOR_TOP);
	AddAnchor(IDC_DIRVER, ANCHOR_TOP);

	AddAnchor(IDC_COLGAMMA1, ANCHOR_TOP);
	AddAnchor(IDC_COLGAMMA2, ANCHOR_TOP);
	AddAnchor(IDC_GROUNDTYPE, ANCHOR_TOP);
	AddAnchor(IDC_GROUND, ANCHOR_TOP);
	AddAnchor(IDC_MUSIC, ANCHOR_TOP);
	AddAnchor(IDC_LENGTH, ANCHOR_TOP);

}

void CLvlOptionsDlg::OnInit(LPARAM lParam)
{
	m_ctl.cbGamma1.AssignWindow(Ctl(IDC_COLGAMMA1));
	m_ctl.cbGamma2.AssignWindow(Ctl(IDC_COLGAMMA2));
	m_ctl.cbGroundType.AssignWindow(Ctl(IDC_GROUNDTYPE));
	m_ctl.cbGround.AssignWindow(Ctl(IDC_GROUND));
	m_ctl.cbMusic.AssignWindow(Ctl(IDC_MUSIC));
	m_ctl.tbLength.AssignWindow(Ctl(IDC_LENGTH));
	
	CString s;

	for(int x=0;x<8;++x) { s.Format(TEXT("Level color #%d"), x+1); m_ctl.cbGamma1.AddString(s.GetString(), x); }
	for(int x=0;x<4;++x) { s.Format(TEXT("Enemy color #%d"), x+1); m_ctl.cbGamma2.AddString(s.GetString(), x); }
	for(int x=0;x<8;++x) { s.Format(TEXT("Ground type #%d"), x+1); m_ctl.cbGroundType.AddString(s.GetString(), x); }
	for(int x=0;x<32;++x) { s.Format(TEXT("Ground #%d"), x+1); m_ctl.cbGround.AddString(s.GetString(), x); }
	for(int x=0;x<4;++x) { s.Format(TEXT("Music #%d"), x+1); m_ctl.cbMusic.AddString(s.GetString(), x); }
	m_ctl.tbLength.SetMinMax(1, 16);

	EnableThemeDialogTexture(GetWindowHandle(), ETDT_ENABLETAB);
}

void CLvlOptionsDlg::OnButton(USHORT uId)
{
	switch(uId)
	{
	case IDC_DIRHOR:
	case IDC_DIRVER:
		{
			CheckRadioButton(GetWindowHandle(), IDC_DIRHOR, IDC_DIRVER, uId);
			break;
		}
	}
}

VOID CLvlOptionsDlg::CheckData()
{
	EnableAll(m_File.IsLoaded(), IDC_DIRHOR, IDC_DIRVER, IDC_COLGAMMA1, IDC_COLGAMMA2, IDC_GROUND, IDC_GROUNDTYPE, IDC_MUSIC, IDC_LENGTH, 0);

	if ( !m_File.IsLoaded() )
		return;

	UINT uCurLevel = ((CMainWnd*) Parent())->GetCurLevel();

/*
	CheckRadioButton(GetWindowHandle(), IDC_DIRHOR, IDC_DIRVER, m_File.levelValue(uCurLevel, 0) ? IDC_DIRHOR : IDC_DIRVER);

	m_ctl.cbGamma1.SetCurSel(m_File.levelValue(uCurLevel,1));
	m_ctl.cbGamma2.SetCurSel(m_File.levelValue(uCurLevel,2));
	m_ctl.cbGroundType.SetCurSel(m_File.levelValue(uCurLevel,7));
	m_ctl.cbGround.SetCurSel(m_File.levelValue(uCurLevel,3));
	m_ctl.cbMusic.SetCurSel(m_File.levelValue(uCurLevel,5));
	m_ctl.tbLength.Pos(m_File.levelValue(uCurLevel,4)+1);
*/
}