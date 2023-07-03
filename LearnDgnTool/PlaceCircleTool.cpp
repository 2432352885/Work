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


BentleyStatus PlaceTriangleTool::CreateTriangle(MSElement cell, DPoint3d  ptTiaCenter)
{
	EditElementHandle eehTriangle;
	DPoint3d center[3];
	//获取给定模型参考的每米UOR数
	double radius = mdlModelRef_getUorPerMeter(ACTIVEMODEL);
	//倒三角形创建
	DEllipse3d elli = DEllipse3d::FromCenterRadiusXY(ptTiaCenter, radius);
	//三角形的点
	center[0] = center[1] = center[2] = ptTiaCenter;

	center[0].x = ptTiaCenter.x; center[0].y = ptTiaCenter.y;
	center[1].x = ptTiaCenter.x + 2 * radius; center[1].y = ptTiaCenter.y + 2 * radius;
	center[2].x = ptTiaCenter.x - 2 * radius; center[2].y = ptTiaCenter.y + 2 * radius;

	CurveVectorPtr pCurveVec = CurveVector::Create(CurveVector::BOUNDARY_TYPE_Outer);
	pCurveVec->Add(ICurvePrimitive::CreateLineString(center, 3));
	if(SUCCESS != DraftingElementSchema::ToElement(eehTriangle, *pCurveVec, nullptr, ACTIVEMODEL->Is3d(), *ACTIVEMODEL))
		return ERROR;

	eehTriangle.GetElementDescrP();
	mdlElmdscr_appendDscr(cellEdP, eehTriangle.ExtractElementDescr());
	RotMatrix rMatrix;
	mdlRMatrix_fromView(&rMatrix, IViewManager::GetManager().GetCurrentViewNumber(), false);
	mdlRMatrix_invert(&rMatrix, &rMatrix);
	Transform transform = Transform::FromMatrixAndFixedPoint(rMatrix, ptTiaCenter);
	TransformInfo transformInfo = TransformInfo(transform);
	eehTriangle.GetDisplayHandler()->ApplyTransform(eehTriangle, transformInfo);
	
	//BentleyStatus bstatus;
	/*
	DPoint3d startPoint = center[1];
	DPoint3d endPoint = {0,0,0};
	endPoint.x = center[1].x - 20000;
	endPoint.y = center[1].y + 10000;
	*/
	DgnFilePtr    pActiveDgnFile = ISessionMgr::GetActiveDgnFile();

	DimensionStylePtr pDimStyle = DimensionStyle::GetSettings(*pActiveDgnFile);

	DgnTextStylePtr textStyle = DgnTextStyle::GetSettings(*pActiveDgnFile);
	
	EditElementHandle noteElem, leaderElement;
	TextBlockPropertiesPtr txtBlockProp = TextBlockProperties::Create(*textStyle, *(ACTIVEMODEL->AsDgnModelP()));
	ParagraphPropertiesPtr paragraphProperties = ParagraphProperties::Create(*textStyle, *(ACTIVEMODEL->AsDgnModelP()));
	RunPropertiesPtr runProp = RunProperties::Create(*textStyle, *(ACTIVEMODEL->AsDgnModelP()));
	TextBlockPtr textbock = TextBlock::Create(*txtBlockProp, *paragraphProperties, *runProp, *(ACTIVEMODEL->AsDgnModelP()));
	textbock->AppendText(L"4.00");

	EditElementHandle eehTxt;
	TextHandlerBase::CreateElement(eehTxt, NULL, *textbock);
	AssocPoint assocPt1;
	AssociativePoint::InitKeypoint(assocPt1, 0, 0, 0, 1);
	AssociativePoint::SetRoot(assocPt1, eehTxt.GetElementId(), 0);
	TextHandlerBase::SetupOffsetAssociation(eehTxt, eehTriangle, assocPt1);
	eehTxt.AddToModel();


/*

	NoteCellHeaderHandler::StdDPointVector  noteLeaderPoints;
	noteLeaderPoints.push_back(startPoint);
	noteLeaderPoints.push_back(endPoint);
	bstatus = NoteCellHeaderHandler::CreateNote(noteElem, leaderElement, *textbock, *pDimStyle, false, *ACTIVEMODEL, noteLeaderPoints);
	IDimensionEdit* iDimEdit = dynamic_cast<IDimensionEdit*>(&leaderElement.GetHandler());
	
	

	EditElementHandle lineEeh;
	
	//DPoint3d ptArr[2] = { startPoint,startPoint };
	//bstatus = LineStringHandler::CreateLineStringElement(lineEeh, NULL, ptArr,2, false, *ACTIVEMODEL);
	//lineEeh.AddToModel();

	AssocPoint assocPt1;
	AssociativePoint::InitKeypoint(assocPt1, 1, 2, 0, 1);
	//通过用一对元素id设置依赖关系的目标或者根元素 完成新关联点的设置
	//bstatus = AssociativePoint::SetRoot(assocPt1, eehTriangle.GetElementId(), 0, 0);
	//eehTriangle.AddToModel();

	if (SUCCESS != iDimEdit->SetPoint(leaderElement, nullptr, &assocPt1, 0))
	{
		return ERROR;
	}
 

	if (iDimEdit->GetNumPoints(leaderElement) > 0)
	{
		iDimEdit->DeletePoint(leaderElement, 0);
		iDimEdit->InsertPoint(leaderElement, &startPoint, nullptr, *pDimStyle, 0);
	}
	iDimEdit->SetRotationMatrix(leaderElement, RotMatrix::FromIdentity());

	
	bstatus = NoteCellHeaderHandler::AddToModel(noteElem, leaderElement, *(ACTIVEMODEL->AsDgnModelP()));
	//lineEeh.DeleteFromModel();
*/
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
	MSElement cell;
	mdlCell_create(&cell, NULL, NULL, FALSE);
	MSElementDescrP cellEdP;
	mdlElmdscr_new(&cellEdP, NULL, &cell);
	if (SUCCESS != CreateTriangle(cellEdp, *ev.GetPoint()))
	 	return;
	RedrawElems     redrawElems;
	redrawElems.SetDynamicsViews(IViewManager::GetActiveViewSet(), ev.GetViewport());
	redrawElems.SetDrawMode(DRAW_MODE_TempDraw);
	redrawElems.SetDrawPurpose(DrawPurpose::Dynamics);
	redrawElems.DoRedraw(eeh);
	
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
	_OnReinitialize();
	return true;
}

void PlaceTriangleTool::_OnRestartTool()
{
	InstallNewInstance();
}

