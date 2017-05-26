
#include "precomp.h"

void CWorldsDlgs::OnPreInit(CDialogTemplate & d)
{
	d.SetDlgPos(0, 0, 350, 250);
	d.SetDlgStyle(DS_SHELLFONT, 0);
	OnPreInitSheet(d);
}


CFileLoader & CWorldsDlgs::File()
{
	return m_file;
}

CWorldsDlgs::CWorldsDlgs(HINSTANCE hInstance, CFileLoader & file)
	: CDialog(hInstance), m_file(file)
{

}

void CWorldsDlgs::FillGradientRect(HDC hDC, RECT rc, COLORREF clr1, COLORREF clr2, BOOL fVertical)
{
	TRIVERTEX tv[2] = { 0 };
	GRADIENT_RECT gr = { 0 };
	tv[0].Blue = GetBValue(clr1) << 8;
	tv[0].Green = GetGValue(clr1) << 8;
	tv[0].Red = GetRValue(clr1) << 8;
	tv[0].x = rc.left; tv[0].y = rc.top;

	tv[1].Blue = GetBValue(clr2) << 8;
	tv[1].Green = GetGValue(clr2) << 8;
	tv[1].Red = GetRValue(clr2) << 8;
	tv[1].x = rc.right; tv[1].y = rc.bottom;

	gr.UpperLeft = 0; gr.LowerRight = 1;

	GdiGradientFill(hDC, tv, 2, &gr, 1, fVertical ? GRADIENT_FILL_RECT_V : GRADIENT_FILL_RECT_H);
}

// ------

CWorldsLevelsDlg::CWorldsLevelsDlg(HINSTANCE hInstance, CFileLoader & file)
	: CWorldsDlgs(hInstance, file)
{
	InitBufView(hInstance, TEXT("SysWorldsLevels32"));
	ZeroMemory(&m_ctlState, sizeof(m_ctlState));
	// m_ctlState.hBrushBg = CreateSolidBrush(RGB(188, 199, 216));
	m_ctlState.clrGradient1 = RGB(233, 236, 238);
	m_ctlState.clrGradient2 = RGB(208, 215, 226);
	m_ctlState.hBrushHl = CreateSolidBrush(RGB(255, 236, 181));
	m_ctlState.hBrushBd = CreateSolidBrush(RGB(181, 236, 255));
}

CWorldsLevelsDlg::~CWorldsLevelsDlg()
{
	// DeleteObject(m_ctlState.hBrushBg);
	DeleteObject(m_ctlState.hBrushHl);
	DeleteObject(m_ctlState.hBrushBd);
}

void CWorldsLevelsDlg::OnPreInitSheet(CDialogTemplate & d)
{
	d.SetDlgTitle(TEXT("Worlds levels"));
	d.AddPreItem(-1, cButton, TEXT("Worlds"), 7, 7, -7, -7, BS_GROUPBOX, TRUE, TRUE, FALSE);
	d.AddPreItem(IDC_LIST1, TEXT("SysWorldsLevels32"), NULL, 15, 20, -15, -15, WS_BORDER);
}

void CWorldsLevelsDlg::OnInit(LPARAM lParam)
{
	m_pBufView = (CBufView*) dSendMessage(IDC_LIST1, BVM_GETINSTANCE);
	m_pBufView->Assign(BVProc, this);
	m_fntCtl.LoadFromWindow(Ctl(IDC_LIST1), 20, 1);


	LoadData();
	CtlDraw();
}

LRESULT CALLBACK CWorldsLevelsDlg::BVProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, PVOID pThis)
{
	CWorldsLevelsDlg * This = (CWorldsLevelsDlg*) pThis;
	return This->CtlProc(This->m_pBufView, uMsg, wParam, lParam);
}

LRESULT CWorldsLevelsDlg::CtlProc(CBufView * pbv, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_LBUTTONDBLCLK:
	case WM_LBUTTONDOWN:
	case WM_MOUSEMOVE:
	case WM_LBUTTONUP:
		{
			OnCtlMouseEvent(uMsg, LOWORD(lParam), HIWORD(lParam));
			break;
		}
	default:
		return DefWindowProc(*pbv, uMsg, wParam, lParam);
	}

	return 0;
}

void CWorldsLevelsDlg::LoadData()
{
	File().LoadData(m_bWorlds, 0x0001E022, sizeof(m_bWorlds));
}

void CWorldsLevelsDlg::StoreData()
{
	File().WriteData(m_bWorlds, 0x0001E022, sizeof(m_bWorlds));
}

void CWorldsLevelsDlg::CtlDraw()
{
	HGDIOBJ hbr = SelectObject(*m_pBufView, GetStockObject(NULL_BRUSH));
	HGDIOBJ hfn = SelectObject(*m_pBufView, m_fntCtl);
	HGDIOBJ hpn = SelectObject(*m_pBufView, GetStockObject(NULL_PEN));

	RECT rc = { 0 };

	const LONG widthProp[2] = { 10, 11 }, heightProp[2] = { 2, 3 };
	const LONG columns = ( 21 + 2 ), rows = ( 7 + 2 );

	GetClientRect(*m_pBufView, &rc);
	SIZE sz = { rc.right * widthProp[1] / ( 2 * widthProp[0] + widthProp[1] * ( columns - 2 )),
				rc.bottom * heightProp[1] / ( 2 * heightProp[0] + heightProp[1] * (rows - 2 ) )
				};

	SIZE szBtn = { widthProp[0] * sz.cx / widthProp[1], heightProp[0] * sz.cy / heightProp[1] };

	SIZE realsz = { 
		2 * szBtn.cx + sz.cx * (columns-2),
		2 * szBtn.cy + sz.cy * (rows-2)
	};

	POINT pt = { (rc.right - realsz.cx) / 2, (rc.bottom - realsz.cy) / 2 };

	FillGradientRect(*m_pBufView, rc, m_ctlState.clrGradient1, m_ctlState.clrGradient2);
	SetBkMode(*m_pBufView, TRANSPARENT);
	SetTextColor(*m_pBufView, GetSysColor(COLOR_GRAYTEXT));

	for(LONG y = 0; y < rows; ++y)
	{
		CString sLabel;
		RECT trc = { 0 };

		if ( y > 0 && y < rows - 1 )
		{
			SetRect(&trc,
						pt.x,
						szBtn.cy + sz.cy * (y - 1) + pt.y, 
						pt.x + realsz.cx, 0);

			trc.bottom = trc.top + sz.cy;
			sLabel.Format(TEXT("World %d"), y);
		}
		else if ( !y )
		{
			SetRect(&trc, pt.x, pt.y,
				pt.x + realsz.cx, pt.y + szBtn.cy);
			sLabel = TEXT("Begin game point");
		}
		else if ( y == ( rows - 1 ) )
		{
			SetRect(&trc, pt.x, szBtn.cy + sz.cy * (y - 1) + pt.y,
				pt.x + realsz.cx, 0);
			trc.bottom = trc.top + szBtn.cy;

			sLabel = TEXT("End game point");
		}

		DrawText(*m_pBufView, sLabel, sLabel.GetLength(), &trc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		for(LONG x = 0; x < columns; ++x)
		{
			RECT frc = { 
				( x > 0 ? szBtn.cx + sz.cx * (x - 1) : 0 ) + pt.x,
				( y > 0 ? szBtn.cy + sz.cy * (y - 1): 0 ) + pt.y,
				0, 0 };

			frc.right = frc.left + ( (x > 0 && x < columns - 1) ? sz.cx : szBtn.cx );
			frc.bottom = frc.top + ( (y > 0 && y < rows - 1) ? sz.cy : szBtn.cy );

			SelectObject(*m_pBufView, GetStockBrush(NULL_BRUSH));
			if ( !y )	// draw left game bound
			{
				if ( ( x - 1 ) == m_bWorlds[0] )
					SelectObject(*m_pBufView, m_ctlState.hBrushBd);
			}
			else if ( y == ( rows - 1 ) )
			{
				if ( x == LONG(m_bWorlds[7]) )
					SelectObject(*m_pBufView, m_ctlState.hBrushBd);
			}
			else if ( x > 0 && x < columns - 1 )
			{
				LONG pos = ( x - 1 );
				if ( pos >= LONG(m_bWorlds[ y - 1]) && pos < LONG(m_bWorlds[ y ]) )
					SelectObject(*m_pBufView, m_ctlState.hBrushHl);
			}

			if ( !x )	// draw buttons
			{
				BOOL * pfState = NULL;
				if ( !y )
				{
					m_ctlState.rcBounds[0][0] = frc;
					pfState = &m_ctlState.fBounds[0][0];
				}
				else if ( y == ( rows - 1 ) )
				{
					m_ctlState.rcBounds[0][1] = frc;
					pfState = &m_ctlState.fBounds[0][1];
				}
				else
				{
					m_ctlState.rcWorlds[0][y-1] = frc;
					pfState = &m_ctlState.fWorlds[0][y-1];
				}

				DrawFrameControl(*m_pBufView, &frc, DFC_SCROLL,
					DFCS_SCROLLLEFT | ( *pfState ? ( DFCS_FLAT | DFCS_PUSHED ): 0 ));
			}
			else if ( x == columns - 1 )
			{
				BOOL * pfState = NULL;
				if ( !y )
				{
					m_ctlState.rcBounds[1][0] = frc;
					pfState = &m_ctlState.fBounds[1][0];
				}
				else if ( y == ( rows - 1 ) )
				{
					m_ctlState.rcBounds[1][1] = frc;
					pfState = &m_ctlState.fBounds[1][1];
				}
				else
				{
					m_ctlState.rcWorlds[1][y-1] = frc;
					pfState = &m_ctlState.fWorlds[1][y-1];
				}
				DrawFrameControl(*m_pBufView, &frc, DFC_SCROLL,
					DFCS_SCROLLRIGHT | ( *pfState ? ( DFCS_FLAT | DFCS_PUSHED ) : 0 ) );
			}
			else
				Rectangle(*m_pBufView, 
					frc.left, frc.top, 
					frc.right, frc.bottom);
		}
	}

	SelectObject(*m_pBufView, hpn);
	SelectObject(*m_pBufView, hfn);
	SelectObject(*m_pBufView, hbr);

	m_pBufView->Refresh();
}

void CWorldsLevelsDlg::OnCtlMouseEvent(UINT uMouseEvent, SHORT x, SHORT y)
{
	BOOL fNeedRedraw = FALSE;
	POINT pt = { x, y };

	switch(uMouseEvent)
	{
	case WM_LBUTTONDBLCLK:
	case WM_LBUTTONDOWN:
		{
			if ( m_ctlState.sel.prc || m_ctlState.sel.pfState )
			{
				ZeroMemory(&m_ctlState.sel, sizeof(m_ctlState.sel));
				fNeedRedraw = TRUE;
			}
			
			for(int bd = 0; bd < 2; ++bd)
			{
				for(int type = 0; type < 2; ++type)
				{
					if ( PtInRect( &m_ctlState.rcBounds[bd][type], pt ) )
					{
						m_ctlState.sel.type.bWorld = type;
						m_ctlState.sel.type.fBounds = TRUE;
						m_ctlState.sel.type.fRight = ( bd > 0 );
						m_ctlState.sel.prc = &m_ctlState.rcBounds[bd][type];
						m_ctlState.sel.pfState = &m_ctlState.fBounds[bd][type];
						*m_ctlState.sel.pfState = TRUE;
						fNeedRedraw = TRUE;
						SetCapture(*m_pBufView);
					}
					else
						m_ctlState.fBounds[bd][type] = FALSE;
				}

				for(int world = 0; world < 7; ++world)
				{
					if ( PtInRect( &m_ctlState.rcWorlds[bd][world], pt ) )
					{
						m_ctlState.sel.type.bWorld = world;
						m_ctlState.sel.type.fBounds = FALSE;
						m_ctlState.sel.type.fRight = ( bd > 0 );

						m_ctlState.sel.prc = &m_ctlState.rcWorlds[bd][world];
						m_ctlState.sel.pfState = &m_ctlState.fWorlds[bd][world];
						*m_ctlState.sel.pfState = TRUE;
						fNeedRedraw = TRUE;
						SetCapture(*m_pBufView);
					}
					else
						m_ctlState.fWorlds[bd][world] = FALSE;
				}
			}
			break;
		}
	case WM_MOUSEMOVE:
		{
			if ( m_ctlState.sel.prc && m_ctlState.sel.pfState )
			{
				BOOL fState = PtInRect(m_ctlState.sel.prc, pt);

				if ( !!fState ^ !!*m_ctlState.sel.pfState )
				{
					fNeedRedraw = TRUE;
					*m_ctlState.sel.pfState = !!fState;
				}
			}
			break;
		}
	case WM_LBUTTONUP:
		{
			if ( m_ctlState.sel.prc && m_ctlState.sel.pfState )
			{
				ReleaseCapture();

				if ( PtInRect( m_ctlState.sel.prc, pt ) )
				{
					// event fired !
					OnCtlClick(
						m_ctlState.sel.type.fBounds, 
						m_ctlState.sel.type.fRight,
						m_ctlState.sel.type.bWorld);
				}

				*m_ctlState.sel.pfState = FALSE;

				ZeroMemory(&m_ctlState.sel, sizeof(m_ctlState.sel));
				fNeedRedraw = TRUE;
			}
			break;
		}
	}

	if ( fNeedRedraw )
		CtlDraw();
}

void CWorldsLevelsDlg::OnCtlClick(BOOL fBounds, BOOL fRight, BYTE world)
{
	if ( fBounds )
	{
		BYTE bStart = m_bWorlds[0], bEnd = m_bWorlds[7];
		if ( !fRight && !world )	// start left
		{			
			if ( !bStart || m_bWorlds[1] - bStart >= 4 )
				return;

			m_bWorlds[0]--;
		}
		else if ( fRight && !world )	// start right
		{
			if ( m_bWorlds[1] - bStart <= 1 )
				return;

			m_bWorlds[0]++;
		}
		else if ( !fRight && world )	// end left
		{
			if ( bEnd - m_bWorlds[6] <= 1 )
				return;

			m_bWorlds[7]--;
		}
		else if ( fRight && world )
		{
			if ( bEnd >= 21 || bEnd - m_bWorlds[6] >= 4 )
				return;

			m_bWorlds[7]++;
		}
	}
	else
	{
		if ( !fRight )	// we're moving only right border
		{
			if ( 6 == world )
				OnCtlClick(TRUE, FALSE, 1);
			else
				if ( m_bWorlds[world + 2] - m_bWorlds[world + 1] < 4 &&
					m_bWorlds[world + 1] - m_bWorlds[world] > 1 
					)
					m_bWorlds[world + 1]--;

		}
		else
		{
			if ( 6 == world )	// same as move right 'end' bound
				OnCtlClick(TRUE, TRUE, 1);
			else
				if ( m_bWorlds[world + 1] - m_bWorlds[world] < 4 &&
					m_bWorlds[world + 2] - m_bWorlds[world + 1] > 1
					)
					m_bWorlds[world + 1]++;
		}
	}
}

INT CWorldsLevelsDlg::PSOnApply()
{
	StoreData();
	return PSNRET_NOERROR;
}

// ----
CWorldsDataDlg::CWorldsDataDlg(HINSTANCE hInstance, CFileLoader & file)
	: CWorldsDlgs(hInstance, file)
{

}

void CWorldsDataDlg::OnPreInitSheet(CDialogTemplate & d)
{
	d.SetDlgTitle(TEXT("Levels sets"));
	d.AddPreItem(-1, cButton, TEXT("Level containers"), 7, 7, -7, -7, BS_GROUPBOX, TRUE, TRUE, FALSE);
	d.AddPreItem(IDC_LIST1, WC_LISTVIEW, NULL, 15, 25, -15, 170, LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS | LVS_NOCOLUMNHEADER | WS_BORDER);
	d.AddPreItem(IDC_CTL + 1, cButton, TEXT("Create bucket..."), 15, 220, 75, 15, BS_PUSHBUTTON);
	d.AddPreItem(IDC_CTL + 2, cButton, TEXT("Find room..."), 92, 220, 50, 15, BS_PUSHBUTTON);
	d.AddPreItem(IDC_CTL, cButton, TEXT("Move link..."), -(15 + 50), 220, 50, 15, BS_PUSHBUTTON);
}

void CWorldsDataDlg::OnInit(LPARAM lParam)
{
	m_lv.AssignWindow(Ctl(IDC_LIST1));
	m_cb.AssignWindow(Ctl(IDC_COMBO1));

	m_lv.ExtendedStyle(LVS_EX_FULLROWSELECT);
	m_lv.AddColumn(NULL);
	m_fnList.LoadFromWindow(Ctl(IDC_LIST1), -1, 1);

	LoadData();
	ListRooms();
}

void CWorldsDataDlg::ListRooms()
{
	m_lv.ResetContent();
	m_cb.cSendMessage(CB_RESETCONTENT);

	m_lv.AddItem(TEXT(""), -2);

	int c = m_vLevelsSets.size();
	for(int i = 0; i < c; ++i)
	{
		NES_LEVEL_SET & nls = m_vLevelsSets[i];
		CString sString;
		sString.Format(TEXT("Room bucket #%d"), i + 1);
		int id = m_lv.AddItem(sString, MAKEWORD(-1, i));

		int k = nls.vRoomsList.size();
		for(int l = 0; l < k; ++l)
		{
			sString.Format(TEXT("\t\t%s"), g_szLevelName[nls.vRoomsList[l]]);
			id = m_lv.AddItem(sString, MAKEWORD(l, i));
		}

		m_lv.AddItem(TEXT(""), -1);
	}

	m_lv.AddItem(TEXT(""), -3);

	c = m_vEnemiesSets.size();
	for(int i = 0; i < c; ++i)
	{
		NES_LEVEL_SET & nls = m_vEnemiesSets[i];
		CString sString;
		sString.Format(TEXT("Enemies bucket #%d"), i + 1);
		int id = m_lv.AddItem(sString, MAKELONG(MAKEWORD(-1, i), 1));

		int k = nls.vRoomsList.size();
		for(int l = 0; l < k; ++l)
		{
			sString.Format(TEXT("\t\t%s"), g_szLevelName[nls.vRoomsList[l]]);
			m_lv.AddItem(sString, MAKELONG(MAKEWORD(l, i), 1));
		}

		m_lv.AddItem(TEXT(""), -1);
	}
}

void CWorldsDataDlg::LoadData()
{
	std::vector<int> vLevelData;
	std::vector<BYTE> vRoomsLevelsList;
	std::vector<int> vEnemyData;
	std::vector<BYTE> vRoomsEnemiesList;

	if ( File().GetGameSets(vLevelData, vRoomsLevelsList, vEnemyData, vRoomsEnemiesList) )
	{
		int c = vLevelData.size();
		for(int i = 0; i < c; ++i)
		{
			NES_LEVEL_SET nls = { 0 };
			m_vLevelsSets.push_back(nls);
		}

		c = vEnemyData.size();
		for(int i = 0; i < c; ++i)
		{
			NES_LEVEL_SET nls = { 0 };
			m_vEnemiesSets.push_back(nls);
		}

		c = vRoomsLevelsList.size();
		for(int i = 0; i < c; ++i)
		{
			if ( vRoomsLevelsList[i] > 0 && vRoomsLevelsList[i] <= m_vLevelsSets.size() )
				m_vLevelsSets[vRoomsLevelsList[i]-1].vRoomsList.push_back(LOBYTE(LOWORD(i)));
			else
				_tprintf(TEXT("WARNING: Found unnamed room %d (%d)\n"), i, vRoomsLevelsList[i]);
		}

		c = vRoomsEnemiesList.size();
		for(int i = 0; i < c; ++i)
		{
			if ( vRoomsEnemiesList[i] > 0 && vRoomsEnemiesList[i] <= m_vEnemiesSets.size() )
				m_vEnemiesSets[vRoomsEnemiesList[i]-1].vRoomsList.push_back(LOBYTE(LOWORD(i)));
			else
				_tprintf(TEXT("WARNING: Found unnamed enemy room %d (%d)\n"), i, vRoomsEnemiesList[i]);
		}
	}
}

void CWorldsDataDlg::StoreData()
{
	std::vector<int> vLevelData;
	std::vector<BYTE> vRoomsLevelsList;
	std::vector<int> vEnemyData;
	std::vector<BYTE> vRoomsEnemiesList;

	vRoomsLevelsList.insert(vRoomsLevelsList.end(), MAX_LEVELS, 0);
	vRoomsEnemiesList.insert(vRoomsEnemiesList.end(), MAX_LEVELS, 0);

	int c = m_vLevelsSets.size();
	for(int i = 0; i < c; ++i)
	{
		std::vector<BYTE> & vRooms = m_vLevelsSets[i].vRoomsList;
		vLevelData.push_back(i);
		
		int k = vRooms.size();
		for(int l = 0; l < k; ++l)
			vRoomsLevelsList[vRooms[l]] = i + 1;
	}

	c = m_vEnemiesSets.size();
	for(int i = 0; i < c; ++i)
	{
		std::vector<BYTE> & vRooms = m_vEnemiesSets[i].vRoomsList;
		vEnemyData.push_back(i);
		
		int k = vRooms.size();
		for(int l = 0; l < k; ++l)
			vRoomsEnemiesList[vRooms[l]] = i + 1;
	}

	File().SetGameSets(vLevelData, vRoomsLevelsList, vEnemyData, vRoomsEnemiesList);
}

INT_PTR CWorldsDataDlg::OnNotify(LPNMHDR lpnm)
{
	if ( IDC_LIST1 == lpnm->idFrom && NM_CUSTOMDRAW == lpnm->code )
		{
			LPNMLVCUSTOMDRAW nmcd = (LPNMLVCUSTOMDRAW) lpnm;
			switch(nmcd->nmcd.dwDrawStage)
			{
			case CDDS_PREPAINT: return ( CDRF_NOTIFYITEMDRAW );
			case CDDS_ITEMPREPAINT: 
				{
					int data = int(nmcd->nmcd.lItemlParam);
					INT_PTR result = 0;
					SIZE sz = { nmcd->nmcd.rc.right - nmcd->nmcd.rc.left,
								nmcd->nmcd.rc.bottom - nmcd->nmcd.rc.top };

					switch(data)
					{
					case -2:
					case -3:
						{
							RECT linerc = { nmcd->nmcd.rc.left, nmcd->nmcd.rc.top + ( sz.cy - 4 ) / 2, 
											nmcd->nmcd.rc.right, nmcd->nmcd.rc.top + ( sz.cy + 4 ) / 2 };
							FillGradientRect(nmcd->nmcd.hdc, nmcd->nmcd.rc, RGB(255, 0, 0), RGB(0, 255, 255), FALSE);

							SetBkMode(nmcd->nmcd.hdc, TRANSPARENT);
							SetTextColor(nmcd->nmcd.hdc, GetSysColor(COLOR_HIGHLIGHTTEXT));

							HGDIOBJ hfn = SelectObject(nmcd->nmcd.hdc, m_fnList);
							DrawText(nmcd->nmcd.hdc, 
								( -3 == data ?
									TEXT("Enemies buckets") :
									TEXT("Rooms buckets")
								),
								-1, &nmcd->nmcd.rc, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
							SelectObject(nmcd->nmcd.hdc, hfn);
							result |= CDRF_SKIPDEFAULT;
							break;
						}
					case -1:
						{
							RECT linerc = { nmcd->nmcd.rc.left, nmcd->nmcd.rc.top + ( sz.cy - 2 ) / 2, 
											nmcd->nmcd.rc.right, nmcd->nmcd.rc.top + ( sz.cy + 2 ) / 2 };

							FillGradientRect(nmcd->nmcd.hdc, linerc, RGB(255, 255, 0), RGB(0, 255, 0), FALSE);
							result |= CDRF_SKIPDEFAULT;
							break;
						}
					default:
						{
							if ( LOBYTE(LOWORD(data)) == 0xFF )
							{
								if ( LOBYTE(HIWORD(data)) )		// enemies buckets
									FillGradientRect(nmcd->nmcd.hdc, nmcd->nmcd.rc, 
										RGB(188, 200, 220), RGB(208, 220, 240), FALSE);
								else
									FillGradientRect(nmcd->nmcd.hdc, nmcd->nmcd.rc, 
										RGB(220, 200, 188), RGB(240, 220, 208), FALSE);

								SetTextColor(nmcd->nmcd.hdc, GetSysColor(COLOR_CAPTIONTEXT));
								DrawText(nmcd->nmcd.hdc, m_lv.GetItemText(nmcd->nmcd.dwItemSpec, 0), -1, &nmcd->nmcd.rc, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
								result |= CDRF_SKIPDEFAULT;
							}
							break;
						}
					}
					return ( result );
				}
			}
		}
		return 0;
}

void CWorldsDataDlg::OnButton(USHORT uId)
{
	switch(uId)
	{
	case IDC_CTL + 0: MoveRoom(); break;
	case IDC_CTL + 1: CreateBucket(); break;
	case IDC_CTL + 2: FindRoom(); break;
	}
}

void CWorldsDataDlg::CreateBucket()
{
	CNewBucketDlg dlg(GetInstance(), FALSE);
	if ( dlg.Show(this) )
	{
		NES_LEVEL_SET nls = { 0 };
		nls.fNew = TRUE;

		BOOL fEnemies = dlg.GetSelection();
		std::vector<NES_LEVEL_SET> & vSets = ( fEnemies ? m_vEnemiesSets : m_vLevelsSets );
		vSets.push_back(nls);

		// find appropriate item to add new item
		int c = m_lv.Count();
		if ( !fEnemies )
		{
			for(int i = 1; i < c; ++i)
			{
				if ( -3 == m_lv.Param(i) )	// found start of enemies list
					c = i;
			}
		}

		CString sString;
		sString.Format(TEXT("%s bucket #%d"), fEnemies ? TEXT("Enemies") : TEXT("Room"), vSets.size());

		int id = m_lv.InsertItem(c, sString, MAKELONG(MAKEWORD(-1, vSets.size() - 1), !!fEnemies));
		m_lv.cSendMessage(LVM_ENSUREVISIBLE, m_lv.InsertItem(id+1, NULL, -1));
	}
}

void CWorldsDataDlg::FindRoom()
{
	CFindRoomDlg dlg(GetInstance());

	if ( dlg.Show(this) )
	{
		BYTE bRoom = 0;
		BOOL fEnemies = dlg.GetSelection(bRoom);

		int c = m_lv.Count();

		std::vector<NES_LEVEL_SET> & vSets = ( fEnemies ? m_vEnemiesSets : m_vLevelsSets );

		for(int i = 0; i < c; ++i)
		{
			int data = m_lv.Param(i);
			if ( data < 0 )
				continue;

			if ( !( !!LOBYTE(HIWORD(data)) ^ !!fEnemies ) && 
				LOBYTE(LOWORD(data)) != 0xFF &&
				vSets[HIBYTE(LOWORD(data))].vRoomsList[LOBYTE(LOWORD(data))] == bRoom )
			{
				m_lv.cSendMessage(WM_VSCROLL, SB_BOTTOM);
				m_lv.cSendMessage(LVM_ENSUREVISIBLE, i);
				m_lv.cSendMessage(WM_VSCROLL, SB_LINEUP);
				m_lv.CurSel(i);
				SetFocus(m_lv);
				break;
			}
		}
	}
}

void CWorldsDataDlg::MoveRoom()
{
	int id = m_lv.CurSel();
	if ( id >= 0 )
	{
		int data = m_lv.Param(id);
		int bucket = HIBYTE(LOWORD(data));
		int item_id = LOBYTE(LOWORD(data));

		if ( data >= 0 && 0xFF != item_id )
		{
			BOOL fEnemies = LOBYTE(HIWORD(data));
			std::vector<NES_LEVEL_SET> & vSets = ( fEnemies ? m_vEnemiesSets : m_vLevelsSets );
			int room_id = vSets[bucket].vRoomsList[item_id];

			CChooseBucketDlg dlg(GetInstance(), vSets, fEnemies, bucket);
			if ( dlg.Show(this) )
			{
				if ( dlg.GetSelected() != bucket )
				{
					int new_bucket = dlg.GetSelected();
					int new_item_id = -1;
					int bnew_bucket = LOBYTE(LOWORD(new_bucket));

					if ( new_bucket < int(vSets.size()) && item_id < int(vSets[bucket].vRoomsList.size()) )
					{
						vSets[bucket].vRoomsList.erase(vSets[bucket].vRoomsList.begin() + item_id);
						vSets[new_bucket].vRoomsList.push_back(room_id);

						new_item_id = vSets[new_bucket].vRoomsList.size() - 1;

						m_lv.DeleteItem(id);		// delete item

						// find appropriate item to insert item after it
						int c = m_lv.Count();
						for(int i = 0; i < c; ++i)
						{
							int idata = m_lv.Param(i);
							if ( HIBYTE(LOWORD(idata)) == bnew_bucket && !( !!fEnemies ^ !!LOBYTE(HIWORD(idata)) ) )
							{
								// find last item in this group
								for(i++; i < c; ++i)
								{
									if ( HIBYTE(LOWORD(m_lv.Param(i))) != bnew_bucket )
										break;
								}

								m_lv.CurSel(m_lv.InsertItem(
										i, g_szLevelName[room_id], 
										MAKELONG(MAKEWORD(new_item_id, new_bucket), 
										( fEnemies ? 1 : 0 ))
									));

								m_lv.cSendMessage(LVM_ENSUREVISIBLE, m_lv.CurSel());

								SetFocus(m_lv);
								break;
							}
						}

						// reorder old bucket items
						for(int i = 0; i < c; ++i)
						{
							int idata = m_lv.Param(i);
							if ( HIBYTE(LOWORD(idata)) == bucket && !( !!fEnemies ^ !!LOBYTE(HIWORD(idata)) ) )
							{
								// find last item in this group
								int k = 0;
								for(i++; i < c; ++i)
								{
									if ( HIBYTE(LOWORD(m_lv.Param(i))) == bucket )
									{
										if ( LOBYTE(LOWORD(m_lv.Param(i))) != 0xFF )
											m_lv.Param(i, MAKELONG(MAKEWORD(k, bucket), (fEnemies ? 1 : 0 )));
									}
									else
										break;

									k++;
								}
								break;
							}
						}
					}
				}
			}
		}
	}

}

INT CWorldsDataDlg::PSOnApply()
{
	StoreData();
	return PSNRET_NOERROR;
}


// ----

CChooseBucketDlg::CChooseBucketDlg(HINSTANCE hInstance, std::vector<NES_LEVEL_SET> & vList, BOOL fEnemiesBucket, int iSelected)
	: CDialog(hInstance)
{
	m_buckets.vList = vList;
	m_buckets.iSelected = iSelected;
	m_buckets.fEnemiesBuckets = fEnemiesBucket;
}

void CChooseBucketDlg::OnPreInit(CDialogTemplate & d)
{
	d.SetDlgPos(0, 0, 250, 75);
	d.SetDlgTitle(TEXT("Choose a bucket"));

	d.AddPreItem(-1, cButton, TEXT("Choose a new bucket for this room"), 7, 7, -7, 40, BS_GROUPBOX, TRUE, TRUE, FALSE);
	d.AddPreItem(IDC_COMBO1, cCombobox, NULL, 15, 25, -15, 100, CBS_DROPDOWNLIST | WS_VSCROLL);
	d.AddPreItem(IDOK, cButton, MB_GetString(0), -(7 + 50 + 5 + 50), -(7 + 15), 50, 15, BS_DEFPUSHBUTTON);
	d.AddPreItem(IDCANCEL, cButton, MB_GetString(1), -(7 + 50), -(7 + 15), 50, 15);
}

void CChooseBucketDlg::OnInit(LPARAM lParam)
{
	m_cb.AssignWindow(Ctl(IDC_COMBO1));
	int c = m_buckets.vList.size();
	CString sName = ( m_buckets.fEnemiesBuckets ? TEXT("Enemies") : TEXT("Room") );

	for(int i = 0; i < c; ++i)
	{
		CString sBucket;
		sBucket.Format(TEXT("%s%s bucket #%d [%d rooms]"), 
			( !m_buckets.vList[i].vRoomsList.size() ? TEXT("[!!!] ") : TEXT("") ),
			sName, i + 1, m_buckets.vList[i].vRoomsList.size());

		int id = m_cb.AddString(sBucket, i);

		if ( i == m_buckets.iSelected )
			m_cb.SetCurSel(id);
	}
}

void CChooseBucketDlg::OnOK()
{
	if ( -1 == m_cb.GetCurSel() )
		return;

	m_buckets.iSelected = m_cb.GetParam(m_cb.GetCurSel());
	Close(1);
}

int CChooseBucketDlg::GetSelected()
{
	return m_buckets.iSelected;
}

CNewBucketDlg::CNewBucketDlg(HINSTANCE hInstance, BOOL fEnemies)
	: CDialog(hInstance), m_fEnemies(fEnemies)
{

}

void CNewBucketDlg::OnPreInit(CDialogTemplate & d)
{
	d.SetDlgPos(0, 0, 200, 110);
	d.SetDlgTitle(TEXT("New bucket"));
	d.AddPreItem(-1, cButton, TEXT("Choose a type of new bucket"), 7, 7, -7, 55, BS_GROUPBOX, TRUE, TRUE, FALSE);
	d.AddPreItem(IDC_CTL, cButton, TEXT("Level bucket"), 15, 25, -15, 10, BS_AUTORADIOBUTTON);
	d.AddPreItem(IDC_CTL + 1, cButton, TEXT("Enemies bucket"), 15, 40, -15, 10, BS_AUTORADIOBUTTON);
	d.AddPreItem(IDOK, cButton, MB_GetString(0), -(7 + 50 + 5 + 50), -(7 + 15), 50, 15, BS_DEFPUSHBUTTON);
	d.AddPreItem(IDCANCEL, cButton, MB_GetString(1), -(7 + 50), -(7 + 15), 50, 15);
}

void CNewBucketDlg::OnInit(LPARAM lParam)
{
	CheckRadioButton(*this, IDC_CTL, IDC_CTL + 1,
		IDC_CTL + ( m_fEnemies ? 1 : 0 ));
}

void CNewBucketDlg::OnOK()
{
	m_fEnemies = Checked(IDC_CTL + 1);
	Close(1);
}

BOOL CNewBucketDlg::GetSelection()
{
	return m_fEnemies;
}


CFindRoomDlg::CFindRoomDlg(HINSTANCE hInstance)
	: CDialog(hInstance)
{

}

void CFindRoomDlg::OnPreInit(CDialogTemplate & d)
{
	d.SetDlgPos(0, 0, 200, 120);
	d.SetDlgTitle(TEXT("Find room"));
	d.AddPreItem(-1, cButton, TEXT("Choose a type of bucket and name of room to search"), 7, 7, -7, 70, BS_GROUPBOX, TRUE, TRUE, FALSE);
	d.AddPreItem(IDC_CTL, cButton, TEXT("Level bucket"), 15, 25, -15, 10, BS_AUTORADIOBUTTON);
	d.AddPreItem(IDC_CTL + 1, cButton, TEXT("Enemies bucket"), 15, 40, -15, 10, BS_AUTORADIOBUTTON);
	d.AddPreItem(IDC_COMBO1, cCombobox, NULL, 15, 55, -15, 100, CBS_DROPDOWNLIST | WS_VSCROLL);
	d.AddPreItem(IDOK, cButton, MB_GetString(0), -(7 + 50 + 5 + 50), -(7 + 15), 50, 15, BS_DEFPUSHBUTTON);
	d.AddPreItem(IDCANCEL, cButton, MB_GetString(1), -(7 + 50), -(7 + 15), 50, 15);
}

void CFindRoomDlg::OnInit(LPARAM lParam)
{
	m_cb.AssignWindow(Ctl(IDC_COMBO1));

	for(int i = 0; i < MAX_LEVELS; ++i)
		m_cb.AddString(g_szLevelName[i], i);

	m_cb.SetCurSel(0);

	CheckRadioButton(*this, IDC_CTL, IDC_CTL + 1, IDC_CTL);
}

void CFindRoomDlg::OnOK()
{
	m_bRoom = LOBYTE(LOWORD(m_cb.GetParam(m_cb.GetCurSel())));
	m_fEnemies = !!Checked(IDC_CTL+1);
	Close(1);
}

BOOL CFindRoomDlg::GetSelection(BYTE & bRoom)
{
	bRoom = m_bRoom;
	return m_fEnemies;
}