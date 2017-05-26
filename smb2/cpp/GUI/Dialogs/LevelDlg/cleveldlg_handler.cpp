
#include "precomp.h"

INT_PTR CLevelDlg::OnNotify(LPNMHDR lpnm)
{
	if ( IDC_LEVEL == lpnm->idFrom )
	{
		PNMLVLDRWHT p = (PNMLVLDRWHT) lpnm;
		CNesEditor & ed = m_File.getLevelEditor(((CMainWnd*) Parent())->GetCurLevel());
		CString sStr;
		CMainWnd * pParent = ((CMainWnd*) Parent());

		switch(lpnm->code)
		{
			case LDN_CHANGEBG:		return OnNotify_ChangeBg(ed, p, pParent);
			case LDN_HITEST:		return OnNotify_HitTest(ed, p, pParent);
			case LDN_HITESTENEMY:	return OnNotify_HitTestEnemy(ed, p, pParent);
			case LDN_INSERTJARENEMY:
			case LDN_INSERT:		return OnNotify_Insert(ed, p, pParent, LDN_INSERTJARENEMY == lpnm->code);
			case LDN_EDIT:			return OnNotify_Edit(ed, p, pParent);
			case LDN_JUMPTOPOINTER:	return OnNotify_JumpPointer(ed, p, pParent);
			case LDN_DELETE:		return OnNotify_Delete(ed, p, pParent);
			case LDN_CHANGELAYER:	return OnNotify_ChangeLayer(ed, p, pParent);
			case LDN_LEVELDATA:		return OnNotify_LevelData(ed, p, pParent);
			case LDN_MAKEJAR:		return OnNotify_MakeJar(ed, p, pParent);
			case LDN_ITEMSELECTED:	return OnNotify_ItemSelected(ed, p, pParent);
		}
	}
	else if ( ( IDC_EDITITEM == lpnm->idFrom || IDC_DELITEM == lpnm->idFrom ) && ( lpnm->code == NM_CLICK || lpnm->code == NM_RETURN) )
	{
		OnButton( lpnm->idFrom );
	}
	return 0;
}

BOOL CLevelDlg::OnNotify_ChangeBg(CNesEditor & ed, PNMLVLDRWHT p, CMainWnd * pParent)
{
	if ( !p->iItem )
		return FALSE;

	CString sStr;
	CNesItem x = ed.LevelItem(p->iItem);
	if ( x.Type() == nitGround )
	{
		BYTE uSets = x.Ground() & 0x1F, uType = (x.Ground() >> 8) & 0x07;
		
		if ( !p->pos.y )
		{
			if ( 1 == p->pos.x )
				uSets = ( uSets < 0x1F ? uSets + 1 : 0 );
			else
			{
				uSets = ( uSets > 0 ? uSets - 1 : 0x1F );
			}
		}
		else
		{
			if ( 1 == p->pos.x )
				uType = ( uType < 0x07 ? uType + 1 : 0 );
			else
			{
				uType = ( uType > 0 ? uType - 1 : 0x07 );		
			}
		}


		m_File.getLevelEditor(pParent->GetCurLevel()).ChangeGround(p->iItem, uSets, uType);
					
		x = ed.LevelItem(p->iItem);
		sStr.Format(TEXT("GroundSet: 0x%02X, 0x%04X"), x.Pos().x, x.Ground());
		pParent->dSendMessage(IDC_STATUSBAR, SB_SETTEXT, 0, (LPARAM) sStr.GetBuffer());
		return TRUE;
	}
	else if ( x.Type() == nitRegular && x.Regular() >= 0x30 )
	{
		BYTE bSize = ( x.Regular() & 0x0F );

		if ( 1 == p->pos.x )
			bSize++;
		else
			bSize--;

		if ( bSize <= 0x0F )
		{
			ed.ChangeRegularSize(p->iItem, bSize);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CLevelDlg::OnNotify_HitTest(CNesEditor & ed, PNMLVLDRWHT p, CMainWnd * pParent)
{
	CString sStr;

	if ( 0xFFFF == p->iItem )
		sStr.Format(TEXT("No items at this point: %d:%d"), p->pos.x, p->pos.y);
	else
	{
		CNesItem x = ed.LevelItem(p->iItem);
		if ( x.Type() == nitEntrance || x.Type() == nitRegular )
			sStr.Format(TEXT("Item: 0x%02X (%s) [%d:%d p%d], %d:%d, Layer: %d"), x.Regular(), g_szItemName[x.RegularId()], x.Pos().x, x.Pos().y, x.Page(), p->pos.x, p->pos.y, x.Layer());
		else if ( x.Type() == nitGround )
			sStr.Format(TEXT("GroundSet: 0x%02X, 0x%04X"), x.Pos().x, x.Ground());

	}
	pParent->dSendMessage(IDC_STATUSBAR, SB_SETTEXT, 0, (LPARAM) sStr.GetBuffer());
	return FALSE;
}

BOOL CLevelDlg::OnNotify_HitTestEnemy(CNesEditor & ed, PNMLVLDRWHT p, CMainWnd * pParent)
{
	CString sStr;
	if ( 0xFFFF == p->iItem )
		sStr.Format(TEXT("No item at this point: %d:%d"), p->pos.x, p->pos.y);
	else
	{
		CNesItem x = ed.LevelItem(p->iItem);
		if ( x.Type() == nitEnemy )
			sStr.Format(TEXT("Item: 0x%02X (%s) [%d:%d p%d] %d:%d"), x.Enemy(), g_pszEnemies[x.Enemy()], x.Pos().x, x.Pos().y, x.Page(), p->pos.x, p->pos.y, x.Layer());
	}

	pParent->dSendMessage(IDC_STATUSBAR, SB_SETTEXT, 1, (LPARAM) sStr.GetBuffer());

	return FALSE;
}

BOOL CLevelDlg::OnNotify_Insert(CNesEditor & ed, PNMLVLDRWHT p, CMainWnd * pParent, BOOL fInsertJarEnemy)
{
	CNewItemDlg dlg(GetInstance());
	if ( dlg.Show(this, fInsertJarEnemy) )
	{
		NES_LEVEL_ITEM item; NES_LEVEL_ENEMY enemy; BOOL fEnemy = FALSE; BYTE bGroundType = 0, bRequestedPtr = 0;
		ZeroMemory(&item, sizeof(item)); ZeroMemory(&enemy, sizeof(enemy));
		dlg.GetData(&item, &enemy, &bGroundType, &fEnemy);
		bRequestedPtr = dlg.GetDataEx();

		if ( fEnemy )
		{
			m_File.getLevelEditor(pParent->GetCurLevel()).InsertEnemy(p->pos.x, p->pos.y, enemy.Enemy.Object.id, fInsertJarEnemy);
		}
		else
		{
			switch(item.nli)
			{
			case nliRegular: 
				{
					m_File.getLevelEditor(pParent->GetCurLevel()).InsertObject(0, p->pos.x, p->pos.y, item.Object.Regular.id);
					break;
				}
			case nliEntrance:
				{
					if ( !m_File.getLevelEditor(pParent->GetCurLevel()).InsertEntrance(
						0, p->pos.x, p->pos.y, item.Object.Entrance.uEntranceId,
										( 0xf5 == item.Object.Entrance.uEntranceParameter[0] ),
										( 0xf5 == item.Object.Entrance.uEntranceParameter[0] ? item.Object.Entrance.uEntranceParameter[1] : item.Object.Entrance.uEntranceParameter[0] ),
										( 0xf5 == item.Object.Entrance.uEntranceParameter[0] ? item.Object.Entrance.uEntranceParameter[2] : item.Object.Entrance.uEntranceParameter[1] )
						) )
					{
						ShowError(TEXT("This page is already have entrance or pointer.\nRemove existing object or insert entrance at another page"));
						return 0;
					}
					else if ( bRequestedPtr )
					{
						DWORD dwLevel = 0, dwMyLevel = pParent->GetCurLevel();
						INT page = 0, 
							my_page = 
								(
									m_File.getLevelEditor(pParent->GetCurLevel())[nlfDirection] ?
									p->pos.x / 0x10 : p->pos.y / 0x0F 
									);
						BOOL fFarPointer = FALSE;
						if ( 0xf5 == item.Object.Entrance.uEntranceParameter[0] )	// far pointer
						{
							dwLevel = 10 * item.Object.Entrance.uEntranceParameter[1] + ( item.Object.Entrance.uEntranceParameter[2] >> 4 );
							page = item.Object.Entrance.uEntranceParameter[2] & 0x0F;
							fFarPointer = TRUE;
						}
						else
						{
							dwLevel = 10 * item.Object.Entrance.uEntranceParameter[0] + ( item.Object.Entrance.uEntranceParameter[1] >> 4 );
							page = item.Object.Entrance.uEntranceParameter[1] & 0x0F;
						}

						if ( dwLevel < MAX_LEVELS )
						{
							CNesEditor & ed = m_File.getLevelEditor(dwLevel);
							if ( page < int(ed.CountPages()) )
							{
								POINT pt = { 15 - (p->pos.x % 16), p->pos.y % 15 };

								if ( ed[nlfDirection] )
									pt.x += page * 0x10;
								else
									pt.y += page * 0x0F;

								if ( !ed.InsertEntrance(0, pt.x, pt.y, 
										bRequestedPtr, fFarPointer, 
										LOBYTE(LOWORD(dwMyLevel / 10)), LOBYTE(LOWORD((( dwMyLevel % 10 ) << 4 ) | my_page)) )
										)
									ShowWarning(TEXT("Unable to create connected door.\nIt seems target point already have door or pointer"));
							}
						}
					}
					break;
				}
			case nliGroundSet:
				{
					m_File.getLevelEditor(pParent->GetCurLevel()).InsertGround(
						m_File.getLevelEditor(pParent->GetCurLevel())[nlfDirection] ?
						p->pos.x : p->pos.y, item.Object.GroundSet.uSets, bGroundType, 0
						);
					break;
				}
			case nliPtr:
				{
					BOOL fDirection = m_File.getLevelEditor(pParent->GetCurLevel())[nlfDirection];
					INT iPage = 
						( fDirection ? p->pos.x/0x10 : p->pos.y/0x0f );

					if ( !m_File.getLevelEditor(pParent->GetCurLevel()).
								InsertPointer(
								iPage, item.Object.Pointer.uParameter[0], item.Object.Pointer.uParameter[1]
							)
						)
					{
						ShowError(TEXT("This page is already have pointer or entrance.\nDelete existing pointer or entrance or place pointer to another page"));
					}
					else
					{
						if ( bRequestedPtr )
						{
							DWORD dwMyLevel = pParent->GetCurLevel();
							DWORD dwLevel = 10 * item.Object.Pointer.uParameter[0] + ( item.Object.Pointer.uParameter[1] >> 4 );
							INT my_page = item.Object.Pointer.uParameter[1] & 0x0F;

							if ( dwLevel < MAX_LEVELS )
							{
								if ( !m_File.getLevelEditor(dwLevel).InsertPointer(
									my_page, LOBYTE(LOWORD(dwMyLevel / 10)), LOBYTE(LOWORD(((dwMyLevel % 10) << 4) | iPage)))
									)
									ShowWarning(TEXT("Unable to create connected pointer.\nIt seems target point already have door or pointer"));
							}
						}

						return TRUE;
					}

					break;
				}
			}
		}
		return TRUE;
	}
	
	return FALSE;
}

BOOL CLevelDlg::OnNotify_Edit(CNesEditor & ed, PNMLVLDRWHT p, CMainWnd * pParent)
{
	CNewItemDlg dlg(GetInstance(), &m_File.getLevelEditor(pParent->GetCurLevel()), p->iItem);
	if ( dlg.Show(this) )
	{
		NES_LEVEL_ITEM item; NES_LEVEL_ENEMY enemy; BOOL fEnemy = FALSE; BYTE bGroundType = 0;
		ZeroMemory(&item, sizeof(item)); ZeroMemory(&enemy, sizeof(enemy));
		dlg.GetData(&item, &enemy, &bGroundType, &fEnemy);
		CNesItem nItem = m_File.getLevelEditor(pParent->GetCurLevel()).LevelItem(p->iItem);
		switch(nItem.Type())
		{
		case nitRegular:
			{
				if ( nItem.Regular() >= 0x30 )
					m_File.getLevelEditor(pParent->GetCurLevel()).ChangeRegularSize(p->iItem, item.Object.Regular.id);
				break;
			}
		case nitEntrance:
			{
				m_File.getLevelEditor(pParent->GetCurLevel()).ChangeEntrancePointer(
					p->iItem,
					item.Object.Entrance.uEntranceParameter[0] == 0xf5,
					item.Object.Entrance.uEntranceParameter[0] == 0xf5 ? item.Object.Entrance.uEntranceParameter[1] : item.Object.Entrance.uEntranceParameter[0],
					item.Object.Entrance.uEntranceParameter[0] == 0xf5 ? item.Object.Entrance.uEntranceParameter[2] : item.Object.Entrance.uEntranceParameter[1]
				);
				break;
			}
		case nitGround:
			{
				m_File.getLevelEditor(pParent->GetCurLevel()).ChangeGround(p->iItem, item.Object.GroundSet.uSets, bGroundType);
				break;
			}
		case nitPointer:
			{
				m_File.getLevelEditor(pParent->GetCurLevel()).ChangePointer(p->iItem, item.Object.Pointer.uParameter[0], item.Object.Pointer.uParameter[1]);
				break;
			}
		}

		return TRUE;
	}
	return FALSE;
}

BOOL CLevelDlg::OnNotify_JumpPointer(CNesEditor & ed, PNMLVLDRWHT p, CMainWnd * pParent)
{
	CNesItem x = ed.LevelItem(p->iItem);
	USHORT ptr = 0;
	BYTE pos = 0;
	switch(x.Type())
	{
	case nitEntrance:
		{
			if ( x.EntranceParameter(0) == 0xF5 )
				ptr = MAKEWORD(x.EntranceParameter(1), x.EntranceParameter(2));
			else
				ptr = MAKEWORD(x.EntranceParameter(0), x.EntranceParameter(1));

			POINT pt = x.Pos();
			pos = 15 - LOBYTE(LOWORD(pt.x % 16));
			break;
		}
	case nitPointer:
		{
			ptr = x.Pointer();
			pos = -1;
			break;
		}
	}

	((CMainWnd*) Parent())->SelectLevel(ptr, pos);
	return FALSE;
}

BOOL CLevelDlg::OnNotify_Delete(CNesEditor & ed, PNMLVLDRWHT p, CMainWnd * pParent)
{
	if ( Confirm(TEXT("Are you sure?")) )
	{
		m_File.getLevelEditor(pParent->GetCurLevel()).DeleteObject(p->iItem);
		OnNotify_ItemSelected(ed, p, pParent);
		return TRUE;
	}
	return FALSE;
}

BOOL CLevelDlg::OnNotify_ChangeLayer(CNesEditor & ed, PNMLVLDRWHT p, CMainWnd * pParent)
{
	CChgLayerDlg dlg(GetInstance(), 
		m_File.getLevelEditor(pParent->GetCurLevel()).LevelItem(p->iItem).Layer(),
		m_File.getLevelEditor(pParent->GetCurLevel()).CountLayers()
		);

	if ( dlg.Show(this) )
	{
		if ( !m_File.getLevelEditor(pParent->GetCurLevel()).ChangeLayer(p->iItem, dlg.GetLayerSelection()) )
			ShowError(TEXT("Failed to set layer for this object."));
		else
			return TRUE;
	}
	return FALSE;
}

BOOL CLevelDlg::OnNotify_LevelData(CNesEditor & ed, PNMLVLDRWHT p, CMainWnd * pParent)
{
	CNesLevel lvlEmpty;
	CNesEnemyData enmEmpty;
	CLevelInfoDlg dlg(GetInstance(), m_File.getLevelEditor(pParent->GetCurLevel()), pParent->GetCurLevel(), m_File,
		lvlEmpty, enmEmpty);

	if ( 127 == dlg.Show(this) )
	{
		CheckData();
	}

	return FALSE;
}

BOOL CLevelDlg::OnNotify_MakeJar(CNesEditor & ed, PNMLVLDRWHT p, CMainWnd * pParent)
{
	if ( !Confirm(TEXT("WARNING! This will delete ALL objects from the level!\n")
		TEXT("Do you want to continue?")) )
		return FALSE;

	if ( ed.MakeJarLevel() )
		m_pLevel->DrawLevelEx( ((CMainWnd*) Parent())->GetCurLevel(), &ed);

	return FALSE;
}

BOOL CLevelDlg::OnNotify_ItemSelected(CNesEditor & ed, PNMLVLDRWHT p, CMainWnd * pParent)
{

	EnableAll( FALSE, IDC_EDITITEM, IDC_DELITEM, 0 );
	if ( TRUE )
	{
		if ( !p->iItem || p->iItem >= ed.SizeOfItemsList() )
		{
			dSendMessage( IDC_OBJECTINFO, WM_SETTEXT, 0, (LPARAM) 0 );
		}
		else
		{
			CNesItem x = ed.LevelItem(p->iItem);
			EnableAll( TRUE, IDC_DELITEM, 0 );							

			std::map<CString, CString> mInfo;

			switch(x.Type())
			{
			case nitEnemy:
				{
					mInfo[TEXT("Type")] = TEXT("Enemy");
					mInfo[TEXT("Position")].Format(TEXT("%d:%d"), x.Pos().x, x.Pos().y);
					mInfo[TEXT("Page")].Format(TEXT("%d"), x.Page());
					mInfo[TEXT("Name")] = g_pszEnemies[x.Enemy()];
					break;
				}
			case nitGround:
				{
					mInfo[TEXT("Type")] = TEXT("Ground");
					mInfo[TEXT("Ground")].Format(TEXT("%02d"), HIBYTE(x.Ground()));
					mInfo[TEXT("Ground set")].Format(TEXT("%02d"), LOBYTE(x.Ground()) & 0x1f);
					EnableAll( TRUE, IDC_EDITITEM, 0 );
					break;
				}
			case nitRegular:
				{
					mInfo[TEXT("Type")] = TEXT("Regular object");
					mInfo[TEXT("Position")].Format(TEXT("%d:%d"), x.Pos().x, x.Pos().y);
					mInfo[TEXT("Page")].Format(TEXT("%d"), x.Page());
					mInfo[TEXT("Name")] = g_szItemName[x.RegularId()];
					mInfo[TEXT("Layer")].Format(TEXT("%d"), x.Layer());
					if ( x.RegularId() >= 0x30 )
					{
						mInfo[TEXT("Size")].Format(TEXT("%d"), 1 + (x.Regular() % 0x10));
						EnableAll( TRUE, IDC_EDITITEM, 0 );
					}
					break;
				}
			case nitEntrance:
				{
					mInfo[TEXT("Type")] = TEXT("Entrance");
					mInfo[TEXT("Position")].Format(TEXT("%d:%d"), x.Pos().x, x.Pos().y);
					mInfo[TEXT("Page")].Format(TEXT("%d"), x.Page());
					mInfo[TEXT("Name")] = g_szItemName[x.RegularId()];
					mInfo[TEXT("Layer")].Format(TEXT("%d"), x.Layer());

					BYTE ptr[2] = { 0 };
					if ( x.EntranceParameter(0) == 0xF5 ) // far pointer
					{
						ptr[0] = LOBYTE(LOWORD(x.EntranceParameter(1)));
						ptr[1] = LOBYTE(LOWORD(x.EntranceParameter(2)));
					}
					else	// near pointer
					{
						ptr[0] = LOBYTE(LOWORD(x.EntranceParameter(0)));
						ptr[1] = LOBYTE(LOWORD(x.EntranceParameter(1)));
					}

					int page = ( int(ptr[1]) & 0x0F ), 
						vid = ( 10 * int(ptr[0]) + ( int(ptr[1]) >> 4 ) );

					mInfo[TEXT("Points to")].Format(TEXT("(%d) %s, Page[%d]"),
						vid, ( vid < 210 ? g_szLevelName[vid] : TEXT("?") ), page);
					EnableAll( TRUE, IDC_EDITITEM, 0 );
					break;
				}
			case nitPointer:
				{
					mInfo[TEXT("Type")] = TEXT("Pointer");
					BYTE ptr[2] = { HIBYTE(x.Pointer()), LOBYTE(x.Pointer()) };
					int page = ( int(ptr[0]) & 0x0F ), 
						vid = ( 10 * int(ptr[1]) + ( int(ptr[0]) >> 4 ) );

					mInfo[TEXT("Points to")].Format(TEXT("(%d) %s, Page[%d]"),
						vid, ( vid < 210 ? g_szLevelName[vid] : TEXT("?") ), page);

					EnableAll( TRUE, IDC_EDITITEM, 0 );
					break;
				}
			}

			CString sInfo;
			for(std::map<CString, CString>::reverse_iterator m = mInfo.rbegin();
				mInfo.rend() != m; ++m)
			{
				sInfo = sInfo + ( sInfo.GetLength() > 0 ? TEXT("\r\n\r\n") : TEXT("") ) +
					m->first + TEXT(": ") + m->second;
			}

			dSendMessage( IDC_OBJECTINFO, WM_SETTEXT, 0, (LPARAM) sInfo.GetString() );
		}
	}
	return FALSE;
}