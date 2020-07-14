#pragma once

#include "..\Include\d3dx9.h"

struct VD
{
	float pos[3];
	DWORD col;
};

struct VT
{
	float pos[3];
	float uv[2];
};

struct VNT
{
	float			pos		[3];
	unsigned char	normal	[4];
	__int16			uv		[2];//int16 as float16
};

struct VNT32
{
	float			pos		[3];
	float			normal	[3];
	float			uv		[2];//int16 as float16
};
