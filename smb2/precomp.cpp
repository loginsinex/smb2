
#include "precomp.h"
#include <algorithm>
HINSTANCE g_hResInstance;

int main(int argc, char ** argv)
{
	return _tWinMain(GetModuleHandle(NULL), NULL, GetCommandLine(), SW_SHOW);
}

int DoMainLoop(HWND hWindow, HACCEL hAcc)
{
	MSG msg = { 0 };
	int ret = 0;
	do
	{
		ret = GetMessage(&msg, NULL, 0, 0);

		if ( -1 == ret || !ret )
			break;

		 // _tprintf(TEXT("Window: 0x%04X, Message: 0x%04x\n"), msg.hwnd, msg.message);

		switch(msg.message)
		{
		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
		case WM_KEYDOWN:
			{
				CDialog * pSelectedDlg = (CDialog*) GetWindowLongPtr( GetActiveWindow() , DWL_USER );

				HWND hCustom = ( pSelectedDlg ? pSelectedDlg->GetCustomMenu() : NULL );
				if ( hCustom )
				{
					if ( WM_KEYDOWN == msg.message )
					{
						switch(msg.wParam)
						{
						case VK_UP:
						case VK_DOWN:
						case VK_RIGHT:
						case VK_LEFT:
						case VK_ESCAPE:
							{
								if ( MNC_EXECUTE == SendMessage(hCustom, WM_MENUCHAR, 0, 0) )	// send directly,
								{																// avoid beep on VK_ESCAPE
									SendMessage(hCustom, msg.message, msg.wParam, msg.lParam);
									continue;
								}
								break;
							}
						}
					}
					else
					{
						msg.hwnd = hCustom;
					}
				}
			}
		}

		if ( !TranslateAccelerator(hWindow, hAcc, &msg) 
			&& !IsDialogMessage(hWindow, &msg) ) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	} while(ret);

	return ret;
}


int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	g_hResInstance = LoadLibrary(TEXT("smb2res.dll"));
	if ( !g_hResInstance )
	{
		FatalAppExit(0, TEXT("Failed to load resources"));
		return 0;
	}

	InitBufView(hInstance, TEXT("wp"));
	InitDDControl(STATUSCLASSNAME);
	InitCommonControls();
	InitLevelDraw();

	CMainWnd * Window = new CMainWnd(hInstance);
	Window->Create();

	ShowWindow(*Window, SW_SHOW);
	UpdateWindow(*Window);

	HACCEL hacc = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCEL));

	DoMainLoop(*Window, hacc);

	DestroyAcceleratorTable(hacc);
	DestroyWindow(*Window);
	delete Window;

	return 0;
}

