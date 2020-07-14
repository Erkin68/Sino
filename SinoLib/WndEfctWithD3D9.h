#pragma once

#include "windows.h"
#include "d3d9.h"
#include "VertexStructs.h"


#define MAX_LOADSTRING		100
#define NUM_EFFECT_THREADS	4
#define START_EFFECT_TIME	1200
#define MAX_MESHES			10

typedef enum EfctType
{
	efctInOpenWindow = 0,//1 marta ishlagandan so'ng tipini close ga to'g'rilaymiz;
	efctInCloseWindow = 1,
	efctInOpenMenu = 2,
	efctInCloseMenu = 3
};

typedef struct TEfctThrTls EfctThrTls;

class Mesh
{
public:
				Mesh();
			   ~Mesh();
	   bool		Load(LPCWSTR,long);
	   void		Draw(EfctThrTls*);
	   void		FreeMem();
protected:
	   int		nFaces;
	   int		nVerts;
	   __int16* inds;
	   VNT32*	verts;
};

typedef struct TEfctThrTls
{
//*******************************   TLS variables:   ****************************************
	HWND	hEfctWnd;						//window handle									*
	TCHAR	szEffectWindowTitle[MAX_LOADSTRING];//window title string						*
	TCHAR	szEffectWindowClass[MAX_LOADSTRING];//window class string						*
	BOOL	bIncrOpasity;					//opasity increase, or decrease					*
	DWORD	prcssSetTime;					//full cyrcle SP time							*
	DWORD	tick;							//tick wia GetTickCount()						*
	HANDLE	evnts[3];//command events from calling process:[0]-termination command,[1]-repeat
					 //command and [3]-restart command										*
	HANDLE	finCircle;						//own self signal when finish full circle		*
	HANDLE  waitForShowParentEvnt;														  //*
	float	interpTime;						//releted fl time between 0 to 1.0f such as tick*
	int		left;							//left coord of the window						*
	int		top;							//top coord of the window						*
	int		animLeft;						//left coord of the window when animate			*
	int		animTop;						//top coord of the window when animate			*
	int		width;							//width of the window							*
	int		height;							//height of the window							*
	int		efctType;						//which effect;									*
	HBITMAP bmp;							//for texturing material;						*
	HDC		bmpDC;																		  //*
	void    **tlsOut;						//For CreateThread calls;						*
	LPDIRECT3DDEVICE9 pd3dDevice;														  //*
	LPDIRECT3DTEXTURE9 pTexture;														  //*
	D3DXMATRIXA16 matView;																  //*
	D3DXMATRIXA16 matProj;																  //*
	D3DXMATRIXA16 mViewPort;															  //*
//*******************************************************************************************
} EfctThrTls;

extern DWORD		dwTlsIndex;
extern HINSTANCE	hInst;

extern int EffectThread(TEfctThrTls*);

