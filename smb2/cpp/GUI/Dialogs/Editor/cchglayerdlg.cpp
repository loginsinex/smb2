
#include "precomp.h"

CChgLayerDlg::CChgLayerDlg(HINSTANCE hInstance, INT iLayer, INT iMaxLayer) : CDialog(hInstance, MAKEINTRESOURCE(IDD_LAYERSEL))
{
	m_LayerCounter.iLayer = iLayer;
	m_LayerCounter.iMaxLayer = max(iMaxLayer,iLayer+1);
}

VOID CChgLayerDlg::OnInit(LPARAM lParam)
{
	CComboBox cb; cb.AssignWindow(Ctl(IDC_COMBO1));

	CString sLayers;
	for(INT i=0; i < m_LayerCounter.iMaxLayer; ++i)
	{
		sLayers.Format(TEXT("Layer #%d"), i);
		int id = cb.AddString(sLayers.GetBuffer(), i);
		if ( m_LayerCounter.iLayer == id )
			cb.SetCurSel(i);
	}

	cb.AddString(TEXT("Place to new layer"), -1);
}

VOID CChgLayerDlg::OnOK()
{
	CComboBox cb; cb.AssignWindow(Ctl(IDC_COMBO1));
	int id = cb.GetCurSel();
	if ( id >= 0 )
	{
		id = cb.GetParam(id);
		m_LayerCounter.iLayer = ( -1 == id ? m_LayerCounter.iMaxLayer : id );
		Close(1);
	}
	else
		ShowError(TEXT("You must select a valid layer"));
}

VOID CChgLayerDlg::OnCancel()
{
	Close(0);
}

INT CChgLayerDlg::GetLayerSelection()
{
	return m_LayerCounter.iLayer;
}