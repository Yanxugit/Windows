#include <Windows.h>
#include <math.h>

LRESULT CALLBACK WndPorc(HWND, UINT, WPARAM, LPARAM);//设置回调函数  WndProc函数的返回值类型为LRESULT。该类型等价于LONG.


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hprevInstance, PSTR szCmdLine, int iCmdShow)
{
	static WCHAR szAppName[] = TEXT("SineWave");  //定义app名称

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
		TEXT("Sine Wave Using PolyLine!"), //窗口标题
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
	static POINT aptFigure[10] = {10,70,50,70,50,10,90,10,90,50,30,50,30,90,70,90,70,30,10,30};
	static int cxClient, cyClient;

	int i;
	HDC          hdc;
	PAINTSTRUCT  ps;
    POINT        apt[10];
	//TEXTMETRIC  tm;
	//RECT         rect;

	switch (message)
	{

	case  WM_SIZE:
		cxClient = LOWORD(lparam);//客户区的长度
		cyClient = HIWORD(lparam);//客户区的高度
		
		return 0;
	
	case WM_PAINT:   //绘制窗口
		hdc = BeginPaint(hwnd, &ps);  //Device Context设备上下文
		//SelectObject
		//把一个对象(位图、画笔、画刷等)选入指定的设备描述表。新的对象代替同一类型的老对象
		SelectObject(hdc,GetStockObject(GRAY_BRUSH));//GetStockObject()该函数检索预定义的备用笔、刷子、字体或者调色板的句柄

		for (i = 0; i < 10; i++) 
		{
			apt[i].x = cxClient * aptFigure[i].x / 200;
			apt[i].y = cyClient * aptFigure[i].y / 100;
		}
	   //参数iMode:其有两种填充模式:ALTERNATE和WINDING；在默认情况下为ALTERNATE;
	   //模式ALTERNATE:其从封闭区域中的一个点向无穷远处水平画一条射线，只有当该射线穿越奇数条边框线时，封闭区域才被填充，如为偶数，则不填充该区域；
	   //模式WINDING：方法一样，如为奇数，填充该区域；如为偶数则要根据边框线的方向来判断：如果穿过的边框线在不同方向的边框线数目相等，则不填充，如不等，则填充。
		SetPolyFillMode(hdc,ALTERNATE);
		Polygon(hdc,apt,10);//该函数画一个由直线相闻的两个以上顶点组成的多边形，用当前画笔画多边形轮廓，用当前画刷和多边形填充模式填充多边形

		for (i = 0; i < 10; i++)
		{
			apt[i].x += cxClient / 2;
			
		}
		SetPolyFillMode(hdc, WINDING);
		Polygon(hdc, apt, 10);

		EndPaint(hwnd, &ps);
		return 0;


	case WM_DESTROY:  //处理窗口关闭的消息
		PostQuitMessage(0);
		return 0;

	}
	return DefWindowProc(hwnd, message, wparam, lparam);

}