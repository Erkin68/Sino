#include <malloc.h>
#include "config.h"
#include "stdio.h"
#include "sino.h"
#include "selectviaplus.h"

#define MAX_SAVE_SELECTION_STR 25

SelectViaPlus fSelViaPlus;
SelectViaPlus::SelectViaPlus():slctnStrs(NULL),iSlctns(0)
{
}

SelectViaPlus::~SelectViaPlus()
{
	free(slctnStrs);
}

VOID SelectViaPlus::AddSelection(HWND dlg, int idEdt, int idCombo)
{
char s[MAX_SEL_PATH]; GetDlgItemText(dlg, idEdt, s, MAX_SEL_PATH);
	char *p = strrchr(s,'.');
	if(p && *(p-1)=='*')//Hech bo'lmasa 1 tagina '*.' belgilari bor;
	{	
		//Check existing:
		for(int k=0; k<iSlctns; k++)
		{	if(strstr(slctnStrs[k].c,s))
				return;
		}

		//Add to CB:
		SendMessage(GetDlgItem(dlg,idCombo),CB_ADDSTRING,0,(LPARAM)s);

		//Add to selection:		
		slctnStrs = (SelStrs*)realloc(slctnStrs,MAX_SEL_PATH*(iSlctns+1));
		MyStringCpy(slctnStrs[iSlctns].c,MAX_SEL_PATH,s);
		iSlctns++;
		//Save selection:
		HANDLE f = MyFopenViaCrF(MyStringAddModulePath("Viewers\\Selection.bin"),"w"); if(!f)return;
		for(int i=0; i<iSlctns; i++)
		{	DWORD nw;
			for(int j=0; j<MAX_SEL_PATH; j++)
			{	if(slctnStrs[i].c[j]==0)break;
				WriteFile(f,&slctnStrs[i].c[j],1,&nw,NULL);
			}
			char da[2]={0x0d,0x0a};
			WriteFile(f,da,2,&nw,NULL);
		}
		CloseHandle(f);
}	}

VOID SelectViaPlus::FindSelectsItems(Panel *pan,char* str,int iFrom,int iTo)
{
	switch(pan->entry.GetCrntRecType())
	{	default:case unknown:
			return;
		case directFolder:
			char pth[MAX_PATH];
			MyStringCpy(pth,MAX_PATH,pan->path);
			int l;l = MyStringLength(pth,MAX_PATH);
			char *p;p = strrchr(pth,'*');
			if(!p)
				p = &pth[l];
			else if('\\'!=*(p-1))
				p = &pth[l];

			for(int i=iFrom; i<=iTo; i++)
				*p++ = str[i];
			*p = 0;

			WIN32_FIND_DATA ff;
			HANDLE hFind;hFind = INVALID_HANDLE_VALUE;

			hFind = FindFirstFile(pth, &ff);
			if(INVALID_HANDLE_VALUE==hFind) return;

			for(int i=1; i<pan->totItems; i++)
			{	if(strstr(ff.cFileName,pan->pItems[i].Name))
				{	int ln = MyStringLength(ff.cFileName,MAX_PATH);
					if(ln==pan->pItems[i].NameCharLen+
						(pan->pItems[i].ExtCharLen?pan->pItems[i].ExtCharLen+1:0))
						pan->AddToSelection(i,selected,TRUE);
			}	}

			while(FindNextFile(hFind, &ff))
			{	for(int i=1; i<pan->totItems; i++)
				{	if(strstr(ff.cFileName,pan->pItems[i].Name))
					{	int ln = MyStringLength(ff.cFileName,MAX_PATH);
						if(ln==pan->pItems[i].NameCharLen+
							(pan->pItems[i].ExtCharLen?pan->pItems[i].ExtCharLen+1:0))
							pan->AddToSelection(i,selected,TRUE);
			}	}	}
			FindClose(hFind);
			return;
		case guidFolder:
		 for(int i=1; i<pan->totItems; i++)
		 {int j=iFrom,k=0;
		  for(;;)//1-st for;
		  {if(j==iTo)
		   {if(k==pan->pItems[i].NameCharLen)
		     break;//to search Ext;
		    if(str[j]=='*')
				goto Select;
		    goto SkipSelect;//Do not select;
		   }
		   else if(k==pan->pItems[i].NameCharLen)
		   {
			   goto SkipSelect;//Do not select;
		   }
		   
		   if(str[j]=='.')
		   {if(j==pan->pItems[i].NameCharLen)
		   {j++;break;}//Ext;      break fr 1-st for;
		    else goto SkipSelect;
		   }
		   else if(str[j]!='*')
		   {if(pan->pItems[i].Name[k]==str[j])
		     {j++;k++;continue;}//cont to 1-st for;
		    else {goto SkipSelect;}//do not select;
		   }
		   else//if(str[j]=='*')
		   {if(str[j+1]=='.'){j+=2;break;}//Goto Ext, skip 1-st for;
		    if(pan->pItems[i].Name[k]==str[j+1])
			  {j++;continue;}//go to next, 1-st for;
		    else if(pan->pItems[i].Name[k+1]==str[j+1])
			  {k++;j++;continue;}//go to next, 1-st for;
			k++;continue;//Name ni sur;
		  }}

		//Ext:
		  k=0;
		  for(;;)//1-st for;
		  {if(j==iTo)
		   {if(k==pan->pItems[i].ExtCharLen)
		     break;//to search Ext;
		    if(str[j]=='*')
				goto Select;
		    goto SkipSelect;//Do not select;
		   }
		   else if(k==pan->pItems[i].ExtCharLen)
		   {  
			   goto SkipSelect;//Do not select;
		   }
		   		   
		   if(str[j]=='.')
		   {if(j==pan->pItems[i].ExtCharLen)
				break;//Ext;      break fr 1-st for;
		    else goto SkipSelect;
		   }
		   else if(str[j]!='*')
		   {if(*(pan->pItems[i].GetExt()+k)==str[j])
		     {j++;k++;continue;}//cont to 1-st for;
		    else {goto SkipSelect;}//do not select;
		   }
		   else//if(str[j]=='*')
		   {if(str[j+1]=='.')break;//Goto Ext, skip 1-st for;
		    if(*(pan->pItems[i].GetExt()+k)==str[j+1])
			  {j++;continue;}//go to next, 1-st for;
		    else if(*(pan->pItems[i].GetExt()+k+1)==str[j+1])
			  {k++;j++;continue;}//go to next, 1-st for;
			k++;continue;//Name ni sur;
		  }}
		Select:
		  pan->AddToSelection(i,selected,TRUE);
		SkipSelect:
		   ;
		 }
		 return;
}	}

HANDLE MyFopenViaCrF(char *nm,const char *mode)
{
HANDLE h;
	if(strstr(mode,"rw"))
	{	h = CreateFile(nm,GENERIC_READ|GENERIC_WRITE,
						  FILE_SHARE_READ|FILE_SHARE_WRITE,
						  NULL,
						  OPEN_ALWAYS,
						  FILE_ATTRIBUTE_NORMAL,NULL);
	}
	else if(strstr(mode,"r"))
	{	h = CreateFile(nm,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	}
	else if(strstr(mode,"w"))
	{	h = CreateFile(nm,GENERIC_WRITE,FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	}
	if(INVALID_HANDLE_VALUE==h)
		return NULL;
	return h;
}

VOID SelectViaPlus::ReadCrntSeln(HWND dlg,int idCombo)
{
	BOOL bWr = TRUE;
	HANDLE f = MyFopenViaCrF(MyStringAddModulePath("Viewers\\Selection.bin:stream"),"r"); if(!f)return;
	for(int i=0; i<MAX_SAVE_SELECTION_STR; i++)//while(!feof(f))
	{	char s[MAX_SEL_PATH],si[MAX_SEL_PATH]; DWORD nr;		
		if(ReadFile(f,s,MAX_SEL_PATH,&nr,NULL))
		{	BOOL bExist = FALSE;
			int r = SendMessage(GetDlgItem(dlg,idCombo),CB_GETCOUNT,0,0);
			if(CB_ERR!=r)
			{	for(int j=0; j<r; j++)
				{	if(CB_ERR!=SendMessage(GetDlgItem(dlg,idCombo),CB_GETLBTEXT,j,(LPARAM)si))
					{	if(!strcmp(s,si))
						{	bExist = TRUE;
							break;
			}	}	}	}
			if(!bExist)
			if(s[0])
				SendMessage(GetDlgItem(dlg,idCombo),CB_ADDSTRING,0,(LPARAM)s);
			if(0==i)
			{	SetDlgItemText(dlg,idCombo,s);
				bWr = FALSE;
	}	}	}
	CloseHandle(f);
	if(bWr)
		SetDlgItemText(dlg,idCombo,"*.*;");
}

VOID SelectViaPlus::ReadSelections(HWND dlg,int idCombo)
{
 if(0==iSlctns)
 {
	 HANDLE f = MyFopenViaCrF(MyStringAddModulePath("Viewers\\Selection.bin"),"r"); if(!f)return;
int i=0;
	for(;;)//while(!feof(f))
	{	char s[MAX_SEL_PATH],*p; DWORD nr;		
Loop:	if(!ReadFile(f,&s[i],1,&nr,NULL))
		{	//Agar oxirida 0d 0a b-masa:
			if(i>0)
			{	s[i]=0;
				SendMessage(GetDlgItem(dlg,idCombo),CB_ADDSTRING,0,(LPARAM)s);
				slctnStrs = (SelStrs*)realloc(slctnStrs,MAX_SEL_PATH*(iSlctns+1));
				MyStringCpy(slctnStrs[iSlctns].c,MAX_SEL_PATH,s);
				iSlctns++; i=0;
			}
			break;
		}
		if(!nr)
		{	//Agar oxirida 0d 0a b-masa:
			if(i>0)
			{	s[i]=0;
				SendMessage(GetDlgItem(dlg,idCombo),CB_ADDSTRING,0,(LPARAM)s);
				slctnStrs = (SelStrs*)realloc(slctnStrs,MAX_SEL_PATH*(iSlctns+1));
				MyStringCpy(slctnStrs[iSlctns].c,MAX_SEL_PATH,s);
				iSlctns++; i=0;
			}		
			break;
		}
		if(i>0 && (s[i]==0x0a || s[i]==0x0d))
		{	s[i] = 0;
		    p = strrchr(s,'.');//Hech bo'lmasa 1 tagina '*.' belgilari bor;
			if(p && *(p-1)=='*')
			{	SendMessage(GetDlgItem(dlg,idCombo),CB_ADDSTRING,0,(LPARAM)s);
				slctnStrs = (SelStrs*)realloc(slctnStrs,MAX_SEL_PATH*(iSlctns+1));
				MyStringCpy(slctnStrs[iSlctns].c,MAX_SEL_PATH,s);
				iSlctns++; i=0;
		}	}
		else 
		{	if(0==i && (s[i] == 0x0d || s[i] == 0x0a)) goto Loop;
			i++;
			goto Loop;
	}	}
	CloseHandle(f);
 }
 else
 {	for(int i=0; i<iSlctns; i++)
	{	SendMessage(GetDlgItem(dlg,idCombo),CB_ADDSTRING,0,(LPARAM)slctnStrs[i].c);
 }	}
 return;
}

VOID SelectViaPlus::RemoveSelections(int rem)
{
	if(iSlctns<1)return;
	for(int i=rem; i<iSlctns-1; i++)
	{	MyStringCpy(slctnStrs[i].c,MAX_SEL_PATH,slctnStrs[i+1].c);
	}
	slctnStrs = (SelStrs*)realloc(slctnStrs,MAX_SEL_PATH*(iSlctns-1));
	iSlctns--;
}

INT_PTR CALLBACK SelectViaPlusDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
static char str[MAX_SEL_PATH];
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		RECT rc; GetWindowRect(hDlg, &rc);
		int width,left,height,top;

		width = rc.right - rc.left;
		left = conf.wndLeft + (conf.wndWidth - width)/2;

		height = rc.bottom - rc.top;
		top = conf.wndTop + (conf.wndHeight - height)/2;
		
		MoveWindow(hDlg, left, top, width, height, TRUE);

		char st[MAX_PATH];
		LoadString(hInst,IDS_STRINGSW_62,st,MAX_PATH);
		SetWindowText(hDlg,st);
		LoadString(hInst,IDS_STRINGSW_63,st,MAX_PATH);
		SetDlgItemText(hDlg,IDC_STATIC_8,st);
		LoadString(hInst,IDS_STRINGSW_64,st,MAX_PATH);
		SetDlgItemText(hDlg,IDC_STATIC_9,st);
		LoadString(hInst,IDS_STRINGSW_65,st,MAX_PATH);
		SetDlgItemText(hDlg,IDB_REMEMBER_SELECTION,st);
		LoadString(hInst,IDS_STRINGSW_41,st,MAX_PATH);
		SetDlgItemText(hDlg,IDOK,st);
		LoadString(hInst,IDS_STRINGSW_13,st,MAX_PATH);
		SetDlgItemText(hDlg,IDCANCEL,st);
		LoadString(hInst,IDS_STRINGSW_66,st,MAX_PATH);
		SetDlgItemText(hDlg,IDB_REMOVE_FROM_SELECTION,st);

		fSelViaPlus.ReadCrntSeln(hDlg,IDC_EDIT_SELECTION);

		fSelViaPlus.ReadSelections(hDlg,IDC_COMBO_SELECTION);
		return (INT_PTR)TRUE;

	case WM_KEYDOWN:
		if(wParam==13)
		{	GetDlgItemText(hDlg,IDC_EDIT_SELECTION,str,MAX_SEL_PATH);
			if(strstr(str,"**"))
			{	MessageBox(hDlg,"Err. symbol sequence is not valid.","**",MB_OK);
				return (INT_PTR)TRUE;
			}
			SendMessage(GetDlgItem(hDlg,IDC_EDIT_SELECTION),CB_INSERTSTRING,0,(LPARAM)str);
			fSelViaPlus.WriteCrntSeln(hDlg,IDC_EDIT_SELECTION);
			EndDialog(hDlg, (INT_PTR)str);
			return (INT_PTR)TRUE;
		}
		break;

	case WM_COMMAND:
		switch(LOWORD(wParam))
		{	case IDOK:
				GetDlgItemText(hDlg,IDC_EDIT_SELECTION,str,MAX_SEL_PATH);
				if(strstr(str,"**"))
				{	MessageBox(hDlg,"Err. symbol sequence is not valid.","**",MB_OK);
					return (INT_PTR)TRUE;
				}
				SendMessage(GetDlgItem(hDlg,IDC_EDIT_SELECTION),CB_INSERTSTRING,0,(LPARAM)str);
				fSelViaPlus.WriteCrntSeln(hDlg,IDC_EDIT_SELECTION);
				EndDialog(hDlg, (INT_PTR)str);
				return (INT_PTR)TRUE;
			case IDCANCEL:
				EndDialog(hDlg, (INT_PTR)NULL);
				return (INT_PTR)TRUE;
			case IDB_REMEMBER_SELECTION:
				fSelViaPlus.AddSelection(hDlg, IDC_EDIT_SAVE_SELECTION, IDC_COMBO_SELECTION);
				return (INT_PTR)TRUE;
			case IDC_COMBO_SELECTION:
				if(CBN_SELCHANGE==HIWORD(wParam))
				{	int selId; selId = 
						SendMessage(GetDlgItem(hDlg,IDC_COMBO_SELECTION),CB_GETCURSEL,0,0);
					if(CB_ERR!=selId)
					if(selId>-1)
					if(selId<fSelViaPlus.iSlctns)
					{	TCHAR s[MAX_SEL_PATH];
						if(CB_ERR!=SendMessage(GetDlgItem(hDlg,IDC_COMBO_SELECTION),CB_GETLBTEXT,selId,(LPARAM)s))
							SetDlgItemText(hDlg,IDC_EDIT_SELECTION,s);
				}	}
				break;
			case IDC_EDIT_SELECTION:
				if(CBN_SELCHANGE==HIWORD(wParam))
				{	int selId; selId = 
						SendMessage(GetDlgItem(hDlg,IDC_EDIT_SELECTION),CB_GETCURSEL,0,0);
					if(CB_ERR!=selId)
					if(selId>-1)
					if(selId<fSelViaPlus.iSlctns)
					{	TCHAR s[MAX_SEL_PATH];
						if(CB_ERR!=SendMessage(GetDlgItem(hDlg,IDC_EDIT_SELECTION),CB_GETLBTEXT,selId,(LPARAM)s))
							SetDlgItemText(hDlg,IDC_EDIT_SELECTION,s);
				}	}
				break;
			case IDB_REMOVE_FROM_SELECTION:
				int selId; selId = 
					SendMessage(GetDlgItem(hDlg,IDC_COMBO_SELECTION),CB_GETCURSEL,0,0);
				if(CB_ERR!=selId)
				if(selId>-1)
				if(selId<fSelViaPlus.iSlctns)
				{	TCHAR s[MAX_SEL_PATH];
					if(CB_ERR!=SendMessage(GetDlgItem(hDlg,IDC_COMBO_SELECTION),CB_GETLBTEXT,selId,(LPARAM)s))
					{	SendMessage(GetDlgItem(hDlg,IDC_COMBO_SELECTION),CB_DELETESTRING,selId,0);
						//SendMessage(GetDlgItem(hDlg,IDC_COMBO_SELECTION),WM_SETREDRAW,TRUE,0);					
						for(int i=0; i<fSelViaPlus.iSlctns; i++)
						{	if(strstr(fSelViaPlus.slctnStrs[i].c,s))
							{	fSelViaPlus.RemoveSelections(i);
								break;
				}	}	}	}
				break;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

VOID SelectViaPlus::Select(Panel *pan)
{
	unsigned char *str = (unsigned char*)DialogBox(hInst,MAKEINTRESOURCE(IDD_DIALOG_SELECT_WIA_PLUS),hWnd,SelectViaPlusDlgProc);
	if(!str)return;

	int selStrLen = MyStringLength((char*)str,MAX_SEL_PATH);

	int iFrom = 0, oldHot = pan->GetHot();
	pan->DeselectAllWithout(-1);

	for(int i=0; i<=selStrLen; i++)//Oxirgi 0 niyam olsun, aks holda 1ta qolib ketadur;
	{	if(' '==str[i] ||//Ajratamiz;
		   ';'==str[i] ||//Ajratamiz;
		   '<'==str[i] ||//Ajratamiz;
		   '>'==str[i] ||//Ajratamiz;
		   ':'==str[i] ||//Ajratamiz;
		  '\"'==str[i] ||//Ajratamiz;
		   '/'==str[i] ||//Ajratamiz;
		  '\\'==str[i] ||//Ajratamiz;
		   '|'==str[i] ||//Ajratamiz;
		   '?'==str[i] ||//Ajratamiz;
		   str[i]<32    )//Ajratamiz;  0 ham kiradi, ya'ni oxiri;
		   //'*' ni qoldiramiz, chunki hammasi degani;
		{	if(i-iFrom>0)
			{	FindSelectsItems(pan,(char*)str,iFrom,i-1);
				iFrom = i+1;
				continue;
	}	}	}

	if(pan->totSelects<1 &&	oldHot>0)
	{	pan->ChangeSelection(oldHot,hotselected,TRUE);
		pan->ScrollToSelectItemView(oldHot);
	}
	else
	{
		pan->ScrollToSelectItemView(pan->pSelects[0]);
}	}

VOID SelectViaPlus::WriteCrntSeln(HWND dlg,int idCombo)
{
	HANDLE f = MyFopenViaCrF(MyStringAddModulePath("Viewers\\Selection.bin:stream"),"w"); if(!f)return;
	for(int i=0; i<MAX_SAVE_SELECTION_STR; i++)//while(!feof(f))
	{	char s[MAX_SEL_PATH]; DWORD nr;
		SendMessage(GetDlgItem(dlg,idCombo),CB_GETLBTEXT,i,(LPARAM)s);
		BOOL bEnd=TRUE;
		for(int k=0; k<MAX_SEL_PATH; k++)
		{	if(0==s[k] && bEnd)
				bEnd = FALSE;
			else if(!bEnd)s[k]=0;
		}
		WriteFile(f,s,MAX_SEL_PATH,&nr,NULL);
	}
	CloseHandle(f);
}

#undef MAX_SEL_PATH
