/*--------------------------------------------------------------------------------------+
|
|     $Source: MstnExamples/Elements/exampletool/exampletool.cpp $
|
|  $Copyright: (c) 2015 Bentley Systems, Incorporated. All rights reserved. $
|
+--------------------------------------------------------------------------------------*/
#include "exampletool.h"
#include"exampletoolids.h"

ExampleCreateToolInfo g_line;
/*---------------------------------------------------------------------------------**//**
* @description  Map key-in to function
* Commands
+---------------+---------------+---------------+---------------+---------------+------*/
static MdlCommandNumber s_commandNumbers[] =
    {
    {startExampleMoveElementTool,       CMD_EXAMPLE_MOVETOOL},
    {startExampleModifyElementTool,     CMD_EXAMPLE_MODIFYTOOL},
    {startExampleFloodElementTool,      CMD_EXAMPLE_FLOODTOOL},
    {startExampleViewTool,              CMD_EXAMPLE_VIEWTOOL},
    {startExampleCreateLineTool,        CMD_EXAMPLE_LINETOOL},
    {startExampleCreateLineStringTool,  CMD_EXAMPLE_LSTRINGTOOL},
    // end of list
    0
    };


//对话框挂钩功能“发布”结构用于和对话框挂钩和对话框项挂钩
DialogHookInfo uHooks[] =
{
	{HOOKITEMID_MyLevelCombo, (PFDialogHook)myLevel_comboHook},
	{HOOKITEMID_MyLineStyle, (PFDialogHook)myLevel_elementStyleHook},
};
/*---------------------------------------------------------------------------------**//**
* @description  MdlMain
* @param        argc      The number of command line parameters sent to the application.
* @param        argv[]    The array of strings sent to the application on the command line.
* @bsimethod                                                              Bentley Systems
+---------------+---------------+---------------+---------------+---------------+------*/
extern "C" void MdlMain (int argc, WCharCP argv[])
    {
    RscFileHandle rfHandle;

    mdlResource_openFile (&rfHandle, NULL, RSC_READONLY);
    mdlState_registerStringIds (STRINGLISTID_Commands, STRINGLISTID_Prompts);
    mdlSystem_registerCommandNumbers (s_commandNumbers);
    mdlParse_loadCommandTable (NULL);

	mdlDialog_hookPublish(sizeof(uHooks) / sizeof(DialogHookInfo), uHooks);

	SymbolSet *setP = mdlCExpression_initializeSet(VISIBILITY_DIALOG_BOX, 0, 0);
	//发布用于C表达式字符串的类型变量
	mdlDialog_publishComplexVariable(setP, "examplecreatetoolinfo", "g_line", &g_line);
	g_line.color = 23;
	g_line.LineWidth = 1;
    }
