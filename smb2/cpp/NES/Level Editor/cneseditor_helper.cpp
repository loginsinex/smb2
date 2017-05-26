
#include "precomp.h"

VOID CNesLayer::Layer(DWORD i)
{
	m_nLayer = i;
}

DWORD CNesLayer::Layer()
{
	return m_nLayer;
}

BOOL CNesLayer::InsertPage(CNesPage * pPage)
{
	return AppendItemToList(pPage);
}

BOOL CNesLayer::SetPagesCount(DWORD c)
{
	BOOL fResult = TRUE;
	for(DWORD i = 0; i < c; ++i)
	{
		CNesPage * pPage = new CNesPage;
		pPage->Page(i);
		fResult &= InsertPage(pPage);
	}

	return fResult;
}

BOOL CNesLayer::SortObjects()
{
	BOOL fResult = TRUE;
	DWORD cItems = SizeOfItemsList();
	for(DWORD i = 0; i < cItems; ++i)
	{
		CNesPage * pPage = ItemFromList(i);

		fResult &= ( pPage->SortObjects() & ( m_fHorzLevel ? pPage->SortGroundsHorz() : pPage->SortGroundsVert() ) );
	}

	return fResult;
}

CNesPage * CNesLayer::Page(DWORD i)
{
	return ItemFromList(i);
}

BOOL CNesLayer::InsertObjectToPage(DWORD i, CNesItem * pItem)
{
	if ( i >= SizeOfItemsList() )
		return FALSE;

	return ItemFromList(i)->InsertObject(pItem);
}

CNesLayer::~CNesLayer()
{
	while(SizeOfItemsList() > 0)
	{
		delete ItemFromList(0);
		DeleteFromItemsList(0);
	}
}

VOID CNesPage::Page(DWORD i)
{
	m_nPage = i;
}

DWORD CNesPage::Page()
{
	return m_nPage;
}

BOOL CNesPage::InsertObject(CNesItem * pItem)
{
	return AppendItemToList(pItem);
}

BOOL CNesPage::SortObjects()
{
	DWORD cItems = SizeOfItemsList();
	if ( !cItems )
		return TRUE;

	// create objects list exclude pointers and ground sets
	DWORD * pItems = new DWORD[cItems];
	DWORD cObjItems = 0;
	for(DWORD i = 0; i < cItems; ++i)
	{
		CNesItem * pItem = ItemFromList(i);
		switch(pItem->Type())
		{
		case nitRegular:
		case nitEntrance:
			{
				pItems[cObjItems++] = i;
				break;
			}
		}
	}

	BOOL fSortInProgress = FALSE;
	do
	{
		if ( ! cObjItems )
			break;

		fSortInProgress = FALSE;
		for(DWORD i = 0; i < cObjItems - 1; ++i)
		{
			CNesItem * p1 = ItemFromList(pItems[i]);
			CNesItem * p2 = ItemFromList(pItems[i+1]);

			if ( p1->Pos().y > p2->Pos().y )
			{
				Swap(pItems[i], pItems[i+1]);
				fSortInProgress = TRUE;
			}
		}
	} while(fSortInProgress);

	delete pItems;

	return TRUE;
}

BOOL CNesPage::SortGroundsHorz()
{
	DWORD cItems = SizeOfItemsList();
	if ( !cItems )
		return TRUE;

	// create grounds list
	DWORD * pItems = new DWORD[cItems];
	DWORD cObjItems = 0;
	for(DWORD i = 0; i < cItems; ++i)
	{
		CNesItem * pItem = ItemFromList(i);
		if ( nitGround == pItem->Type() )
			pItems[cObjItems++] = i;
	}

	BOOL fSortInProgress = FALSE;
	do
	{
		if ( !cObjItems )
			break;

		fSortInProgress = FALSE;
		for(DWORD i = 0; i < cObjItems - 1; ++i)
		{
			CNesItem * p1 = ItemFromList(pItems[i]);
			CNesItem * p2 = ItemFromList(pItems[i+1]);

			if ( p1->Pos().x > p2->Pos().x )
			{
				Swap(pItems[i], pItems[i+1]);
				fSortInProgress = TRUE;
			}
		}
	} while(fSortInProgress);

	delete pItems;

	return TRUE;	
}

BOOL CNesPage::SortGroundsVert()
{
	DWORD cItems = SizeOfItemsList();
	if ( !cItems )
		return TRUE;

	// create grounds list
	DWORD * pItems = new DWORD[cItems];
	DWORD cObjItems = 0;
	for(DWORD i = 0; i < cItems; ++i)
	{
		CNesItem * pItem = ItemFromList(i);
		if ( nitGround == pItem->Type() )
			pItems[cObjItems++] = i;
	}

	BOOL fSortInProgress = FALSE;
	do
	{
		if ( !cObjItems )
			break;

		fSortInProgress = FALSE;
		for(DWORD i = 0; i < cObjItems - 1; ++i)
		{
			CNesItem * p1 = ItemFromList(pItems[i]);
			CNesItem * p2 = ItemFromList(pItems[i+1]);

			if ( p1->Pos().y > p2->Pos().y )
			{
				Swap(pItems[i], pItems[i+1]);
				fSortInProgress = TRUE;
			}
		}
	} while(fSortInProgress);

	delete pItems;

	return TRUE;	
}

CNesItem * CNesPage::Item(DWORD i)
{
	return ItemFromList(i);
}

BOOL CNesObjectsList::InsertObject(NES_LEVEL_ITEM * pItem)
{
	return AppendItemToList(pItem);
}

PNES_LEVEL_ITEM CNesObjectsList::Object(DWORD i)
{
	return ItemFromList(i);
}

BOOL CNesObjectsList::InsertSkipper(BOOL fDoubleSkipper)
{
	PNES_LEVEL_ITEM pSkipper = new NES_LEVEL_ITEM;
	if ( pSkipper )
	{
		ZeroMemory(pSkipper, sizeof(*pSkipper));
		pSkipper->nli = nliSkipper;
		pSkipper->Object.Skipper.uType = 0xF2 + ( fDoubleSkipper ? 1 : 0 );
		return InsertObject(pSkipper);
	}
	return FALSE;
}

PNES_LEVEL_ITEM CNesObjectsList::InsertNewObject()
{
	PNES_LEVEL_ITEM pItem = new NES_LEVEL_ITEM;
	if ( pItem )
	{
		ZeroMemory(pItem, sizeof(*pItem));
		InsertObject(pItem);
		return pItem;
	}
	return NULL;
}

BOOL CNesObjectsList::TrimSkippers()
{
	if ( SizeOfItemsList() < 1 )
		return FALSE;

	PNES_LEVEL_ITEM pItem = ItemFromList(SizeOfItemsList()-1);
	while(nliSkipper == pItem->nli)
	{
		delete pItem;
		DeleteFromItemsList(SizeOfItemsList()-1);

		if ( !SizeOfItemsList() )
			break;

		pItem = ItemFromList(SizeOfItemsList()-1);
	}

	return TRUE;
}

CNesObjectsList::~CNesObjectsList()
{
	while(SizeOfItemsList() > 0)
	{
		delete ItemFromList(0);
		DeleteFromItemsList(0);
	}
}

CNesLayerArray::~CNesLayerArray()
{
	while(SizeOfItemsList() > 0)
	{
		delete ItemFromList(0);
		DeleteFromItemsList(0);
	}
}

BOOL CNesLayerArray::SetLayersCount(DWORD c, BOOL fHorz)
{
	BOOL fResult = TRUE;
	for(DWORD i = 0; i < c; ++i)
	{
		CNesLayer * pLayer = new CNesLayer(fHorz);
		fResult &= AppendItemToList(pLayer);
	}
	return fResult;
}

CNesLayer *	CNesLayerArray::Layer(DWORD i)
{
	return ItemFromList(i);
}

CNesObjectsListsArray::~CNesObjectsListsArray()
{
	while(SizeOfItemsList() > 0)
	{
		delete ItemFromList(0);
		DeleteFromItemsList(0);
	}
}

BOOL CNesObjectsListsArray::SetListsCount(DWORD c)
{
	BOOL fResult = TRUE;
	for(DWORD i = 0; i < c; ++i)
	{
		CNesObjectsList * pObjList = new CNesObjectsList;
		fResult &= AppendItemToList(pObjList);
	}
	return fResult;
}

CNesObjectsList * CNesObjectsListsArray::ObjList(DWORD i)
{
	return ItemFromList(i);
}
