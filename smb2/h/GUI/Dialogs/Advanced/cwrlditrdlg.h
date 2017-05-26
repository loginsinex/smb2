
class CWrldItrDlg: public CDialog
{
	CFileLoader			& m_File;
	CComboBox			m_cbIterior[7];
	VOID				OnInit(LPARAM lParam);

	USHORT				m_pData[7];
	VOID				LoadItems();
	VOID				SaveItems();
	VOID				SetItems();
	VOID				WriteItems();
	INT					PSOnApply();

public:
	CWrldItrDlg(HINSTANCE hInstance, CFileLoader & fl);
};