// CLASS:			CBitmap
// DESCRIPTION:		Bitmaps helper. Replaces some GDI API functions (because its works very ugly)
// REVISION:		Created Mar-24-2007 by ALXR

//	---------------------------------------------------------------------------------------------

#ifndef _DEBUG
#define DebugInfo(x, y)			(y)
#endif

#define ALPHA_COLOR(front,back,alpha)	(alpha*(front-back)/255+back)
#define XCHGCOLORREF(_x)              ((((_x)&0xFF00FF00L)|(((_x)&0x00FF0000L)>>16)|(((_x)&0x000000FFL)<<16)))

#define CBMP_ALPHA_RENDER				0
#define CBMP_TRANSPARENT				1
#define CBMP_ALPHA_CHANNEL_FROM_SRC		2

#define GRADIENT_FILL_RECT_D			0x100

#define ALIGN(p, s)						(p + ( (p%s) > 0 ? s-(p%s) : 0 ))

#define PTINSIZE(x, y, w, h)			( (x) >= 0 && (y) >= 0 && (x) < (w) && (y) < (h) )

typedef struct _WRGBQUAD
{
	BYTE rgbRed;
	BYTE rgbGreen;
	BYTE rgbBlue;
	BYTE rgbAlpha;
} WRGBQUAD, *LPWRGBQUAD;

class CBitmap;
template <class CLR> class CBitmapRow;

template <class CLR> class CBitmapDot
{
	CBitmap & m_bmp;
	CLR			m_wfake;
	BOOL		m_fIsInRect;
	int			m_x;

public:
	CBitmapDot(CBitmap & bmp, int x);
	CLR & operator[](int y);

	friend class CBitmapRow<CLR>;
};

template <class CLR> class CBitmapRow
{
	CBitmap & m_bmp;
	WRGBQUAD	m_wfake;
	CBitmapDot<CLR>	m_dot;

public:
	CBitmapRow(CBitmap & bmp);
	CBitmapDot<CLR> & operator[](int x);
};

class CBitmapBrush
{
	BITMAPINFOHEADER *		m_pbmi;
	HBRUSH					m_hBrush;
	BOOL					CreateBrush(CBitmap & bmp);
	void					DeleteBr();

public:
	CBitmapBrush(CBitmap & bmp);
	CBitmapBrush();
	~CBitmapBrush();

	HBRUSH					Create(CBitmap & bmp);
	HBRUSH					Brush();
};

class CDCBuffer
{
	BOOL			m_fCreated;
	struct
	{
		HDC			hDCScreen;
		HDC			hDC;
		HBITMAP		hBmp;
		HBITMAP		hOldBmp;
		SIZE		sz;
	} m_Buffer;

	BOOL			Initialize();

public:
	CDCBuffer();
	~CDCBuffer();
	CDCBuffer(INT x, INT y);
	BOOL			Create(INT x, INT y);
	BOOL			Resize(INT x, INT y);

	BOOL			BitBltSrc(HDC hDÑ, int x, int y, int cx, int cy, int x1, int y1, DWORD rop);
	BOOL			BitBltDst(int x, int y, int cx, int cy, HDC hDC, int x1, int y1, DWORD rop);
	HDC				Handle();
	HDC				SrcHandle();
	HBITMAP			Bitmap();
	BOOL			CreateBitmap(CBitmap & bm);
	operator HDC();
};

class CBitmap
{
	DWORD				*m_pvBits;		// bitmap bits
	INT					m_szX, m_szY;	// bitmap size
	UINT				m_uSize;		// bitmap size in byts
	mutable CRITICAL_SECTION	m_lpcrs;		// critical section for reallocations
	BOOL				m_bLocked;		// bitmap bits are locked

	HDC					m_hdesktopDC;	// hDC of desktop
	HDC					m_hbufferDC;	// device context in memory
	HBITMAP				m_hbufferBm;	// bitmap for text render
	HBITMAP				m_holdBm;		// previous bitmap from m_hbufferDC

	// shadow buffer
	PBYTE				*m_bmAlphaMask;
	PBYTE				*m_bmAlphaRestored;
	BOOL				m_fShadow;

	void				CreateShadowBuffer();
	void				DestroyShadowBuffer();

#ifdef _DEBUG
	void				DebugInfo(LPSTR pszFuncName, UINT funcRet);
#endif

public:
	BOOL				LoadBitmap(HINSTANCE hInstance, LPCTSTR lpBitmapName);
	BOOL				LoadIcon(HICON hIcon);
	BOOL				Create(INT x, INT y);
	BOOL				Resize(INT x, INT y);
	VOID				Destroy();
	BOOL				RenderToDevice(HDC hDC, UINT xDest, UINT yDest, UINT cxDest, UINT cyDest, UINT xSrc, UINT ySrc);
	HBITMAP				ConvertToHandle(INT y = 0);
//	VOID				SetBitmapBits(HBITMAP hBmpSrc);
	VOID				GradientFill(INT x, INT y, INT cx, INT cy, COLORREF crColor1, COLORREF crColor2, DWORD dwFlags);
	VOID				GradientFillEx(INT x, INT y, INT cx, INT cy, COLORREF crColor1, COLORREF crColor2, BYTE bAlpha1, BYTE bAlpha2, DWORD dwFlags);
	VOID				RenderRawBitmap(CONST WRGBQUAD* pvBits, INT src_width, INT src_height, BYTE bAlphaConstant, BOOL bUseSourceAlphaChannel, INT x, INT y, INT cx, INT cy, INT dest_x, INT dest_y, DWORD dwFlags);
	VOID				CopyRawRect(CONST WRGBQUAD * pvBits, INT src_cx, INT src_cy, INT x, INT y, INT cx, INT cy, INT dest_x, INT dest_y);
	VOID				RenderBitmap(HBITMAP hSourceBmp, BYTE bAlphaConstant, INT x, INT y, INT cx, INT cy, INT dest_x, INT dest_y, COLORREF crTransparent, DWORD dwFlags);
	VOID				RenderRawTransparentBitmap(CONST WRGBQUAD *pvBits, INT src_width, INT src_height, INT x, INT y, INT cx, INT cy, INT dest_x, INT dest_y, COLORREF crTransparent, DWORD dwFlags);
	COLORREF			Dot(INT x, INT y);
	VOID				Dot(INT x, INT y, COLORREF crColor);
	VOID				FillRect(INT x, INT y, INT cx, INT cy, COLORREF crColor, COLORREF crBorder, INT iWidth, BYTE fInnerAlpha = 255);
	VOID				FillRectTransparentBits(WRGBQUAD * pvBits, INT src_width, INT src_height, INT chunk_X, INT chunk_Y, INT chunk_CX, INT chunk_CY, INT start_X, INT start_Y, BOOL bVertical, INT count, COLORREF crTransparent);
	VOID				FillRectBits(WRGBQUAD * pvBits, INT src_width, INT src_height, INT chunk_X, INT chunk_Y, INT chunk_CX, INT chunk_CY, INT start_X, INT start_Y, BOOL bVertical, INT count);
	VOID				GrayScale();
	VOID				CopyRect(HBITMAP hSrcBitmap, INT x, INT y, INT cx, INT cy, INT dest_x, INT dest_y);
	VOID				RenderText(INT x, INT y, INT cx, INT cy, LPTSTR pszText, HFONT hFont, BOOL bTransparent, COLORREF crTextColor, COLORREF crBackColor, DWORD dwFlags);
	VOID				Copy(CBitmap * pSource);
	VOID				AlphaGradient(INT x, INT y, INT cx, INT cy, DWORD dwFlags, INT bDirection = 0);
	VOID				ResetAlpha(BYTE bAlpha = 255);
	VOID				CreateFromBitmap(HBITMAP hBmp);
	HBRUSH				CreateBrush();
	HDC					LockDC();
	void				FreeDC();
	void				DropShadow(CBitmap & bmp, int width, BOOL fRectangle = FALSE, BOOL fRectUpSide = FALSE);
	void				DrawShadow(int xLeft, int yTop, int xWidth, int yHeight, CBitmap & bmShadow, int x, int y, BYTE bAlpha = 255, BOOL fRestoreColor = FALSE);

	void				ContrastBitmap(CBitmap & bmpTarget, double level);
	void				ContrastBits(double level);

	INT					Width()
	{
		return m_szX;
	}

	INT					Height()
	{
		return m_szY;
	}

	WRGBQUAD *			LockRawBits();

	VOID				UnlockRawBits()
	{
		//if (!m_uSize) return;
		//m_bLocked = false;
		//LocalUnlock(m_pvBits);
	}

	CBitmapRow<COLORREF>	clr;
	CBitmapRow<WRGBQUAD>	rgb;

	CBitmap();

	~CBitmap();
	std::map<int,HGDIOBJ>	m_vObjects;

	void				InitBuffer();

	friend				CBitmapRow<COLORREF>;
	friend				CBitmapRow<WRGBQUAD>;
	friend				CBitmapDot<COLORREF>;
	friend				CBitmapDot<WRGBQUAD>;
};

template <class CLR> CBitmapDot<CLR>::CBitmapDot(CBitmap & bmp, int x)
	: m_bmp(bmp), m_x(x)
{
	ZeroMemory(&m_wfake, sizeof(m_wfake));
	m_fIsInRect = PTINSIZE(x, 0, bmp.m_szX, bmp.m_szY);
}

template <class CLR> CLR & CBitmapDot<CLR>::operator[](int y)
{
	if ( m_fIsInRect && y >= 0 && y < m_bmp.m_szY )
		return ((CLR*)m_bmp.m_pvBits)[m_x + m_bmp.m_szX * y];
	else
	{
		ZeroMemory(&m_wfake, sizeof(m_wfake));
		return m_wfake;
	}
}

template <class CLR> CBitmapRow<CLR>::CBitmapRow(CBitmap & bmp)
	: m_bmp(bmp), m_dot(bmp, 0)
{
	ZeroMemory(&m_wfake, sizeof(m_wfake));
}

template <class CLR> CBitmapDot<CLR> & CBitmapRow<CLR>::operator[](int x)
{
	m_dot.m_x = x;
	m_dot.m_fIsInRect = PTINSIZE(x, 0, m_bmp.m_szX, m_bmp.m_szY);
	return m_dot;
}
