
#include "precomp.h"

CMainWnd::CMainWnd(HINSTANCE hInstance) 
	: CDialog(hInstance, MAKEINTRESOURCE(IDD_MAINWINDOW)),
	  dlvl(m_File, hInstance), dlvloptions(m_File, hInstance)
{
	 

	AddAnchor(IDC_MENU, ANCHOR_TOP);
	AddAnchor(IDC_LEVELS, ANCHOR_LEFT);
	AddAnchor(IDC_PAGES, ANCHOR_FULL);
	AddAnchor(IDC_STATUSBAR, ANCHOR_BOTTOM);
	MainWindow();
}

void CMainWnd::OnCommand(USHORT uCmd, USHORT uId, HWND hCtl)
{
	if ( 1 == uCmd )
		OnButton(uId);
}

VOID CMainWnd::ListLevels()
{
	m_ctl.tv.ResetContent();

	HTREEITEM hWorlds[ MAX_LEVELS / 30 ] = { 0 };
	HTREEITEM hLevels[ MAX_LEVELS / 30 ][ MAX_LEVELS / 10 ] = { 0 };
	HTREEITEM hSelected = NULL;

	for(int l = 0; l < MAX_LEVELS / 30; ++l)
	{
		CString sWorld;
		sWorld.Format(TEXT("World %d"), l + 1);
		hWorlds[l] = m_ctl.tv.AddItem(NULL, sWorld, -MAKEWORD(0, 30*l), TRUE, FALSE);
	}

	for(int l = 0; l < MAX_LEVELS; ++l)
	{
		int world = m_File.getLevelEditor(l).GetCurWorld();
		int sublevel = m_File.getLevelEditor(l).GetSubLevel();

		if ( -1 == world || -1 == sublevel )
			continue;

		if ( !hLevels[world][sublevel] )
		{
			CString sLevel;
			sLevel.Format(TEXT("Level %d"), sublevel + 1);
			hLevels[world][sublevel] = m_ctl.tv.AddItem(hWorlds[world], sLevel, -l, FALSE, FALSE, FALSE);
		}

		HTREEITEM hItem = m_ctl.tv.AddItem(hLevels[world][sublevel], g_szLevelName[l], l, FALSE);
		if ( !hSelected )
			hSelected = hItem;
	}

	m_ctl.tv.SetCurSel(hSelected);
}


void CMainWnd::OnInit(LPARAM lParam)
{
	m_ctl.tv.AssignWindow(Ctl(IDC_LEVELS));
	m_ctl.tc.AssignWindow(Ctl(IDC_PAGES));
	m_ctl.tb.AssignWindow(Ctl(IDC_TOOLBAR));

	dlvl.Create(this, (LPARAM) &m_File);
	dlvloptions.Create(this, (LPARAM) &m_File);
	INT parts[3];
	parts[0] = 500; parts[1] = 800; parts[2] = -1;
	dSendMessage(IDC_STATUSBAR, SB_SETPARTS, 3, (LPARAM) &parts[0]);
	dSendMessage(IDC_STATUSBAR, SB_SETTEXT, 0, 0);
	dSendMessage(IDC_STATUSBAR, SB_SETTEXT, 1, 0);
	dSendMessage(IDC_STATUSBAR, SB_SETTEXT, 2, 0);


	HMENU hMenu = LoadMenu(GetInstance(), MAKEINTRESOURCE(IDR_MENU1));

	SetMenu(GetWindowHandle(), hMenu);

	HIMAGELIST himl = ImageList_Create(16, 16, ILC_COLOR32, 32, 1);
	HBITMAP hbmp = LoadBitmap(g_hResInstance, MAKEINTRESOURCE(IDB_MENU));
	ImageList_Add(himl, hbmp, hbmp);

	m_ctl.tb.cSendMessage(TB_SETIMAGELIST, 0, (LPARAM) himl);
	m_ctl.tb.AddButton(13, ID_SAVE, 0, 0, FALSE);
	m_ctl.tb.AddButton(12, ID_OPEN);
	m_ctl.tb.AddSeparator();
	m_ctl.tb.AddButton(3, ID_ADVANCED, 0, 0, FALSE);
	m_ctl.tb.AddButton(23, ID_OBJ_LIST, 0, 0, FALSE);
	m_ctl.tb.AddButton(23, ID_CUROBJLIST, 0, 0, FALSE);
	m_ctl.tb.AddButton(15, ID_APPLYCHANGES, 0, 0, FALSE);
	m_ctl.tb.AddButton(9, IDC_ROTATELEVEL, 0, 0, FALSE);
	m_ctl.tb.AddButton(10, ID_FLUSHLEVEL, 0, 0, FALSE);
	m_ctl.tb.AddSeparator();
	m_ctl.tb.AddButton(0, ID_EXIT);

	// vm_ctl.rb.InsertBand(m_ctl.tb);
	// m_ctl.rb.InsertBand(m_ctl.tb);

	m_ctl.tc.AddItem(TEXT("Level"));
	m_ctl.tc.AddItem(TEXT("Options"));
	m_ctl.tc.Adjust(dlvl.GetWindowHandle());
	m_ctl.tc.Adjust(dlvloptions.GetWindowHandle());
	SetPage(0);

	m_ctl.tv.Enable(TRUE);
	m_ctl.tv.AssignControl(GetWindowHandle(), IDC_LEVELS);

	m_uCurLevel = 0;
	
	m_ctl.tv.AddItem(NULL, TEXT("Game not loaded"), -1, TRUE);

	UpdatePages();
}

void CMainWnd::OnButton(USHORT uId)
{
	switch(uId)
	{
	case ID_EXIT: Close(0); break;
	case ID_OPEN:
		{
			if ( m_File.IsLoaded() )
				break;

			do
			{
				CString sFile = OpenFile(TEXT("smb2.nes"), TEXT("Nes files"), TEXT("*.nes"), 0);
				if ( !lstrlen(sFile.GetBuffer()) )
					break;

				int err = m_File.LoadFile(sFile);

				if ( !err )
				{
					err = m_File.Read();
					if ( !err ) 
						break;
					else
						ShowErrorF(TEXT("Invalid ROM file. Try to load other"));
				}
				else
					ShowErrorF(TEXT("Failed to load file '%s'.\nFile is inaccessible or corrupt.\n\nLoad error: %d"), sFile, err);
			} while(1);

			if ( m_File.IsLoaded() )
			{
				ListLevels();
				m_ctl.tb.EnableAll(FALSE, ID_OPEN, 0);
				EnableMenuItem( GetSubMenu( GetMenu( *this ), 0 ), ID_OPEN, MF_DISABLED | MF_BYCOMMAND );
				m_ctl.tb.EnableAll(TRUE, ID_SAVE, ID_OBJ_LIST, ID_CUROBJLIST, ID_APPLYCHANGES, ID_FLUSHLEVEL, ID_LEVELHEADER, ID_ADVANCED, IDC_ROTATELEVEL, ID_WORLDS, 0);

				EnableMenuItem( GetSubMenu( GetMenu( *this ), 1 ), ID_OBJ_LIST, MF_ENABLED | MF_BYCOMMAND );
				EnableMenuItem( GetSubMenu( GetMenu( *this ), 1 ), ID_CUROBJLIST, MF_ENABLED | MF_BYCOMMAND );
				EnableMenuItem( GetSubMenu( GetMenu( *this ), 1 ), ID_APPLYCHANGES, MF_ENABLED | MF_BYCOMMAND );
				EnableMenuItem( GetSubMenu( GetMenu( *this ), 1 ), ID_FLUSHLEVEL, MF_ENABLED | MF_BYCOMMAND );
				EnableMenuItem( GetSubMenu( GetMenu( *this ), 1 ), ID_LEVELHEADER, MF_ENABLED | MF_BYCOMMAND );
				
				EnableMenuItem( GetSubMenu( GetSubMenu( GetMenu( *this ), 1 ), 0 ), ID_ADVANCED, MF_ENABLED | MF_BYCOMMAND );
				EnableMenuItem( GetSubMenu( GetSubMenu( GetMenu( *this ), 1 ), 0 ), ID_WORLDS, MF_ENABLED | MF_BYCOMMAND );

				ShowStat();
				UpdatePages();
			}

			break;
		}
	case ID_SAVEAS:
	case ID_SAVE:
		{
			if ( !m_File.IsLoaded() )
				break;
			
			int err = 0;
			if ( ( err = m_File.Write()) != 0 )
			{
				ShowErrorF(TEXT("Failed to store level data to ROM file\n")
							TEXT("Possilbe out of memory or internal editor error\n")
							TEXT("Try to delete some objects and try again\n\n")
							TEXT("Write error: %d"), err);
				break;
			}

			do
			{
				if ( ID_SAVEAS == uId )
				{
					CString sFile = SaveFile(TEXT("smb2.nes"), TEXT("Nes files"), TEXT("*.nes"), 0);
					if ( !lstrlen(sFile.GetBuffer()) )
						break;

					err = m_File.SaveFile(sFile);
				}
				else
					err = m_File.SaveFile();

				if ( err )
				{
					ShowError(TEXT("Unable to save the file\n")
								TEXT("It may be not enough space on the disk or access error\n"));
					uId = ID_SAVEAS;
				}
				else
					break;
			} while(1);

			break;
		}
	case ID_ABOUT:
		{
			ShowMessage(TEXT("SMB Editor attempt Open Source _PUBLIC VERSION_ 0.1"));
			break;
		}
	case ID_OBJ_LIST:
		{
			if ( !m_File.IsLoaded() ) break;

			CLvlObjDlg dlg(GetInstance(), m_File.getLevelEditor(m_uCurLevel));
			dlg.Show(this);
			break;
		}
	case ID_CUROBJLIST:
		{
			if ( !m_File.IsLoaded() ) break;

			CLvlObjDlg dlg(GetInstance(), m_File.getLevelEditor(m_uCurLevel));
			std::vector<NES_LEVEL_ITEM> pItems;
			DWORD dwCount = 0, dwLevelHeader = 0;

			if ( m_File.getLevelEditor(m_uCurLevel).GetCurrentLevelItems(pItems) )
			{
				dlg.CurrentList(pItems, dwCount);
				dlg.Show(this);
			}
			break;
		}
	case ID_APPLYCHANGES:
		{
			if ( !m_File.IsLoaded() ) break;

			std::vector<NES_LEVEL_ITEM> pItems;
			DWORD dwCount = 0, dwLevelHeader = 0;
			std::vector<NES_LEVEL_ENEMY> pEnemies;
			DWORD dwCountEnemies = 0;
			if ( !m_File.getLevelEditor(m_uCurLevel).Apply() )
				ShowError(TEXT("Failed to apply changes"));
			else
				ShowStat();

			break;
		}
	case ID_FLUSHLEVEL:
		{
			if ( !m_File.IsLoaded() ) break;

			if ( Confirm(TEXT("This is delete ALL objects from the level!!!\nAre you sure to continue?")) )
			{
				m_File.getLevelEditor(m_uCurLevel).RemoveAll();
				UpdatePages();
			}
			break;
		}
	case ID_LEVELHEADER:
		{
			if ( !m_File.IsLoaded() ) break;

			CNesLevel lvlEmpty;
			CNesEnemyData enmEmpty;

			CLevelInfoDlg dlg(GetInstance(), m_File.getLevelEditor(m_uCurLevel), m_uCurLevel, m_File,
				lvlEmpty, enmEmpty);

			if ( 127 == dlg.Show(this) )
				UpdatePages();

			break;
		}
	case ID_ADVANCED:
		{
			if ( !m_File.IsLoaded() ) break;
			BOOL fShiftPressed = ( ( GetKeyState( VK_SHIFT ) & 0x8000 ) != 0 );

			CBgTilesDlg dlgBgTiles(GetInstance(), m_File);
			CWrldItrDlg dlgWInterior(GetInstance(), m_File);
			CObjTilesDlg dlgObjTiles(GetInstance(), m_File);
			CBgSetHorzDlg dlgHorzSet(GetInstance(), m_File, TRUE);
			CBgSetHorzDlg dlgVertSet(GetInstance(), m_File, FALSE);
			CPaletteDlg dlgPal(GetInstance(), m_File);
			CWarpsDlg dlgWarps(GetInstance(), m_File);
			CMiscDataDlg dlgMisc(GetInstance(), m_File);

			CPropSheet dlgSheets(GetInstance());
			dlgSheets.FApply(FALSE);
			dlgSheets.AddSheet(&dlgBgTiles, 0, TEXT("Ground type"));
			dlgSheets.AddSheet(&dlgHorzSet, 0, TEXT("Horz ground set"));
			dlgSheets.AddSheet(&dlgVertSet, 0, TEXT("Vert ground set"));
			dlgSheets.AddSheet(&dlgObjTiles, 0, TEXT("Objects settings"));
			dlgSheets.AddSheet(&dlgWInterior, 0, TEXT("Worlds interiors"));
			dlgSheets.AddSheet(&dlgPal, 0, TEXT("Palette sets"));
			dlgSheets.AddSheet(&dlgWarps, 0, TEXT("Warp zones"));

			if ( fShiftPressed )
				dlgSheets.AddSheet(&dlgMisc, 0, TEXT("Misc"));

			if ( dlgSheets.Show(this, 0, TEXT("Advanced settings")) )
				UpdatePages();
			break;
		}
	case ID_WORLDS:
		{
			CWorldsLevelsDlg w1(GetInstance(), m_File);
			CWorldsDataDlg w2(GetInstance(), m_File);

			CPropSheet pSheet(GetInstance());
			pSheet.FApply(FALSE);
			pSheet.AddSheet(&w1);
			pSheet.AddSheet(&w2);

			if ( pSheet.Show(this, 0, TEXT("Advanced worlds settings")) )
			{
				dlvl.CheckData(TRUE);
				for(int i = 0; i < MAX_LEVELS; ++i)
					m_File.getLevelEditor(i).UpdateCounters();
				
				dlvl.Visible(FALSE);
				ListLevels();
				dlvl.Visible(TRUE);
			}
			break;
		}
	case IDC_ROTATELEVEL:
		{
			if ( !m_File.IsLoaded() )
				break;

			SendMessage(dlvl, WM_COMMAND, MAKELONG(IDC_ROTATELEVEL, BN_CLICKED), 0);
			break;
		}
	}
}

VOID CMainWnd::AlignPage()
{
	INT iIndex = m_ctl.tc.GetCurSel();

	switch(iIndex)
	{
	case 0: dlvl.Visible(TRUE); dlvloptions.Visible(FALSE); break;
	case 1: dlvloptions.Visible(TRUE); dlvl.Visible(FALSE); break;
	}
}

VOID CMainWnd::SetPage(INT iIndex)
{
	if ( iIndex < 0 || iIndex >= m_ctl.tc.ItemsCount() )
		return;

	m_ctl.tc.SetCurSel(iIndex);
	AlignPage();
}

VOID CMainWnd::OnSizing(UINT fwSizeType, LPRECT lprc)
{
	OnSize(fwSizeType, (SHORT) (0xFFFF & lprc->right) , (SHORT) (0xFFFF & lprc->bottom) );
}

VOID CMainWnd::OnSize(UINT fwSizeType, SHORT cx, SHORT cy)
{
	HDWP hdwp = BeginDeferWindowPos(2);
	m_ctl.tc.DeferAdjust(hdwp, dlvl.GetWindowHandle());
	m_ctl.tc.DeferAdjust(hdwp, dlvloptions.GetWindowHandle());
	EndDeferWindowPos(hdwp);
	dSendMessage(IDC_STATUSBAR, WM_SIZE);
}

INT_PTR CMainWnd::OnNotify(LPNMHDR lpnm)
{
	switch(lpnm->idFrom)
	{
	case IDC_PAGES:
		{
			if ( TCN_SELCHANGE == lpnm->code )
				AlignPage();
			break;
		}
	case IDC_LEVELS:
		{
			if ( TVN_SELCHANGED == lpnm->code )
			{
				INT level = (INT) m_ctl.tv.Param( m_ctl.tv.GetCurSel() );
				if ( level >= 0 )
				{
					m_uCurLevel = level;
					UpdatePages();
				}
			}
			break;
		}
	case IDC_TOOLBAR:
		{
			if ( TBN_HOTITEMCHANGE == lpnm->code )
			{
				CString str;
				switch(LPNMTBHOTITEM(lpnm)->idNew)
				{
				case ID_OPEN: str = TEXT("Open a file..."); break;
				case ID_SAVE: str = TEXT("Save a file"); break;
				case ID_ADVANCED: str = TEXT("Advanced settings..."); break;
				case ID_APPLYCHANGES: str = TEXT("Apply changes in this level"); break;
				case ID_CUROBJLIST: str = TEXT("Shows modified objects list..."); break;
				case ID_OBJ_LIST: str = TEXT("Shows ROM objects list..."); break;
				case ID_FLUSHLEVEL: str = TEXT("Delete all objects from the level"); break;
				case IDC_ROTATELEVEL: str = TEXT("Rotate level"); break;
				case ID_EXIT: str = TEXT("Exit from editor"); break;
				}
				
				if ( str.GetLength() > 0 &&
					m_ctl.tb.Enabled(LPNMTBHOTITEM(lpnm)->idNew)
					&& 0 != ( 
						( HICF_MOUSE | HICF_LMOUSE | HICF_ARROWKEYS ) 
						& LPNMTBHOTITEM(lpnm)->dwFlags )  
					)
					dSendMessage(IDC_STATUSBAR, SB_SETTEXT, 0, (LPARAM) str.GetString());
				else
					dSendMessage(IDC_STATUSBAR, SB_SETTEXT, 0, 0);
			}
			break;
		}
	}
	return 0;
}

VOID CMainWnd::UpdatePages()
{
	dlvl.CheckData();
	dlvloptions.CheckData();
}

UINT CMainWnd::GetCurLevel()
{
	return m_uCurLevel;
}

BOOL CMainWnd::UnlockLevel()
{
	if ( Confirm(TEXT("Unlocking level causes grow up size of levels data.\nData from original level will be copied to current level.\nAre you sure that you want to do it?")) )
	{
		m_File.getLevelEditor(m_uCurLevel).SetItemsLayerLink(-1);
		return TRUE;
	}
	return FALSE;
}

BOOL CMainWnd::UnlockEnemy()
{
	if ( Confirm(TEXT("Unlocking enemies layer causes grow up size of enemies data.\nData from original layer will be copied to current layer.\nAre you sure that you want to do it?")) )
	{
		m_File.getLevelEditor(m_uCurLevel).SetEnemiesLayerLink(-1);
		return TRUE;
	}
	return FALSE;
}

void CMainWnd::SelectLevel(USHORT ptr, BYTE pos)
{
	int page = ( HIBYTE(ptr) & 0x0F ), vid = ( 10 * LOBYTE(ptr) + ( HIBYTE(ptr) >> 4 ) );
	HTREEITEM hItem = TreeView_GetRoot(m_ctl.tv);
	BOOL fFound = FALSE;

	while(hItem && !fFound )
	{
		HTREEITEM hChild = (HTREEITEM) m_ctl.tv.cSendMessage(TVM_GETNEXTITEM, TVGN_CHILD, (LPARAM) hItem);
		while(hChild && !fFound)
		{			
			HTREEITEM hChildSub = (HTREEITEM) m_ctl.tv.cSendMessage(TVM_GETNEXTITEM, TVGN_CHILD, (LPARAM) hChild);
			
			while(hChildSub && !fFound)
			{
				if ( m_ctl.tv.Param(hChildSub) == vid )
				{
					fFound = TRUE;
					m_ctl.tv.SetCurSel(hChildSub);
					break;
				}
				hChildSub = (HTREEITEM) m_ctl.tv.cSendMessage(TVM_GETNEXTITEM, TVGN_NEXT, (LPARAM) hChildSub);
			}

			hChild = (HTREEITEM) m_ctl.tv.cSendMessage(TVM_GETNEXTITEM, TVGN_NEXT, (LPARAM) hChild);
		}
		hItem = (HTREEITEM) m_ctl.tv.cSendMessage(TVM_GETNEXTITEM, TVGN_NEXT, (LPARAM) hItem);
	}

	dlvl.SelectPos(page, pos);
}

void CMainWnd::ShowStat()
{
	DWORD sd = 0, ed = 0, ld = 0, od = 0, td = 0;
	CString str = TEXT("");

	if ( m_File.GetMemoryStatus(sd, ld, ed, od, td) )
	{
		DWORD prc = 100 * od / td;
		str.Format(TEXT("Memory usage: %d%% [%d kB from %d kB], %d bytes free"),
			prc, od / 1024, td / 1024, td - od);

	}

	dSendMessage(IDC_STATUSBAR, SB_SETTEXT, 2, (LPARAM) str.GetString());
}