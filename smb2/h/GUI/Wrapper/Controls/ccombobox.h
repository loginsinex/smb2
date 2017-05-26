
class CComboBox: public CControl
{

public:
	INT			AddString(LPCTSTR lpszString, LPARAM lParam = 0);
	INT			GetCurSel();
	void		SetCurSel(INT iIndex);
	LPARAM		GetParam(INT iIndex);
};

