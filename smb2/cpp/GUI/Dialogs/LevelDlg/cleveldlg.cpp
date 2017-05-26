
#include "precomp.h"

CLevelDlg::CLevelDlg(CFileLoader & fl, HINSTANCE hInstance) : CDialog(hInstance, MAKEINTRESOURCE(IDD_LEVEL)), m_File(fl)
{
	AddAnchor(IDC_LEVEL, ANCHOR_FULL);
	//AddAnchor(IDC_EDIT2, ANCHOR_RIGHT_TOP);
	AddAnchor(IDC_LAYER1, ANCHOR_RIGHT_TOP);
	AddAnchor(IDC_LAYER2, ANCHOR_RIGHT_TOP);
	AddAnchor(IDC_STATIC_LAYER_CHOOSE, ANCHOR_RIGHT_TOP);
	AddAnchor(IDC_STATIC_WARN, ANCHOR_RIGHT_TOP);
	AddAnchor(IDC_ICON_WARN, ANCHOR_RIGHT_TOP);
	AddAnchor(IDC_STATIC_LEVEL_LINK, ANCHOR_RIGHT_TOP);
	AddAnchor(IDC_LEVEL_UNLOCK, ANCHOR_RIGHT_TOP);
	AddAnchor(IDC_STATIC_WARN2, ANCHOR_RIGHT_TOP);
	AddAnchor(IDC_ICON_WARN2, ANCHOR_RIGHT_TOP);
	AddAnchor(IDC_STATIC_LEVEL_LINK2, ANCHOR_RIGHT_TOP);
	AddAnchor(IDC_LEVEL_UNLOCK2, ANCHOR_RIGHT_TOP);
	AddAnchor(IDC_WINPANE, ANCHOR_RIGHT);

	AddAnchor(IDC_LAYER1, ANCHOR_RIGHT_TOP);
	AddAnchor(IDC_LAYER2, ANCHOR_RIGHT_TOP);
	AddAnchor(IDC_LAYER3, ANCHOR_RIGHT_TOP);

	AddAnchor(IDC_OBJECTINFO, ANCHOR_RIGHT_TOP);

	AddAnchor(IDC_EDITITEM, ANCHOR_RIGHT_TOP);
	AddAnchor(IDC_DELITEM, ANCHOR_RIGHT_TOP);

	AddAnchor(IDC_LEVEL_UNLOCK, ANCHOR_RIGHT_TOP);
	AddAnchor(IDC_LEVEL_UNLOCK2, ANCHOR_RIGHT_TOP);
}

void CLevelDlg::OnInit(LPARAM lParam)
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
	else
		ShowError(TEXT("Failed to create level draw"));

	dSendMessage(IDC_ICON_WARN, STM_SETICON, (WPARAM) LoadIcon(NULL, IDI_WARNING));
	dSendMessage(IDC_ICON_WARN2, STM_SETICON, (WPARAM) LoadIcon(NULL, IDI_WARNING));

	dSendMessage(IDC_WINPANE, WM_UPDATEUISTATE, MAKELONG(UIS_CLEAR, UISF_HIDEACCEL | UISF_HIDEFOCUS));
}

void CLevelDlg::OnButton(USHORT uId)
{
	switch(uId)
	{
	case IDC_LAYER1:
	case IDC_LAYER2:
	case IDC_LAYER3:
		{
			CheckRadioButton(GetWindowHandle(), IDC_LAYER1, IDC_LAYER2, uId);
			m_pLevel->SetCatchCanvas(IDC_LAYER2 == uId, IDC_LAYER3 == uId );
			break;
		}
	case IDC_LEVEL_UNLOCK:
	case IDC_LEVEL_UNLOCK2:
		{
			std::vector<int> vLevelData;
			std::vector<BYTE> vRoomsLevelsList;
			std::vector<int> vEnemyData;
			std::vector<BYTE> vRoomsEnemiesList;

			if ( !m_File.GetGameSets(vLevelData, vRoomsLevelsList,
				vEnemyData, vRoomsEnemiesList) )
				break;

			CNewSpaceDlg dlg(GetInstance(), 
				vLevelData, vRoomsLevelsList, vEnemyData, vRoomsEnemiesList,
				(IDC_LEVEL_UNLOCK2 == uId), ((CMainWnd*) Parent())->GetCurLevel());


			if ( dlg.Show(this) )
			{
				m_pLevel->DrawLevelEx(-1, NULL);
				dlg.GetSelection(vLevelData, vRoomsLevelsList, vEnemyData, vRoomsEnemiesList);
				m_File.SetGameSets(vLevelData, vRoomsLevelsList,
						vEnemyData, vRoomsEnemiesList);

				CheckData();
			}

			break;
		}
	case IDC_INSITEM:
		{
			break;
		}
	case IDC_EDITITEM:
		{
			CNesEditor & ed = m_File.getLevelEditor(((CMainWnd*) Parent())->GetCurLevel());
			NMLVLDRWHT p = { 0 };
			p.iItem = m_pLevel->GetSelectedItem();
			if ( p.iItem && p.iItem < ed.SizeOfItemsList() )
				if ( OnNotify_Edit(ed, &p, (CMainWnd*) Parent()) )
					m_pLevel->RequestRedraw();

			break;
		}
	case IDC_DELITEM:
		{
			CNesEditor & ed = m_File.getLevelEditor(((CMainWnd*) Parent())->GetCurLevel());
			NMLVLDRWHT p = { 0 };
			p.iItem = m_pLevel->GetSelectedItem();
			if ( p.iItem && p.iItem < ed.SizeOfItemsList() )
				if ( OnNotify_Delete(ed, &p, (CMainWnd*) Parent()) )
					m_pLevel->RequestRedraw();

			break;
		}
	case IDC_ROTATELEVEL:
		{
			if ( Confirm(TEXT("WARNING!\nRotating level can cause loose placements of some objects.\n")
				TEXT("Do you want to continue?")) )
			{
				CNesEditor & ed = m_File.getLevelEditor(((CMainWnd*) Parent())->GetCurLevel());
				ed.SetHeaderInfo(nlfDirection, !ed[nlfDirection]);
				CheckData();
			}
			break;
		}
	}
}


VOID CLevelDlg::CheckData(BOOL fInvalidateCanvas)
{
	if ( !m_File.IsLoaded() )
		return;

	CheckRadioButton(GetWindowHandle(), IDC_LAYER1, IDC_LAYER2, IDC_LAYER1);
	EnableAll(TRUE, IDC_LAYER1, IDC_LAYER2, IDC_LAYER3, 0);


	if ( fInvalidateCanvas )	// warning! currently selected level editor is not valid!
		m_pLevel->DrawLevelEx(-1, NULL);

	m_pLevel->SetCatchCanvas(FALSE, FALSE);

	DWORD dwLevel = ((CMainWnd*) Parent())->GetCurLevel();

	EnableAll( ( dwLevel % 10 ) == 4, IDC_LAYER3, 0 );

	EnableAll( m_File.getLevelEditor(dwLevel).IsItemsLayerLocked(), IDC_LEVEL_UNLOCK, 0 );
	EnableAll( m_File.getLevelEditor(dwLevel).IsEnemyLayerLocked(), IDC_LEVEL_UNLOCK2, 0 );
	
	BOOL fLoaded = m_pLevel->DrawLevelEx(dwLevel, &m_File.getLevelEditor(dwLevel));

}

VOID CLevelDlg::SelectPos(int page, BYTE pos)
{
	m_pLevel->SelectPos(page, pos);
}


// new space allocator

CNewSpaceDlg::CNewSpaceDlg(HINSTANCE hInstance, std::vector<int> & vLevelData, std::vector<BYTE> & vRoomsLevelsList,
				std::vector<int> & vEnemyData, std::vector<BYTE> & vRoomsEnemiesList, BOOL fEnemies,
				DWORD dwLevel)
				: CDialog(hInstance),
				m_vLevelData(vLevelData),
				m_vRoomsLevelsList(vRoomsLevelsList),
				m_vEnemyData(vEnemyData),
				m_vRoomsEnemiesList(vRoomsEnemiesList),
				m_fCreateForEnemies(fEnemies),
				m_dwLevel(dwLevel)
{

}

void CNewSpaceDlg::OnPreInit(CDialogTemplate & d)
{
	CString sData = ( m_fCreateForEnemies ? TEXT("enemies") : TEXT("level") );
	CString sAddData = ( !m_fCreateForEnemies ? TEXT("enemies") : TEXT("level") );
	CString sLevel = g_szLevelName[m_dwLevel];

	d.SetDlgPos(0, 0, 200, 120);
	d.SetDlgTitle(TEXT("Allocate new space"));
	d.AddPreItem(-1, cButton, NULL, 7, 7, -7, 75, BS_GROUPBOX, TRUE, TRUE, FALSE);
	d.AddPreItem(-1, cStatic, TEXT("Allocating space for '") + 
		sLevel + TEXT("' ") + sData + TEXT(" will grow up memory usage.\n")
		TEXT("Do you want to continue?"), 45, 20, -15, 50, 0, TRUE, TRUE, FALSE);

	d.AddPreItem(IDI_ICON1, cStatic, NULL, 15, 20, 10, 10, SS_ICON, TRUE, TRUE, FALSE);

	d.AddPreItem(IDC_CHECK1, cButton, TEXT("Create new space for ") + sAddData + TEXT(" too."),
		7, 85, 125, 10, BS_AUTOCHECKBOX);

	d.AddPreItem(IDOK, cButton, MB_GetString(IDYES-1), 
		-(7 + 50 + 5 + 50), -(7 + 15), 50, 15, BS_DEFPUSHBUTTON);

	d.AddPreItem(IDCANCEL, cButton, MB_GetString(IDNO-1), 
		-(7 + 50), -(7 + 15), 50, 15, BS_PUSHBUTTON);
}

void CNewSpaceDlg::OnInit(LPARAM lParam)
{
	dSendMessage(IDI_ICON1, STM_SETICON, (WPARAM) LoadIcon(NULL, IDI_QUESTION));
	CheckAll(TRUE, IDC_CHECK1);
}

void CNewSpaceDlg::OnOK()
{
	BOOL fCreateAll = !!Checked(IDC_CHECK1);

	DWORD dwLevelsLinks = 0, dwEnemiesLinks = 0;
	DWORD dwLevelBucket = m_vRoomsLevelsList[m_dwLevel];
	DWORD dwEnemiesBucket = m_vRoomsEnemiesList[m_dwLevel];

	for(int i = 0; i < MAX_LEVELS; ++i)
	{
		dwLevelsLinks += ( ( m_vRoomsLevelsList[i] == dwLevelBucket ) ? 1 : 0 );
		dwEnemiesLinks += ( ( m_vRoomsEnemiesList[i] == dwEnemiesBucket ) ? 1 : 0 );
	}

	BOOL fLostLevelData = FALSE, fLostEnemiesData = FALSE;

	fLostLevelData = ( ( fCreateAll || !m_fCreateForEnemies ) && dwLevelsLinks < 2 );
	fLostEnemiesData = ( ( fCreateAll || m_fCreateForEnemies ) && dwEnemiesLinks < 2 );

	CString sLostData;
	if ( fLostLevelData )
		sLostData = TEXT("* Level data;\n");
	if ( fLostEnemiesData )
		sLostData = TEXT("* Enemies data;\n");

	if ( ( fLostLevelData || fLostEnemiesData ) &&
		!Confirm(TEXT("WARNING!\n")
			TEXT("Creating new space for this level will cause loosing data for following items:\n")
			+ sLostData + 
			TEXT("\nDo you want to continue?"))
			)
	{
		Close(0);
		return;
	}

	if ( fCreateAll || !m_fCreateForEnemies )
	{
		m_vLevelData.push_back(m_vLevelData.size());
		m_vRoomsLevelsList[m_dwLevel] = m_vLevelData.size();
	}

	if ( fCreateAll || m_fCreateForEnemies )
	{
		m_vEnemyData.push_back(m_vEnemyData.size());
		m_vRoomsEnemiesList[m_dwLevel] = m_vEnemyData.size();
	}

	Close(1);
}

BOOL CNewSpaceDlg::GetSelection(std::vector<int> & vLevelData, std::vector<BYTE> & vRoomsLevelsList,
					std::vector<int> & vEnemyData, std::vector<BYTE> & vRoomsEnemiesList)
{
	vLevelData = m_vLevelData;
	vRoomsLevelsList = m_vRoomsLevelsList;
	vEnemyData = m_vEnemyData;
	vRoomsEnemiesList = m_vRoomsEnemiesList;

	return TRUE;
}

/*
object data

BYTE: x+(y<<4)
BYTE: object_id

id: 
00 - mushroom
01 - POW
02 - breakable brick
03 - peace of vine
04 - pot
05 - peace of ladder
06 - vase [enterable]
07 - vase [non-enterable]
08 - vase [non-enterable]
09 - locked door
0a - door
0b - entrance
0c - vine
0d - endless vine
0e - stars
0f - column
10 - big cloud
11 - small cloud
12 - bottom of vine
13 - light entrance [right]
14 - light entrance [left]
15 - high lighter entrance
16 - palm tree
17 - pyramid
18 - brick wall
19 - brickwork
1a - [underground] ?
1b - [underground] ?
1c - [brick] ?
1d - [brick] ?
1e - ledge
1f - red background
20 - veg coin
21 - veg big
22 - veg small
23 - veg rocket
24 - veg turtle
25 - veg bomb
26 - veg door to subspace
27 - veg 1up
28 - veg POW
29 - cherry
2a - veg bomo-bomb
2b - [unknown]
2c - devil face
2d - [unknown]
2e - [background] ?
2f - wood brick
30-3f - brick
40-4f - blue brick
50-5f - gardenbed
60-6f - footbridge
70-7f - drills barrier
80-8f - vertical rock leadge
90-9f - skew vertical rock leadge
a0-af - ladder
b0-bf - water background [?]
c0-cf - green rock
d0-df - beam
e0-ef - clouds
f0-ff - waterfall

00 - health heart
01 - red
02 - birdy
03 - pink
04 - hedgehog
05 - bullet red
06 - bullet gray
07 - bullet pink
08 - red with camel-bird
09 - bomo-bomb
0a - bird with bomo-bomb
0b - bird
0c - [unknown] ?
0d - running cat
0e - jumping cat
0f - bullet red birdy [ver]
10 - bullet gray birdy [hor]
11 - boss4 weapon
12 - carpet
13 - fish
14 - vine snake
15 - [unknown] ?
16 - [unknown] ?
17 - evil face
18 - bullet
19 - [unknown] ?
1a - cactus
1b - slow bullet
1c - levelboss3
1d - boss mouser
1e - levelboss1 weapon [egg]
1f - boss fire snake
20 - fire
21 - boss5
22 - boss5 weapon
23 - fire red thrower
24 - running fire pink thrower
25 - fire gray thrower
26 - running red with camel-bird
27 - running fire
28 - keith
29 - running enemy [from 7 level]
2a - boss4
2b - boss4 child
2c - wart
2d - right eagle face
2e-31 - static fire
32 - flying small vegetable
33 - flying big vegetable
34 - flying???
35 - turtle
36 - coin
37 - explosion???
38 - [unknown] ?
39 - mushroom
3a - transparent POW
3b - waterfall beams
3c - door???
3d - key
3e - subspace effect???
3f - health mushroom
40 - 1up mushroom
41 - carpet
42 - right eagle face
43 - left eagle face
44 - pearl
45 - star
46 - timer
47 - flying???
48 - swarm of bullet red birdies [ver]
49 - [unknown] ?
4a - [unknown] ?
4b - [game freeze] ?
4c - [unknown] ?
4d - [game freeze] ?
4e - [game freeze] ?
4f - [game freeze] ?
50 - [game freeze] ?
51 - [game freeze] ?
52 - [game freeze] ?
53 - [game freeze] ?
54 - [unknown] ?
55 - [game freeze] ?
56 - [game freeze] ?
57 - [breeze effect] ?
58 - [breeze effect] ?
59 - [game freeze] ?
5a - [game freeze] ?
5b - [game freeze] ?
5c-7f <==> 1c-3f
80-ff - [unknown] ?
*/