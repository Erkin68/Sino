#pragma once

#define MAX_SUBMENU_ELEM_TEXT			64	//submenu har 1 elementi max chari
#define HEIGHT_OF_EACH_SUBMENUELEMENT	30	//submenu har 1 elementchasi bo'yi
#define SUBMENU_LEFT_SPACE				40	//eng ko'p widthidan shuncha o'ngga

class SubMenu
{
public:

static bool CloseAnimatedSubMenu(int);
static bool InitEachSubMenu(long,long,int,int,char*,...);   //init each sub;
static bool OpenAnimatedSubMenu (int);
static bool ReserveTotalSubMenus(int);//first reserv total;
static void SetChecked  (int,int,bool);
static void SetDisabled (int,int,bool);
static void SetImaged   (int,int,bool);
static void SetParented (int,int,bool);
static void SetSeparated(int,int,bool);
static int  iTotal;

			SubMenu();
		   ~SubMenu();

	typedef struct TElement
	{	char caption[MAX_SUBMENU_ELEM_TEXT];
		typedef struct TType
		{	int	checked   : 1;
			int parent    : 1;
			int imaged    : 1;
			int separator : 1;
			int disabled  : 1;
		} Type;
		Type type;
		int ln;
		int txtExtnt[2];
		long hotKey;//1-byte->Ctrl,2-byte->Alt,3-byte->char;
	} Element;
	Element *elements;
	int  totElems;
	int	 staticWidth ;//width in static time without animate
	int	 staticHeight;
	int  iAnimate;//animate time
	HWND hwnd;
};
extern SubMenu *subMenus;
