#include "DrawThreePrismTool.h"
#include <DgnView/AccuDraw.h>

void DrawThreePrismTool::InstallNewInstance()
{
	DrawThreePrismTool* newTool = new DrawThreePrismTool();
	newTool->InstallTool();
}

BentleyStatus DrawThreePrismTool::CreateThreePrism(EditElementHandleR eehThreePrism, DPoint3dR ptStart, DPoint3dR ptEnd, DPoint3dR ptDirection)
{
	DPoint3d ptProjOnLine;
	mdlVec_projectPointToLine(&ptProjOnLine, NULL, &ptDirection, &ptStart, &ptEnd);
	double radius = 0;
	if (0==(radius=mdlVec_distance(&ptProjOnLine, &ptDirection)))
	{
		DSegment3d segment = DSegment3d::From(ptStart, ptEnd);
		if (SUCCESS != LineHandler::CreateLineElement(eehThreePrism, NULL, segment, true, *ACTIVEMODEL))
		{
			return ERROR;
		}
		return SUCCESS;
	}
	DPoint3d ptArr[3] = { {0,radius,0},{-radius / 2 * sqrt(3),-radius / 2,0},{radius / 2 * sqrt(3),-radius / 2,0} };
	ICurvePrimitivePtr iCvePri = ICurvePrimitive::CreateLineString(ptArr, 3);
	CurveVectorPtr cveVec = CurveVector::Create(CurveVector::BoundaryType::BOUNDARY_TYPE_Outer);
	cveVec->Add(iCvePri);

	DgnExtrusionDetail dgnExtruDeta(cveVec, DVec3d::From(0,0, ptStart.Distance(ptEnd)), true);
	ISolidPrimitivePtr iSolidPri = ISolidPrimitive::CreateDgnExtrusion(dgnExtruDeta);
	if (SUCCESS != DraftingElementSchema::ToElement(eehThreePrism, *iSolidPri, NULL, *ACTIVEMODEL))
	{
		return ERROR;
	}
	DVec3d vecZ = DVec3d::FromStartEnd(ptStart, ptEnd);
	vecZ.Normalize();
	DVec3d vecY= DVec3d::FromStartEnd(ptProjOnLine, ptDirection);
	vecY.Normalize();
	DVec3d vecX;
	vecX.CrossProduct(vecY, vecZ);
	RotMatrix rMatrix = RotMatrix::FromColumnVectors(vecX, vecY, vecZ);
	Transform transform = Transform::From(rMatrix, ptStart);
	TransformInfo tranInfo(transform);
	eehThreePrism.GetHandler().ApplyTransform(eehThreePrism, tranInfo);
	return SUCCESS;
}

DrawThreePrismTool::DrawThreePrismTool():DgnPrimitiveTool(0,0)
{

}

bool DrawThreePrismTool::_OnInstall()
{
	if (ACTIVEMODEL->Is3d())
	{
		return true;
	}
	return false;
}

void DrawThreePrismTool::_OnPostInstall()
{
	AccuDraw::GetInstance().Activate();
	EnableUndoPreviousStep();
}

bool DrawThreePrismTool::_OnDataButton(DgnButtonEventCR ev)
{
	RotMatrix rMatrix;
	int accurDrawFlags = AccuDrawFlags::ACCUDRAW_SetOrigin | AccuDrawFlags::ACCUDRAW_SetRMatrix;
	if (0 == m_ptVcr.size())
	{
		accurDrawFlags |= AccuDrawFlags::ACCUDRAW_SetModeRect;
		m_ptVcr.push_back(*ev.GetPoint());
		mdlRMatrix_fromView(&rMatrix, 0, FALSE); 
		DVec3d vec;
		rMatrix.GetColumn(vec, 0);
		AccuDraw::GetInstance().SetContext((AccuDrawFlags)accurDrawFlags, &m_ptVcr[0], (DVec3dCP)rMatrix.form3d, NULL, NULL, NULL, NULL);
		_BeginDynamics();
		return false;
	}
	if (1 == m_ptVcr.size())
	{
		accurDrawFlags |= AccuDrawFlags::ACCUDRAW_SetModePolar;
		if (!mdlVec_distance(&m_ptVcr[0], ev.GetPoint()))
		{
			return false;
		}
		m_ptVcr.push_back(*ev.GetPoint());
		DVec3d vecZ =DVec3d::FromStartEnd(m_ptVcr[0], m_ptVcr[1]);
		vecZ.Normalize();
		if (1 == vecZ.z)
		{
			rMatrix = RotMatrix::FromColumnVectors(DVec3d::UnitX(), DVec3d::UnitY(), DVec3d::UnitZ());
		}
		else
		{
			DVec3d vecX;
			vecX.CrossProduct(DVec3d::UnitZ(), vecZ);
			DVec3d vecY;
			vecY.CrossProduct(vecZ, vecX);
			rMatrix = RotMatrix::FromRowVectors(vecX, vecY, vecZ);
		}
		AccuDraw::GetInstance().SetContext((AccuDrawFlags)accurDrawFlags, &m_ptVcr[1], (DVec3dCP)rMatrix.form3d, NULL, NULL, NULL, NULL);
		return false;
	}
	m_ptVcr.push_back(*ev.GetPoint());
	EditElementHandle eehThreePrism;
	if (SUCCESS == CreateThreePrism(eehThreePrism, m_ptVcr[0], m_ptVcr[1], m_ptVcr[2]))
	{
		eehThreePrism.AddToModel();
	}
	_OnReinitialize();
	return false;
}

bool DrawThreePrismTool::_OnResetButton(DgnButtonEventCR ev)
{
	_OnReinitialize();
	return false;
}

void DrawThreePrismTool::_OnDynamicFrame(DgnButtonEventCR ev)
{
	EditElementHandle eehLineStr;
	DPoint3dR ptStart = m_ptVcr[0];
	DPoint3d ptEnd;
	if (m_ptVcr.size() > 1)
	{
		ptEnd = m_ptVcr[1];
	}
	else
	{
		ptEnd = *ev.GetPoint();
	}
	DPoint3d ptDir = *ev.GetPoint();
	if (SUCCESS == CreateThreePrism(eehLineStr, ptStart, ptEnd, ptDir))
	{
		RedrawElems     redrawElems;
		redrawElems.SetDynamicsViews(IViewManager::GetActiveViewSet(), ev.GetViewport());
		redrawElems.SetDrawMode(DRAW_MODE_TempDraw);
		redrawElems.SetDrawPurpose(DrawPurpose::Dynamics);
		redrawElems.DoRedraw(eehLineStr);
	}
}

void DrawThreePrismTool::_OnUndoPreviousStep()
{
	if (0 == m_ptVcr.size())
		return;
	m_ptVcr.pop_back();
	RotMatrix rMatrix;
	int accurDrawFlags = AccuDrawFlags::ACCUDRAW_SetOrigin | AccuDrawFlags::ACCUDRAW_SetRMatrix;
	if (1 == m_ptVcr.size())
	{
		accurDrawFlags |= AccuDrawFlags::ACCUDRAW_SetModeRect;
		mdlRMatrix_fromView(&rMatrix, 0, FALSE);
		DVec3d vec;
		rMatrix.GetColumn(vec, 0);
		AccuDraw::GetInstance().SetContext((AccuDrawFlags)accurDrawFlags, &m_ptVcr[0], (DVec3dCP)rMatrix.form3d, NULL, NULL, NULL, NULL);
	}
	if (0 == m_ptVcr.size())
	{
		_EndDynamics();
	}
}

void DrawThreePrismTool::_OnRestartTool()
{
	InstallNewInstance();
}
