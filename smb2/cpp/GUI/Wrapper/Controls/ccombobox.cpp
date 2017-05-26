
#include "precomp.h"

INT CComboBox::GetCurSel()
{
	return (INT) cSendMessage(CB_GETCURSEL, 0, 0);
}

INT CComboBox::AddString(LPCTSTR lpszString, LPARAM lParam)
{
	INT i = (INT) cSendMessage(CB_ADDSTRING, 0, LPARAM(lpszString));
	cSendMessage(CB_SETITEMDATA, i, lParam);
	return i;
}

LPARAM CComboBox::GetParam(INT iIndex)
{
	return cSendMessage(CB_GETITEMDATA, iIndex, 0);
}

void CComboBox::SetCurSel(INT iIndex)
{
	int k = (INT) cSendMessage(CB_SETCURSEL, iIndex, 0);
}
