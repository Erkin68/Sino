#include "Panel.h"
#include "Config.h"
#include "Operations\MyShell\MyShell.h"


VOID Panel::TIimgBckgrnd::Free()
{
	if(bm)DeleteObject(bm);
	if(dc)DeleteObject(dc);
	bm=0;dc=0;
}

BOOL Panel::TIimgBckgrnd::Load(Panel* prnt,wchar_t *imgPth)
{
	Free();
	if(imgPth)
	{	if(IsFileExist(imgPth))
			MyStringCpy(conf::Dlg.panBckgrndImgPth[prnt->iThis],MAX_PATH-1,imgPth);
		else
		{	conf::Dlg.panBckgrndImgPth[prnt->iThis][0]=0;
			return FALSE;
		}
	    if(!image::Load$24(conf::Dlg.panBckgrndImgPth[prnt->iThis],&dc,&bm,
						&width,&height,&bpp))
		{	conf::Dlg.panBckgrndImgPth[prnt->iThis][0]=0;
			return FALSE;
		}
		if(conf::Dlg.panBckgrndImgAnimTime!=0)
		{	if(hTimer)KillTimer(prnt->GetHWND(),hTimer);
			if(0xffffffff==conf::Dlg.panBckgrndImgAnimTime[prnt->iThis])
			{	int mstm=10000*(GetTickCount()%20);
				hTimer = SetTimer(prnt->GetHWND(),IDPANELTIMER_CHANGEBCKIMG,mstm,NULL);
			}
			else if(0!=conf::Dlg.panBckgrndImgAnimTime[prnt->iThis])
				hTimer = SetTimer(prnt->GetHWND(),IDPANELTIMER_CHANGEBCKIMG,
								  1000*conf::Dlg.panBckgrndImgAnimTime[prnt->iThis],NULL);
		}
		InvalidateRgn(::hWnd,NULL,TRUE);
		return TRUE;
	}
	return FALSE;
}

VOID Panel::TIimgBckgrnd::OnTimer(Panel *prnt)
{
WIN32_FIND_DATA ff;
HANDLE hf = INVALID_HANDLE_VALUE;
int bFind=0;
wchar_t *p,s[MAX_PATH];

	p = wcsrchr(&conf::Dlg.panBckgrndImgPth[prnt->iThis][0],'\\');
	if(!p) return;
	int l=MyStringCpy(s,(int)(p-&conf::Dlg.panBckgrndImgPth[prnt->iThis][0]),
						&conf::Dlg.panBckgrndImgPth[prnt->iThis][0]);
	s[l++]='\\';
Loop:
	s[l]='*';
	s[l+1]=0;
	hf = MyFindFirstFileEx(s,FindExInfoStandard,&ff,FindExSearchLimitToDirectories,NULL,0);
	s[l]=0;
	do
	{	if(INVALID_HANDLE_VALUE==hf) return;
		if(!(FILE_ATTRIBUTE_DIRECTORY & ff.dwFileAttributes))
		{	MyStringCpy(&s[l],MAX_PATH-1,ff.cFileName);
			if(bFind>0)
			{	if(Load(prnt,s))
				{	FindClose(hf);
					return;
			}	}
			if(0==wcscmp(s,&conf::Dlg.panBckgrndImgPth[prnt->iThis][0]))
				++bFind;
	}	}
	while(FindNextFile(hf, &ff));
	FindClose(hf);
	if(bFind!=2)
	{	bFind=2;
		goto Loop;
}	}

BOOL Panel::TIimgBckgrnd::CloneOnWM_SIZE(Panel* prnt,Panel::TIimgBckgrnd *to)
{if(!to)return FALSE;
 if(0==prnt->iThis && (!dc))return FALSE;
 else if(1!=conf::Dlg.panBckgrndImgType[0] && (!dc))return FALSE;

 to->Free();
 HDC tmpDC=::GetDC(prnt->GetHWND());
 to->dc = CreateCompatibleDC(dc);
 if(!to->dc)
 {::ReleaseDC(prnt->GetHWND(),tmpDC);
  return FALSE;
 }
 to->bm = CreateCompatibleBitmap(tmpDC,prnt->width,prnt->height);
 HGDIOBJ oldObj = SelectObject(to->dc,to->bm);
 ::ReleaseDC(prnt->GetHWND(),tmpDC);
 RECT r={0,0,prnt->width,prnt->height};
 FillRect(to->dc,&r,prnt->bkBrsh);

 //Agar Image kichik bo'lsa, o'rtaga qo'yamiz:
 if(0==conf::Dlg.panBckgrndImgType[prnt->iThis])
 {if(width<prnt->width && height<prnt->height)
   TransparentBlt(to->dc,
				 (prnt->width-width)/2,
				 (prnt->height-height)/2,
 				  width,height,
				  dc,0,0,width,height,0);//0xffffffff);
  else TransparentBlt(to->dc,0,0,prnt->width,prnt->height,dc,0,0,width,height,0);//0xffffffff);
  return TRUE;
 }
 else if(1==conf::Dlg.panBckgrndImgType[0])//oldingisidan suriladig'on:
 {if(0==prnt->iThis)
  {RECT rSino;GetClientRect(::hWnd,&rSino);
   if(width<rSino.right && height<rSino.bottom)
   {POINT pFr={(rSino.right-width)/2,(rSino.bottom-height)/2};
    POINT pTo={pFr.x+width,pFr.y+height};
	//frSino/toSino -->>> frPanle/toPanel:
	ClientToScreen(::hWnd,&pFr);ClientToScreen(::hWnd,&pTo);
	ScreenToClient(prnt->GetHWND(),&pFr);ScreenToClient(prnt->GetHWND(),&pTo);
	if(pFr.x>prnt->GetWidth())return FALSE;
	if(pTo.x>prnt->GetWidth())pTo.x=prnt->GetWidth();
    TransparentBlt(to->dc,
				   pFr.x,
				   pFr.y,
 				   pTo.x-pFr.x,
 				   pTo.y-pFr.y,
				   dc,0,0,pTo.x-pFr.x,pTo.y-pFr.y,0);//0xffffffff);
   }
   else//Hammasiga ham sig'magan:
   {POINT toXY={prnt->GetWidth(),prnt->GetHeight()};
    RECT rTot;GetClientRect(::hWnd,&rTot);
    if(0==width)return FALSE;
    POINT toSino=toXY;ClientToScreen(prnt->GetHWND(),&toSino);ScreenToClient(::hWnd,&toSino);
	TransparentBlt(	to->dc,0,0,toXY.x,toXY.y,
					dc,0,0,
					width*toSino.x/rTot.right,
					height,
					0);//0xffffffff);
   }
   return TRUE;
  }
  else if(panel[0].imgBckgrnd[1].dc)//Qolgan panellarga 1-panelning 0-imgBackgroundidan olamiz:
  {RECT rTot;GetClientRect(::hWnd,&rTot);
   if(panel[0].imgBckgrnd[1].width<rTot.right && panel[0].imgBckgrnd[1].height<rTot.bottom)
   {POINT pFr={(rTot.right-panel[0].imgBckgrnd[1].width)/2,(rTot.bottom-panel[0].imgBckgrnd[1].height)/2};
    POINT pTo={pFr.x+panel[0].imgBckgrnd[1].width,pFr.y+panel[0].imgBckgrnd[1].height};
	ClientToScreen(::hWnd,&pFr);ClientToScreen(::hWnd,&pTo);
	ScreenToClient(prnt->GetHWND(),&pFr);ScreenToClient(prnt->GetHWND(),&pTo);
	if(pFr.x<0)pFr.x=0;
	if(pFr.x>prnt->GetWidth())return FALSE;
	if(pTo.x>prnt->GetWidth())pTo.x=prnt->GetWidth();

	POINT pFrSino=pFr;ClientToScreen(prnt->GetHWND(),&pFrSino);ScreenToClient(::hWnd,&pFrSino);
	pFrSino.x -= (rTot.right-panel[0].imgBckgrnd[1].width)/2;
	pFrSino.y -= (rTot.bottom-panel[0].imgBckgrnd[1].height)/2;
	if(pFrSino.x<0)pFrSino.x = 0;if(pFrSino.y<0)pFrSino.y = 0;

    TransparentBlt(to->dc,
				   pFr.x,
				   pFr.y,
 				   pTo.x-pFr.x,
 				   pTo.y-pFr.y,
				   panel[0].imgBckgrnd[1].dc,
				   pFrSino.x,pFrSino.y,
				   pTo.x-pFr.x,pTo.y-pFr.y,0);//0xffffffff);
   }
   else//Hammasiga ham sig'magan:
   {RECT rTot;GetClientRect(::hWnd,&rTot);
    if(0==rTot.right || 0==rTot.right || 0==panel[0].imgBckgrnd[1].width)return FALSE;
	POINT toXY={prnt->GetWidth(),prnt->GetHeight()};
	POINT frSino={0,0};ClientToScreen(prnt->GetHWND(),&frSino);ScreenToClient(::hWnd,&frSino);
    POINT toSino=toXY;ClientToScreen(prnt->GetHWND(),&toSino);ScreenToClient(::hWnd,&toSino);
	TransparentBlt(	to->dc,0,0,toXY.x,toXY.y,
					panel[0].imgBckgrnd[1].dc,
					panel[0].imgBckgrnd[1].width*frSino.x/rTot.right,
					0,
					panel[0].imgBckgrnd[1].width*(toSino.x-frSino.x)/rTot.right,
					panel[0].imgBckgrnd[1].height,
					0);//0xffffffff);
   }
   return TRUE;
 }}
 return FALSE;
}
