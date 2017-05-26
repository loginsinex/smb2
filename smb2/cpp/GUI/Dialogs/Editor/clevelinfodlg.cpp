
#include "precomp.h"

CLevelInfoDlg::CLevelInfoDlg(HINSTANCE hInstance, CNesEditor & pvEditor, DWORD dwCurLevel, CFileLoader & file,
		CNesLevel & lvl, CNesEnemyData & enm)
	:
	CDialog(hInstance, MAKEINTRESOURCE(IDD_LEVELINFO)), m_pvEditor( pvEditor ), m_dwCurLevel( dwCurLevel ), m_File( file ),
	m_vEditor(lvl, enm, file, dwCurLevel)
{

}


VOID CLevelInfoDlg::OnInit(LPARAM lParam)
{
	m_pLevel = (CLvlDraw*) dSendMessage(IDC_LEVEL, 0x1133);
	if ( m_pLevel )
	{
		m_pLevel->LoadBmp(g_hResInstance, MAKEINTRESOURCE(IDB_BITMAP1), 0, 16, 16);
		m_pLevel->LoadBmp(g_hResInstance, MAKEINTRESOURCE(IDB_BITMAP2), 1, 16, 16);
		m_pLevel->LoadBmp(g_hResInstance, MAKEINTRESOURCE(IDB_BITMAP3), 2, 16, 16);
		m_pLevel->LoadBmp(g_hResInstance, MAKEINTRESOURCE(IDB_BITMAP4), 3, 16, 16);
		m_pLevel->LoadBmp(g_hResInstance, MAKEINTRESOURCE(IDB_BITMAP5), 4, 16, 16);
		m_pLevel->LoadBmp(g_hResInstance, MAKEINTRESOURCE(IDB_BITMAP6), 5, 16, 16);
		m_pLevel->LoadBmp(g_hResInstance, MAKEINTRESOURCE(IDB_BITMAP7), 6, 16, 16);
		m_pLevel->LoadBmp(g_hResInstance, MAKEINTRESOURCE(IDB_BITMAP8), 7, 16, 16);
		m_pLevel->LoadBmp(g_hResInstance, MAKEINTRESOURCE(IDB_BITMAP10), 8, 16, 16);
		m_pLevel->LoadGamma(g_hResInstance, MAKEINTRESOURCE(IDB_BITMAP9));

		m_pLevel->LoadBmp(g_hResInstance, MAKEINTRESOURCE(IDB_BITMAP11), 10, 16, 16);
		m_pLevel->LoadBmp(g_hResInstance, MAKEINTRESOURCE(IDB_BITMAP12), 11, 16, 16);
		m_pLevel->LoadBmp(g_hResInstance, MAKEINTRESOURCE(IDB_BITMAP13), 12, 16, 16);
		m_pLevel->LoadBmp(g_hResInstance, MAKEINTRESOURCE(IDB_BITMAP14), 13, 16, 16);
		m_pLevel->LoadBmp(g_hResInstance, MAKEINTRESOURCE(IDB_BITMAP15), 14, 16, 16);

		m_pLevel->UseEnemyMask(8, 0);
		m_pLevel->UseEnemyMask(8, 1);
		m_pLevel->UseEnemyMask(8, 2);
		m_pLevel->UseEnemyMask(8, 3);
	}

	m_tbLength.AssignWindow(Ctl(IDC_SLIDER1));
	m_tbBgSet.AssignWindow(Ctl(IDC_SLIDER2));
	m_tbBgType.AssignWindow(Ctl(IDC_SLIDER3));
	m_tbObjType.AssignWindow(Ctl(IDC_SLIDER4));
	m_tbPallete.AssignWindow(Ctl(IDC_SLIDER5));
	m_tbEnemyPallete.AssignWindow(Ctl(IDC_SLIDER6));
	m_cbMusic.AssignWindow(Ctl(IDC_COMBO1));

	m_tbLength.SetMinMax(1, 10);
	m_tbBgSet.SetMinMax(0, 31);
	m_tbBgType.SetMinMax(0, 7);
	m_tbObjType.SetMinMax(0, 15);
	m_tbPallete.SetMinMax(0, 7);
	m_tbEnemyPallete.SetMinMax(0, 3);

	m_cbMusic.AddString(TEXT("Normal"), 0);
	m_cbMusic.AddString(TEXT("Underground"), 1);
	m_cbMusic.AddString(TEXT("Level boss"), 2);
	m_cbMusic.AddString(TEXT("Wart room"), 3);

	m_tbLength.Pos( m_pvEditor[nlfLength] + 1 );
	m_tbBgSet.Pos( m_pvEditor[nlfGround] );
	m_tbBgType.Pos( m_pvEditor[nlfTypeOfGround]);
	m_tbObjType.Pos( m_pvEditor[nlfObjectType]);
	m_tbPallete.Pos( m_pvEditor[nlfColor]);
	m_tbEnemyPallete.Pos( m_pvEditor[nlfEnemyColor]);

	m_cbMusic.SetCurSel( m_pvEditor[nlfMusic]);
	
	CheckAll(m_pvEditor[nlfDirection], IDC_LVLDIRECTION, 0);

	// init level sample
	LoadLevelSample();
}

INT CLevelInfoDlg::OnMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & pHandle)
{
	if ( WM_HSCROLL == uMsg && lParam )
	{
		NMHDR nm = { 0 };
		nm.code = WM_HSCROLL;
		nm.hwndFrom = HWND(lParam);
		nm.idFrom = GetWindowLong(HWND(lParam), GWL_ID);
		OnTrackbar(&nm);
	}

	return 0;
}

INT_PTR CLevelInfoDlg::OnTrackbar(LPNMHDR lpnm)
{
	if ( 
		IDC_SLIDER2 == lpnm->idFrom ||
		IDC_SLIDER3 == lpnm->idFrom ||
		IDC_SLIDER4 == lpnm->idFrom ||
		IDC_SLIDER5 == lpnm->idFrom ||
		IDC_SLIDER6 == lpnm->idFrom 
		)
	{

		NES_LEVEL_FIELDS nlf = nlfLength;
		switch(lpnm->idFrom)
		{
		case IDC_SLIDER2: nlf = nlfGround; break;
		case IDC_SLIDER3: nlf = nlfTypeOfGround; break;
		case IDC_SLIDER4: nlf = nlfObjectType; break;
		case IDC_SLIDER5: nlf = nlfColor; break;
		case IDC_SLIDER6: nlf = nlfEnemyColor; break;
		}

		CTrackBar tb; tb.AssignWindow(lpnm->hwndFrom);
		
		if ( tb.Pos() != m_vEditor[nlf] )
		{
			m_vEditor.SetHeaderInfo(nlf, tb.Pos());
			m_pLevel->DrawLevelEx(m_dwCurLevel, &m_vEditor);
			InvalidateRect(Ctl(IDC_LEVEL), NULL, FALSE);
		}
	}
	return 0;
}


VOID CLevelInfoDlg::OnButton(USHORT uId)
{
	if ( IDC_LVLDIRECTION == uId )
	{
		m_vEditor.SetHeaderInfo(nlfDirection, ( BST_CHECKED == dSendMessage(IDC_LVLDIRECTION, BM_GETCHECK) ? 1 : 0 ) );
		m_pLevel->DrawLevelEx(m_dwCurLevel, &m_vEditor);
		InvalidateRect(Ctl(IDC_LEVEL), NULL, FALSE);
	}
}


VOID CLevelInfoDlg::OnOK()
{
	m_pvEditor.SetHeaderInfo(nlfLength, m_tbLength.Pos()-1);
	m_pvEditor.SetHeaderInfo(nlfGround, m_tbBgSet.Pos());
	m_pvEditor.SetHeaderInfo(nlfTypeOfGround, m_tbBgType.Pos());
	m_pvEditor.SetHeaderInfo(nlfMusic, (BYTE) m_cbMusic.GetParam(m_cbMusic.GetCurSel()));
	m_pvEditor.SetHeaderInfo(nlfObjectType, m_tbObjType.Pos());
	m_pvEditor.SetHeaderInfo(nlfDirection, dSendMessage(IDC_LVLDIRECTION, BM_GETCHECK));
	m_pvEditor.SetHeaderInfo(nlfColor, m_tbPallete.Pos());
	m_pvEditor.SetHeaderInfo(nlfEnemyColor, m_tbEnemyPallete.Pos());

	Close(127);
}

VOID CLevelInfoDlg::LoadLevelSample()
{
	m_vEditor.SetHeaderInfo(nlfGround, m_pvEditor[nlfGround] );
	m_vEditor.SetHeaderInfo(nlfTypeOfGround, m_pvEditor[nlfTypeOfGround]);
	m_vEditor.SetHeaderInfo(nlfObjectType, m_pvEditor[nlfObjectType]);
	m_vEditor.SetHeaderInfo(nlfColor, m_pvEditor[nlfColor]);
	m_vEditor.SetHeaderInfo(nlfEnemyColor, m_pvEditor[nlfEnemyColor]);
	m_vEditor.SetHeaderInfo(nlfLength, 0);
	m_vEditor.SetHeaderInfo(nlfDirection, m_pvEditor[nlfDirection]);

	m_vEditor.InsertObject(0, 2, 2, 0x32);
	m_vEditor.InsertObject(0, 5, 2, 0x42);
	m_vEditor.InsertObject(0, 8, 2, 0x84);
	m_vEditor.InsertObject(0, 3, 3, 0xa3);
	m_vEditor.InsertObject(0, 5, 3, 0xf2);
	m_vEditor.InsertEnemy(2, 1, 0x1, FALSE);
	m_vEditor.InsertEnemy(5, 1, 0x2, FALSE);
	m_vEditor.InsertEnemy(7, 1, 0x3, FALSE);

	m_vEditor.Apply();

	m_pLevel->DrawLevelEx(m_dwCurLevel, &m_vEditor);
}