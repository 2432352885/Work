#include <Bentley\Bentley.r.h>

#define STRINGLISTID_Commands         1
#define STRINGLISTID_Prompts          2

#define CMDNAME_PlaceBsSurfaceTool    1

#define PROMPT_PlaceBsSurfaceTool     1

#define ItemList_BsplineSurface       1

#define TEXTID_BaseArcRadius          1

#define COMBOBOXID_MyLevel            1

#define HOOKITEMID_MyLevelCombo       1

typedef struct helloworldinfo
{
	double     baseArcRadius;
	Utf16Char  levelName[512];
}HelloWorldInfo;

