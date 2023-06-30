#include "DrawLineStringTool.h"

void DrawLineStringTool::InstallNewInstance()
{
	DrawLineStringTool* newTool = new DrawLineStringTool();
	newTool->InstallTool();
}

BentleyStatus DrawLineStringTool::CreateLineString(EditElementHandleR eehLineStr, bvector<DPoint3d> const &points)
{
	ICurvePrimitivePtr cvePri = ICurvePrimitive::CreateLineString(points);
	return DraftingElementSchema::ToElement(eehLineStr, *cvePri, NULL, true, *ACTIVEMODEL);
}

DrawLineStringTool::DrawLineStringTool():DgnPrimitiveTool(0,0)
{

}

void DrawLineStringTool::_OnPostInstall()
{
	EnableUndoPreviousStep();
}

bool DrawLineStringTool::_OnDataButton(DgnButtonEventCR ev)
{
	m_ptVcr.push_back(*ev.GetPoint());
	if (1 == m_ptVcr.size())
	{
		_BeginDynamics();
	}
	return false;
}

bool DrawLineStringTool::_OnResetButton(DgnButtonEventCR ev)
{
	if (m_ptVcr.size() > 1)
	{
		EditElementHandle eehLineStr;
		if (SUCCESS == CreateLineString(eehLineStr, m_ptVcr))
		{
			eehLineStr.AddToModel();
		}
	}
	_OnReinitialize();
	return false;
}

void DrawLineStringTool::_OnDynamicFrame(DgnButtonEventCR ev)
{
	m_ptVcr.push_back(*ev.GetPoint());
	EditElementHandle eehLineStr;
	if (SUCCESS == CreateLineString(eehLineStr, m_ptVcr))
	{
		RedrawElems     redrawElems;
		redrawElems.SetDynamicsViews(IViewManager::GetActiveViewSet(), ev.GetViewport());
		redrawElems.SetDrawMode(DRAW_MODE_TempDraw);
		redrawElems.SetDrawPurpose(DrawPurpose::Dynamics);
		redrawElems.DoRedraw(eehLineStr);
	}
	m_ptVcr.pop_back();
}

void DrawLineStringTool::_OnUndoPreviousStep()
{
	if (0 == m_ptVcr.size())
		return;
	m_ptVcr.pop_back();
	if (0 == m_ptVcr.size())
	{
		_EndDynamics();
	}
}

void DrawLineStringTool::_OnRestartTool()
{
	InstallNewInstance();
}
