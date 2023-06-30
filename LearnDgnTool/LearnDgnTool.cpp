#include <time.h>
#include <stdarg.h>
#include <Mstn\MdlApi\MdlApi.h>
#include <Mstn\ISessionMgr.h>
#include <Mstn\MdlApi\mselems.h>
#include "LearnDgnToolcmd.h"  
#include "PlaceCircleTool.h"

USING_NAMESPACE_BENTLEY_DGNPLATFORM;
USING_NAMESPACE_BENTLEY_MSTNPLATFORM;
USING_NAMESPACE_BENTLEY_MSTNPLATFORM_ELEMENT;

void PlaceCircle(char *unparsed)
{
	PlaceCircleTool::InstallNewInstance();
}

void PlaceTriangle(char *unparsed)
{
	PlaceTriangleTool::InstallNewInstance();
}

extern "C" DLLEXPORT  void MdlMain(int argc, WCharCP argv[])
{
    RscFileHandle   rscFileH;  
    if (mdlResource_openFile (&rscFileH, NULL, RSC_READ) != SUCCESS)
    {
        mdlSystem_exit( ERROR, 1);
	}
    MdlCommandNumber  commandNumbers [] ={
        {(CmdHandler)PlaceCircle,     CMD_LEARNDGNTOOL_PLACECIRCLE},
        {(CmdHandler)PlaceTriangle, CMD_LEARNDGNTOOL_PLACETRIANGLE},
        0};
    mdlSystem_registerCommandNumbers (commandNumbers);
    if (mdlParse_loadCommandTable (NULL) == NULL)
    {
        mdlSystem_exit( ERROR,1); 
	}
	return;
}

