
class CChgLayerDlg: public CDialog
{
	struct
	{
		INT			iLayer;
		INT			iMaxLayer;
	} m_LayerCounter;

	VOID			OnInit(LPARAM lParam);
	VOID			OnOK();
	VOID			OnCancel();

public:
	CChgLayerDlg(HINSTANCE hInstance, INT iCurrentLayer, INT iMaximumLayer);
	INT				GetLayerSelection();
};