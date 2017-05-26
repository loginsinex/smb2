
#define			ZLVLDRAW_CLASS		TEXT("SysLevelDraw32")
#define			MAX_TPL				20
#define			LDN_HITEST			-1201
#define			LDN_HITESTENEMY		-1202
#define			LDN_DELETE			-1203
#define			LDN_INSERT			-1204
#define			LDN_EDIT			-1205
#define			LDN_BGLIST			-1206
#define			LDN_PTRLIST			-1207
#define			LDN_LEVELDATA		-1208
#define			LDN_CHANGELAYER		-1209
#define			LDN_CHANGEBG		-1210
#define			LDN_NEXTITEM		-1211
#define			LDN_JUMPTOPOINTER	-1212
#define			LDN_ITEMSELECTED	-1213
#define			LDN_INSERTJARENEMY	-1214
#define			LDN_MAKEJAR			-1215

#define			CONVERT_REGULAR(r)	((r) >= 0x30 ? ( 0x30 + ((r)-0x30)/0x10 ) : (r))

#define			BG_PRIOR(regular, bitset, fx, grtype, horz)					\
			( !g_mPriorityList[(regular)].bgPriority ?						\
				0 :															\
				(															\
					(horz) ?												\
						g_mbgPriority[8*(fx)+(grtype)].gr.gr[(bitset)] :	\
						g_mvbgPriority[8*(fx)+(grtype)].gr.gr[(bitset)]		\
				)															\
			)

void InitLevelDraw();

typedef struct _tagNMLVLDRWHT
{
	NMHDR		nm;
	POINT		pos;
	USHORT		iItem;
} NMLVLDRWHT, *PNMLVLDRWHT;

typedef struct _tagNES_GITEM
{
	union
	{
		UINT		idItem;
		struct
		{
			USHORT	id;
			USHORT	type;
		} Item;
	} Item;
	UINT		idTile;
	BOOL		fVisible;
	BOOL		fHidden;
} NES_GITEM, *PNES_GITEM;

class CLvlDraw: public CNesItemsList<NES_LEVEL_ITEM>
{
	CFont			m_Font;
	HWND			m_hWnd;
	HWND			m_hParentWnd;
	HFONT			m_hGlobalFont;

	struct
	{
		CBitmap		bmBuffer;
		CBitmap		bmLevel;
		CBitmap		bmSpecial;
		CBitmap		bmTpl[MAX_TPL];
		CBitmap		bmGamma;
		CBitmap		bmGammaTpl;
		CBitmap		bmGrGammaTpl;
	} m_Bmp;

	struct
	{
		HCURSOR		hArrow;
		HCURSOR		hMove;
		HCURSOR		hCurrent;
	} m_cursor;


	CNesEditor *	m_pvEditor;

	PNES_ITEM_DIM	m_vDim;
	SIZE			m_dcsz;
	SIZE			m_sz[MAX_TPL];
	UINT			m_uGammaIndex;
	COLORREF		m_crBg;
	HDC				m_hDC;

	struct
	{
		INT				fx;
		BYTE			eColor;
		SIZE			sz;
		PNES_GITEM		pItems;
		PNES_GITEM		pEnemyItems;

		SIZE			szSpecial;
		PNES_GITEM		pSpecial;
		BOOL			fDisableEditor;
		std::vector<BOOL>	vHilite;
	} m_Canvas;

	struct
	{
		SIZE		sz;
		UINT		scrollX;
		UINT		scrollY;
		BOOL		fVertical;
		BOOL		fHorizontal;
	} m_scroll;

	struct
	{
		BOOL		fObjectCatched;
		BOOL		fCatchEnemies;
		BOOL		fCatchJarEnemies;
		POINT		ptDelta;
		DWORD		dwCatchedObject;
	} m_Catch;

	static			LRESULT CALLBACK ControlProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	VOID			OnErase(HDC hDC);
	VOID			OnKeyDown(UINT vKey, USHORT uScanCode);
	VOID			OnMouseMove(UINT uCtl, SHORT x, SHORT y);
	VOID			OnMouseDown(UINT uCtl, SHORT x, SHORT y);
	VOID			OnMouseUp(UINT uCtl, SHORT x, SHORT y);
	VOID			OnContextMenu();
	VOID			OnResize();
	VOID			OnDeleteItem();
	VOID			OnChangeBg(int code);
	void			OnNextItem(BOOL fPrevious);

	INT_PTR			DoNotify(UINT uCode, DWORD i, UINT x, UINT y);
	VOID			SetScroll();
	VOID			UpdateScroll(UINT uScrollType, USHORT uPos, USHORT uReq);
	CLvlDraw(LPCREATESTRUCT lpcs, HWND hWnd);
	~CLvlDraw();

	VOID			AllocCanvas(UINT cx, UINT cy, INT fx);
	BOOL			SetCanvasEnemyItem(PNES_GITEM pItem, UINT x, UINT y, BOOL fHiddenEnemy = FALSE);
	BOOL			EraseEnemyCanvas();
	BOOL			EraseItemsCanvas();
	BOOL			GetCanvasEnemyItem(PNES_GITEM pItem, UINT x, UINT y);
	VOID			DestroyCanvas();
	BOOL			SetCanvasItem(PNES_GITEM pItem, UINT x, UINT y);
	BOOL			SetCanvasNullItem(UINT x, UINT y);
	BOOL			IsCanvasItemUsed(UINT x, UINT y);
	BOOL			IsCanvasLineContainLayerLimit(UINT x, UINT y, UINT cx, INT iLayer);
	BOOL			GetCanvasItem(UINT x, UINT y, PNES_GITEM nItem);
	BOOL			SetSpecialCanvasItem(UINT x, UINT y, PNES_GITEM nItem);
	BOOL			GetSpecialCanvasItem(UINT x, UINT y, PNES_GITEM nItem);

	BOOL			DrawCanvas();
	BOOL			DrawSpecialCanvas();
	VOID			RedrawBuffer(BOOL fSetScroll = TRUE);
	BOOL			RedrawLevel(BOOL fEraseEnemies = FALSE);
	VOID			UseGamma(UINT gammaIndex, UINT tplIndex, UINT tplGrIndex);
	VOID			InitSize(COLORREF clr, UINT cx, UINT cy, UINT pixX, UINT pixY);
	VOID			Draw(UINT index, UINT x, UINT y, UINT cx, UINT cy, UINT dx, UINT dy, BOOL fSelected = FALSE);
	VOID			DrawGamma(UINT gammaIndex, UINT index, UINT x, UINT y, UINT cx, UINT cy, UINT dx, UINT dy, BOOL fSelected = FALSE, BOOL fHilited = FALSE);
	VOID			DrawGrGamma(BYTE index, UINT x, UINT y, BOOL fSelected = FALSE, BOOL fHilited = FALSE);
	VOID			DrawSpecialGamma(UINT gammaIndex, UINT index, UINT x, UINT y, UINT cx, UINT cy, UINT dx, UINT dy, BOOL fSelected = FALSE);

	BOOL			DrawObjectEx(USHORT item, USHORT fx, USHORT uObjectType, CNesItem & x);
	BOOL			DrawSingleObjectEx(USHORT item, USHORT fx, CNesItem & x);
	BOOL			DrawHorzObjectEx(USHORT item, USHORT fx, CNesItem & x);
	BOOL			DrawVertObjectEx(USHORT item, USHORT fx, CNesItem & x, BOOL fLimitByLayer = FALSE, BOOL fDirection = FALSE);	// fDirection: TRUE - to top, FALSE - to down
	BOOL			DrawHorzGrObjectEx(USHORT item, CNesItem & x);
	BOOL			DrawVertGrObjectEx(USHORT item, CNesItem & x, BOOL fLimitByLayer = FALSE);
	BOOL			DrawMasvObjectEx(USHORT item, USHORT fx, CNesItem & x, BOOL fLimitByLayer = FALSE);
	BOOL			DrawSpecialObjectEx(USHORT item, USHORT fx, CNesItem & x);
	BOOL			DrawGroundEx(USHORT item, USHORT fx, CNesItem & x);
	BOOL			DrawPyramidEx(USHORT item, USHORT fx, CNesItem & x);
	BOOL			DrawDesertEntrance(USHORT item, USHORT fx, CNesItem & x);
	BOOL			DrawRedBg(USHORT item, USHORT fx, CNesItem & x);
	BOOL			DrawStarBg(USHORT item, USHORT fx, CNesItem & x);

	friend void InitLevelDraw();

public:
	VOID			LoadBmp(HINSTANCE hInstance, LPTSTR pszResource, UINT index, UINT cx, UINT cy);
	VOID			LoadGamma(HINSTANCE hInstance, LPTSTR pszResource);
	VOID			UseEnemyMask(UINT indexOfMask, UINT indexOfEnemy);
	BOOL			DrawLevelEx(DWORD dwCurLevel, CNesEditor * ed);
	BOOL			SetCatchCanvas(BOOL fEnemiesCanvas, BOOL fJarEnemies);
	void			SetCursorMode(BOOL fMove);
	VOID			SelectPos(int page, BYTE pos);
	UINT			GetSelectedItem();
	VOID			RequestRedraw(BOOL fWithEnemies = FALSE);
};