#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <mmsystem.h>



const char g_szClassName[] = "cWind";




LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow){

	
	
	WNDCLASSEX wc;
	HWND hwnd,hwnd2;
	MSG Msg;

	//Step 1: Registering the Window Class
	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.style		 = 0;
	wc.lpfnWndProc	 = WndProc;
	wc.cbClsExtra	 = 0;
	wc.cbWndExtra	 = 0;
	wc.hInstance	 = hInstance;
	wc.hIcon		 = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszMenuName	 = NULL;
	wc.lpszClassName = g_szClassName;
	wc.hIconSm		 = LoadIcon(NULL, IDI_APPLICATION);

	if(!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "Window Registration Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	// Step 2: Creating the Window
	hwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,//Window styles (optional)
		g_szClassName,//Class name
		"Counter",//Window title
		WS_OVERLAPPEDWINDOW,//Window style
		CW_USEDEFAULT, //X position
		CW_USEDEFAULT, //Y position
		230, //Width
		206, //Height
		NULL, //Parent Window
		NULL, //Menu
		hInstance, //Instance handle
		NULL //Additional application data
		);
		

	if(hwnd == NULL)
	{
		MessageBox(NULL, "Window Creation Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}
	if(hwnd2 == NULL)
	{
		MessageBox(NULL, "Window Creation Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}
	
	ShowWindow(hwnd, SW_SHOW);
	ShowWindow(GetConsoleWindow(), SW_HIDE);
	UpdateWindow(hwnd);
	// Step 3: The Message Loop
	while(GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg.wParam;
}

void calcFont();
void initComponents(HWND hwnd);
HWND txtB,minB,addB;
HFONT font=CreateFont(100,0,0,0,FW_NORMAL,0,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FF_DONTCARE,"Calibri");
HFONT bfont=CreateFont(50,0,0,0,FW_NORMAL,0,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FF_DONTCARE,"Calibri");
int font_width,font_height;
int ww,wh;
int state=0;
int fontS=100;
char str[] = "0";

void set(int val){
	state+=val;
	if(state<0){
		state=0;
	}
	sprintf(str,"%d",state);
	
	SetWindowText(txtB,str);
	calcFont();
	MoveWindow(txtB,ww/2-font_width/2,10,ww,wh-80,true);
}

void del(){
	state=0;
	sprintf(str,"%d",state);
	
	SetWindowText(txtB,str);
	calcFont();
	MoveWindow(txtB,ww/2-font_width/2,10,ww,wh-80,true);
}



// Step 4: the Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//printf("MESSAGE:%d WPARAM:%d LPARAM:%X\n",msg,wParam,lParam);
	
	switch(msg)
	{
		case WM_KEYDOWN:
			int keyCode;
			keyCode=wParam;
			if(keyCode==40){
				set(-1);
			}else if(keyCode==38){
				set(1);
			}else if(keyCode==82){
				del();
			}
		break;
		
		case WM_MOUSEWHEEL:
			//int fwKeys = GET_KEYSTATE_WPARAM(wParam);
			//int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
			int scroll;
			scroll=(short)HIWORD(wParam);
			int ctrlDown;
			ctrlDown=LOWORD(wParam)>>3&0xf;
			
			if(ctrlDown){
				if(scroll>0){
					fontS+=5;
				}else{
					if(fontS>10){
						fontS-=5;
					}
				}
				font=CreateFont(fontS,0,0,0,FW_NORMAL,0,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FF_DONTCARE,"Calibri");
				SendMessage(txtB,WM_SETFONT,(WPARAM)font,MAKELPARAM(true,0));
				calcFont();
				MoveWindow(txtB,ww/2-font_width/2,10,ww,wh-80,true);
			}else{
				if(scroll>0){
					set(1);
				}else{
					set(-1);
				}
			}
			
			break;
		case WM_COMMAND:
			if(wParam==252){
				set(-1);
			}
			if(wParam==253){
				set(1);
			}
			
			
		break;
		case WM_CREATE:
			initComponents(hwnd);
			
			{
			
			SendMessage(txtB,WM_SETFONT,(WPARAM)font,MAKELPARAM(true,0));
			SendMessage(minB,WM_SETFONT,(WPARAM)bfont,MAKELPARAM(true,0));
			SendMessage(addB,WM_SETFONT,(WPARAM)bfont,MAKELPARAM(true,0));
			}
			
		break;
		case WM_CLOSE:
			//ShowWindow(hwnd, 2);
			
			DestroyWindow(hwnd);
		break;
		case WM_DESTROY:
			
			PostQuitMessage(0);
		
		break;
		
		case WM_SIZE:
		calcFont();
			//printf("%d %d\n",LOWORD(lParam),HIWORD(lParam));
			ww=LOWORD(lParam);
			wh=HIWORD(lParam);
			MoveWindow(txtB,LOWORD(lParam)/2-font_width/2,10,LOWORD(lParam),HIWORD(lParam)-80,true);
			MoveWindow(minB,ww/4-25,HIWORD(lParam)-50,50,25,true);
			MoveWindow(addB,3*ww/4-25,HIWORD(lParam)-50,50,25,true);
			
			//printf("%d %d\n",LOWORD(lParam),HIWORD(lParam));
		break;
		case WM_CTLCOLORSTATIC:
			SetTextColor((HDC)wParam,RGB(0,0,0));
			SetBkMode((HDC)wParam,TRANSPARENT);
			//SetTextAlign((HDC)wParam, TA_RIGHT);
			
			
			return (LRESULT)GetStockObject(WHITE_BRUSH);
		case WM_CTLCOLOREDIT:
			SetTextColor((HDC)wParam,RGB(0,255,0));
			SetBkMode((HDC)wParam,TRANSPARENT);
			return (LRESULT)GetStockObject(BLACK_BRUSH);
		
			
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
	
	
}

void calcFont(){
		HDC heDC = GetDC(NULL);
			LOGFONT logfont;
			GetObject(font, sizeof(LOGFONT), &logfont);
			HFONT hNewFont = CreateFontIndirect(&logfont);
			SelectObject(heDC, hNewFont);
			RECT re = { 0, 0, 0, 0 };
			char str[] = "00:00";
			sprintf(str,"%d",state);
			DrawText(heDC, str, strlen(str), &re, DT_CALCRECT);
			//printf("%d %d\n",re.right,re.bottom);
			font_width=re.right;
			font_height=re.bottom;
			DeleteObject(hNewFont);
	}
void initComponents(HWND hwnd){
	txtB=CreateWindow(
			"static",
			"0",
			WS_VISIBLE |WS_CHILD,
			20,20,
			250,100,
			hwnd, NULL, NULL, NULL);
			
			minB = CreateWindow(
			"BUTTON",	// Predefined class; Unicode assumed 
			"-",		// Button text 
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,	// Styles 
			20,			// x position 
			40,		// y position 
			50,		// Button width
			25,			// Button height
			hwnd,		// Parent window
			(HMENU)252, // Assign appropriate control ID
			(HINSTANCE)GetWindowLong(hwnd, GWLP_HINSTANCE),
			NULL);		// Pointer not needed.
			addB = CreateWindow(
			"BUTTON",	// Predefined class; Unicode assumed 
			"+",		// Button text 
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,	// Styles 
			20,			// x position 
			40,		// y position 
			50,		// Button width
			25,			// Button height
			hwnd,		// Parent window
			(HMENU)253, // Assign appropriate control ID
			(HINSTANCE)GetWindowLong(hwnd, GWLP_HINSTANCE),
			NULL);
}