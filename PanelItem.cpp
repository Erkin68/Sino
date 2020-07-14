#include "Sino.h"
#include "Panel.h"
#include "Config.h"
#include "strsafe.h"
#include "FileAssociation.h"
#include "Operations\LinkSocket.h"
#include "Operations\MyShell\MyShell.h"
#include "WindowsManagmentInstrumentation.h"

PanelItem::PanelItem():hMasterIcon(0),iCstmItem(0xff),assocNum(-1),allSize(0)			
{
}
PanelItem::~PanelItem()//Heap larni o'zi qilaveradi
{
}
VOID PanelItem::FreeMem()//Heap larni o'zi qilaveradi
{	
	iCstmItem=0xff;
	if(hMasterIcon)
	{	DestroyIcon(hMasterIcon);
		hMasterIcon = NULL;
}	}
BOOL PanelItem::DrawOtsech(Panel *prnt)
{	if(0==prnt->bItemsViewType)
	{	if(rc.left>prnt->rcClient.right)
			return TRUE;
		if(rc.right<0)
			return TRUE;
		return FALSE;
	}
	//else
	if(rc.top>prnt->rcClient.bottom)
		return TRUE;
	if(rc.bottom<0)
		return TRUE;
	return FALSE;
}

VOID PanelItem::CheckForCstmItems(Panel* prnt,WIN32_FIND_DATA *ff)
{
 if(!ff)
	 return;
 for(int it=0; it<conf::Dlg.iCustPanItemDefs[prnt->iThis]; it++)
 {  SearchItem *item = &conf::Dlg.cstmItms[prnt->iThis][it].itm;
	int iFrom = 0,selStrLen = MyStringLength(item->filtr,MAX_PATH);
	for(int i=0; i<=selStrLen; i++)//Oxirgi 0 niyam olsun, aks holda 1ta qolib ketadur;
	{	if(' '==item->filtr[i] ||//Ajratamiz;
		   ';'==item->filtr[i] ||//Ajratamiz;
		   '<'==item->filtr[i] ||//Ajratamiz;
		   '>'==item->filtr[i] ||//Ajratamiz;
		   ':'==item->filtr[i] ||//Ajratamiz;
		  '\"'==item->filtr[i] ||//Ajratamiz;
		   '/'==item->filtr[i] ||//Ajratamiz;
		  '\\'==item->filtr[i] ||//Ajratamiz;
		   '|'==item->filtr[i] ||//Ajratamiz;
		   '?'==item->filtr[i] ||//Ajratamiz;
		   item->filtr[i]<32    )//Ajratamiz;  0 ham kiradi, ya'ni oxiri;
		   //'*' ni qoldiramiz, chunki hammasi degani;
		{	if(i-iFrom>0 || (!selStrLen))//Hech narsa filtri yo'q, lekin search for text bor;
			{	if(MyStringCmprFltr(ff->cFileName,item->filtr,iFrom,i))
				{	if(file==attribute)
					{	if(CheckForOtherCondtns(item,prnt,ff))
						{	iCstmItem=it;
							return;
					}	}
					else
					{	iCstmItem=it;
						return;
					}
					iFrom = i+1;
	}	}	}	}

		//Agar hech narsa belgilamagan bo'lsayam dopoln.parametrlar bo'yicha poisk qilsun;
	if((!selStrLen) || (wcsstr(item->filtr,L"*.*")))
	{	if(file==attribute)
		{	if(CheckForOtherCondtns(item,prnt,ff))
			{	iCstmItem=it;
				return;
		}	}
		else
		{	if(item->bFindForText) continue;
			if(item->bFindForExcldText) continue;
			if(item->bFileSz) continue;
			if(item->bFindForAlterName) continue;
			if(item->bCrTimeBef) continue;
			if(item->bCrTimeAft) continue;
			if(item->bCrTimeBet) continue;
			if(item->bLstAccTimeBef) continue;
			if(item->bLstAccTimeAft) continue;
			if(item->bLastAccTimeBet) continue;
			if(item->bLstWrTimeBef) continue;
			if(item->bLstWrTimeAft) continue;
			if(item->bLstWrTimeBet) continue;
			if(item->bFileAttr) continue;
			iCstmItem=it;
			return;
}}	}	}

VOID PanelItem::CheckForCstmItems(Panel* prnt,NSKERNEL::PFILE_BOTH_DIR_INFORMATION di)
{
 if(!di)
	 return;
 for(int it=0; it<conf::Dlg.iCustPanItemDefs[prnt->iThis]; it++)
 {  SearchItem *item = &conf::Dlg.cstmItms[prnt->iThis][it].itm;
	int iFrom = 0,selStrLen = MyStringLength(item->filtr,MAX_PATH);
	for(int i=0; i<=selStrLen; i++)//Oxirgi 0 niyam olsun, aks holda 1ta qolib ketadur;
	{	if(' '==item->filtr[i] ||//Ajratamiz;
		   ';'==item->filtr[i] ||//Ajratamiz;
		   '<'==item->filtr[i] ||//Ajratamiz;
		   '>'==item->filtr[i] ||//Ajratamiz;
		   ':'==item->filtr[i] ||//Ajratamiz;
		  '\"'==item->filtr[i] ||//Ajratamiz;
		   '/'==item->filtr[i] ||//Ajratamiz;
		  '\\'==item->filtr[i] ||//Ajratamiz;
		   '|'==item->filtr[i] ||//Ajratamiz;
		   '?'==item->filtr[i] ||//Ajratamiz;
		   item->filtr[i]<32    )//Ajratamiz;  0 ham kiradi, ya'ni oxiri;
		   //'*' ni qoldiramiz, chunki hammasi degani;
		{	if(i-iFrom>0 || (!selStrLen))//Hech narsa filtri yo'q, lekin search for text bor;
			{	if(MyStringCmprFltr(Name,item->filtr,iFrom,i))
				{	if(file==attribute)
					{	if(CheckForOtherCondtns(item,prnt,di))
						{	iCstmItem=it;
							return;
					}	}
					else
					{	iCstmItem=it;
						return;
					}
					iFrom = i+1;
	}	}	}	}

	//Agar hech narsa belgilamagan bo'lsayam dopoln.parametrlar bo'yicha poisk qilsun;
	if((!selStrLen) || (wcsstr(item->filtr,L"*.*")))
	{	if(file==attribute)
		{	if(CheckForOtherCondtns(item,prnt,di))
			{	iCstmItem=it;
				return;
		}	}
		else
		{	if(item->bFindForText) continue;
			if(item->bFindForExcldText) continue;
			if(item->bFileSz) continue;
			if(item->bFindForAlterName) continue;
			if(item->bCrTimeBef) continue;
			if(item->bCrTimeAft) continue;
			if(item->bCrTimeBet) continue;
			if(item->bLstAccTimeBef) continue;
			if(item->bLstAccTimeAft) continue;
			if(item->bLastAccTimeBet) continue;
			if(item->bLstWrTimeBef) continue;
			if(item->bLstWrTimeAft) continue;
			if(item->bLstWrTimeBet) continue;
			if(item->bFileAttr) continue;
			iCstmItem=it;
			return;
}}	}	}

BOOL PanelItem::CheckForOtherCondtns(SearchItem* item,Panel* prnt,WIN32_FIND_DATA *ff)
{
DWORD bCndtns = 0x00000000;
 	if(file==attribute)//file
	{if(item->bFindForText)
	 {if(item->FindForTextLn)
	  {int from=0;
	   for(int r=0; r<=item->FindForTextLn; r++)//Oxirgi 0 niyam olsun, aks holda 1ta qolib ketadur;
	   {if(';'==item->FindForText[r] || r==item->FindForTextLn)//Ajratamiz;
	    {if(r-from>0)
		 {wchar_t sub[MAX_PATH];if(item->bFindForText_Unicode)MySubstr((wchar_t*)item->FindForText,(wchar_t*)sub,from,r);
		  else MySubstr(item->FindForText,sub,from,r);
		  if(item->bFindForText_Unicode?fSearchViaF7::SearchForContainTextW$12(&ff->cFileName[0],(wchar_t*)sub,ff):fSearchViaF7::SearchForContainText$12(ff->cFileName,sub,ff))
		  {	bCndtns |= 0x00000001;//[0] = TRUE;
		 	goto ToNotContainText;
		  }
		  from = r+1;
     }}}}}
	 else bCndtns |= 0x00000001;//[0] = TRUE;
	 //3.1-Page, Find for do not contain text:
ToNotContainText:
	 if(item->bFindForExcldText)
	 {if(item->FindForExcldTextLn)
	  {int from=0;
	   for(int r=0; r<=item->FindForExcldTextLn; r++)//Oxirgi 0 niyam olsun, aks holda 1ta qolib ketadur;
	   {if(';'==item->FindForExcldText[r] || r==item->FindForExcldTextLn)//Ajratamiz;
	    {if(r-from>0)
	     {wchar_t sub[MAX_PATH];if(item->bFindForExcldText_Unicode)MySubstr((wchar_t*)item->FindForExcldText,(wchar_t*)sub,from,r);
		  else MySubstr(item->FindForExcldText,sub,from,r);
		  if(item->bFindForExcldText_Unicode?fSearchViaF7::SearchForNotContainTextW$12(&ff->cFileName[0],(wchar_t*)sub,ff):fSearchViaF7::SearchForNotContainText$12(ff->cFileName,sub,ff))
		  {	bCndtns |=0x00000002;//[1] = TRUE;
		 	break;//goto FindAlternName;
		  }
	 	  from = r+1;
     }}}}}
	 else bCndtns |= 0x00000002;//[1] = TRUE;

	 if(item->bFileSz)
	 {if(!item->sFileSzEqu[0])
		bCndtns |=0x00000004;//[2] = TRUE;
	  else
	  { if(!item->sFileSz[0])
		  bCndtns |=0x00000004;//[2] = TRUE;
	    else
		{	if(CB_ERR==item->iFileSzQual) bCndtns |= 0x00000003;//[2] = TRUE;
			else
			{	unsigned __int64 sz = MyAtoU64(item->sFileSz);
				for(int i=0; i<item->iFileSzQual; i++)
					sz = sz <<  10;
				unsigned __int64 fsz = ((unsigned __int64)ff->nFileSizeHigh << 32) + (unsigned __int64)ff->nFileSizeLow;
				if('>'==item->sFileSzEqu[0])
				{	if(sz < fsz)
						bCndtns |= 0x00000004;
					//bCndtns[2] = (sz < fsz);
				}
				else if('<'==item->sFileSzEqu[0])
				{	if(sz > fsz)
						bCndtns |= 0x00000004;
					//bCndtns[2] = (sz > fsz);
				}
				else //if('='==item->sFileSzEqu[0])
				{	if(sz == fsz)
						bCndtns |= 0x00000004;
					//bCndtns[2] = (sz == fsz);
	 }}	}	}	}
	 else bCndtns |= 0x00000004;//[2] = TRUE;
	}
	else//if(1==0)//folder
	{//1.1-Page, Filtr va RootPath -->> BeginSearch da;
	 //2.1-Page, Find for text:
	 //if(BST_CHECKED==SendMessage(GetDlgItem(hDlg,IDC_CHECK_TEXT_SEACH),BM_GETCHECK,0,0))
	 if(item->bFindForText)
		 return FALSE;
	 //3.1-Page, Find for do not contain text:
	 if(item->bFindForExcldText)
		 return FALSE;
	 //4.2-Page, Find for alternative name:
	 if(item->bFindForAlterName)
		 return FALSE;
	 if(item->bFileSz)
		 return FALSE;
	 bCndtns |= 0x00000007;//bCndtns[0] = bCndtns[1] = bCndtns[2] = TRUE;
	}

	//4.2-Page, Find for alternative name:
//FindAlternName:
	if(item->bFindForAlterName)
	{if(item->altName[0])
	{	if(0==wcscmp(ff->cAlternateFileName,item->altName))
			bCndtns |= 0x00000008;
	 }
	} else bCndtns |= 0x00000008;//[3] = TRUE;

	//5.2-Page, Find for creation time before:
	if(item->bCrTimeBef)
	{	if(-1!=CmpFILETIMEs(&item->CrTimeBef,&ff->ftCreationTime))
			bCndtns |= 0x00000010;
	}
	else bCndtns |= 0x00000010;//[4] = TRUE;

	//6.2-Page, Find for creation time after:
	if(item->bCrTimeAft)
	{	if(1!=CmpFILETIMEs(&item->CrTimeAft,&ff->ftCreationTime))
			bCndtns |= 0x00000020;
	}
	else bCndtns |= 0x00000020;//bCndtns[5] = TRUE;

	//7.2-Page, Find for creation time between:
	if(item->bCrTimeBet)
	{	if(1==CmpFILETIMEsBetween(&item->CrTimeBet[0],&item->CrTimeBet[1],&ff->ftCreationTime))
			bCndtns |= 0x00000040;
	}
	else bCndtns |= 0x00000040;//bCndtns[6] = TRUE;

	//8.2-Page, Find for last access time before:
	if(item->bLstAccTimeBef)
	{	if(-1!=CmpFILETIMEs(&item->LstAccTimeBef,&ff->ftCreationTime))
			bCndtns |= 0x00000080;
	}
	else bCndtns |= 0x00000080;//bCndtns[7] = TRUE;

	//9.2-Page, Find last access time after:
	if(item->bLstAccTimeAft)
	{	if(1!=CmpFILETIMEs(&item->LstAccTimeAft,&ff->ftLastAccessTime))
			bCndtns |= 0x00000100;
	}
	else bCndtns |= 0x00000100;//bCndtns[8] = TRUE;

	//10.2-Page, Find for last access time between:
	if(item->bLastAccTimeBet)
	{	if(1==CmpFILETIMEsBetween(&item->LastAccTimeBet[0],&item->LastAccTimeBet[1],&ff->ftLastAccessTime))
			bCndtns |= 0x00000200;
	}
	else bCndtns |= 0x00000200;//bCndtns[9] = TRUE;

	//11.2-Page, Find for last write time before:
	if(item->bLstWrTimeBef)
	{	if(-1!=CmpFILETIMEs(&item->LstWrTimeBef,&ff->ftLastWriteTime))
			bCndtns |= 0x00000400;
	}
	else bCndtns |= 0x00000400;//bCndtns[10] = TRUE;

	//12.2-Page, Find last write time after:
	if(item->bLstWrTimeAft)
	{	if(1!=CmpFILETIMEs(&item->LstWrTimeAft,&ff->ftLastWriteTime))
			bCndtns |= 0x00000800;
	}
	else bCndtns |= 0x00000800;//bCndtns[11] = TRUE;

	//13.2-Page, Find for last write time between:
	if(item->bLstWrTimeBet)
	{	if(1==CmpFILETIMEsBetween(&item->LstWrTimeBet[0],&item->LstWrTimeBet[1],&ff->ftLastWriteTime))
			bCndtns |= 0x00001000;
	}
	else bCndtns |= 0x00001000;//bCndtns[12] = TRUE;

	//14.2-Page, Find for file attribute:
	if(item->bFileAttr)
	{	if(item->bFileAttArchive)
		if((FILE_ATTRIBUTE_ARCHIVE & ff->dwFileAttributes) != 0)
			bCndtns |= 0x00002000;
		if(item->bFileAttCompr)
		if((FILE_ATTRIBUTE_COMPRESSED & ff->dwFileAttributes) != 0)
			bCndtns |= 0x00004000;
		if(item->bFileAttDevice)
		if((FILE_ATTRIBUTE_DEVICE & ff->dwFileAttributes) != 0)
			bCndtns |= 0x00008000;
		if(item->bFileAttDir)
		if((FILE_ATTRIBUTE_DIRECTORY & ff->dwFileAttributes) != 0)
			bCndtns |= 0x00010000;
		if(item->bFileAttEncr)
		if((FILE_ATTRIBUTE_ENCRYPTED & ff->dwFileAttributes) != 0)
			bCndtns |= 0x00020000;
		if(item->bFileAttHidden)
		if((FILE_ATTRIBUTE_HIDDEN & ff->dwFileAttributes) != 0)
			bCndtns |= 0x00040000;
		if(item->bFileAttNormal)
		if((FILE_ATTRIBUTE_NORMAL & ff->dwFileAttributes) != 0)
			bCndtns |= 0x00080000;
		if(item->bFileAttNotInd)
		if((FILE_ATTRIBUTE_NOT_CONTENT_INDEXED & ff->dwFileAttributes) != 0)
			bCndtns |= 0x00100000;
		if(item->bFileAttOffl)
		if((FILE_ATTRIBUTE_OFFLINE & ff->dwFileAttributes) != 0)
			bCndtns |= 0x00200000;
		if(item->bFileAttReadOnly)
		if((FILE_ATTRIBUTE_READONLY & ff->dwFileAttributes) != 0)
			bCndtns |= 0x00400000;
		if(item->bFileAttRepPt)
		if((FILE_ATTRIBUTE_REPARSE_POINT & ff->dwFileAttributes) != 0)
			bCndtns |= 0x00800000;
		if(item->bFileAttSparseFile)
		if((FILE_ATTRIBUTE_SPARSE_FILE & ff->dwFileAttributes) != 0)
			bCndtns |= 0x01000000;
		if(item->bFileAttSys)
		if((FILE_ATTRIBUTE_SYSTEM & ff->dwFileAttributes) != 0)
			bCndtns |= 0x02000000;
		if(item->bFileAttTemp)
		if((FILE_ATTRIBUTE_TEMPORARY & ff->dwFileAttributes) != 0)
			bCndtns |= 0x04000000;
		if(item->bFileAttVirt)
		if((FILE_ATTRIBUTE_VIRTUAL & ff->dwFileAttributes) != 0)
			bCndtns |= 0x08000000;
	}
	else
	{	bCndtns |= 0x0fffe000;
	}
	//Hamma usloviyalarni tekshirib chiqdik, endi qaytamiz:
	if(0x0fffffff==bCndtns)
		return TRUE;
	return FALSE;
}

BOOL PanelItem::CheckForOtherCondtns(SearchItem* item,Panel* prnt,
									  NSKERNEL::PFILE_BOTH_DIR_INFORMATION di)
{
DWORD bCndtns = 0x00000000;
	if(file==attribute)//file
 	{//1.1-Page, Filtr va RootPath -->> BeginSearch da;
	 //2.1-Page, Find for text:
	 if(item->bFindForText)
	 {if(item->FindForTextLn)
	  {int from=0;
	   for(int r=0; r<=item->FindForTextLn; r++)//Oxirgi 0 niyam olsun, aks holda 1ta qolib ketadur;
	   {if(';'==item->FindForText[r] || r==item->FindForTextLn)//Ajratamiz;
	    {if(r-from>0)
		 {wchar_t sub[MAX_PATH];if(item->bFindForExcldText_Unicode)MySubstr(&item->FindForText[0],sub,from,r);
		  else MySubstr(item->FindForText,sub,from,r);
		  if(item->bFindForExcldText_Unicode?NSKERNEL::SearchForContainText(item,&di->FileName[0],sub,di):NSKERNEL::SearchForContainText(item,di->FileName,sub,di))
		  {	bCndtns |= 0x00000001;//[0] = TRUE;
		 	goto ToNotContainTextK;
		  }
		  from = r+1;
	  }}}}
	  //bCndtns[0] = FALSE;
	 }
	 else bCndtns |= 0x00000001;//[0] = TRUE;
	 //3.1-Page, Find for do not contain text:
ToNotContainTextK:
	 if(item->bFindForExcldText)
	 {if(item->FindForExcldTextLn)
	  {int from=0;
	   for(int r=0; r<=item->FindForExcldTextLn; r++)//Oxirgi 0 niyam olsun, aks holda 1ta qolib ketadur;
	   {if(';'==item->FindForText[r] || r==item->FindForExcldTextLn)//Ajratamiz;
	    {if(r-from>0)
	     {wchar_t sub[MAX_PATH];if(item->bFindForExcldText_Unicode)MySubstr(item->FindForExcldText,sub,from,r);
		  else MySubstr(item->FindForExcldText,sub,from,r);
		  if(item->bFindForExcldText_Unicode?NSKERNEL::SearchForNotContainText(item,di->FileName,sub,di):NSKERNEL::SearchForNotContainText(item,di->FileName,sub,di))
		  {	bCndtns |=0x00000002;//[1] = TRUE;
		 	break;//goto FindAlternName;
		  }
	 	  from = r+1;
	  }}}}
	  //bCndtns[1] = FALSE;
	 }
	 else bCndtns |= 0x00000002;//[1] = TRUE;

	 if(item->bFileSz)
	 {if(!item->sFileSzEqu[0])
		bCndtns |=0x00000004;//[2] = TRUE;
	  else
	  { if(!item->sFileSz[0])
		  bCndtns |=0x00000004;//[2] = TRUE;
	    else
		{	if(CB_ERR==item->iFileSzQual) bCndtns |= 0x00000003;//[2] = TRUE;
			else
			{	__int64 sz = (__int64)MyAtoU64(item->sFileSz);
				for(int i=0; i<item->iFileSzQual; i++)
					sz = sz << 10;
				if('>'==item->sFileSzEqu[0])
				{	if(sz < di->EndOfFile.QuadPart)
						bCndtns |= 0x00000004;
					//bCndtns[2] = (sz < fsz);
				}
				else if('<'==item->sFileSzEqu[0])
				{	if(sz > di->EndOfFile.QuadPart)
						bCndtns |= 0x00000004;
					//bCndtns[2] = (sz > fsz);
				}
				else //if('='==item->sFileSzEqu[0])
				{	if(sz == di->EndOfFile.QuadPart)
						bCndtns |= 0x00000004;
					//bCndtns[2] = (sz == fsz);
	 }}	}	}	}
	 else bCndtns |= 0x00000004;//[2] = TRUE;
	}
	else if(folder==attribute)
	{//1.1-Page, Filtr va RootPath -->> BeginSearch da;
	 //2.1-Page, Find for text:
	 //if(BST_CHECKED==SendMessage(GetDlgItem(hDlg,IDC_CHECK_TEXT_SEACH),BM_GETCHECK,0,0))
	 if(item->bFindForText)
		 return FALSE;
	 //3.1-Page, Find for do not contain text:
	 if(item->bFindForExcldText)
		 return FALSE;
	 //4.2-Page, Find for alternative name:
	 if(item->bFindForAlterName)
		 return FALSE;
	 if(item->bFileSz)
		 return FALSE;
	 bCndtns |= 0x00000007;//bCndtns[0] = bCndtns[1] = bCndtns[2] = TRUE;
	}

	//4.2-Page, Find for alternative name:
//FindAlternName:
	if(item->bFindForAlterName)
	{if(item->altName[0])
	{	if(0==wcscmp(di->ShortName,item->altName))
			bCndtns |= 0x00000008;
	 }
	} else bCndtns |= 0x00000008;//[3] = TRUE;

	//5.2-Page, Find for creation time before:
	if(item->bCrTimeBef)
	{	if(-1!=CmpFILETIMEsL(&item->CrTimeBef,&di->CreationTime))
			bCndtns |= 0x00000010;
	}
	else bCndtns |= 0x00000010;//[4] = TRUE;

	//6.2-Page, Find for creation time after:
	if(item->bCrTimeAft)
	{	if(1!=CmpFILETIMEsL(&item->CrTimeAft,&di->CreationTime))
			bCndtns |= 0x00000020;
	}
	else bCndtns |= 0x00000020;//bCndtns[5] = TRUE;

	//7.2-Page, Find for creation time between:
	if(item->bCrTimeBet)
	{	if(1==CmpFILETIMEsBetweenL(&item->CrTimeBet[0],&item->CrTimeBet[1],&di->CreationTime))
			bCndtns |= 0x00000040;
	}
	else bCndtns |= 0x00000040;//bCndtns[6] = TRUE;

	//8.2-Page, Find for last access time before:
	if(item->bLstAccTimeBef)
	{	if(-1!=CmpFILETIMEsL(&item->LstAccTimeBef,&di->CreationTime))
			bCndtns |= 0x00000080;
	}
	else bCndtns |= 0x00000080;//bCndtns[7] = TRUE;

	//9.2-Page, Find last access time after:
	if(item->bLstAccTimeAft)
	{	if(1!=CmpFILETIMEsL(&item->LstAccTimeAft,&di->LastAccessTime))
			bCndtns |= 0x00000100;
	}
	else bCndtns |= 0x00000100;//bCndtns[8] = TRUE;

	//10.2-Page, Find for last access time between:
	if(item->bLastAccTimeBet)
	{	if(1==CmpFILETIMEsBetweenL(&item->LastAccTimeBet[0],&item->LastAccTimeBet[1],&di->LastAccessTime))
			bCndtns |= 0x00000200;
	}
	else bCndtns |= 0x00000200;//bCndtns[9] = TRUE;

	//11.2-Page, Find for last write time before:
	if(item->bLstWrTimeBef)
	{	if(-1!=CmpFILETIMEsL(&item->LstWrTimeBef,&di->LastWriteTime))
			bCndtns |= 0x00000400;
	}
	else bCndtns |= 0x00000400;//bCndtns[10] = TRUE;

	//12.2-Page, Find last write time after:
	if(item->bLstWrTimeAft)
	{	if(1!=CmpFILETIMEsL(&item->LstWrTimeAft,&di->LastWriteTime))
			bCndtns |= 0x00000800;
	}
	else bCndtns |= 0x00000800;//bCndtns[11] = TRUE;

	//13.2-Page, Find for last write time between:
	if(item->bLstWrTimeBet)
	{	if(1==CmpFILETIMEsBetweenL(&item->LstWrTimeBet[0],&item->LstWrTimeBet[1],&di->LastWriteTime))
			bCndtns |= 0x00001000;
	}
	else bCndtns |= 0x00001000;//bCndtns[12] = TRUE;

	//14.2-Page, Find for file attribute:
	if(item->bFileAttr)
	{	if(item->bFileAttArchive)
		if((FILE_ATTRIBUTE_ARCHIVE & di->FileAttributes) != 0)
			bCndtns |= 0x00002000;
		if(item->bFileAttCompr)
		if((FILE_ATTRIBUTE_COMPRESSED & di->FileAttributes) != 0)
			bCndtns |= 0x00004000;
		if(item->bFileAttDevice)
		if((FILE_ATTRIBUTE_DEVICE & di->FileAttributes) != 0)
			bCndtns |= 0x00008000;
		if(item->bFileAttDir)
		if((FILE_ATTRIBUTE_DIRECTORY & di->FileAttributes) != 0)
			bCndtns |= 0x00010000;
		if(item->bFileAttEncr)
		if((FILE_ATTRIBUTE_ENCRYPTED & di->FileAttributes) != 0)
			bCndtns |= 0x00020000;
		if(item->bFileAttHidden)
		if((FILE_ATTRIBUTE_HIDDEN & di->FileAttributes) != 0)
			bCndtns |= 0x00040000;
		if(item->bFileAttNormal)
		if((FILE_ATTRIBUTE_NORMAL & di->FileAttributes) != 0)
			bCndtns |= 0x00080000;
		if(item->bFileAttNotInd)
		if((FILE_ATTRIBUTE_NOT_CONTENT_INDEXED & di->FileAttributes) != 0)
			bCndtns |= 0x00100000;
		if(item->bFileAttOffl)
		if((FILE_ATTRIBUTE_OFFLINE & di->FileAttributes) != 0)
			bCndtns |= 0x00200000;
		if(item->bFileAttReadOnly)
		if((FILE_ATTRIBUTE_READONLY & di->FileAttributes) != 0)
			bCndtns |= 0x00400000;
		if(item->bFileAttRepPt)
		if((FILE_ATTRIBUTE_REPARSE_POINT & di->FileAttributes) != 0)
			bCndtns |= 0x00800000;
		if(item->bFileAttSparseFile)
		if((FILE_ATTRIBUTE_SPARSE_FILE & di->FileAttributes) != 0)
			bCndtns |= 0x01000000;
		if(item->bFileAttSys)
		if((FILE_ATTRIBUTE_SYSTEM & di->FileAttributes) != 0)
			bCndtns |= 0x02000000;
		if(item->bFileAttTemp)
		if((FILE_ATTRIBUTE_TEMPORARY & di->FileAttributes) != 0)
			bCndtns |= 0x04000000;
		if(item->bFileAttVirt)
		if((FILE_ATTRIBUTE_VIRTUAL & di->FileAttributes) != 0)
			bCndtns |= 0x08000000;
	}
	else
	{	bCndtns |= 0x0fffe000;
	}
	//Hamma usloviyalarni tekshirib chiqdik, endi qaytamiz:
	if(0x0fffffff==bCndtns)
		return TRUE;
  return FALSE;
}

/*char* PanelItem::GetNameA()
{
static char s[MAX_PATH];
	WideCharToMultiByte(AreFileApisANSI()?CP_ACP:CP_OEMCP,WC_COMPOSITECHECK,Name,-1,s,MAX_PATH-1,NULL,NULL);
	return s;
}*/

VOID PanelItem::GetRect(RECT *r,Panel *prnt,BOOL bCalc)
{
	if(bCalc)
		GetRectPtr(prnt,TRUE);
	r->left = rc.left;
	r->top = rc.top;
	r->right = rc.right;
	r->bottom = rc.bottom;
	return;
}

RECT* PanelItem::GetRectPtr(Panel *prnt,BOOL bCalc)
{
	if(!bCalc)return &rc;
	if(0==prnt->bItemsViewType)
	{	rc.left = prnt->GetScrollOffst() + nColumn * prnt->itemMaxWidth;
		rc.top  = nRow * prnt->GetItemHeight();
		rc.right= rc.left + (prnt->itemMaxWidth < prnt->GetClientRight() ?
							 prnt->itemMaxWidth : prnt->GetClientRight());
		rc.bottom = rc.top + prnt->GetItemHeight();
		return &rc;
	}
	//else
	rc.left = 0;
	rc.right= prnt->width;
	rc.top = prnt->GetScrollOffst() + nRow * prnt->GetItemHeight();
	rc.bottom = rc.top + prnt->GetItemHeight();
	return &rc;
}

COLORREF chngClr(DWORD rgb)
{	DWORD r = rgb & 0x00ff0000;
	DWORD g = rgb & 0x0000ff00;
	DWORD b = rgb & 0x000000ff;
	if(r>0x00250000)r -= 0x00250000;
	else			r  = 0x00000000;
	if(g>0x00003500)g -= 0x00003500;
	else 		    g  = 0x00000000;
	if(b<0x00000070)b += 0x0000008f;
	else		    b  = 0x000000ff;
	return (r | g | b);
}

//Tez ishlashi uchun rasshireniyesidan tashqari qismini ExtTextOutni ishlatib chizdik.Lekin hot bo'lsa
//DrawText bilan chizdik;
VOID PanelItem::Render(HDC DC,Panel *prnt,BOOL bRenderFast)//, PAINTSTRUCT *ps
{
	if(0xff!=iCstmItem)
	{	if(iCstmItem<conf::Dlg.iCustPanItemDefs[prnt->iThis])
			return RenderCustomItem(DC,prnt,bRenderFast);
		else iCstmItem = 0xff;
	}
RECT rc; GetRect(&rc,prnt,TRUE);
	if(DrawOtsech(prnt)) return;
	++rc.bottom;
RECT rcBht=rc; rcBht.left+=2;rcBht.top+=1;rcBht.right-=2;rcBht.bottom-=1;

HFONT oldFnt;
HDC dc = (0==DC) ? prnt->GetDC() : DC;
	if(0==DC)
		oldFnt=(HFONT)SelectObject(dc,prnt->hf);

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
	{	SetBkColor(dc,prnt->color[2]);
		rc.right = rc.left + XBITMAP_prnt;
		FillRect(dc,&rc,prnt->hBrshSrtBtn);
	} else
	{switch(state)//If non in edit mode
	 {	case normal     :
			if(bht)
			{	/*wchar_t s[260];static int i=0;
				wsprintf(s,L" bht: %d %d %d %d", rc.left,rc.top,rc.right,rc.bottom);
				OutputDebugString(s);
				wsprintf(s,L" bhthotbrsh: %d %d %d %d", rcBht.left,rcBht.top,rcBht.right,rcBht.bottom);
				OutputDebugString(s);*/
				
				SetTextColor(dc,lstChng?chngClr(prnt->color[4]):prnt->color[4]);//RGB(255,255,255));
				SetBkColor(dc,prnt->color[5]);
				FillRect(dc,&rc,prnt->brEdit);
				FillRect(dc,&rcBht,prnt->hotBkBrsh);
				//DrawEdge(dc,&rc,BDR_SUNKENOUTER,BF_RECT);
			} else
			{	SetTextColor(dc,lstChng?chngClr(prnt->color[1]):prnt->color[1]);//RGB(0,0,155));
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
	if(hMasterIcon)//if icon file or cursor file; ico b-sa ExtractIcon 32 lik ham ochishi mumkin ekan;
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

	//Text:
	if(file!=attribute)//folder & header:
	{	static wchar_t sFldr[MAX_PATH]=L"[";sFldr[1]=0;
		int l=1+MyStringCpy(&sFldr[1],MAX_PATH-1,Name);//MyStringCat(sFldr,MAX_PATH-1,Name);
		//MyStringCat(sFldr,MAX_PATH-1,"]");
		sFldr[l++]=']';
		sFldr[l]=0;
		//if(bht)
		//{	//TextOut(dc,
			//	(bht?rcBht.left:rc.left) + XBITMAP,
			//	bht?rcBht.top:rc.top,
			//	sFldr,
			//	NameCharLen+2);
		//	DrawText(dc,sFldr,NameCharLen+2,&rcTxt,DT_LEFT);
		//}
		//else
		{	//SetTextAlign(dc,TA_LEFT);
			ExtTextOut(dc,rcTxt.left,rcTxt.top,ETO_OPAQUE,&rcTxt,sFldr,NameCharLen+2,NULL);
	}	} else//File:
	{	int dev = prnt->MyGetTextExtentPoint32(L"...",3);
		int ovDel = nameWidth+extWidth+dev-prnt->rcClient.right;
		rcExt.left = rcExt.left-extWidth-MIN_DIST_FROM_COLUMNS-dev;
		if(ovDel>0)
		{	if(bht)
			{	//TextOut(dc,
				//	(bht?rcBht.left:rc.left) + XBITMAP, 
				//	bht?rcBht.top:rc.top, 
				//	Name,
				//	NameCharLen-2*ovDel/dev);
				DrawText(dc,Name,NameCharLen-2*ovDel/dev,&rcTxt,DT_LEFT);
			}
			else
			{	//SetTextAlign(dc,TA_LEFT);
				ExtTextOut(dc,rcTxt.left,rcTxt.top,ETO_OPAQUE,&rcTxt,Name,NameCharLen-2*ovDel/dev,NULL);
			}
			//TextOut(dc,
			//		(bht?rcBht.right:rc.right)-extWidth-MIN_DIST_FROM_COLUMNS-dev,
			//		bht?rcBht.top:rc.top, 
			//		"...",
			//		3);
			DrawText(dc,L"...",3,&rcExt,DT_RIGHT);
			//SetTextAlign(dc,TA_RIGHT);
			//ExtTextOut(dc,rcExt.left,rcExt.top,ETO_OPAQUE,&rcExt,"...",3,NULL);
		} else
		{	if(bht)
			{	//TextOut(dc, 
				//	(bht?rcBht.left:rc.left) + XBITMAP, 
				//	bht?rcBht.top:rc.top, 
				//	Name,
				//	NameCharLen);
				DrawText(dc,Name,NameCharLen,&rcTxt,DT_LEFT);
			}
			else
			{	//SetTextAlign(dc,TA_LEFT);
				ExtTextOut(dc,rcTxt.left,rcTxt.top,ETO_OPAQUE,&rcTxt,Name,NameCharLen,NULL);
		}	}
		//TextOut(dc,
		//		(bht?rcBht.right:rc.right) - extWidth - MIN_DIST_FROM_COLUMNS,
		//		bht?rcBht.top:rc.top, 
		//		GetExt(),//&Name[NameCharLen+1],
		//		ExtCharLen);
		rcExt.left = rcExt.right-extWidth-MIN_DIST_FROM_COLUMNS;
		DrawText(dc,GetExt(),ExtCharLen,&rcExt,DT_RIGHT);
		//SetTextAlign(dc,TA_RIGHT);
		//ExtTextOut(dc,rcExt.left,rcExt.top,ETO_OPAQUE,&rcExt,GetExt(),ExtCharLen,NULL);
	}
	if(0==DC)//Har 1 item alohida chizilyapti:
	{	SelectObject(dc,oldFnt);
		prnt->ReleaseDC(dc);
		//if(linkSock::sockDlg[prnt->iThis])
		//	RedrawWindow(linkSock::sockDlg[prnt->iThis],0,0,RDW_VALIDATE);
	}
	return;
}

VOID PanelItem::RenderCustomItem(HDC DC,Panel *prnt,BOOL bRenderFast)//, PAINTSTRUCT *ps
{
conf::CstmItemDef *cstm = &conf::Dlg.cstmItms[prnt->iThis][iCstmItem];
RECT rc; GetRect(&rc,prnt,TRUE);
	if(DrawOtsech(prnt))return;
	++rc.bottom;
RECT rcBht=rc; rcBht.left+=2;rcBht.top+=1;rcBht.right-=2;rcBht.bottom-=1;

HFONT oldFnt;
HDC dc = (0==DC) ? prnt->GetDC() : DC;
	oldFnt=(HFONT)SelectObject(dc,cstm->hFnt);

	//Panel::AddItem ni tezlashtirish uchun:
	/*if(-1==extWidth)
	{	SIZE sz;
		GetTextExtentPoint32(dc,&Name[NameCharLen+1],10,&sz);
		ExtCharLen = sz.cx;
		//if(-1>assocNum)
		//	prnt->fAssociation.AddMasterIcon(this,path);//Render ga qo'yamizmi? System32 ni chini juda sekin ochdiku???
	}*/
	
	if(!bRenderFast)//0 b-sa tezlash uchun;
	if(!hMasterIcon)
	if(-1>assocNum)
	if(-100<assocNum)
	{	//static char s[MAX_PATH];
		//prnt->pathLn=MyStringCpy(s,MAX_PATH-1,prnt->path);MyStringEraseEndAndCat(s,MAX_PATH-1,Name);
		prnt->fAssociation.AddMasterIcon(this,prnt->path);//s);
		if(!hMasterIcon)
		{	wchar_t* ext=ConvertToLittleR(&Name[NameCharLen+1],0);
			assocNum = prnt->fAssociation.IsRecExist(ext);//&Name[NameCharLen+1]);
		}
		else
			assocNum = -100;//Okonchatelniy, qaytib bu yergayam kirmasun;
	}

	BOOL bht = (prnt->iHot==this-&prnt->pItems[0] && Panel::iActivePanel==prnt->iThis);

	if(prnt->hEditItem && this==prnt->iEditItem)//If in edit mode
	{	SetBkColor(dc,cstm->clrRGBs[2]);
		rc.right = rc.left + XBITMAP_prnt;
		FillRect(dc,&rc,prnt->txtBkBrsh);
	} else
	{switch(state)//If non in edit mode
	 {	case normal     :
			if(bht)
			{	SetTextColor(dc,cstm->clrRGBs[4]);
				SetBkColor(dc,cstm->clrRGBs[5]);
				FillRect(dc,&rc,cstm->txtBkBrsh);
				FillRect(dc,&rcBht,cstm->hotBkBrsh);
			} else
			{	SetTextColor(dc,cstm->clrRGBs[1]);
				SetBkColor(dc,cstm->clrRGBs[0]);
				FillRect(dc,&rc,cstm->bkBrsh);
			}
		break;
		case selected   :
			if(bht)
			{	SetTextColor(dc,cstm->clrRGBs[3]);
				SetBkColor(dc,cstm->clrRGBs[5]);
				FillRect(dc,&rc,cstm->txtBkBrsh);
				FillRect(dc,&rcBht,cstm->hotBkBrsh);
			} else
			{	SetTextColor(dc,cstm->clrRGBs[3]);
				SetBkColor(dc,cstm->clrRGBs[2]);
				FillRect(dc,&rc,cstm->txtBkBrsh);
			}
		break;
   }}

	if(bht)rc=rcBht;
	int sz = conf::Dlg.iIconSize[prnt->iThis];

	//Icon:
	if(hMasterIcon)//if icon file or cursor file; ico b-sa ExtractIcon 32 lik ham ochishi mumkin ekan;
	{	DrawIconEx(	dc,
					rc.left+1,
					rc.top,
					hMasterIcon,
					sz,
					bht?(sz-2):sz,//16
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
						bht?(sz-2):sz,
						0,0,DI_NORMAL);
		}
		else
			DrawIconEx(	dc,
						rc.left+1,
						rc.top,
						prnt->hIcons[attribute],
						sz,
						bht?(sz-2):sz,//16
						0,0,DI_NORMAL);
	}

	if(prnt->hEditItem && this==prnt->iEditItem) return;


	RECT rcTxt = bht?rcBht:rc;
	rcTxt.left += XBITMAP_prnt;
	RECT rcExt = bht?rcBht:rc;

	//Text:
	if(file!=attribute)//folder & header:
	{	static wchar_t sFldr[MAX_PATH]=L"[";sFldr[1]='\0';
		int l=1+MyStringCpy(&sFldr[1],MAX_PATH-1,Name);//MyStringCat(sFldr,MAX_PATH-1,Name);
		sFldr[l++]=']';//MyStringCat(sFldr,MAX_PATH-1,"]");
		sFldr[l]=0;
		//TextOut(dc,
		//		(bht?rcBht.left:rc.left) + XBITMAP,
		//		bht?rcBht.top:rc.top,
		//		sFldr,
		//		NameCharLen+2);
		//DrawText(dc,sFldr,NameCharLen+2,&rcTxt,DT_LEFT);
		//SetTextAlign(dc,TA_LEFT);
		ExtTextOut(dc,rcTxt.left,rcTxt.top,ETO_OPAQUE,&rcTxt,sFldr,NameCharLen+2,NULL);
	} else//File:
	{	int dev = prnt->MyGetTextExtentPoint32(L"...",3);
		int ovDel = nameWidth+extWidth+dev-prnt->rcClient.right;
		rcExt.left = rcExt.left-extWidth-MIN_DIST_FROM_COLUMNS-dev;
		if(ovDel>0)
		{	if(bht)
			{	//TextOut(dc,
				//	(bht?rcBht.left:rc.left) + XBITMAP, 
				//	bht?rcBht.top:rc.top, 
				//	Name,
				//	NameCharLen-2*ovDel/dev);
				DrawText(dc,Name,NameCharLen-2*ovDel/dev,&rcTxt,DT_LEFT);
			}
			else
			{	//SetTextAlign(dc,TA_LEFT);
				ExtTextOut(dc,rcTxt.left,rcTxt.top,ETO_OPAQUE,&rcTxt,Name,NameCharLen-2*ovDel/dev,NULL);
			}
			//TextOut(dc,
			//		(bht?rcBht.right:rc.right)-extWidth-MIN_DIST_FROM_COLUMNS-dev,
			//		bht?rcBht.top:rc.top, 
			//		"...",
			//		3);
			DrawText(dc,L"...",3,&rcExt,DT_RIGHT);
			//SetTextAlign(dc,TA_RIGHT);
			//ExtTextOut(dc,rcExt.left,rcExt.top,ETO_OPAQUE,&rcExt,"...",3,NULL);
		} else
		{	if(bht)
			{	//TextOut(dc, 
				//	(bht?rcBht.left:rc.left) + XBITMAP, 
				//	bht?rcBht.top:rc.top, 
				//	Name,
				//	NameCharLen);
				DrawText(dc,Name,NameCharLen,&rcTxt,DT_LEFT);
			}
			else
			{	//SetTextAlign(dc,TA_LEFT);
				ExtTextOut(dc,rcTxt.left,rcTxt.top,ETO_OPAQUE,&rcTxt,Name,NameCharLen,NULL);
		}	}
		//TextOut(dc,
		//		(bht?rcBht.right:rc.right) - extWidth - MIN_DIST_FROM_COLUMNS,
		//		bht?rcBht.top:rc.top, 
		//		GetExt(),//&Name[NameCharLen+1],
		//		ExtCharLen);
		rcExt.left = rcExt.right-extWidth-MIN_DIST_FROM_COLUMNS;
		DrawText(dc,GetExt(),ExtCharLen,&rcExt,DT_RIGHT);
		//SetTextAlign(dc,TA_RIGHT);
		//ExtTextOut(dc,rcExt.left,rcExt.top,ETO_OPAQUE,&rcExt,GetExt(),ExtCharLen,NULL);
	}
	SelectObject(dc,oldFnt);
	if(0==DC)//Har 1 item alohida chizilyapti:
	{	prnt->ReleaseDC(dc);
		//if(linkSock::sockDlg[prnt->iThis])
		//	RedrawWindow(linkSock::sockDlg[prnt->iThis],0,0,RDW_VALIDATE);
	}
	return;
}

VOID PanelItem::RenderCustomItemTransparent(HDC DC,Panel *prnt,BOOL bRenderFast)
{
conf::CstmItemDef *cstm = &conf::Dlg.cstmItms[prnt->iThis][iCstmItem];
RECT rc; GetRect(&rc,prnt,TRUE);
	if(DrawOtsech(prnt))return;
	++rc.bottom;
RECT rcBht=rc; rcBht.left+=2;rcBht.top+=1;rcBht.right-=2;rcBht.bottom-=1;

HFONT oldFnt;
HDC dc = (0==DC) ? prnt->GetDC() : DC;
	oldFnt=(HFONT)SelectObject(dc,cstm->hFnt);

int rw = rc.right-rc.left;
int rh = rc.bottom-rc.top;
	//BitBlt(dc,rc.left,rc.top,rw,rh,prnt->imgBckgrnd[0].dc,rc.left,rc.top,SRCCOPY);

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

	if(prnt->hEditItem && this==prnt->iEditItem)//If in edit mode
	{	SetBkColor(dc,cstm->clrRGBs[2]);//RGB(255,255,255));
	} else
	{switch(state)//If non in edit mode
	 {	case normal     :
			if(bht)
			{	//SetTextColor(dc,cstm->clrRGBs[4]);
				//SetBkColor(dc,cstm->clrRGBs[5]);
			} else
			{	//SetTextColor(dc,cstm->clrRGBs[1]);
				//SetBkColor(dc,cstm->clrRGBs[0]);
			}
		break;
		case selected   :
			if(bht)
			{	//SetTextColor(dc,cstm->clrRGBs[3]);
				//SetBkColor(dc,cstm->clrRGBs[5]);
			} else
			{	//SetTextColor(dc,cstm->clrRGBs[3]);
				//SetBkColor(dc,cstm->clrRGBs[2]);
			}
		break;
   }}

	if(bht)rc=rcBht;
	int sz = conf::Dlg.iIconSize[prnt->iThis];

	//Icon:
	if(hMasterIcon)//if icon file or cursor file; ico b-sa ExtractIcon 32 lik ham ochishi mumkin ekan;
	{	DrawIconEx(	dc,
					rc.left+1,
					rc.top,
					hMasterIcon,
					sz,
					bht?(sz-2):sz,
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
						bht?(sz-2):sz,
						0,0,DI_NORMAL);
		}
		else
			DrawIconEx(	dc,
						rc.left+1,
						rc.top,
						prnt->hIcons[attribute],
						sz,
						bht?(sz-2):sz,
						0,0,DI_NORMAL);
	}

	if(prnt->hEditItem && this==prnt->iEditItem) return;


	RECT rcTxt = bht?rcBht:rc;
	rcTxt.left += XBITMAP_prnt;
	RECT rcExt = bht?rcBht:rc;

	//Text:
	if(file!=attribute)//folder & header:
	{	static wchar_t sFldr[MAX_PATH]=L"[";sFldr[1]='\0';
		int l=1+MyStringCpy(&sFldr[1],MAX_PATH-1,Name);//MyStringCat(sFldr,MAX_PATH-1,Name);
		sFldr[l++]=']';
		sFldr[l]=0;
		ExtTextOut(dc,rcTxt.left,rcTxt.top,ETO_OPAQUE,&rcTxt,sFldr,NameCharLen+2,NULL);
	} else//File:
	{	int dev = prnt->MyGetTextExtentPoint32(L"...",3);
		int ovDel = nameWidth+extWidth+dev-prnt->rcClient.right;
		rcExt.left = rcExt.left-extWidth-MIN_DIST_FROM_COLUMNS-dev;
		if(ovDel>0)
		{	if(bht)
			{	DrawText(dc,Name,NameCharLen-2*ovDel/dev,&rcTxt,DT_LEFT);
			}
			else
			{	ExtTextOut(dc,rcTxt.left,rcTxt.top,ETO_OPAQUE,&rcTxt,Name,NameCharLen-2*ovDel/dev,NULL);
			}
			DrawText(dc,L"...",3,&rcExt,DT_RIGHT);
		} else
		{	if(bht)
			{	DrawText(dc,Name,NameCharLen,&rcTxt,DT_LEFT);
			}
			else
			{	ExtTextOut(dc,rcTxt.left,rcTxt.top,ETO_OPAQUE,&rcTxt,Name,NameCharLen,NULL);
		}	}
		rcExt.left = rcExt.right-extWidth-MIN_DIST_FROM_COLUMNS;
		DrawText(dc,GetExt(),ExtCharLen,&rcExt,DT_RIGHT);
	}
	SelectObject(dc,oldFnt);
	if(0==DC)//Har 1 item alohida chizilyapti:
	{	prnt->ReleaseDC(dc);
	}
	return;
}

VOID PanelItem::RenderFullIcon(HDC dc,Panel *prnt)
{
RECT rc; GetRect(&rc,prnt,FALSE);
	if(DrawOtsech(prnt))return;
	++rc.bottom;
RECT rcBht=rc; rcBht.left+=2;rcBht.top+=1;rcBht.right-=2;rcBht.bottom-=1;

	if((!hMasterIcon) && (-1>assocNum) && (-100<assocNum))
	{	prnt->fAssociation.AddMasterIcon(this,prnt->path);//s);
		if(!hMasterIcon)
		{	wchar_t* ext=ConvertToLittleR(&Name[NameCharLen+1],ExtCharLen);//,0);
			assocNum = prnt->fAssociation.IsRecExist(ext);//&Name[NameCharLen+1]);
		}
		else
			assocNum = -100;//Okonchatelniy, qaytib bu yergayam kirmasun;
	}
	else return;

	BOOL bht = (prnt->iHot==this-&prnt->pItems[0] && Panel::iActivePanel==prnt->iThis);

	int sz = conf::Dlg.iIconSize[prnt->iThis];
	RECT r={rc.left+1,rc.top,rc.left+sz+1,rc.top+sz};
	if(bht)
		{r.left+=1;r.top+=1;r.bottom-=2;r.right-=2;}

	if(prnt->hEditItem && this==prnt->iEditItem)//If in edit mode
   		FillRect(dc,&r,prnt->txtBkBrsh);//(HBRUSH)GetStockObject(WHITE_BRUSH));

	if(prnt->hEditItem && this==prnt->iEditItem)//If in edit mode
   		FillRect(dc,&rc,prnt->txtBkBrsh);//(HBRUSH)GetStockObject(WHITE_BRUSH));
	else
	{switch(state)//If non in edit mode
	 {	case normal     :
			if(bht)
				FillRect(dc,&r,prnt->hotBkBrsh);//blueBrush);
			else
				FillRect(dc,&r,prnt->bkBrsh);//(HBRUSH)GetStockObject(WHITE_BRUSH));
		break;
		case selected   :
			if(bht)
				FillRect(dc,&r,prnt->hotBkBrsh);//prnt->blueBrush);
			else
				FillRect(dc,&r,prnt->txtBkBrsh);//(HBRUSH)GetStockObject(WHITE_BRUSH));
		break;
	}}

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
		{	DrawIconEx(dc,
						rc.left+1,
						rc.top,
						prnt->hIcons[attribute],
						sz,
						bht?(sz-1):sz,
						0,0,DI_NORMAL);
	}	}

	if(prnt->hEditItem && this==prnt->iEditItem) return;
	return;
}

VOID PanelItem::RenderFullIconTransparent(HDC dc,Panel *prnt)
{
RECT rc; GetRect(&rc,prnt,FALSE);
	if(DrawOtsech(prnt))return;
	++rc.bottom;
RECT rcBht=rc; rcBht.left+=2;rcBht.top+=1;rcBht.right-=2;rcBht.bottom-=1;

	if((!hMasterIcon) && (-1>assocNum) && (-100<assocNum))
	{	prnt->fAssociation.AddMasterIcon(this,prnt->path);//s);
		if(!hMasterIcon)
		{	wchar_t* ext=ConvertToLittleR(&Name[NameCharLen+1],ExtCharLen);//,0);
			assocNum = prnt->fAssociation.IsRecExist(ext);//&Name[NameCharLen+1]);
		}
		else
			assocNum = -100;//Okonchatelniy, qaytib bu yergayam kirmasun;
	}
	else return;

	BOOL bht = (prnt->iHot==this-&prnt->pItems[0] && Panel::iActivePanel==prnt->iThis);

	int sz = conf::Dlg.iIconSize[prnt->iThis];
	RECT r={rc.left+1,rc.top,rc.left+sz+1,rc.top+sz};
	if(bht)
		{r.left+=1;r.top+=1;r.bottom-=2;r.right-=2;}

int rw = rc.right-rc.left;
int rh = rc.bottom-rc.top;
	//BitBlt(dc,rc.left,rc.top,rw,rh,prnt->imgBckgrnd[0].dc,rc.left,rc.top,SRCCOPY);

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
		{	DrawIconEx(dc,
						rc.left+1,
						rc.top,
						prnt->hIcons[attribute],
						sz,
						bht?(sz-1):sz,
						0,0,DI_NORMAL);
	}	}

	if(prnt->hEditItem && this==prnt->iEditItem) return;
	return;
}

VOID PanelItem::SetName(Panel *prnt,wchar_t *txt,BOOL bFile)
{	
	//NameCharLen = MyStringCpy(Name,MAX_PATH,txt);
	wchar_t *pt=txt,*pext=0,*pn=&Name[0];
	int l=-1;
	do
	{	*pn = *pt;
		if('.'==(*pt)) pext = pn;
		++pn;++pt;++l;
	} while(*(pt-1) && l<MAX_GUID_CHAR);//char *pext = strrchr(Name,'.');
	int kp=(int)(pn-&Name[0])-1;
	NameCharLen = (U16)(kp>0?kp:0);
	if(bFile && pext && (*(pext+1)) && '.'!=(*(pext+1)))
	{	kp=(int)(pn - pext)-2;
		if(kp>0)
		{	ExtCharLen = (U8)kp;//MyStringLength(pext+1,STRSAFE_MAX_CCH);
			NameCharLen -= ExtCharLen+1;
		}else ExtCharLen=0;
	}
	else
		ExtCharLen = 0;
	nameWidth = prnt->MyGetTextExtentPoint32(Name,NameCharLen);
	if(bFile)
		extWidth = prnt->MyGetTextExtentPoint32(pext+1,ExtCharLen);
	else
		extWidth =  prnt->MyGetTextExtentPoint32(L"[]",2);
}

VOID PanelItem::SetGuidName(Panel *prnt,wchar_t *txt,wchar_t *guidTxt,BOOL bFile)
{	
	SetName(prnt,txt,bFile);

	Name[GUID_CHAR_POS-1]=0;
	MyStringCpy(&Name[GUID_CHAR_POS],GUID_CHAR_LEN,guidTxt);
}

VOID PanelItem::RenderTransparent(HDC DC,Panel *prnt,BOOL bRenderFast)//, PAINTSTRUCT *ps
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
			{	SetTextColor(dc,lstChng?chngClr(prnt->color[4]):prnt->color[4]);
				//SetBkColor(dc,prnt->color[5]);
			} else
			{	SetTextColor(dc,lstChng?chngClr(prnt->color[1]):prnt->color[1]);
				//SetBkColor(dc,prnt->color[0]);
			}
		break;
		case selected   :
			if(bht)
			{	SetTextColor(dc,lstChng?chngClr(prnt->color[3]):prnt->color[3]);
				//SetBkColor(dc,prnt->color[5]);
			} else
			{	SetTextColor(dc,lstChng?chngClr(prnt->color[3]):prnt->color[3]);
				//SetBkColor(dc,prnt->color[2]);
			}
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
	if(file!=attribute)//folder & header:
	{	static wchar_t sFldr[MAX_PATH]=L"[";sFldr[1]=0;
		int l=1+MyStringCpy(&sFldr[1],MAX_PATH-1,Name);
		sFldr[l++]=']';
		sFldr[l]=0;
		if(bht)
		{	//TextOut(dc,
			//	(bht?rcBht.left:rc.left) + XBITMAP,
			//	bht?rcBht.top:rc.top,
			//	sFldr,
			//	NameCharLen+2);
			DrawText(dc,sFldr,NameCharLen+2,&rcTxt,DT_LEFT);
		}
		else
		{	//SetTextAlign(dc,TA_LEFT);
			ExtTextOut(dc,rcTxt.left,rcTxt.top,0,&rcTxt,sFldr,NameCharLen+2,NULL);
	}	} else//File:
	{	int dev = prnt->MyGetTextExtentPoint32(L"...",3);
		int ovDel = nameWidth+extWidth+dev-prnt->rcClient.right;
		rcExt.left = rcExt.left-extWidth-MIN_DIST_FROM_COLUMNS-dev;
		if(ovDel>0)
		{	if(bht)
			{	DrawText(dc,Name,NameCharLen-2*ovDel/dev,&rcTxt,DT_LEFT);
			}
			else
			{	ExtTextOut(dc,rcTxt.left,rcTxt.top,0,&rcTxt,Name,NameCharLen-2*ovDel/dev,NULL);
		}	}
		else
		{	if(bht)
			{	DrawText(dc,Name,NameCharLen,&rcTxt,DT_LEFT);
			}
			else
			{	ExtTextOut(dc,rcTxt.left,rcTxt.top,0/*ETO_OPAQUE*/,&rcTxt,Name,NameCharLen,NULL);
		}	}
		rcExt.left = rcExt.right-extWidth-MIN_DIST_FROM_COLUMNS;
		ExtTextOut(dc,rcExt.left,rcExt.top,0,&rcExt,GetExt(),ExtCharLen,NULL);
	}

	if(bht)
	{MoveToEx(dc,rcTxt.left-sz,rcTxt.top,NULL);
	 LineTo(dc,rcTxt.right,rcTxt.top);
	 LineTo(dc,rcTxt.right,rcTxt.bottom);
	 LineTo(dc,rcTxt.left-sz,rcTxt.bottom);
	 LineTo(dc,rcTxt.left-sz,rcTxt.top);
	}

	if(0==DC)//Har 1 item alohida chizilyapti:
	{	SelectObject(dc,oldFnt);
		prnt->ReleaseDC(dc);
	}
	return;
}

#undef XBITMAP
