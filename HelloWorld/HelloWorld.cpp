/*-------------------------------------------------------------+
|   HelloWorld.cpp                                                |
+-------------------------------------------------------------*/
#include <Mstn\MdlApi\MdlApi.h>
#include <DgnPlatform\DgnPlatformApi.h>
#include <DgnView\DgnElementSetTool.h>
#include <Mstn\ISessionMgr.h>
#include "HelloWorldCmd.h"
#include "HelloWorld.h"

USING_NAMESPACE_BENTLEY_DGNPLATFORM
USING_NAMESPACE_BENTLEY_MSTNPLATFORM
USING_NAMESPACE_BENTLEY_MSTNPLATFORM_ELEMENT

HelloWorldInfo g_helloWorld;
double         g_1mu;

struct PlaceBsSurfaceTool : DgnPrimitiveTool
{
	PlaceBsSurfaceTool(int toolId, int promptId) : DgnPrimitiveTool(toolId, promptId) {}

	bool CreateBsSurface(EditElementHandleR eeh, DPoint3dCP pBasePt)
	{
		MSBsplineSurface bsSurface;
		MSBsplineCurve   bsCurves[4];
		DPoint3d         center[4];
		RotMatrix        rMatrix[4];
		double           radius = g_helloWorld.baseArcRadius;

		center[0] = center[1] = center[2] = center[3] = *pBasePt;
		center[0].x += radius;
		center[1].x += radius*2;  center[1].y += radius;
		center[2].x += radius;    center[2].y += radius*2;
		center[3].y += radius;

		DVec3d xVec = DVec3d::From(1, 0, 0), negativeXVec = DVec3d::From(-1, 0, 0);
		DVec3d yVec = DVec3d::From(0, 1, 0), negativeYVec = DVec3d::From(0, -1, 0);
		DVec3d zVec = DVec3d::From(0, 0, 1);
		rMatrix[0].InitFrom2Vectors(xVec, zVec);  //Front View
		rMatrix[1].InitFrom2Vectors(yVec, zVec);  //Right View
		rMatrix[2].InitFrom2Vectors(negativeXVec, zVec);  //Back View
		rMatrix[3].InitFrom2Vectors(negativeYVec, zVec);  //Left View

		for (int i = 0; i<4; i++)
		{
			bsCurves[i].InitEllipticArc(center[i], radius, radius, 0, PI, &rMatrix[i]);
		}
		if (SUCCESS != mdlBspline_coonsPatch(&bsSurface, bsCurves))
		{
			for (int i = 0; i<4; i++)
				mdlBspline_freeCurve(&bsCurves[i]);
			return false;
		}
		DraftingElementSchema::ToElement(eeh, bsSurface, nullptr, *ACTIVEMODEL);
		mdlBspline_freeSurface(&bsSurface);
		for (int i = 0; i<4; i++)
			mdlBspline_freeCurve(&bsCurves[i]);
		return true;
	}

	virtual void _OnPostInstall() override
	{
		_BeginDynamics();
		AccuSnap::GetInstance().EnableSnap(true);
		__super::_OnPostInstall();
	}

	virtual void _OnRestartTool() override
	{
		PlaceBsSurfaceTool *pTool = new PlaceBsSurfaceTool(GetToolId(), GetToolPrompt());
		pTool->InstallTool();
	}

	virtual void _OnDynamicFrame(DgnButtonEventCR ev) override
	{
		EditElementHandle eeh;
		if (!CreateBsSurface(eeh, ev.GetPoint()))
			return;

		RedrawElems redrawElems;
		redrawElems.SetDynamicsViews(IViewManager::GetActiveViewSet(), ev.GetViewport());
		redrawElems.SetDrawMode(DRAW_MODE_TempDraw);
		redrawElems.SetDrawPurpose(DrawPurpose::Dynamics);
		redrawElems.DoRedraw(eeh);
	}

	virtual bool _OnDataButton(DgnButtonEventCR ev) override
	{
		EditElementHandle  eeh;
		if (CreateBsSurface(eeh, ev.GetPoint()))
		{
			WString wStr(g_helloWorld.levelName);
			FileLevelCacheR levelCache = ISessionMgr::GetActiveDgnFile()->GetLevelCacheR();
			LevelHandle levelHandle = levelCache.GetLevelByName(wStr.GetWCharCP());
			ElementPropertiesSetterPtr setter = ElementPropertiesSetter::Create();
			setter->SetLevel(levelHandle.GetLevelId());
			setter->Apply(eeh);
			eeh.AddToModel();
		}
		_OnReinitialize();
		return true;
	}

	virtual bool _OnResetButton(DgnButtonEventCR ev) override
	{
		_EndDynamics();
		_ExitTool();
		return true;
	}
};

void createALine(WCharCP unparsed)
{
	DPoint3d basePt = DPoint3d::FromZero();

	EditElementHandle eeh;
	DSegment3d seg;
	seg.Init(basePt, DPoint3d::From(basePt.x + g_1mu * 2, basePt.y + g_1mu));
	ICurvePrimitivePtr pCurve = ICurvePrimitive::CreateLine(seg);
	DraftingElementSchema::ToElement(eeh, *pCurve, nullptr, ACTIVEMODEL->Is3d(), *ACTIVEMODEL);
	eeh.AddToModel();
}

void createAComplexShape(WCharCP unparsed)
{
	EditElementHandle eeh;
	DPoint3d          basePt, pts[3];

	basePt.x = 1.7*g_1mu;   basePt.y = -0.3*g_1mu;    basePt.z = -0.6*g_1mu;
	pts[0] = pts[1] = pts[2] = basePt;
	pts[1].x += g_1mu*0.3;    pts[1].y += g_1mu*0.7;
	pts[0].x += g_1mu;        pts[0].y += g_1mu;
	//三个初始点初始化圆弧
	DEllipse3d arcPts = DEllipse3d::FromPointsOnArc(pts[2], pts[1], pts[0]);
	CurveVectorPtr pCurveVec = CurveVector::Create(CurveVector::BOUNDARY_TYPE_Outer);
	pCurveVec->Add(ICurvePrimitive::CreateArc(arcPts));

	pts[1].x = pts[0].x;    pts[1].y = pts[2].y;
	pCurveVec->Add(ICurvePrimitive::CreateLineString(pts, 3));
	DraftingElementSchema::ToElement(eeh, *pCurveVec, nullptr, ACTIVEMODEL->Is3d(), *ACTIVEMODEL);
	eeh.AddToModel();
}

void createAProjectedSolid(WCharCP unparsed)
{
	DPoint3d  basePt, pts[6];
	basePt.x = 3.2*g_1mu;     basePt.y = -0.6*g_1mu;    basePt.z = -1.2*g_1mu;
	pts[0] = basePt;
	pts[1].x = pts[0].x;               pts[1].y = pts[0].y - g_1mu / 2;   pts[1].z = pts[0].z;
	pts[2].x = pts[1].x + g_1mu / 2;   pts[2].y = pts[1].y;               pts[2].z = pts[0].z;
	pts[3].x = pts[2].x;               pts[3].y = pts[2].y - g_1mu / 2;   pts[3].z = pts[0].z;
	pts[4].x = pts[3].x + g_1mu / 2;   pts[4].y = pts[3].y;               pts[4].z = pts[0].z;
	pts[5].x = pts[4].x;               pts[5].y = pts[0].y;               pts[5].z = pts[0].z;

	//CreateLinear从xyz是数据创建一行字符串或者多边形
	CurveVectorPtr pCurveVec = CurveVector::CreateLinear(pts, 6, CurveVector::BOUNDARY_TYPE_Outer);
	DVec3d extrusionVec = DVec3d::From(0, 0, g_1mu);
	DgnExtrusionDetail  data(pCurveVec, extrusionVec, true);
	ISolidPrimitivePtr pSolid = ISolidPrimitive::CreateDgnExtrusion(data);
	EditElementHandle eeh;
	DraftingElementSchema::ToElement(eeh, *pSolid, nullptr, *ACTIVEMODEL);
	eeh.AddToModel();
}

void createABsplineSurface(WCharCP unparsed)
{
	PlaceBsSurfaceTool *pTool = new PlaceBsSurfaceTool(CMDNAME_PlaceBsSurfaceTool, PROMPT_PlaceBsSurfaceTool);
	pTool->InstallTool();
}

void myLevel_comboHook(DialogItemMessage *dimP)
{
	RawItemHdr      *riP = dimP->dialogItemP->rawItemP;

	dimP->msgUnderstood = TRUE;
	switch (dimP->messageType)
	{
	case DITEM_MESSAGE_CREATE:
	{
		ListModelP pListModel = mdlListModel_create(1);
		DgnFileP   pDgnFile = ISessionMgr::GetActiveDgnFile();
		for (LevelHandle lh : pDgnFile->GetLevelCacheR())
		{
			WString    lvlName;
			lh.GetDisplayName(lvlName);
			mdlListModel_insertString (pListModel, lvlName.GetWCharCP(), -1);
		}
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

MdlCommandNumber cmdNums[] =
{
	{ (CmdHandler)createALine,		        CMD_HELLOWORLD_CREATE_LINE },
	{ (CmdHandler)createAComplexShape,	    CMD_HELLOWORLD_CREATE_COMPLEXSHAPE },
	{ (CmdHandler)createAProjectedSolid,	CMD_HELLOWORLD_CREATE_PROJECTEDSOLID },
	{ (CmdHandler)createABsplineSurface,    CMD_HELLOWORLD_CREATE_BSPLINESURFACE },
	0
};
DialogHookInfo uHooks[] =
{
	{ HOOKITEMID_MyLevelCombo,	   (PFDialogHook)myLevel_comboHook },
};
extern "C" DLLEXPORT void MdlMain(int argc, WCharCP argv[])
{
	ModelInfoCP pInfo = ACTIVEMODEL->GetModelInfoCP();
	g_1mu = pInfo->GetUorPerStorage();

	RscFileHandle rscFileH;
	mdlResource_openFile(&rscFileH, NULL, RSC_READONLY);
	mdlParse_loadCommandTable(NULL);

	mdlSystem_registerCommandNumbers(cmdNums);
	mdlState_registerStringIds(STRINGLISTID_Commands, STRINGLISTID_Prompts);

	mdlDialog_hookPublish(sizeof(uHooks) / sizeof(DialogHookInfo), uHooks);

	SymbolSet  *setP = mdlCExpression_initializeSet(VISIBILITY_DIALOG_BOX, 0, 0);
	mdlDialog_publishComplexVariable(setP, "helloworldinfo", "g_helloWorld", &g_helloWorld);
	g_helloWorld.baseArcRadius = 2000;
	//wstrcpy_s(g_helloWorld.levelName, 512, L"Default");


}
