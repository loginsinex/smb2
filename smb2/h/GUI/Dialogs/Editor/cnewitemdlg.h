
class CNewItemDlg: public CDialog
{
	CDialog	*		m_pDlg[4];
	CEntrancePtrDlg	m_dlgEntrancePtr;
	CGroundSetDlg	m_dlgGroundSet;
	CItemSizeDlg	m_dlgItemSize;
	CEntrancePtrDlg	m_dlgPointer;
	BOOL			m_fEditMode;
	BYTE			m_bRequestConnectedPtr;

	struct
	{
		CNesEditor	*pvEditor;
		DWORD		dwItem;
	} m_Item;

	struct
	{
		BOOL			fEnemy;
		NES_LEVEL_ITEM	nItem;
		NES_LEVEL_ENEMY	nEnemy;
		BYTE			bGroundType;
	} m_NewItem;
	
	VOID			OnInit(LPARAM lParam);
	VOID			OnCommand(USHORT uCmd, USHORT uId, HWND hWnd);
	VOID			ShowProp(INT i);
	VOID			OnOK();
	VOID			OnCancel();

public:
	CNewItemDlg(HINSTANCE hInstance);
	CNewItemDlg(HINSTANCE hInstance, CNesEditor * pvEditor, DWORD iItem);
	BOOL			GetData(PNES_LEVEL_ITEM pItem, PNES_LEVEL_ENEMY pEnemy, PBYTE pbGroundType, PBOOL pfEnemy);
	BYTE			GetDataEx();
};