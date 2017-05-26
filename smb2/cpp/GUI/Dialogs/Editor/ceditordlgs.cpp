
#include "precomp.h"


CEntrancePtrDlg::CEntrancePtrDlg(HINSTANCE hInstance, BOOL fCreation) : CDialog(hInstance), m_fCreation( fCreation )
{
	
}

VOID CEntrancePtrDlg::OnPreInit(CDialogTemplate & d)
{
	d.SetDlgPos(0, 0, 170, 51 + ( m_fCreation ? 15 : 0 ) );
	d.SetDlgStyle(DS_SHELLFONT | DS_CONTROL | WS_CHILD, 0);

/*
    COMBOBOX        IDC_LEVELN,70,7,93,16,CBS_DROPDOWNLIST | WS_VSCROLL | WS_TABSTOP
    EDITTEXT        IDC_PAGERN,71,22,81,13,ES_AUTOHSCROLL
    CONTROL         "",IDC_PAGER,"msctls_updown32",UDS_WRAP | UDS_SETBUDDYINT | UDS_ALIGNRIGHT | UDS_AUTOBUDDY | UDS_ARROWKEYS | UDS_HOTTRACK,152,22,11,12
    RTEXT           "Points to level:",IDC_STATIC,7,10,53,9
    RTEXT           "Points to page:",IDC_STATIC,7,24,53,9
*/
	d.AddPreItem(-1, cStatic, TEXT("Points to level:"), 7,10,53,9, SS_RIGHT, TRUE, TRUE, FALSE);
	d.AddPreItem(-1, cStatic, TEXT("Points to page:"), 7,24,53,9, SS_RIGHT, TRUE, TRUE, FALSE);
	d.AddPreItem(IDC_LEVELN, cCombobox, NULL, 70,7,93,16, CBS_DROPDOWNLIST | WS_VSCROLL);
	d.AddPreItem(IDC_PAGERN, cEdit, NULL, 70,22,93,13, ES_AUTOHSCROLL | WS_BORDER);
	d.AddPreItem(IDC_PAGER, UPDOWN_CLASS, NULL, 0, 0, 0, 0, UDS_WRAP | UDS_SETBUDDYINT | UDS_ALIGNRIGHT | UDS_AUTOBUDDY | UDS_ARROWKEYS | UDS_HOTTRACK,
		TRUE, TRUE, FALSE);

	if ( m_fCreation )
	{
		d.AddPreItem(IDC_CHECK1, cButton, TEXT("Create connected door"), 70, 40, 93, 10, BS_AUTOCHECKBOX);
		d.AddPreItem(IDC_COMBO1, cCombobox, NULL, 70, 55, 93, 100, CBS_DROPDOWNLIST | WS_VSCROLL, TRUE, FALSE);
	}
}

VOID CEntrancePtrDlg::OnInit(LPARAM lParam)
{
	CComboBox cb;
	cb.AssignWindow(Ctl(IDC_LEVELN));

	for(int i=0; i<MAX_LEVELS; ++i)
		cb.AddString(g_szLevelName[i], i);	

	dSendMessage(IDC_PAGER, UDM_SETRANGE32, 0, 9);
	cb.SetCurSel(0);

	if ( m_fCreation )
	{
		CComboBox cbmy;
		cbmy.AssignWindow(Ctl(IDC_COMBO1));

		if ( 1 == m_fCreation )
		{
			BYTE x[] = { 9, 10, 11, 19, 20, 21, 28, 29 };
			for(int i = 0; i < sizeof(x); ++i)
				cbmy.AddString(g_szItemName[x[i]], x[i]);
		}
		else if ( 2 == m_fCreation )
			cbmy.AddString(TEXT("Pointer"), 1);

		cbmy.SetCurSel(0);
	}
}

VOID CEntrancePtrDlg::OnButton(USHORT uId)
{
	EnableAll(Checked(IDC_CHECK1), IDC_COMBO1, 0);
}

BOOL CEntrancePtrDlg::IsFarPointer()
{
	CComboBox cb;
	cb.AssignWindow(Ctl(IDC_LEVELN));
	return ( cb.GetParam(cb.GetCurSel()) > 150 );
}

BYTE CEntrancePtrDlg::GetRequestedPtr()
{
	if ( !Checked(IDC_CHECK1) )
		return 0;

	CComboBox cb;
	cb.AssignWindow(Ctl(IDC_COMBO1));

	return LOBYTE(LOWORD(cb.GetParam(cb.GetCurSel())));
}

BYTE CEntrancePtrDlg::GetPointerParam(BYTE id)
{
	CComboBox cb;
	cb.AssignWindow(Ctl(IDC_LEVELN));

	UINT vid = cb.GetParam(cb.GetCurSel()), page = dSendMessage(IDC_PAGER, UDM_GETPOS);

	switch(id)
	{
	case 0: return vid/10;
	case 1: return ((vid%10) << 4 | page);
	}

	return -1;
}

VOID CEntrancePtrDlg::SetPointerData(BOOL fFarPointer, BYTE param1, BYTE param2)
{
	UINT page = ( param2 & 0x0F ), vid = ( param1*10+(param2>>4) );
	
	CComboBox cb;
	cb.AssignWindow(Ctl(IDC_LEVELN));
	cb.SetCurSel(vid);
	dSendMessage(IDC_PAGER, UDM_SETPOS, 0, page);
}

CGroundSetDlg::CGroundSetDlg(HINSTANCE hInstance) : CDialog(hInstance, MAKEINTRESOURCE(IDD_GROUND_SET))
{

}

VOID CGroundSetDlg::OnInit(LPARAM lParam)
{
	CTrackBar tbSet, tbType;
	tbSet.AssignWindow(Ctl(IDC_GROUNDSET)); tbType.AssignWindow(Ctl(IDC_GROUNDTYPE));
	tbSet.SetMinMax(0, 31);
	tbType.SetMinMax(0, 7);
}

BYTE CGroundSetDlg::GroundSet()
{
	CTrackBar tbSet;
	tbSet.AssignWindow(Ctl(IDC_GROUNDSET));
	return (BYTE) ( tbSet.Pos() & 0x1f );
}

BYTE CGroundSetDlg::GroundType()
{
	CTrackBar tbType;
	tbType.AssignWindow(Ctl(IDC_GROUNDTYPE));
	return (BYTE) ( tbType.Pos() & 0x7 );
}

VOID CGroundSetDlg::GroundType(BYTE bBgType)
{
	CTrackBar tbType;
	tbType.AssignWindow(Ctl(IDC_GROUNDTYPE));
	tbType.Pos(0x7 & bBgType);
}

VOID CGroundSetDlg::GroundSet(BYTE bBgType)
{
	CTrackBar tbSet;
	tbSet.AssignWindow(Ctl(IDC_GROUNDSET));
	tbSet.Pos(0x1f & bBgType);
}

CItemSizeDlg::CItemSizeDlg(HINSTANCE hInstance) : CDialog(hInstance, MAKEINTRESOURCE(IDD_ITEM_SIZE))
{

}

VOID CItemSizeDlg::OnInit(LPARAM lParam)
{
	CTrackBar tbSize;
	tbSize.AssignWindow(Ctl(IDC_OBJSIZE));
	tbSize.SetMinMax(1, 16);
	tbSize.Pos(7);
}

BYTE CItemSizeDlg::GetSizeParam()
{
	CTrackBar tbSize;
	tbSize.AssignWindow(Ctl(IDC_OBJSIZE));
	return (BYTE) ((0xff & tbSize.Pos()) - 1);
}

VOID CItemSizeDlg::SetSizeParam(BYTE bSize)
{
	CTrackBar tbSize;
	tbSize.AssignWindow(Ctl(IDC_OBJSIZE));
	tbSize.Pos(1+(0xf & bSize));
}