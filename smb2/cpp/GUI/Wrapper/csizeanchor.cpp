
#include "precomp.h"

CSizeAnchor::CSizeAnchor(UINT uId, AnchorType left, AnchorType top, AnchorType right, AnchorType bottom)
{
	m_hParent = NULL;
	m_uId = uId;
	ZeroMemory(&m_ad, sizeof(m_ad));

	m_ad.left.type = left;
	m_ad.top.type = top;
	m_ad.right.type = right;
	m_ad.bottom.type = bottom;
}

USHORT CSizeAnchor::ControlId()
{
	return m_uId;
}

VOID CSizeAnchor::Initialize(HWND hParent)
{
	m_hParent = hParent;

	RECT rc = { 0 };
	GetClientRect(hParent, &rc);
	rc.bottom -= rc.top;
	rc.right -= rc.left;

	RECT ctl_rc = { 0 };
	GetWindowRect(GetDlgItem(hParent, m_uId), &ctl_rc);
	ScreenToClient(hParent, LPPOINT(&ctl_rc));
	ScreenToClient(hParent, LPPOINT(&ctl_rc) + 1);

	// left
	switch(m_ad.left.type)
	{
	case AnchorHardLinkLeftTop: m_ad.left.corner.hard.topleft = ctl_rc.left; break;
	case AnchorHardLinkRightBottom: m_ad.left.corner.hard.bottomright = rc.right - ctl_rc.left; break;
	case AnchorSoftLink: m_ad.left.corner.soft.up = ctl_rc.left; m_ad.left.corner.soft.down = rc.right; break;
	}

	// top
	switch(m_ad.top.type)
	{
	case AnchorHardLinkLeftTop: m_ad.top.corner.hard.topleft = ctl_rc.top; break;
	case AnchorHardLinkRightBottom: m_ad.top.corner.hard.bottomright = rc.bottom - ctl_rc.top; break;
	case AnchorSoftLink: m_ad.top.corner.soft.up = ctl_rc.top; m_ad.top.corner.soft.down = rc.bottom; break;
	}

	// right
	switch(m_ad.right.type)
	{
	case AnchorHardLinkLeftTop: m_ad.right.corner.hard.topleft = ctl_rc.right; break;
	case AnchorHardLinkRightBottom: m_ad.right.corner.hard.bottomright = rc.right - ctl_rc.right; break;
	case AnchorSoftLink: m_ad.right.corner.soft.up = ctl_rc.right; m_ad.right.corner.soft.down = rc.right; break;
	}

	// bottom
	switch(m_ad.bottom.type)
	{
	case AnchorHardLinkLeftTop: m_ad.bottom.corner.hard.topleft = ctl_rc.bottom; break;
	case AnchorHardLinkRightBottom: m_ad.bottom.corner.hard.bottomright = rc.bottom - ctl_rc.bottom; break;
	case AnchorSoftLink: m_ad.bottom.corner.soft.up = ctl_rc.bottom; m_ad.bottom.corner.soft.down = rc.bottom; break;
	}
}

VOID CSizeAnchor::InternalResize(LPRECT lprc)
{
	RECT rc = { 0 };
	GetClientRect(m_hParent, &rc);

	rc.right -= rc.left;
	rc.bottom -= rc.top;

	HWND hCtl = GetDlgItem(m_hParent, m_uId);
	GetWindowRect(hCtl, lprc);
	ScreenToClient(m_hParent, LPPOINT(lprc));
	ScreenToClient(m_hParent, LPPOINT(lprc) + 1);

	// left corner
	switch(m_ad.left.type)
	{
	case AnchorHardLinkLeftTop: lprc->left = m_ad.left.corner.hard.topleft; break;
	case AnchorHardLinkRightBottom: lprc->left = rc.right - m_ad.left.corner.hard.bottomright; break;
	case AnchorSoftLink: lprc->left = rc.right * m_ad.left.corner.soft.up / m_ad.left.corner.soft.down; break;
	}

	// right corner
	switch(m_ad.right.type)
	{
	case AnchorHardLinkLeftTop: lprc->right = m_ad.right.corner.hard.topleft; break;
	case AnchorHardLinkRightBottom: lprc->right = rc.right - m_ad.right.corner.hard.bottomright; break;
	case AnchorSoftLink: lprc->right = rc.right * m_ad.right.corner.soft.up / m_ad.right.corner.soft.down; break;
	}

	// top corner
	switch(m_ad.top.type)
	{
	case AnchorHardLinkLeftTop: lprc->top = m_ad.top.corner.hard.topleft; break;
	case AnchorHardLinkRightBottom: lprc->top = rc.bottom - m_ad.top.corner.hard.bottomright; break;
	case AnchorSoftLink: lprc->top = rc.bottom * m_ad.top.corner.soft.up / m_ad.top.corner.soft.down; break;
	}

	// bottom corner
	switch(m_ad.bottom.type)
	{
	case AnchorHardLinkLeftTop: lprc->bottom = m_ad.bottom.corner.hard.topleft; break;
	case AnchorHardLinkRightBottom: lprc->bottom = rc.bottom - m_ad.bottom.corner.hard.bottomright; break;
	case AnchorSoftLink: lprc->bottom = rc.bottom * m_ad.bottom.corner.soft.up / m_ad.bottom.corner.soft.down; break;
	}
}

VOID CSizeAnchor::ResizeByDefer(HDWP hDwp)
{
	RECT ctl_rc = { 0 };
	
	//if ( m_uId == 1007 ) MessageBox(m_hParent, TEXT("stop?"), TEXT(""), 0);

	InternalResize(&ctl_rc);
	DeferWindowPos(hDwp, GetDlgItem(m_hParent, m_uId), NULL, ctl_rc.left, ctl_rc.top, ctl_rc.right - ctl_rc.left, ctl_rc.bottom - ctl_rc.top, SWP_NOACTIVATE | SWP_NOZORDER);
}