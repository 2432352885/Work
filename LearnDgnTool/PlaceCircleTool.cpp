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
		//_BeginDynamics();
	//启用精准捕捉
	AccuSnap::GetInstance().EnableSnap(true);
	
	__super::_OnPostInstall();
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

WCharCP doubleToConst(double value)
{
	//多小数
	//std::stringstream ss;
	//ss << value;
	//std::string str = ss.str();

	//两位小数
	char buf[5];
	sprintf(buf, "%.2f", value);
	string str = buf;

	WCharP buffer = new WChar[str.length() + 1];
	for(size_t i = 0; i < str.length(); ++i)
	{
		buffer[i] = static_cast<WChar>(str[i]);
	}
	buffer[str.length()] = 0;

	return reinterpret_cast<WCharCP>(buffer);
	
}

BentleyStatus PlaceTriangleTool::CreateTriangle(MSElementDescrP groupDP, DPoint3d  ptTiaCenter)
{
	EditElementHandle eehTriangle;
	EditElementHandle eehTxt;
	DPoint3d center[3];
	//获取给定模型参考的每米UOR数
	double radius = mdlModelRef_getUorPerMeter(ACTIVEMODEL)/1000;
	//倒三角形创建
	DEllipse3d elli = DEllipse3d::FromCenterRadiusXY(ptTiaCenter, radius);
	//三角形的点
	center[0] = center[1] = center[2] = ptTiaCenter;

	center[0].x = ptTiaCenter.x; center[0].y = ptTiaCenter.y;
	center[1].x = ptTiaCenter.x + 15 * radius; center[1].y = ptTiaCenter.y + 15 * radius;
	center[2].x = ptTiaCenter.x - 15 * radius; center[2].y = ptTiaCenter.y + 15 * radius;

	CurveVectorPtr pCurveVec = CurveVector::Create(CurveVector::BOUNDARY_TYPE_Outer);
	pCurveVec->Add(ICurvePrimitive::CreateLineString(center, 3));
	if(SUCCESS != DraftingElementSchema::ToElement(eehTriangle, *pCurveVec, nullptr, ACTIVEMODEL->Is3d(), *ACTIVEMODEL))
		return ERROR;

	//旋转
	RotMatrix rMatrix;
	mdlRMatrix_fromView(&rMatrix, IViewManager::GetManager().GetCurrentViewNumber(), false);
	mdlRMatrix_invert(&rMatrix, &rMatrix);
	Transform transform = Transform::FromMatrixAndFixedPoint(rMatrix, ptTiaCenter);
	TransformInfo transformInfo = TransformInfo(transform);
	eehTriangle.GetDisplayHandler()->ApplyTransform(eehTriangle, transformInfo);
	
//文本
	DgnFilePtr    pActiveDgnFile = ISessionMgr::GetActiveDgnFile();

	DimensionStylePtr pDimStyle = DimensionStyle::GetSettings(*pActiveDgnFile);

	DgnTextStylePtr textStyle = DgnTextStyle::GetSettings(*pActiveDgnFile);
	//修改文字类型以及大小位置
	textStyle->SetProperty(TextStyleProperty::TextStyle_ColorFlag, true);
	UInt32 color = 0;
	textStyle->SetProperty(TextStyleProperty::TextStyle_Color, color);
	double Height = 4 * radius;
	double Width = 4 * radius;
	textStyle->SetProperty(TextStyleProperty::TextStyle_Height, Height);
	textStyle->SetProperty(TextStyleProperty::TextStyle_Width, Width);

	WCharCP ptr =  doubleToConst(center[0].y);
	textStyle->Replace(ptr);
	EditElementHandle noteElem, leaderElement;
	TextBlockPropertiesPtr txtBlockProp = TextBlockProperties::Create(*textStyle, *(ACTIVEMODEL->AsDgnModelP()));
	ParagraphPropertiesPtr paragraphProperties = ParagraphProperties::Create(*textStyle, *(ACTIVEMODEL->AsDgnModelP()));
	RunPropertiesPtr runProp = RunProperties::Create(*textStyle, *(ACTIVEMODEL->AsDgnModelP()));
	TextBlockPtr textbock = TextBlock::Create(*txtBlockProp, *paragraphProperties, *runProp, *(ACTIVEMODEL->AsDgnModelP()));
	double dx = (center[1].x + center[2].x)/2 - 1;
	double dy = center[1].y + 5 * radius;
	
	textbock->AppendText(ptr);
	TextHandlerBase::CreateElement(eehTxt, NULL, *textbock);

	delete[] reinterpret_cast<WCharCP>(ptr);

	//移动
	RotMatrix matrix = RotMatrix::FromIdentity();
	DPoint3d translation = DPoint3d::From(dx, dy, 0);
	Transform tran = Transform::From(matrix, translation);
	TransformInfo transform1 (tran);
	eehTxt.GetHandler().ApplyTransform(eehTxt, transform1);
	eehTxt.ReplaceInModel(eehTxt.GetElementRef());
	//eehTxt.AddToModel();

//合并成cell
	//MSElementDescrP groupDP;
	//EditElementHandle  eeh;
	//NormalCellHeaderHandler::CreateOrphanCellElement(eeh, NULL, ACTIVEMODEL->Is3d(), *ACTIVEMODEL);
	//mdlElement_memcpy(&cell, eeh.GetElementCP());
	//mdlElmdscr_new(&groupDP, NULL, &cell);
	eehTriangle.GetElementDescrP();
	eehTxt.GetElementDescrP();
	MSElementDescrP elmDP1 = eehTriangle.ExtractElementDescr();
	mdlElmdscr_appendDscr(groupDP, elmDP1);
	MSElementDescrP elmDP2 = eehTxt.ExtractElementDescr();
	mdlElmdscr_appendDscr(groupDP, elmDP2);
	
	//ITxnManager::GetManager().GetCurrentTxn().DeleteElement(elmDP1->h.elementRef);
	//ITxnManager::GetManager().GetCurrentTxn().DeleteElement(elmDP2->h.elementRef);
	/*if (groupDP->h.firstElem && groupDP->h.firstElem->h.next)
	{
		groupDP->h.isValid = false;
		mdlCell_setOriginAndRange(groupDP);
		mdlElmdscr_add(groupDP);
		if (0L != mdlElmdscr_add(groupDP))
			SelectionSetManager::GetManager().AddElement(groupDP->h.elementRef, groupDP->h.dgnModelRef);
		//mdlElmdscr_freeAll(&groupDP);
	}
	

	/*AssocPoint assocPt1;
	AssociativePoint::InitKeypoint(assocPt1, 0, 0, 0, 1);
	AssociativePoint::SetRoot(assocPt1, eehTxt.GetElementId(), 0);
	TextHandlerBase::SetupOffsetAssociation(eehTxt, eehTriangle, assocPt1);
	eehTxt.AddToModel();*/

	return SUCCESS;

}

PlaceTriangleTool::PlaceTriangleTool() : DgnPrimitiveTool(0, 0)
{

}

void PlaceTriangleTool::_OnPostInstall()
{
	//启用动态绘制
	_BeginDynamics();
	AccuSnap::GetInstance().EnableSnap(true);
	__super::_OnPostInstall();
}

//获取鼠标位置 
void PlaceTriangleTool::_OnDynamicFrame(DgnButtonEventCR ev)
{
	EditElementHandle eeh;
	MSElement cell;
	mdlCell_create(&cell, NULL, NULL, FALSE);
	MSElementDescrP cellEdP;
	mdlElmdscr_new(&cellEdP, NULL, &cell);
	if (SUCCESS != CreateTriangle(cellEdP, *ev.GetPoint()))
	 	return;
	
	//由于DoRedraw这个接口会自动忽略.next，我们通过循环来预览所有元素
	RedrawElems redrawElems;
	redrawElems.SetDynamicsViews(IViewManager::GetActiveViewSet(), ev.GetViewport());
	redrawElems.SetDrawMode(DRAW_MODE_TempDraw);
	redrawElems.SetDrawPurpose(DrawPurpose::Dynamics);
	MSElementDescrP ChildedP = cellEdP->h.firstElem;
	//由于DoRedraw这个接口会自动忽略.next，我们通过循环来预览所有元素
	while (ChildedP != NULL)
	{
			redrawElems.DoRedraw(ChildedP);
			ChildedP = ChildedP->h.next;
	}
}

bool PlaceTriangleTool::_OnResetButton(DgnButtonEventCR ev)
{
	_EndDynamics();
	_ExitTool();
	//EditElementHandle eeh;
	//MSElement cell;
	//mdlCell_create(&cell, NULL, NULL, FALSE);
	//MSElementDescrP cellEdP;
	//mdlElmdscr_new(&cellEdP, NULL, &cell);
	//EditElementHandle eeh;
	///if (CreateTriangle(cellEdP, *ev.GetPoint()))
		//mdlElmdscr_add(cellEdP);

	//_OnReinitialize();
	return true;
}

bool PlaceTriangleTool::_OnDataButton(DgnButtonEventCR ev)
{
	
	//EditElementHandle eeh;
	MSElement cell;
	mdlCell_create(&cell, NULL, NULL, FALSE);
	MSElementDescrP cellEdP;
	mdlElmdscr_new(&cellEdP, NULL, &cell);
    if (SUCCESS != CreateTriangle(cellEdP, *ev.GetPoint()))
		return false;
	
	mdlElmdscr_add(cellEdP);
	_BeginDynamics();
	EnableUndoPreviousStep();//使用Ctrl + Z 可以取消
	
	_OnReinitialize();
	return true;
}

void PlaceTriangleTool::_OnRestartTool()
{
	InstallNewInstance();
}

