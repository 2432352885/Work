/*--------------------------------------------------------------------------------------+
|
|     $Source: MstnExamples/Elements/exampletool/exampletool.h $
|
|  $Copyright: (c) 2015 Bentley Systems, Incorporated. All rights reserved. $
|
+--------------------------------------------------------------------------------------*/
#pragma once

#include <Mstn/MdlApi/MdlApi.h>
#include <Mstn/ElementPropertyUtils.h>
#include <DgnView/AccuDraw.h>
#include <DgnView/DgnElementSetTool.h>
#include <DgnView\DgnTool.h>
#include <DgnView/DgnRegionElementTool.h>
#include <DgnPlatform/DropGraphics.h>
#include <DgnPlatform/NotificationManager.h>
#include <DgnPlatform\DgnPlatformApi.h>
#include <Bentley\Bentley.r.h>
#include <DgnView\DgnTool.h>
#include <DgnView\DgnElementSetTool.h>
#include <Mstn\MdlApi\MdlApi.h>
#include <Mstn\ISessionMgr.h>
#include <Mstn\MdlApi\mselems.h>
#include <Bentley\Bentley.r.h>
#include "exampletoolids.h"
#include "exampletoolcmd.h"

void startExampleMoveElementTool (WCharCP);
void startExampleModifyElementTool (WCharCP);
void startExampleFloodElementTool (WCharCP);
void startExampleViewTool (WCharCP);
void startExampleCreateLineTool (WCharCP);
void startExampleCreateLineStringTool (WCharCP);

void myLevel_comboHook(DialogItemMessage *dimP);
void myLevel_elementStyleHook(DialogItemMessage *dimP);


extern ExampleCreateToolInfo g_line;


