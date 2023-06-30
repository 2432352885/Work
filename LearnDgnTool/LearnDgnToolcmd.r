#pragma suppressREQCmds
#include <mstn\MdlApi\rscdefs.r.h>
#include <mstn\MdlApi\cmdclass.r.h>

#define  DLLAPP_MYAPP        1

DllMdlApp DLLAPP_MYAPP =
    {
    L"LearnDgnTool", L"LearnDgnTool"          // taskid, dllName
    }

#define CT_NONE     0
#define CT_MAIN     1
#define CT_SECOND     2
#define CT_TEST    3

CommandTable   CT_MAIN =
{
    { 1,  CT_SECOND,     PLACEMENT,      REQ,            "LearnDgnTool" },
};

CommandTable   CT_SECOND =
{
    { 1,  CT_NONE,      INHERIT,        NONE,           "PlaceCircle" },
    { 2,  CT_NONE,      INHERIT,        NONE,           "PlaceTriangle" },
};