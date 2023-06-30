#pragma  once
#include <vector>
#include <Mstn\MdlApi\MdlApi.h>
#include <DgnView\DgnTool.h>
#include <DgnView\DgnElementSetTool.h>
#include <Mstn\ISessionMgr.h>
#include <Mstn\MdlApi\mselems.h>
#include <DgnPlatform\DgnPlatformApi.h>

USING_NAMESPACE_BENTLEY_DGNPLATFORM;
using namespace std;

class DrawLineStringTool :public DgnPrimitiveTool
{
	bvector<DPoint3d> m_ptVcr;
public:
	static void InstallNewInstance();

	BentleyStatus CreateLineString(EditElementHandleR eehLineStr, bvector<DPoint3d> const &points);

	DrawLineStringTool();

protected:
	virtual void _OnPostInstall()override;

	virtual bool _OnDataButton(DgnButtonEventCR ev)override;

	virtual bool _OnResetButton(DgnButtonEventCR ev)override;

	virtual void _OnDynamicFrame(DgnButtonEventCR ev) override;

	virtual void _OnUndoPreviousStep()override;

	virtual void _OnRestartTool()override;

};