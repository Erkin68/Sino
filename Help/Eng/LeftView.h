// LeftView.h : interface of the CLeftView class
//

#pragma once

typedef enum TListType
{	contents,
	index,
	search
} ListType;

extern ListType listType;

class CSinoSHlp32Doc;

class CLeftView : public CTreeView
{
protected: // create from serialization only
	CLeftView();
	DECLARE_DYNCREATE(CLeftView)

// Attributes
public:
	CSinoSHlp32Doc* GetDocument();

// Operations
public:

// Overrides
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT&);

			void SetPage(ListType,int);
			void SelectItem(int);
			void AddSearchPage(int,UINT);//OnSearchSelChanged(NMHDR*,LRESULT*); o'rniga;
	protected:
	//virtual BOOL OnNotify(WPARAM,LPARAM,LRESULT*);
	//virtual BOOL OnCmdMsg(UINT,int,void*,AFX_CMDHANDLERINFO*);
	virtual void OnSelChanged(NMHDR*,LRESULT*);
	virtual void OnItemExpanding(NMHDR*, LRESULT*);
	virtual void OnInitialUpdate(); // called first time after construct

// Implementation
public:
	virtual ~CLeftView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	
	void OnContentsSelChanged(NMHDR*,LRESULT*);
	void OnIndexSelChanged(NMHDR*,LRESULT*);
	void OnSearchSelChanged(NMHDR*,LRESULT*);
	void SetContentsPage(int);
	void SetIndexPage(int);
	void SetSearchPage(int);


// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

extern CLeftView *listView;

#ifndef _DEBUG  // debug version in LeftView.cpp
inline CSinoSHlp32Doc* CLeftView::GetDocument()
   { return reinterpret_cast<CSinoSHlp32Doc*>(m_pDocument); }
#endif

