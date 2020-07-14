#include "windows.h"
#include "commctrl.h"
#include "shlwapi.h"
#include "sino.h"
#include "panel.h"
#include "resizebar.h"
#include "config.h"
#include "Profiler.h"
#include "Operations\MyErrors.h"
#include "Operations\Execution.h"
#include "Operations\MyShell\ComboToDisk.h"
#include "Operations\DragInterface.h"
#include "Operations\FilesToCBEdit.h"
#include "Operations\BtnBar.h"
#include "Operations\MyShell\MyShell.h"
//#include "Operations\MyShell\MyButtonC++.h"
#include "WindowsManagmentInstrumentation.h"
#include "DiskToolbar.h"
//#include "strsafe.h"
//#include "ole2.h"

extern HFONT hfCmdBtns;

HWND btns[7];//MyButton btns[7];
HWND sortBtns[MAX_PANELS][3],sheetTab[MAX_PANELS];
int panelHeight[MAX_PANELS],pathAndNamesEditY[MAX_PANELS],sortBtnsY[MAX_PANELS],
	selFilesInfoEditY,cmndsComboY,panelsTop;
const int SHEET_TAB_HEIGHT = 22;

namespace CmnCntrl
{
	DropTarget* mDropTarget;//[MAX_PANELS];//={NULL,NULL,NULL,NULL,NULL};
	int  cmndsCBEdtCursorPos = 0;

//HPEN hPenBlue=NULL;

//int TabCtrl_GetRowCount(HWND hTab)
//{
/*	int tot =	TabCtrl_GetItemCount(hTab);
	RECT r[2];

	if(!TabCtrl_GetItemRect(hTab,0,&r[0]))
		return 0;

	if(!TabCtrl_GetItemRect(hTab,tot-1,&r[1]))
		return 0;
	return (r[1].bottom - r[0].bottom)/SHEET_TAB_HEIGHT;*/

	//TabCtrl_SetMinTabWidth(hTab,-1);
//	return TabCtrl_GetRowCount(hTab);
//}
	
BOOL CmndsCBMsgQueue(MSG *msg)
{	
wchar_t s[MAX_PATH];
	switch(msg->message)
	{	case WM_KEYDOWN:
			switch(msg->wParam)
			{	case 38://Up
					//panel[0].nRows
					int it;BOOL bScrl;bScrl=FALSE;
					if(0==panel[0].bItemsViewType)
					{int col;col = (panel[0].GetItemMaxWidth()>0)?((1-panel[0].GetScrollOffst()/panel[0].GetItemMaxWidth())):0;
					 if(col>panel[0].GetNColumns())col=panel[0].GetNColumns();
					 --col;
					 it = col*panel[0].GetNRows()+panel[0].GetNRows()-1;
					 if(it>panel[0].GetTotItems()-1)
					 {	it=panel[0].GetTotItems()-1;
				 		bScrl=TRUE;
					}}
					else it = panel[0].totItems-1;
				    if(it<0)it=0;
					if(socketCl!=panel[0].GetEntry()->GetCrntRecType())
						panel[0].SetFocus();
					panel[0].SetHot(it);
					if(bScrl)panel[0].ScrollItemToView(it);
					break;
				case 40://Dwn
					break;
				case 13://Enter
Ent:				int sh;sh = GetKeyState(VK_LCONTROL);
					if(sh & 0x8000)
					{	int h = panel[Panel::iActivePanel].GetHot();
						if(h<0) h =0;
						if(h>panel[Panel::iActivePanel].GetTotItems()) h=0;
						InsertToCmndLine(&panel[Panel::iActivePanel], h);
					}
					else
					{	int CBCnt = (int)SendMessage(cmndsCB,CB_GETCOUNT,0,0);
						while(CBCnt > 39)
						{	SendMessage(cmndsCB,CB_DELETESTRING,0,0);
							CBCnt = (int)SendMessage(cmndsCB,CB_GETCOUNT,0,0);
						}
						if(CB_ERR!=CBCnt)
						{	if(GetWindowText(cmndsCBEdt,s,MAX_PATH))
							CBToDisk::AddToCB(cmndsCB,s);//SendMessage(cmndsCB,CB_INSERTSTRING,0,(LPARAM)s);
						}
						Execution::exeFrCmndCB(s);
						MSG msg;//WM_CHAR ni tiqadi,bo'lmasa!!!!!!
						PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE);
					}
					return 0;//break;
				default:
					if(panel[Panel::iActivePanel].totSelects<1 &&
					  (panel[Panel::iActivePanel].iHot<0 || 
							panel[Panel::iActivePanel].iHot>panel[Panel::iActivePanel].totItems-1))
						break;
					int bShftPressed = 0; sh = GetKeyState(VK_LSHIFT);
					if(sh & 0x8000) bShftPressed = 1;
					else
					{	sh = GetKeyState(VK_RSHIFT);
						if(sh & 0x8000) bShftPressed = 1;
					}
					int bCtrlPressed = 0; sh = GetKeyState(VK_LCONTROL);
					if(sh & 0x8000) bCtrlPressed = 1;
					else
					{	sh = GetKeyState(VK_RCONTROL);
						if(sh & 0x8000) bCtrlPressed = 1;
					}					
					int bAltPressed = 0; sh = GetKeyState(VK_MENU);
					if(sh & 0x8000) bAltPressed = 1;
					conf::TKey ik((int)msg->wParam,bCtrlPressed,bAltPressed,bShftPressed);
					int key = *((int*)&ik);
					__int32* pk=(int*)&conf::Dlg.keys[0];
					if( key==pk[0] ||//Ctrl + A
						key==pk[1] ||//Ctrl + T
						key==pk[2] ||//Ctrl + W
						key==pk[4] ||//Ctrl + F1
						key==pk[5] ||//Ctrl + F2
						key==pk[6] ||//Ctrl + F3
						key==pk[7] ||//Ctrl + F4
						key==pk[8] ||//F2
						key==pk[9] ||//F3 View
						key==pk[26] ||//Exit Alt+F4
						key==pk[11] ||//F5 Copy
						key==pk[12] ||//F6 Renmove
						key==pk[13] ||//Alt+F7 Search
						key==pk[14] ||//F7 CreateFolder
						//key==pk[15] ||//Backspace
						key==pk[30] ||//Enter
						key==pk[43] ||//Cntrl + Enter
						key==pk[29] ||//Properties
						key==pk[18] ||//Insert
						key==pk[17] ||//Space
						//key==pk[39] ||//Delete
						key==pk[16])//Fast select, +
						//key==pk[41])//Shift Delete
					panel[Panel::iActivePanel].OnKeybPressed(msg->wParam,msg->lParam);
					break;
			}
			SendMessage(cmndsCBEdt,EM_GETSEL,NULL,(WPARAM)&cmndsCBEdtCursorPos);
			break;
		case WM_CHAR:
			GetWindowText(cmndsCBEdt,s,MAX_PATH-1);
			wchar_t *p = wcsrchr(s,' ');
			if(!(p && 0==*(p+1)))
			{	int r=FilesToCBEdit::Attach(cmndsCB,cmndsCBEdt,FALSE,FALSE,&s[0],(int)(p?(p-&s[0]+1):0));//not for only dir
				SetFocus(cmndsCBEdt);
				SendMessage(cmndsCBEdt,EM_SETSEL,-1,-1);
				if(1==r)
					goto Ent;
			}
			return 0;
	}
	return TRUE;
}

VOID InsertToCmndLine(Panel* p, int id)
{
	wchar_t s[2][MAX_PATH];
	int ln = GetWindowText(cmndsCBEdt,s[0],MAX_PATH);
	if(ln)
	{	
		if(ln>=cmndsCBEdtCursorPos)
		{	int i,k;for(i=0; i<cmndsCBEdtCursorPos; i++)
				s[1][i]=s[0][i];
			if(' ' != s[1][i-1])
				s[1][i++] = ' ';
			for(k=i; k<i+p->GetItem(id)->NameCharLen+(p->GetItem(id)->ExtCharLen ? p->GetItem(id)->ExtCharLen+1:0); k++)
				s[1][k]=p->GetItem(id)->Name[k-i];
			for(i=k; i<=k+ln-cmndsCBEdtCursorPos; i++)
				s[1][i]=s[0][cmndsCBEdtCursorPos+i-k];
			cmndsCBEdtCursorPos += p->GetItem(id)->NameCharLen;
			if(p->GetItem(id)->ExtCharLen)
				cmndsCBEdtCursorPos += p->GetItem(id)->ExtCharLen+1;
		}
		SetWindowText(cmndsCBEdt,s[1]);
		SendMessage(cmndsCBEdt,EM_SETSEL,cmndsCBEdtCursorPos,cmndsCBEdtCursorPos);
	}
	else
	{	SetWindowText(cmndsCBEdt,p->GetItem(id)->Name);
		cmndsCBEdtCursorPos += p->GetItem(id)->NameCharLen;
		if(p->GetItem(id)->ExtCharLen)
			cmndsCBEdtCursorPos += p->GetItem(id)->ExtCharLen+1;
		SendMessage(cmndsCBEdt,EM_SETSEL,cmndsCBEdtCursorPos,cmndsCBEdtCursorPos);
}	}

BOOL CreatePanel(HWND wnd, int iPanel, int *y, int height)
{
//Agar C:\* pathda bo'lsa, myWMI b-n bir teksirib ko'rish kerak:
for(int i=0; i<MAX_PANELS; i++)
{	if(!IsDirExist(conf::panel_path[i]))
	{	for(int k=1; k<32 ; k++)
		{	if(myWMI::logDrNames[k][0])
			{	conf::panel_path[i][0]=myWMI::logDrNames[k][0];
				conf::panel_path[i][1]=':';conf::panel_path[i][2]='\\';
				conf::panel_path[i][3]='*';conf::panel_path[i][4]=0;
				break;
}	}	}	}

int w,width = rcClient.right - rcClient.left;
switch(conf::Dlg.iTotPanels)
	{	case 1:
			if(!panel[0].Create(hInst,
								wnd,
								0,
								2,
								*y,
								width-4,
								height,
								conf::panel_path[0]))
				return FALSE;
		break;
		case 2: default:
 			w = (width-9)/2;
			switch(iPanel)
			{	case 0:
					if(!panel[0].Create(hInst,
										wnd,
										0,
										2,
										*y,
										w + (int)(w*0.02f*(conf::resizeBarPersnt[0]-50.0f)),
										height,
										conf::panel_path[0]))
						return FALSE;
					break;
				case 1:
					if(!panel[1].Create(hInst,
										wnd,
										1,
										w+7 + (int)(w*0.02f*(conf::resizeBarPersnt[0]-50.0f)),
										*y,
										w - (int)(w*0.02f*(conf::resizeBarPersnt[0]-50.0f)),
										height,
										conf::panel_path[1]))
					return FALSE;
				break;
			}
		break;
		case 3:
			w = (width-14)/3;
			switch(iPanel)
			{	case 0:
					if(!panel[0].Create(hInst,
										wnd,
										0,
										2,
										*y,
										w + (int)(w*0.03f*(conf::resizeBarPersnt[0]-33.33f)),
										height,
										conf::panel_path[0]))
					return FALSE;
					break;
			case 1:
				if(!panel[1].Create(hInst,
									wnd,
									1,
									7 + (int)(w*0.03f*conf::resizeBarPersnt[0]),
									*y,
									(int)(w*0.03f*(conf::resizeBarPersnt[1]-conf::resizeBarPersnt[0])),
									height,
									conf::panel_path[1]))
					return FALSE;
				break;
			case 2:
				if(!panel[2].Create(hInst,
									wnd,
									2,
									9 + (int)(w*0.03f*conf::resizeBarPersnt[1]),
									*y,
									(int)(w*0.03f*(100.0f-conf::resizeBarPersnt[1])),
									height,
									conf::panel_path[2]))
					return FALSE;
				break;
			}
		break;
		case 4:
			//w = (width-25)/4;
			switch(iPanel)
			{	case 0:
					if(!panel[0].Create(hInst,
										wnd,
										0,
										2,
										*y,
										(int)(0.01f*conf::resizeBarPersnt[0]*(width-19.0f)),
										height,
										conf::panel_path[0]))
						return FALSE;
					break;
				case 1:
					if(!panel[1].Create(hInst,
										wnd,
										1,
										7 + (int)(0.01f*conf::resizeBarPersnt[0]*(width-19)),
										*y,
										(int)(0.01f*(conf::resizeBarPersnt[1]-conf::resizeBarPersnt[0])*(width-25.0f)),
										height,
										conf::panel_path[1]))
						return FALSE;
					break;
				case 2:
					if(!panel[2].Create(hInst,
										wnd,
										2,
										12 + (int)(0.01f*conf::resizeBarPersnt[1]*(width-19)),
										*y,
										(int)(0.01f*(conf::resizeBarPersnt[2]-conf::resizeBarPersnt[1])*(width-25.0f)),
										height,
										conf::panel_path[2]))
						return FALSE;
					break;
				case 3:
					if(!panel[3].Create(hInst,
										wnd,
										3,
										17 + (int)(0.01f*conf::resizeBarPersnt[2]*(width-19)),
										*y,
										(int)(0.01f*(100.0f-conf::resizeBarPersnt[2])*(width-19.0f)),
										height,
										conf::panel_path[3]))
						return FALSE;
					break;
			}
		break;
	}
	*y += height;
	return TRUE;
}

BOOL CreatePanelSelFilesInfoEdits(HWND wnd,int *y)
{
//profilerOutStr("Beg of CreatePanelSelFilesInfoEdits")

int w,width = rcClient.right - rcClient.left;
	switch(conf::Dlg.iTotPanels)
	{	case 1:
			panel[0].selFilesInfoEdt = CreateWindowEx(WS_EX_CLIENTEDGE,
								L"EDIT",
                                NULL,
                                WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY | ES_LEFT,
                                2,
								*y,
								width-4,
								INFO_EDT_HEIGHT,
                                wnd,
                                NULL,
                                hInst,
                                NULL);
			if(NULL==panel[0].selFilesInfoEdt) return FALSE;
			*y += INFO_EDT_HEIGHT;
		break;
		case 2: default:
			w = (width-9)/2;
			panel[0].selFilesInfoEdt = CreateWindowEx(WS_EX_CLIENTEDGE,
								L"EDIT",
                                NULL,
                                WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY | ES_LEFT,
                                2,
								*y,
								w+ (int)(w*0.02f*(conf::resizeBarPersnt[0]-50.0f)),
								INFO_EDT_HEIGHT,
                                wnd,
                                NULL,
                                hInst,
                                NULL);
			if(NULL==panel[0].selFilesInfoEdt) return FALSE;
			panel[1].selFilesInfoEdt = CreateWindowEx(WS_EX_CLIENTEDGE,
								L"EDIT",
                                NULL,
                                WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY | ES_LEFT,
                                w+7 + (int)(w*0.02f*(conf::resizeBarPersnt[0]-50.0f)),
								*y,
								w - (int)(w*0.02f*(conf::resizeBarPersnt[0]-50.0f)),
								INFO_EDT_HEIGHT,
                                wnd,
                                NULL,
                                hInst,
                                NULL);
			if(NULL==panel[1].selFilesInfoEdt) return FALSE;
			*y += INFO_EDT_HEIGHT;
		break;
		case 3:
			w = (width-14)/3;
			panel[0].selFilesInfoEdt = CreateWindowEx(WS_EX_CLIENTEDGE,
								L"EDIT",
                                NULL,
                                WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY | ES_LEFT,
                                2,
								*y,
								w + (int)(w*0.03f*(conf::resizeBarPersnt[0]-33.33f)),
								INFO_EDT_HEIGHT,
                                wnd,
                                NULL,
                                hInst,
                                NULL);
			if(NULL==panel[0].selFilesInfoEdt) return FALSE;
			panel[1].selFilesInfoEdt = CreateWindowEx(WS_EX_CLIENTEDGE,
								L"EDIT",
                                NULL,
                                WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY | ES_LEFT,
                                7 + (int)(w*0.03f*conf::resizeBarPersnt[0]),
								*y,
								(int)(w*0.03f*(conf::resizeBarPersnt[1]-conf::resizeBarPersnt[0])),
								INFO_EDT_HEIGHT,
                                wnd,
                                NULL,
                                hInst,
                                NULL);
			if(NULL==panel[1].selFilesInfoEdt) return FALSE;
			panel[2].selFilesInfoEdt = CreateWindowEx(WS_EX_CLIENTEDGE,
								L"EDIT",
                                NULL,
                                WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY | ES_LEFT,
                                12 + (int)(w*0.03f*conf::resizeBarPersnt[1]),
								*y,
								(int)(w*0.03f*(100.0f-conf::resizeBarPersnt[1])),
								INFO_EDT_HEIGHT,
                                wnd,
                                NULL,
                                hInst,
                                NULL);
			if(NULL==panel[2].selFilesInfoEdt) return FALSE;
			*y += INFO_EDT_HEIGHT;
		break;
		case 4:
			w = (width-19)/4;
			panel[0].selFilesInfoEdt = CreateWindowEx(WS_EX_CLIENTEDGE,
								L"EDIT",
                                NULL,
                                WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY | ES_LEFT,
                                2,
								*y,
								(int)(0.01f*conf::resizeBarPersnt[0]*(width-19.0f)),
								INFO_EDT_HEIGHT,
                                wnd,
                                NULL,
                                hInst,
                                NULL);
			if(NULL==panel[0].selFilesInfoEdt) return FALSE;
			panel[1].selFilesInfoEdt = CreateWindowEx(WS_EX_CLIENTEDGE,
								L"EDIT",
                                NULL,
                                WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY | ES_LEFT,
								7 + (int)(0.01f*conf::resizeBarPersnt[0]*(width-19)),
								*y,
								(int)(0.01f*(conf::resizeBarPersnt[1]-conf::resizeBarPersnt[0])*(width-25.0f)),
								INFO_EDT_HEIGHT,
                                wnd,
                                NULL,
                                hInst,
                                NULL);
			if(NULL==panel[1].selFilesInfoEdt) return FALSE;
			panel[2].selFilesInfoEdt = CreateWindowEx(WS_EX_CLIENTEDGE,
								L"EDIT",
                                NULL,
                                WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY | ES_LEFT,
								12 + (int)(0.01f*conf::resizeBarPersnt[1]*(width-19)),
								*y,
								(int)(0.01f*(conf::resizeBarPersnt[2]-conf::resizeBarPersnt[1])*(width-19.0f)),
								INFO_EDT_HEIGHT,
                                wnd,
                                NULL,
                                hInst,
                                NULL);
			if(NULL==panel[2].selFilesInfoEdt) return FALSE;
			panel[3].selFilesInfoEdt = CreateWindowEx(WS_EX_CLIENTEDGE,
								L"EDIT",
                                NULL,
                                WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY | ES_LEFT,
								17 + (int)(0.01f*conf::resizeBarPersnt[2]*(width-19)),
								*y,
								(int)(0.01f*(100.0f-conf::resizeBarPersnt[2])*(width-19.0f)),
								INFO_EDT_HEIGHT,
                                wnd,
                                NULL,
                                hInst,
                                NULL);
			if(NULL==panel[3].selFilesInfoEdt) return FALSE;
			*y += INFO_EDT_HEIGHT;
		break;
	}
	return TRUE;
}

BOOL CreatePatnAndNameEdit(HWND wnd, int iPanel, int *y)
{
//profilerOutStr("Beg of CreatePanelSelFilesInfoEdits")
COMBOBOXINFO ci;ci.cbSize=sizeof(COMBOBOXINFO);

int w,width = rcClient.right - rcClient.left;
	switch(conf::Dlg.iTotPanels)
	{	case 1:
			panel[0].pathAndNameEdt[0] = CreateWindowEx(WS_EX_CLIENTEDGE,
								L"COMBOBOX",
                                NULL,
                                CBS_AUTOHSCROLL|CBS_DROPDOWN|WS_CHILD|WS_VISIBLE|WS_VSCROLL,
                                2,
								*y,
								width-4,
								380,
                                wnd,
                                (HMENU)pathAndNameEdtID0,
                                hInst,
                                NULL);
			if(NULL==panel[0].pathAndNameEdt[0]) return FALSE;
			if(GetComboBoxInfo(panel[0].pathAndNameEdt[0],&ci))
				panel[0].pathAndNameEdt[1] = ci.hwndItem;
	
			SendMessage(panel[0].pathAndNameEdt[0],CB_RESETCONTENT,0,0);
		break;
		case 2: default:
			w = (width-9)/2;
			switch(iPanel)
			{	case 0:
					panel[0].pathAndNameEdt[0] = CreateWindowEx(WS_EX_CLIENTEDGE,
										L"COMBOBOX",
										NULL,
										CBS_AUTOHSCROLL|CBS_DROPDOWN|WS_CHILD|WS_VISIBLE|WS_VSCROLL,
										2,
										*y,
										w+ (int)(w*0.02f*(conf::resizeBarPersnt[0]-50.0f)),
										380,
										wnd,
										(HMENU)pathAndNameEdtID0,
										hInst,
										NULL);
					if(NULL==panel[0].pathAndNameEdt[0]) return FALSE;
					if(GetComboBoxInfo(panel[0].pathAndNameEdt[0],&ci))
						panel[0].pathAndNameEdt[1] = ci.hwndItem;
			
					SendMessage(panel[0].pathAndNameEdt[0],CB_RESETCONTENT,0,0);
					break;
				case 1:
					panel[1].pathAndNameEdt[0] = CreateWindowEx(WS_EX_CLIENTEDGE,
										L"COMBOBOX",
										NULL,
										CBS_AUTOHSCROLL|CBS_DROPDOWN|WS_CHILD|WS_VISIBLE|WS_VSCROLL,
										w+7 + (int)(w*0.02f*(conf::resizeBarPersnt[0]-50.0f)),
										*y,
										w - (int)(w*0.02f*(conf::resizeBarPersnt[0]-50.0f)),
										380,
										wnd,
										(HMENU)pathAndNameEdtID1,
										hInst,
										NULL);
					if(NULL==panel[1].pathAndNameEdt[0]) return FALSE;
					if(GetComboBoxInfo(panel[1].pathAndNameEdt[0],&ci))
						panel[1].pathAndNameEdt[1] = ci.hwndItem;
			
					SendMessage(panel[1].pathAndNameEdt[0],CB_RESETCONTENT,0,0);
					break;
			}
		break;
		case 3:
			w = (width-14)/3;
			switch(iPanel)
			{	case 0:
					panel[0].pathAndNameEdt[0] = CreateWindowEx(WS_EX_CLIENTEDGE,
										L"COMBOBOX",
										NULL,
										CBS_AUTOHSCROLL|CBS_DROPDOWN|WS_CHILD|WS_VISIBLE|WS_VSCROLL,
										2,
										*y,
										w + (int)(w*0.03f*(conf::resizeBarPersnt[0]-33.33f)),
										380,
										wnd,
										(HMENU)pathAndNameEdtID0,
										hInst,
										NULL);
					if(NULL==panel[0].pathAndNameEdt[0]) return FALSE;
					if(GetComboBoxInfo(panel[0].pathAndNameEdt[0],&ci))
						panel[0].pathAndNameEdt[1] = ci.hwndItem;
			
					SendMessage(panel[0].pathAndNameEdt[0],CB_RESETCONTENT,0,0);
					break;
			case 1:
				panel[1].pathAndNameEdt[0] = CreateWindowEx(WS_EX_CLIENTEDGE,
									L"COMBOBOX",
									NULL,
									CBS_AUTOHSCROLL|CBS_DROPDOWN|WS_CHILD|WS_VISIBLE|WS_VSCROLL,
									7 + (int)(w*0.03f*conf::resizeBarPersnt[0]),
									*y,
									(int)(w*0.03f*(conf::resizeBarPersnt[1]-conf::resizeBarPersnt[0])),
									380,
									wnd,
									(HMENU)pathAndNameEdtID1,
									hInst,
									NULL);
				if(NULL==panel[1].pathAndNameEdt[0]) return FALSE;
				if(GetComboBoxInfo(panel[1].pathAndNameEdt[0],&ci))
					panel[1].pathAndNameEdt[1] = ci.hwndItem;
		
				SendMessage(panel[1].pathAndNameEdt[0],CB_RESETCONTENT,0,0);
				break;
			case 2:
				panel[2].pathAndNameEdt[0] = CreateWindowEx(WS_EX_CLIENTEDGE,
									L"COMBOBOX",
									NULL,
									CBS_AUTOHSCROLL|CBS_DROPDOWN|WS_CHILD|WS_VISIBLE|WS_VSCROLL,
									12 + (int)(w*0.03f*conf::resizeBarPersnt[1]),
									*y,
									(int)(w*0.03f*(100.0f-conf::resizeBarPersnt[1])),
									380,
									wnd,
									(HMENU)pathAndNameEdtID2,
									hInst,
									NULL);
				if(NULL==panel[2].pathAndNameEdt[0]) return FALSE;
				if(GetComboBoxInfo(panel[2].pathAndNameEdt[0],&ci))
					panel[2].pathAndNameEdt[1] = ci.hwndItem;
		
				SendMessage(panel[2].pathAndNameEdt[0],CB_RESETCONTENT,0,0);
				break;
			}
		break;
		case 4:
			w = (width-19)/4;
			switch(iPanel)
			{	case 0:
					panel[0].pathAndNameEdt[0] = CreateWindowEx(WS_EX_CLIENTEDGE,
										L"COMBOBOX",
										NULL,
										CBS_AUTOHSCROLL|CBS_DROPDOWN|WS_CHILD|WS_VISIBLE|WS_VSCROLL,
										2,
										*y,
										(int)(0.01f*conf::resizeBarPersnt[0]*(width-19.0f)),
										380,
										wnd,
										(HMENU)pathAndNameEdtID0,
										hInst,
										NULL);
					if(NULL==panel[0].pathAndNameEdt[0]) return FALSE;
					if(GetComboBoxInfo(panel[0].pathAndNameEdt[0],&ci))
						panel[0].pathAndNameEdt[1] = ci.hwndItem;
			
					SendMessage(panel[0].pathAndNameEdt[0],CB_RESETCONTENT,0,0);
					break;
			case 1:
				panel[1].pathAndNameEdt[0] = CreateWindowEx(WS_EX_CLIENTEDGE,
									L"COMBOBOX",
									NULL,
									CBS_AUTOHSCROLL|CBS_DROPDOWN|WS_CHILD|WS_VISIBLE|WS_VSCROLL,
									7 + (int)(0.01f*conf::resizeBarPersnt[0]*(width-19)),
									*y,
									(int)(0.01f*(conf::resizeBarPersnt[1]-conf::resizeBarPersnt[0])*(width-25.0f)),
									380,
									wnd,
									(HMENU)pathAndNameEdtID1,
									hInst,
									NULL);
				if(NULL==panel[1].pathAndNameEdt[0]) return FALSE;
				if(GetComboBoxInfo(panel[1].pathAndNameEdt[0],&ci))
					panel[1].pathAndNameEdt[1] = ci.hwndItem;
		
				SendMessage(panel[1].pathAndNameEdt[0],CB_RESETCONTENT,0,0);
				break;
			case 2:
				panel[2].pathAndNameEdt[0] = CreateWindowEx(WS_EX_CLIENTEDGE,
									L"COMBOBOX",
									NULL,
									CBS_AUTOHSCROLL|CBS_DROPDOWN|WS_CHILD|WS_VISIBLE|WS_VSCROLL,
									12 + (int)(0.01f*conf::resizeBarPersnt[1]*(width-19)),
									*y,
									(int)(0.01f*(conf::resizeBarPersnt[2]-conf::resizeBarPersnt[1])*(width-19.0f)),
									380,
									wnd,
									(HMENU)pathAndNameEdtID2,
									hInst,
									NULL);
				if(NULL==panel[2].pathAndNameEdt[0]) return FALSE;
				if(GetComboBoxInfo(panel[2].pathAndNameEdt[0],&ci))
					panel[2].pathAndNameEdt[1] = ci.hwndItem;
		
				SendMessage(panel[2].pathAndNameEdt[0],CB_RESETCONTENT,0,0);
				break;
			case 3:
				panel[3].pathAndNameEdt[0] = CreateWindowEx(WS_EX_CLIENTEDGE,
									L"COMBOBOX",
									NULL,
									CBS_AUTOHSCROLL|CBS_DROPDOWN|WS_CHILD|WS_VISIBLE|WS_VSCROLL,
									17 + (int)(0.01f*conf::resizeBarPersnt[2]*(width-19)),
									*y,
									(int)(0.01f*(100.0f-conf::resizeBarPersnt[2])*(width-19.0f)),
									380,
									wnd,
									(HMENU)pathAndNameEdtID3,
									hInst,
									NULL);
				if(NULL==panel[3].pathAndNameEdt[0]) return FALSE;
				if(GetComboBoxInfo(panel[3].pathAndNameEdt[0],&ci))
					panel[3].pathAndNameEdt[1] = ci.hwndItem;
		
				SendMessage(panel[3].pathAndNameEdt[0],CB_RESETCONTENT,0,0);
				break;
			}
		break;
	}
	*y += INFO_EDT_HEIGHT;
	return TRUE;
}

BOOL CreatePanelResizeBars(HWND wnd,int y)//bottom
{
//profilerOutStr("Beg of CreatePanelresizeBar")

int w,width = rcClient.right - rcClient.left;
	switch(conf::Dlg.iTotPanels)
	{	case 1:
		break;
		case 2: default:
			w = (width-15)/2;
			if(!resizeBar[0].Create(hInst,wnd,0,
								w+6 + (int)(w*0.02f*(conf::resizeBarPersnt[0]-50.0f)),
								y-SHEET_TAB_HEIGHT-(panelHeight[0]>panelHeight[1]?panelHeight[0]:panelHeight[1]),
								3,
								panelHeight[0]>panelHeight[1]?panelHeight[0]:panelHeight[1]))
				return FALSE;
		break;
		case 3:
			w = (width-20)/3;
			if(!resizeBar[0].Create(hInst,wnd,0,
								6 + (int)(w*0.03f*conf::resizeBarPersnt[0]),
								y-SHEET_TAB_HEIGHT-(panelHeight[0]>panelHeight[1]?panelHeight[0]:panelHeight[1]),
								3,
								panelHeight[0]>panelHeight[1]?panelHeight[0]:panelHeight[1]))
				return FALSE;
			if(!resizeBar[1].Create(hInst,wnd,1,
								11 + (int)(w*0.03f*conf::resizeBarPersnt[1]),
								y-SHEET_TAB_HEIGHT-(panelHeight[1]>panelHeight[2]?panelHeight[1]:panelHeight[2]),
								3,
								panelHeight[1]>panelHeight[2]?panelHeight[1]:panelHeight[2]))
				return FALSE;
		break;
		case 4:
			//w = (width-25)/4;
			if(!resizeBar[0].Create(hInst,wnd,0,
								6 + (int)(0.01f*conf::resizeBarPersnt[0]*(width-25)),
								y-SHEET_TAB_HEIGHT-(panelHeight[0]>panelHeight[1]?panelHeight[0]:panelHeight[1]),
								3,
								panelHeight[0]>panelHeight[1]?panelHeight[0]:panelHeight[1]))
				return FALSE;
			if(!resizeBar[1].Create(hInst,wnd,1,
								11 + (int)(0.01f*conf::resizeBarPersnt[1]*(width-25)),
								y-SHEET_TAB_HEIGHT-(panelHeight[1]>panelHeight[2]?panelHeight[1]:panelHeight[2]),
								3,
								panelHeight[1]>panelHeight[2]?panelHeight[1]:panelHeight[2]))
				return FALSE;
			if(!resizeBar[2].Create(hInst,wnd,2,
								16 + (int)(0.01f*conf::resizeBarPersnt[2]*(width-25)),
								y-SHEET_TAB_HEIGHT-(panelHeight[2]>panelHeight[3]?panelHeight[2]:panelHeight[3]),
								3,
								panelHeight[2]>panelHeight[3]?panelHeight[2]:panelHeight[3]))
				return FALSE;
		break;
	}
	return TRUE;
}

BOOL CreateCmndButtons(HWND wnd,int *y)
{
wchar_t s[MAX_PATH];
int width = rcClient.right - rcClient.left,
	height = rcClient.bottom - rcClient.top;
	int w = (width - 7*2)/7;
	int h = 20; if(height<70 && height > 50) h = height+20-70; else if(height<50) h = 0;

	BOOL bXP=TRUE;
	OSVERSIONINFOEX os;os.dwOSVersionInfoSize=sizeof(OSVERSIONINFOEX);
	if(GetVersionEx((LPOSVERSIONINFO)&os))
	{	if(os.dwMajorVersion>5)// || 0==os.dwMinorVersion)
			bXP = FALSE;
	}

	int x=2;DWORD style = WS_CHILDWINDOW | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON | BS_TEXT;
//#ifndef _WIN64
	if(!bXP) style |= BS_SPLITBUTTON;
//#else
//	if(!bXP) style |= 0x0000000DL;
//#endif
	for(int b=0; b<7; b++)//Pastdagi knopkalar:
	{	int ln;
		switch(b)
		{	case 0:
				ln=conf::Dlg.KeyToText(9,s); s[ln++]=' ';
				LoadString(hInst,IDS_STRINGSW_156,&s[ln],MAX_PATH-1);
				break;
			case 1:
				ln=conf::Dlg.KeyToText(10,s); s[ln++]=' ';
				LoadString(hInst,IDS_STRINGSW_157,&s[ln],MAX_PATH-1);
				break;
			case 2:
				ln=conf::Dlg.KeyToText(11,s); s[ln++]=' ';
				LoadString(hInst,IDS_STRINGSW_36,&s[ln],MAX_PATH-1);
				for(wchar_t *ps=&s[0]; *ps; ps++)if(':'==(*ps)){*ps=0;break;};
				break;
			case 3:
				style=WS_CHILDWINDOW | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON | BS_TEXT;
				ln=conf::Dlg.KeyToText(12,s); s[ln++]=' ';
				LoadString(hInst,IDS_STRINGSW_176,&s[ln],MAX_PATH-1);
				break;
			case 4:
				ln=conf::Dlg.KeyToText(13,s); s[ln++]=' ';
				LoadString(hInst,IDS_STRINGSW_141,&s[ln],MAX_PATH-1);
				break;
			case 5:
				style=WS_CHILDWINDOW | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON | BS_TEXT;
//#ifndef _WIN64
				if(!bXP) style |= BS_SPLITBUTTON;
//#else
//				if(!bXP) style |= 0x0000000DL;
//#endif _WIN64
				ln=conf::Dlg.KeyToText(14,s); s[ln++]=' ';
				LoadString(hInst,IDS_STRINGSW_203,&s[ln],MAX_PATH-1);
				break;
			case 6:
				style=WS_CHILDWINDOW | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON | BS_TEXT;
				ln=conf::Dlg.KeyToText(26,s); s[ln++]=' ';
				LoadString(hInst,IDS_STRINGSW_2,&s[ln],MAX_PATH-1);
				break;
		}
		btns[b] = CreateWindowEx(WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR | WS_EX_NOPARENTNOTIFY,//WS_EX_CLIENTEDGE,
								 L"BUTTON",
								 s,
								 //conf::Dlg.bBtnsStdView ? 
								 style,//(WS_VISIBLE | WS_CHILD | BS_OWNERDRAW) : // | BS_PUSHBUTTON,//|BS_OWNERDRAW,
								 //(WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_PUSHLIKE),
								 x,
								 *y,
								 w,
								 h,
								 wnd,
								 (HMENU)(IDB_BTN0+b),
								 hInst,
								 NULL);
		//btns[b].Init(hWnd,s,IDB_BTN0+b,0,x,*y,w,h);
		//btns[b]=MyButtonGetHWND(MyButtonInit(hWnd,s,IDB_BTN0+b,conf::Dlg.iBtnsType,x,*y,w,h));
 		if(!btns[b]) return FALSE;
		ShowWindow(btns[b],SW_SHOW);
		x += w+2;
	}
	*y += h;
	return TRUE;
}

BOOL CreateCmndsCombo(HWND wnd, int *y)
{
int width=rcClient.right - rcClient.left;
	cmndsCB = CreateWindowEx(WS_EX_CLIENTEDGE,
							 L"ComboBox",
							 L"",
							 CBS_DROPDOWN | WS_CHILD | WS_VISIBLE,
							 2,
							 *y,
							 width-4,
							 250,
							 wnd,
							 (HMENU)(NULL),
							 hInst,
							 NULL);
	if(!cmndsCB) return FALSE;
	COMBOBOXINFO pcbi;pcbi.cbSize=sizeof(pcbi);
	if(!GetComboBoxInfo(cmndsCB,&pcbi)) return FALSE;
	cmndsCBEdt = pcbi.hwndItem;

	//Read strings, count total:
	cmnCmndsCB.Read(cmnCmndsCBFName,cmndsCB,40,MAX_PATH);
	*y += 25;
	return (cmndsCB!=NULL);
}

BOOL CreateSheetTabs(HWND wnd, int iPanel, int *y)
{
int w,x=2,width = rcClient.right - rcClient.left;
	switch(conf::Dlg.iTotPanels)
	{	case 1:
			sheetTab[0]=CreateWindow(WC_TABCONTROL,L"",
								WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE | TCS_MULTILINE | TCS_FLATBUTTONS | TCS_TOOLTIPS | TCS_FOCUSNEVER,
								2,
								*y,
								width-4,
								conf::iTabSheetRowCount[0]*(conf::iSheetTabs[0]?SHEET_TAB_HEIGHT:0),
								wnd,
								(HMENU)(ID_SHEET_TAB),
								hInst,
								NULL);
			if(!sheetTab[0]) return FALSE;
			*y += conf::iTabSheetRowCount[0]*(conf::iSheetTabs[0]?SHEET_TAB_HEIGHT:0);
		break;
		case 2: default:
 			w = (width-9)/2;
			switch(iPanel)
			{	case 0:
					sheetTab[0]=CreateWindow(WC_TABCONTROL,L"",
								WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE | TCS_MULTILINE | TCS_FLATBUTTONS | TCS_TOOLTIPS | TCS_FOCUSNEVER,
								2,
								*y,
								w + (int)(w*0.02f*(conf::resizeBarPersnt[0]-50.0f)),
								conf::iTabSheetRowCount[0]*(conf::iSheetTabs[0]?SHEET_TAB_HEIGHT:0),
								wnd,
								(HMENU)(ID_SHEET_TAB),
								hInst,
								NULL);
					if(!sheetTab[0]) return FALSE;
					*y += conf::iTabSheetRowCount[0]*(conf::iSheetTabs[0]?SHEET_TAB_HEIGHT:0);
					break;
				case 1:
					sheetTab[1]=CreateWindow(WC_TABCONTROL,L"",
								WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE | TCS_MULTILINE | TCS_FLATBUTTONS | TCS_TOOLTIPS | TCS_FOCUSNEVER,
								w+7 + (int)(w*0.02f*(conf::resizeBarPersnt[0]-50.0f)),
								*y,
								w - (int)(w*0.02f*(conf::resizeBarPersnt[0]-50.0f)),
								conf::iTabSheetRowCount[1]*(conf::iSheetTabs[1]?SHEET_TAB_HEIGHT:0),
								wnd,
								(HMENU)(ID_SHEET_TAB+1),
								hInst,
								NULL);
					if(!sheetTab[1]) return FALSE;
					*y += conf::iTabSheetRowCount[1]*(conf::iSheetTabs[1]?SHEET_TAB_HEIGHT:0);
					break;
			}
		break;
		case 3:
			w = (width-14)/3;
			switch(iPanel)
			{	case 0:
					sheetTab[0]=CreateWindow(WC_TABCONTROL,L"",
								WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE | TCS_MULTILINE | TCS_FLATBUTTONS | TCS_TOOLTIPS | TCS_FOCUSNEVER,
								2,
								*y,
								w + (int)(w*0.03f*(conf::resizeBarPersnt[0]-33.33f)),
								conf::iTabSheetRowCount[0]*(conf::iSheetTabs[0]?SHEET_TAB_HEIGHT:0),
								wnd,
								(HMENU)(ID_SHEET_TAB),
								hInst,
								NULL);
					if(!sheetTab[0]) return FALSE;
					*y += conf::iTabSheetRowCount[0]*(conf::iSheetTabs[0]?SHEET_TAB_HEIGHT:0);
					break;
				case 1:
					sheetTab[1]=CreateWindow(WC_TABCONTROL,L"",
								WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE | TCS_MULTILINE | TCS_FLATBUTTONS | TCS_TOOLTIPS | TCS_FOCUSNEVER,
								7 + (int)(w*0.03f*conf::resizeBarPersnt[0]),
								*y,
								(int)(w*0.03f*(conf::resizeBarPersnt[1]-conf::resizeBarPersnt[0])),
								conf::iTabSheetRowCount[1]*(conf::iSheetTabs[1]?SHEET_TAB_HEIGHT:0),
								wnd,
								(HMENU)(ID_SHEET_TAB+1),
								hInst,
								NULL);
					if(!sheetTab[1]) return FALSE;
					*y += conf::iTabSheetRowCount[1]*(conf::iSheetTabs[1]?SHEET_TAB_HEIGHT:0);
					break;
				case 2:
					sheetTab[2]=CreateWindow(WC_TABCONTROL,L"",
								WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE | TCS_MULTILINE | TCS_FLATBUTTONS | TCS_TOOLTIPS | TCS_FOCUSNEVER,
								12 + (int)(w*0.03f*conf::resizeBarPersnt[1]),
								*y,
								(int)(w*0.03f*(100.0f-conf::resizeBarPersnt[1])),
								conf::iTabSheetRowCount[2]*(conf::iSheetTabs[2]?SHEET_TAB_HEIGHT:0),
								wnd,
								(HMENU)(ID_SHEET_TAB+2),
								hInst,
								NULL);
					if(!sheetTab[2]) return FALSE;
					*y += conf::iTabSheetRowCount[2]*(conf::iSheetTabs[2]?SHEET_TAB_HEIGHT:0);
					break;
			}
		break;
		case 4:
			switch(iPanel)
			{	case 0:
					sheetTab[0]=CreateWindow(WC_TABCONTROL,L"",
								WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE | TCS_MULTILINE | TCS_FLATBUTTONS | TCS_TOOLTIPS | TCS_FOCUSNEVER,
								2,
								*y,
								(int)(0.01f*conf::resizeBarPersnt[0]*(width-19.0f)),
								conf::iTabSheetRowCount[0]*(conf::iSheetTabs[0]?SHEET_TAB_HEIGHT:0),
								wnd,
								(HMENU)(ID_SHEET_TAB),
								hInst,
								NULL);
					if(!sheetTab[0]) return FALSE;
					*y += conf::iTabSheetRowCount[0]*(conf::iSheetTabs[0]?SHEET_TAB_HEIGHT:0);
					break;
				case 1:
					sheetTab[1]=CreateWindow(WC_TABCONTROL,L"",
								WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE | TCS_MULTILINE | TCS_FLATBUTTONS | TCS_TOOLTIPS | TCS_FOCUSNEVER,
								7 + (int)(0.01f*conf::resizeBarPersnt[0]*(width-19)),
								*y,
								(int)(0.01f*(conf::resizeBarPersnt[1]-conf::resizeBarPersnt[0])*(width-25.0f)),
								conf::iTabSheetRowCount[1]*(conf::iSheetTabs[1]?SHEET_TAB_HEIGHT:0),
								wnd,
								(HMENU)(ID_SHEET_TAB+1),
								hInst,
								NULL);
					if(!sheetTab[1]) return FALSE;
					*y += conf::iTabSheetRowCount[1]*(conf::iSheetTabs[1]?SHEET_TAB_HEIGHT:0);
					break;
				case 2:
					sheetTab[2]=CreateWindow(WC_TABCONTROL,L"",
								WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE | TCS_MULTILINE | TCS_FLATBUTTONS | TCS_TOOLTIPS | TCS_FOCUSNEVER,
								12 + (int)(0.01f*conf::resizeBarPersnt[1]*(width-19)),
								*y,
								(int)(0.01f*(conf::resizeBarPersnt[2]-conf::resizeBarPersnt[1])*(width-25.0f))/3,
								conf::iTabSheetRowCount[3]*(conf::iSheetTabs[2]?SHEET_TAB_HEIGHT:0),
								wnd,
								(HMENU)(ID_SHEET_TAB+2),
								hInst,
								NULL);
					if(!sheetTab[2]) return FALSE;
					*y += conf::iTabSheetRowCount[3]*(conf::iSheetTabs[2]?SHEET_TAB_HEIGHT:0);
					break;
				case 3:
					sheetTab[3]=CreateWindow(WC_TABCONTROL,L"",
								WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE | TCS_MULTILINE | TCS_FLATBUTTONS | TCS_TOOLTIPS | TCS_FOCUSNEVER,
								20 + (int)(0.01f*conf::resizeBarPersnt[2]*(width-25)),
								*y,
								(int)(0.01f*(100.0f-conf::resizeBarPersnt[2])*(width-25.0f)),
								conf::iTabSheetRowCount[4]*(conf::iSheetTabs[3]?SHEET_TAB_HEIGHT:0),
								wnd,
								(HMENU)(ID_SHEET_TAB+3),
								hInst,
								NULL);
					if(!sheetTab[3]) return FALSE;
					*y += conf::iTabSheetRowCount[4]*(conf::iSheetTabs[3]?SHEET_TAB_HEIGHT:0);
					break;
			}
		break;
	}
	return TRUE;
}

BOOL CreateSortButtons(HWND wnd, int iPanel, int *y)
{
int w,h,width,height;wchar_t s[MAX_PATH];
	width = rcClient.right - rcClient.left;
	height = rcClient.bottom - rcClient.top;
	h = 24; if(height<70 && height > 50) h = height+20-70; else if(height<50) h = 0;

	int x=2;
	switch(conf::Dlg.iTotPanels)
	{	case 1: for(int b=0; b<3; b++)		{
			if(!b) LoadString(hInst,IDS_STRINGSW_21,s,MAX_PATH);
			else  LoadString(hInst,IDS_STRINGSW_24+(b-1)*5,s,MAX_PATH);
			sortBtns[0][b]=CreateWindowEx(WS_EX_CLIENTEDGE,
								L"BUTTON",
								s,
								WS_VISIBLE | WS_CHILD,// | BS_PUSHBUTTON,
								2+b*(width-4)/3,
								*y,
								(width-4)/3,
								h,
								wnd,
								(HMENU)(IDB_BTN0+7+b),
								hInst,
								NULL);
			if(!sortBtns[0][b]) return FALSE;}
		break;
		case 2: default:
			w = (width-9)/2;
			switch(iPanel)
			{	case 0:
 					for(int b=0; b<3; b++)	{
					if(!b) LoadString(hInst,IDS_STRINGSW_21,s,MAX_PATH);
					else  LoadString(hInst,IDS_STRINGSW_24+(b-1)*5,s,MAX_PATH);
					sortBtns[0][b]=CreateWindowEx(WS_EX_CLIENTEDGE,L"BUTTON",
										s,
										WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,// | BS_PUSHBUTTON, WM_CTLCOLORBTN uchun, colorini chizmaydur;
										2+b*(w + (int)(w*0.02f*(conf::resizeBarPersnt[0]-50.0f)))/3,
										*y,
										(w + (int)(w*0.02f*(conf::resizeBarPersnt[0]-50.0f)))/3,
										h,
										wnd,
										(HMENU)(IDB_BTN0+7+b),
										hInst,
										NULL);
					if(!sortBtns[0][b]) return FALSE; }
					break;
				case 1:
 					for(int b=0; b<3; b++)	{
					if(!b) LoadString(hInst,IDS_STRINGSW_21,s,MAX_PATH);
					else  LoadString(hInst,IDS_STRINGSW_24+(b-1)*5,s,MAX_PATH);
					sortBtns[1][b]=CreateWindowEx(WS_EX_CLIENTEDGE,L"BUTTON",
										s,
										WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,// | BS_PUSHBUTTON,
										w+7 + (int)(w*0.02f*(conf::resizeBarPersnt[0]-50.0f))+b*(w - (int)(w*0.02f*(conf::resizeBarPersnt[0]-50.0f)))/3,
										*y,
										(w - (int)(w*0.02f*(conf::resizeBarPersnt[0]-50.0f)))/3,
										h,
										wnd,
										(HMENU)(IDB_BTN0+7+3+b),
										hInst,
										NULL);
					if(!sortBtns[1][b]) return FALSE;	}
					break;
			}
		break;
		case 3:
			w = (width-14)/3; 
			switch(iPanel)
			{	case 0:
					for(int b=0; b<3; b++)	{
					if(!b) LoadString(hInst,IDS_STRINGSW_21,s,MAX_PATH);
					else  LoadString(hInst,IDS_STRINGSW_24+(b-1)*5,s,MAX_PATH);
					sortBtns[0][b]=CreateWindowEx(WS_EX_CLIENTEDGE,L"BUTTON",
										s,
										WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,// | BS_PUSHBUTTON,
										5+b*(w + (int)(w*0.03f*(conf::resizeBarPersnt[0]-33.33f)))/3,
										*y,
										(w + (int)(w*0.03f*(conf::resizeBarPersnt[0]-33.33f)))/3,
										h,
										wnd,
										(HMENU)(IDB_BTN0+7+b),
										hInst,
										NULL);
					if(!sortBtns[0][b]) return FALSE;		}
					break;
				case 1:
					for(int b=0; b<3; b++)	{
					if(!b) LoadString(hInst,IDS_STRINGSW_21,s,MAX_PATH);
					else  LoadString(hInst,IDS_STRINGSW_24+(b-1)*5,s,MAX_PATH);
					sortBtns[1][b]=CreateWindowEx(WS_EX_CLIENTEDGE,L"BUTTON",
										s,
										WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,// | BS_PUSHBUTTON,
										7 + (int)(w*0.03f*conf::resizeBarPersnt[0])+b*(int)(w*0.03f*(conf::resizeBarPersnt[1]-conf::resizeBarPersnt[0]))/3,
										*y,
										(int)(w*0.03f*(conf::resizeBarPersnt[1]-conf::resizeBarPersnt[0]))/3,
										h,
										wnd,
										(HMENU)(IDB_BTN0+7+3+b),
										hInst,
										NULL);
					if(!sortBtns[1][b]) return FALSE;	}
					break;
				case 2:
					for(int b=0; b<3; b++)	{
					if(!b) LoadString(hInst,IDS_STRINGSW_21,s,MAX_PATH);
					else  LoadString(hInst,IDS_STRINGSW_24+(b-1)*5,s,MAX_PATH);
					sortBtns[2][b]=CreateWindowEx(WS_EX_CLIENTEDGE,L"BUTTON",
										s,
										WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,// | BS_PUSHBUTTON,
										9 + (int)(w*0.03f*conf::resizeBarPersnt[1])+b*(int)(w*0.03f*(100.0f-conf::resizeBarPersnt[1]))/3,
										*y,
										(int)(w*0.03f*(100.0f-conf::resizeBarPersnt[1]))/3,
										h,
										wnd,
										(HMENU)(IDB_BTN0+7+6+b),
										hInst,
										NULL);
					if(!sortBtns[2][b]) return FALSE;	}
					break;
			}
		break;
		case 4:
			//w = (width-25)/4;
			switch(iPanel)
			{	case 0:
					for(int b=0; b<3; b++)	{
					if(!b) LoadString(hInst,IDS_STRINGSW_21,s,MAX_PATH);
					else  LoadString(hInst,IDS_STRINGSW_24+(b-1)*5,s,MAX_PATH);
					sortBtns[0][b]=CreateWindowEx(WS_EX_CLIENTEDGE,L"BUTTON",
										s,
										WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,// | BS_PUSHBUTTON,
										2+b*(int)(0.01f*conf::resizeBarPersnt[0]*(width-19.0f))/3,
										*y,
										(int)(0.01f*conf::resizeBarPersnt[0]*(width-19.0f))/3,
										h,
										wnd,
										(HMENU)(IDB_BTN0+7+b),
										hInst,
										NULL);
					if(!sortBtns[0][b]) return FALSE;	}
					break;
				case 1:
					for(int b=0; b<3; b++)	{
					if(!b) LoadString(hInst,IDS_STRINGSW_21,s,MAX_PATH);
					else  LoadString(hInst,IDS_STRINGSW_24+(b-1)*5,s,MAX_PATH);
					sortBtns[1][b]=CreateWindowEx(WS_EX_CLIENTEDGE,L"BUTTON",
										s,
										WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,// | BS_PUSHBUTTON,
										7 + (int)(0.01f*conf::resizeBarPersnt[0]*(width-19))+b*(int)(0.01f*(conf::resizeBarPersnt[1]-conf::resizeBarPersnt[0])*(width-25.0f))/3,
										*y,
										(int)(0.01f*(conf::resizeBarPersnt[1]-conf::resizeBarPersnt[0])*(width-25.0f))/3,
										h,
										wnd,
										(HMENU)(IDB_BTN0+7+3+b),
										hInst,
										NULL);
					if(!sortBtns[1][b]) return FALSE;	}
					break;
				case 2:
					for(int b=0; b<3; b++)	{
					if(!b) LoadString(hInst,IDS_STRINGSW_21,s,MAX_PATH);
					else  LoadString(hInst,IDS_STRINGSW_24+(b-1)*5,s,MAX_PATH);
					sortBtns[2][b]=CreateWindowEx(WS_EX_CLIENTEDGE,L"BUTTON",
										s,
										WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,// | BS_PUSHBUTTON,
										12 + (int)(0.01f*conf::resizeBarPersnt[1]*(width-19)) + b*(int)((0.01f*(conf::resizeBarPersnt[2]-conf::resizeBarPersnt[1])*(width-25.0f)))/3,
										*y,
										(int)(0.01f*(conf::resizeBarPersnt[2]-conf::resizeBarPersnt[1])*(width-25.0f))/3,
										h,
										wnd,
										(HMENU)(IDB_BTN0+7+6+b),
										hInst,
										NULL);
					if(!sortBtns[2][b]) return FALSE;	}
					break;
				case 3:
					for(int b=0; b<3; b++)	{
					if(!b) LoadString(hInst,IDS_STRINGSW_21,s,MAX_PATH);
					else  LoadString(hInst,IDS_STRINGSW_24+(b-1)*5,s,MAX_PATH);
					sortBtns[3][b]=CreateWindowEx(WS_EX_CLIENTEDGE,L"BUTTON",
										s,
										WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,// | BS_PUSHBUTTON,
										17 + (int)(0.01f*conf::resizeBarPersnt[2]*(width-19))+b*(int)(0.01f*(100.0f-conf::resizeBarPersnt[2])*(width-19.0f))/3,
										*y,
										(int)(0.01f*(100.0f-conf::resizeBarPersnt[2])*(width-19.0f))/3,
										h,
										wnd,
										(HMENU)(IDB_BTN0+7+9+b),
										hInst,
										NULL);
					if(!sortBtns[3][b]) return FALSE;	}
					break;
			}
		break;
	}
	for(int b=0; b<3; b++)//Init paytida sort:
	{	if(0==b)
			panel[iPanel].SetSortBtnState(0,panel[iPanel].folderAndFileMixingSortType);
		else if(1==b)
			panel[iPanel].SetSortBtnState(1,panel[iPanel].folderSortType);
		else// if(2==b)
			panel[iPanel].SetSortBtnState(2,panel[iPanel].fileSortType);
	}
	*y += h;
	return TRUE;
}

BOOL CreateDragAndDrop()
{
	if(mDropTarget) return TRUE;
	if(!hWnd) return TRUE;
	//if(!panel[0].GetHWND()) return TRUE;
	OleInitialize(NULL);// CoInitializeEx DiskToolbarda ishlatganda buni qayta
	//for(int p=0; p<conf::Dlg.iTotPanels; p++)
	{	mDropTarget/*[p]*/ = new DropTarget(hWnd);//panel[p].GetHWND());
		mDropTarget/*[p]*/->AddRef();
		CoLockObjectExternal(mDropTarget/*[p]*/, TRUE, FALSE);
		if(S_OK!=RegisterDragDrop(/*panel[p].GetHWND()*/hWnd, mDropTarget))//[p]))//panel[0].GetHWND() hWnd
		//if(S_OK!=RegisterDragDrop(panel[1].GetHWND(), mDropTarget))
		{	Err::msg(hWnd,-1,L"RegisterDragDrop failed.");
			return FALSE;
		}
		mDropTarget/*[p]*/->Release();
	}
	return TRUE;
}

BOOL CreateEachPanelControls(HWND wnd,int iPanel,int *y, int btmSpaceHeight)
{
	if(!CreateSheetTabs(wnd, iPanel, y)) return FALSE;
	sortBtnsY[iPanel] = *y;
	if(!CreateSortButtons(wnd, iPanel, y)) return FALSE;
	pathAndNamesEditY[iPanel] = *y;
	if(!CreatePatnAndNameEdit(wnd, iPanel, y)) return FALSE;
	panelHeight[iPanel] = rcClient.bottom - rcClient.top - (*y) - (int)(3.2*SHEET_TAB_HEIGHT)-btmSpaceHeight;
	if(!CreatePanel(wnd, iPanel, y, panelHeight[iPanel])) return FALSE;
	return TRUE;
}

BOOL CreateControls(HWND wnd,BOOL bChngTotPanel)
{
INITCOMMONCONTROLSEX icex;icex.dwSize=sizeof(INITCOMMONCONTROLSEX);
icex.dwICC =  ICC_BAR_CLASSES;// ICC_USEREX_CLASSES; tooltip teper nenujen;

	if(!InitCommonControlsEx(&icex))
		return FALSE;

//	WM_NCCREATE dan WM_CREATE ga ko'chirdim, shu uchun kerakmas;
/*	GetWindowRect(wnd,&rcClient);POINT p={rcClient.left,rcClient.top};
	ScreenToClient(wnd,&p);
	POINT pp={rcClient.left,rcClient.top};
	ScreenToClient(wnd,&pp);
	rcClient.left = p.x;
	rcClient.top = p.y;
	rcClient.right = pp.x;
	rcClient.bottom = pp.y;*/

	if(CBtnBar::ActnBarClass)//2-marta kiryaptiyu,destroy qilingan:
	{	if(!topBar.GetBtns())//destroy qilingan:
		{	BOOL r = topBar.Read(L"topbar.cnf:stream");
			if(!r)r=topBar.TopCreateOnFail();//Reset();
			if(r)r = topBar.ReadIcons();
			BOOL r1 = bottomBar.InitUt();
			if(r1)r1 = bottomBar.ReadUtIcons();
	}	}

	int y=topBar.Create(wnd,0,0,rcClient.right,102);
	if(!DskToolBar::Create(wnd,&y)) return FALSE;

	int btmBarHeght = (1==conf::Dlg.bShowBtmPnl?bottomBar.CalcHeight(::rcClient.right):0);

	int sy = y;
	for(int i=0; i<conf::Dlg.iTotPanels; i++)
	{	y = sy;
		if(1==conf::Dlg.bShowDiskSpace[i])
			y += HEIGHT_OF_DISK_SPACE_IMAGE;
		CreateEachPanelControls(wnd,i,&y,btmBarHeght);
	}
	selFilesInfoEditY = y;
	if(!CreatePanelSelFilesInfoEdits(wnd,&y)) return FALSE;
	if(!CreatePanelResizeBars(wnd,y)) return FALSE;
	if(!CreateCmndButtons(wnd,&y)) return FALSE;
	cmndsComboY = y;
	if(!CreateCmndsCombo(wnd,&y)) return FALSE;

	if(1==conf::Dlg.bShowBtmPnl)
	{	bottomBar.Create(wnd,0,y,rcClient.right,103);
	}

	//DragAcceptFiles(wnd,TRUE);//Panelgamas,aynan otasiga olarkan;
	if(!CreateDragAndDrop()) return FALSE;

	//conf::ChangeTotPanelsNum(conf::Dlg.iTotPanels);
	//hPenBlue = CreatePen(PS_SOLID, 1, RGB(0,0,255));
	return TRUE;
}

BOOL ResizePanel(int iPanel, int *y, int h)
{
//profilerOutStr("Beg of ResizePanels")

int w,width;
width = rcClient.right - rcClient.left;

	switch(conf::Dlg.iTotPanels)
	{	case 1:
			panel[0].Move(
						2, 
						*y,
						width-4,
						panelHeight[0]);
					*y += panelHeight[0];
		break;
		case 2: default:
			w = (width-9)/2;
			switch(iPanel)
			{	case 0:
					panel[0].Move(
								2, 
								*y,
								w + (int)(w*0.02f*(conf::resizeBarPersnt[0]-50.0f)),
								panelHeight[0]);
					*y += panelHeight[0];
					break;
				case 1:
					panel[1].Move(
								w+7 + (int)(w*0.02f*(conf::resizeBarPersnt[0]-50.0f)), 
								*y,
								w - (int)(w*0.02f*(conf::resizeBarPersnt[0]-50.0f)),
								panelHeight[1]);
					*y += panelHeight[1];
				break;
			}
		break;
		case 3:
			w = (width-14)/3;
			switch(iPanel)
			{	case 0:
					panel[0].Move(
								2, 
								*y,
								w + (int)(w*0.03f*(conf::resizeBarPersnt[0]-33.33f)),
								panelHeight[0]);
					*y += panelHeight[0];
					break;
				case 1:
					panel[1].Move(
								7 + (int)(w*0.03f*conf::resizeBarPersnt[0]),
								*y,
								(int)(w*0.03f*(conf::resizeBarPersnt[1]-conf::resizeBarPersnt[0])),
								panelHeight[1]);
					*y += panelHeight[1];
					break;
				case 2:
					panel[2].Move(
								12 + (int)(w*0.03f*conf::resizeBarPersnt[1]),
								*y,
								(int)(w*0.03f*(100.0f-conf::resizeBarPersnt[1])),
								panelHeight[2]);
					*y += panelHeight[2];
					break;
			}
		break;
		case 4:
			w = (width-19)/4;
			switch(iPanel)
			{	case 0:
					panel[0].Move(
								2, 
								*y,
								(int)(0.01f*conf::resizeBarPersnt[0]*(width-19.0f)),
								panelHeight[0]);
					*y += panelHeight[0];
					break;
				case 1:
					panel[1].Move(
								7 + (int)(0.01f*conf::resizeBarPersnt[0]*(width-19)),
								*y,
								(int)(0.01f*(conf::resizeBarPersnt[1]-conf::resizeBarPersnt[0])*(width-25.0f)),
								panelHeight[1]);
					*y += panelHeight[1];
					break;
				case 2:
					panel[2].Move(
								12 + (int)(0.01f*conf::resizeBarPersnt[1]*(width-19)),
								*y,
								(int)(0.01f*(conf::resizeBarPersnt[2]-conf::resizeBarPersnt[1])*(width-19.0f)),
								panelHeight[2]);
					*y += panelHeight[2];
					break;
				case 3:
					panel[3].Move(
								20 + (int)(0.01f*conf::resizeBarPersnt[2]*(width-25)),
								*y,
								(int)(0.01f*(100.0f-conf::resizeBarPersnt[2])*(width-25.0f)),
								panelHeight[3]);
					*y += panelHeight[3];
					break;
			}
		break;
	}
    return TRUE;
}

BOOL ResizePanelSelFilesInfoEdits(int *y)
{
//profilerOutStr("Beg of ResizePanelSelFilesInfoEdits")

int w,width;
width = rcClient.right - rcClient.left;
BOOL hidden = (rcClient.bottom-4-INFO_EDT_HEIGHT-HOT_KEY_BAR_HEIGHT+2 < 110);
	switch(conf::Dlg.iTotPanels)
	{	case 1:
			MoveWindow(	panel[0].selFilesInfoEdt,
						2,
						*y,
						width-4,
						hidden ? 0 : 24,
						TRUE);
			*y += hidden ? 0 : 24;
		break;
		case 2: default:
			w = (width-9)/2;
			MoveWindow(	panel[0].selFilesInfoEdt,
						2,
						*y,
						w + (int)(w*0.02f*(conf::resizeBarPersnt[0]-50.0f)),
						hidden ? 0 : 24,
						TRUE);
			MoveWindow(	panel[1].selFilesInfoEdt,
						w+7 + (int)(w*0.02f*(conf::resizeBarPersnt[0]-50.0f)),
						*y,
						w - (int)(w*0.02f*(conf::resizeBarPersnt[0]-50.0f)),
						hidden ? 0 : 24,
						TRUE);
			*y += hidden ? 0 : 24;
		break;
		case 3:
			w = (width-20)/3;
			MoveWindow(	panel[0].selFilesInfoEdt,
						2,
						*y,
						w + (int)(w*0.03f*conf::resizeBarPersnt[0]-33.33f),
						hidden ? 0 : 24,
						TRUE);
			MoveWindow(	panel[1].selFilesInfoEdt,
						7 + (int)(w*0.03f*conf::resizeBarPersnt[0]),
						*y,
						(int)(w*0.03f*(conf::resizeBarPersnt[1]-conf::resizeBarPersnt[0])),
						hidden ? 0 : 24,
						TRUE);
			MoveWindow(	panel[2].selFilesInfoEdt,
						12 + (int)(w*0.03f*conf::resizeBarPersnt[1]),
						*y,
						(int)(w*0.03f*(100.0f-conf::resizeBarPersnt[1])),
						hidden ? 0 : 24,
						TRUE);
			*y += hidden ? 0 : 24;
		break;
		case 4:
			w = (width-25)/4;
			MoveWindow(	panel[0].selFilesInfoEdt,
						2,
						*y,
						(int)(0.01f*conf::resizeBarPersnt[0]*(width-25.0f)),
						hidden ? 0 : 24,
						TRUE);
			MoveWindow(	panel[1].selFilesInfoEdt,
						7 + (int)(0.01f*conf::resizeBarPersnt[0]*(width-19)),
						*y,
						(int)(0.01f*(conf::resizeBarPersnt[1]-conf::resizeBarPersnt[0])*(width-25.0f)),
						hidden ? 0 : 24,
						TRUE);
			MoveWindow(	panel[2].selFilesInfoEdt,
						12 + (int)(0.01f*conf::resizeBarPersnt[1]*(width-19)),
						*y,
						(int)(0.01f*(conf::resizeBarPersnt[2]-conf::resizeBarPersnt[1])*(width-25.0f)),
						hidden ? 0 : 24,
						TRUE);
			MoveWindow(panel[3].selFilesInfoEdt,
						17 + (int)(0.01f*conf::resizeBarPersnt[2]*(width-19)),
						*y,
						(int)(0.01f*(100.0f-conf::resizeBarPersnt[2])*(width-19.0f)),
						hidden ? 0 : 24,
						TRUE);
			*y += hidden ? 0 : 24;
		break;
	}
    return TRUE;
}

BOOL ResizePathAndNamesEdit(int iPanel, int *y)
{
//profilerOutStr("Beg of ResizePanelSelFilesInfoEdits")

int w,width;
width = rcClient.right - rcClient.left;
BOOL hidden = (rcClient.bottom-(*y) < 110);
	switch(conf::Dlg.iTotPanels)
	{	case 1:
			MoveWindow(	panel[0].pathAndNameEdt[0],
						2,
						*y,
						width-4,
						hidden ? 0 : 24,
						TRUE);
		break;
		case 2: default:
			w = (width-9)/2;
			switch(iPanel)
			{	case 0:
					MoveWindow(	panel[0].pathAndNameEdt[0],
								2,
								*y,
								w + (int)(w*0.02f*(conf::resizeBarPersnt[0]-50.0f)),
								hidden ? 0 : 24,
								TRUE);
					break;
				case 1:
					MoveWindow(	panel[1].pathAndNameEdt[0],
								w+7 + (int)(w*0.02f*(conf::resizeBarPersnt[0]-50.0f)),
								*y,
								w - (int)(w*0.02f*(conf::resizeBarPersnt[0]-50.0f)),
								hidden ? 0 : 24,
								TRUE);
					break;
			}
		break;
		case 3:
			w = (width-20)/3;
			switch(iPanel)
			{	case 0:
					MoveWindow(	panel[0].pathAndNameEdt[0],
								2,
								*y,
								w + (int)(w*0.03f*(conf::resizeBarPersnt[0]-33.33f)),
								hidden ? 0 : 24,
								TRUE);
					break;
				case 1:
					MoveWindow(	panel[1].pathAndNameEdt[0],
								7 + (int)(w*0.03f*conf::resizeBarPersnt[0]),
								*y,
								(int)(w*0.03f*(conf::resizeBarPersnt[1]-conf::resizeBarPersnt[0])),
								hidden ? 0 : 24,
								TRUE);
					break;
				case 2:
					MoveWindow(	panel[2].pathAndNameEdt[0],
								12 + (int)(w*0.03f*conf::resizeBarPersnt[1]),
								*y,
								(int)(w*0.03f*(100.0f-conf::resizeBarPersnt[1])),
								hidden ? 0 : 24,
								TRUE);
					break;
			}
		break;
		case 4:
			w = (width-25)/4;
			switch(iPanel)
			{	case 0:
					MoveWindow(	panel[0].pathAndNameEdt[0],
								2,
								*y,
								(int)(0.01f*conf::resizeBarPersnt[0]*(width-25.0f)),
								hidden ? 0 : 24,
								TRUE);
					break;
				case 1:
					MoveWindow(	panel[1].pathAndNameEdt[0],
								7 + (int)(0.01f*conf::resizeBarPersnt[0]*(width-19)),
								*y,
								(int)(0.01f*(conf::resizeBarPersnt[1]-conf::resizeBarPersnt[0])*(width-25.0f)),
								hidden ? 0 : 24,
								TRUE);
					break;
				case 2:
					MoveWindow(	panel[2].pathAndNameEdt[0],
								12 + (int)(0.01f*conf::resizeBarPersnt[1]*(width-19)),
								*y,
								(int)(0.01f*(conf::resizeBarPersnt[2]-conf::resizeBarPersnt[1])*(width-25.0f)),
								hidden ? 0 : 24,
								TRUE);
					break;
				case 3:
					MoveWindow(panel[3].pathAndNameEdt[0],
								17 + (int)(0.01f*conf::resizeBarPersnt[2]*(width-19)),
								*y,
								(int)(0.01f*(100.0f-conf::resizeBarPersnt[2])*(width-19.0f)),
								hidden ? 0 : 24,
								TRUE);
					break;
			}
		break;
	}
	*y += hidden ? 0 : 24;
    return TRUE;
}

BOOL ResizeSheetTabs(int iPanel, int *y)
{
int rows = TabCtrl_GetRowCount(sheetTab[iPanel]);
if(1==TabCtrl_GetItemCount(sheetTab[iPanel]))rows=1;//1-marta
int h = rows*SHEET_TAB_HEIGHT;
int width = rcClient.right - rcClient.left;
int w;

BOOL hidden=FALSE;
	switch(conf::Dlg.iTotPanels)
	{	case 1:
			if(panelHeight[0] < 10)
				hidden = TRUE;
			MoveWindow(	sheetTab[0],
						2,
						*y,
						width-4,
						hidden ? 0 : h,
						TRUE);
			*y += hidden ? 0 : h;
		break;
		case 2: default:
			w = (width-9)/2;
			switch(iPanel)
			{	case 0:
					if(panelHeight[0] < 10)
						hidden = TRUE;
					MoveWindow(	sheetTab[0],
								2,
								*y,
								w + (int)(w*0.02f*(conf::resizeBarPersnt[0]-50.0f)),
								hidden ? 0 : h,
								TRUE);
					*y += hidden ? 0 : h;
					break;
				case 1:
					if(panelHeight[1] < 10)
						hidden = TRUE;
					MoveWindow(	sheetTab[1],
								w+7 + (int)(w*0.02f*(conf::resizeBarPersnt[0]-50.0f)),
								*y,
								w - (int)(w*0.02f*(conf::resizeBarPersnt[0]-50.0f)),
								hidden ? 0 : h,
								TRUE);
					*y += hidden ? 0 : h;
					break;
			}
		break;
		case 3:
			w = (width-20)/3;
			switch(iPanel)
			{	case 0:
					if(panelHeight[0] < 10)
						hidden = TRUE;
					MoveWindow(	sheetTab[0],
								2,
								*y,
								w + (int)(w*0.03f*conf::resizeBarPersnt[0]-33.33f),
								hidden ? 0 : h,
								TRUE);
					*y += hidden ? 0 : h;
					break;
				case 1:
					if(panelHeight[1] < 10)
						hidden = TRUE;
					MoveWindow(	sheetTab[1],
								7 + (int)(w*0.03f*conf::resizeBarPersnt[0]),
								*y,
								(int)(w*0.03f*(conf::resizeBarPersnt[1]-conf::resizeBarPersnt[0])),
								hidden ? 0 : h,
								TRUE);
					*y += hidden ? 0 : h;
					break;
				case 2:
					if(panelHeight[2] < 10)
						hidden = TRUE;
					MoveWindow(	sheetTab[2],
								12 + (int)(w*0.03f*conf::resizeBarPersnt[1]),
								*y,
								(int)(w*0.03f*(100.0f-conf::resizeBarPersnt[1])),
								hidden ? 0 : h,
								TRUE);
					*y += hidden ? 0 : h;
					break;
			}
		break;
		case 4:
			w = (width-25)/4;
			switch(iPanel)
			{	case 0:
					if(panelHeight[0] < 10)
						hidden = TRUE;
					MoveWindow(	sheetTab[0],
								2,
								*y,
								(int)(0.01f*conf::resizeBarPersnt[0]*(width-25.0f)),
								hidden ? 0 : h,
								TRUE);
					*y += hidden ? 0 : h;
					break;
				case 1:
					if(panelHeight[1] < 10)
						hidden = TRUE;
					MoveWindow(	sheetTab[1],
								7 + (int)(0.01f*conf::resizeBarPersnt[0]*(width-19)),
								*y,
								(int)(0.01f*(conf::resizeBarPersnt[1]-conf::resizeBarPersnt[0])*(width-25.0f)),
								hidden ? 0 : h,
								TRUE);
					*y += hidden ? 0 : h;
					break;
				case 2:
					if(panelHeight[2] < 10)
						hidden = TRUE;
					MoveWindow(	sheetTab[2],
								12 + (int)(0.01f*conf::resizeBarPersnt[1]*(width-19)),
								*y,
								(int)(0.01f*(conf::resizeBarPersnt[2]-conf::resizeBarPersnt[1])*(width-25.0f)),
								hidden ? 0 : h,
								TRUE);
					*y += hidden ? 0 : h;
					break;
				case 3:
					if(panelHeight[3] < 10)
						hidden = TRUE;
					MoveWindow(sheetTab[3],
								17 + (int)(0.01f*conf::resizeBarPersnt[2]*(width-19)),
								*y,
								(int)(0.01f*(100.0f-conf::resizeBarPersnt[2])*(width-19.0f)),
								hidden ? 0 : h,
								TRUE);
					*y += hidden ? 0 : h;
					break;
			}
		break;
	}
    return TRUE;
}

BOOL ResizePanelResizeBars(int y)
{
int w,width;
width = rcClient.right - rcClient.left;

	switch(conf::Dlg.iTotPanels)
	{	case 1:
		break;
		case 2: default:
			w = (width-15)/2;
			resizeBar[0].Move(
						w+6 + (int)(w*0.02f*(conf::resizeBarPersnt[0]-50.0f)),
						y-SHEET_TAB_HEIGHT-(panelHeight[0]>panelHeight[1]?panelHeight[0]:panelHeight[1]),
						3,
						panelHeight[0]>panelHeight[1]?panelHeight[0]:panelHeight[1]);
		break;
		case 3:
			w = (width-20)/3;
			resizeBar[0].Move(
						6 + (int)(w*0.03f*(conf::resizeBarPersnt[0])),
						y-SHEET_TAB_HEIGHT-(panelHeight[1]>panelHeight[2]?panelHeight[1]:panelHeight[2]),
						3,
						panelHeight[0]>panelHeight[1]?panelHeight[0]:panelHeight[1]);
			resizeBar[1].Move(
						11 + (int)(w*0.03f*conf::resizeBarPersnt[1]),
						y-SHEET_TAB_HEIGHT-(panelHeight[1]>panelHeight[2]?panelHeight[1]:panelHeight[2]),
						3,
						panelHeight[1]>panelHeight[2]?panelHeight[1]:panelHeight[2]);
		break;
		case 4:
			w = (width-25)/4;
			resizeBar[0].Move(
						6 + (int)(0.01f*conf::resizeBarPersnt[0]*(width-25)),
						y-SHEET_TAB_HEIGHT-(panelHeight[0]>panelHeight[1]?panelHeight[0]:panelHeight[1]),
						3,
						panelHeight[0]>panelHeight[1]?panelHeight[0]:panelHeight[1]);
			resizeBar[1].Move(
						11 + (int)(0.01f*conf::resizeBarPersnt[1]*(width-25)),
						y-SHEET_TAB_HEIGHT-(panelHeight[1]>panelHeight[2]?panelHeight[1]:panelHeight[2]),
						3,
						panelHeight[1]>panelHeight[2]?panelHeight[1]:panelHeight[2]);
			resizeBar[2].Move(
						16 + (int)(0.01f*conf::resizeBarPersnt[2]*(width-25)),
						y-SHEET_TAB_HEIGHT-(panelHeight[2]>panelHeight[3]?panelHeight[2]:panelHeight[3]),
						3,
						panelHeight[2]>panelHeight[3]?panelHeight[2]:panelHeight[3]);
		break;
	}
    return TRUE;
}

VOID ReleaseDragAndDrop()
{
	//for(int p=0; p<conf::Dlg.iTotPanels; p++)
	{	if(mDropTarget)//[p])
		{	RevokeDragDrop(hWnd);//panel[p].GetHWND());
			CoLockObjectExternal(mDropTarget/*[p]*/, FALSE, TRUE);
			//if(mDropTarget[p])mDropTarget[p]->Release();// yuqoridagining o'zi qilobdur(3-parametri);
			mDropTarget/*[p]*/ = NULL;
	}	}
	OleUninitialize();
}

BOOL ResizeCmndButtons(int *y)
{
int w,h,width,height;
BOOL hidden = (rcClient.bottom-4-INFO_EDT_HEIGHT-HOT_KEY_BAR_HEIGHT+2 < 90);
	width = rcClient.right - rcClient.left;
	height = rcClient.bottom - rcClient.top;
	w = (width - 7*2)/7;
	h = 26; if(height<70 && height > 50) h = height+20-70; else if(height<50) h = 0;

	int x=2;
	for(int b=0; b<7; b++)
	{	MoveWindow(btns[b],//btns[b].GetHWND()
							 x,
							 *y,
							 w,
							 hidden?0:h,
							 TRUE);
		x += w+2;
	}
	*y += hidden?0:h;
	return TRUE;
}

BOOL ResizeCmndsCombo(int *y)
{
BOOL hidden = (rcClient.bottom-4-INFO_EDT_HEIGHT-HOT_KEY_BAR_HEIGHT+2 < 90);
int width = rcClient.right - rcClient.left;
	MoveWindow(cmndsCB,
				2,
				*y,
				width-4,
				hidden?0:250,TRUE);
	*y += hidden?0:25;
	return TRUE;
}

BOOL ResizeSortButtons(int iPanel, int *y)
{
int w,h,width,height;
BOOL hidden = (rcClient.bottom-(*y) < 90);
	width = rcClient.right - rcClient.left;
	height = rcClient.bottom - rcClient.top;
	h = 24; if(height<70 && height > 50) h = height+20-70; else if(height<50) h = 0;

	switch(conf::Dlg.iTotPanels)
	{	case 1: 
			for(int b=0; b<3; b++)			{
			MoveWindow(sortBtns[0][b],
								2+b*(width-4)/3,
								*y,
								(width-4)/3,
								hidden?0:h,TRUE);}
		break;
		case 2: default:
 			w = (width-9)/2;
			switch(iPanel)
			{	case 0:
					for(int b=0; b<3; b++)	{
					MoveWindow(sortBtns[0][b],
										2+b*(w + (int)(w*0.02f*(conf::resizeBarPersnt[0]-50.0f)))/3,
										*y,
										(w + (int)(w*0.02f*(conf::resizeBarPersnt[0]-50.0f)))/3,
										hidden?0:h,TRUE);}
					break;
				case 1:
					for(int b=0; b<3; b++)		{
					MoveWindow(sortBtns[1][b],
										w+7 + (int)(w*0.02f*(conf::resizeBarPersnt[0]-50.0f))+b*(w - (int)(w*0.02f*(conf::resizeBarPersnt[0]-50.0f)))/3,
										*y,
										(w - (int)(w*0.02f*(conf::resizeBarPersnt[0]-50.0f)))/3,
										hidden?0:h,TRUE);}
					break;
			}
		break;
		case 3:
			w = (width-14)/3;
			switch(iPanel)
			{	case 0:
					for(int b=0; b<3; b++){
					MoveWindow(sortBtns[0][b],
										5+b*(w + (int)(w*0.03f*(conf::resizeBarPersnt[0]-33.33f)))/3,
										*y,
										(w + (int)(w*0.03f*(conf::resizeBarPersnt[0]-33.33f)))/3,
										hidden?0:h,TRUE);}
					break;
				case 1:
					for(int b=0; b<3; b++)	{
					MoveWindow(sortBtns[1][b],
										7 + (int)(w*0.03f*conf::resizeBarPersnt[0])+b*(int)(w*0.03f*(conf::resizeBarPersnt[1]-conf::resizeBarPersnt[0]))/3,
										*y,
										(int)(w*0.03f*(conf::resizeBarPersnt[1]-conf::resizeBarPersnt[0]))/3,
										hidden?0:h,TRUE);}
					break;
				case 2:
					for(int b=0; b<3; b++)	{
					MoveWindow(sortBtns[2][b],
										9 + (int)(w*0.03f*conf::resizeBarPersnt[1])+b*(int)(w*0.03f*(100.0f-conf::resizeBarPersnt[1]))/3,
										*y,
										(int)(w*0.03f*(100.0f-conf::resizeBarPersnt[1]))/3,
										hidden?0:h,TRUE);}
					break;
			}
		break;
		case 4:
			//w = (width-25)/4;
			switch(iPanel)
			{	case 0:
					for(int b=0; b<3; b++)	{
					MoveWindow(sortBtns[0][b],
										2+b*(int)(0.01f*conf::resizeBarPersnt[0]*(width-19.0f))/3,
										*y,
										(int)(0.01f*conf::resizeBarPersnt[0]*(width-19.0f))/3,
										hidden?0:h,TRUE);}
					break;
				case 1:
					for(int b=0; b<3; b++)	{
					MoveWindow(sortBtns[1][b],
										7 + (int)(0.01f*conf::resizeBarPersnt[0]*(width-19))+b*(int)(0.01f*(conf::resizeBarPersnt[1]-conf::resizeBarPersnt[0])*(width-25.0f))/3,
										*y,
										(int)(0.01f*(conf::resizeBarPersnt[1]-conf::resizeBarPersnt[0])*(width-25.0f))/3,
										hidden?0:h,TRUE);}
					break;
				case 2:
					for(int b=0; b<3; b++)	{
					MoveWindow(sortBtns[2][b],
										12 + (int)(0.01f*conf::resizeBarPersnt[1]*(width-19)) + b*(int)((0.01f*(conf::resizeBarPersnt[2]-conf::resizeBarPersnt[1])*(width-25.0f)))/3,
										*y,
										(int)(0.01f*(conf::resizeBarPersnt[2]-conf::resizeBarPersnt[1])*(width-25.0f))/3,
										hidden?0:h,TRUE);}
					break;
				case 3:
					for(int b=0; b<3; b++)	{
					MoveWindow(sortBtns[3][b],
										17 + (int)(0.01f*conf::resizeBarPersnt[2]*(width-19))+b*(int)(0.01f*(100.0f-conf::resizeBarPersnt[2])*(width-19.0f))/3,
										*y,
										(int)(0.01f*(100.0f-conf::resizeBarPersnt[2])*(width-19.0f))/3,
										hidden?0:h,TRUE);}
					break;
			}
		break;
	}
	*y += hidden?0:h;
	return TRUE;
}

BOOL ResizeEachPanelControls(int iPanel, int *y, int btmBarHeght)
{
	if(!ResizeSheetTabs(iPanel, y)) return FALSE;
	sortBtnsY[iPanel] = *y;
	if(!ResizeSortButtons(iPanel, y)) return FALSE;
	pathAndNamesEditY[iPanel] = *y;
	if(!ResizePathAndNamesEdit(iPanel, y)) return FALSE;
	panelHeight[iPanel] = rcClient.bottom - rcClient.top - (*y) - (int)(3.2*SHEET_TAB_HEIGHT)-btmBarHeght;
	if(!ResizePanel(iPanel, y, panelHeight[iPanel])) return FALSE;
	return TRUE;
}

BOOL ResizeControls()
{
	int y=topBar.Resize(0);
	DskToolBar::Resize(&y);	

	int btmBarHeght = (1==conf::Dlg.bShowBtmPnl?bottomBar.CalcHeight(::rcClient.right):0);

	panelsTop = y;
	for(int i=0; i<conf::Dlg.iTotPanels; i++)
	{	y = panelsTop;
		if(1==conf::Dlg.bShowDiskSpace[i])
			y += HEIGHT_OF_DISK_SPACE_IMAGE;		
		ResizeEachPanelControls(i, &y, btmBarHeght);
	}
	selFilesInfoEditY = y;
	ResizePanelSelFilesInfoEdits(&y);
	ResizePanelResizeBars(y);
	ResizeCmndButtons(&y);
	cmndsComboY = y;
	ResizeCmndsCombo(&y);
	bottomBar.Resize(y);
	return TRUE;
}

BOOL DestroyControls(HWND wnd)
{
	ReleaseDragAndDrop();
	DskToolBar::Destroy();
	if(0==conf::Dlg.bBtnsPanelType)
	{	topBar.Destroy();
		bottomBar.Destroy();
	}

	for(int i=0; i<MAX_PANELS/*conf::Dlg.iTotPanels*/; i++)
	{	DestroyWindow(sheetTab[i]);
		for(int b=0; b<3; b++)
			DestroyWindow(sortBtns[i][b]);
		DestroyWindow(panel[i].pathAndNameEdt[0]);
		DestroyWindow(panel[i].selFilesInfoEdt);
		DestroyWindow(panel[i].hWnd);
		//Not childs:
		if(i>0)
			resizeBar[i-1].Destroy(hInst,i-1);
	}
	for(int b=0; b<7; b++)//Pastdagi knopkalar:
		DestroyWindow(btns[b]);//btns[b].Destroy();
	cmnCmndsCB.Save(cmnCmndsCBFName,cmndsCB,40);
	DestroyWindow(cmndsCB);
	//DeleteObject(hPenBlue);
	return TRUE;
}

#define PACKVERSION(major,minor) MAKELONG(minor,major)
DWORD GetDllVersion(LPCTSTR lpszDllName)
{
    HINSTANCE hinstDll;
    DWORD dwVersion = 0;

    /* For security purposes, LoadLibrary should be provided with a 
       fully-qualified path to the DLL. The lpszDllName variable should be
       tested to ensure that it is a fully qualified path before it is used. */
    hinstDll = LoadLibrary(lpszDllName);
	
    if(hinstDll)
    {
        DLLGETVERSIONPROC pDllGetVersion;
        pDllGetVersion = (DLLGETVERSIONPROC)GetProcAddress(hinstDll, 
                          "DllGetVersion");

        /* Because some DLLs might not implement this function, you
        must test for it explicitly. Depending on the particular 
        DLL, the lack of a DllGetVersion function can be a useful
        indicator of the version. */

        if(pDllGetVersion)
        {
            DLLVERSIONINFO dvi;
            HRESULT hr;

            ZeroMemory(&dvi, sizeof(dvi));
            dvi.cbSize = sizeof(dvi);

            hr = (*pDllGetVersion)(&dvi);

            if(SUCCEEDED(hr))
            {
               dwVersion = PACKVERSION(dvi.dwMajorVersion, dvi.dwMinorVersion);
            }
        }

        FreeLibrary(hinstDll);
    }
    return dwVersion;
}

BOOL GetClientRect(HWND hWnd, RECT *rect)
{
RECT rc={0,0,0,0};
	::GetClientRect(hWnd,&rc);
	if(rc.right-rc.left>0)
	if(rc.bottom-rc.top>0)
	{	rect->bottom = rc.bottom;
		rect->left = rc.left;
		rect->right = rc.right;
		rect->top = rc.top;
		return TRUE;
	}
	return FALSE;
}

BOOL GetWindowRect(HWND hWnd, RECT *rect)
{
RECT rc={0,0,0,0};
	::GetClientRect(hWnd,&rc);
	if(rc.right-rc.left>0)
	if(rc.bottom-rc.top>0)
	{	::GetWindowRect(hWnd,rect);
		return TRUE;
	}
	return FALSE;
}

int IsPointInsidePatnAndNameEditRect(POINT* pt)
{
POINT rc; int r=-1;
	rc.x = pt->x;
	rc.y = pt->y;
	switch(conf::Dlg.iTotPanels)
	{	case 1:
			ScreenToClient(panel[0].pathAndNameEdt[0],&rc);
			if(rc.x>0)
			if(rc.x<panel[0].rcClient.right)
			if(rc.y>0)
			if(rc.y<25)
				r = 0;
		break;
		case 2: default:
			ScreenToClient(panel[0].pathAndNameEdt[0],&rc);
			if(rc.x>0)
			if(rc.x<panel[0].rcClient.right)
			if(rc.y>0)
			if(rc.y<25)
				r = 0;
			if(-1==r)
			{	rc.x = pt->x;
				rc.y = pt->y;
				ScreenToClient(panel[1].pathAndNameEdt[0],&rc);
				if(rc.x>0)
				if(rc.x<panel[1].rcClient.right)
				if(rc.y>0)
				if(rc.y<25)
					r = 1;
			}
		break;
		case 3:
			ScreenToClient(panel[0].pathAndNameEdt[0],&rc);
			if(rc.x>0)
			if(rc.x<panel[0].rcClient.right)
			if(rc.y>0)
			if(rc.y<25)
				r = 0;
			if(-1==r)
			{	rc.x = pt->x;
				rc.y = pt->y;
				ScreenToClient(panel[1].pathAndNameEdt[0],&rc);
				if(rc.x>0)
				if(rc.x<panel[1].rcClient.right)
				if(rc.y>0)
				if(rc.y<25)
					r = 1;
			}
			if(-1==r)
			{	rc.x = pt->x;
				rc.y = pt->y;
				ScreenToClient(panel[2].pathAndNameEdt[0],&rc);
				if(rc.x>0)
				if(rc.x<panel[2].rcClient.right)
				if(rc.y>0)
				if(rc.y<25)
					r = 2;
			}
		break;
		case 4:
			ScreenToClient(panel[0].pathAndNameEdt[0],&rc);
			if(rc.x>0)
			if(rc.x<panel[0].rcClient.right)
			if(rc.y>0)
			if(rc.y<25)
				r = 0;
			if(-1==r)
			{	rc.x = pt->x;
				rc.y = pt->y;
				ScreenToClient(panel[1].pathAndNameEdt[0],&rc);
				if(rc.x>0)
				if(rc.x<panel[1].rcClient.right)
				if(rc.y>0)
				if(rc.y<25)
					r = 1;
			}
			if(-1==r)
			{	rc.x = pt->x;
				rc.y = pt->y;
				ScreenToClient(panel[2].pathAndNameEdt[0],&rc);
				if(rc.x>0)
				if(rc.x<panel[2].rcClient.right)
				if(rc.y>0)
				if(rc.y<25)
					r = 2;
			}
			if(-1==r)
			{	rc.x = pt->x;
				rc.y = pt->y;
				ScreenToClient(panel[3].pathAndNameEdt[0],&rc);
				if(rc.x>0)
				if(rc.x<panel[3].rcClient.right)
				if(rc.y>0)
				if(rc.y<25)
					r = 3;
			}
		break;
	}
	return r;
}

BOOL IsPointInsideCmndCBEdit(POINT* pt)
{
POINT rc;RECT rcCl;
	::GetClientRect(cmndsCBEdt,&rcCl);
	rc.x = pt->x;
	rc.y = pt->y;
	::ScreenToClient(cmndsCBEdt,&rc);
	if(rc.x>0)
	if(rc.x<rcCl.right)
	if(rc.y>0)
	if(rc.y<rcCl.bottom)
		return TRUE;
	return FALSE;
} 

VOID DrawDskSpace(HDC DC,int numPanel)
{HDC dc;wchar_t pth[4]=L"C:\\",txt[MAX_PATH]=L"C",mnuStr[32]=L"";int txtln;
	GetEnvironmentVariableW(L"languge",mnuStr,64);//Language instartup qo'yadur;
	if(DC)dc=DC;
	else dc = GetDC(::hWnd);
	SetBkMode(dc,TRANSPARENT);
	HFONT oldFnt = (HFONT)SelectObject(dc,hfCmdBtns);
	//HGDIOBJ oldPen=SelectObject(dc,hPenBlue);
	for(int p=(numPanel>-1?numPanel:0); p<(numPanel>-1?(numPanel+1):conf::Dlg.iTotPanels); ++p)
	{	if(0==conf::Dlg.bShowDiskSpace[p])continue;
		int x = panel[p].GetLeft();
		int y = panel[p].GetTop();
		int tsheetcnt=TabCtrl_GetItemCount(sheetTab[p]);
		if(tsheetcnt>0)
			y -= TabCtrl_GetRowCount(sheetTab[p])*SHEET_TAB_HEIGHT;
		txt[0]=pth[0] = panel[p].GetPath()[0];
		wchar_t *dsklab=myWMI::GetLogicalDriveLabel(pth);
		txtln=1;
		if(dsklab)
		{	txt[txtln++]='[';
			txtln+=MyStringCpy(&txt[txtln],MAX_PATH-txtln-1,dsklab);
			txt[txtln++]=']';
		}
		txt[txtln++]=' ';
		if(mnuStr[0])
		{	if(!wcscmp(mnuStr,L"russian"))
				{txt[txtln++]=L'В';txt[txtln++]=L'с';txt[txtln++]=L'е';txt[txtln++]=L'г';txt[txtln++]=L'о';}
			else if(!wcscmp(mnuStr,L"uzbekl"))
				{txt[txtln++]='H';txt[txtln++]='a';txt[txtln++]='m';txt[txtln++]='m';txt[txtln++]='a';txt[txtln++]='s';txt[txtln++]='i';}
			else if(!wcscmp(mnuStr,L"uzbekk"))				
				{txt[txtln++]=L'Ҳ';txt[txtln++]=L'а';txt[txtln++]=L'м';txt[txtln++]=L'м';txt[txtln++]=L'а';txt[txtln++]=L'с';txt[txtln++]=L'и';}////{txt[txtln++]=0x4b2/*1202'Ҳ'*/;txt[txtln++]=0x430/*'а'*/;txt[txtln++]=0x43c/*'м'*/;txt[txtln++]=0x43c/*'м'*/;txt[txtln++]=0x430/*'а'*/;txt[txtln++]=0x441/*'с'*/;txt[txtln++]=0x438/*'и'*/;}
			else
				{txt[txtln++]='T';txt[txtln++]='o';txt[txtln++]='t';}
		}
		txt[txtln++]=':';txt[txtln++]=' ';
		ULARGE_INTEGER freeBytesAv,totNumbOfBytes,totNumOfFreeBytes;
		if(!GetDiskFreeSpaceEx(pth,&freeBytesAv,&totNumbOfBytes,&totNumOfFreeBytes))continue;
		double busy = ((double)totNumOfFreeBytes.QuadPart)/((double)totNumbOfBytes.QuadPart);
		RECT rc={x-6,y-64,x+panel[p].GetWidth()+5,y-49};
		FillRect(dc,&rc,(HBRUSH)(COLOR_WINDOW));//Rectangle(dc,x,y-65,x+40,y-48);
		x+=4;
		Pie(dc,
			x,rc.top,//y-64,
			x+40,y-49,
			x+(int)(40.0*(1.0-busy)),y-60,
			x+(int)(40.0*(1.0-busy)),y-53);//Arc(dc,x,y-64,x+40,y-48,x+40,y-56,x+40,y-56);
		/*StringCchPrintf(txt,MAX_PATH,
			L"Tot: %d mb, free: %d mb, av: %d mb.",
			totNumbOfBytes.QuadPart/1048576,
			totNumOfFreeBytes.QuadPart/1048576,
			freeBytesAv.QuadPart/1048576);*/
		txtln+=MyStringPrintDecSpace1(&txt[txtln],totNumbOfBytes.QuadPart/1048576);
		if(mnuStr[0])//Language instartup qo'yadur;
		{	if(!wcscmp(mnuStr,L"russian"))
			{	txtln+=MyStringCpy(&txt[txtln],MAX_PATH-txtln-1,L"mb; своб: ");
				txtln+=MyStringPrintDecSpace1(&txt[txtln],totNumOfFreeBytes.QuadPart/1048576);
				txtln+=MyStringCpy(&txt[txtln],MAX_PATH-txtln-1,L"mb; дост: ");
			}
			else if(!wcscmp(mnuStr,L"uzbekl"))
			{	txtln+=MyStringCpy(&txt[txtln],MAX_PATH-txtln-1,L"mb; bo'sh: ");
				txtln+=MyStringPrintDecSpace1(&txt[txtln],totNumOfFreeBytes.QuadPart/1048576);
				txtln+=MyStringCpy(&txt[txtln],MAX_PATH-txtln-1,L"mb; ochiq: ");
			}
			else if(!wcscmp(mnuStr,L"uzbekk"))
			{	txtln+=MyStringCpy(&txt[txtln],MAX_PATH-txtln-1,L"mb; бўш: ");
				txtln+=MyStringPrintDecSpace1(&txt[txtln],totNumOfFreeBytes.QuadPart/1048576);
				txtln+=MyStringCpy(&txt[txtln],MAX_PATH-txtln-1,L"mb; очиқ: ");
			}
			else goto A;//if(wcscmp(mnuStr,L"English")
		}
		else//if(wcscmp(mnuStr,L"English")
		{A:	txtln+=MyStringCpy(&txt[txtln],MAX_PATH-txtln-1,L"mb; free: ");
			txtln+=MyStringPrintDecSpace1(&txt[txtln],totNumOfFreeBytes.QuadPart/1048576);
			txtln+=MyStringCpy(&txt[txtln],MAX_PATH-txtln-1,L"mb; av: ");
		}

		txtln+=MyStringPrintDecSpace1(&txt[txtln],freeBytesAv.QuadPart/1048576);
		txtln+=MyStringCpy(&txt[txtln],MAX_PATH-txtln-1,L"mb;");
		rc.left+= 42;

		DrawText(dc,txt,(int)wcslen(txt),&rc,DT_LEFT|DT_VCENTER);
		//TextOut(dc,x+41,y-64,txt,(int)wcslen(txt));
		//MoveToEx(dc,x+36,y-60,NULL);
		//LineTo(dc,x+20,y-56);
		//LineTo(dc,x+36,y-52);
	}
	//SelectObject(dc,oldPen);
	SelectObject(dc,oldFnt);
	if(!DC)ReleaseDC(::hWnd,dc);
}

}//namespace CmnCntrl

VOID DeselectAllWithout(int pan)
{
	switch(conf::Dlg.iTotPanels)
	{	case 1:
		break;
		case 2: default:
			if(pan!=0)
				panel[0].FreeSelection();
			if(pan!=1)
				panel[1].FreeSelection();
		break;
		case 3:
			if(pan!=0)
				panel[0].FreeSelection();
			if(pan!=1)
				panel[1].FreeSelection();
			if(pan!=2)
				panel[2].FreeSelection();
		break;
		case 4:
			if(pan!=0)
				panel[0].FreeSelection();
			if(pan!=1)
				panel[1].FreeSelection();
			if(pan!=2)
				panel[2].FreeSelection();
			if(pan!=3)
				panel[3].FreeSelection();
		break;
}	}

int FindMouseClickPanel(int x, int y)
{
	switch(conf::Dlg.iTotPanels)
	{	case 1:
			if(x > panel[0].GetLeft())if(x < panel[0].GetLeft()+panel[0].GetClientRight())
			if(y > panel[0].GetTop() )if(y < panel[0].GetTop()+panel[0].GetClientBottom())
				return 0;
		break;
		case 2: default:
			if(x > panel[0].GetLeft())if(x < panel[0].GetLeft()+panel[0].GetClientRight())
			if(y > panel[0].GetTop() )if(y < panel[0].GetTop()+panel[0].GetClientBottom())
				return 0;
			if(x > panel[1].GetLeft())if(x < panel[1].GetLeft()+panel[1].GetClientRight())
			if(y > panel[1].GetTop() )if(y < panel[1].GetTop()+panel[1].GetClientBottom())
				return 1;
		break;
		case 3:
			if(x > panel[0].GetLeft())if(x < panel[0].GetLeft()+panel[0].GetClientRight())
			if(y > panel[0].GetTop() )if(y < panel[0].GetTop()+panel[0].GetClientBottom())
				return 0;
			if(x > panel[1].GetLeft())if(x < panel[1].GetLeft()+panel[1].GetClientRight())
			if(y > panel[1].GetTop() )if(y < panel[1].GetTop()+panel[1].GetClientBottom())
				return 1;
			if(x > panel[2].GetLeft())if(x < panel[2].GetLeft()+panel[2].GetClientRight())
			if(y > panel[2].GetTop() )if(y < panel[2].GetTop()+panel[2].GetClientBottom())
				return 2;
		break;
		case 4:
			if(x > panel[0].GetLeft())if(x < panel[0].GetLeft()+panel[0].GetClientRight())
			if(y > panel[0].GetTop() )if(y < panel[0].GetTop()+panel[0].GetClientBottom())
				return 0;
			if(x > panel[1].GetLeft())if(x < panel[1].GetLeft()+panel[1].GetClientRight())
			if(y > panel[1].GetTop() )if(y < panel[1].GetTop()+panel[1].GetClientBottom())
				return 1;
			if(x > panel[2].GetLeft())if(x < panel[2].GetLeft()+panel[2].GetClientRight())
			if(y > panel[2].GetTop() )if(y < panel[2].GetTop()+panel[2].GetClientBottom())
				return 2;
			if(x > panel[3].GetLeft())if(x < panel[3].GetLeft()+panel[3].GetClientRight())
			if(y > panel[3].GetTop() )if(y < panel[3].GetTop()+panel[3].GetClientBottom())
				return 3;
		break;
	}
	return -1;
}