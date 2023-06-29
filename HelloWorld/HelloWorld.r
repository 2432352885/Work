#include <Mstn\MdlApi\rscdefs.r.h>
#include <Mstn\MdlApi\dlogbox.r.h>
#include "HelloWorld.h"

#define  DLLAPPID     1

DllMdlApp   DLLAPPID =
{
	L"HELLOWORLD", L"HelloWorld"    // taskid, dllName
}

MessageList STRINGLISTID_Commands =
{
	{
		{ CMDNAME_PlaceBsSurfaceTool,            "放置B样条曲面" },
	}
};

MessageList STRINGLISTID_Prompts =
{
	{
		{ PROMPT_PlaceBsSurfaceTool,    "指定一个点" },
	}
};

CmdItemListRsc  ItemList_BsplineSurface =
{ {
	{ { 10 * XC,   YC / 2,   15 * XC, 0 }, Text, 	  TEXTID_BaseArcRadius,  ON, 0, "", "" },
	{ { 10 * XC,   YC * 2,   20 * XC, 0 }, ComboBox,  COMBOBOXID_MyLevel,    ON, 0, "", "" },
	} };

DItem_TextRsc TEXTID_BaseArcRadius =
{
	NOCMD, LCMD, NOSYNONYM, NOHELP, LHELP, NOHOOK, NOARG,
	15, "%w", "%w", "0.0", "", NOMASK, TEXT_NOCONCAT, "基准弧半径(~B):", "g_helloWorld.baseArcRadius"
};

DItem_ComboBoxRsc COMBOBOXID_MyLevel =
{
	NOCMD, LCMD, NOSYNONYM, NOHELP, MHELP, HOOKITEMID_MyLevelCombo, NOARG,
	512, "%s", "%s", "", "", NOMASK,
	0, 18, 4, 0, 0, COMBOATTR_READONLY | COMBOATTR_FULLWIDTH | COMBOATTR_SORT,
	"放置层(~L):", "g_helloWorld.levelName",
	{
		{ 0, 512, ALIGN_LEFT, "" },
	}
};

