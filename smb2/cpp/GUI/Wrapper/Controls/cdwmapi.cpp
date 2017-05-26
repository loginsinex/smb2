
#include "precomp.h"

CDwmApi::CDwmApi()
	: m_hDwm( (HANDLE) LoadLibrary(TEXT("dwmapi")) )
{

}

HRESULT CDwmApi::DwmDefWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, LRESULT* plResult)
{
	DWMCALL(DWMDEFWINDOWPROC, "DwmDefWindowProc")(hWnd, msg, wParam, lParam, plResult);
}

HRESULT CDwmApi::DwmEnableBlurBehindWindow(HWND hWnd, const DWM_BLURBEHIND * pcdwmBlurBehind)
{
	DWMCALL(DWMENABLEBLURBEHINDWINDOW, "DwmEnableBlurBehindWindow")(hWnd, pcdwmBlurBehind);
}

HRESULT CDwmApi::DwmEnableComposition(UINT uCompositionAction)
{
	DWMCALL(DWMENABLECOMPOSITION, "DwmEnableComposition")(uCompositionAction);
}

HRESULT CDwmApi::DwmExtendFrameIntoClientArea(HWND hWnd, const MARGINS * pcMargins)
{
	DWMCALL(DWMEXTENDFRAMEINTOCLIENTAREA, "DwmExtendFrameIntoClientArea")(hWnd, pcMargins);
}

HRESULT CDwmApi::DwmGetColorizationColor(DWORD* pcrColorization, BOOL* pfOpaqueBlend)
{
	DWMCALL(DWMGETCOLORIZATIONCOLOR, "DwmGetColorizationColor")(pcrColorization, pfOpaqueBlend);
}

HRESULT CDwmApi::DwmGetCompositionTimingInfo(HWND hWnd, DWM_TIMING_INFO* pdwmTimingInfo)
{
	DWMCALL(DWMGETCOMPOSITIONTIMINGINFO, "DwmGetCompositionTimingInfo")(hWnd, pdwmTimingInfo);
}

HRESULT CDwmApi::DwmGetWindowAttribute(HWND hWnd, DWORD dwAttribute, PVOID pvAttribute, DWORD cbAttribute)
{
	DWMCALL(DWMGETWINDOWATTRIBUTE, "DwmGetWindowAttribute")(hWnd, dwAttribute, pvAttribute, cbAttribute);
}

HRESULT CDwmApi::DwmIsCompositionEnabled(BOOL* pfEnabled)
{
	DWMCALL(DWMISCOMPOSITIONENABLED, "DwmIsCompositionEnabled")(pfEnabled);
}

HRESULT CDwmApi::DwmModifyPreviousDxFrameDuration(HWND hWnd, INT cRefreshes, BOOL fRelative)
{
	DWMCALL(DWMMODIFYPREVIOUSDXFRAMEDURATION, "DwmModifyPreviousDxFrameDuration")(hWnd, cRefreshes, fRelative);
}

HRESULT CDwmApi::DwmQueryThumbNailSourceSize(HTHUMBNAIL hThumbnail, PSIZE pSize)
{
	DWMCALL(DWMQUERYTHUMBNAILSOURCESIZE, "DwmQueryThumbNailSourceSize")( hThumbnail, pSize );
}

HRESULT CDwmApi::DwmRegisterThumbNail(HWND hWnd, HWND* phWnd, PHTHUMBNAIL phThumbnails)
{
	DWMCALL(DWMREGISTERTHUMBNAIL, "DwmRegisterThumbNail") ( hWnd, phWnd, phThumbnails );
}

HRESULT CDwmApi::DwmSetDxFrameDuration(HWND hWnd, INT cRefreshes)
{
	DWMCALL(DWMSETDXFRAMEDURATION, "DwmSetDxFrameDuration")( hWnd, cRefreshes );
}

HRESULT CDwmApi::DwmSetPresentParameters(HWND hWnd, DWM_PRESENT_PARAMETERS* pdwmPresentParameters)
{
	DWMCALL(DWMSETPRESENTPARAMETERS, "DwmSetPresentParameters")( hWnd, pdwmPresentParameters );
}

HRESULT CDwmApi::DwmSetWindowAttribute(HWND hWnd, DWORD dwAttribute, LPCVOID pvAttribute, DWORD cbAttribute)
{
	DWMCALL(DWMSETWINDOWATTRIBUTE, "DwmSetWindowAttribute")(hWnd, dwAttribute, pvAttribute, cbAttribute);
}

HRESULT CDwmApi::DwmUnregisterThumbnail(HTHUMBNAIL hThumbnail)
{
	DWMCALL(DWMUNREGISTERTHUMBNAIL, "DwmUnregisterThumbnail")( hThumbnail );
}

HRESULT CDwmApi::DwmUpdateThumbnailProperties(HTHUMBNAIL hThumbnail, const DWM_THUMBNAIL_PROPERTIES* pcdwmThumbnailProperties)
{
	DWMCALL(DWMUPDATETHUMBNAILPROPERTIES, "DwmUpdateThumbnailProperties")( hThumbnail, pcdwmThumbnailProperties );
}