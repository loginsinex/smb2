
#include "precomp.h"

CNesTiles::CNesTiles()
{
	m_bmTiles.Create(TILES_WIDTH*TILE_WIDTH, TILES_HEIGHT*TILE_HEIGHT);
	m_bmTiles.FillRect(0, 0, m_bmTiles.Width(), m_bmTiles.Height(), 0xff00ff, 0, 0);
}

BOOL CNesTiles::LoadTiles(PBYTE ptr, DWORD uSize)
{
	if ( TILES_PTR + TILES_WIDTH*TILES_HEIGHT + 8 > uSize ) 
		return FALSE;

	DWORD uColList[4] = { TILE_COL0, TILE_COL1, TILE_COL2, TILE_COL3 };

	LPDWORD lpdwBits = (LPDWORD) m_bmTiles.LockRawBits();
	
	DWORD k = 0, ki = 0;
	for(int y=0; y<TILES_HEIGHT; ++y)
	{
		for(int x=0;x<TILES_WIDTH; ++x)
		{
			for(int cy=0;cy<8;++cy)
			{
				BYTE bCol[2] = { ptr[TILES_PTR+k+cy], ptr[TILES_PTR+k+8+cy] };
				ki = (x*TILE_WIDTH+cy*TILES_WIDTH*TILE_WIDTH+y*TILE_HEIGHT*TILES_WIDTH*TILE_WIDTH);
				for(int cx=0;cx<8;++cx)
				{
					BYTE vCol = (( bCol[0] >> (7-cx) ) & 1 ) | (( (bCol[1] >> (7-cx)) & 1) << 1 );

					lpdwBits[ki++] = uColList[vCol];
				}
			}
			k += 16;
		}
	}
	m_bmTiles.UnlockRawBits();

	return TRUE;
}

BOOL CNesTiles::MakeTile(CBitmap & bmTile, DWORD uPtr, char cx, char cy)
{
	if ( uPtr + ((UINT)cx)*((UINT)cy) >= (UINT) m_bmTiles.Width()*m_bmTiles.Height() )
		return FALSE;

	WRGBQUAD * pvBits = m_bmTiles.LockRawBits();

	m_bmTiles.UnlockRawBits();
	return FALSE;
}