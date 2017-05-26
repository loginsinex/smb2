
#include "precomp.h"

CNewItemDlg::CNewItemDlg(HINSTANCE hInstance, CNesEditor * pvEditor, DWORD iItem)
	: CDialog(hInstance, MAKEINTRESOURCE(IDD_INSERTITEM)), 
		m_dlgEntrancePtr(hInstance, FALSE),
		m_dlgGroundSet(hInstance),
		m_dlgItemSize(hInstance),
		m_dlgPointer(hInstance),
		m_fEditMode( TRUE )
{
	m_pDlg[0] = &m_dlgEntrancePtr;
	m_pDlg[1] = &m_dlgGroundSet;
	m_pDlg[2] = &m_dlgItemSize;
	m_pDlg[3] = &m_dlgPointer;
	m_Item.dwItem = iItem;
	m_Item.pvEditor = pvEditor;
	ZeroMemory(&m_NewItem, sizeof(m_NewItem));
}


CNewItemDlg::CNewItemDlg(HINSTANCE hInstance) :
			CDialog(hInstance, MAKEINTRESOURCE(IDD_INSERTITEM)),
				m_dlgEntrancePtr(hInstance, 1),
				m_dlgGroundSet(hInstance),
				m_dlgItemSize(hInstance),
				m_dlgPointer(hInstance, 2),
				m_fEditMode( FALSE )
{
	m_pDlg[0] = &m_dlgEntrancePtr;
	m_pDlg[1] = &m_dlgGroundSet;
	m_pDlg[2] = &m_dlgItemSize;
	m_pDlg[3] = &m_dlgPointer;

	ZeroMemory(&m_NewItem, sizeof(m_NewItem));
}

VOID CNewItemDlg::OnInit(LPARAM lParam)
{
	for(int i = 0; i < sizeof(m_pDlg)/sizeof(m_pDlg[0]); ++i)
		m_pDlg[i]->Create(this);

	dSendMessage(IDC_TYPES, LB_ADDSTRING, 0, (LPARAM) TEXT("Regular object"));
	dSendMessage(IDC_TYPES, LB_ADDSTRING, 0, (LPARAM) TEXT("Massive object"));
	dSendMessage(IDC_TYPES, LB_ADDSTRING, 0, (LPARAM) TEXT("Entrance"));
	dSendMessage(IDC_TYPES, LB_ADDSTRING, 0, (LPARAM) TEXT("Ground set"));
	dSendMessage(IDC_TYPES, LB_ADDSTRING, 0, (LPARAM) TEXT("Pointer"));
	dSendMessage(IDC_TYPES, LB_ADDSTRING, 0, (LPARAM) TEXT("Enemy"));

	if ( m_fEditMode )
	{
		CNesItem nItem = m_Item.pvEditor->LevelItem(m_Item.dwItem);
		UINT vid = 0;

		switch(nItem.Type())
		{
		case nitRegular: vid = (nItem.Regular() < 0x30 ? 0 : 1 ); break;
		case nitEntrance: vid = 2; break; 
		case nitGround: vid = 3; break; 
		case nitPointer: vid = 4; break; 
		case nitEnemy: vid = 5; break; 
		}
		dSendMessage(IDC_TYPES, LB_SETCURSEL, vid);
		OnCommand(LBN_SELCHANGE, IDC_TYPES, Ctl(IDC_TYPES));
		EnableAll(FALSE, IDC_TYPES, IDC_NAME, 0);
	}
	else if ( lParam )	// inserting jar enemy
	{
		dSendMessage(IDC_TYPES, LB_SETCURSEL, 5);
		OnCommand(LBN_SELCHANGE, IDC_TYPES, Ctl(IDC_TYPES));
		EnableAll(FALSE, IDC_TYPES, 0);
	}
}

VOID CNewItemDlg::OnCommand(USHORT uCmd, USHORT uId, HWND hWnd)
{
	if ( IDC_TYPES == uId && LBN_SELCHANGE == uCmd )
	{
		UINT id = dSendMessage(IDC_TYPES, LB_GETCURSEL);
		if ( -1 != id )
		{
			CComboBox cb;
			cb.AssignWindow(Ctl(IDC_NAME));
			UINT uTotal = 0, uSetCurSel = -1;
			BYTE ids[256] = { 0 };
			INT iPropPage = -1;

			switch(id)
			{
			case 0:
				{
					BYTE x[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 12, 13, 14, 15, 16, 17, 18, 22, 23, 24, 25, 26, 30, 31,
								32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47 };
					uTotal = sizeof(x)/sizeof(x[0]);
					CopyMemory(ids, x, uTotal);
					iPropPage = (-1);
					if ( m_fEditMode ) uSetCurSel = m_Item.pvEditor->LevelItem(m_Item.dwItem).Regular();
					break;
				}
			case 1:
				{
					BYTE x[] = { 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60 };
					uTotal = sizeof(x)/sizeof(x[0]);
					CopyMemory(ids, x, uTotal);
					iPropPage = (2);
					if ( m_fEditMode )
					{
						uSetCurSel = m_Item.pvEditor->LevelItem(m_Item.dwItem).Regular();
						m_dlgItemSize.SetSizeParam(uSetCurSel % 0x10);
						uSetCurSel = ( uSetCurSel >= 0x30 ? 0x30 + (uSetCurSel-0x30)/0x10 : -1 );
					}
					break;
				}
			case 2:
				{
					BYTE x[] = { 9, 10, 11, 19, 20, 21, 28, 29, 30 };
					uTotal = sizeof(x)/sizeof(x[0]);
					CopyMemory(ids, x, uTotal);
					iPropPage = (0);
					if ( m_fEditMode )
					{
						CNesItem nItem = m_Item.pvEditor->LevelItem(m_Item.dwItem);
						uSetCurSel = nItem.Regular();
						m_dlgEntrancePtr.SetPointerData(
							nItem.EntranceParameter(0) == 0xf5,
							( 0xf5 == nItem.EntranceParameter(0) ? nItem.EntranceParameter(1) : nItem.EntranceParameter(0) ),
							( 0xf5 == nItem.EntranceParameter(0) ? nItem.EntranceParameter(2) : nItem.EntranceParameter(1) )
							);
					}
					break;
				}
			case 3:
				{
					iPropPage = (1);
					if ( m_fEditMode )
					{
						CNesItem nItem = m_Item.pvEditor->LevelItem(m_Item.dwItem);
						m_dlgGroundSet.GroundSet(nItem.Ground() & 0x1f);
						m_dlgGroundSet.GroundType(nItem.Ground() >> 0x8);
					}
					break;
				}
			case 4:
				{
					iPropPage = (3);
					if ( m_fEditMode )
					{
						CNesItem nItem = m_Item.pvEditor->LevelItem(m_Item.dwItem);
						m_dlgPointer.SetPointerData(TRUE, nItem.Pointer() & 0xFF, nItem.Pointer() >> 0x8);
					}
					break;
				}
			case 5:
				{
					if ( m_fEditMode ) uSetCurSel = m_Item.pvEditor->LevelItem(m_Item.dwItem).Enemy();
					iPropPage = (-1);
					break;
				}
			}

			cb.cSendMessage(CB_RESETCONTENT);

			if ( id < 3 )
			{
				EnableAll(TRUE, IDC_NAME, 0);
				for(UINT i = 0; i < uTotal; ++i)
				{
					( uSetCurSel == ids[i] ? cb.SetCurSel( cb.AddString(g_szItemName[ids[i]], ids[i]) ) : cb.AddString(g_szItemName[ids[i]], ids[i]) );
				}

				if ( !m_fEditMode ) cb.SetCurSel(0);
			}
			else if ( id >=3 && id < 5 )
			{
				EnableAll(FALSE, IDC_NAME, 0);
			}
			else if ( 5 == id )
			{
				EnableAll(TRUE, IDC_NAME, 0);
				for(UINT i = 0; i < 128; ++i) 
				{
					if ( i<75 || i>91) 
						( uSetCurSel == i ?  cb.SetCurSel(cb.AddString(g_pszEnemies[i], i)) : cb.AddString(g_pszEnemies[i], i) );
				}
				if ( !m_fEditMode ) cb.SetCurSel(0);
			}

			ShowProp(iPropPage);
		}
	}
}

VOID CNewItemDlg::ShowProp(INT i)
{
	for(INT k = 0; k < sizeof(m_pDlg)/sizeof(m_pDlg[0]); ++k)
		m_pDlg[k]->Visible(i==k);

	if ( i >= 0 && i < sizeof(m_pDlg)/sizeof(m_pDlg[0]) )
	{
		RECT rc = { 0 };
		GetWindowRect(Ctl(IDC_SIZEPTR), &rc);
		ScreenToClient(GetWindowHandle(), (LPPOINT) (&rc));
		ScreenToClient(GetWindowHandle(), (LPPOINT) (&rc)+1);
		SetWindowPos(m_pDlg[i]->GetWindowHandle(), NULL, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_FRAMECHANGED | SWP_NOREPOSITION);
	}

}

VOID CNewItemDlg::OnOK()
{
	CComboBox cb;
	cb.AssignWindow(Ctl(IDC_NAME));
	UINT id = dSendMessage(IDC_TYPES, LB_GETCURSEL), vid = cb.GetParam(cb.GetCurSel());
	
	m_NewItem.fEnemy = ( 5 == id );

	switch(id)
	{
	case 0: // regular
		{
			m_NewItem.nItem.nli = nliRegular;
			m_NewItem.nItem.Object.Regular.id = vid;
			break;
		}
	case 1: // massive object
		{
			m_NewItem.nItem.nli = nliRegular;
			m_NewItem.nItem.Object.Regular.id = 0x30+0x10*(vid-0x30)+(m_dlgItemSize.GetSizeParam() & 0x0F);
			break;
		}
	case 2: // entrance
		{
			m_NewItem.nItem.nli = nliEntrance;
			m_NewItem.nItem.Object.Entrance.uEntranceId = vid;

			if ( m_dlgEntrancePtr.IsFarPointer() )
			{
				m_NewItem.nItem.Object.Entrance.uEntranceParameter[0] = 0xf5;
				m_NewItem.nItem.Object.Entrance.uEntranceParameter[1] = m_dlgEntrancePtr.GetPointerParam(0);
				m_NewItem.nItem.Object.Entrance.uEntranceParameter[2] = m_dlgEntrancePtr.GetPointerParam(1);
			}
			else
			{
				m_NewItem.nItem.Object.Entrance.uEntranceParameter[0] = m_dlgEntrancePtr.GetPointerParam(0);
				m_NewItem.nItem.Object.Entrance.uEntranceParameter[1] = m_dlgEntrancePtr.GetPointerParam(1);
			}
			m_bRequestConnectedPtr = m_dlgEntrancePtr.GetRequestedPtr();
			break;
		}
	case 3: // ground
		{
			m_NewItem.nItem.nli = nliGroundSet;
			m_NewItem.nItem.Object.GroundSet.uSets = m_dlgGroundSet.GroundSet();
			m_NewItem.bGroundType = m_dlgGroundSet.GroundType();
			break;
		}
	case 4: // pointer
		{
			m_NewItem.nItem.nli = nliPtr;
			m_NewItem.nItem.Object.Pointer.uParameter[0] = m_dlgPointer.GetPointerParam(0);
			m_NewItem.nItem.Object.Pointer.uParameter[1] = m_dlgPointer.GetPointerParam(1);
			m_bRequestConnectedPtr = m_dlgPointer.GetRequestedPtr();
			break;
		}
	case 5: // enemy
		{
			m_NewItem.nEnemy.Enemy.Object.id = vid;
			break;
		}
	}
	Close(1);
}

VOID CNewItemDlg::OnCancel()
{
	Close(0);
}

BOOL CNewItemDlg::GetData(PNES_LEVEL_ITEM pItem, PNES_LEVEL_ENEMY pEnemy, PBYTE pbGroundType, PBOOL pfEnemy)
{
	*pfEnemy = m_NewItem.fEnemy;
	*pbGroundType = m_NewItem.bGroundType;
	*pEnemy = m_NewItem.nEnemy;
	*pItem = m_NewItem.nItem;
	return TRUE;
}

BYTE CNewItemDlg::GetDataEx()
{
	return m_bRequestConnectedPtr;
}