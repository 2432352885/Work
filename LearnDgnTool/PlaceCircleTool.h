#pragma  once
#include <vector>
#include <Mstn\MdlApi\MdlApi.h>
#include <DgnView\DgnTool.h>
#include <DgnView\DgnElementSetTool.h>
#include <Mstn\ISessionMgr.h>
#include <Mstn\MdlApi\mselems.h>
#include <DgnPlatform\DgnPlatformApi.h>
#include <DgnPlatform/DimensionHandler.h>


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

	BentleyStatus CreateTriangle(EditElementHandleR eehTriangle, DPoint3d ptTiaCenter);
	
	PlaceTriangleTool();


private:
	virtual void _OnPostInstall() override;

	virtual bool _OnDataButton(DgnButtonEventCR ev) override;

	virtual bool _OnResetButton(DgnButtonEventCR ev) override;

	virtual void _OnDynamicFrame(DgnButtonEventCR ev) override;

	virtual void _OnRestartTool() override;


};
class DimensionProperty : public IDimCreateData
{
private:
	DimensionStylePtr m_dimstyle;
	DgnTextStylePtr m_textStyle;
	Symbology m_symbology;
	LevelId m_levelId;
	DirectionFormatterPtr m_directionFormatter;
	RotMatrix m_rot1;
	RotMatrix m_rot2;

public:
	DimensionProperty(DimensionStylePtr dimstyle, DgnTextStylePtr textstyle, Symbology symb, LevelId levelId, DirectionFormatterPtr formatter, RotMatrix  rot1, RotMatrix rot2)
	{
		m_dimstyle = dimstyle;
		m_textStyle = textstyle;
		m_symbology = symb;
		m_levelId = levelId;
		m_directionFormatter = formatter;
		m_rot1 = rot1;
		m_rot2 = rot2;
	}

	virtual DimensionStyleCR _GetDimStyle() const
	{
		return *m_dimstyle;
	}

	virtual DgnTextStyleCR   _GetTextStyle()  const
	{
		return *m_textStyle;
	}

	virtual Symbology   _GetSymbology() const
	{
		return m_symbology;
	}
	virtual LevelId   _GetLevelID() const
	{
		return m_levelId;
	}
	virtual DirectionFormatterCR   _GetDirFormat()  const
	{
		return *m_dirFormatter;
	}
	virtual int  _GetViewNumber()  const
	{
		return 0;
	}

	virtual RotMatrixCR  _GetDimRMatrix()  const
	{
		return m_rot1;
	}

	virtual RotMatrixCR  _GetViewRMatrix()  const
	{
		return m_rot2;
	}

	virtual bool   _GetAnnScaleAllowed()  const
	{
		return true;
	}
};
