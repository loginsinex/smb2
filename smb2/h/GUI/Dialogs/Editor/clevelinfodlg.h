
class CLevelInfoDlg: public CDialog
{
	CNesEditor		&	m_pvEditor;
	CFileLoader		&	m_File;
	CTrackBar			m_tbLength;
	CTrackBar			m_tbBgSet;
	CTrackBar			m_tbBgType;
	CTrackBar			m_tbObjType;
	CTrackBar			m_tbPallete;
	CTrackBar			m_tbEnemyPallete;
	CComboBox			m_cbMusic;
	CLvlDraw		*	m_pLevel;
	CNesEditor			m_vEditor;
	DWORD				m_dwCurLevel;


	VOID				OnInit(LPARAM lParam);
	VOID				OnOK();
	VOID				LoadLevelSample();
	INT					OnMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & pHandle);
	INT_PTR				OnTrackbar(LPNMHDR lpnm);
	VOID				OnButton(USHORT uId);


public:
	CLevelInfoDlg(HINSTANCE hInstance, CNesEditor & pvEditor, DWORD dwCurLevel, CFileLoader & file,
		CNesLevel & lvl, CNesEnemyData & enm);
};