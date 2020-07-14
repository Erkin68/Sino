#ifndef _FILE_MANAGER_H_
#define _FILE_MANAGER_H_


#include "windows.h"

class Conf;
class Panel;


BOOL IsDirectory(char*,WIN32_FIND_DATA*,BOOL);

class FileManager
{
	friend class Panel;
	friend class PanelItem;
	friend class MyWMI;
	friend VOID  WaitForChangeDirNotify(FileManager*);
public:
	static 	VOID SetNillPaths(Conf*);

			FileManager();
		   ~FileManager();

	BOOL	Fill();
	BOOL	FillDirectFolder();
	BOOL	FillGuidFolder();
	BOOL	FolderIn(int);
	BOOL	FolderUp(int);
	VOID	SetParent(Panel*);
	VOID	SetWaitForPathChangeNotify();

protected:
	BOOL	IsCrntOrPrntDirAttrb(WIN32_FIND_DATA*);
	HANDLE	hFstChngNot,waitForChngeNotThr;
	char    pathForWaitThr[MAX_PATH];
	Panel	*p;
};

//har 1 dirni ochsa 2 ta, [..] va [.] pustoy blank bor,shuni olish uchun:
inline BOOL FileManager::IsCrntOrPrntDirAttrb(WIN32_FIND_DATA* fd)
{//see Naming Conventions in MSDN:
	if('.'==fd->cFileName[0])
	{	if('\0'==fd->cFileName[1])//"."
			return FALSE;
		if('.'==fd->cFileName[1])
			if('\0'==fd->cFileName[2])//".."
				return FALSE;
	}
	return TRUE;//Hozircha;
}

#endif