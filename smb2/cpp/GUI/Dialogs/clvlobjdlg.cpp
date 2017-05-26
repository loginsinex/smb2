
#include "precomp.h"

CLvlObjDlg::CLvlObjDlg(HINSTANCE hInstance, CNesEditor & Level) : CDialog(hInstance, MAKEINTRESOURCE(IDD_LVLOBJ)) , m_Level( Level )
{
	AddAnchor(IDC_LIST1, ANCHOR_FULL);
	AddAnchor(IDOK, ANCHOR_RIGHT_BOTTOM);

	m_CurrentItems.dwCount = 0;
}

VOID CLvlObjDlg::CurrentList(std::vector<NES_LEVEL_ITEM> & pItems, DWORD dwCount)
{
	m_CurrentItems.pItems = pItems;
	m_CurrentItems.dwCount = dwCount;
}

VOID CLvlObjDlg::OnInit(LPARAM lParam)
{
	m_lv.AssignWindow(Ctl(IDC_LIST1));
	m_lv.AddColumn(TEXT("Type"), 50);
	m_lv.AddColumn(TEXT("ID"), 150);
	m_lv.AddColumn(TEXT("Parameters"), 100);
	m_lv.AddColumn(TEXT("Object x:y"), 100);
	m_lv.AddColumn(TEXT("Real x:y"), 100);
	m_lv.AddColumn(TEXT("Page"), 0);

	m_lv.ExtendedStyle(LVS_EX_FULLROWSELECT);

	CString oxy, rxy, params;
	std::vector<NES_LEVEL_ITEM> pLocalItems;
	if ( !m_CurrentItems.pItems.size() )
		m_Level.GetLocalLevelItems(pLocalItems);

	std::vector<NES_LEVEL_ITEM> & pItems = ( m_CurrentItems.pItems.size() ? m_CurrentItems.pItems : pLocalItems );

	UINT cItems = pItems.size();

	for(UINT i = 0; i < cItems; ++i)
	{
		NES_LEVEL_ITEM &x = pItems[i];
		int id = m_lv.AddItem(x.nli, i);
		
		switch(x.nli)
		{
		case nliBackToStart: m_lv.SetItem(id, 0, TEXT("New layer")); break;
		case nliEntrance: m_lv.SetItem(id, 0, TEXT("Entrance")); break;
		case nliGroundSet: m_lv.SetItem(id, 0, TEXT("Ground set")); break;
		case nliGroundType: m_lv.SetItem(id, 0, TEXT("Ground type")); break;
		case nliPtr: m_lv.SetItem(id, 0, TEXT("Pointer")); break;
		case nliRegular: m_lv.SetItem(id, 0, TEXT("Regular")); break;
		case nliSkipper: m_lv.SetItem(id, 0, TEXT("Skipper")); break;
		case nliUnknown: m_lv.SetItem(id, 0, TEXT("UNKNOWN")); break;
		}

		oxy.Format(TEXT("%d:%d [0x%02x:0x%02x]"), x.Object.General.x, x.Object.General.y, x.Object.General.x, x.Object.General.y);
//		( m_Level[nlfDirection] ?
			rxy.Format(TEXT("%d:%d [0x%02x:0x%02x]"), x.pt.x, x.pt.y, x.pt.x, x.pt.y);
//			:
//			rxy.Format(TEXT("%d:%d [0x%02x:0x%02x]"), ( x.pt.x % 0x10 ), ( x.pt.y + 0x10*(x.pt.x/0x10) ), ( x.pt.x % 0x10 ), ( x.pt.y + 0x10*(x.pt.x/0x10) ))
//		);

		switch(x.nli)
		{
		case nliRegular:
			{
				params.Format(TEXT("[0x%02X] %s"), x.Object.Regular.id, 
					(x.Object.Regular.id < 0x30 ? g_szItemName[x.Object.Regular.id] : g_szItemName[0x30+(x.Object.Regular.id-0x30)/0x10]));
				m_lv.SetItem(id, 1, params.GetString());
				m_lv.SetItem(id, 3, oxy.GetString());
				m_lv.SetItem(id, 4, rxy.GetString());
				break;
			}
		case nliEntrance:
			{
				params.Format(TEXT("[0x%02X] %s"), x.Object.Regular.id, g_szItemName[x.Object.Regular.id]);
				m_lv.SetItem(id, 1, params.GetString());

				( 0xFF != x.Object.Entrance.uEntranceParameter[0] ?
				params.Format( 
					( 0xF5 == x.Object.Entrance.uEntranceParameter[0] ?
						TEXT("Ptr: 0x%02X 0x%02X") : TEXT("Obj: 0x%02X 0x%02X")
					),
					( 0xF5 == x.Object.Entrance.uEntranceParameter[0] ?	x.Object.Entrance.uEntranceParameter[1] : x.Object.Entrance.uEntranceParameter[0] ),
					( 0xF5 == x.Object.Entrance.uEntranceParameter[0] ?	x.Object.Entrance.uEntranceParameter[2] : x.Object.Entrance.uEntranceParameter[1] )
					)
					:
				params = TEXT("No pointer. See near pointers")
				);

				m_lv.SetItem(id, 2, params.GetString());
				m_lv.SetItem(id, 3, oxy.GetString());
				m_lv.SetItem(id, 4, rxy.GetString());
				break;
			}
		case nliSkipper:
			{
				params.Format(TEXT("0x%02X"), x.Object.Skipper.uType);
				m_lv.SetItem(id, 2, params.GetString());
				break;
			}
		case nliPtr:
			{
				params.Format( 
						TEXT("Ptr: 0x%02X 0x%02X"),
						x.Object.Pointer.uParameter[0], x.Object.Pointer.uParameter[1]);
				m_lv.SetItem(id, 2, params.GetString());
				break;
			}
		case nliGroundSet:
			{
				params.Format(TEXT("0x%02X [%s]"), x.Object.GroundSet.Reserved, g_szSOName[x.Object.GroundSet.Reserved & 0x0F]);
				m_lv.SetItem(id, 1, params.GetString());
				params.Format(TEXT("0x%02X"), x.Object.GroundSet.uSets);
				m_lv.SetItem(id, 2, params.GetString());
				m_lv.SetItem(id, 4, rxy.GetString());
				break;
			}
		case nliGroundType:
			{
				params.Format(TEXT("0x%02X"), x.Object.GroundType.uType);
				m_lv.SetItem(id, 2, params.GetString());
				break;
			}
		}

		m_lv.SetItem(id, 5, x.Page);
	}

	std::vector<NES_LEVEL_ENEMY> pEnemies;

	if ( m_CurrentItems.pItems.size() )
		m_Level.GetCurrentEnemyItems(pEnemies);
	else
		m_Level.GetLocalEnemyItems(pEnemies);

	cItems = pEnemies.size();
	for(UINT i = 0; i < cItems; ++i)
	{
		int id = m_lv.AddItem(TEXT("Enemy"), 0);

		params.Format(TEXT("[0x%02X] %s"), pEnemies[i].Enemy.Object.id, 
			g_pszEnemies[pEnemies[i].Enemy.Object.id % 0x7f]);

		m_lv.SetItem(id, 1, params);
		oxy.Format(TEXT("%d:%d [0x%02x:0x%02x]"), pEnemies[i].Enemy.Object.x, pEnemies[i].Enemy.Object.y,
			pEnemies[i].Enemy.Object.x, pEnemies[i].Enemy.Object.y);
		rxy.Format(TEXT("%d:%d [0x%02x:0x%02x] (horz)"), pEnemies[i].pt.x, pEnemies[i].pt.y, 
			pEnemies[i].pt.x, pEnemies[i].pt.y);
		m_lv.SetItem(id, 3, oxy);
		m_lv.SetItem(id, 4, rxy);

		m_lv.SetItem(id, 5, pEnemies[i].iPage);
	}
}

VOID CLvlObjDlg::OnOK()
{
	Close(0);
}