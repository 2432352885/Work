#pragma  once
#include <DgnView\DgnTool.h>
#include <DgnView\DgnElementSetTool.h>
#include <Mstn\MdlApi\MdlApi.h>
#include <Mstn\ISessionMgr.h>
#include <Mstn\MdlApi\mselems.h>

USING_NAMESPACE_BENTLEY_DGNPLATFORM;

class ChangeColorTool :public DgnElementSetTool
{
public:
	ChangeColorTool();

	void static InstallNewInstance();
protected:
	virtual StatusInt   _OnElementModify(EditElementHandleR el)override;

	virtual void _OnRestartTool()override;
};