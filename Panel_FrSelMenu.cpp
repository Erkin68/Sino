#include "Panel.h"
#include "Sino.h"
#include "strsafe.h"
#include <mbstring.h>
#include "shlobj.h"
#include "Operations\MyShell\MyShell.h"



/*BOOL Panel::ClpbrdCpy()
{
//1.Calc requwered clpbrd size:
long clpbrdSz=0;
	if(!totSelects)
	{	if(iHot<0 || iHot>totItems-1) return FALSE;
		clpbrdSz = 2+MyStringLength(GetFullPathAndName(iHot),MAX_PATH);
	}
	else
	{	for(int i=0; i<totSelects; i++)
			clpbrdSz += 2+MyStringLength(GetFullPathAndName(GetSelectedItemNum(i)),MAX_PATH);
	}clpbrdSz+=3;//oxirgi 0 va boshidagi operatsiyasi uchun;
//2.Open clpbrd:
	if(!OpenClipboard(hWnd)) return FALSE;
	EmptyClipboard();
//3.Alloc for clpbrd:
	HGLOBAL hglbCopy=GlobalAlloc(GMEM_MOVEABLE,sizeof(wchar_t)*clpbrdSz);
	wchar_t *pClpbrd=(wchar_t*)GlobalLock(hglbCopy); 
//4.Fill datas for:
	*pClpbrd++=0;//cpy
	if(!totSelects)
	{	if(file==pItems[iHot].attribute)
			*pClpbrd++=1;
		else
			*pClpbrd++=2;
		pClpbrd += 1+GetFullPathAndName(iHot,pClpbrd,clpbrdSz-2);
		if(!_wcsicmp(pItems[iHot].GetExt(),L"htm"))
		{	wchar_t s[MAX_PATH];MyStringCpy(s,pItems[iHot].NameCharLen,pItems[iHot].Name);
			MyStringCpy(&s[pItems[iHot].NameCharLen],MAX_PATH-pItems[iHot].NameCharLen,L"_files");
			int i=FindItem(s,2);
			if(i)
				pClpbrd += 1+GetFullPathAndName(i,pClpbrd,clpbrdSz-2);
	}	}
	else
	{	for(int i=0; i<totSelects; i++)
		{	if(file==pItems[GetSelectedItemNum(i)].attribute)
				*pClpbrd++=1;
			else
				*pClpbrd++=2;
			int l=GetFullPathAndName(GetSelectedItemNum(i),pClpbrd,clpbrdSz-1);
			pClpbrd += l+1;
			if(!_wcsicmp(pItems[GetSelectedItemNum(i)].GetExt(),L"htm"))
			{	wchar_t s[MAX_PATH];MyStringCpy(s,pItems[GetSelectedItemNum(i)].NameCharLen,pItems[GetSelectedItemNum(i)].Name);
				MyStringCpy(&s[pItems[GetSelectedItemNum(i)].NameCharLen],MAX_PATH-pItems[GetSelectedItemNum(i)].NameCharLen,L"_files");
				int i=FindItem(s,2);
				if(i)
					pClpbrd += 1+GetFullPathAndName(i,pClpbrd,clpbrdSz-2);
	}	}	}
	*pClpbrd = 0;//Oxiri;
//5.Release clpbrd:
	GlobalUnlock(hglbCopy);
	SetClipboardData(CF_TEXT,hglbCopy); 
	CloseClipboard();
	//GlobalFree(hglbCopy);
	FreeSelection();
	//EnableMenuItem(GetMenu(::hWnd),IDM_EDIT_CUT,MF_BYCOMMAND|MF_GRAYED);
	//EnableMenuItem(GetMenu(::hWnd),IDM_EDIT_COPY,MF_BYCOMMAND|MF_GRAYED);
	return TRUE;
}*/

BOOL Panel::ClpbrdCpy()
{
//1.Open clpbrd:
	if(!OpenClipboard(hWnd)) return FALSE;
	EmptyClipboard();

//2.Calc requwered clpbrd size:
long clpbrdSz=0;
	if(!totSelects)
		clpbrdSz+=1+MyStringLength(GetFullPathAndName(iHot),MAX_PATH);
	else
	{	for(int i=0; i<totSelects; i++)
			clpbrdSz+=1+MyStringLength(GetFullPathAndName(iHot),MAX_PATH);
	}
	clpbrdSz = clpbrdSz * sizeof(wchar_t) + sizeof(DROPFILES)+1;

//3.Alloc for clpbrd:
	HGLOBAL hglbCopy=GlobalAlloc(GMEM_MOVEABLE,sizeof(wchar_t)*clpbrdSz);
	if(!hglbCopy)return FALSE;
	DROPFILES *pClpbrdDropFiles=(DROPFILES*)GlobalLock(hglbCopy);
	wchar_t *pClpbrd = (wchar_t*)(((char*)pClpbrdDropFiles)+sizeof(DROPFILES));
	pClpbrdDropFiles->pFiles=sizeof(DROPFILES);
	pClpbrdDropFiles->fWide=TRUE;
//4.Fill datas for:
	if(!totSelects)
		pClpbrd += 1+GetFullPathAndName(iHot,pClpbrd,MAX_PATH);
	else
	{	for(int i=0; i<totSelects; i++)
			pClpbrd += 1+GetFullPathAndName(iHot,pClpbrd,MAX_PATH);
	}
	*pClpbrd = 0;//Oxiri;
//5.Release clpbrd:
	GlobalUnlock(hglbCopy);
	SetClipboardData(CF_HDROP,hglbCopy); 
	CloseClipboard();
	FreeSelection();
	return TRUE;
}

/*BOOL Panel::ClpbrdCut()
{
//1.Calc requwered clpbrd size:
long clpbrdSz=0;
	if(!totSelects)
	{	if(iHot<0 || iHot>totItems-1) return FALSE;
		clpbrdSz = 2+MyStringLength(GetFullPathAndName(iHot),MAX_PATH);
	}
	else
	{	for(int i=0; i<totSelects; i++)
			clpbrdSz += 2+MyStringLength(GetFullPathAndName(GetSelectedItemNum(i)),MAX_PATH);
	}clpbrdSz+=3;//oxirgi 0 va boshidagi operatsiyasi uchun;
//2.Open clpbrd:
	if(!OpenClipboard(hWnd)) return FALSE;
	EmptyClipboard();
	
//3.Alloc for clpbrd:
	HGLOBAL hglbCopy=GlobalAlloc(GMEM_MOVEABLE,2*clpbrdSz);
	wchar_t *pClpbrd=(wchar_t*)GlobalLock(hglbCopy); 
//4.Fill datas for:
	*pClpbrd++=1;//cut
	if(!totSelects)
	{	if(file==pItems[iHot].attribute)
			*pClpbrd++=1;
		else
			*pClpbrd++=2;
		pClpbrd += 1+GetFullPathAndName(iHot,pClpbrd,clpbrdSz-2);
	}
	else
	{	for(int i=0; i<totSelects; i++)
		{	if(file==pItems[GetSelectedItemNum(i)].attribute)
				*pClpbrd++=1;
			else
				*pClpbrd++=2;
			int l=GetFullPathAndName(GetSelectedItemNum(i),pClpbrd,clpbrdSz-1);
			pClpbrd += l+1;
	}	}
	*pClpbrd = 0;//Oxiri;
//5.Release clpbrd:
	GlobalUnlock(hglbCopy);
	SetClipboardData(CF_TEXT,hglbCopy); 
	CloseClipboard();
	//GlobalFree(hglbCopy);
	FreeSelection();
	//EnableMenuItem(GetMenu(::hWnd),IDM_EDIT_CUT,MF_BYCOMMAND|MF_GRAYED);
	//EnableMenuItem(GetMenu(::hWnd),IDM_EDIT_COPY,MF_BYCOMMAND|MF_GRAYED);
	return TRUE;
}*/

BOOL Panel::ClpbrdCut()
{
//1.Calc requwered clpbrd size:
long clpbrdSz =	0;
	if(!totSelects)
	{	if(iHot<0 || iHot>totItems-1) return FALSE;
		clpbrdSz = 1+MyStringLength(GetFullPathAndName(iHot),MAX_PATH);
	}
	else
	{	for(int i=0; i<totSelects; i++)
			clpbrdSz += 1+MyStringLength(GetFullPathAndName(GetSelectedItemNum(i)),MAX_PATH);
	}clpbrdSz = (clpbrdSz+1)*sizeof(wchar_t)+sizeof(DROPFILES);//oxirgi 0 va boshidagi operatsiyasi uchun;
//2.Open clpbrd:
	if(!OpenClipboard(hWnd)) return FALSE;
	EmptyClipboard();
	
//3.Alloc for clpbrd:
	HGLOBAL hglbMove=GlobalAlloc(GMEM_MOVEABLE,clpbrdSz);
	if(!hglbMove)return FALSE;
	DROPFILES *pClpbrdDropFiles=(DROPFILES*)GlobalLock(hglbMove);
	wchar_t *pClpbrd = (wchar_t*)(((char*)pClpbrdDropFiles)+sizeof(DROPFILES));
	pClpbrdDropFiles->pFiles=sizeof(DROPFILES);
	pClpbrdDropFiles->fWide=TRUE;
//4.Fill datas for:
	if(!totSelects)
	{	pClpbrd += 1+GetFullPathAndName(iHot,pClpbrd,clpbrdSz-2);
	}
	else
	{	for(int i=0; i<totSelects; i++)
		{	int l=GetFullPathAndName(GetSelectedItemNum(i),pClpbrd,clpbrdSz-1);
			pClpbrd += l+1;
	}	}
	*pClpbrd = 0;//Oxiri;

//5.Release clpbrd:
	SetClipboardData(CF_HDROP,hglbMove);
	GlobalUnlock(hglbMove);

	UINT uFmtDROPEFFECT_MOVE = RegisterClipboardFormat(CFSTR_PREFERREDDROPEFFECT);
	hglbMove = GlobalAlloc(GMEM_MOVEABLE, sizeof(DWORD));
	pClpbrd = (wchar_t*)GlobalLock(hglbMove);
	*((DWORD*)pClpbrd) = 2; // 2 if cut or 5 if copy
	SetClipboardData(uFmtDROPEFFECT_MOVE, hglbMove);
	GlobalUnlock(hglbMove);

	CloseClipboard();
	FreeSelection();
	return TRUE;
}

static int GetClpbrdFiles(LPVOID *pOut)
{	*pOut = 0;
	HGLOBAL hglb = GetClipboardData(CF_HDROP);
	if(!hglb)return 0;

	//Retrieve number of dropped files
	UINT wNumFiles = DragQueryFile((HDROP)hglb, 0xFFFFFFFF, NULL, 0);
	if(0==wNumFiles)
		return 0;

	//1.Calc requered size:
	int sz=0;
	for(DWORD i=0; i<wNumFiles; i++)
		sz += 1+DragQueryFile((HDROP)hglb, i, NULL, 0);
	sz *= sizeof(wchar_t);
	*pOut = malloc(sz);
	wchar_t* p = (wchar_t*)*pOut;
	for(DWORD i=0; i<wNumFiles; i++)
	{	UINT cb = DragQueryFile((HDROP)hglb, i, p, MAX_PATH);
		p += cb+1;		
	}
	return sz;
}

BOOL Panel::ClpbrdApndCpy()
{
//1.Open clpbrd:
	if(!OpenClipboard(hWnd)) return FALSE;
	LPVOID preglPtr;int szPre=GetClpbrdFiles(&preglPtr);
	EmptyClipboard();
//2.Calc requwered clpbrd size:
long clpbrdSz=0;
	if(!totSelects)
		clpbrdSz+=1+MyStringLength(GetFullPathAndName(iHot),MAX_PATH);
	else
	{	for(int i=0; i<totSelects; i++)
		{	int sl=GetSelectedItemNum(i);
			clpbrdSz+=1+MyStringLength(GetFullPathAndName(sl),MAX_PATH);
	}	}
	clpbrdSz = szPre + clpbrdSz * sizeof(wchar_t) + sizeof(DROPFILES)+1;

//3.Alloc for clpbrd:
	HGLOBAL hglbCopy=GlobalAlloc(GMEM_MOVEABLE,sizeof(wchar_t)*clpbrdSz);
	if(!hglbCopy)
	{	if(preglPtr)free(preglPtr);
		return FALSE;
	}
	DROPFILES *pClpbrdDropFiles=(DROPFILES*)GlobalLock(hglbCopy);
	wchar_t *pClpbrd = (wchar_t*)(((char*)pClpbrdDropFiles)+sizeof(DROPFILES));
	pClpbrdDropFiles->pFiles=sizeof(DROPFILES);
	pClpbrdDropFiles->fWide=TRUE;
//4.Fill datas for:
	if(szPre)
	{	memcpy(pClpbrd,preglPtr,szPre);
		pClpbrd = (wchar_t*)(((char*)pClpbrd) + szPre);
		if(preglPtr)free(preglPtr);
	}
	if(!totSelects)
		pClpbrd += 1+GetFullPathAndName(iHot,pClpbrd,MAX_PATH);
	else
	{	for(int i=0; i<totSelects; i++)
		{	int sl=GetSelectedItemNum(i);
			pClpbrd += 1+GetFullPathAndName(sl,pClpbrd,MAX_PATH);
	}	}
	*pClpbrd = 0;//Oxiri;
//5.Release clpbrd:
	GlobalUnlock(hglbCopy);
	SetClipboardData(CF_HDROP,hglbCopy); 
	CloseClipboard();
	FreeSelection();
	return TRUE;
}

BOOL Panel::ClpbrdApndCut()
{
//1.Open clpbrd:
	if(!OpenClipboard(hWnd)) return FALSE;
	LPVOID preglPtr;int szPre=GetClpbrdFiles(&preglPtr);
	EmptyClipboard();

//2.Calc requwered clpbrd size:
long clpbrdSz=szPre+sizeof(DROPFILES)+1;
	if(!totSelects)
	{	if(iHot<0 || iHot>totItems-1) return FALSE;
		clpbrdSz += 1+MyStringLength(GetFullPathAndName(iHot),MAX_PATH);
	}
	else
	{	for(int i=0; i<totSelects; i++)
			clpbrdSz += 1+MyStringLength(GetFullPathAndName(GetSelectedItemNum(i)),MAX_PATH);
	}
	clpbrdSz+=1;//oxirgi 0 va boshidagi operatsiyasi uchun;
	
//3.Alloc for clpbrd:
	HGLOBAL hglbMove=GlobalAlloc(GMEM_MOVEABLE,sizeof(wchar_t)*clpbrdSz);
	DROPFILES *pClpbrdDropFiles=(DROPFILES*)GlobalLock(hglbMove);
	wchar_t *pClpbrd = (wchar_t*)(((char*)pClpbrdDropFiles)+sizeof(DROPFILES));
	pClpbrdDropFiles->pFiles=sizeof(DROPFILES);
	pClpbrdDropFiles->fWide=TRUE;
//4.Fill datas for:
	if(szPre)
	{	memcpy(pClpbrd,preglPtr,szPre);
		pClpbrd = (wchar_t*)(((char*)pClpbrd) + szPre);
		if(preglPtr)free(preglPtr);
	}
	if(!totSelects)
	{	pClpbrd += 1+GetFullPathAndName(iHot,pClpbrd,clpbrdSz-2);
	}
	else
	{	for(int i=0; i<totSelects; i++)
		{	int l=GetFullPathAndName(GetSelectedItemNum(i),pClpbrd,clpbrdSz-1);
			pClpbrd += l+1;
	}	}
	*pClpbrd = 0;//Oxiri;

//5.Release clpbrd:
	SetClipboardData(CF_HDROP,hglbMove);
	GlobalUnlock(hglbMove);

	UINT uFmtDROPEFFECT_MOVE = RegisterClipboardFormat(CFSTR_PREFERREDDROPEFFECT);
	hglbMove = GlobalAlloc(GMEM_MOVEABLE, sizeof(DWORD));
	pClpbrd = (wchar_t*)GlobalLock(hglbMove);
	*((DWORD*)pClpbrd) = 2; // 2 if cut or 5 if copy
	SetClipboardData(uFmtDROPEFFECT_MOVE, hglbMove);
	GlobalUnlock(hglbMove);

	CloseClipboard();
	FreeSelection();
	return TRUE;
}

DWORD CALLBACK SimpleCopyProgressRoutine( LARGE_INTEGER TotalFileSize,          // file size
										  LARGE_INTEGER TotalBytesTransferred,  // bytes transferred
										  LARGE_INTEGER StreamSize,             // bytes in stream
										  LARGE_INTEGER StreamBytesTransferred, // bytes transferred for stream
										  DWORD dwStreamNumber,                 // current stream
										  DWORD dwCallbackReason,               // callback reason
										  HANDLE hSourceFile,                   // handle to source file
										  HANDLE hDestinationFile,              // handle to destination file
										  LPVOID lpData)                        // from CopyFileEx
{
	double lw=0.0;if(TotalFileSize.LowPart)lw=(double)TotalBytesTransferred.LowPart/(double)TotalFileSize.LowPart;
	double hg=0.0;if(TotalFileSize.HighPart)hg=(double)TotalBytesTransferred.HighPart/(double)TotalFileSize.HighPart;
	double l=(lw+hg*32)*100.0f;
	SendMessage(GetDlgItem((HWND)lpData,IDC_PROGRESS_COPY_TOTAL),PBM_SETPOS,(int)l,0);
wchar_t s[32];StringCchPrintf(s,32,L"%d",TotalBytesTransferred);
	SetDlgItemText((HWND)lpData,IDC_STATIC6,s);

	VARIANT varFS;
	VariantInit(&varFS);
	varFS.llVal = TotalFileSize.QuadPart;

	VARIANT varFST;
	VariantInit(&varFST);
	varFST.llVal = TotalBytesTransferred.QuadPart;

	VARIANT varF;
	VarSub(&varFS,&varFST,&varF);

	HRESULT hres = VariantChangeType(&varF, &varF, 0, VT_BSTR);
	if(hres) throw hres;

	SetDlgItemText((HWND)lpData,IDC_STATIC8,varF.bstrVal);
	return PROGRESS_CONTINUE;
}

BOOL bClpbrdCpyPeplExstAsk=TRUE,bClpbrdCpyPeplExst=TRUE;
BOOL MoveDirFrClpbrdDatas(wchar_t *path,BOOL ClpbrdCut,wchar_t* dest,int destCharLen,HWND prgrsDlg)
{BOOL r=TRUE,bCancel=FALSE;WIN32_FIND_DATA ff;HANDLE hf = INVALID_HANDLE_VALUE;wchar_t s[MAX_PATH];

	if('*'==dest[destCharLen])dest[destCharLen]=0;
	wchar_t *p=wcsrchr(path,'\\');if(!p)p=path;else ++p;
	destCharLen+=MyStringCpy(&dest[destCharLen],MAX_PATH-destCharLen-1,p);
	if(!IsDirExist(dest))
		CreateDirectory(dest,NULL);
	dest[destCharLen++]='\\';dest[destCharLen]=0;

	int l=MyStringCpy(s,MAX_PATH-1,path);
	s[l++]='\\';s[l++]='*';s[l]=0;
			
	hf = MyFindFirstFileEx(s,FindExInfoStandard,&ff,FindExSearchLimitToDirectories,NULL,0);
	s[--l]=0;
	do
	{	if(INVALID_HANDLE_VALUE==hf) return FALSE;
		if(FILE_ATTRIBUTE_DIRECTORY & ff.dwFileAttributes)//if(IsDirectory(path, &ff, FALSE))
		{	if(IsCrntOrPrntDirAttrb(ff.cFileName))
			{	MyStringCpy(&s[l],MAX_PATH-1,ff.cFileName);
				if(!MoveDirFrClpbrdDatas(s,ClpbrdCut,dest,destCharLen,prgrsDlg))
					r=FALSE;	
		}	}
		else
		{	MyStringCpy(&s[l],MAX_PATH-1,ff.cFileName);
			MyStringCpy(&dest[destCharLen],MAX_PATH-destCharLen,ff.cFileName);
			SetDlgItemText(prgrsDlg,IDC_STATIC,s);
			SetDlgItemText(prgrsDlg,IDC_STATIC5,dest);
			if(IsFileExist(dest))
			{	if(bClpbrdCpyPeplExstAsk)
				{	if(IDYES==MessageBox(prgrsDlg,L"This file already exist.",L"Replase existing file and do this for all next files?",MB_YESNO))
					{	bClpbrdCpyPeplExst = TRUE;
						if(IDYES==MessageBox(prgrsDlg,L"In future replace existing files",L"for all next files?",MB_YESNO))
							bClpbrdCpyPeplExstAsk=FALSE;
					} else bClpbrdCpyPeplExst = FALSE;
				}
				if(bClpbrdCpyPeplExst)
				{	if(ClpbrdCut)
					{	if(!MoveFileWithProgress(s,dest,SimpleCopyProgressRoutine,prgrsDlg,MOVEFILE_REPLACE_EXISTING|MOVEFILE_COPY_ALLOWED))
							r=FALSE;
					}
					else
					{	if(!CopyFileEx(s,dest,SimpleCopyProgressRoutine,prgrsDlg,&bCancel,COPY_FILE_RESTARTABLE))
							r=FALSE;
			}	}	}
			else
			{	if(ClpbrdCut)
				{	if(!MoveFileWithProgress(s,dest,SimpleCopyProgressRoutine,prgrsDlg,MOVEFILE_REPLACE_EXISTING|MOVEFILE_COPY_ALLOWED))
						r=FALSE;
				}
				else
				{	if(!CopyFileEx(s,dest,SimpleCopyProgressRoutine,prgrsDlg,&bCancel,COPY_FILE_RESTARTABLE))
						r=FALSE;
			}	}
			//if(ClpbrdCut)DeleteFile(s);
	}	}
	while(FindNextFile(hf, &ff));
	if(ClpbrdCut)RemoveDirectory(path);
	FindClose(hf);
	return TRUE;
}

INT_PTR SimpleDlgProc(HWND hwndDlg,UINT uMsg,WPARAM wParam,LPARAM lParam){return FALSE;}
/*BOOL Panel::ClpbrdPst()
{
BOOL r;
	if(!OpenClipboard(hWnd)) return FALSE;
	HGLOBAL hglb = GetClipboardData(CF_TEXT);
	if(hglb!=NULL)
    {	wchar_t *p = (wchar_t*)GlobalLock(hglb); 
        if(p!=NULL)
		{	HWND dlg = CreateDialog(hInst,MAKEINTRESOURCE(IDD_DIALOG_COPY_QUEUE),
									hWnd,(DLGPROC)SimpleDlgProc);
			if(dlg)ShowWindow(dlg,SW_SHOW);
			wchar_t pdest[MAX_PATH];int pdestLn=MyStringCpy(pdest,MAX_PATH,path);
			BOOL ClpbrdCut=(1==(*p)?TRUE:FALSE);//cut
			++p;
			do
			{	if(2==(*p++)) MoveDirFrClpbrdDatas(p,ClpbrdCut,pdest,pdestLn-1,dlg);
				else
				{	BOOL bCancel=FALSE;							
					wchar_t *psl=wcsrchr(p,'\\');if(!psl)psl=p;else ++psl;
					MyStringCpy(&pdest[pdestLn-1],MAX_PATH-pdestLn-1,psl);
					if(IsFileExist(pdest))
					{	if(bClpbrdCpyPeplExstAsk)
						{	if(IDYES==MessageBox(dlg,L"This file already exist.",L"Replase existing file and do this for all next files?",MB_YESNO))
							{	bClpbrdCpyPeplExst = TRUE;
								if(IDYES==MessageBox(dlg,L"In future replace existing files",L"for all next files?",MB_YESNO))
									bClpbrdCpyPeplExstAsk=FALSE;
							} else bClpbrdCpyPeplExst = FALSE;
						}
						if(bClpbrdCpyPeplExst)
						{	if(ClpbrdCut)
								r=MoveFileWithProgress(p,pdest,SimpleCopyProgressRoutine,dlg,MOVEFILE_REPLACE_EXISTING|MOVEFILE_COPY_ALLOWED);
							else
								r=CopyFileEx(p,pdest,SimpleCopyProgressRoutine,dlg,&bCancel,COPY_FILE_RESTARTABLE);
					}	}
					else
					{	if(ClpbrdCut)
							r=MoveFileWithProgress(p,pdest,SimpleCopyProgressRoutine,dlg,MOVEFILE_REPLACE_EXISTING|MOVEFILE_COPY_ALLOWED);
						else
							r=CopyFileEx(p,pdest,SimpleCopyProgressRoutine,dlg,&bCancel,COPY_FILE_RESTARTABLE);
						if(!r)r=GetLastError();
						//if(ClpbrdCut)DeleteFile(p);
				}	}
				p+=1+MyStringLength(p,MAX_PATH);
			} while(*p);
            GlobalUnlock(hglb);
			if(dlg)DestroyWindow(dlg);
	}	}
	CloseClipboard();
	//EnableMenuItem(GetMenu(::hWnd),IDM_EDIT_CUT,MF_BYCOMMAND|MF_ENABLED);
	//EnableMenuItem(GetMenu(::hWnd),IDM_EDIT_COPY,MF_BYCOMMAND|MF_ENABLED);
	return TRUE;
}*/

BOOL Panel::ClpbrdPst()
{BOOL r=FALSE,bPaste;// = GetOleClipboardPasteState();
	wchar_t s[MAX_PATH];HWND dlg=0;

	if(!OpenClipboard(hWnd)) return FALSE;

	UINT uFmtDROPEFFECT_MOVE = RegisterClipboardFormat(CFSTR_PREFERREDDROPEFFECT);
	HGLOBAL hglb = GetClipboardData(uFmtDROPEFFECT_MOVE);
	if(hglb)
	{DWORD *pdw = (DWORD*)GlobalLock(hglb);
	 if(2==(*pdw))bPaste=TRUE;// 2 if cut or 5 if copy
	 else bPaste=FALSE;//if(5==(*pdw))
	 GlobalUnlock(hglb);
	}

	hglb = GetClipboardData(CF_HDROP);
	if(!hglb)goto End;

	//Retrieve number of dropped files
	UINT wNumFiles = DragQueryFile((HDROP)hglb, 0xFFFFFFFF, NULL, 0);
	if(0==wNumFiles)
		goto End;

	dlg = CreateDialog(	hInst,MAKEINTRESOURCE(IDD_DIALOG_COPY_QUEUE),
						hWnd,(DLGPROC)SimpleDlgProc);
	if(!dlg)goto End1;
	ShowWindow(dlg,SW_SHOW);
	wchar_t pdest[MAX_PATH];int pdestLn=MyStringCpy(pdest,MAX_PATH,path);
	
	// Simple case of one dropped file; more than one can be dropped
	for(DWORD i=0; i<wNumFiles; i++)
	{	UINT cb = DragQueryFile((HDROP)hglb, i, s, MAX_PATH);
		// If no characters copied, there were no files
		if(0==cb)
			continue;
		wchar_t *psl=wcsrchr(s,'\\');if(!psl)psl=&s[0];else ++psl;
		if(IsDirExist(s))
			r=MoveDirFrClpbrdDatas(s,bPaste,pdest,pdestLn-1,dlg);//1-kirishdayoq oxirgi \* ni \ ga aylantiradi;
		else
		{	MyStringCpy(&pdest[pdestLn-1],MAX_PATH-pdestLn-1,psl);//-1 \* uchun;
			if(bPaste)
				r=MoveFileWithProgress(s,pdest,SimpleCopyProgressRoutine,dlg,MOVEFILE_REPLACE_EXISTING|MOVEFILE_COPY_ALLOWED);
			else
			{	BOOL bCancel=FALSE;
				r=CopyFileEx(s,pdest,SimpleCopyProgressRoutine,dlg,&bCancel,COPY_FILE_RESTARTABLE);
	}	}	}
End1:
    DragFinish((HDROP)hglb);
End:
	if(dlg)DestroyWindow(dlg);
	CloseClipboard();
	return r;
}

static wchar_t *pnlSaveSelectn=0;
VOID Panel::Select(int cmdType,int cmd)
{
int id,i;
	switch(cmdType)
	{	case 0://select
			switch(cmd)
			{	case 0://all
					FreeSelection();
					for(i=1; i<totItems; i++)
						AddToSelection(i);
					break;
				case 1://all folders
					FreeSelection();
					for(i=1; i<totItems; i++)
					{	if(file!=pItems[i].attribute)
							AddToSelection(i);
					}
					break;
				case 2://all files
					FreeSelection();
					for(i=1; i<totItems; i++)
					{	if(file==pItems[i].attribute)
							AddToSelection(i);
					}
					break;
				case 3://all from selected name
					if(iHot<0 || iHot>totItems-1) break;
					id=iHot;
					FreeSelection();
					for(i=1; i<totItems; i++)
					{	if(id==i)AddToSelection(i);//continue;
						else if(!_mbsnbicmp((BYTE*)pItems[id].Name,(BYTE*)pItems[i].Name,pItems[id].NameCharLen))
							AddToSelection(i);
					}
					break;
				case 4://all folders from selected name
					if(iHot<0 || iHot>totItems-1) break;
					id=iHot;
					FreeSelection();
					for(i=1; i<totItems; i++)
					{	if(id==i)AddToSelection(i);//continue;
						else if(file==pItems[i].attribute)continue;
						else if(!_mbsnbicmp((BYTE*)pItems[id].Name,(BYTE*)pItems[i].Name,pItems[id].NameCharLen))
							AddToSelection(i);
					}
					break;
				case 5://all files from selected name
					if(iHot<0 || iHot>totItems-1) break;
					id=iHot;
					FreeSelection();
					for(i=1; i<totItems; i++)
					{	if(id==i)AddToSelection(i);//continue;
						else if(file!=pItems[i].attribute)continue;
						else if(!_mbsnbicmp((BYTE*)pItems[id].Name,(BYTE*)pItems[i].Name,pItems[id].NameCharLen))
							AddToSelection(i);
					}
					break;
				case 6://all from selected extension
					if(iHot<0 || iHot>totItems-1) break;
					if(!pItems[iHot].ExtCharLen) break;
					id=iHot;
					FreeSelection();
					for(i=1; i<totItems; i++)
					{	if(id==i)AddToSelection(i);//continue;
						else if(!pItems[i].ExtCharLen) continue;
						else if(!_mbsnbicmp((BYTE*)pItems[id].GetExt(),(BYTE*)pItems[i].GetExt(),pItems[id].ExtCharLen))
							AddToSelection(i);
					}
					break;
				case 7://all folders from selected extension
					if(iHot<0 || iHot>totItems-1) break;
					if(!pItems[iHot].ExtCharLen) break;
					id=iHot;
					FreeSelection();
					for(i=1; i<totItems; i++)
					{	if(id==i)AddToSelection(i);//continue;
						else if(file==pItems[i].attribute)continue;
						else if(!pItems[i].ExtCharLen) continue;
						else if(!_mbsnbicmp((BYTE*)pItems[id].GetExt(),(BYTE*)pItems[i].GetExt(),pItems[id].ExtCharLen))
							AddToSelection(i);
					}
					break;
				case 8://all files from selected extension
					if(iHot<0 || iHot>totItems-1) break;
					if(!pItems[iHot].ExtCharLen) break;
					id=iHot;
					FreeSelection();
					for(i=1; i<totItems; i++)
					{	if(id==i)AddToSelection(i);//continue;
						else if(file!=pItems[i].attribute)continue;
						else if(!pItems[i].ExtCharLen) continue;
						else if(!_mbsnbicmp((BYTE*)pItems[id].GetExt(),(BYTE*)pItems[i].GetExt(),pItems[id].ExtCharLen))
							AddToSelection(i);
					}
					break;
			}	
			break;
		case 1://unselect
			int delSels,delSelsFiles,delSelsFolders;
			switch(cmd)
			{	case 0://all
					FreeSelection();
					break;
				case 1://all folders
					delSels=0;
					for(i=0; i<totSelects; i++)
					{	if(file!=pItems[GetSelectedItemNum(i)].attribute)
						{	pItems[GetSelectedItemNum(i)].state=normal;
							++delSels;
					}	}
					totSelects -= delSels;
					totSelectsFolders -= delSels;
					break;
				case 2://all files
					delSels=0;
					for(i=0; i<totSelects; i++)
					{	if(file==pItems[GetSelectedItemNum(i)].attribute)
						{	pItems[GetSelectedItemNum(i)].state=normal;
							++delSels;
					}	}
					totSelects -= delSels;
					totSelectsFiles -= delSels;
					break;
				case 3://all from selected name
					delSelsFolders=0;
					delSelsFiles=0;
					if(iHot<0 || iHot>totItems-1) break;
					id=iHot;
					for(i=0; i<totSelects; i++)
					{	if(id==GetSelectedItemNum(i) || (!_mbsnbicmp((BYTE*)pItems[id].Name,
																	 (BYTE*)pItems[GetSelectedItemNum(i)].Name,
																	 pItems[id].NameCharLen)))
						{	pItems[GetSelectedItemNum(i)].state=normal;
							if(file==pItems[GetSelectedItemNum(i)].attribute) ++delSelsFiles;
							else ++delSelsFolders;
					}	}
					totSelects -= delSelsFolders+delSelsFiles;
					totSelectsFolders -= delSelsFolders;
					totSelectsFiles -= delSelsFiles;
					break;
				case 4://all folders from selected name
					delSelsFolders=0;
					delSelsFiles=0;
					if(iHot<0 || iHot>totItems-1) break;
					id=iHot;
					for(i=0; i<totSelects; i++)
					{	if(id==GetSelectedItemNum(i) || (!_mbsnbicmp((BYTE*)pItems[id].Name,
																	 (BYTE*)pItems[GetSelectedItemNum(i)].Name,
																	  pItems[id].NameCharLen)))
						{	if(file!=pItems[GetSelectedItemNum(i)].attribute)
							{	pItems[GetSelectedItemNum(i)].state=normal;
								if(file!=pItems[GetSelectedItemNum(i)].attribute) ++delSelsFiles;
								else ++delSelsFolders;
					}	}	}
					totSelects -= delSelsFolders+delSelsFiles;
					totSelectsFolders -= delSelsFolders;
					totSelectsFiles -= delSelsFiles;
					break;
				case 5://all files from selected name
					delSelsFolders=0;
					delSelsFiles=0;
					if(iHot<0 || iHot>totItems-1) break;
					id=iHot;
					for(i=0; i<totSelects; i++)
					{	if(id==GetSelectedItemNum(i) || (!_mbsnbicmp((BYTE*)pItems[id].Name,
																	 (BYTE*)pItems[GetSelectedItemNum(i)].Name,
																	 pItems[id].NameCharLen)))
						{	if(file!=pItems[GetSelectedItemNum(i)].attribute)
							{	pItems[GetSelectedItemNum(i)].state=normal;
								if(file==pItems[GetSelectedItemNum(i)].attribute) ++delSelsFiles;
								else ++delSelsFolders;
					}	}	}
					totSelects -= delSelsFolders+delSelsFiles;
					totSelectsFolders -= delSelsFolders;
					totSelectsFiles -= delSelsFiles;
					break;
				case 6://all from selected extension
					if(!pItems[iHot].ExtCharLen) break;
					delSelsFolders=0;
					delSelsFiles=0;
					if(iHot<0 || iHot>totItems-1) break;
					id=iHot;
					for(i=0; i<totSelects; i++)
					{	if(!pItems[GetSelectedItemNum(i)].ExtCharLen) break;
						if(id==GetSelectedItemNum(i) || (!_mbsnbicmp((BYTE*)pItems[id].GetExt(),
																	 (BYTE*)pItems[GetSelectedItemNum(i)].GetExt(),
																	 pItems[id].ExtCharLen)))
						{	pItems[GetSelectedItemNum(i)].state=normal;
							if(file==pItems[GetSelectedItemNum(i)].attribute) ++delSelsFiles;
							else ++delSelsFolders;
					}	}
					totSelects -= delSelsFolders+delSelsFiles;
					totSelectsFolders -= delSelsFolders;
					totSelectsFiles -= delSelsFiles;
					break;
				case 7://all folders from selected extension
					if(!pItems[iHot].ExtCharLen) break;
					delSelsFolders=0;
					delSelsFiles=0;
					if(iHot<0 || iHot>totItems-1) break;
					id=iHot;
					for(i=0; i<totSelects; i++)
					{	if(!pItems[GetSelectedItemNum(i)].ExtCharLen) break;
						if(id==GetSelectedItemNum(i) || (!_mbsnbicmp((BYTE*)pItems[id].GetExt(),
																	 (BYTE*)pItems[GetSelectedItemNum(i)].GetExt(),
																	 pItems[id].ExtCharLen)))
						{	if(file!=pItems[GetSelectedItemNum(i)].attribute)
							{	pItems[GetSelectedItemNum(i)].state=normal;
								if(file==pItems[GetSelectedItemNum(i)].attribute) ++delSelsFiles;
								else ++delSelsFolders;
					}	}	}
					totSelects -= delSelsFolders+delSelsFiles;
					totSelectsFolders -= delSelsFolders;
					totSelectsFiles -= delSelsFiles;
					break;
				case 8://all files from selected extension
					if(!pItems[iHot].ExtCharLen) break;
					delSelsFolders=0;
					delSelsFiles=0;
					if(iHot<0 || iHot>totItems-1) break;
					id=iHot;
					for(int i=0; i<totSelects; i++)
					{	if(!pItems[GetSelectedItemNum(i)].ExtCharLen) break;
						if(id==GetSelectedItemNum(i) || (!_mbsnbicmp((BYTE*)pItems[id].GetExt(),
																	 (BYTE*)pItems[GetSelectedItemNum(i)].GetExt(),
																	 pItems[id].ExtCharLen)))
						{	if(file==pItems[GetSelectedItemNum(i)].attribute)
							{	pItems[GetSelectedItemNum(i)].state=normal;
								if(file==pItems[GetSelectedItemNum(i)].attribute) ++delSelsFiles;
								else ++delSelsFolders;
					}	}	}
					totSelects -= delSelsFolders+delSelsFiles;
					totSelectsFolders -= delSelsFolders;
					totSelectsFiles -= delSelsFiles;
					break;
			}	
			break;
		case 2://invert selection
			int sFolders,sFiles;
			switch(cmd)
			{	case 0://all
					sFolders=0,sFiles=0;
					for(i=1; i<totItems; i++)
					{	if(selected==pItems[i].state)
							pItems[i].state=normal;
						else
						{	pItems[i].state=selected;
							if(file==pItems[i].attribute)++sFiles;
							else ++sFolders;
					}	}
					totSelectsFolders = sFolders;
					totSelectsFiles = sFiles;
					totSelects = sFolders+sFiles;
					break;
				case 1://all folders
					sFolders=0;
					for(i=1; i<totItems; i++)
					{	if(file==pItems[i].attribute) continue;
						if(selected==pItems[i].state)
							pItems[i].state=normal;
						else
						{	pItems[i].state=selected;
							++sFolders;
					}	}
					totSelectsFolders = sFolders;
					totSelects = sFolders+totSelectsFiles;
					break;
				case 2://all files
					int sFiles=0;
					for(i=1; i<totItems; i++)
					{	if(file!=pItems[i].attribute) continue;
						if(selected==pItems[i].state)
							pItems[i].state=normal;
						else
						{	pItems[i].state=selected;
							++sFiles;
					}	}
					totSelectsFiles = sFiles;
					totSelects = sFiles+totSelectsFolders;
					break;
			}
			break;
		case 3://save selection
			wchar_t *buf;long sz;
			switch(cmd)
			{	case 0://all
					//1.Calc size for selection:
					sz=0; for(i=0; i<totSelects; i++)
					{	int k=GetSelectedItemNum(i);
						sz += pItems[k].NameCharLen+pItems[k].ExtCharLen+2;
					} sz+=2;
					//2.alloc buf:
					if(pnlSaveSelectn)free(pnlSaveSelectn);
					pnlSaveSelectn=(wchar_t*)malloc(sizeof(wchar_t)*sz);
					//3.fill buf:
					buf=pnlSaveSelectn;
					for(i=0; i<totSelects; i++)
					{	int k=MyStringCpy(buf,MAX_PATH,pItems[GetSelectedItemNum(i)].Name);
						buf+=k+1;
					}
					*buf=0;//2 ta nol;
					CheckMenuItem(GetMenu(::hWnd),IDM_SAVESELECTION_ALL,MF_BYCOMMAND|MF_CHECKED);
					break;
				case 1://all folders
					//1.Calc size for selection:
					sz=0; for(i=0; i<totSelects; i++)
					{	int k=GetSelectedItemNum(i);
						if(file==pItems[k].attribute)continue;
						sz += pItems[k].NameCharLen+pItems[k].ExtCharLen+1;
					} sz+=2;
					//2.alloc buf:
					if(pnlSaveSelectn)free(pnlSaveSelectn);
					pnlSaveSelectn=(wchar_t*)malloc(sizeof(wchar_t)*sz);
					//3.fill buf:
					buf=pnlSaveSelectn;
					for(i=0; i<totSelects; i++)
					{	int k=GetSelectedItemNum(i);
						if(file==pItems[k].attribute)continue;
						k=MyStringCpy(buf,MAX_PATH,pItems[k].Name);
						buf+=k;
					}
					*buf=0;//2 ta nol;
					CheckMenuItem(GetMenu(::hWnd),IDM_SAVESELECTION_FOLDERS,MF_BYCOMMAND|MF_CHECKED);
					break;
				case 2://all files
					//1.Calc size for selection:
					sz=0; for(i=0; i<totSelects; i++)
					{	int k=GetSelectedItemNum(i);
						if(file!=pItems[k].attribute)continue;
						sz += pItems[k].NameCharLen+pItems[k].ExtCharLen+1;
					} sz+=2;
					//2.alloc buf:
					if(pnlSaveSelectn)free(pnlSaveSelectn);
					pnlSaveSelectn=(wchar_t*)malloc(sizeof(wchar_t)*sz);
					//3.fill buf:
					buf=pnlSaveSelectn;
					for(i=0; i<totSelects; i++)
					{	int k=GetSelectedItemNum(i);
						if(file!=pItems[k].attribute)continue;
						k=MyStringCpy(buf,MAX_PATH,pItems[k].Name);
						buf+=k;
					}
					*buf=0;//2 ta nol;
					CheckMenuItem(GetMenu(::hWnd),IDM_SAVESELECTION_FILES,MF_BYCOMMAND|MF_CHECKED);
					break;
			}
			break;
		case 4://restore selection
			if(!pnlSaveSelectn)break;
			FreeSelection();
			for(i=1; i<totItems; i++)
			{	wchar_t *buf=pnlSaveSelectn;
				do
				{	if(!wcscmp(pItems[i].Name,buf))
					{	AddToSelection(i);
						//if(i==iHot)pItems[i].state=hotselected;
					}
					buf += wcslen(buf)+1;
				} while(*buf);
			}
			if(pnlSaveSelectn)free(pnlSaveSelectn);
			pnlSaveSelectn=0;
			CheckMenuItem(GetMenu(::hWnd),IDM_SAVESELECTION_ALL,MF_BYCOMMAND|MF_UNCHECKED);
			CheckMenuItem(GetMenu(::hWnd),IDM_SAVESELECTION_FOLDERS,MF_BYCOMMAND|MF_UNCHECKED);
			CheckMenuItem(GetMenu(::hWnd),IDM_SAVESELECTION_FILES,MF_BYCOMMAND|MF_UNCHECKED);
			break;
	}
	Render();
}