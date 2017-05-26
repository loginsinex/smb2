
#include "precomp.h"

CBufView::CBufView(LPCREATESTRUCT lpcs)
	:
	m_pwndProc( NULL )
{
	m_hInstance = lpcs->hInstance;
	m_cs = *lpcs;

	m_buffer.sz.cx = GetSystemMetrics(SM_CXSCREEN);
	m_buffer.sz.cy = GetSystemMetrics(SM_CYSCREEN);

	m_buffer.hDCDesktop = GetDC(GetDesktopWindow());
	m_buffer.hDC = CreateCompatibleDC(m_buffer.hDCDesktop);
	// m_buffer.hBmp = CreateCompatibleBitmap(m_buffer.hDCDesktop, m_buffer.sz.cx, m_buffer.sz.cy);
	ULONG uSize = m_buffer.sz.cx * m_buffer.sz.cy * sizeof(WRGBQUAD);
	BITMAPINFO & bmi = m_buffer.bmi;
	bmi.bmiHeader.biSize = sizeof(bmi);
	bmi.bmiHeader.biWidth = m_buffer.sz.cx;
	bmi.bmiHeader.biHeight = -m_buffer.sz.cy;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biSizeImage = uSize;

	m_buffer.hBmp = CreateDIBSection( m_buffer.hDC, &bmi, DIB_RGB_COLORS, (PVOID*) &m_buffer.pvBits, NULL, 0 );

	m_buffer.hOldBmp = (HBITMAP) SelectObject(m_buffer.hDC, m_buffer.hBmp);

	m_buffer.brush.hOldBrush = m_buffer.brush.hBrush = NULL;
	m_buffer.pen.hOldPen = m_buffer.pen.hPen = NULL;
	m_buffer.font.hOldFont = m_buffer.font.hFont = NULL;
}

CBufView::~CBufView()
{
	SetPen(NULL);
	SetBrush(NULL);
	SetFont(NULL);
	SelectObject(m_buffer.hDC, m_buffer.hOldBmp);
	DeleteObject(m_buffer.hBmp);
	DeleteDC(m_buffer.hDC);
	ReleaseDC(GetDesktopWindow(), m_buffer.hDCDesktop);
}

VOID InitBufView(HINSTANCE hInstance, LPTSTR pszClass)
{
	WNDCLASS cs = { 0 };
	cs.cbWndExtra = 8;
	cs.hbrBackground = (HBRUSH) ( COLOR_WINDOW + 1 );
	cs.hCursor = LoadCursor(NULL, IDC_ARROW);
	cs.hInstance = hInstance;
	cs.lpszClassName = pszClass;
	cs.lpfnWndProc = CBufView::BufViewProc;
	cs.style = CS_DBLCLKS;

	RegisterClass(&cs);
}

VOID CBufView::Assign(BVPROC pwndProc, PVOID pParam)
{
	m_pwndProc = pwndProc;
	m_pParam = pParam;
}

LRESULT CALLBACK CBufView::BufViewProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CBufView * This = (CBufView*) GetWindowLong(hWnd, 4);
	if ( !This )
	{
		switch(uMsg)
		{
		case WM_CREATE:
			{
				This = new CBufView((LPCREATESTRUCT) lParam);
				This->m_hWnd = hWnd;
				This->m_hDC = GetDC(hWnd);
				SetWindowLong(hWnd, 4, (LPARAM) This);
				return 0;
			}
		default:
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
		}
	}
	
	if ( WM_NCDESTROY == uMsg && This )
	{
		This->OnMessage(uMsg, wParam, lParam);
		ReleaseDC(hWnd, This->m_hDC);
		delete This;
		return 0;
	}

	return This->OnMessage(uMsg, wParam, lParam);
}

LRESULT CBufView::OnMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_SIZE:
	case WM_SIZING:
		{
			if ( WM_SIZING == uMsg )
			{
				LPRECT lprc = (LPRECT) lParam;
				m_cs.cx = lprc->right - lprc->left;
				m_cs.cy = lprc->bottom - lprc->top;
			}
			else
			{
				m_cs.cx = LOWORD(lParam);
				m_cs.cy = HIWORD(lParam);
			}
			Refresh();

			if ( m_pwndProc )
				return m_pwndProc(m_hWnd, uMsg, wParam, lParam, m_pParam);

			break;
		}
	case WM_ERASEBKGND:
		{
			RenderOnDevice((HDC) wParam);
			break;
		}
	case BVM_GETINSTANCE:
		{
			return (LRESULT) this;
		}
	case WM_SETFONT:
		{
			m_hGlobalFont = (HFONT) wParam;
			if ( m_pwndProc )
				return m_pwndProc(m_hWnd, uMsg, wParam, lParam, m_pParam);

			break;
		}
	case WM_GETFONT:
		{
			return (LRESULT) m_hGlobalFont;
		}
	default:
		{
			if ( m_pwndProc )
				return m_pwndProc(m_hWnd, uMsg, wParam, lParam, m_pParam);
			else
				return DefWindowProc(m_hWnd, uMsg, wParam, lParam);
		}
	}
	return 0;
}

VOID CBufView::RenderOnDevice(HDC hDC)
{
	RECT rc = { 0 };
	if ( OnMessage(BVM_GETUPDATEAREA, 0, (LPARAM) &rc) )
		BitBlt(hDC, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, m_buffer.hDC, rc.left, rc.top, SRCCOPY);
	else
		SetDIBitsToDevice(hDC, 0, 0, m_cs.cx, m_cs.cy, 0, 0, 0, m_cs.cy, m_buffer.pvBits, &m_buffer.bmi, DIB_RGB_COLORS);
}

HWND CBufView::GetWindowHandle()
{
	return m_hWnd;
}

HDC CBufView::GetBufferHandle()
{
	return m_buffer.hDC;
}

VOID CBufView::Refresh()
{
	RenderOnDevice(m_hDC);
}

VOID CBufView::SetPen(HPEN hPen, BOOL fDestroy)
{
	if ( !hPen )
	{
		if ( !m_buffer.pen.hOldPen )
			return;

		HPEN hPrevPen = (HPEN) SelectObject(m_buffer.hDC, m_buffer.pen.hOldPen);
		m_buffer.pen.hOldPen = m_buffer.pen.hPen = NULL;
		if ( fDestroy )
			DeleteObject( hPrevPen );

		return;
	}

	HPEN hPrevPen = (HPEN) SelectObject(m_buffer.hDC, hPen);
	if ( !hPrevPen )
		return;

	if ( !m_buffer.pen.hOldPen )
		m_buffer.pen.hOldPen = hPrevPen;
	else
		if ( fDestroy )
			DeleteObject(hPrevPen);

	m_buffer.pen.hPen = hPen;
}

VOID CBufView::SetBrush(HBRUSH hBrush, BOOL fDestroy)
{
	if ( !hBrush )
	{
		if ( !m_buffer.brush.hOldBrush )
			return;

		HBRUSH hPrevBrush = (HBRUSH) SelectObject(m_buffer.hDC, m_buffer.brush.hOldBrush);
		m_buffer.brush.hOldBrush = m_buffer.brush.hBrush = NULL;
		if ( fDestroy )
			DeleteObject( hPrevBrush );

		return;
	}

	HBRUSH hPrevBrush = (HBRUSH) SelectObject(m_buffer.hDC, hBrush);
	if ( !hPrevBrush )
		return;

	if ( !m_buffer.brush.hOldBrush )
		m_buffer.brush.hOldBrush = hPrevBrush;
	else
		if ( fDestroy )
			DeleteObject(hPrevBrush);

	m_buffer.brush.hBrush = hBrush;
}

VOID CBufView::SetFont(HFONT hFont, BOOL fDestroy)
{
	if ( !hFont )
	{
		if ( !m_buffer.font.hOldFont )
			return;

		HFONT hPrevFont = (HFONT) SelectObject(m_buffer.hDC, m_buffer.font.hOldFont);
		m_buffer.font.hOldFont = m_buffer.font.hFont = NULL;
		if ( fDestroy )
			DeleteObject( hPrevFont );

		return;
	}

	HFONT hPrevFont = (HFONT) SelectObject(m_buffer.hDC, hFont);
	if ( !hPrevFont )
		return;

	if ( !m_buffer.font.hOldFont )
		m_buffer.font.hOldFont = hPrevFont;
	else
		if ( fDestroy )
			DeleteObject(hPrevFont);

	m_buffer.font.hFont = hFont;
}

VOID CBufView::Erase(HBRUSH hBrush, BOOL fDestroy)
{
	RECT rc = { 0, 0, m_buffer.sz.cx, m_buffer.sz.cy };
	FillRect(m_buffer.hDC, &rc, hBrush);
	if ( fDestroy )
		DeleteObject( hBrush );
}

CBufView::operator HDC()
{
	return GetBufferHandle();
}

CBufView::operator HWND()
{
	return GetWindowHandle();
}