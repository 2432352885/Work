/*--------------------------------------------------------------------------------------+
|
|     $Source: MstnExamples/Elements/exampletool/ExampleCreateTool.cpp $
|
|  $Copyright: (c) 2015 Bentley Systems, Incorporated. All rights reserved. $
|
+--------------------------------------------------------------------------------------*/
#include "exampletool.h"
#include "exampletoolids.h"
USING_NAMESPACE_BENTLEY_DGNPLATFORM;
USING_NAMESPACE_BENTLEY_MSTNPLATFORM;
USING_NAMESPACE_BENTLEY_MSTNPLATFORM_ELEMENT;

extern ExampleCreateToolInfo g_line;

/*=================================================================================**//**
* Example showing how to use DgnPrimitiveTool to write a place line tool.
*
* Demonstrates using the RedrawElems class for element dynamics.
* 
* @bsiclass                                                               Bentley Systems
+===============+===============+===============+===============+===============+======*/
//DgnPrimitiveTool�ṹ�� �̳�DgnPrimitiveTool
/*
	DgnPrimitiveTool������ʵ�ֻ�Ԫ����
		���캯��(int toolId, int toolPrompt) toolid����ID toolPrompt������ʾ
*/
struct          ExampleCreateLineTool : DgnPrimitiveTool
{
protected:

bvector<DPoint3d>   m_points;

ExampleCreateLineTool (int toolName, int toolPrompt) : DgnPrimitiveTool (toolName, toolPrompt) {}

virtual void _OnPostInstall () override;
/*���ⲿʱ������ǵ�ǰ���ߵ�״̬��Чʱ����*/
virtual void _OnRestartTool () override {InstallNewInstance (GetToolId (), GetToolPrompt ());}
//���������ݵ�ʱ����
virtual bool _OnDataButton (DgnButtonEventCR ev) override;
/*����������ʱ����*/
virtual bool _OnResetButton (DgnButtonEventCR ev) override {_OnRestartTool (); return true;}
/*ʹ��BeginComplexDynamic����*/
virtual void _OnDynamicFrame (DgnButtonEventCR ev) override;

bool CreateElement (EditElementHandleR eeh, bvector<DPoint3d> const& points);
void SetupAndPromptForNextAction ();

public:

static void InstallNewInstance (int toolId, int toolPrompt);

}; // ExampleCreateLineTool

/*---------------------------------------------------------------------------------**//**
* @bsimethod                                                              Bentley Systems
+---------------+---------------+---------------+---------------+---------------+------*/
/*����Ԫ��*/
bool ExampleCreateLineTool::CreateElement (EditElementHandleR eeh, bvector<DPoint3d> const& points)
    {
    if (2 != points.size ())
        return false;

    // NOTE: Easier to always work with CurveVector/CurvePrimitive and not worry about element specific create methods, ex. LineHandler::CreateLineElement.
    if (SUCCESS != DraftingElementSchema::ToElement (eeh, *ICurvePrimitive::CreateLine (DSegment3d::From (points.front (), points.back ())), NULL, ACTIVEMODEL->Is3d (), *ACTIVEMODEL))
        return false;

	//Line�����޸� ͨ��info
	ElementPropertiesSetterPtr elePropSet = ElementPropertiesSetter::Create();
	//�޸�Line ͨ��Set���������޸�
	elePropSet->SetColor(g_line.color);
	elePropSet->SetWeight(g_line.LineWidth);
    elePropSet->SetLinestyle(g_line.style, NULL);
	elePropSet->Apply(eeh);
	//ElementPropertyUtils::ApplyActiveSettings (eeh);

    return true;
    }

/*---------------------------------------------------------------------------------**//**
* @bsimethod                                                              Bentley Systems
+---------------+---------------+---------------+---------------+---------------+------*/
void ExampleCreateLineTool::SetupAndPromptForNextAction ()
    {
    WString     msgStr;

    RmgrResource::LoadWString (msgStr, (RscFileHandle) 0, STRINGLISTID_Prompts, 1 == m_points.size () ? PROMPT_NextPoint : PROMPT_NextPointOrReset, GetMdlDesc ());
    NotificationManager::OutputPrompt (msgStr.c_str ());

    if (2 != m_points.size ())
        return;

    DVec3d      xVec = DVec3d::FromStartEndNormalize (m_points.front (), m_points.back ());

    AccuDraw::GetInstance ().SetContext (ACCUDRAW_SetXAxis, NULL, &xVec); // Orient AccuDraw to last segment.
    }

/*---------------------------------------------------------------------------------**//**
* @bsimethod                                                              Bentley Systems
+---------------+---------------+---------------+---------------+---------------+------*/
void ExampleCreateLineTool::_OnDynamicFrame (DgnButtonEventCR ev)
    {
    bvector<DPoint3d>   tmpPts = m_points;
    EditElementHandle   eeh;

    tmpPts.push_back (*ev.GetPoint ()); // Use current button location as end point.

    if (!CreateElement (eeh, tmpPts))
        return;

    RedrawElems redrawElems;

    redrawElems.SetDynamicsViews (IViewManager::GetActiveViewSet (), ev.GetViewport ()); // Display in all views, draws to cursor view first...
    redrawElems.SetDrawMode (DRAW_MODE_TempDraw);
    redrawElems.SetDrawPurpose (DrawPurpose::Dynamics);

    redrawElems.DoRedraw (eeh);
    }

/*---------------------------------------------------------------------------------**//**
* @bsimethod                                                              Bentley Systems
+---------------+---------------+---------------+---------------+---------------+------*/
bool ExampleCreateLineTool::_OnDataButton (DgnButtonEventCR ev)
    {
    if (0 == m_points.size ())
        _BeginDynamics (); // Start dynamics on first point. Enables AccuDraw and triggers _OnDynamicFrame being called.

    m_points.push_back (*ev.GetPoint ()); // Save current data point location.
    SetupAndPromptForNextAction ();

    if (m_points.size () < 2)
        return false;

    EditElementHandle   eeh;

    if (CreateElement (eeh, m_points))
        eeh.AddToModel (); // Add new line element to active model.

    m_points.clear (); m_points.push_back (*ev.GetPoint ()); // Start of next line is end of current line.

    return _CheckSingleShot (); // Tool should exit after creating a single line if started in single-shot mode.
    }

/*---------------------------------------------------------------------------------**//**
* @bsimethod                                                              Bentley Systems
+---------------+---------------+---------------+---------------+---------------+------*/
void ExampleCreateLineTool::_OnPostInstall ()
    {
    AccuSnap::GetInstance ().EnableSnap (true); // Enable snapping for create tools.

    __super::_OnPostInstall ();
    }

/*---------------------------------------------------------------------------------**//**
* Method to create and install a new instance of the tool. If InstallTool returns ERROR,
* the new tool instance will be freed/invalid. Never call delete on RefCounted classes.
*
* @bsimethod                                                              Bentley Systems
+---------------+---------------+---------------+---------------+---------------+------*/
void ExampleCreateLineTool::InstallNewInstance (int toolId, int toolPrompt)
    {
    ExampleCreateLineTool* tool = new ExampleCreateLineTool (toolId, toolPrompt);

    tool->InstallTool ();
    }

/*---------------------------------------------------------------------------------**//**
* Function that was associated with the command number for starting the tool.
*
* @param[in] unparsed Additional input supplied after command string.
* @bsimethod                                                              Bentley Systems
+---------------+---------------+---------------+---------------+---------------+------*/
Public void startExampleCreateLineTool (WCharCP unparsed)
    {
    // NOTE: Call the method to create/install the tool, RefCounted classes don't have public constructors...
    ExampleCreateLineTool::InstallNewInstance (CMDNAME_ExampleLineTool, PROMPT_FirstPoint);
    }

/*=================================================================================**//**
* Example showing how to use DgnPrimitiveTool to write a place line string tool.
*
* Demonstrates using transients for snapping to the previously accepted segments 
* during placement dynamics as well as using Ctrl+Z to undo the previous data point.
* 
* @bsiclass                                                               Bentley Systems
+===============+===============+===============+===============+===============+======*/
struct          ExampleCreateLineStringTool : DgnPrimitiveTool
{
protected:

bvector<DPoint3d>   m_points;

ExampleCreateLineStringTool (int toolName, int toolPrompt) : DgnPrimitiveTool (toolName, toolPrompt) {}

virtual void _OnPostInstall () override;
virtual void _OnRestartTool () override {InstallNewInstance (GetToolId (), GetToolPrompt ());}
virtual bool _OnDataButton (DgnButtonEventCR ev) override;
virtual bool _OnResetButton (DgnButtonEventCR ev) override;
virtual void _OnDynamicFrame (DgnButtonEventCR ev) override;
virtual void _OnUndoPreviousStep () override;

bool CreateElement (EditElementHandleR eeh, bvector<DPoint3d> const& points);
void CreateAcceptedSegmentsTransient ();
void SetupAndPromptForNextAction ();

public:

static void InstallNewInstance (int toolId, int toolPrompt);

}; // ExampleCreateLineStringTool

/*---------------------------------------------------------------------------------**//**
* @bsimethod                                                              Bentley Systems
+---------------+---------------+---------------+---------------+---------------+------*/
bool ExampleCreateLineStringTool::CreateElement (EditElementHandleR eeh, bvector<DPoint3d> const& points)
    {
    if (points.size () < 2)
        return false;

    // NOTE: Create a line, linestring, or complex string based on point count.
    if (SUCCESS != DraftingElementSchema::ToElement (eeh, *ICurvePrimitive::CreateLineString (points), NULL, ACTIVEMODEL->Is3d (), *ACTIVEMODEL))
        return false;

    ElementPropertyUtils::ApplyActiveSettings (eeh);

    return true;
    }

/*---------------------------------------------------------------------------------**//**
* @bsimethod                                                              Bentley Systems
+---------------+---------------+---------------+---------------+---------------+------*/
void ExampleCreateLineStringTool::CreateAcceptedSegmentsTransient ()
    {
    EditElementHandle   eeh;

    mdlTransient_free (&msTransientElmP, true); // Use general purpose tool transient (automatically cleared when tool exits).

    if (CreateElement (eeh, m_points))
        msTransientElmP = mdlTransient_addElemDescr (msTransientElmP, eeh.GetElementDescrP (), true, 0xffff, DRAW_MODE_Normal, false, false, true);
    }

/*---------------------------------------------------------------------------------**//**
* @bsimethod                                                              Bentley Systems
+---------------+---------------+---------------+---------------+---------------+------*/
void ExampleCreateLineStringTool::SetupAndPromptForNextAction ()
    {
    WString     msgStr;

    RmgrResource::LoadWString (msgStr, (RscFileHandle) 0, STRINGLISTID_Prompts, 1 == m_points.size () ? PROMPT_NextPoint : PROMPT_NextPointOrReset, GetMdlDesc ());
    NotificationManager::OutputPrompt (msgStr.c_str ());

    if (m_points.size () < 1)
        return;

    DVec3d          xVec   = DVec3d::From (0.0, 0.0, 0.0); 
    DPoint3d        origin = m_points.back (); // Move AccuDraw to last point in case of undo previous step.
    AccuDrawFlags   flags  = ACCUDRAW_SetOrigin;

    if (m_points.size () > 1)
        {
        xVec = DVec3d::FromStartEndNormalize (m_points.back (), *(&m_points.back ()-1)); // Rotate AccuDraw to last segment.
        flags = (AccuDrawFlags) (flags | ACCUDRAW_SetXAxis);
        }

    AccuDraw::GetInstance ().SetContext (flags, &origin, &xVec);
    }

/*---------------------------------------------------------------------------------**//**
* @bsimethod                                                              Bentley Systems
+---------------+---------------+---------------+---------------+---------------+------*/
void ExampleCreateLineStringTool::_OnDynamicFrame (DgnButtonEventCR ev)
    {
    if (m_points.size () < 1)
        return;

    bvector<DPoint3d>   tmpPts;
    EditElementHandle   eeh;

    tmpPts.push_back (m_points.back ()); // Only draw the current segment in dynamics, the accepted segments drawn as transients.
    tmpPts.push_back (*ev.GetPoint ());

    if (!CreateElement (eeh, tmpPts))
        return;

    RedrawElems redrawElems;

    redrawElems.SetDynamicsViews (IViewManager::GetActiveViewSet (), ev.GetViewport ()); // Display in all views, draws to cursor view first...
    redrawElems.SetDrawMode (DRAW_MODE_TempDraw);
    redrawElems.SetDrawPurpose (DrawPurpose::Dynamics);

    redrawElems.DoRedraw (eeh);
    }

/*---------------------------------------------------------------------------------**//**
* @bsimethod                                                              Bentley Systems
+---------------+---------------+---------------+---------------+---------------+------*/
void ExampleCreateLineStringTool::_OnUndoPreviousStep ()
    {
    if (m_points.size () < 2)
        {
        _OnRestartTool (); // Start over from a clean initial state (i.e. Dynamics/AccuDraw not started) by installing a new tool instance.
        return;
        }

    m_points.pop_back ();
    SetupAndPromptForNextAction ();
    CreateAcceptedSegmentsTransient ();
    }

/*---------------------------------------------------------------------------------**//**
* @bsimethod                                                              Bentley Systems
+---------------+---------------+---------------+---------------+---------------+------*/
bool ExampleCreateLineStringTool::_OnDataButton (DgnButtonEventCR ev)
    {
    if (0 == m_points.size ())
        {
        _BeginDynamics (); // Start dynamics on first point. Enables AccuDraw and triggers _OnDynamicFrame being called.
        EnableUndoPreviousStep (); // Enable Ctrl+Z notification to undo the previous data point.
        }

    m_points.push_back (*ev.GetPoint ());
    SetupAndPromptForNextAction ();
    CreateAcceptedSegmentsTransient ();

    return false;
    }

/*---------------------------------------------------------------------------------**//**
* @bsimethod                                                              Bentley Systems
+---------------+---------------+---------------+---------------+---------------+------*/
bool ExampleCreateLineStringTool::_OnResetButton (DgnButtonEventCR ev)
    {
    EditElementHandle   eeh;

    if (CreateElement (eeh, m_points))
        eeh.AddToModel (); // Add new line string element to active model. The place linestring tool uses a reset to accept.

    _OnReinitialize (); // Exits tool if started in single-shot mode, otherwise restarts it.

    return true;
    }

/*---------------------------------------------------------------------------------**//**
* @bsimethod                                                              Bentley Systems
+---------------+---------------+---------------+---------------+---------------+------*/
void ExampleCreateLineStringTool::_OnPostInstall ()
    {
    AccuSnap::GetInstance ().EnableSnap (true); // Enable snapping for create tools.

    __super::_OnPostInstall ();
    }

/*---------------------------------------------------------------------------------**//**
* Method to create and install a new instance of the tool. If InstallTool returns ERROR,
* the new tool instance will be freed/invalid. Never call delete on RefCounted classes.
*
* @bsimethod                                                              Bentley Systems
+---------------+---------------+---------------+---------------+---------------+------*/
void ExampleCreateLineStringTool::InstallNewInstance (int toolId, int toolPrompt)
    {
    ExampleCreateLineStringTool* tool = new ExampleCreateLineStringTool (toolId, toolPrompt);

    tool->InstallTool ();
    }

/*---------------------------------------------------------------------------------**//**
* Function that was associated with the command number for starting the tool.
*
* @param[in] unparsed Additional input supplied after command string.
* @bsimethod                                                              Bentley Systems
+---------------+---------------+---------------+---------------+---------------+------*/
Public void startExampleCreateLineStringTool (WCharCP unparsed)
    {
    // NOTE: Call the method to create/install the tool, RefCounted classes don't have public constructors...
    ExampleCreateLineStringTool::InstallNewInstance (CMDNAME_ExampleLStringTool, PROMPT_FirstPoint);
    }


//�Ի���Ĺ����� �Ի�����ṹ��dialogItemMessage
void myLevel_comboHook(DialogItemMessage *dimP)
{
	//��Դ��ָ������Ϣ�����������ض���Ϣ
	RawItemHdr      *riP = dimP->dialogItemP->rawItemP;

	dimP->msgUnderstood = TRUE;
	switch (dimP->messageType)
	{
	case DITEM_MESSAGE_CREATE:
	{
		//��������ָ��������ListModel
		ListModelP pListModel = mdlListModel_create(1);
		//��ȡ��ǰ�ģʽ���ļ�ָ��
		DgnFileP   pDgnFile = ISessionMgr::GetActiveDgnFile();
		//��ȡ���ļ�����ļ���ķ���Ȩ��
		for (LevelHandle lh : pDgnFile->GetLevelCacheR())
		{
			WString    lvlName;
			//��ȡ�������ŵ���ʾ���� Ҫ��UIѡ��������ʾ������
			lh.GetDisplayName(lvlName);
			//��ListModel��ָ����Unicode�ַ�����Ϊֵ��ĳ������֮ǰ����ListRow
			mdlListModel_insertString(pListModel, lvlName.GetWCharCP(), -1);
		}
		//���öԻ���ʹ��listModelָ�� ListModels��StringList����
		//pListModel:ָ��Ҫ������Ͽ�����Դ��ListModel
		//riPָ����Ͽ����rawItemHdr
		mdlDialog_comboBoxSetListModelP(riP, pListModel);
		break;
	}
	case DITEM_MESSAGE_DESTROY:
	{
		ListModelP   pListModel = mdlDialog_comboBoxGetListModelP(riP);
		mdlListModel_destroy(pListModel, TRUE);
		break;
	}
	default:
		dimP->msgUnderstood = FALSE;
		break;
	}
}

void myLevel_elementStyleHook(DialogItemMessage* dimP)
{
    dimP->msgUnderstood = TRUE;

    switch(dimP->messageType)
    {
    case DITEM_MESSAGE_JOURNALSTATE:
    {
        if (dimP->u.value.msValueDescrP->IsLong())
            {
                int     status=SUCCESS;

                if(SUCCESS == status)
                    dimP->u.value.hookHandled = TRUE;
            }
            break;
    };

    default:
        dimP->msgUnderstood = FALSE;
        break;
    }
}