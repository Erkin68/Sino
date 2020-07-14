// SinoSHlp32Doc.h : interface of the CSinoSHlp32Doc class
//


#pragma once


class CSinoSHlp32Doc : public CDocument
{
protected: // create from serialization only
	CSinoSHlp32Doc();
	DECLARE_DYNCREATE(CSinoSHlp32Doc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	//virtual BOOL OnEvent(UINT,AFX_EVENT*,AFX_CMDHANDLERINFO*);
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CSinoSHlp32Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


