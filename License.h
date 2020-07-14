#include "windows.h"
#include <time.h>



namespace License
{

typedef enum TType
{	beta,
	omitLicFile,
	emptyLicFile,
	uncorrLicFile,
	actived
} Type;

typedef struct THrdwrInfo
{	wchar_t CPUDesc[64];
	wchar_t CPUName[64];
	wchar_t CPUManufacturer[64];
	wchar_t CPUId[64];
    unsigned __int16 CPURevision;
	wchar_t CPUSysName[64];
	wchar_t BBManufacturer[64];
	wchar_t BBProduct[64];
	wchar_t BBSerNum[64];
	wchar_t BBVersion[64];
	wchar_t IStrMD5[40];
} HrdwrInfo;

typedef struct TLicFile
{	char type;
	char serNumMD5[40];
	char key[40];
	time_t actTime;
	wchar_t actName[64];
} LicFile;
#define LICFILE_STRICTSIZE 224

extern LRESULT CALLBACK keyDlgProc(HWND,UINT,WPARAM,LPARAM);
extern BOOL ShowBetaInfo();
extern BOOL OnEntWinMain(HINSTANCE);

};