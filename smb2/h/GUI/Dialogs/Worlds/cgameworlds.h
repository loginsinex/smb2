
typedef struct _tagNES_LEVEL_SET
{
	int					id;
	BOOL				fNew;
	BOOL				fDeleted;
	std::vector<BYTE>	vRoomsList;
} NES_LEVEL_SET, *PNES_LEVEL_SET;

class CWorldsDlgs: public CDialog
{
	CFileLoader		&		m_file;

	void					OnPreInit(CDialogTemplate & d);
	virtual void			OnPreInitSheet(CDialogTemplate & d) PURE;

protected:
	CFileLoader		&		File();
	CWorldsDlgs(HINSTANCE hInstance, CFileLoader & file);
	void			FillGradientRect(HDC hDC, RECT rc, COLORREF clr1, COLORREF clr2, BOOL fVertical = TRUE);
};

class CWorldsLevelsDlg: public CWorldsDlgs
{
	CBufView		*m_pBufView;
	BYTE			m_bWorlds[8];
	CFont			m_fntCtl;

	struct
	{
		HBRUSH		hBrushHl;	// hilite
		HBRUSH		hBrushBd;	// game bounds
		COLORREF	clrGradient1;	// gradient for background
		COLORREF	clrGradient2;
		
		BOOL		fBounds[2][2];		// game bounds buttons states. 0 - left, 1 - right
		BOOL		fWorlds[2][7];		// worlds buttons states. 0 - left, 1 - right
		RECT		rcBounds[2][2];		// buttons rect
		RECT		rcWorlds[2][7];

		struct
		{
			RECT *	prc;
			BOOL *	pfState;
			struct
			{
				BOOL	fBounds;
				BOOL	fRight;
				BYTE	bWorld;
			} type;
		} sel;
	} m_ctlState;

	void			OnInit(LPARAM lParam);
	void			OnPreInitSheet(CDialogTemplate & d);
	INT				PSOnApply();

	static			LRESULT CALLBACK BVProc(HWND, UINT, WPARAM, LPARAM, PVOID);
	LRESULT			CtlProc(CBufView *, UINT, WPARAM, LPARAM);
	void			OnCtlMouseEvent(UINT uMouseEvent, SHORT x, SHORT y);
	void			CtlDraw();
	void			OnCtlClick(BOOL fBounds, BOOL fRight, BYTE world);
	
	void			LoadData();
	void			StoreData();
	
public:
	CWorldsLevelsDlg(HINSTANCE hInstance, CFileLoader & file);
	~CWorldsLevelsDlg();
};

class CWorldsDataDlg: public CWorldsDlgs
{
	CListView					m_lv;
	CComboBox					m_cb;
	CFont						m_fnList;
	std::vector<NES_LEVEL_SET>	m_vLevelsSets;
	std::vector<NES_LEVEL_SET>	m_vEnemiesSets;

	void						OnPreInitSheet(CDialogTemplate & d);
	void						OnInit(LPARAM lParam);
	INT_PTR						OnNotify(LPNMHDR lpnm);
	void						OnButton(USHORT uId);
	INT							PSOnApply();

	void						LoadData();
	void						StoreData();
	void						ListRooms();
	void						MoveRoom();
	void						CreateBucket();
	void						FindRoom();

public:
	CWorldsDataDlg(HINSTANCE hInstance, CFileLoader & file);
};

// helpers dialogs

class CChooseBucketDlg: public CDialog
{
	CComboBox					m_cb;
	void						OnPreInit(CDialogTemplate & d);
	void						OnInit(LPARAM lParam);
	void						OnOK();

	struct
	{
		BOOL						fEnemiesBuckets;
		int							iSelected;
		std::vector<NES_LEVEL_SET>	vList;
	} m_buckets;

public:
	CChooseBucketDlg(HINSTANCE hInstance, std::vector<NES_LEVEL_SET> & vList, BOOL fEnemiesBucket, int iSelected);
	int							GetSelected();
};

class CNewBucketDlg: public CDialog
{
	void						OnPreInit(CDialogTemplate & d);
	void						OnInit(LPARAM lParam);
	void						OnOK();
	BOOL						m_fEnemies;

public:
	CNewBucketDlg(HINSTANCE hInstance, BOOL fEnemies);

	BOOL						GetSelection();
};

class CFindRoomDlg: public CDialog
{
	CComboBox					m_cb;
	void						OnPreInit(CDialogTemplate & d);
	void						OnInit(LPARAM lParam);
	void						OnOK();
	BOOL						m_fEnemies;
	BYTE						m_bRoom;

public:
	CFindRoomDlg(HINSTANCE hInstance);

	BOOL						GetSelection(BYTE & bRoom);
};