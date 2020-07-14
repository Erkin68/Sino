// SinoSHlp32View.h : interface of the CSinoSHlp32View class
//


#pragma once

#include "LeftView.h"

#define  MAX_HTMLS  16


class CSinoSHlp32View : public CHtmlView
{
protected: // create from serialization only
	CSinoSHlp32View();
	DECLARE_DYNCREATE(CSinoSHlp32View)

// Attributes
public:
	LPCTSTR GetHTMLName(UINT,int);
	CSinoSHlp32Doc* GetDocument() const;
	BOOL OnPrevBtnPressed();
	BOOL OnNextBtnPressed();
	BOOL OnPrevStepBtnPressed();
	BOOL OnNextStepBtnPressed();
	BOOL OnFindTextBtnPressed();
	BOOL OnSearchTextBtnPressed();
	//BOOL PushStep(int);
	//BOOL PopStep(int);
	void ChangeFont(int);

// Operations
public:
	typedef enum TBeforeNavigateType
	{	undefined,
		fromHtmlElement,//default
		fromContentsButton,
		fromIndexButton,
		fromSearchButton,
		fromContentsList,
		fromIndexList,
		fromSearchList,
		selectListItem
	} BeforeNavigateType;
	BeforeNavigateType beforeNavigateType;

	HWND findTxtDlg;
	UINT uiCrntRes;
	TCHAR tchCrntRes[MAX_PATH];
	virtual BOOL LoadFromResource(UINT);
	virtual BOOL LoadFromResource(LPCTSTR);
	virtual void DocumentComplete(LPDISPATCH,VARIANT*);

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT&);
	//		void MyNavigateComplete2(UINT);
protected:
	virtual void OnBeforeNavigate2(LPCTSTR,DWORD,LPCTSTR,CByteArray&,LPCTSTR,BOOL*);
	virtual void CSinoSHlp32View::NavigateComplete2(LPDISPATCH,VARIANT*);
	//virtual void Invalidate(BOOL);
	//virtual BOOL OnCmdMsg(UINT,int,void*,AFX_CMDHANDLERINFO*);
	//virtual void NotifyWinEvent(DWORD,LONG,LONG);
	//virtual void DoDataExchange(CDataExchange*);
	virtual void OnInitialUpdate(); // called first time after construct

// Implementation
public:
	virtual ~CSinoSHlp32View();
	void NavigatePage(int);//,BOOL);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	int iPage,iList;
	//int iStep;
	//int steps[MAX_STEPS];

protected:

// Generated message map functions
protected:

	DECLARE_MESSAGE_MAP()
};

extern CSinoSHlp32View *htmlView;

#ifndef _DEBUG  // debug version in SinoSHlp32View.cpp
inline CSinoSHlp32Doc* CSinoSHlp32View::GetDocument() const
   { return reinterpret_cast<CSinoSHlp32Doc*>(m_pDocument); }
#endif

