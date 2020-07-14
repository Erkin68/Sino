// SinoSHlp32.h : main header file for the SinoSHlp32 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CSinoSHlp32App:
// See SinoSHlp32.cpp for the implementation of this class
//

class CSinoSHlp32App : public CWinApp
{
public:
	CSinoSHlp32App();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual BOOL PreTranslateMessage(MSG*);

// Implementation
	afx_msg void OnAppAbout();
	void OnContentsBtnPressed();
	void OnIndexBtnPressed();
	void OnSearchBtnPressed();
	void OnPrevBtnPressed();
	void OnNextBtnPressed();
	void OnPrevStepBtnPressed();
	void OnNextStepBtnPressed();
	void OnFindTextBtnPressed();
	void CSinoSHlp32App::OnFilePrint();
	void CSinoSHlp32App::OnEditCopy();
	void CSinoSHlp32App::OnEditSelectAll();
	void CSinoSHlp32App::OnClearSelection();
	void CSinoSHlp32App::OnFontsSmallest();
	void CSinoSHlp32App::OnFontsSmall();
	void CSinoSHlp32App::OnFontsMedium();
	void CSinoSHlp32App::OnFontsLarge();
	void CSinoSHlp32App::OnFontsLargest();

	DECLARE_MESSAGE_MAP()
};

extern CSinoSHlp32App theApp;
