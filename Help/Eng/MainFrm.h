// MainFrm.h : interface of the CMainFrame class
//


#pragma once

class CSinoSHlp32View;

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
protected:
	CSplitterWnd m_wndSplitter;
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
			int  FindText(CString&,int,BOOL);
			BOOL IsConsistFindText(CString&);
			VOID SelectAllFindStrInCrntPage();
			void ClearSearchResults();

// Implementation
public:
	virtual ~CMainFrame();
	void	EnableToolbarBtn(int,BOOL);
	CSinoSHlp32View* GetRightPane();

	//For search total :***********************************	
	BOOL BeginFindTextInAllHTMLs(CString&,int,BOOL);	//*
	VOID ContinueFindTextInAllHTMLs();					//*
	VOID EndFindTextInAllHTMLs();						//*
	int  iSearchProcessBegin;							//*
	int  crntSearchRes;									//*
	int  foundedPages;									//*
	UINT saveuiRes;										//*
	HWND seachPrgsDlg;									//*
	CString schText;									//*
	//*****************************************************

	int iTotalHtmlResources;
	int *resNamesBuf;
	CToolTipCtrl *tooltip;


#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	//CToolBarCtrl m_wndContents;
	CToolBar m_wndToolBar;


// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
};

extern CMainFrame *mainFrame;
