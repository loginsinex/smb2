
class CEntrancePtrDlg: public CDialog
{
	int				m_fCreation;
	BYTE			m_bRequestedPtr;

	VOID			OnInit(LPARAM lParam);
	VOID			OnPreInit(CDialogTemplate & d);
	VOID			OnButton(USHORT uId);

public:
	CEntrancePtrDlg(HINSTANCE hInstance, BOOL fCreation = 0);
	BOOL			IsFarPointer();
	BYTE			GetPointerParam(BYTE id);
	BYTE			GetRequestedPtr();

	VOID			SetPointerData(BOOL fFarPointer, BYTE param1, BYTE param2);
};

class CGroundSetDlg: public CDialog
{
	VOID			OnInit(LPARAM lParam);

public:
	CGroundSetDlg(HINSTANCE hInstance);
	BYTE			GroundSet();
	BYTE			GroundType();
	VOID			GroundSet(BYTE bBgSet);
	VOID			GroundType(BYTE bBgType);
};

class CItemSizeDlg: public CDialog
{
	VOID			OnInit(LPARAM lParam);

public:
	CItemSizeDlg(HINSTANCE hInstance);
	BYTE			GetSizeParam();
	VOID			SetSizeParam(BYTE bSize);
};

