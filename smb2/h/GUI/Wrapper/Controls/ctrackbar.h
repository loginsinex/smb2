
class CTrackBar: public CControl
{

public:
	VOID		SetMinMax(UINT min, UINT max);
	VOID		Pos(UINT uPos);
	UINT		Pos();
};