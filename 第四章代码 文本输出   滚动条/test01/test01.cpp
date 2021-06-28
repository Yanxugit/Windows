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
		WS_OVERLAPPEDWINDOW,              //窗口样式
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
	static int cxChar,cxCaps, cYChar;
	TCHAR szBuffer[10];

	static int cxClient, cyClient; //定义变量表示宽和高

	switch (message)
	{

	case WM_SIZE:
		cxClient = LOWORD(lparam);
		cyClient = HIWORD(lparam);
		//StringCchPrintf(szBuffer,128,TEXT("分辨率是 %d  *  %d px"),cxClient,cyClient);

		return 0;
	case  WM_CREATE: // 当要获取系统的字体时
		//PlaySound(TEXT("Helloein.wav"), NULL, SND_FILENAME | SND_ASYNC);
		hdc = GetDC(hwnd);//获取窗口设备环境

		GetTextMetrics(hdc,&tm);//获取默认系统字体的尺寸
		cxChar = tm.tmAveCharWidth; //平均字符宽度
		/*SYSMETS1还将大写字符的平均宽度保存在静态变量 cxCaps 中。
			在等宽字体中，cxCaps等于cxChar。在变宽字体中，cxCaps设为cxChar 的1.5倍。
			在TEXTMETRIC结构中，tmPitchAndFamily字段的低位决定字体是否为等宽字体:1代表变宽字体，0代表等宽字体。
			SYSMETS1利用这个来计算 cxCaps :*/
		cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2)* cxChar / 2;


		cYChar = tm.tmHeight + tm.tmExternalLeading;//字符的总高度


		ReleaseDC(hwnd,hdc);//销毁
	

		return 0;
	case WM_PAINT:   //绘制窗口
		hdc = BeginPaint(hwnd, &ps);  //Device Context设备上下文
									  //GetClientRect(hwnd,&rect);  //获得客户区的位置坐标

									  //DrawText(hdc,TEXT("这是阎旭的第一个Windows窗口程序！"),-1,&rect,DT_SINGLELINE | DT_CENTER  | DT_VCENTER);

		//TextOut(hdc, 400, 300, TEXT("Yan Xu King!"), 12);
		for (int i = 0; i < NUMLINES; i++)
		{
			//  cYChar * i  这个参数指定了每行字符的顶部相对于客户区的顶部的位置。

				//TEXTOUT是显示文本的最重要的GDI函数。它的语法如下:
				//TextOut(hdc,x,y,psText,iLength);

			TextOut(hdc,0,cYChar * i,	sysmetrics[i].szLabel,lstrlen(sysmetrics[i].szLabel));

			TextOut(hdc, 22*cxCaps, cYChar * i, sysmetrics[i].szDesc, lstrlen(sysmetrics[i].szDesc));



			SetTextAlign(hdc,TA_RIGHT  | TA_TOP); //显示位置
			/*通知 Windows后继的TextOut函数调用使用的坐标将从客户区的右上角开始，而不是从左上角开始。*/
			
			/*其中 40xcxChar 的值包括了第二栏和第三栏的宽度。*/

			TextOut(hdc, 22 * cxCaps + 40 * cxChar, cYChar * i, szBuffer, wsprintf(szBuffer,TEXT("%5d"),GetSystemMetrics(sysmetrics[i].Index)));
			
			
			//在调用了这个 TextOut后，另一个SetTextAlign函数调用将对齐方式设回为正常方式。
			SetTextAlign(hdc,TA_LEFT | TA_TOP); //显示位置

		}

		
		EndPaint(hwnd, &ps);
		return 0;

		//鼠标左键按钮按下后弹出消息框
	case WM_LBUTTONDOWN:
		MessageBox(NULL, TEXT("我是左边我被按下了！"), TEXT("提示"), 0);
		return 0;

		//鼠标右键按钮按下后弹出消息框
	case WM_RBUTTONDOWN:
		MessageBox(NULL, TEXT("我是右边我被按下了！"), TEXT("提示"), 0);
		return 0;
	case WM_DESTROY:  //处理窗口关闭的消息
		PostQuitMessage(0);
		return 0;

	}
	return DefWindowProc(hwnd, message, wparam, lparam);

}