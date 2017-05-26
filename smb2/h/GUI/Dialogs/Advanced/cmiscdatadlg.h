/*
typedef enum _tagNES_MISC_DATA_ENUM
{
	nmdNone,		
// 0x000C77 - Amount of time needed to charge jump
	nmdChargeJumpTime,
// 0x00102A - Number of cherries required for Starman
	nmdCherriesForStar,
// 0x001034 - sound effect for grabbing a cherry
	nmdCherrySound,
// 0x0010DF - Item pulled once the 1-Up flag is activated
// 0x0010F2 - Sprite pulled after number of big vegetables pulled is activated
// 0x0011FE - Button for opening doors
// 0X001820 ~ 0X001A1C - Text for Title
// 0X001892 ~ 0X0018B1 - Palettes for Title screen
// 0x001AA9 - song to play at Title screen
// 0x002E70 ~ 0x00309F - Empty
// 0x003B70 ~ 0x00400F - Empty
// 0x004099 - Sound effect from falling in 1-1
	nmdFallSound,
// 0x004556 - Number of enemies defeated for heart
	nmdEnemiesForHeart,
// 0x004C4A - Panser's spitting time
// 0x004C74 - Grey panser's spitting direction
// 0x004D53 - Jump height on coming out of jars
// 0x004F4E - Sprite Graphic to use for Heart floating
// 0x004F7B - Health for Birdos
	nmdBirdoHealth,
// 0x004FA8 - Birdo's jump height
	nmdBirdoJumpHeight,
// 0x004FC3 - Birdo's spitting time
	nmdBirdoSpittingTime,
// 0x004FD6 - Birdo's speed (right)
	nmdBirdoSpeedRight,
// 0x004FDC - Birdo's speed (left)
	nmdBirdoSpeedLeft,
// 0x004FEA - Birdo's spitting sound
	nmdBirdoSpittingSound,
// 0x005061 - Sound a 1-up makes
// 0X0050B8 - sound effect for grabbing a mushroom
// 0x0050CB - Amount of time the stop watch is active
	nmdStopWatchTimeout,
// 0x0053C7 - Jumping Ninja's 1st jump
// 0x0053C8 - Jumping Ninja's 2nd jump
// 0x0053C9 - Running Ninja's 1st jump
// 0x0053CA - Running Ninja's 2nd jump
// 0X005523 - Tweeter AI (it can follow you)
// 0x00552A - Tweeter jump before the other jump
// 0x00552E - Tweeter's number of jumps of first jump before the second jump
// 0x005532 - Tweeter's other jump after performing the previous jump
// 0x00556C - Snifit's jump height
// 0x0055A3 - Snifit's spitting time
// 0x005703 - Amount of time the ground shakes from a POW
	nmdPOWTimeEffect,
// 0x005818 ~ 0x005848 - How the Doors open
// 0x005ECD ~ 0x00603F - Empty
// 0x00611E - Health for Claw Grip of World 5
	nmdHealthClawGlip,
// 0x006171 - Claw Grip's rock throwing time
// 0x0061D5 - Rock speed
// 0X006486 - Amount of time to fly Pidget's carpet
	nmdCarpetTimeout,
// 0x00657F - Health for Mouser of World 1
	nmdMouserHealth1,
// 0x006586 - Health for Mouser of World 3
	nmdMouserHealth3,
// 0x0065B0 - Mouser's jump height
	nmdMouserJumpHeight,
// 0x0065BC - Mouser's bomb holding time
	nmdMouserBombHoldingTime,
// 0x0065F4 - Mouser's bomb throwing height
	nmdMouserThrowHeight,
// 0x0065FB - Time before bomb explosion
	nmdBombTimeout,
// 0x0065FF - Mouser's bomb throwing speed
	nmdMouserThrowSpeed,
// 0x00673D - Health for Triclydes
	nmdTriclydeHealth,
// 0x0068BB - Triclyde's spitting sound
	nmdTriclydeSpittingSound,
// 0x006C7E - Health for Fry Guy of World 4
	nmdFryGuyHealth,
// 0x006FF7 - Health for Hawkmouth of World 7
	nmdHawkmouthHealth,
// 0x0070BB - Health for revived Hawkmouth
	nmdHawkmouthRevivedHealth,
// 0x0070C6 - Amount of time Hawkmouth is open (before Wart)
	nmdHawkmouthTimeout,
// 0x0071E0 - Health for Wart of World 7
	nmdWartHealth,
// 0x00721D - Wart's speed (left)
	nmdWartSpeedLeft,
// 0x007223 - Wart's speed (right)
	nmdWartSpeedRight
} NES_MISC_DATA_ENUM, *PNES_MISC_DATA_ENUM;
*/

typedef DWORD NES_MISC_DATA_ENUM;

typedef struct _tagNES_MISC_DATA
{
	CString				sDescription;
	DWORD				ptr;
	BYTE				value;
} NES_MISC_DATA, *PNES_MISC_DATA;

class CMiscDataDlg: public CDialog
{
	CFileLoader			& m_File;
	CListView			m_lv;
	CComboBox			m_cb;
	NES_MISC_DATA_ENUM	m_nmCurSel;

	static int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);  
	int					LVSort(NES_MISC_DATA_ENUM lv1, NES_MISC_DATA_ENUM lv2);

	std::map<NES_MISC_DATA_ENUM, NES_MISC_DATA>	m_mData;

	VOID				OnInit(LPARAM lParam);
	INT_PTR				OnNotify(LPNMHDR lpnm);
	VOID				OnCommand(USHORT uCmd, USHORT uId, HWND hCtl);

	VOID				LoadItems();
	VOID				SaveItems();
	INT					PSOnApply();

public:
	CMiscDataDlg(HINSTANCE hInstance, CFileLoader & fl);
};