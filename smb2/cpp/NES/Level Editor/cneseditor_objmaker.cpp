
#include "precomp.h"

BOOL CNesEditor::MakeObjectsList(DWORD & lpdwCount, std::vector<NES_LEVEL_ITEM> & pItemsList, DWORD & lpdwHeader)
{
	DWORD cPages = CountPages(), cItems = SizeOfItemsList();
	CNesLayerArray Layers;
	Layers.SetLayersCount(CountLayers(), m_levelFields.fDirection);
	
	CNesObjectsListsArray ObjLists;
	ObjLists.SetListsCount(CountLayers());

	for(DWORD i = 0; i < CountLayers(); ++i)
		Layers.Layer(i)->SetPagesCount(cPages);

	// start from second object. First object is a level ground!
	for(DWORD i = 1; i < cItems; ++i)
	{
		CNesItem * pItem = ItemFromList(i);
		if ( nitEnemy == pItem->Type() )
			continue;

		Layers.Layer(pItem->Layer())->InsertObjectToPage(pItem->Page(), pItem);
	}
	
	INT iLastBgX = 0;
	BYTE bLastBgType = m_levelFields.uTypeOfGround;
	// sort all layers and create objects lists
	for(DWORD i = 0; i < CountLayers(); ++i)
	{
		Layers.Layer(i)->SortObjects();

		DWORD cLayerPages = Layers.Layer(i)->SizeOfItemsList();
		BOOL fSkipperWas = FALSE;

		INT iLastY = 0;

		if ( i > 0 )
		{
			// create 'new layer' object for 1 and major layers
			PNES_LEVEL_ITEM pNewLayer = ObjLists.ObjList(i)->InsertNewObject();
			pNewLayer->nli = nliBackToStart;
			pNewLayer->Object.BackToStart.Reserved[0] = 0xF4;
		}

		for(DWORD k = 0; k < cLayerPages; ++k)
		{
			CNesPage * pPage1 = Layers.Layer(i)->Page(k);
			CNesPage * pPage2 = ( k < (cLayerPages-1) ? Layers.Layer(i)->Page(k+1) : NULL );

			if ( 0 == pPage1->SizeOfItemsList() )
			{
				BOOL fDoubleSkipper = ( pPage2 && 0 == pPage2->SizeOfItemsList() );
				ObjLists.ObjList(i)->InsertSkipper(fDoubleSkipper);
				if ( fDoubleSkipper )
					k++;				// if next page is empty too - skip 2 pages

				if ( m_levelFields.fDirection )
					iLastY = 0;
				else
					iLastY = 0x0f*(k+1);

				fSkipperWas = TRUE;
				continue;
			}

			// run on page for objects list
			DWORD cObjects = pPage1->SizeOfItemsList();
			BOOL fObjectWas = FALSE;
			if ( TRUE ) //m_levelFields.fDirection )		// horizontal & vertical
			{
				for(DWORD n = 0; n < cObjects; ++n)
				{
					CNesItem * pItem = pPage1->Item(n);
					if ( !pItem )
						continue;

					if ( nitGround == pItem->Type() || nitPointer == pItem->Type() )
					{
						if ( !fObjectWas && 0 == n && k > 0 && !fSkipperWas ) // fSkipperWas )
						{
							ObjLists.ObjList(i)->InsertSkipper(FALSE);
							iLastY = ( m_levelFields.fDirection ? 0 : 0x0f*(k+1) );
						}

						PNES_LEVEL_ITEM pLvlItem = ObjLists.ObjList(i)->InsertNewObject();
						
						switch(pItem->Type())
						{
						case nitPointer:
							{
								pLvlItem->nli = nliPtr;
								pLvlItem->Object.Pointer.Reserved = 0xF5;
								pLvlItem->Object.Pointer.uParameter[0] = pItem->Pointer() & 0xFF;
								pLvlItem->Object.Pointer.uParameter[1] = (pItem->Pointer() >> 8) & 0xFF;
								break;
							}
						case nitGround:
							{
								pLvlItem->nli = nliGroundSet;
								pLvlItem->pt.x = pItem->Pos().x;
								pLvlItem->pt.y = pItem->Pos().y;
								pLvlItem->Object.GroundSet.uSets = (BYTE) ( pItem->Ground() & 0x1F );
								pLvlItem->Object.GroundSet.Reserved = 0xF0;
								BYTE bCurBg = (BYTE) (( pItem->Ground() >> 8 ) & 0xFF );

								if ( m_levelFields.fDirection )
								{
									BYTE lenBit = 0;
									if ( iLastBgX >= pItem->Pos().x )		// something wrong!
										break;								// it is possible when bg in this layer
																			// located before bg in minor layers

									if ( pItem->Pos().x - iLastBgX > 1 )
									{
										if (( pItem->Pos().x % 0x10 ) >= 8 )
										{
											pLvlItem->Object.GroundSet.Reserved++;
										}
											
										lenBit = (pItem->Pos().x % 8);
									}
									pLvlItem->Object.GroundSet.uSets |= ( lenBit << 5 );
									iLastBgX = pItem->Pos().x;
								}
								else
								{
									BYTE lenBit = 0;
									if ( iLastBgX >= pItem->Pos().y )		// something wrong!
										break;								// it is possible when bg in this layer
																			// located before bg in minor layers

									if ( pItem->Pos().y - iLastBgX > 1 )
									{
										if (( pItem->Pos().y % 0x0f ) >= 8 )
										{
											pLvlItem->Object.GroundSet.Reserved++;
										}
											
										lenBit = ((pItem->Pos().y % 0x0f) % 8);
									}
									pLvlItem->Object.GroundSet.uSets |= ( lenBit << 5 );
									iLastBgX = pItem->Pos().y;
								}

								if ( bCurBg != bLastBgType )
								{
									PNES_LEVEL_ITEM pNewBg = ObjLists.ObjList(i)->InsertNewObject();
									pNewBg->nli = nliGroundType;
									pNewBg->Object.GroundType.Reserved = 0xF6;
									pNewBg->Object.GroundType.uType = bCurBg;
									bLastBgType = bCurBg;
								}
								break;
							}
						}
						fSkipperWas = FALSE;
						// also if this page does not contain pointable objects, insert skipper 
						// if next page is also empty - skip too
						// and continue, because at end of for we're insert new one skipper
						if ( !fObjectWas && ( cObjects - 1 ) == n )
						{
							ObjLists.ObjList(i)->InsertSkipper(pPage2 && 0 == pPage2->SizeOfItemsList());
							k += ( pPage2 && 0 == pPage2->SizeOfItemsList() ? 1 : 0 );
							iLastY = ( m_levelFields.fDirection ? 0 : 0x0f*(k+1) );
							fSkipperWas = TRUE;
							continue;
						}
					}
					else
					{
						int cy = pItem->Pos().y - iLastY;
						if ( cy < 0 && !m_levelFields.fDirection )
						{
							cy = 0x0f + cy;
							iLastY = 0x0f*k;
						}

						if ( m_levelFields.fDirection )
						{
							if ( iLastY >= 0x0F )
							{
								iLastY -= 0x0F;
								cy = 15 + pItem->Pos().y - iLastY;
								if ( cy >= 0x0F )
								{
									iLastY = 0;
									cy = pItem->Pos().y;
									ObjLists.ObjList(i)->InsertSkipper(FALSE);
								}
							}
						}
						else
						{
							if ( cy >= 0x0F )
							{
								iLastY = 0x0f*(k+1);
								cy = ( pItem->Pos().y % 15 );
								ObjLists.ObjList(i)->InsertSkipper(FALSE);
							}
						}

						PNES_LEVEL_ITEM pLvlItem = ObjLists.ObjList(i)->InsertNewObject();
						switch(pItem->Type())
						{
							case nitEntrance:
								{
									pLvlItem->nli = nliEntrance;
									pLvlItem->Object.Entrance.uEntranceId = pItem->Regular();
									pLvlItem->Object.Entrance.uEntranceParameter[0] = pItem->EntranceParameter(0);
									pLvlItem->Object.Entrance.uEntranceParameter[1] = pItem->EntranceParameter(1);
									pLvlItem->Object.Entrance.uEntranceParameter[2] = pItem->EntranceParameter(2);
									break;
								}
							case nitRegular:
								{
									pLvlItem->nli = nliRegular;
									pLvlItem->Object.Regular.id = pItem->Regular();
									break;
								}
						}

						
						pLvlItem->Object.General.x = ( pItem->Pos().x % 0x10 );
						pLvlItem->Object.General.y = ( cy ); if ( cy < 0 ) pLvlItem->Object.General.y--;
						pLvlItem->pt.x = pItem->Pos().x;
						pLvlItem->pt.y = pItem->Pos().y;
						iLastY = pItem->Pos().y;
						fObjectWas = TRUE;
						fSkipperWas = FALSE;
					}
					if ( n == ( cObjects - 1 ) )
					{
						iLastY += ( m_levelFields.fDirection ? 0x0f : 0 );
						if ( pPage2 && 0 == pPage2->SizeOfItemsList() )
						{
							ObjLists.ObjList(i)->InsertSkipper(TRUE);
							//if ( !m_levelFields.fDirection )
							fSkipperWas = TRUE;
							k++;
							iLastY = ( m_levelFields.fDirection ? 0 : 0x0f*(k+1) );
						}
					}
				}
			}
		}
	}


	// create header
	lpdwHeader = AssembleHeader();
	lpdwHeader = SWAP_DWORD(lpdwHeader);

	// create list
	DWORD count = 0;
	for(DWORD i = 0; i < CountLayers(); ++i)
	{
		ObjLists.ObjList(i)->TrimSkippers();
		count+=ObjLists.ObjList(i)->SizeOfItemsList();
	}

	lpdwCount = count;

	count = 0;
	for(DWORD i = 0; i < CountLayers(); ++i)
	{
		DWORD cItems = ObjLists.ObjList(i)->SizeOfItemsList();
		for(DWORD k = 0; k < cItems; ++k)
		{
			pItemsList.push_back( *(ObjLists.ObjList(i)->Object(k)) );
		}
	}

	return TRUE;
}

BOOL CNesEditor::MakeEnemiesList(DWORD & lpdwCount, std::vector<NES_LEVEL_ENEMY> & pItemsList)
{
	DWORD cItems = SizeOfItemsList(), cObjects = 0, wObject = 0;
	for(DWORD i = 0; i < cItems; ++i)
	{
		CNesItem * pItem = (CNesItem*) ItemFromList(i);
		if ( nitEnemy == pItem->Type() ) 
			cObjects++;
	}

	lpdwCount = cObjects;
	// pItemsList = new NES_LEVEL_ENEMY[cObjects];

	for(DWORD i = 0; i < cItems; ++i)
	{
		CNesItem * pItem = (CNesItem*) ItemFromList(i);
		if ( nitEnemy == pItem->Type() ) 
		{
			NES_LEVEL_ENEMY nEnemy = { 0 };
			nEnemy.Enemy.Object.id = pItem->Enemy();
			nEnemy.Enemy.Object.x = (BYTE) ( pItem->Pos().x % 0x10 );
			nEnemy.Enemy.Object.y = (BYTE) ( pItem->Pos().y % 0x10 );
			nEnemy.iPage = ( m_levelFields.fDirection ? pItem->Pos().x : pItem->Pos().y ) / 0x10;
			pItemsList.push_back( nEnemy );
			wObject++;
		}
	}

	return TRUE;
}

BOOL CNesEditor::MakeJarLevel()
{
	CNesLevel & lvl = m_lvl;
	BOOL fResult = lvl.MakeJarLevel();
	if ( !fResult )		// already jar
		return FALSE;

#define SETHEADER(x)  SetHeaderInfo((x), lvl[(x)])
	SETHEADER(nlfDirection);
	SETHEADER(nlfReserved1); //		= 1,
	SETHEADER(nlfColor); //			= 2,
	SETHEADER(nlfReserved2); //		= 3,
	SETHEADER(nlfEnemyColor); //		= 4,
	SETHEADER(nlfReserved3); //		= 5,
	SETHEADER(nlfGround); //			= 6,
	SETHEADER(nlfLength); //			= 7,
	SETHEADER(nlfObjectType); //		= 8,
	SETHEADER(nlfReserved4); //		= 9,
	SETHEADER(nlfTypeOfGround); //		= 10,
	SETHEADER(nlfReserved5); //		= 11,
	SETHEADER(nlfMusic); //			= 12
#undef SETHEADER

	m_Counters.dwPages = 1;

	RemoveAll();
	InsertGround( 2, 0x04, 0x00, 0);
	InsertGround( 3, 0x05, 0x00, 0);
	InsertGround( 4, 0x06, 0x00, 0);
	InsertGround( 5, 0x07, 0x00, 0);
	InsertGround(12, 0x06, 0x00, 0);
	InsertGround(13, 0x03, 0x00, 0);
	InsertGround(14, 0x08, 0x00, 0);
	InsertEnemy(11, 13, 0x01, TRUE);

	Modified();
	return TRUE;
}

BOOL CNesEditor::AssembleHeader()
{
	BYTE x[32] = { 0 };

	x[32-1] = m_levelFields.fDirection;
	x[32-3] = (1&(m_levelFields.uColor >> 2)); x[32-4] = (1&(m_levelFields.uColor >> 1)); 
	x[32-5] = (1&(m_levelFields.uColor));

	x[32-7] = (1&(m_levelFields.uEnemyColor >> 1)); x[32-8] = (1&(m_levelFields.uEnemyColor));

	x[32-9] = 1; x[32-10] = 1; x[32-11] = 1;

	x[32-12] = (1&(m_levelFields.uGround>>4)); x[32-13] = (1&(m_levelFields.uGround>>3));
	x[32-14] = (1&(m_levelFields.uGround>>2)); x[32-15] = (1&(m_levelFields.uGround>>1));
	x[32-16] = (1&(m_levelFields.uGround));

	x[32-17] = (1&(m_levelFields.uLength)>>3); x[32-18] = (1&(m_levelFields.uLength)>>2);
	x[32-19] = (1&(m_levelFields.uLength)>>1); x[32-20] = (1&(m_levelFields.uLength));

	x[32-21] = (1&(m_levelFields.uObjectType)>>3); x[32-22] = (1&(m_levelFields.uObjectType)>>2);
	x[32-23] = (1&(m_levelFields.uObjectType)>>1); x[32-24] = (1&(m_levelFields.uObjectType));

	x[32-27] = (1&(m_levelFields.uTypeOfGround)>>2); x[32-28] = (1&(m_levelFields.uTypeOfGround)>>1);
	x[32-29] = (1&(m_levelFields.uTypeOfGround));

	x[32-31] = (1&(m_levelFields.uMusic >> 1)); x[32-32] = (1&(m_levelFields.uMusic));

	DWORD w = 0;

	for(int i=0;i<32;++i) 
		w |= ( x[i] << i );

	//x[i] = 1 & ( m_gameData.level[level].levelBits / ( 1 << i ) );
		
	return w;
}

BOOL CNesEditor::RewritePoints()
{
	DWORD cItems = SizeOfItemsList();
	for(DWORD i = 1; i < cItems; ++i)
	{
		CNesItem * pItem = (CNesItem*) ItemFromList(i);
		if ( nitGround == pItem->Type() )
		{
			// warning! Positions collision can be occurs! When X pos is 0x0f, then it can be rewritten to 0 after convertion horz->vert->horz
			// it is normal!! Because horizontal screen contains 0x10 points, and vertical - 0x0f. Converting horz->vert->horz losts this information
			// but we cannot have groundset object at 0,0 and need to increment to 0x0f points for horz and 0x10 for vert
			if ( m_levelFields.fDirection )
			{
				pItem->m_pt.x = (pItem->m_pt.y % 0x10) + ( m_levelFields.fDirection ? pItem->m_iPage*0x10 : 0 );
				pItem->m_pt.y = 0;
				if ( 0 == pItem->m_pt.x ) do{ pItem->m_pt.x+=0x0f; } while(CheckGroundCollision(pItem->m_pt.x, pItem->Layer(), i));
			}
			else
			{
				pItem->m_pt.y = (pItem->m_pt.x % 0x10) + ( m_levelFields.fDirection ? 0 : pItem->m_iPage*0x0f );
				pItem->m_pt.x = 0;
				if ( 0 == pItem->m_pt.y ) do{ pItem->m_pt.y+=0x0f; } while(CheckGroundCollision(pItem->m_pt.y, pItem->Layer(), i));
			}

		}
		else if ( nitEnemy == pItem->Type() )
		{
			pItem->m_pt.x = (pItem->m_pt.x % 0x10) + ( m_levelFields.fDirection ? pItem->m_iPage*0x10 : 0 );
			pItem->m_pt.y = (pItem->m_pt.y % 0x10) + ( m_levelFields.fDirection ? 0 : pItem->m_iPage*0x10 );
		}
		else
		{
			pItem->m_pt.x = (pItem->m_pt.x % 0x10) + ( m_levelFields.fDirection ? pItem->m_iPage*0x10 : 0 );
			pItem->m_pt.y = (pItem->m_pt.y % 0x0f) + ( m_levelFields.fDirection ? 0 : pItem->m_iPage*0x0f );
		}
	}
	return TRUE;
}