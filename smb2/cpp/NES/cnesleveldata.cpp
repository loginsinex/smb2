
#include "precomp.h"

const LPTSTR		g_szItemName[] = 
{
  /*	0	*/ TEXT("Mushroom block"),
  /*	1	*/ TEXT("POW block"),
  /*	2	*/ TEXT("Bombable rock"),
  /*	3	*/ TEXT("Vine"),
  /*	4	*/ TEXT("Jar (small, can't go in)"),
  /*	5	*/ TEXT("Ladder (1 square)"),
  /*	6	*/ TEXT("Jar, extends to ground, can go in"),
  /*	7	*/ TEXT("Jar, extends to ground, can go in (not the same as #06)"),
  /*	8	*/ TEXT("Jar, extends to ground, can't go in (used for warp zones)"),
  /*	9	*/ TEXT("Locked door"),
  /*	10	*/ TEXT("Door"),
  /*	11	*/ TEXT("Dark entrance"),
  /*	12	*/ TEXT("Vine, extends to ground"),
  /*	13	*/ TEXT("Vine, extends to ground (no top)"),
  /*	14	*/ TEXT("Star background"),
  /*	15	*/ TEXT("Red pillar, extends to ground"),
  /*	16	*/ TEXT("Cloud"),
  /*	17	*/ TEXT("Small cloud"),
  /*	18	*/ TEXT("Vine, extends to top"),
  /*	19	*/ TEXT("Entrance/exit (light right)"),
  /*	20	*/ TEXT("Entrance/exit (light left)"),
  /*	21	*/ TEXT("White entrance, extends to ground"),
  /*	22	*/ TEXT("Tree, extends to ground"),
  /*	23	*/ TEXT("Pyramid"),
  /*	24	*/ TEXT("Brick background, extends to ground"),
  /*	25	*/ TEXT("Brick wall, extends to ground"),
  /*	26	*/ TEXT("Vegetable thrower (used in Wart's room)"),
  /*	27	*/ TEXT("???"),
  /*	28	*/ TEXT("Castle entrance 1"),
  /*	29	*/ TEXT("Castle entrance 2"),
  /*	30	*/ TEXT("Big mouth entrance used in desert"),
  /*	31	*/ TEXT("Large red platform background, extends to ground"),
  /*	32	*/ TEXT("Herb with coin"),
  /*	33	*/ TEXT("Herb with fresh vegetable"),
  /*	34	*/ TEXT("Herb with small vegetable"),
  /*	35	*/ TEXT("Herb with rocket"),
  /*	36	*/ TEXT("Herb with turtle shell"),
  /*	37	*/ TEXT("Herb with bomb"),
  /*	38	*/ TEXT("Herb with potion"),
  /*	39	*/ TEXT("Herb with 1UP"),
  /*	40	*/ TEXT("Herb with POW"),
  /*	41	*/ TEXT("Cherry"),
  /*	42	*/ TEXT("Herb with Bob-omb"),
  /*	43	*/ TEXT("1st sub-space Mushroom"),
  /*	44	*/ TEXT("White/red evil head"),
  /*	45	*/ TEXT("2nd sub-space Mushroom"),
  /*	46	*/ TEXT("Whale eye"),
  /*	47	*/ TEXT("Wood wall, 1 square"),
  /*	48	*/ TEXT("X-Blocks [1]"),
  /*	49	*/ TEXT("X-Blocks [2]"),
  /*	50	*/ TEXT("Herb(s) with small vegetable"),
  /*	51	*/ TEXT("Bridge"),
  /*	52	*/ TEXT("Spikes barrier"),
  /*	53	*/ TEXT("Column of bombable rock"),
  /*	54	*/ TEXT("Column of brown brick"),
  /*	55	*/ TEXT("Ladder"),
  /*	56	*/ TEXT("Whale"),
  /*	57	*/ TEXT("Green platform"),
  /*	58	*/ TEXT("Red wood platform"),
  /*	59	*/ TEXT("Cloud platform"),
  /*	60	*/ TEXT("Waterfall")
};

const LPTSTR		g_szSOName[] =
{
 /*	f0	*/ TEXT("[!] Ground setting"),
 /*	f1	*/ TEXT("[!] Ground setting"),
 /*	f2	*/ TEXT("[!] Skip 1 page"),
 /*	f3	*/ TEXT("[!] Skip 2 pages"),
 /*	f4	*/ TEXT("[!] Start new layer"),
 /*	f5	*/ TEXT("[!] Pointer"),
 /*	f6	*/ TEXT("[!] Ground type")
};

DWORD NES_LEVEL_ITEM::SizeOfItem()
{
	switch(nli)
	{
	case nliSkipper: return 1;
	case nliBackToStart: return 1;
	case nliRegular: return 2;
	case nliGroundSet: return 2;
	case nliGroundType: return 2;
	case nliPtr: return 3;		
	case nliEntrance: return 
						  ( 0xFF == Object.Entrance.uEntranceParameter[0] ? 
							2
							:
							(4 + ( 0xF5 == Object.Entrance.uEntranceParameter[0] ? 1 : 0 ))
							);

	case nliUnknown: return 2;
	}
	return 0;
}

CNesLevelContainer::CNesLevelContainer()
	: m_dwLevelSize( 0 )
{
	m_dwLinkedToLevel = 0;
}

VOID CNesLevelContainer::FlushObjects()
{
	while(SizeOfItemsList() > 0)
	{
		delete ItemFromList(0);
		DeleteFromItemsList(0);
	}
}

CNesLevelContainer::~CNesLevelContainer()
{
	FlushObjects();
}


DWORD CNesLevelContainer::LevelHeader()
{
	return SWAP_DWORD( m_dwLevelHeader );
}

BOOL CNesLevelContainer::MakeJarLevel()
{
	DWORD dwNew = ( NES_JAR_HEADER );
	BOOL fResult = ( m_dwLevelHeader != dwNew );
	m_dwLevelHeader = dwNew;
	StoreHeader();
	return fResult;
}

BOOL CNesLevelContainer::RereadPoints()
{
	DWORD count = SizeOfItemsList();

	DWORD iDeltaX = 0, iDeltaY = 0;
	DWORD lastX=0, lastY=0, lastBgX=0;
	BYTE uGroundType = operator[](nlfTypeOfGround)+1;

	for(DWORD i=0;i<count;++i)
	{
		PNES_LEVEL_ITEM x = ItemFromList(i);
		
		switch(x->nli)
		{
		case nliSkipper:
			{
				iDeltaY = 0;
				lastX = iDeltaX += ( (x->Object.Skipper.uType & 0x0F) - 1 ) * 0x10;
				break;
			}
		case nliBackToStart:
			{
				iDeltaY = 0;
				lastX = iDeltaX = 0;
				break;
			}
		case nliEntrance:
		case nliRegular:
			{
				iDeltaY += x->Object.General.y;
				if ( iDeltaY >= 0x0F )
				{
					iDeltaY = ( iDeltaY + 1 ) % 16;
					iDeltaX += 0x10;
				}
				x->pt.x = lastX = iDeltaX + x->Object.General.x;
				x->pt.y = iDeltaY;
				x->Page = ( iDeltaX / 0x10 );
				break;
			}
		case nliGroundSet:
			{
				break;
			}
		case nliGroundType:
			{
				break;
			}
		}
	}

	StoreSizeOfLevel();

	return TRUE;
}

void CNesLevelContainer::LoadLevelData(DWORD dwLevelHeader, std::vector<NES_LEVEL_ITEM> & pItemsList, DWORD dwCount)
{
	FlushObjects();
	m_dwLevelHeader = SWAP_DWORD(dwLevelHeader);
	StoreHeader();

	for(DWORD i=0; i<dwCount; ++i)
	{
		PNES_LEVEL_ITEM pItem = new NES_LEVEL_ITEM;
		*pItem = pItemsList[i];
		AppendItemToList(pItem);
	}

	RereadPoints();
}

VOID CNesLevelContainer::StoreHeader()
{
	BYTE x[32] = { 0 };
	for(int k=0;k<32;++k) 
		x[k] = 1 & ( m_dwLevelHeader / ( 1 << k ) );
		
	m_levelFields.fDirection = x[32-1];
	m_levelFields.uColor = 4*x[32-3]+2*x[32-4]+1*x[32-5];
	m_levelFields.uEnemyColor = 2*x[32-7]+1*x[32 - 8];
	m_levelFields.uGround = 16*x[32-12]+8*x[32-13]+4*x[32-14]+2*x[32-15]+1*x[32-16];
	m_levelFields.uLength = 8*x[32-17]+4*x[32-18]+2*x[32-19]+1*x[32-20];
	m_levelFields.uObjectType = 8*x[32-21]+4*x[32-22]+2*x[32-23]+1*x[32-24];
	m_levelFields.uTypeOfGround = 4*x[32-27]+2*x[32-28]+1*x[32-29];
	m_levelFields.uMusic = 2*x[32-31]+1*x[32-32];
}

VOID CNesLevelContainer::LoadLevel(PBYTE ptr, DWORD dwLinkedLevel)
{
	FlushObjects();
	DWORD i = sizeof(DWORD);
	m_dwLevelHeader = SWAP_DWORD(*((DWORD*) ptr));
	m_dwLinkedToLevel = dwLinkedLevel;

	StoreHeader();

	while(0xFF != ptr[i])
	{
		PNES_LEVEL_ITEM pItem = new NES_LEVEL_ITEM;

		BYTE ix = ( ptr[i] & 0x0F ), iy = ( ( ptr[i] >> 4 ) & 0x0F );

		if ( 0x0F == iy )
		{
			switch(ix)	
			{
			case 0:
			case 1:
				{
					pItem->nli = nliGroundSet;
					pItem->Object.pData[0] = ptr[i];
					pItem->Object.pData[1] = ptr[i+1];
					i+=2;
					break;
				}
			case 2:
			case 3:
				{
					pItem->nli = nliSkipper;
					pItem->Object.pData[0] = ptr[i];
					i++;
					break;
				}
			case 4:
				{
					pItem->nli = nliBackToStart;
					pItem->Object.pData[0] = ptr[i];
					i++;
					break;
				}
			case 5:
				{
					pItem->nli = nliPtr;
					pItem->Object.pData[0] = ptr[i];
					pItem->Object.pData[1] = ptr[i+1];
					pItem->Object.pData[2] = ptr[i+2];
					i+=3;
					break;
				}
			case 6:
				{
					pItem->nli = nliGroundType;
					pItem->Object.pData[0] = ptr[i];
					pItem->Object.pData[1] = ptr[i+1];
					i+=2;
					break;
				}
			default:
				{
					pItem->nli = nliUnknown;
					pItem->Object.pData[0] = ptr[i];
					pItem->Object.pData[1] = ptr[i+1];
					i+=2;
					break;
				}
			}
		}
		else
		{
			BYTE item = ptr[i+1];

			switch(item)
			{
			case 0x09:
			case 0x0a:
			case 0x0b:
			case 0x13:
			case 0x14:
			case 0x15:
			case 0x1c:
			case 0x1d:
			case 0x1e:
				{
					pItem->nli = nliEntrance;
					pItem->Object.pData[0] = ptr[i];
					pItem->Object.pData[1] = ptr[i+1];
					if ( ptr[i+2] < 0xF0 || ptr[i+2] == 0xF5 )
					{
						pItem->Object.pData[2] = ptr[i+2];
						pItem->Object.pData[3] = ptr[i+3];
						pItem->Object.pData[4] = ptr[i+4];
						i+=2 + ( 0xF5 == ptr[i+2] ? 1 : 0 );
					}
					else
					{
						pItem->Object.pData[2] = 0xFF;
						// no additional object. Its end of level?
					}
					break;
				}
			default:
				{
					pItem->nli = nliRegular;
					pItem->Object.pData[0] = ptr[i];
					pItem->Object.pData[1] = ptr[i+1];
					break;
				}
			}
			i+=2;
		}
		/*
		if ( 0xF1 == pItem->Object.pData[0] && 0x73 == pItem->Object.pData[1] ) __asm int 03;

		_tprintf(TEXT("NLI: 0x%02x; Arr[%02X:%02X:%02X:%02X:%02X]\n"), 
			pItem->nli, 
			pItem->Object.pData[0], pItem->Object.pData[1], 
			pItem->Object.pData[2], pItem->Object.pData[3], 
			pItem->Object.pData[4]
		);
		*/

		AppendItemToList(pItem);
	}	

	RereadPoints();
}


DWORD CNesLevelContainer::SizeOfLevel()
{
	return m_dwLevelSize;
}

VOID CNesLevelContainer::StoreSizeOfLevel()
{
	DWORD s = 0;
	for(DWORD i = 0; i < SizeOfItemsList(); ++i)
		s += ItemFromList(i)->SizeOfItem();

	m_dwLevelSize = ( sizeof(DWORD) + s);
}

const NES_LEVEL_ITEM CNesLevelContainer::LevelData(UINT itemIndex)
{
	if ( itemIndex >= SizeOfItemsList() )
		throw CString("Subscript is out of range");

	return *ItemFromList(itemIndex);
}

DWORD CNesLevelContainer::TotalPages()
{
	DWORD dwPage = 0;
	for(UINT c = 0; c < SizeOfItemsList(); ++c)
	{
		NES_LEVEL_ITEM x = LevelData(c);
		if ( nliEntrance == x.nli || nliRegular == x.nli )
			if ( dwPage < x.Page ) dwPage = x.Page;
	}

	return dwPage;
}

BYTE CNesLevelContainer::operator[](NES_LEVEL_FIELDS index)
{
	switch(index)
	{
	case nlfDirection: return m_levelFields.fDirection; //		= 0,
	case nlfReserved1: return m_levelFields.Reserved1; //		= 1,
	case nlfColor: return m_levelFields.uColor; //			= 2,
	case nlfReserved2: return m_levelFields.Reserved2; //		= 3,
	case nlfEnemyColor: return m_levelFields.uEnemyColor; //		= 4,
	case nlfReserved3: return m_levelFields.Reserved3; //		= 5,
	case nlfGround: return m_levelFields.uGround; //			= 6,
	case nlfLength: return m_levelFields.uLength; //			= 7,
	case nlfObjectType: return m_levelFields.uObjectType; //		= 8,
	case nlfReserved4: return m_levelFields.Reserved4; //		= 9,
	case nlfTypeOfGround: return m_levelFields.uTypeOfGround; //		= 10,
	case nlfReserved5: return m_levelFields.Reserved5; //		= 11,
	case nlfMusic: return m_levelFields.uMusic; //			= 12
	}
	return -1;
}

BOOL CNesLevelContainer::MakeByteArray(PBYTE * pArray, LPDWORD lpdwCount)
{
	*lpdwCount = SizeOfLevel()+1;

	DWORD cItems = SizeOfItemsList(), index = sizeof(DWORD);
	*pArray = new BYTE[*lpdwCount];
	if ( !(*pArray) ) return FALSE;

	*((LPDWORD) *pArray) = LevelHeader();

	for(DWORD i = 0; i < cItems; ++i)
	{
		PNES_LEVEL_ITEM pItem = ItemFromList(i);
		
		CopyMemory(&(*pArray)[index], &pItem->Object.pData[0], pItem->SizeOfItem());
		index += pItem->SizeOfItem();
	}
	(*pArray)[index++] = 0xff;
	return ( index > 0 );
}

DWORD CNesLevelContainer::LinkLevel()
{
	return m_dwLinkedToLevel;
}

BOOL CNesLevelContainer::IsLinkedLevel()
{
	return ( m_dwLinkedToLevel > 1 );
}

DWORD CNesLevelContainer::AddRef()
{
	return ++m_dwLinkedToLevel;
}

DWORD CNesLevelContainer::Release()
{
	return --m_dwLinkedToLevel;
}


VOID CNesLevelContainer::SetLevelPtr(DWORD ptr)
{
	m_dwLevelPtr = ptr;
}

DWORD CNesLevelContainer::GetLevelPtr()
{
	return m_dwLevelPtr;
}

