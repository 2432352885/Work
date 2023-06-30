#include "ChangeColorTool.h"


ChangeColorTool::ChangeColorTool()
{

}

void ChangeColorTool::_OnRestartTool()
{
	InstallNewInstance();
	return;
}

StatusInt   ChangeColorTool::_OnElementModify(EditElementHandleR el)
{
	ElementPropertiesSetterPtr elePropSet = ElementPropertiesSetter::Create();
	elePropSet->SetColor(23);
	elePropSet->Apply(el);
	return SUCCESS;
}

void ChangeColorTool::InstallNewInstance()
{
	ChangeColorTool* newToolP = new ChangeColorTool();
	newToolP->InstallTool();
}