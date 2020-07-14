#include "windows.h"
#include "strsafe.h"

void errhandler(LPWSTR, HWND);


void CreateBMPFile(HWND hwnd, LPWSTR pszFile, PBITMAPINFO pbi, HBITMAP hBMP, HDC hDC)
{ 
HANDLE	hf;                 // file handle 
BITMAPFILEHEADER hdr;       // bitmap file-header 
PBITMAPINFOHEADER pbih;     // bitmap info-header 
LPBYTE	lpBits;             // memory pointer 
DWORD	dwTotal;            // total count of bytes 
DWORD	cb;                 // incremental count of bytes 
BYTE	*hp;                // byte pointer 
DWORD	dwTmp; 

	pbih = (PBITMAPINFOHEADER) pbi; 
    lpBits = (LPBYTE) LocalAlloc(GMEM_FIXED, pbih->biSizeImage);//Global edi

    if (!lpBits) errhandler(L"GlobalAlloc", hwnd); 

    // Retrieve the color table (RGBQUAD array) and the bits 
    // (array of palette indices) from the DIB. 
    if (!GetDIBits(hDC, hBMP, 0, (WORD) pbih->biHeight, lpBits, pbi, DIB_RGB_COLORS)) 
        errhandler(L"GetDIBits", hwnd); 

    // Create the .BMP file. 
    hf = CreateFile(pszFile, 
                   GENERIC_READ | GENERIC_WRITE, 
                   (DWORD) 0, 
                    NULL, 
                   CREATE_ALWAYS, 
                   FILE_ATTRIBUTE_NORMAL, 
                   (HANDLE) NULL); 
    if (hf == INVALID_HANDLE_VALUE) 
        errhandler(L"CreateFile", hwnd); 
    hdr.bfType = 0x4d42;        // 0x42 = "B" 0x4d = "M" 
    // Compute the size of the entire file. 
    hdr.bfSize = (DWORD) (sizeof(BITMAPFILEHEADER) + 
                 pbih->biSize + pbih->biClrUsed 
                 * sizeof(RGBQUAD) + pbih->biSizeImage); 
    hdr.bfReserved1 = 0; 
    hdr.bfReserved2 = 0; 

    // Compute the offset to the array of color indices. 
    hdr.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER) + 
                    pbih->biSize + pbih->biClrUsed 
                    * sizeof (RGBQUAD); 

    // Copy the BITMAPFILEHEADER into the .BMP file. 
    if (!WriteFile(hf, (LPVOID) &hdr, sizeof(BITMAPFILEHEADER), (LPDWORD) &dwTmp,  NULL))
       errhandler(L"WriteFile", hwnd); 

    // Copy the BITMAPINFOHEADER and RGBQUAD array into the file. 
    if (!WriteFile(hf, (LPVOID) pbih, sizeof(BITMAPINFOHEADER) 
                  + pbih->biClrUsed * sizeof (RGBQUAD), 
                  (LPDWORD) &dwTmp, ( NULL)))
        errhandler(L"WriteFile", hwnd); 

    // Copy the array of color indices into the .BMP file. 
    dwTotal = cb = pbih->biSizeImage; 
    hp = lpBits; 
    if (!WriteFile(hf, (LPSTR) hp, (int) cb, (LPDWORD) &dwTmp,NULL)) 
		errhandler(L"WriteFile", hwnd);

    // Close the .BMP file. 
     if (!CloseHandle(hf)) 
        errhandler(L"CloseHandle", hwnd); 

    // Free memory. 
    LocalFree(lpBits);//Global edi
}

PBITMAPINFO CreateBitmapInfoStruct(HWND hwnd, HBITMAP hBmp)
{ 
BITMAP		bmp; 
PBITMAPINFO pbmi; 
WORD		cClrBits; 

    // Retrieve the bitmap color format, width, and height. 
    if (!GetObject(hBmp, sizeof(BITMAP), (LPSTR)&bmp)) 
        errhandler(L"GetObject", hwnd); 

    // Convert the color format to a count of bits. 
    cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel);
    if (cClrBits == 1) 
        cClrBits = 1; 
    else if (cClrBits <= 4) 
        cClrBits = 4; 
    else if (cClrBits <= 8) 
        cClrBits = 8; 
    else if (cClrBits <= 16) 
        cClrBits = 16; 
    else if (cClrBits <= 24) 
        cClrBits = 24; 
    else cClrBits = 32; 

    // Allocate memory for the BITMAPINFO structure. (This structure 
    // contains a BITMAPINFOHEADER structure and an array of RGBQUAD 
    // data structures.) 
	if (cClrBits != 24)
		pbmi = (PBITMAPINFO) LocalAlloc(LPTR, 
                    sizeof(BITMAPINFOHEADER) + 
                    sizeof(RGBQUAD) * (1 << cClrBits)); 
    // There is no RGBQUAD array for the 24-bit-per-pixel format. 
    else 
		pbmi = (PBITMAPINFO) LocalAlloc(LPTR, 
                    sizeof(BITMAPINFOHEADER)); 

    // Initialize the fields in the BITMAPINFO structure. 

    pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER); 
    pbmi->bmiHeader.biWidth = bmp.bmWidth; 
    pbmi->bmiHeader.biHeight = bmp.bmHeight; 
    pbmi->bmiHeader.biPlanes = bmp.bmPlanes; 
    pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel; 
    if (cClrBits < 24) 
        pbmi->bmiHeader.biClrUsed = (1<<cClrBits); 

    // If the bitmap is not compressed, set the BI_RGB flag. 
    pbmi->bmiHeader.biCompression = BI_RGB; 

    // Compute the number of bytes in the array of color 
    // indices and store the result in biSizeImage. 
    // For Windows NT, the width must be DWORD aligned unless 
    // the bitmap is RLE compressed. This example shows this. 
    // For Windows 95/98/Me, the width must be WORD aligned unless the 
    // bitmap is RLE compressed.
    pbmi->bmiHeader.biSizeImage = ((pbmi->bmiHeader.biWidth * cClrBits +31) & ~31) /8
                                  * pbmi->bmiHeader.biHeight; 
    // Set biClrImportant to 0, indicating that all of the 
    // device colors are important. 
    pbmi->bmiHeader.biClrImportant = 0; 
    return pbmi; 
}

HBITMAP OpenBMPFile(LPTSTR pszFile, HDC compDC, HDC dc)
{ 
HANDLE	hf;                 // file handle 
BITMAPFILEHEADER hdr;       // bitmap file-header
BITMAPINFOHEADER bihdr;     // bitmap info-header
PBITMAPINFO		 pbmi;		//
LPBYTE	lpBits;             // memory pointer 
DWORD	dwTmp;
WORD	cClrBits;
HBITMAP hBMP;

	wchar_t p[MAX_PATH]; GetModuleFileName(NULL,p,MAX_PATH);
	wchar_t *pext = wcsrchr(p,'\\');
	if(pext) *pext = 0;
	StringCchCat(p,MAX_PATH,L"\\");
	StringCchCat(p,MAX_PATH,pszFile);
    // Open the .BMP file. 
    hf = CreateFile(p, 
                    GENERIC_READ,
                   (DWORD)0,
                    NULL,
                    OPEN_EXISTING,
                    FILE_ATTRIBUTE_READONLY,
                   (HANDLE)NULL);
    if(hf == INVALID_HANDLE_VALUE) return NULL;//errhandler("CreateFile", hwnd);

    // Read the BITMAPFILEHEADER from .BMP file.
    if(!ReadFile(hf, (LPVOID) &hdr, sizeof(BITMAPFILEHEADER), (LPDWORD) &dwTmp,  NULL))
	{ CloseHandle(hf); return NULL; }//errhandler("ReadFile", hwnd);
    if(0x4d42 != hdr.bfType) { CloseHandle(hf); return NULL; }// 0x42 = "B" 0x4d = "M" 

    // Read the BITMAPINFOHEADER and RGBQUAD array from file.
    if(!ReadFile(hf, &bihdr, sizeof(BITMAPINFOHEADER),//+ pbih->biClrUsed * sizeof (RGBQUAD),
                  (LPDWORD)&dwTmp, ( NULL)))
		{ CloseHandle(hf); return NULL; }//errhandler("WriteFile", hwnd);

	cClrBits = (WORD)(bihdr.biPlanes * bihdr.biBitCount);
    if (cClrBits == 1) 
        cClrBits = 1; 
    else if (cClrBits <= 4) 
        cClrBits = 4; 
    else if (cClrBits <= 8) 
        cClrBits = 8; 
    else if (cClrBits <= 16) 
        cClrBits = 16; 
    else if (cClrBits <= 24) 
        cClrBits = 24; 
    else cClrBits = 32; 

	if(bihdr.biBitCount != 24)
		pbmi=(PBITMAPINFO)LocalAlloc(LPTR,sizeof(BITMAPINFOHEADER)+sizeof(RGBQUAD)*(1<<cClrBits));
	else pbmi = (PBITMAPINFO)LocalAlloc(LPTR, sizeof(BITMAPINFOHEADER)); 
	memcpy(pbmi, &bihdr, sizeof(BITMAPINFOHEADER));

    if(!ReadFile(hf,(LPVOID)((char*)pbmi+sizeof(BITMAPINFOHEADER)),(1<<cClrBits)*sizeof(RGBQUAD),
                  (LPDWORD)&dwTmp, ( NULL)))
		{ CloseHandle(hf); return NULL; }//errhandler("WriteFile", hwnd);

	lpBits = (LPBYTE)LocalAlloc(LPTR,bihdr.biSizeImage);
	if(!lpBits) { CloseHandle(hf); return NULL; }//errhandler("WriteFile", hwnd);

	if(!ReadFile(hf, (LPSTR)lpBits, bihdr.biSizeImage, (LPDWORD)&dwTmp, NULL))
		{ CloseHandle(hf); return NULL; }

    // Close the .BMP file.
	if(!CloseHandle(hf)) return NULL;

	hBMP = CreateDIBitmap(compDC,(BITMAPINFOHEADER*)&bihdr,CBM_INIT,lpBits,pbmi,
		(bihdr.biBitCount==4 || bihdr.biBitCount==8)?DIB_PAL_COLORS:DIB_RGB_COLORS);
	SelectObject(dc, hBMP);

	//Checking:
	//if(GetDIBits(dc,hBMP,0,bihdr.biHeight,lpBits,pbmi,DIB_RGB_COLORS))
	//	hBMP =hBMP;

	/*for(int y=0; y<bihdr.biHeight; y++)
	for(int x=0; x<bihdr.biWidth; x++)
	{
		DWORD rgb = GetPixel(dc,x,y);
		if(rgb)
			rgb = 0;
	}*/
	LocalFree(pbmi); LocalFree(lpBits);

	return hBMP;
}

void errhandler(LPWSTR lpszFunction, HWND h)
{ 
    // Retrieve the system error message for the last-error code

    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
    DWORD dw = GetLastError(); 

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0, NULL );

    // Display the error message and exit the process

    lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, 
        (lstrlen((LPCTSTR)lpMsgBuf)+lstrlen((LPCTSTR)lpszFunction)+40)*sizeof(TCHAR)); 
    StringCchPrintf((LPTSTR)lpDisplayBuf, 
        LocalSize(lpDisplayBuf),
        TEXT("%s failed with error %d: %s"), 
        lpszFunction, dw, lpMsgBuf); 
    MessageBox(h, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK); 

    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);
    //ExitProcess(dw); 
}
