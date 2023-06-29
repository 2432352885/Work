/*--------------------------------------------------------------------------------------+
|
|     $Source: MstnExamples/Elements/exampletool/exampletoolids.h $
|
|  $Copyright: (c) 2015 Bentley Systems, Incorporated. All rights reserved. $
|
+--------------------------------------------------------------------------------------*/
#pragma once

#include <Bentley\Bentley.r.h>
#define STRINGLISTID_Commands               0
#define STRINGLISTID_Prompts                1

#define CMDNAME_ExampleMoveTool             1
#define CMDNAME_ExampleModifyTool           2
#define CMDNAME_ExampleFloodTool            3
#define CMDNAME_ExampleViewTool             4
#define CMDNAME_ExampleLineTool             5
#define CMDNAME_ExampleLStringTool          6

#define PROMPT_SelectView                   1
#define PROMPT_FirstPoint                   2
#define PROMPT_NextPoint                    3
#define PROMPT_NextPointOrReset             4

/*
//�ô��Ǵ���GUIͼ���û�����
#define ItemList_ExampleCreateTool 1
//�߿�
#define TEXTID_LineWidth  1
//��ɫ
#define TEXTID_Color 1
//������
#define HOOLLITEMID_MyLevelCombo 1
*/

#define ItemList_ExampletoolCreateTool       1

#define COLORCHOOSERID_Color          1

#define TEXTID_LineWidth             2
#define TEXTID_LineColor             1

#define COMBOBOXID_MyLevel            1

#define HOOKITEMID_MyLevelCombo       1

#define HOOKITEMID_MyLineStyle   2

#define SYNONYMID_DialogDemoColor 1
#define SYNONYMID_DialogDemoWeight 2

#define PULLDOWNPTION_ElementStyly 1

#define OPTIONBUTTONID_LineStyle 1
#define OPTIONBUTTONID_LineWidth 2

typedef struct examplecreatetoolinfo
{
	UInt32 LineWidth;
	UInt32 color;
	Int32 style;
}ExampleCreateToolInfo;
