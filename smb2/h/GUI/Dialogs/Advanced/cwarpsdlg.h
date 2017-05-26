
class CWarpsDlg: public CDialog
{
	CFileLoader			& m_File;
	CComboBox			m_cbWarps[7];

	VOID				OnInit(LPARAM lParam);

	BYTE				m_pData[7];
	VOID				LoadItems();
	VOID				SaveItems();
	VOID				WriteItems();
	INT					PSOnApply();

public:
	CWarpsDlg(HINSTANCE hInstance, CFileLoader & fl);
};