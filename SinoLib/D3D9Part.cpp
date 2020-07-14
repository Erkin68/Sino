#include "WndEfctWithD3D9.h"
#include "strsafe.h"
#include "..\Config.h"
#include "..\Sino.h"

struct CUSTOMVERTEX
{
    FLOAT position[3];// The position
    FLOAT tu, tv;     // The texture coordinates
};
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_TEX1)

LPDIRECT3D9 pD3D = NULL;
extern void errhandler(LPTSTR, HWND);

POINT rgPts[4];

FLOAT Dot(FLOAT *p1, FLOAT *p2)
{
	return (p1[0]*p2[0] + p1[1]*p2[1] + p1[2]*p2[2]);
}
VOID Cross(FLOAT *res, FLOAT *a, FLOAT *b)
{
	res[0] = a[1] * b[2] - a[2] * b[1];
	res[1] = a[2] * b[0] - a[0] * b[2];
	res[2] = a[0] * b[1] - a[1] * b[0];
	return;
} 

VOID lookAtLH(FLOAT *m, FLOAT *peye, FLOAT *pcenter, FLOAT *pup)
{
	FLOAT x[3], y[3], z[3];
	FLOAT mag;

	z[0] = pcenter[0] - peye[0];//vView
	z[1] = pcenter[1] - peye[1];//vView
	z[2] = pcenter[2] - peye[2];//vView

	mag = sqrtf(Dot(&z[0],&z[0]));
	if(0.001f<mag)//0.0f ham 0x000000000 b-di;
	{	mag = 1.0f / mag;
		z[0] *= mag;
		z[1] *= mag;
		z[2] *= mag;
	}

	y[0] = pup[0];//vUp
	y[1] = pup[1];
	y[2] = pup[2];

	Cross(&x[0], &y[0], &z[0]);//vRight
	Cross(&y[0], &z[0], &x[0]);//recalc vUp 

	mag = sqrtf(Dot(&x[0], &x[0]));
	if(0.001f<mag)
	{	mag = 1.0f / mag;
		x[0] *= mag;
		x[1] *= mag;
		x[2] *= mag;
	}

	mag = sqrtf(Dot(&y[0], &y[0]));
	if(mag) 
	{	mag = 1.0f / mag;
		y[0] *= mag;
		y[1] *= mag;
		y[2] *= mag;
	}

	m[0] = x[0];
	m[1] = y[0];
	m[2] = z[0];
	m[3] = 0.0f;

	m[4] = x[1];
	m[5] = y[1];
	m[6] = z[1];
	m[7] = 0.0f;

	m[8] = x[2];
	m[9] = y[2];
	m[10]= z[2];
	m[11]= 0.0f;

	FLOAT row1[3], row2[3], row3[3];

	row1[0] = peye[0] * x[0];
	row1[1] = peye[1] * x[1];
	row1[2] = peye[2] * x[2];

	row2[0] = peye[0] * y[0];
	row2[1] = peye[1] * y[1];
	row2[2] = peye[2] * y[2];

	row3[0] = peye[0] * z[0];
	row3[1] = peye[1] * z[1];
	row3[2] = peye[2] * z[2];

	m[12]= - row1[0] - row1[1] - row1[2];
	m[13]= - row2[0] - row2[1] - row2[2];
	m[14]= - row3[0] - row3[1] - row3[2];;
	m[15]= 1.0f;

	return;
}

VOID PerspectiveFovLH(FLOAT *m,FLOAT fovY,FLOAT Aspect,FLOAT zn,FLOAT zf)
{
	FLOAT yScale = 1.0f / tan(0.5f * fovY);
	FLOAT xScale = yScale / Aspect;

	m[0] = xScale;
	m[4] = 0.0f;
	m[8] = 0.0f;
	m[12]= 0.0f;

	m[1] = 0.0f;
	m[5] = yScale;
	m[9] = 0.0f;
	m[13]= 0.0f;

	m[2] = 0.0f;
	m[6] = 0.0f;
	m[10]= zf/(zf-zn);
	m[14]= -zn*zf/(zf-zn);

	m[3] = 0.0f;
	m[7] = 0.0f;
	m[11]= 1;
	m[15]= 0.0f;
}

VOID m_matF_x_matF(FLOAT *mresult, FLOAT *a, FLOAT *b)
{
FLOAT m[16];
	m[0] = a[0]*b[0] + a[1]*b[4] + a[2]*b[8 ] + a[3]*b[12];
	m[1] = a[0]*b[1] + a[1]*b[5] + a[2]*b[9 ] + a[3]*b[13];
	m[2] = a[0]*b[2] + a[1]*b[6] + a[2]*b[10] + a[3]*b[14];
	m[3] = a[0]*b[3] + a[1]*b[7] + a[2]*b[11] + a[3]*b[15];

	m[4] = a[4]*b[0] + a[5]*b[4] + a[6]*b[8 ] + a[7]*b[12];
	m[5] = a[4]*b[1] + a[5]*b[5] + a[6]*b[9 ] + a[7]*b[13];
	m[6] = a[4]*b[2] + a[5]*b[6] + a[6]*b[10] + a[7]*b[14];
	m[7] = a[4]*b[3] + a[5]*b[7] + a[6]*b[11] + a[7]*b[15];

	m[8] = a[8]*b[0] + a[9]*b[4] + a[10]*b[8 ] + a[11]*b[12];
    m[9] = a[8]*b[1] + a[9]*b[5] + a[10]*b[9 ] + a[11]*b[13];
	m[10]= a[8]*b[2] + a[9]*b[6] + a[10]*b[10] + a[11]*b[14];
	m[11]= a[8]*b[3] + a[9]*b[7] + a[10]*b[11] + a[11]*b[15];

	m[12] = a[12]*b[0] + a[13]*b[4] + a[14]*b[8 ] + a[15]*b[12];
	m[13] = a[12]*b[1] + a[13]*b[5] + a[14]*b[9 ] + a[15]*b[13];
	m[14]= a[12]*b[2] + a[13]*b[6] + a[14]*b[10] + a[15]*b[14];
	m[15]= a[12]*b[3] + a[13]*b[7] + a[14]*b[11] + a[15]*b[15];

	for(int i=0; i<16; i++)
		mresult[i] = m[i];
	return;
}

VOID MatrixScaling(FLOAT *m, FLOAT scx, FLOAT scy, FLOAT scz)
{
	m[0] = scx;
	m[1] = 0.0f;
	m[2] = 0.0f;
	m[3] = 0.0f;

	m[4] = 0.0f;
	m[5] = scy;
	m[6] = 0.0f;
	m[7] = 0.0f;

	m[8] = 0.0f;
	m[9] = 0.0f;
	m[10]= scz;
	m[11]= 0.0f;

	m[12]= 0.0f;
	m[13]= 0.0f;
	m[14]= 0.0f;
	m[15]= 1.0f;
}

//extern "C" {
//VOID m_sincos_ASM(FLOAT*,FLOAT*,FLOAT);

VOID m_sincos_ASM(FLOAT *s, FLOAT *c, FLOAT angle)
{
	*s = sinf(angle);
	*c = cosf(angle);
/*__asm	{
			mov     eax, c
			fld     angle
			fsincos
			fstp    dword ptr [eax]
			mov     eax, s
			fstp    dword ptr [eax]
}*/		}
 
VOID MatrixRotationY(FLOAT *m, FLOAT ry)
{
FLOAT cy,sy;

	m_sincos_ASM(&sy, &cy, ry);

    m[0] = cy;
	m[1] = 0.f;
    m[2] = -sy;
	m[3] = 0.f;

	m[4] = 0.f;
	m[5] = 1.f;
	m[6] = 0.f;
	m[7] = 0.f;

    m[8] = sy;
	m[9] = 0.f;
    m[10]= cy;
	m[11]= 0.f;

	m[12]= 0.f;
	m[13]= 0.f;
	m[14]= 0.f;
	m[15]= 1.f;
}

VOID m_tmatF_x_point4F(FLOAT *presult, FLOAT *m, FLOAT *p)
{
F32 r[4];
	r[0] = m[0 ]*p[0] + m[4 ]*p[1] + m[8 ]*p[2] + m[12]*p[3];   
	r[1] = m[1 ]*p[0] + m[5 ]*p[1] + m[9 ]*p[2] + m[13]*p[3];   
	r[2] = m[2 ]*p[0] + m[6 ]*p[1] + m[10]*p[2] + m[14]*p[3];   
	r[3] = m[3 ]*p[0] + m[7 ]*p[1] + m[11]*p[2] + m[15]*p[3];   
	presult[0] = r[0];
	presult[1] = r[1];
	presult[2] = r[2];
	presult[3] = r[3];
}

HRESULT InitD3D(EfctThrTls *tls)
{
    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory( &d3dpp, sizeof(d3dpp) );
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;

	if( FAILED(pD3D->CreateDevice(	D3DADAPTER_DEFAULT,
									D3DDEVTYPE_HAL,
									tls->hEfctWnd,
									D3DCREATE_SOFTWARE_VERTEXPROCESSING,
									&d3dpp,
									&tls->pd3dDevice ) ) )
		return E_FAIL;

	tls->pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	tls->pd3dDevice->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
	tls->pd3dDevice->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
	tls->pd3dDevice->SetSamplerState(0,D3DSAMP_MIPFILTER,D3DTEXF_LINEAR);

	DWORD txtrWidth=tls->width,txtrHeight=tls->height;
	txtrWidth = (txtrWidth + 255) & (~255);
	txtrHeight = (txtrHeight + 255) & (~255);
	if(txtrWidth > conf::MaxTextureWidth) txtrWidth = conf::MaxTextureWidth;
	if(txtrHeight > conf::MaxTextureHeight) txtrHeight = conf::MaxTextureHeight;
	if(txtrHeight > txtrWidth)
		txtrWidth = txtrHeight;
	else if(txtrHeight < txtrWidth)
		txtrHeight = txtrWidth;

	//Load texture from HBITMAP:
	if(FAILED(tls->pd3dDevice->CreateTexture(txtrWidth,
											txtrHeight,
											1,
											0,
											D3DFMT_X8R8G8B8,
											D3DPOOL_MANAGED,
											&tls->pTexture,
											NULL)))
	if(FAILED(tls->pd3dDevice->CreateTexture(txtrWidth,
											txtrHeight,
											1,
											0,
											D3DFMT_R8G8B8,
											D3DPOOL_MANAGED,
											&tls->pTexture,
											NULL)))
	if(FAILED(tls->pd3dDevice->CreateTexture(txtrWidth,
											txtrHeight,
											1,
											0,
											D3DFMT_A8R8G8B8,
											D3DPOOL_MANAGED,
											&tls->pTexture,
											NULL)))
		return E_FAIL;
	IDirect3DSurface9 *pSurf;
	if(D3D_OK==tls->pTexture->GetSurfaceLevel(0,&pSurf))
	{	HDC dc; if(D3D_OK!=pSurf->GetDC(&dc)) return E_FAIL;
		if(!StretchBlt(	dc,0,0,txtrWidth,txtrHeight,
						tls->bmpDC,0,0,tls->width,tls->height,SRCCOPY))//if(!BitBlt(dc,0,0,txtrWidth,txtrHeight,tls->bmpDC,0,0,SRCCOPY))
			return E_FAIL;
		pSurf->ReleaseDC(dc); pSurf->Release();
	}
	//D3DXSaveTextureToFile("Txtr.dds",D3DXIFF_DDS,tls->pTexture,NULL);

	//Checking:
	/*D3DLOCKED_RECT rc;
	if(D3D_OK==tls->pTexture->LockRect(0, &rc, NULL, 0))
	{	BYTE* pDestBytes = (BYTE*) rc.pBits;
        for( int y=0; y<tls->height; y++ )
        {	DWORD* pDest = (DWORD*)pDestBytes;
            for( int x=0; x < tls->width; x++ )
            {	COLORREF rgb = GetPixel(tls->bmpDC,x,y);
				*pDest = rgb;
				if(rgb)
					rgb = 0;
            }
            pDestBytes += rc.Pitch;
        } tls->pTexture->UnlockRect(0);
    }*/
	D3DXVECTOR3 vEyePt(-1.0f,0.0f,-11.0f);
	D3DXVECTOR3 vLookatPt(-1.0f,0.0f,0.0f);
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f );
	lookAtLH((FLOAT*)&tls->matView, (FLOAT*)&vEyePt, (FLOAT*)&vLookatPt, (FLOAT*)&vUpVec);

	D3DXMATRIX t,t1;
	//D3DXMatrixLookAtLH(&t, &vEyePt, &vLookatPt, &vUpVec);

	PerspectiveFovLH((FLOAT*)&tls->matProj,D3DX_PI/2,1.0f,0.01f,40.0f);
	//D3DXMatrixPerspectiveFovLH(&t,D3DX_PI/2,1.0f,0.1f,100.0f);

	tls->mViewPort = D3DXMATRIXA16(0.5f*tls->width, 0.0f            ,0.0f,0.0f,
							       0.0f           ,-0.5f*tls->height,0.0f,0.0f,
								   0.0f           , 0.0f            ,1.0f,0.0f,
								   0.5f*tls->width, 0.5f*tls->height,0.0f,1.0f);
	tls->pd3dDevice->SetTransform(D3DTS_VIEW,&tls->matView);
	tls->pd3dDevice->SetTransform(D3DTS_PROJECTION,&tls->matProj);

	m_matF_x_matF(t, tls->matView, tls->matProj);
	m_matF_x_matF(tls->mViewPort, t, tls->mViewPort);

	tls->pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
	tls->pd3dDevice->SetTexture( 0, tls->pTexture );
	tls->pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);/*	tls->pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	D3DMATERIAL9 mtrl;
	ZeroMemory( &mtrl, sizeof(mtrl) );
	mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
	mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
	mtrl.Diffuse.b = mtrl.Ambient.b = 0.0f;
	mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;
	tls->pd3dDevice->SetMaterial( &mtrl );
	tls->pd3dDevice->SetRenderState( D3DRS_AMBIENT, 0xfff0f0f0 );
	tls->pd3dDevice->LightEnable( 0, TRUE);

	D3DXVECTOR3 vecDir;
	D3DLIGHT9 light;
	ZeroMemory( &light, sizeof(light) );
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Diffuse.r = 1.0f;
	light.Diffuse.g = 1.0f;
	light.Diffuse.b = 1.0f;
	vecDir = D3DXVECTOR3(0,0,-1);
	D3DXVec3Normalize( (D3DXVECTOR3*)&light.Direction, &vecDir );
	light.Range = 150000.0f;
	tls->pd3dDevice->SetLight(0, &light);*/

	//tls->pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	//tls->pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	//tls->pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//tls->pd3dDevice->SetRenderState(D3DRS_COLORVERTEX, FALSE);
    //tls->pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    //tls->pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
    //tls->pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
    //tls->pd3dDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
    //tls->pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    /*tls->pd3dDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);*/
    //tls->pd3dDevice->SetRenderState(D3DRS_ALPHAREF, 0x00);
    //tls->pd3dDevice->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, FALSE);
    //tls->pd3dDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_BORDER);
    //tls->pd3dDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_BORDER);
	//tls->pd3dDevice->SetSamplerState(0, D3DSAMP_BORDERCOLOR, 0xff8fc8ff);
	//tls->pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE,TRUE);
    tls->pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
 
    return S_OK;
}

VOID CleanupD3D(EfctThrTls *tls)
{
	if( tls->pTexture != NULL )
		tls->pTexture->Release();
	if( tls->pd3dDevice != NULL ) 
        tls->pd3dDevice->Release();
	tls->pd3dDevice = NULL;
}

CUSTOMVERTEX verts[4] = {-10.0f, -10.0f, 0.0f,		0.0f, 1.0f,
						 -10.0f,  10.0f, 0.0f,		0.0f, 0.0f,
						  10.0f, -10.0f, 0.0f,		1.0f, 1.0f,
						  10.0f,  10.0f, 0.0f,		1.0f, 0.0f};
D3DXVECTOR4 cverts[4] = {D3DXVECTOR4(-10.0f, -10.0f, 0.0f, 1.0f),
						 D3DXVECTOR4(-10.0f,  10.0f, 0.0f, 1.0f),
						 D3DXVECTOR4( 10.0f,  10.0f, 0.0f, 1.0f),
						 D3DXVECTOR4( 10.0f, -10.0f, 0.0f, 1.0f)};
D3DXVECTOR4 trlVerts[4];
struct SCREEN_VERTEX 
{   FLOAT x, y, z, rhw;
    FLOAT       tu, tv;
    static const DWORD FVF;
};
const DWORD SCREEN_VERTEX::FVF = D3DFVF_XYZRHW | D3DFVF_TEX1;
SCREEN_VERTEX svQuad[4] = {	0.0f, 0.0f, 0.0f, 1.0f,	0.0f,  0.0f,
							1.0f, 0.0f, 0.0f, 1.0f,	1.01f, 0.0f,
						 	0.0f, 1.0f, 0.0f, 1.0f,	0.0f,  1.0f,
							1.0f, 1.0f, 0.0f, 1.0f,	1.01f, 1.0f};
VOID Render(EfctThrTls *tls)
{
	D3DXMATRIXA16 matScale,matRot,matWorld;
	MatrixScaling((FLOAT*)&matScale,tls->interpTime,tls->interpTime,tls->interpTime);
	//D3DXMatrixScaling(&matScale,tls->interpTime,tls->interpTime,tls->interpTime);

	MatrixRotationY((FLOAT*)&matRot,1.7f-1.7f*tls->interpTime);
	//D3DXMatrixRotationY(&matRot,1.7f-1.7f*tls->interpTime);

	m_matF_x_matF(matWorld, matScale, matRot);
	//matWorld = matScale * matRot;








	D3DXMATRIX matRgn,t;
	m_matF_x_matF(matRgn, matWorld, tls->mViewPort);//tls->matView * tls->matProj * tls->mViewPort;

	for(int v=0; v<4; v++)
	{	//D3DXVECTOR4 V = (1.7f-tls->interpTime)*cverts[v] + 1.7f*tls->interpTime*cverts1[v];
		m_tmatF_x_point4F(trlVerts[v],matRgn,cverts[v]);
	}

	svQuad[0].x = trlVerts[1].x / trlVerts[1].w;
	svQuad[0].y = trlVerts[1].y / trlVerts[1].w;

    svQuad[1].x = trlVerts[2].x / trlVerts[2].w;
    svQuad[1].y = trlVerts[2].y / trlVerts[2].w;

    svQuad[2].x = trlVerts[0].x / trlVerts[0].w;
    svQuad[2].y = trlVerts[0].y / trlVerts[0].w;

    svQuad[3].x = trlVerts[3].x / trlVerts[3].w;
    svQuad[3].y = trlVerts[3].y / trlVerts[3].w;

	rgPts[0].x = (int)(svQuad[2].x);
	rgPts[0].y = (int)(svQuad[2].y);
	rgPts[1].x = (int)(svQuad[0].x);
	rgPts[1].y = (int)(svQuad[0].y);
	rgPts[2].x = (int)(svQuad[1].x);
	rgPts[2].y = (int)(svQuad[1].y);
	rgPts[3].x = (int)(svQuad[3].x);
	rgPts[3].y = (int)(svQuad[3].y);

	HRGN rgn = CreatePolygonRgn(rgPts,4,WINDING);
	SetWindowRgn(tls->hEfctWnd,rgn,TRUE);
	DeleteObject(rgn);



	//tls->pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
    //tls->pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    //tls->pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
    //tls->pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );


	tls->pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET,//|D3DCLEAR_ZBUFFER,
							0xff8fc8ff, 1.0f, 0 );
    if( SUCCEEDED( tls->pd3dDevice->BeginScene() ) )
    {	tls->pd3dDevice->SetTransform(D3DTS_WORLD,&matWorld);
		tls->pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,
										 2,
										 verts,
										 sizeof(CUSTOMVERTEX));
		// For more info see the doc page titled "Directly Mapping Texels to Pixels"
  
		//tls->pd3dDevice->SetFVF(SCREEN_VERTEX::FVF);
		//tls->pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2,
		//								 svQuad, sizeof(SCREEN_VERTEX));
        tls->pd3dDevice->EndScene();
    }
    tls->pd3dDevice->Present( NULL, NULL, NULL, NULL );
}
