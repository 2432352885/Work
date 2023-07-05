#pragma  once
#include <vector>
#include <Mstn\MdlApi\MdlApi.h>
#include <DgnView\DgnTool.h>
#include <DgnView\DgnElementSetTool.h>
#include <Mstn\ISessionMgr.h>
#include <Mstn\MdlApi\mselems.h>
#include <DgnPlatform\DgnPlatformApi.h>
#include <DgnPlatform/TextHandlers.h>
#include <DgnPlatform/DimensionHandler.h>
#include <Mstn\MdlApi\dlogids.r.h>



USING_NAMESPACE_BENTLEY_DGNPLATFORM;
using namespace std;

class PlaceCircleTool :public DgnPrimitiveTool
{
public:
	static void InstallNewInstance();

	BentleyStatus CreateCircle(EditElementHandleR eehCirc, DPoint3d const &ptCircCenter);

	PlaceCircleTool();

protected:
	virtual void _OnPostInstall()override;

	virtual bool _OnDataButton(DgnButtonEventCR ev)override;

	virtual bool _OnResetButton(DgnButtonEventCR ev)override;

	virtual void _OnDynamicFrame(DgnButtonEventCR ev) override;

	virtual void _OnRestartTool()override;

};


class PlaceTriangleTool : public DgnPrimitiveTool
{
public:
	static void InstallNewInstance();

	BentleyStatus CreateTriangle(MSElementDescrP cell, DPoint3d ptTiaCenter);
	
	PlaceTriangleTool();




private:
	virtual void _OnPostInstall() override;

	virtual bool _OnDataButton(DgnButtonEventCR ev) override;

	virtual bool _OnResetButton(DgnButtonEventCR ev) override;

	virtual void _OnDynamicFrame(DgnButtonEventCR ev) override;

	virtual void _OnRestartTool() override;

};
