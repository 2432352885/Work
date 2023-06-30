#include "PlaceCircleTool.h"

void PlaceCircleTool::InstallNewInstance()
{
	PlaceCircleTool* newTool = new PlaceCircleTool();
	newTool->InstallTool();
}

BentleyStatus PlaceCircleTool::CreateCircle(EditElementHandleR eehCirc, DPoint3d const &ptCircCenter)
{
	
	double radius = mdlModelRef_getUorPerMeter(ACTIVEMODEL);
	DEllipse3d elli = DEllipse3d::FromCenterRadiusXY(ptCircCenter, radius);
	ICurvePrimitivePtr cvePri = ICurvePrimitive::CreateArc(elli);
	if(SUCCESS!=DraftingElementSchema::ToElement(eehCirc, *cvePri, NULL, true, *ACTIVEMODEL))
	{
		return ERROR;
	}
	RotMatrix rMatrix;
	mdlRMatrix_fromView(&rMatrix, IViewManager::GetManager().GetCurrentViewNumber(), false);
	mdlRMatrix_invert(&rMatrix, &rMatrix);
	Transform transform = Transform::FromMatrixAndFixedPoint(rMatrix, ptCircCenter);
	TransformInfo transformInfo = TransformInfo(transform);
	eehCirc.GetDisplayHandler()->ApplyTransform(eehCirc, transformInfo);
	return SUCCESS;
}

PlaceCircleTool::PlaceCircleTool():DgnPrimitiveTool(0,0)
{

}

void PlaceCircleTool::_OnPostInstall()
{
		_BeginDynamics();
}

bool PlaceCircleTool::_OnDataButton(DgnButtonEventCR ev)
{
	EditElementHandle eehCirc;
	if (SUCCESS == CreateCircle(eehCirc, *ev.GetPoint()))
	{
		eehCirc.AddToModel();
	}
	return false;
}

bool PlaceCircleTool::_OnResetButton(DgnButtonEventCR ev)
{
	_OnReinitialize();
	return false;
}

void PlaceCircleTool::_OnDynamicFrame(DgnButtonEventCR ev)
{
	EditElementHandle eehCirc;
	if (SUCCESS == CreateCircle(eehCirc, *ev.GetPoint()))
	{
		RedrawElems     redrawElems;
		redrawElems.SetDynamicsViews(IViewManager::GetActiveViewSet(), ev.GetViewport());
		redrawElems.SetDrawMode(DRAW_MODE_TempDraw);
		redrawElems.SetDrawPurpose(DrawPurpose::Dynamics);
		redrawElems.DoRedraw(eehCirc);
	}
}

void PlaceCircleTool::_OnRestartTool()
{
	InstallNewInstance();
}


void PlaceTriangleTool::InstallNewInstance()
{
	PlaceTriangleTool* newTool = new PlaceTriangleTool();
	newTool->InstallTool();
}


BentleyStatus PlaceTriangleTool::CreateTriangle(EditElementHandleR eehTriangle, DPoint3d  ptTiaCenter)
{
	DPoint3d center[3];
	//获取给定模型参考的每米UOR数
	double radius = mdlModelRef_getUorPerMeter(ACTIVEMODEL);
	

	DEllipse3d elli = DEllipse3d::FromCenterRadiusXY(ptTiaCenter, radius);

	center[0] = center[1] = center[2] = ptTiaCenter;

	center[0].x = ptTiaCenter.x; center[0].y = ptTiaCenter.y;
	center[1].x = ptTiaCenter.x + 2 * radius; center[1].y = ptTiaCenter.y + 2 * radius;
	center[2].x = ptTiaCenter.x - 2 * radius; center[2].y = ptTiaCenter.y + 2 * radius;

	CurveVectorPtr pCurveVec = CurveVector::Create(CurveVector::BOUNDARY_TYPE_Outer);
	pCurveVec->Add(ICurvePrimitive::CreateLineString(center, 3));
	if(SUCCESS != DraftingElementSchema::ToElement(eehTriangle, *pCurveVec, nullptr, ACTIVEMODEL->Is3d(), *ACTIVEMODEL))
		return ERROR;
    
	RotMatrix rMatrix;
	mdlRMatrix_fromView(&rMatrix, IViewManager::GetManager().GetCurrentViewNumber(), false);
	mdlRMatrix_invert(&rMatrix, &rMatrix);
	Transform transform = Transform::FromMatrixAndFixedPoint(rMatrix, ptTiaCenter);
	TransformInfo transformInfo = TransformInfo(transform);
	eehTriangle.GetDisplayHandler()->ApplyTransform(eehTriangle, transformInfo);

	return SUCCESS;

}

PlaceTriangleTool::PlaceTriangleTool() : DgnPrimitiveTool(0, 0)
{

}

void PlaceTriangleTool::_OnPostInstall()
{
	//启用动态绘制
	_BeginDynamics();
	//AccuSnap::GetInstance().EnableSnap(true);
	//__super::_OnPostInstall();
}

//获取鼠标位置 
void PlaceTriangleTool::_OnDynamicFrame(DgnButtonEventCR ev)
{
	EditElementHandle eeh;
	if (SUCCESS == CreateTriangle(eeh, *ev.GetPoint()))
	{
		RedrawElems     redrawElems;
		redrawElems.SetDynamicsViews(IViewManager::GetActiveViewSet(), ev.GetViewport());
		redrawElems.SetDrawMode(DRAW_MODE_TempDraw);
		redrawElems.SetDrawPurpose(DrawPurpose::Dynamics);
		redrawElems.DoRedraw(eeh);
	}
	
}

bool PlaceTriangleTool::_OnResetButton(DgnButtonEventCR ev)
{
	//_EndDynamics();
	//_ExitTool();
	_OnReinitialize();
	return true;
}

bool PlaceTriangleTool::_OnDataButton(DgnButtonEventCR ev)
{
	EditElementHandle eeh;
	if (SUCCESS != CreateTriangle(eeh, *ev.GetPoint()))
		return false;
	eeh.AddToModel();
	return true;
}



void PlaceTriangleTool::_OnRestartTool()
{
	InstallNewInstance();
}

