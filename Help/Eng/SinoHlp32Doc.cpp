// SinoSHlp32Doc.cpp : implementation of the CSinoSHlp32Doc class
//

#include "stdafx.h"
#include "SinoHlp32.h"

#include "SinoHlp32Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSinoSHlp32Doc

IMPLEMENT_DYNCREATE(CSinoSHlp32Doc, CDocument)

BEGIN_MESSAGE_MAP(CSinoSHlp32Doc, CDocument)
END_MESSAGE_MAP()


// CSinoSHlp32Doc construction/destruction

CSinoSHlp32Doc::CSinoSHlp32Doc()
{
	// TODO: add one-time construction code here

}

CSinoSHlp32Doc::~CSinoSHlp32Doc()
{
}

BOOL CSinoSHlp32Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CSinoSHlp32Doc serialization

void CSinoSHlp32Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

//BOOL CSinoSHlp32Doc::OnEvent(UINT idCtrl,AFX_EVENT *pEvent,AFX_CMDHANDLERINFO *pHandlerInfo)
//{
//	return CDocument::OnEvent(idCtrl,pEvent,pHandlerInfo);
//}

// CSinoSHlp32Doc diagnostics

#ifdef _DEBUG
void CSinoSHlp32Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSinoSHlp32Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CSinoSHlp32Doc commands
