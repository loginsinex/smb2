
template <class T> class CNesItemsList
{
	struct
	{
		BOOL						fRemoveSelf;
		std::vector<T*>				vList;
	} ItemsList;

	VOID						AllocItemsList();
	VOID						DestroyItemsList();

protected:
	CNesItemsList(BOOL fRemoveSelf = FALSE);
	virtual ~CNesItemsList();
	BOOL						AppendItemToList(T * pItem);
	BOOL						InsertItemToList(DWORD i, T * pItem);
	BOOL						DeleteFromItemsList(DWORD i);
	T*							ItemFromList(DWORD i);
	BOOL						Swap(DWORD i1, DWORD i2);

public:
	DWORD						SizeOfItemsList();
};

template <class T> CNesItemsList<T>::CNesItemsList(BOOL fRemoveSelf)
{
	ItemsList.fRemoveSelf = fRemoveSelf;
	AllocItemsList();
}

template <class T> CNesItemsList<T>::~CNesItemsList()
{
	DestroyItemsList();
}

template <class T> VOID CNesItemsList<T>::AllocItemsList()
{
	ItemsList.vList.reserve(100);
}

template <class T> VOID CNesItemsList<T>::DestroyItemsList()
{
	if ( ItemsList.fRemoveSelf )
	{
		while(SizeOfItemsList() > 0)
		{
			delete ItemsList.vList[ItemsList.vList.size() - 1];
			DeleteFromItemsList(ItemsList.vList.size() - 1);
		}
	}
	else
		ItemsList.vList.clear();
}

template <class T> DWORD CNesItemsList<T>::SizeOfItemsList()
{
	return ItemsList.vList.size();
}

template <class T> BOOL CNesItemsList<T>::AppendItemToList(T * pItem)
{
	ItemsList.vList.push_back(pItem);
	return ( ItemsList.vList.size() > 0 );
}

template <class T> BOOL CNesItemsList<T>::InsertItemToList(DWORD i, T * pItem)
{
	if ( i < ItemsList.vList.size() )
		ItemsList.vList.insert(ItemsList.vList.begin() + i, pItem);
	else
		return AppendItemToList(pItem);

	return ( ItemsList.vList.size() > 0 );
}

template <class T> BOOL CNesItemsList<T>::DeleteFromItemsList(DWORD i)
{
	if ( i >= SizeOfItemsList() )
		return FALSE;

	ItemsList.vList.erase(ItemsList.vList.begin() + i);
	return TRUE;
}

template <class T> T* CNesItemsList<T>::ItemFromList(DWORD i)
{
	if ( i >= SizeOfItemsList() )
		return NULL;

	return ItemsList.vList[i];
}

template <class T> BOOL CNesItemsList<T>::Swap(DWORD i1, DWORD i2)
{
	if ( i1 >= SizeOfItemsList() || i2 >= SizeOfItemsList() )
		return FALSE;

	if ( i1 == i2 )
		return TRUE;

	T * nit = *(ItemsList.vList.begin() + i1);

	*(ItemsList.vList.begin() + i1) = *(ItemsList.vList.begin() + i2);
	*(ItemsList.vList.begin() + i2) = nit;
	return TRUE;
}