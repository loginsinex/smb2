
class CLvlObjDlg: public CDialog
{
	CNesEditor & 		m_Level;
	CListView			m_lv;
	struct
	{
		DWORD							dwCount;
		std::vector<NES_LEVEL_ITEM>		pItems;
	} m_CurrentItems;

	VOID				OnInit(LPARAM lParam);
	VOID				OnOK();

public:
	CLvlObjDlg(HINSTANCE hInstance, CNesEditor & Level);
	VOID				CurrentList(std::vector<NES_LEVEL_ITEM> & pItems, DWORD dwCount);
};