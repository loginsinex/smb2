#include "precomp.h"

CGlassWnd::CGlassWnd(HWND hWnd, BYTE alpha)
{
	m_hParent = hWnd;
	SubclassChilds(hWnd, alpha);
}

CGlassWnd::~CGlassWnd()
{
	for(vector<CGlassCtl*>::iterator v = m_vCtlList.begin(); v != m_vCtlList.end(); v++)
		delete (*v);
}

CGlassCtl::CGlassCtl(HWND hWnd, BYTE alpha, USHORT cx, USHORT cy)
{
	m_hWnd = hWnd;
	m_gBuffer.pBuffer = NULL;
	m_gBuffer.sz.cx = m_gBuffer.sz.cy = 0;
	m_gBuffer.hOriginalDC = NULL;
	m_gBuffer.hOldBitmap = NULL;
	m_gBuffer.hBuffer = NULL;
	m_gBuffer.hBufferBmp = NULL;
	m_gBuffer.pBuffer = NULL;
	m_gBuffer.alpha = alpha;
	SETGLASS(hWnd, this);

	m_pfnProc = (WNDPROC) SetWindowLong(hWnd, GWL_WNDPROC, (LONG) CGlassCtl::CtlWndProc);

	RECT rc = { 0 };
	GetClientRect(hWnd, &rc);
	USHORT lcx = (USHORT) 0xFFFFL & rc.right, lcy = (USHORT) 0xFFFFL & rc.bottom;
	CreateBuffer(max(lcx,cx), max(lcy,cy));
}

CGlassCtl::~CGlassCtl()
{
	ReleaseBuffer();
	SetWindowLong(m_hWnd, GWL_WNDPROC, (LONG) m_pfnProc);
	DELGLASS(m_hWnd);
}

VOID CGlassCtl::CreateBuffer(USHORT cx, USHORT cy)
{
	if ( m_gBuffer.pBuffer )
		return;

	ULONG dwSize = cx * cy;
	m_gBuffer.pLockedBuffer = FALSE;

	m_gBuffer.hOriginalDC = GetDC(NULL);
	m_gBuffer.hBufferBmp = CreateCompatibleBitmap(m_gBuffer.hOriginalDC, cx, cy);
	m_gBuffer.hBuffer = CreateCompatibleDC(m_gBuffer.hOriginalDC);
	m_gBuffer.hOldBitmap = (HBITMAP) SelectObject(m_gBuffer.hBuffer, m_gBuffer.hBufferBmp);
	m_gBuffer.pBuffer = LocalAlloc(LMEM_ZEROINIT | LMEM_MOVEABLE, dwSize*sizeof(WRGB));
	m_gBuffer.sz.cx = cx;
	m_gBuffer.sz.cy = cy;
}

VOID CGlassCtl::ResizeBuffer(USHORT cx, USHORT cy)
{
	if ( !m_gBuffer.pBuffer ) 
		return;

	cx += 10;
	cy += 10;

	LONG lcx = max(m_gBuffer.sz.cx, cx), lcy = max(m_gBuffer.sz.cy, cy);

	if ( lcx > m_gBuffer.sz.cx || lcy > m_gBuffer.sz.cy )
	{
		if ( m_gBuffer.pLockedBuffer )
		{
			LocalUnlock(m_gBuffer.pBuffer);
			m_gBuffer.pLockedBuffer = NULL;
		}

		SelectObject(m_gBuffer.hBuffer, m_gBuffer.hOldBitmap);
		DeleteObject(m_gBuffer.hBufferBmp);
		DeleteDC(m_gBuffer.hBuffer);
		m_gBuffer.hBuffer = CreateCompatibleDC(m_gBuffer.hOriginalDC);
		m_gBuffer.hBufferBmp = CreateCompatibleBitmap(m_gBuffer.hOriginalDC, lcx, lcy);
		m_gBuffer.hOldBitmap = (HBITMAP) SelectObject(m_gBuffer.hBuffer, m_gBuffer.hBufferBmp);
		HANDLE ret = LocalReAlloc(m_gBuffer.pBuffer, lcx*lcy*sizeof(WRGB), LMEM_ZEROINIT | LMEM_MOVEABLE);
		SIZE_T size = LocalSize(ret);
		m_gBuffer.sz.cx = (USHORT) ( lcx & 0xFFFFL );
		m_gBuffer.sz.cy = (USHORT) ( lcy & 0xFFFFL );
	}
}

VOID CGlassCtl::ReleaseBuffer()
{
	if ( !m_gBuffer.pBuffer ) 
		return;

	if ( m_gBuffer.pLockedBuffer )
	{
		LocalUnlock(m_gBuffer.pBuffer);
		m_gBuffer.pLockedBuffer = NULL;
	}

	SelectObject(m_gBuffer.hBuffer, m_gBuffer.hOldBitmap);
	DeleteObject(m_gBuffer.hBufferBmp);
	DeleteDC(m_gBuffer.hBuffer);
	ReleaseDC(NULL, m_gBuffer.hOriginalDC);
	LocalFree(m_gBuffer.pBuffer);
}

PWRGB CGlassCtl::Buffer()
{
	if ( !m_gBuffer.pBuffer )
		return NULL;

	if ( m_gBuffer.pLockedBuffer )
		return m_gBuffer.pLockedBuffer;
	
	m_gBuffer.pLockedBuffer = (PWRGB) LocalLock(m_gBuffer.pBuffer);
	return m_gBuffer.pLockedBuffer;
}

VOID CGlassWnd::SubclassChilds(HWND hParent, BYTE alpha)
{
	HWND hChild = GetWindow(hParent, GW_CHILD);

	while(hChild)
	{
		if ( !GLASSPTR(hChild) )
		{
			CGlassCtl * pCtl = new CGlassCtl(hChild, alpha);
			m_vCtlList.push_back(pCtl);
		}
		SubclassChilds(hChild, alpha);
		hChild = GetWindow(hChild, GW_HWNDNEXT);
	}
}

LRESULT CALLBACK CGlassCtl::CtlWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CGlassCtl * This = GLASSPTR(hWnd);
	if ( !This )
		return DefWindowProc(hWnd, uMsg, wParam, lParam);

	WNDPROC pfnProc = This->m_pfnProc;
	if ( !pfnProc )
		return DefWindowProc(hWnd, uMsg, wParam, lParam);

	//return pfnProc(hWnd, uMsg, wParam, lParam);

	switch(uMsg)
	{
	case WM_SIZING:
	case WM_SIZE:
		{
			RECT rc = { 0 };
			GetClientRect(hWnd, &rc);
			This->ResizeBuffer((USHORT) 0xFFFFL & rc.right, (USHORT) 0xFFFFL & rc.bottom);
			break;
		}
	case WM_ERASEBKGND:
		{
			LRESULT ret = pfnProc(hWnd, uMsg, WPARAM(This->m_gBuffer.hBuffer), lParam);
			This->OnPaintProc(hWnd, (HDC) wParam);
			return ret;
		}
	//case WM_NCPAINT:
	//	{
	//		SendMessage(hWnd, WM_PRINT, PRF_ERASEBKGND | PRF_CLIENT | PRF_NONCLIENT, (LPARAM) This->m_gBuffer.hBuffer);

	//		HDC hDC = GetWindowDC(hWnd);
	//		RECT rc = { 0 };
	//		GetWindowRect(hWnd, &rc);
	//		rc.bottom -= rc.top; rc.right -= rc.left;
	//		This->OnPaintProc(hWnd, hDC);
	//		ReleaseDC(hWnd, hDC);
	//		return 0;
	//	}
	case WM_PAINT:
		{
			RECT rc = { 0 };
			GetClientRect(hWnd, &rc);

			PAINTSTRUCT ps = { 0 };
			HDC hDC = BeginPaint(hWnd, &ps);
			pfnProc(hWnd, uMsg,	WPARAM(This->m_gBuffer.hBuffer), lParam);
			//SendMessage(hWnd, WM_PRINT, PRF_ERASEBKGND | PRF_CLIENT | PRF_NONCLIENT | PRF_CHILDREN, (LPARAM) This->m_gBuffer.hBuffer);
			This->OnPaintProc(hWnd, hDC);
			//BitBlt(hDC, 0, 0, rc.right, rc.bottom, This->m_gBuffer.hBuffer, 0, 0, SRCCOPY);
			EndPaint(hWnd, &ps);
			return 0;
		}
	}

	return pfnProc(hWnd, uMsg, wParam, lParam);
}

VOID CGlassCtl::OnPaintProc(HWND hWnd, HDC hDC)
{
	RECT rc = { 0 };
	GetClientRect(hWnd, &rc);

	PWRGB pBuffer = Buffer(m_gBuffer.hBuffer, rc);
	if ( pBuffer )
	{
		LONG lSize = rc.right * rc.bottom;

		for(LONG cy = 0; cy < rc.bottom; ++cy)
		{
			LONG acx = cy*rc.right;
			for(LONG cx = 0; cx < rc.right; ++cx)
			{
				pBuffer[acx].color.alpha = 255 ;
				acx++;
			}
		}

		WriteToDevice(pBuffer, hDC, rc) ; //m_gBuffer.hBuffer, rc);
	}

}

PWRGB CGlassCtl::Buffer(HDC hDC, RECT rc)
{
	BITMAPINFO bmpi = { 0 };

	PWRGB pBuffer = Buffer();
	bmpi.bmiHeader.biSize = sizeof(bmpi.bmiHeader);			// sizeof(bmiHeader)
	bmpi.bmiHeader.biWidth = rc.right;
	bmpi.bmiHeader.biHeight = -rc.bottom;
	bmpi.bmiHeader.biBitCount = 32;
	bmpi.bmiHeader.biPlanes = 1;			// always 1
	bmpi.bmiHeader.biSizeImage = rc.right * rc.bottom;

	ULONG ugdb =  GetDIBits(hDC, m_gBuffer.hBufferBmp, 0, rc.bottom, pBuffer, &bmpi, DIB_RGB_COLORS);
	
	return ( 0 != ugdb ? pBuffer : NULL );
}

VOID CGlassCtl::WriteToDevice(PWRGB pBuffer, HDC hDC, RECT rc)
{
	BITMAPINFO bmpi = { 0 };
	bmpi.bmiHeader.biSize = sizeof(bmpi.bmiHeader);			// sizeof(bmiHeader)
	bmpi.bmiHeader.biWidth = rc.right;
	bmpi.bmiHeader.biHeight = -rc.bottom;
	bmpi.bmiHeader.biBitCount = 32;
	bmpi.bmiHeader.biPlanes = 1;			// always 1
	bmpi.bmiHeader.biSizeImage = rc.right * rc.bottom;

	SetDIBitsToDevice(hDC, 0, 0, rc.right, rc.bottom, 0, 0, 0, rc.bottom, pBuffer, &bmpi, DIB_RGB_COLORS);
}