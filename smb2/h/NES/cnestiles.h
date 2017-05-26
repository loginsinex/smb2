
#define		TILES_WIDTH			0x10
#define		TILES_HEIGHT		0x2000
#define		TILE_WIDTH			0x08
#define		TILE_HEIGHT			0x08
#define		TILES_PTR			0x20010
#define		TILE_COL0			0x00000000
#define		TILE_COL1			0x000000FF
#define		TILE_COL2			0x0000FF00
#define		TILE_COL3			0x00FF0000

typedef	enum _tagNES_OBJECTS
{
	noMushroom			= 0
} NES_OBJECTS, *PNES_OBJECTS;

class CNesTile
{
	SIZE		m_sz;
	POINT*		m_pt;


};

class CNesTiles
{
	CBitmap		m_bmTiles;

public:
	CNesTiles();
	BOOL		LoadTiles(PBYTE ptr, DWORD uSize);
	BOOL		MakeTile(CBitmap & bTile, DWORD uPtr, char cx, char cy);

};