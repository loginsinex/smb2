#pragma once

#include <SDKDDKVer.h>

#pragma comment(linker, \
    "\"/manifestdependency:type='Win32' "\
    "name='Microsoft.Windows.Common-Controls' "\
    "version='6.0.0.0' "\
    "processorArchitecture='*' "\
    "publicKeyToken='6595b64144ccf1df' "\
    "language='*'\"")


#define WIN32_LEAN_AND_MEAN             // Исключите редко используемые компоненты из заголовков Windows
// Файлы заголовков Windows:
#include <windows.h>
#include <WindowsX.h>
#include <CommCtrl.h>
#include <CommDlg.h>

// Файлы заголовков C RunTime
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <xstring>
#include <vector>
#include <map>
#include <algorithm>
#include <atlstr.h>



using namespace std;
extern HINSTANCE g_hResInstance;

#define		SWAP_DWORD(x)		( (((x) >> 24) & 0x000000FF) | (((x)>>8) & 0x0000FF00) | (((x)<<8) & 0x00FF0000) | (((x)<<24) & 0xFF000000) )
#define		SWAP_BITS(x)	(																						\
				(((x)>>31) & 0x00000001) | (((x)>>29) & 0x00000002) | (((x)>>27) & 0x00000004) | (((x)>>25) & 0x00000008) |	\
				(((x)>>23) & 0x00000010) | (((x)>>21) & 0x00000020) | (((x)>>19) & 0x00000040) | (((x)>>17) & 0x00000080) |	\
				(((x)>>15) & 0x00000100) | (((x)>>13) & 0x00000200) | (((x)>>11) & 0x00000400) | (((x)>> 9) & 0x00000800) |	\
				(((x)>> 7) & 0x00001000) | (((x)>> 5) & 0x00002000) | (((x)>> 3) & 0x00004000) | (((x)>> 1) & 0x00008000) |	\
				(((x)<< 1) & 0x00010000) | (((x)<< 3) & 0x00020000) | (((x)<< 5) & 0x00040000) | (((x)<< 7) & 0x00080000) |	\
				(((x)<< 9) & 0x00100000) | (((x)<<11) & 0x00200000) | (((x)<<13) & 0x00400000) | (((x)<<15) & 0x00800000) |	\
				(((x)<<17) & 0x01000000) | (((x)<<19) & 0x02000000) | (((x)<<21) & 0x04000000) | (((x)<<23) & 0x08000000) |	\
				(((x)<<25) & 0x10000000) | (((x)<<27) & 0x20000000) | (((x)<<29) & 0x40000000) | (((x)<<31) & 0x80000000)	\
				)


inline void * __cdecl operator new(size_t size) 
{ 
	PVOID pParam = malloc(size);// GlobalAlloc(GMEM_ZEROINIT | GMEM_FIXED, size); // LocalAlloc(LMEM_ZEROINIT, size);
	
	if ( !pParam )
		FatalAppExit(0, TEXT("Can't allocate memory\n\nProgram will abnormal closed"));
	memset(pParam, 0, size);
	return (void *) pParam;
}

inline void __cdecl operator delete(void *ptr) 
{ 
	free(ptr);
	//GlobalFree(ptr);
    //LocalFree(ptr); 
}

#define	SETCTLTHEME		1

#include "resource.h"
#include "..\smb2res\resource.h"
#include "GUI\Wrapper\csizeanchor.h"
#include "GUI\Wrapper\cfont.h"
#include "GUI\Wrapper\Controls\cdwmapi.h"
#include "GUI\Wrapper\Controls\cglasswnd.h"
#include "GUI\Wrapper\cdialog.h"
#include "GUI\Wrapper\cpropsheet.h"
#include "GUI\Wrapper\Controls\bitmap.h"
#include "GUI\Wrapper\Controls\CControl.h"
#include "GUI\Wrapper\Controls\ctabcontrol.h"
#include "GUI\Wrapper\Controls\ctreeview.h"
#include "GUI\Wrapper\Controls\ccombobox.h"
#include "GUI\Wrapper\Controls\ctoolbar.h"
#include "GUI\Wrapper\Controls\ctrackbar.h"
#include "GUI\Wrapper\Controls\clistview.h"
#include "GUI\Wrapper\Controls\cbufview.h"
#include "GUI\Wrapper\Controls\listview.h"

#include "NES\nesleveldef.h"
#include "NES\cnestiles.h"
#include "NES\cnesitemslist.h"
#include "NES\cnesenemydata.h"
#include "NES\cnesleveldata.h"
#include "NES\cneseditor.h"
#include "NES\cnesfile.h"


// -------
#include "GUI\Dialogs\clvldraw.h"
#include "GUI\Dialogs\Worlds\cgameworlds.h"
#include "GUI\Dialogs\Advanced\cobjtiles.h"
#include "GUI\Dialogs\Advanced\cbgsetdlg.h"
#include "GUI\Dialogs\Advanced\cwrlditrdlg.h"
#include "GUI\Dialogs\Advanced\cbgtiles.h"
#include "GUI\Dialogs\Advanced\cpalette.h"
#include "GUI\Dialogs\Advanced\cwarpsdlg.h"
#include "GUI\Dialogs\Advanced\cmiscdatadlg.h"
#include "GUI\Dialogs\Editor\cchglayerdlg.h"
#include "GUI\Dialogs\Editor\clevelinfodlg.h"
#include "GUI\Dialogs\Editor\ceditordlgs.h"
#include "GUI\Dialogs\Editor\cnewitemdlg.h"
#include "GUI\Dialogs\clvlobjdlg.h"
#include "GUI\Dialogs\clvloptionsdlg.h"
#include "GUI\Dialogs\cleveldlg.h"
#include "GUI\Dialogs\cmainwnd.h"
