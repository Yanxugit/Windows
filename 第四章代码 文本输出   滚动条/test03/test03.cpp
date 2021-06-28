#include <Windows.h>
#include "sysmets.h"


LRESULT CALLBACK WndPorc(HWND, UINT, WPARAM, LPARAM);//设置回调函数  WndProc函数的返回值类型为LRESULT。该类型等价于LONG.


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hprevInstance, PSTR szCmdLine, int iCmdShow)
{
	static WCHAR szAppName[] = TEXT("HelloWindows");  //定义app名称

	HWND     hwnd;
	MSG      msg;
	WNDCLASS wndclass;

	wndclass.style = CS_HREDRAW | CS_VREDRAW;						//指定窗口类型，各种“类风格”（详见下方↓）可以使用按位或操作符组合起来
	wndclass.lpfnWndProc = WndPorc;									//指定窗口过程（必须是回调函数） *
																	//下面两个字段用于在类结构和 Windows内部维护的窗口结构中预留
																	//一些额外的空间:
	wndclass.cbClsExtra = 0;										//预留的额外空间，一般为 0
	wndclass.cbWndExtra = 0;										//预留的额外空间，一般为 0
																	//接下来的字段表示应用程序的实例句柄(是WinMain的一个参数):
	wndclass.hInstance = hInstance;									//应用程序的实例句柄
																	//下面的语句为所有基于该窗口类的窗口设定一个图标:
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);				//为所有基于该窗口类的窗口设定一个图标
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);					//为所有基于该窗口类的窗口设定一个鼠标指针
																	/*
																	下一个字段为这类窗口的客户区指定了背景色。
																	字段名hbrBackground 的前缀hbr表示“画刷的句柄”(handle to a brush)。
																	画刷是一个图形学术语，表示用于区域填充的像素着色模式。
																	Windows有几个标准的画刷，又称“库存”画刷。
																	下面对GetStockObject的调用返回一个白色画刷的句柄:
																	*/
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	//指定窗口背景色
																	//接下来的一个字段指定了窗口类的菜单。
																	//由于程序 HELLOWIN不带任何菜单，所以该字段被设为NULL:
	wndclass.lpszMenuName = NULL;									//指定窗口菜单
																	//最后，必须为窗口类赋予一个名称。对一个小程序来说，
																	//这个名称可以简单地用程序名表示，
																	//如保存在变量szAppName中的字符串“HelloWin”:
	wndclass.lpszClassName = szAppName;								//指定窗口类名　 *

	if (!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("This program  requires Windows NT!"), szAppName, MB_ICONERROR);
		return 0;
	}

	hwnd = CreateWindow(szAppName,                        //窗口类名
		TEXT("THIS  IS  YANXU  WINDOWS"), //窗口标题
		WS_OVERLAPPEDWINDOW | WS_VSCROLL| WS_HSCROLL,              //窗口样式   WS_VSCROLL 垂直滚动条
		CW_USEDEFAULT,	                  //初始x位置
		CW_USEDEFAULT,                    //初始y位置
		CW_USEDEFAULT,                    //初始x尺寸
		CW_USEDEFAULT,                    //初始y尺寸
		NULL,                             //父窗口句柄
		NULL,							  //窗口菜单句柄
		hInstance,						  //程序实例句柄
		NULL                              //创建参数
	);


	ShowWindow(hwnd, iCmdShow);  //显示窗口 函  数用于设置窗口的显示状态。
	UpdateWindow(hwnd);  //函数绕过应用程序的消息队列，直接发送 WM_PAINT 消息给指定窗口的窗口过程


	while (GetMessage(&msg, NULL, 0, 0))   //函数的作用是从当前线程的消息队列里获取一个消息并填入 MSG 结构 中。
	{
		TranslateMessage(&msg);  //函数将虚拟键消息转换为字符消息，字符消息被寄送到当前线程的消息队列里。
		DispatchMessage(&msg);  //函数分派一个消息给窗口过程（回调函数），通常该消息从 GetMessage 函数获得。Windows 的控制权在该函数交给了应用程序。
	}


	return msg.wParam;
}

LRESULT CALLBACK WndPorc(HWND  hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	HDC          hdc;
	PAINTSTRUCT  ps;
	//RECT         rect;
	TEXTMETRIC   tm;
	static int cxChar,cxCaps, cyChar,cxClient, cyClient,iMaxWidth ;
	TCHAR szBuffer[10];
	int i,x,y,iVertPos,iHorzPos,iPaintBeg,iPaintEnd;
	SCROLLINFO si;
	
	

	switch (message)
	{

	
	case  WM_CREATE: // 当要获取系统的字体时
		//PlaySound(TEXT("Helloein.wav"), NULL, SND_FILENAME | SND_ASYNC);
		hdc = GetDC(hwnd);//获取窗口设备环境

		GetTextMetrics(hdc,&tm);//获取默认系统字体的尺寸
		cxChar = tm.tmAveCharWidth; //平均字符宽度
		cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2)* cxChar / 2;
		cyChar = tm.tmHeight + tm.tmExternalLeading;//字符的总高度
		ReleaseDC(hwnd,hdc);//销毁

		//保存三列的宽度
		iMaxWidth = 40 * cxChar + 22 * cxCaps;
		

		return 0;

	case WM_SIZE:
		//这两个宏返回WORD值，也就是16位无符号整数(从0到0xFFFF)。
		//通常你会将这些值保存在32位有符号整型变量中。
		//这并不会牵涉到任何转换问题，而且将来在计算时会比较方便。

		cxClient = LOWORD(lparam);
		cyClient = HIWORD(lparam);
		//设置垂直条范围和页面大小
		si.cbSize = sizeof(si);
		si.fMask = SIF_RANGE | SIF_PAGE;
		si.nMin = 0;
		si.nMax = NUMLINES -1;
		si.nPage = cyClient / cyChar;
		SetScrollInfo(hwnd,SB_VERT,&si,TRUE);

		//设置水平条范围和页面大小
		si.cbSize = sizeof(si);
		si.fMask = SIF_RANGE | SIF_PAGE;
		si.nMin = 0;
		si.nMax = 2+iMaxWidth/cxChar;
		si.nPage = cxClient / cxChar;
		SetScrollInfo(hwnd, SB_HORZ, &si, TRUE);

		return 0;

		
	case WM_VSCROLL:
		//获取所有垂直滚动条信息
		si.cbSize = sizeof(si);
		si.fMask = SIF_ALL;
		GetScrollInfo(hwnd,SB_VERT,&si);
		//保存位置以便以后比较
		iVertPos = si.nPos;
		switch (LOWORD(wparam))
		{
		case SB_TOP:
			si.nPos = si.nMin;
			break;
		case SB_BOTTOM:
			si.nPos = si.nMax;
			break;
		case SB_LINEUP:
			si.nPos -= 1;
			break;
		case SB_LINEDOWN:
			si.nPos += 1;
			break;
		case SB_PAGEUP:
			si.nPos -= si.nPage;
			break;
		case SB_PAGEDOWN:
			si.nPos += si.nPage;
			break;
		case SB_THUMBTRACK:
			si.nPos = si.nTrackPos;
			break;
		default:
			break;


		}

		//设置位置，然后检索它。由于调整//
		//在Windows下，它可能与设置的值不同。
		si.fMask = SIF_POS;
		SetScrollInfo(hwnd,SB_VERT,&si,TRUE);
		GetScrollInfo(hwnd,SB_VERT,&si);

		//如果位置已更改，请滚动窗口并进行更新
		if (si.nPos != iVertPos) 
		{
			ScrollWindow(hwnd,0,cyChar * (iVertPos-si.nPos),NULL,NULL);
			UpdateWindow(hwnd);
		}
		return 0;

	case WM_HSCROLL:
		///水平滚动条相关信息，和垂直滚动条类似
		si.cbSize = sizeof(si);
		si.fMask = SIF_ALL;
		//保存位置以便以后比较
		GetScrollInfo(hwnd,SB_HORZ,&si);
		iHorzPos = si.nPos;

		switch (LOWORD(wparam))
		{
		case SB_LINELEFT:
			si.nPos -=1;
			break;
		case SB_LINERIGHT:
			si.nPos += 1;
			break;
		case SB_PAGELEFT:
			si.nPos -= si.nPage;
			break;
		case SB_PAGERIGHT:
			si.nPos += si.nPage;
			break;
		case SB_THUMBPOSITION:
			si.nPos = si.nTrackPos;
			break;
		default:
			break;
		}
		//设置位置，然后检索它。由于调整SBY窗口，它可能与设置的值不相同。
		si.fMask = SIF_POS;
		SetScrollInfo(hwnd,SB_HORZ,&si,TRUE);
		GetScrollInfo(hwnd,SB_HORZ,&si);

		//如果位置发生变化，请将窗口跟新
		if (si.nPos != iHorzPos)
		{
			ScrollWindow(hwnd,cxChar * (iHorzPos - si.nPos),0,NULL,NULL);
			//UpdateWindow(hwnd);
		}
		return 0;

	case WM_PAINT:   //绘制窗口
		hdc = BeginPaint(hwnd, &ps);  
			//获取垂直滚动条位置
		si.cbSize = sizeof(si);
		si.fMask = SIF_POS;
		GetScrollInfo(hwnd,SB_VERT,&si);
		iVertPos = si.nPos;
		//获取水平滚动条位置
		GetScrollInfo(hwnd, SB_HORZ, &si);
		iHorzPos = si.nPos;
		//寻找绘画极限
		iPaintBeg = max(0,iVertPos+ps.rcPaint.top/cyChar);
		iPaintEnd = min(NUMLINES - 1, iVertPos + ps.rcPaint.bottom / cyChar);

		
		//TextOut(hdc, 400, 300, TEXT("Yan Xu King!"), 12);
		for ( i = iPaintBeg; i <= iPaintEnd; i++)
		{
			//计算x,y值位置
			x = cxChar*(1 - iHorzPos);
			y = cyChar *(i - iVertPos);


			TextOut(hdc,x,y,	sysmetrics[i].szLabel,lstrlen(sysmetrics[i].szLabel));
			TextOut(hdc,x + 22 * cxCaps, y, sysmetrics[i].szDesc, lstrlen(sysmetrics[i].szDesc));
			SetTextAlign(hdc,TA_RIGHT  | TA_TOP); //显示位置
			/*通知 Windows后继的TextOut函数调用使用的坐标将从客户区的右上角开始，而不是从左上角开始。*/
			
			/*其中 40xcxChar 的值包括了第二栏和第三栏的宽度。*/
			TextOut(hdc,x + 22 * cxCaps + 40 * cxChar, y, szBuffer, wsprintf(szBuffer,TEXT("%5d"),GetSystemMetrics(sysmetrics[i].Index)));
			//在调用了这个 TextOut后，另一个SetTextAlign函数调用将对齐方式设回为正常方式。
			SetTextAlign(hdc,TA_LEFT | TA_TOP); //显示位置

		}
		EndPaint(hwnd, &ps);
		return 0;

	case WM_DESTROY:  //处理窗口关闭的消息
		PostQuitMessage(0);
		return 0;

	}
	return DefWindowProc(hwnd, message, wparam, lparam);

}