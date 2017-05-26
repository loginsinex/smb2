
class CTreeView: public CControl
{
	BOOL		ReqItem(HTREEITEM hItem, LPTV_ITEM lptvi);
	BOOL		UpdItem(HTREEITEM hItem, LPTV_ITEM lptvi);

public:
	HTREEITEM	AddItem(HTREEITEM hParentItem, LPCTSTR lpszText, LPARAM lParam, BOOL bBold = FALSE, BOOL bSelected = FALSE, BOOL bExpanded = FALSE, HTREEITEM hInsertAfter = TVI_LAST);
	HTREEITEM	GetItem(DWORD dwFlag, HTREEITEM hFrom = NULL);
	HTREEITEM	GetCurSel();
	void		SetCurSel(HTREEITEM hItem);

	void		Param(HTREEITEM hItem, LPARAM lParam);
	LPARAM		Param(HTREEITEM hItem);
	void		Text(HTREEITEM hItem, LPCTSTR lpszText);
	CString		Text(HTREEITEM hItem);
	void		State(HTREEITEM hItem, DWORD dwState);
	DWORD		State(HTREEITEM hItem);
	void		CheckItem(HTREEITEM hTree, BOOL bCheck);
	BOOL		CheckItem(HTREEITEM hTree);
	void		ResetContent();
};
