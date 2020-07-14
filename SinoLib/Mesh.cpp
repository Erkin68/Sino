#include "windows.h"
#include "strsafe.h"
#include "d3d9.h"
#include "WndEfctWithD3D9.h"

extern void errhandler(LPTSTR, HWND);



Mesh::Mesh()
{
	verts = NULL;
	inds  = NULL;
}

Mesh::~Mesh()
{
}

/*BOOL CALLBACK enumResProc(HMODULE hModule, LPTSTR lpszType,LONG_PTR lParam)
{
	return TRUE;
}
BOOL CALLBACK enumResNamesProc(HMODULE hModule,LPCTSTR lpszType,LPTSTR lpszName,LONG_PTR lParam)
{
	return TRUE;
}*/

bool Mesh::Load(LPCWSTR resId, long lb)
{
	//EnumResourceTypes((HMODULE)lb,enumResProc,NULL);
	//EnumResourceNames((HMODULE)lb,"MESH",enumResNamesProc,NULL);
	//char RN[16] = "#"; char RNN[16]; StringCchPrintf(RNN,16,"%d",resId);
	//StringCchCat(RN,16,RNN);
	//HRSRC fr = FindResource((HMODULE)lb, RN, "MESH");//HRSRC fr = FindResourceEx((HMODULE)lb, RN,"MESH", MAKELANGID(LANG_RUSSIAN,SUBLANG_RUSSIAN_RUSSIA));
	HRSRC fr = FindResource((HMODULE)lb, resId, L"MESH");//HRSRC fr = FindResourceEx((HMODULE)lb, RN,"MESH", MAKELANGID(LANG_RUSSIAN,SUBLANG_RUSSIAN_RUSSIA));
	if(!fr) return false;
	HGLOBAL res = LoadResource((HMODULE)lb, fr);
	if(!res) return false;
	LPVOID lr = LockResource(res);
	if(!lr) {return false;}
	char *pl = (char*)lr;
	//shapkasi mat pos/sc/eul
	pl += 20 + 72 + 36;//Consum thet, this mesh is not strip-mesh;
	nFaces = *((int*)pl); pl += 4;
	nVerts = *((int*)pl); pl += 4 + 6*4;//6-Sizes;

	//Vertex-Pixel shader ishlatmaslik uchun:
	verts = (VNT32*) malloc(nVerts*sizeof(VNT32));//verts = (VNT*) malloc(nVerts*sizeof(VNT));
	inds = (__int16*)malloc(2*3*nFaces);
	VNT *pv = (VNT*)pl;
	for(int i=0; i<nVerts; i++)
	{	verts[i].pos[0]=pv->pos[0];verts[i].pos[1]=pv->pos[1];verts[i].pos[2]=pv->pos[2];
		verts[i].normal[0] = pv->normal[0]/127.5f-1.0f;
		verts[i].normal[1] = pv->normal[1]/127.5f-1.0f;
		verts[i].normal[2] = pv->normal[2]/127.5f-1.0f;
		D3DXFloat16To32Array(verts[i].uv,(D3DXFLOAT16*)pv->uv,2);
		pv++;
	}
	__int16 *pi = (__int16*)pv;
	for(int i=0; i<nFaces*3; i++) inds[i] = *pi++;
	return true;
}

void Mesh::Draw(EfctThrTls* tls)
{
	//pd3dDevice->SetPixelShader (pSpecularPShader);
	//pd3dDevice->SetVertexShader(pVNTShader);
	//tls->pd3dDevice->SetFVF(D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX2);//pd3dDevice->SetVertexDeclaration(pVNTDecl);
	tls->pd3dDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST,
											0,
											nVerts,
											nFaces,
											inds,
											D3DFMT_INDEX16,
											verts,
											sizeof(VNT32));
	return;
}

void Mesh::FreeMem()
{
	if(verts)
		free(verts);
	if(inds)
		free(inds);
}
