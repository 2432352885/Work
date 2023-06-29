/*--------------------------------------------------------------------------------------+
|
|     $Source: MstnExamples/Elements/exampletool/english/exampletoolmsg.r $
|
|  $Copyright: (c) 2015 Bentley Systems, Incorporated. All rights reserved. $
|
+--------------------------------------------------------------------------------------*/
#include <Mstn\MdlApi\rscdefs.r.h>
#include <Mstn\MicroStation.r.h>
#include <Mstn\MdlApi\dlogbox.r.h>
#include <Mstn\MdlApi\dlogids.r.h> 
#include <Mstn\MdlApi\keys.r.h>
#include "exampletoolids.h"

#define Y45 GENY(11.5)
#define  W2 (5*XC)
#define X6 (8 * XC)  /*text Color*/
#define W1 (6 * XC)  /*text color*/
#define X7 (X6 + W1 + 2 * XC) /*color pocker*/
/*----------------------------------------------------------------------+
|    Command Names                                                      |
+----------------------------------------------------------------------*/
MessageList STRINGLISTID_Commands =
{
    {
    { CMDNAME_ExampleMoveTool,      "Move Element Example" },
    { CMDNAME_ExampleModifyTool,    "Modify Element Example" },
    { CMDNAME_ExampleFloodTool,     "Flood Element Example" },
    { CMDNAME_ExampleViewTool,      "Un-rotate View Example" },
    { CMDNAME_ExampleLineTool,      "Place Line Example" },
    { CMDNAME_ExampleLStringTool,   "Place Line String Example" },
    }
};

MessageList STRINGLISTID_Prompts =
{
    {
    { PROMPT_SelectView,            "Select view" },
    { PROMPT_FirstPoint,            "Enter first point" },
    { PROMPT_NextPoint,             "Enter next point" },
    { PROMPT_NextPointOrReset,      "Enter next point or reset to complete" },
    }
};

CmdItemListRsc  ItemList_ExampletoolCreateTool =
{ {
    { {10 * XC, YC / 2, W2, 0},  Text,           TEXTID_LineColor, ON, 0, "", ""},/*color框*/
	{ {15 * XC, YC / 2, 0, 0 },  ColorPicker, 	  COLORCHOOSERID_Color,  ON, 0, "", "" },/*颜色选择*/
    { {10 * XC, YC * 2, W2, 0},  Text, 	  TEXTID_LineWidth,  ON, 0, "", "" },/*线宽变化*/
    { {15 * XC, YC * 2, 0, 0 }, OptionButton, OPTIONBUTTONID_LineWidth, ON, 0, "", ""},/*线宽选择*/
    { {10 * XC, YC * 4, 0, 0}, OptionButton, OPTIONBUTTONID_LineStyle, ON, 0, "", ""},/*线样式选择*/

} };

/*线样式通过宏定义中的ICONID_LineStyle通过OptionButton中显示*/
DItem_OptionButtonRsc OPTIONBUTTONID_LineStyle = 
{
    NOSYNONYM, NOHELP, LHELP,
    HOOKITEMID_MyLineStyle, OPTNBTNATTR_NEWSTYLE | NOARG,
    "LineStyle #:",
    "g_line.style",
    {
    {Icon, ICONID_LineStyle0Wide, NOCMD, MCMD, 0, NOMASK, ON, "~0"},
    {Icon, ICONID_LineStyle1Wide, NOCMD, MCMD, 1, NOMASK, ON, "~1"},
    {Icon, ICONID_LineStyle2Wide, NOCMD, MCMD, 2, NOMASK, ON, "~2"},
    {Icon, ICONID_LineStyle3Wide, NOCMD, MCMD, 3, NOMASK, ON, "~3"},
    {Icon, ICONID_LineStyle4Wide, NOCMD, MCMD, 4, NOMASK, ON, "~4"},
    {Icon, ICONID_LineStyle5Wide, NOCMD, MCMD, 5, NOMASK, ON, "~5"},
    {Icon, ICONID_LineStyle6Wide, NOCMD, MCMD, 6, NOMASK, ON, "~6"},
    {Icon, ICONID_LineStyle7Wide, NOCMD, MCMD, 7, NOMASK, ON, "~7"},
    }
};

/*LineWidth线宽变化 通过同义结构实现两个Item交互*/
DItem_OptionButtonRsc OPTIONBUTTONID_LineWidth = 
{
    SYNONYMID_DialogDemoWeight, NOHELP, LHELP,
    HOOKITEMID_MyLineStyle, OPTNBTNATTR_NEWSTYLE | NOARG,
    "",
    "g_line.LineWidth",
    {
    {Icon, ICONID_LineWeight0, NOCMD, MCMD, 0, NOMASK, ON, "~0"},
    {Icon, ICONID_LineWeight1, NOCMD, MCMD, 1, NOMASK, ON, "~1"},
    {Icon, ICONID_LineWeight2, NOCMD, MCMD, 2, NOMASK, ON, "~2"},
    {Icon, ICONID_LineWeight3, NOCMD, MCMD, 3, NOMASK, ON, "~3"},
    {Icon, ICONID_LineWeight4, NOCMD, MCMD, 4, NOMASK, ON, "~4"},
    {Icon, ICONID_LineWeight5, NOCMD, MCMD, 5, NOMASK, ON, "~5"},
    {Icon, ICONID_LineWeight6, NOCMD, MCMD, 6, NOMASK, ON, "~6"},
    {Icon, ICONID_LineWeight7, NOCMD, MCMD, 7, NOMASK, ON, "~7"},
    {Icon, ICONID_LineWeight8, NOCMD, MCMD, 8, NOMASK, ON, "~8"},
    {Icon, ICONID_LineWeight9, NOCMD, MCMD, 9, NOMASK, ON, "~9"},
    {Icon, ICONID_LineWeight10, NOCMD, MCMD, 10, NOMASK, ON, "~10"},
    }
};

DItem_ColorPickerRsc COLORCHOOSERID_Color =
    {
    NOCMD, LCMD, SYNONYMID_DialogDemoColor, NOHELP, LHELPCMD,
    NOHOOK, NOARG,
    TEXTID_LineColor, NOMASK,
    "",
    "g_line.color"
    };

DItem_TextRsc TEXTID_LineColor =
    {
    NOCMD, LCMD, SYNONYMID_DialogDemoColor, NOHELP, LHELPCMD,
    NOHOOK, NOARG,
    3, "%-ld", "%ld", "0", "253", NOMASK, NOCONCAT,
    "Color #:",
    "g_line.color"
    };

DItem_TextRsc TEXTID_LineWidth =
{
	NOCMD, LCMD, SYNONYMID_DialogDemoWeight, NOHELP, MHELP, NOHOOK, NOARG,
	2, "%ld", "%ld", "0", "", NOMASK, TEXT_NOCONCAT, "LineWidth:", "g_line.LineWidth"
};

/*项目同义词结构*/
DItem_SynonymsRsc SYNONYMID_DialogDemoColor =
    {
    {
    {Text,        TEXTID_LineColor},
    {ColorPicker, COLORCHOOSERID_Color},
    }
    };


DItem_SynonymsRsc SYNONYMID_DialogDemoWeight =
    {
    {
    {Text,        TEXTID_LineWidth},
    {ColorPicker, OPTIONBUTTONID_LineWidth},
    }
    };
