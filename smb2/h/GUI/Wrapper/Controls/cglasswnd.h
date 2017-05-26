
#define	GLASSOP					TEXT("__glassctl")
#define GLASSPTR(wnd)			((CGlassCtl*) GetProp(wnd, GLASSOP))
#define SETGLASS(wnd, _this)	(SetProp(wnd, GLASSOP, (HANDLE) _this))
#define DELGLASS(wnd)			(RemoveProp(wnd, GLASSOP))

typedef union _tagWRGB
{
	struct
	{
		unsigned char red;
		unsigned char green;
		unsigned char blue;
		unsigned char alpha;
	} color;
	ULONG uColor;
} WRGB, *PWRGB;

class CGlassWnd;

class CGlassCtl
{
	HWND					m_hWnd;
	WNDPROC					m_pfnProc;

	CGlassCtl(HWND hControl, BYTE alpha, USHORT cx = 100, USHORT cy = 100);
	~CGlassCtl();

	static LRESULT CALLBACK	CtlWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	struct
	{
		HDC					hOriginalDC;
		HBITMAP				hOldBitmap;
		HDC					hBuffer;
		HBITMAP				hBufferBmp;
		HANDLE				pBuffer;
		PWRGB				pLockedBuffer;
		BYTE				alpha;
		SIZE				sz;
	} m_gBuffer;

	VOID					CreateBuffer(USHORT cx, USHORT cy);
	VOID					ResizeBuffer(USHORT cx, USHORT cy);
	VOID					ReleaseBuffer();
	PWRGB					Buffer();
	VOID					OnPaintProc(HWND hWnd, HDC hDC);

	PWRGB					Buffer(HDC, RECT);
	VOID					WriteToDevice(PWRGB pBuffer, HDC hDC, RECT);

	friend CGlassWnd;
};

class CGlassWnd
{
	HWND					m_hParent;
	vector<CGlassCtl*>		m_vCtlList;

	VOID					SubclassChilds(HWND hParent, BYTE alpha);

public:
	CGlassWnd(HWND hDialog, BYTE alpha = 255);
	~CGlassWnd();
};

