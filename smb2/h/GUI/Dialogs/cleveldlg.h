
class CMainWnd;

class CLevelDlg: public CDialog
{
	CFileLoader &	m_File;
	CLvlDraw*		m_pLevel;

	CListView		m_lv;

	void			OnInit(LPARAM lParam);
	INT_PTR			OnNotify(LPNMHDR lpnm);
	void			OnButton(USHORT uId);


	// events from leveleditor
	BOOL			OnNotify_ChangeBg(CNesEditor & ed, PNMLVLDRWHT p, CMainWnd * pParent);
	BOOL			OnNotify_HitTest(CNesEditor & ed, PNMLVLDRWHT p, CMainWnd * pParent);
	BOOL			OnNotify_HitTestEnemy(CNesEditor & ed, PNMLVLDRWHT p, CMainWnd * pParent);
	BOOL			OnNotify_Insert(CNesEditor & ed, PNMLVLDRWHT p, CMainWnd * pParent, BOOL fInsertJarEnemy);
	BOOL			OnNotify_Edit(CNesEditor & ed, PNMLVLDRWHT p, CMainWnd * pParent);
	BOOL			OnNotify_JumpPointer(CNesEditor & ed, PNMLVLDRWHT p, CMainWnd * pParent);
	BOOL			OnNotify_Delete(CNesEditor & ed, PNMLVLDRWHT p, CMainWnd * pParent);
	BOOL			OnNotify_ChangeLayer(CNesEditor & ed, PNMLVLDRWHT p, CMainWnd * pParent);
	BOOL			OnNotify_LevelData(CNesEditor & ed, PNMLVLDRWHT p, CMainWnd * pParent);
	BOOL			OnNotify_MakeJar(CNesEditor & ed, PNMLVLDRWHT p, CMainWnd * pParent);
	BOOL			OnNotify_ItemSelected(CNesEditor & ed, PNMLVLDRWHT p, CMainWnd * pParent);

public:
	CLevelDlg(CFileLoader & fl, HINSTANCE hInstance);
	VOID			CheckData(BOOL fInvalidateCanvas = FALSE);
	VOID			SelectPos(int page, BYTE pos);
};

class CNewSpaceDlg: public CDialog
{
	BOOL			m_fCreateForEnemies;
	DWORD			m_dwLevel;
	std::vector<int>  m_vLevelData;
	std::vector<BYTE>  m_vRoomsLevelsList;
	std::vector<int>  m_vEnemyData;
	std::vector<BYTE>  m_vRoomsEnemiesList;

	void			OnPreInit(CDialogTemplate & d);
	void			OnInit(LPARAM lParam);
	void			OnOK();

public:
	CNewSpaceDlg(HINSTANCE hInstance, std::vector<int> & vLevelData, std::vector<BYTE> & vRoomsLevelsList,
				std::vector<int> & vEnemyData, std::vector<BYTE> & vRoomsEnemiesList, BOOL fEnemies,
				DWORD dwLevel);

	BOOL		GetSelection(std::vector<int> & vLevelData, std::vector<BYTE> & vRoomsLevelsList,
					std::vector<int> & vEnemyData, std::vector<BYTE> & vRoomsEnemiesList);
};