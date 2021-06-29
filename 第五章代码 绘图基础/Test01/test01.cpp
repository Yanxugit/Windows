#include <Windows.h>
#define NUMLINES ((int) (sizeof(devcaps) / sizeof(devcaps[0])))
 struct
 {
	     int iIndex;
	    TCHAR *szLabel;
		TCHAR *szDesc;
	 }
devcaps[] =
 {
	     HORZSIZE, TEXT("HORZSIZE"), TEXT("Width in millimeters:"),
		 VERTSIZE, TEXT("VERTSIZE"), TEXT("Height in milimeters:"),
		 HORZRES, TEXT("HORZRES"), TEXT("Width in pixels:"),
		 VERTRES, TEXT("VERTRES"), TEXT("Height in raster lines:"),
		 BITSPIXEL, TEXT("BITSPIXEL"), TEXT("Color bits per pixel:"),
		 PLANES, TEXT("PLANES"), TEXT("Number of color planes:"),
		 NUMBRUSHES, TEXT("NUMBRUSHES"), TEXT("Number of device brushes"),
		 NUMPENS, TEXT("NUMPENS"), TEXT("Number of device pens:"),
		 NUMMARKERS, TEXT("NUMMARKERS"), TEXT("Number of device markers:"),
		 NUMFONTS, TEXT("NUMFONTS"), TEXT("Number of device fonts:"),
		 NUMCOLORS, TEXT("NUMCOLORS"), TEXT("Number of device colors:"),
		 PDEVICESIZE, TEXT("PDEVICESIZE"), TEXT("Size of device structure:"),
		 ASPECTX, TEXT("ASPECTX"), TEXT("Relative width of pixel:"),
		 ASPECTY, TEXT("ASPECTY"), TEXT("Relative height of pixel:"),
		 ASPECTXY, TEXT("ASPECTXY"), TEXT("Relative diagonal of pixel:"),
		 LOGPIXELSX, TEXT("LOGPIXELSX"), TEXT("Horizontal dots per inch:"),
		 LOGPIXELSY, TEXT("LOGPIXELSY"), TEXT("Vertical dots per inch:"),
		 SIZEPALETTE, TEXT("SIZEPALETTE"), TEXT("Number of palette entries:"),
		 NUMRESERVED, TEXT("NUMRESERVED"), TEXT("Reserved palette entries:"),
		 COLORRES, TEXT("COLORRES"), TEXT("Actual color resolution:")
		 };
LRESULT CALLBACK WndPorc(HWND, UINT, WPARAM, LPARAM);//设置回调函数  WndProc函数的返回值类型为LRESULT。该类型等价于LONG.


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hprevInstance, PSTR szCmdLine, int iCmdShow)
{
	static WCHAR szAppName[] = TEXT("Devcaps1");  //定义app名称

	HWND     hwnd;
	MSG      msg;
	WNDCLASS wndclass;
	
	wndclass.style = CS_HREDRAW | CS_VREDRAW ;						//指定窗口类型，各种“类风格”（详见下方↓）可以使用按位或操作符组合起来
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
		TEXT("画直线"), //窗口标题
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
	static int cxChar, cyChar, cxCaps;
	TCHAR szBuffer[10];
	int i;
	HDC          hdc;
	PAINTSTRUCT  ps;

	TEXTMETRIC  tm;
	//RECT         rect;

	switch (message)
	{

	case  WM_CREATE: // 当要获取系统的字体时
		hdc = GetDC(hwnd);

		GetTextMetrics(hdc, &tm);
		cxChar = tm.tmAveCharWidth;
		cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2)*cxChar / 2;
		cyChar = tm.tmHeight + tm.tmExternalLeading;

		ReleaseDC(hwnd, hdc);
		return 0;
	case WM_PAINT:   //绘制窗口
		hdc = BeginPaint(hwnd, &ps);  //Device Context设备上下文

		GetClientRect();

			//for (i = 0; i != NUMLINES; ++i)
			//	       {
			//	           TextOut(hdc, 0, cyChar * i, devcaps[i].szLabel,lstrlen(devcaps[i].szLabel));
			//	         TextOut(hdc, 14 * cxCaps, cyChar * i, devcaps[i].szDesc, lstrlen(devcaps[i].szDesc));
			//	         SetTextAlign(hdc, TA_RIGHT | TA_TOP);
			//	         TextOut(hdc, 14 * cxCaps + 35 * cxChar, cyChar * i, szBuffer, wsprintf(szBuffer, TEXT("%5d"), GetDeviceCaps(hdc, devcaps[i].iIndex)));
			//	           SetTextAlign(hdc, TA_LEFT | TA_TOP);
			//	       }
		

		EndPaint(hwnd, &ps);
		return 0;

	
	case WM_DESTROY:  //处理窗口关闭的消息
		PostQuitMessage(0);
		return 0;

	}
	return DefWindowProc(hwnd, message, wparam, lparam);

}