
class CControl
{
	HWND			m_hWnd;

protected:
	CControl();
	CControl(HWND hWnd);

public:
	void			AssignWindow(HWND hWnd);
	void			AssignControl(HWND hWnd, USHORT uCtlId);
	INT_PTR			cSendMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = NULL);
	HWND			GetWindowHandle();
	void			Enable(BOOL bEnabled);
	BOOL			Enable();
	BOOL			Visible();
	void			Visible(BOOL bVisible);
	operator		HWND();
};