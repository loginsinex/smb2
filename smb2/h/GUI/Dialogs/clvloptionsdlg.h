
class CLvlOptionsDlg: public CDialog
{
	CFileLoader &	m_File;
	void			OnInit(LPARAM lParam);
	void			OnButton(USHORT uId);

	struct
	{
		CComboBox	cbGamma1;
		CComboBox	cbGamma2;
		CComboBox	cbGroundType;
		CComboBox	cbGround;
		CComboBox	cbMusic;
		CTrackBar	tbLength;
	} m_ctl;

public:
	CLvlOptionsDlg(CFileLoader & fl, HINSTANCE hInstance);
	void			CheckData();
};