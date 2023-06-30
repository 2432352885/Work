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

class DrawThreePrismTool :public DgnPrimitiveTool
{
	bvector<DPoint3d> m_ptVcr;
public:
	static void InstallNewInstance();

	BentleyStatus CreateThreePrism(EditElementHandleR eehThreePrism,DPoint3dR ptStart,DPoint3dR ptEnd,DPoint3dR ptDirection);

	DrawThreePrismTool();

protected:
	virtual bool _OnInstall()override;

	virtual void _OnPostInstall()override;

	virtual bool _OnDataButton(DgnButtonEventCR ev)override;

	virtual bool _OnResetButton(DgnButtonEventCR ev)override;

	virtual void _OnDynamicFrame(DgnButtonEventCR ev) override;

	virtual void _OnUndoPreviousStep()override;

	virtual void _OnRestartTool()override;

};