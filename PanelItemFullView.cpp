#include "Sino.h"
#include "Panel.h"
#include "Config.h"
#include "strsafe.h"
#include "FileAssociation.h"
#include "Operations\LinkSocket.h"
#include "Operations\MyShell\MyShell.h"
#include "WindowsManagmentInstrumentation.h"


extern COLORREF chngClr(DWORD);


VOID PanelItem::RenderFullCnfg(HDC DC,Panel *prnt,BOOL bRenderFast)//, PAINTSTRUCT *ps
{
	if(0xff!=iCstmItem)
	{	if(iCstmItem<conf::Dlg.iCustPanItemDefs[prnt->iThis])
			return RenderCustomItem(DC,prnt,bRenderFast);
		else iCstmItem = 0xff;
	}
RECT rc; GetRect(&rc,prnt,TRUE);
	if(DrawOtsech(prnt))return;
	++rc.bottom;
RECT rcBht=rc; rcBht.left+=2;rcBht.top+=1;rcBht.right-=2;rcBht.bottom-=1;


HFONT oldFnt;
HDC dc = (0==DC) ? prnt->GetDC() : DC;
	if(0==DC)
		oldFnt=(HFONT)SelectObject(dc,prnt->hf);

	if(!bRenderFast)
	if(!hMasterIcon)
	if(-1>assocNum)
	if(-100<assocNum)
	{	prnt->fAssociation.AddMasterIcon(this,prnt->path);
		if(!hMasterIcon)
		{	wchar_t* ext=ConvertToLittleR(&Name[NameCharLen+1],0);
			assocNum = prnt->fAssociation.IsRecExist(ext);
		}
		else
			assocNum = -100;
	}

	BOOL bht = (prnt->iHot==this-&prnt->pItems[0] && Panel::iActivePanel==prnt->iThis);
	BOOL lstChng = (attribute==file)?((CompareFileTime(&conf::strtTm,&ftWrite)>=0)?FALSE:TRUE):FALSE;

	if(prnt->hEditItem && this==prnt->iEditItem)//If in edit mode
	{	SetBkColor(dc,prnt->color[2]);
		rc.right = rc.left + XBITMAP_prnt;
		FillRect(dc,&rc,prnt->hBrshSrtBtn);
	} else
	{switch(state)//If non in edit mode
	 {	case normal     :
			if(bht)
			{	SetTextColor(dc,lstChng?chngClr(prnt->color[4]):prnt->color[4]);//RGB(255,255,255));
				SetBkColor(dc,prnt->color[5]);
				FillRect(dc,&rc,prnt->brEdit);
				FillRect(dc,&rcBht,prnt->hotBkBrsh);
			} else
			{	SetTextColor(dc,lstChng?chngClr(prnt->color[1]):prnt->color[1]);
				SetBkColor(dc,prnt->color[0]);
				FillRect(dc,&rc,prnt->bkBrsh);
			}
		break;
		case selected   :
			if(bht)
			{	SetTextColor(dc,lstChng?chngClr(prnt->color[3]):prnt->color[3]);
				SetBkColor(dc,prnt->color[5]);
				FillRect(dc,&rc,prnt->brEdit);
				FillRect(dc,&rcBht,prnt->hotBkBrsh);
			} else
			{	SetTextColor(dc,lstChng?chngClr(prnt->color[3]):prnt->color[3]);
				SetBkColor(dc,prnt->color[2]);
				FillRect(dc,&rc,prnt->txtBkBrsh);
			}
		break;
	}}

	if(bht)rc=rcBht;
	int sz = conf::Dlg.iIconSize[prnt->iThis];

	//Icon:
	if(hMasterIcon)
	{	DrawIconEx(	dc,
					rc.left+1,
					rc.top,
					hMasterIcon,
					sz,
					bht?(sz-3):sz,
					0,0,DI_NORMAL);
	}
	else
	{	HICON *pi = prnt->fAssociation.GetIcon(assocNum);
		if(pi)
		{	DrawIconEx(	dc,
						rc.left+1,
						rc.top,
						*pi,
						sz,
						bht?(sz-3):sz,
						0,0,DI_NORMAL);
		}
		else
			DrawIconEx(	dc,
						rc.left+1,
						rc.top,
						prnt->hIcons[attribute],
						sz,
						bht?(sz-3):sz,
						0,0,DI_NORMAL);
	}

	if(prnt->hEditItem && this==prnt->iEditItem) return;


	RECT rcTxt = bht?rcBht:rc;
	if(bht)
	{	++rcTxt.top;
	}
	rcTxt.left += XBITMAP_prnt;
	RECT rcExt = bht?rcBht:rc;

	wchar_t sFldr[MAX_PATH]=L"[";
	//Text:
	if(file!=attribute)//folder & header:
	{	int l=1+MyStringCpy(&sFldr[1],MAX_PATH-1,Name);
		sFldr[l++]=']';
		sFldr[l]=0;
		ExtTextOut(dc,rcTxt.left,rcTxt.top,ETO_OPAQUE,&rcTxt,sFldr,NameCharLen+2,NULL);

		if(folder==attribute)//folder & header:
		{	sFldr[0]='<';sFldr[1]='F';sFldr[2]='>';sFldr[3]=' ';sFldr[4]=0;
			int l=4;SYSTEMTIME st;if(FileTimeToSystemTime(&ftWrite,&st))
			StringCchPrintf(&sFldr[4],32,L"%0.2d%c%0.2d%c%0.4d %0.2d:%0.2d:%0.2d",
						(int)st.wDay,'.',(int)st.wMonth,'.',(int)st.wYear,st.wHour,st.wMinute,st.wSecond);
			if(sFldr[4])
				DrawText(dc,sFldr,23,&rcTxt,DT_RIGHT);
			else
			{	MyStringCpy(&sFldr[4],36,L"                                ");
				DrawText(dc,sFldr,36,&rcTxt,DT_RIGHT);
	}	}	}
	else//File:
	{	int dev = prnt->MyGetTextExtentPoint32(L"...",3);
		int ovDel = nameWidth+extWidth+dev-prnt->rcClient.right;
		if(ovDel>0)
		{	if(bht)
				DrawText(dc,Name,NameCharLen-2*ovDel/dev,&rcTxt,DT_LEFT);
			else
				ExtTextOut(dc,rcTxt.left,rcTxt.top,ETO_OPAQUE,&rcTxt,Name,NameCharLen-2*ovDel/dev,NULL);
			DrawText(dc,L"...",3,&rcExt,DT_RIGHT);
		} else
		{	if(bht)
				DrawText(dc,Name,NameCharLen,&rcTxt,DT_LEFT);
			else
				ExtTextOut(dc,rcTxt.left,rcTxt.top,ETO_OPAQUE,&rcTxt,Name,NameCharLen,NULL);
		}
		rcExt.left = (LONG)(0.6f*rcExt.right);
		DrawText(dc,GetExt(),ExtCharLen,&rcExt,DT_LEFT);

		rcExt.left += extWidth+1;
		int l = MyStringPrintDecSpace1(sFldr,size/1024);
		rcExt.right = rcExt.left + 40;
		DrawText(dc,sFldr,l,&rcExt,DT_RIGHT);

		rcExt.right = prnt->rcClient.right;
		SYSTEMTIME st;if(FileTimeToSystemTime(&ftWrite,&st))
		if(S_OK==StringCchPrintf(sFldr,32,L"%0.2d%c%0.2d%c%0.4d %0.2d:%0.2d",
					(int)st.wDay,'.',(int)st.wMonth,'.',(int)st.wYear,st.wHour,st.wMinute))
			DrawText(dc,sFldr,16,&rcExt,DT_RIGHT);
	}
	if(0==DC)//Har 1 item alohida chizilyapti:
	{	SelectObject(dc,oldFnt);
		prnt->ReleaseDC(dc);
	}
	return;
}

VOID PanelItem::RenderTransparentFullCnfg(HDC DC,Panel *prnt,BOOL bRenderFast)
{
	if(0xff!=iCstmItem)
	{	if(iCstmItem<conf::Dlg.iCustPanItemDefs[prnt->iThis])
			return RenderCustomItemTransparent(DC,prnt,bRenderFast);
		else iCstmItem = 0xff;
	}
RECT rc; GetRect(&rc,prnt,TRUE);
	if(DrawOtsech(prnt))return;
	++rc.bottom;
RECT rcBht=rc; rcBht.left+=2;rcBht.top+=1;rcBht.right-=2;rcBht.bottom-=1;

HFONT oldFnt;
HDC dc = (0==DC) ? prnt->GetDC() : DC;
	if(0==DC)
		oldFnt=(HFONT)SelectObject(dc,prnt->hf);

int rw = rc.right-rc.left;
int rh = rc.bottom-rc.top;
	BitBlt(dc,rc.left,rc.top,rw,rh,prnt->imgBckgrnd[0].dc,rc.left,rc.top,SRCCOPY);

	if(!bRenderFast)//0 b-sa tezlash uchun;
	if(!hMasterIcon)
	if(-1>assocNum)
	if(-100<assocNum)
	{	prnt->fAssociation.AddMasterIcon(this,prnt->path);//s);
		if(!hMasterIcon)
		{	wchar_t* ext=ConvertToLittleR(&Name[NameCharLen+1],0);
			assocNum = prnt->fAssociation.IsRecExist(ext);//&Name[NameCharLen+1]);
		}
		else
			assocNum = -100;//Okonchatelniy, qaytib bu yergayam kirmasun;
	}

	BOOL bht = (prnt->iHot==this-&prnt->pItems[0] && Panel::iActivePanel==prnt->iThis);
	BOOL lstChng = (attribute==file)?((CompareFileTime(&conf::strtTm,&ftWrite)>=0)?FALSE:TRUE):FALSE;

	if(prnt->hEditItem && this==prnt->iEditItem)//If in edit mode
	{	rc.right = rc.left + XBITMAP_prnt;
	} else
	{switch(state)//If non in edit mode
	 {	case normal     :
			if(bht)
				SetTextColor(dc,lstChng?chngClr(prnt->color[4]):prnt->color[4]);
			else
				SetTextColor(dc,lstChng?chngClr(prnt->color[1]):prnt->color[1]);
		break;
		case selected   :
			if(bht)
				SetTextColor(dc,lstChng?chngClr(prnt->color[3]):prnt->color[3]);
			else
				SetTextColor(dc,lstChng?chngClr(prnt->color[3]):prnt->color[3]);
		break;
    }}

	if(bht)rc=rcBht;
	int sz = conf::Dlg.iIconSize[prnt->iThis];

	//Icon:
	if(hMasterIcon)//if icon file or cursor file; ico b-sa ExtractIcon 32 lik ham ochishi mumkin ekan;
	{	DrawIconEx(dc,
					rc.left+1,
					rc.top,
					hMasterIcon,
					sz,
					bht?(sz-1):sz,
					0,0,DI_NORMAL);
	}
	else
	{	HICON *pi = prnt->fAssociation.GetIcon(assocNum);
		if(pi)
		{	DrawIconEx(dc,
						rc.left+1,
						rc.top,
						*pi,
						sz,
						bht?(sz-1):sz,
						0,0,DI_NORMAL);
		}
		else
		{	DrawIconEx(	dc,
						rc.left+1,
						rc.top,
						prnt->hIcons[attribute],
						sz,
						bht?(sz-1):sz,
						0,0,DI_NORMAL);
	}	}

	if(prnt->hEditItem && this==prnt->iEditItem) return;


	RECT rcTxt = bht?rcBht:rc;
	if(bht)
	{	++rcTxt.top;
	}
	rcTxt.left += XBITMAP_prnt;
	RECT rcExt = bht?rcBht:rc;

	//Text:
	wchar_t sFldr[MAX_PATH]=L"[";
	if(file!=attribute)//folder & header:
	{	int l=1+MyStringCpy(&sFldr[1],MAX_PATH-1,Name);
		sFldr[l++]=']';
		sFldr[l]=0;
		if(bht)
			DrawText(dc,sFldr,NameCharLen+2,&rcTxt,DT_LEFT);
		else
			ExtTextOut(dc,rcTxt.left,rcTxt.top,0,&rcTxt,sFldr,NameCharLen+2,NULL);

		if(folder==attribute)//folder & header:
		{	sFldr[0]='<';sFldr[1]='F';sFldr[2]='>';sFldr[3]=' ';sFldr[4]=0;
			int l=4;SYSTEMTIME st;if(FileTimeToSystemTime(&ftWrite,&st))
			StringCchPrintf(&sFldr[4],32,L"%0.2d%c%0.2d%c%0.4d %0.2d:%0.2d:%0.2d",
						(int)st.wDay,'.',(int)st.wMonth,'.',(int)st.wYear,st.wHour,st.wMinute,st.wSecond);
			if(sFldr[4])
				DrawText(dc,sFldr,23,&rcTxt,DT_RIGHT);
			else
			{	MyStringCpy(&sFldr[4],36,L"                                ");
				DrawText(dc,sFldr,36,&rcTxt,DT_RIGHT);
	}	}	}
	else//File:
	{	int dev = prnt->MyGetTextExtentPoint32(L"...",3);
		int ovDel = nameWidth+extWidth+dev-prnt->rcClient.right;
		rcExt.left = rcExt.left-extWidth-MIN_DIST_FROM_COLUMNS-dev;
		if(ovDel>0)
		{	if(bht)
				DrawText(dc,Name,NameCharLen-2*ovDel/dev,&rcTxt,DT_LEFT);
			else
				ExtTextOut(dc,rcTxt.left,rcTxt.top,0,&rcTxt,Name,NameCharLen-2*ovDel/dev,NULL);
		}
		else
		{	if(bht)
				DrawText(dc,Name,NameCharLen,&rcTxt,DT_LEFT);
			else
				ExtTextOut(dc,rcTxt.left,rcTxt.top,0/*ETO_OPAQUE*/,&rcTxt,Name,NameCharLen,NULL);
		}
		rcExt.left = (LONG)(0.6f*rcExt.right);
		DrawText(dc,GetExt(),ExtCharLen,&rcExt,DT_LEFT);

		rcExt.left += extWidth+1;
		int l = MyStringPrintDecSpace1(sFldr,size/1024);
		rcExt.right = rcExt.left + 40;
		DrawText(dc,sFldr,l,&rcExt,DT_RIGHT);

		rcExt.right = prnt->rcClient.right;
		SYSTEMTIME st;if(FileTimeToSystemTime(&ftWrite,&st))
		if(S_OK==StringCchPrintf(sFldr,32,L"%0.2d%c%0.2d%c%0.4d %0.2d:%0.2d",
					(int)st.wDay,'.',(int)st.wMonth,'.',(int)st.wYear,st.wHour,st.wMinute))
			DrawText(dc,sFldr,16,&rcExt,DT_RIGHT);
	}
	if(0==DC)//Har 1 item alohida chizilyapti:
	{	SelectObject(dc,oldFnt);
		prnt->ReleaseDC(dc);
	}
	return;
}