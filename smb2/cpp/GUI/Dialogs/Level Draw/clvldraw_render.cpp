
#include "precomp.h"

VOID CLvlDraw::Draw(UINT index, UINT x, UINT y, UINT cx, UINT cy, UINT dx, UINT dy, BOOL fSelected)
{
	if ( index >= 10 )
		return;

	RECT rc = { x*m_sz[index].cx, y*m_sz[index].cy, cx*m_sz[index].cx, cy*m_sz[index].cy };
	RECT rc_dc = { dx*m_dcsz.cx, dy*m_dcsz.cy, rc.right, rc.bottom };

	if ( 
		rc.left > m_Bmp.bmTpl[index].Width() || rc.top > m_Bmp.bmTpl[index].Height() ||
		rc.left+rc.right > m_Bmp.bmTpl[index].Width() || rc.top+rc.bottom > m_Bmp.bmTpl[index].Height() ||
		rc_dc.left > m_Bmp.bmLevel.Width() || rc_dc.top > m_Bmp.bmLevel.Height() ||
		rc_dc.left+rc_dc.right > m_Bmp.bmLevel.Width() || rc_dc.top+rc_dc.bottom > m_Bmp.bmLevel.Height()
		)
		return;

	HDC hdc = m_Bmp.bmLevel.LockDC();
	TransparentBlt(hdc, 
		rc_dc.left, rc_dc.top, rc.right, rc.bottom, 
		m_Bmp.bmTpl[index].LockDC(), rc.left, rc.top, rc.right, rc.bottom, RGB(0xfe, 0xfe, 0xfe)
		);

	m_Bmp.bmTpl[index].FreeDC();
	m_Bmp.bmLevel.FreeDC();

/*
	WRGBQUAD * bits = m_Bmp.bmTpl[index].LockRawBits();

	m_Bmp.bmLevel.RenderRawTransparentBitmap(
		bits, 
		m_Bmp.bmTpl[index].Width(), m_Bmp.bmTpl[index].Height(), 
		rc.left, rc.top, rc.right, rc.bottom, rc_dc.left, rc_dc.top, 
		0xfefefefe, 0);
*/

	if ( fSelected )
	{
		m_Bmp.bmLevel.RenderRawBitmap(
			m_Bmp.bmGammaTpl.LockRawBits(),
			m_Bmp.bmGammaTpl.Width(), m_Bmp.bmGammaTpl.Height(),
			100, FALSE, m_dcsz.cx*6, m_dcsz.cy*7, 1*m_dcsz.cx, 1*m_dcsz.cy, rc_dc.left, rc_dc.top, 
			0);
		m_Bmp.bmGammaTpl.UnlockRawBits();
	}
	m_Bmp.bmTpl[index].UnlockRawBits();
}

VOID CLvlDraw::DrawGrGamma(BYTE index, UINT x, UINT y, BOOL fSelected, BOOL fHilited)
{
	if ( x >= (UINT) m_Canvas.sz.cx || y >= (UINT) m_Canvas.sz.cy )
		return;

	HDC hdc = m_Bmp.bmLevel.LockDC();
	/*
	WRGBQUAD * bits = m_Bmp.bmGrGammaTpl.LockRawBits();
	m_Bmp.bmLevel.RenderRawTransparentBitmap(
		bits,
		m_Bmp.bmGrGammaTpl.Width(), m_Bmp.bmGrGammaTpl.Height(),
		m_dcsz.cx*index, 0, m_dcsz.cx, m_dcsz.cy, x*m_dcsz.cx, y*m_dcsz.cy,
		0xfefefefe, 0);
	m_Bmp.bmGrGammaTpl.UnlockRawBits();
	*/
	TransparentBlt(hdc, 
		x*m_dcsz.cx, y*m_dcsz.cy, m_dcsz.cx, m_dcsz.cy,
		m_Bmp.bmGrGammaTpl.LockDC(), m_dcsz.cx*index, 0, m_dcsz.cx, m_dcsz.cy,
		RGB(0xfe, 0xfe, 0xfe)
		);

	m_Bmp.bmGrGammaTpl.FreeDC();
	m_Bmp.bmLevel.FreeDC();

	if ( fSelected )
	{
		m_Bmp.bmLevel.RenderRawBitmap(
			m_Bmp.bmGammaTpl.LockRawBits(),
			m_Bmp.bmGammaTpl.Width(), m_Bmp.bmGammaTpl.Height(),
			100, FALSE, m_dcsz.cx*6, m_dcsz.cy*7, m_dcsz.cx, m_dcsz.cy, x*m_dcsz.cx, y*m_dcsz.cy,
			0);
		m_Bmp.bmGammaTpl.UnlockRawBits();
	}

	if ( fHilited )
	{
		m_Bmp.bmLevel.RenderRawBitmap(
			m_Bmp.bmGammaTpl.LockRawBits(),
			m_Bmp.bmGammaTpl.Width(), m_Bmp.bmGammaTpl.Height(),
			100, FALSE, m_dcsz.cx*5, m_dcsz.cy*7, m_dcsz.cx, m_dcsz.cy, x*m_dcsz.cx, y*m_dcsz.cy,
			0);
		m_Bmp.bmGammaTpl.UnlockRawBits();
	}
}

VOID CLvlDraw::DrawGamma(UINT gammaIndex, UINT index, UINT x, UINT y, UINT cx, UINT cy, UINT dx, UINT dy, BOOL fSelected, BOOL fHilited)
{
	if ( index >= MAX_TPL )
		return;

	RECT rc = { x*m_sz[index].cx, y*m_sz[index].cy, cx*m_sz[index].cx, cy*m_sz[index].cy };
	RECT rc_dc = { dx*m_dcsz.cx, dy*m_dcsz.cy, rc.right, rc.bottom };

	if ( 
		rc.left > m_Bmp.bmGammaTpl.Width() || rc.top > m_Bmp.bmGammaTpl.Height() ||
		rc.left+rc.right > m_Bmp.bmGammaTpl.Width() || rc.top+rc.bottom > m_Bmp.bmGammaTpl.Height() ||
		rc_dc.left > m_Bmp.bmLevel.Width() || rc_dc.top > m_Bmp.bmLevel.Height() ||
		rc_dc.left+rc_dc.right > m_Bmp.bmLevel.Width() || rc_dc.top+rc_dc.bottom > m_Bmp.bmLevel.Height()
		)
		return;

	
	HDC hdc = m_Bmp.bmLevel.LockDC();
	TransparentBlt(hdc, dx*m_dcsz.cx, dy*m_dcsz.cy, cx*m_dcsz.cx, cy*m_dcsz.cy,
		m_Bmp.bmGammaTpl.LockDC(), m_dcsz.cx*x, m_dcsz.cy*y, cx*m_dcsz.cx, cy*m_dcsz.cy,
		RGB(0xfe, 0xfe, 0xfe)
		);
	m_Bmp.bmGammaTpl.FreeDC();
	m_Bmp.bmLevel.FreeDC();

	WRGBQUAD * bits = m_Bmp.bmGammaTpl.LockRawBits();
	/*
	m_Bmp.bmLevel.RenderRawTransparentBitmap(
		bits,
		m_Bmp.bmGammaTpl.Width(), m_Bmp.bmGammaTpl.Height(),
		m_dcsz.cx*x, m_dcsz.cy*y, cx*m_dcsz.cx, cy*m_dcsz.cy, dx*m_dcsz.cx, dy*m_dcsz.cy,
		0xfefefefe, 0);
	*/


	if ( fSelected )
	{
		m_Bmp.bmLevel.RenderRawBitmap(
			bits,
			m_Bmp.bmGammaTpl.Width(), m_Bmp.bmGammaTpl.Height(),
			100, FALSE, m_dcsz.cx*6, m_dcsz.cy*7, 1*m_dcsz.cx, 1*m_dcsz.cy, dx*m_dcsz.cx, dy*m_dcsz.cy,
			0);
	}

	if ( fHilited )
	{
		m_Bmp.bmLevel.RenderRawBitmap(
			bits,
			m_Bmp.bmGammaTpl.Width(), m_Bmp.bmGammaTpl.Height(),
			100, FALSE, m_dcsz.cx*5, m_dcsz.cy*7, 1*m_dcsz.cx, 1*m_dcsz.cy, dx*m_dcsz.cx, dy*m_dcsz.cy,
			0);	
	}

	m_Bmp.bmGammaTpl.UnlockRawBits();
}

VOID CLvlDraw::DrawSpecialGamma(UINT gammaIndex, UINT index, UINT x, UINT y, UINT cx, UINT cy, UINT dx, UINT dy, BOOL fSelected)
{
	if ( index >= MAX_TPL )
		return;

	RECT rc = { x*m_sz[index].cx, y*m_sz[index].cy, cx*m_sz[index].cx, cy*m_sz[index].cy };
	RECT rc_dc = { dx*m_dcsz.cx, dy*m_dcsz.cy, rc.right, rc.bottom };

	if ( 
		rc.left > m_Bmp.bmGammaTpl.Width() || rc.top > m_Bmp.bmGammaTpl.Height() ||
		rc.left+rc.right > m_Bmp.bmGammaTpl.Width() || rc.top+rc.bottom > m_Bmp.bmGammaTpl.Height() ||
		rc_dc.left > m_Bmp.bmSpecial.Width() || rc_dc.top > m_Bmp.bmSpecial.Height() ||
		rc_dc.left+rc_dc.right > m_Bmp.bmSpecial.Width() || rc_dc.top+rc_dc.bottom > m_Bmp.bmLevel.Height()
		)
		return;

	HDC hdc = m_Bmp.bmSpecial.LockDC();

	TransparentBlt(
		hdc, dx*m_dcsz.cx, dy*m_dcsz.cy, cx*m_dcsz.cx, cy*m_dcsz.cy,
		m_Bmp.bmGammaTpl.LockDC(), m_dcsz.cx*x, m_dcsz.cy*y, cx*m_dcsz.cx, cy*m_dcsz.cy,
		RGB(0xfe, 0xfe, 0xfe)
		);

	m_Bmp.bmGammaTpl.FreeDC();
	m_Bmp.bmSpecial.FreeDC();

	WRGBQUAD * bits = m_Bmp.bmGammaTpl.LockRawBits();

/*
	m_Bmp.bmSpecial.RenderRawTransparentBitmap(
		bits,
		m_Bmp.bmGammaTpl.Width(), m_Bmp.bmGammaTpl.Height(),
		m_dcsz.cx*x, m_dcsz.cy*y, cx*m_dcsz.cx, cy*m_dcsz.cy, dx*m_dcsz.cx, dy*m_dcsz.cy,
		0xfefefefe, 0);
*/

	if ( fSelected )
	{
		m_Bmp.bmSpecial.RenderRawBitmap(
			bits,
			m_Bmp.bmGammaTpl.Width(), m_Bmp.bmGammaTpl.Height(),
			100, FALSE, m_dcsz.cx*6, m_dcsz.cy*7, 1*m_dcsz.cx, 1*m_dcsz.cy, dx*m_dcsz.cx, dy*m_dcsz.cy,
			0);
	}
	m_Bmp.bmSpecial.UnlockRawBits();
}


VOID CLvlDraw::RedrawBuffer(BOOL fSetScroll)
{
	if ( !m_dcsz.cx || !m_dcsz.cy )
		return;

	HDC hdc = m_Bmp.bmBuffer.LockDC();

	RECT rc = { 0, 0, m_Bmp.bmBuffer.Width(), m_Bmp.bmBuffer.Height() };
	//m_Bmp.bmBuffer.FillRect(0, 0, m_Bmp.bmBuffer.Width(), m_Bmp.bmBuffer.Height(), 0, 0, 0);
	FillRect(hdc, &rc, GetStockBrush(BLACK_BRUSH));
	BitBlt(hdc, 0, 0, m_Bmp.bmLevel.Width(), m_Bmp.bmLevel.Height(), m_Bmp.bmLevel.LockDC(), 0, 0, SRCCOPY);
	m_Bmp.bmLevel.FreeDC();
	//m_Bmp.bmBuffer.RenderRawBitmap( m_Bmp.bmLevel.LockRawBits(), m_Bmp.bmLevel.Width(), m_Bmp.bmLevel.Height(), 255, FALSE, 0, 0, m_Bmp.bmLevel.Width(), m_Bmp.bmLevel.Height(), 0, 0, 0);
	//m_Bmp.bmLevel.UnlockRawBits();

	if ( (*m_pvEditor)[nlfDirection] )
	{
		BitBlt(hdc, 
			0, m_Bmp.bmLevel.Height(), m_Bmp.bmSpecial.Width(), m_Bmp.bmSpecial.Height(),
			m_Bmp.bmSpecial.LockDC(), 0, 0, SRCCOPY
			);
		/*
		m_Bmp.bmBuffer.RenderRawBitmap( 
			m_Bmp.bmSpecial.LockRawBits(), 
			m_Bmp.bmSpecial.Width(), m_Bmp.bmSpecial.Height(), 
			255, FALSE, 
			0, 0, m_Bmp.bmSpecial.Width(), m_Bmp.bmSpecial.Height(),
			0, m_Bmp.bmLevel.Height(), 0
			);
		m_Bmp.bmSpecial.UnlockRawBits();
		*/
		m_Bmp.bmSpecial.FreeDC();
	}
	else
	{
		BitBlt(hdc,
			m_Bmp.bmLevel.Width(), 0, m_Bmp.bmSpecial.Width(), m_Bmp.bmSpecial.Height(), 
			m_Bmp.bmSpecial.LockDC(), 0, 0, SRCCOPY
			);

		/*
		m_Bmp.bmBuffer.RenderRawBitmap( 
			m_Bmp.bmSpecial.LockRawBits(), 
			m_Bmp.bmSpecial.Width(), m_Bmp.bmSpecial.Height(), 
			255, FALSE, 
			0, 0, m_Bmp.bmSpecial.Width(), m_Bmp.bmSpecial.Height(),
			m_Bmp.bmLevel.Width(), 0, 0
			);
		m_Bmp.bmSpecial.UnlockRawBits();
		*/

		m_Bmp.bmSpecial.FreeDC();
	}

	m_Bmp.bmBuffer.FreeDC();

	if ( fSetScroll )
		SetScroll();
}

VOID CLvlDraw::LoadBmp(HINSTANCE hInstance, LPTSTR pszResource, UINT index, UINT cx, UINT cy)
{
	if ( index >= MAX_TPL )
		return;

	m_Bmp.bmTpl[index].LoadBitmapW(hInstance, pszResource);
	m_sz[index].cx = cx; m_sz[index].cy = cy;
}

VOID CLvlDraw::LoadGamma(HINSTANCE hInstance, LPTSTR pszResource)
{
	m_Bmp.bmGamma.LoadBitmapW(hInstance, pszResource);
	m_uGammaIndex = 0;
}

VOID CLvlDraw::UseEnemyMask(UINT indexOfMask, UINT indexOfEnemy)
{
	if ( indexOfEnemy >= MAX_TPL || indexOfMask >= MAX_TPL )
		return;

	if (
		m_Bmp.bmTpl[indexOfMask].Width() != m_Bmp.bmTpl[indexOfEnemy].Width() ||
		m_Bmp.bmTpl[indexOfMask].Height() != m_Bmp.bmTpl[indexOfEnemy].Height()
		)
		return;

	WRGBQUAD * enm = m_Bmp.bmTpl[indexOfEnemy].LockRawBits();
	WRGBQUAD * mask = m_Bmp.bmTpl[indexOfMask].LockRawBits();
	
	UINT count = m_Bmp.bmTpl[indexOfMask].Width()*m_Bmp.bmTpl[indexOfMask].Height();
	
	for(UINT c = 0; c < count; ++c)
		if ( 0 != *((DWORD*) &mask[c]) )
			*((DWORD*) &enm[c]) = 0xfefefefe;

	m_Bmp.bmTpl[indexOfMask].UnlockRawBits();
	m_Bmp.bmTpl[indexOfEnemy].UnlockRawBits();
}

VOID CLvlDraw::UseGamma(UINT gammaIndex, UINT tplIndex, UINT tplGrIndex)
{
	if ( 2*gammaIndex >= (UINT) m_Bmp.bmGamma.Height() 
		|| tplIndex >= MAX_TPL 
		)
		return;

	COLORREF clArray[16] = { 0 };
	if ( !m_pvEditor->GetColFromPalette(&clArray[0]) )
		return;

	m_uGammaIndex = gammaIndex;
	m_crBg = XCHGCOLORREF( clArray[0] );

	m_Bmp.bmGammaTpl.Destroy();
	m_Bmp.bmGammaTpl.Create(m_Bmp.bmTpl[tplIndex].Width(),m_Bmp.bmTpl[tplIndex].Height());
	m_Bmp.bmGrGammaTpl.Destroy();
	m_Bmp.bmGrGammaTpl.Create(m_Bmp.bmTpl[tplGrIndex].Width(), m_Bmp.bmTpl[tplGrIndex].Height());

	WRGBQUAD * gamma = m_Bmp.bmGammaTpl.LockRawBits();
	WRGBQUAD * grgamma = m_Bmp.bmGrGammaTpl.LockRawBits();

	WRGBQUAD * tpl = m_Bmp.bmTpl[tplIndex].LockRawBits();
	WRGBQUAD * grtpl = m_Bmp.bmTpl[tplGrIndex].LockRawBits();

	UINT length = m_Bmp.bmTpl[tplIndex].Width()*m_Bmp.bmTpl[tplIndex].Height(),
		lengthGr = m_Bmp.bmTpl[tplGrIndex].Width()*m_Bmp.bmTpl[tplGrIndex].Height();
	
	m_Bmp.bmLevel.FillRect(0, 0, m_Bmp.bmLevel.Width(), m_Bmp.bmLevel.Height(), m_crBg, 0, 0);


	for(UINT c = 0; c < length; ++c)
	{
		UINT clrf = (*(DWORD*) &tpl[c]),
		index = clrf/0x101010;
		if ( clrf == 0x00FF00FF )
			*(DWORD*) &gamma[c] = 0xfefefefe;
		else if ( index < (UINT) m_Bmp.bmGamma.Width() )
			*(DWORD*) &gamma[c] = clArray[index]; 
	}

	for(UINT c = 0; c < lengthGr; ++c)
	{
		UINT clrf = (*(DWORD*) &grtpl[c]),
		index = clrf/0x00101010;
		if ( clrf == 0x00FF00FF )
			*(DWORD*) &grgamma[c] = 0xfefefefe;
		else if ( index < (UINT) m_Bmp.bmGamma.Width() )
			*(DWORD*) &grgamma[c] = clArray[index];
	}

	m_Bmp.bmTpl[tplIndex].UnlockRawBits();
	m_Bmp.bmGamma.UnlockRawBits();
	m_Bmp.bmGammaTpl.UnlockRawBits();
	m_Bmp.bmGrGammaTpl.UnlockRawBits();
}

VOID CLvlDraw::InitSize(COLORREF clr, UINT cx, UINT cy, UINT pixX, UINT pixY)
{
	m_Bmp.bmLevel.Resize(cx*pixX, cy*pixY);
	m_Bmp.bmSpecial.Resize( (*m_pvEditor)[nlfDirection] ? cx*pixX : 3*pixX, (*m_pvEditor)[nlfDirection] ? 3*pixY : cy*pixY );

	m_dcsz.cx = pixX; m_dcsz.cy = pixY;
	m_crBg = XCHGCOLORREF( m_Bmp.bmGamma.Dot(0, 2*m_uGammaIndex) );
	m_Bmp.bmLevel.FillRect(0, 0, cx*pixX, cy*pixY, m_crBg, m_crBg, 0);
	m_Bmp.bmSpecial.FillRect(0, 0, m_Bmp.bmSpecial.Width(), m_Bmp.bmSpecial.Height(), 0, 0, 0);
	m_scroll.scrollX = m_scroll.scrollY = 0;
	SetScrollPos(m_hWnd, SB_VERT, 0, TRUE);
	SetScrollPos(m_hWnd, SB_HORZ, 0, TRUE);

	(*m_pvEditor)[nlfDirection] ? cy+=3 : cx += 3;

	RECT rc = { 0 };
	GetClientRect(m_hWnd, &rc);
	m_Bmp.bmBuffer.Resize(max((UINT) rc.right, cx*pixX), max((UINT) rc.bottom, cy*pixY));
	RedrawBuffer();
}