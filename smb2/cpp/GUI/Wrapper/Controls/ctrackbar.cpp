
#include "precomp.h"

VOID CTrackBar::SetMinMax(UINT min, UINT max)
{
	cSendMessage(TBM_SETRANGEMIN, TRUE, min);
	cSendMessage(TBM_SETRANGEMAX, TRUE, max);
}

VOID CTrackBar::Pos(UINT uPos)
{
	cSendMessage(TBM_SETPOS, TRUE, uPos);
}

UINT CTrackBar::Pos()
{
	return (UINT) cSendMessage(TBM_GETPOS);
}