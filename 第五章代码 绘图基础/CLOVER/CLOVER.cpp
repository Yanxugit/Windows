
#include <windows.h>
#include <math.h>

#define TWO_PI	(2.0 * 3.14159)

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void DrawRectangle(HWND);

int cxClient, cyClient;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	static TCHAR	szAppName[] = TEXT("clover");
	HWND			hwnd;
	MSG				msg;
	WNDCLASS		wndclass;

	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;

	if (!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("Program requires Windows NT!"), szAppName, MB_ICONERROR);
		return 0;
	}

	hwnd = CreateWindow(szAppName, TEXT("Draw a Clover"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, hInstance, NULL);

	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HRGN		hRgnClip;
	static int		cxClient, cyClient;
	double			fAngle, fRadius;
	HCURSOR			hCursor;
	HDC				hdc;
	HRGN			hRgnTemp[6];
	int				i;
	PAINTSTRUCT		ps;

	switch (message)
	{
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);

		hCursor = SetCursor(LoadCursor(NULL, IDC_WAIT));
		ShowCursor(TRUE);

		if (hRgnClip)
			DeleteObject(hRgnClip);
		//CreateEllipticRgn创建一个椭圆，该椭圆与X1，Y1和X2，Y2坐标点确定的矩形内切
		hRgnTemp[0] = CreateEllipticRgn(0, cyClient / 3, cxClient / 2, 2 * cyClient / 3);
		hRgnTemp[1] = CreateEllipticRgn(cxClient / 2, cyClient / 3, cxClient, 2 * cyClient / 3);
		hRgnTemp[2] = CreateEllipticRgn(cxClient / 3, 0, 2 * cxClient / 3, cyClient / 2);
		hRgnTemp[3] = CreateEllipticRgn(cxClient / 3, cyClient / 2, 2 * cxClient / 3, cyClient);
		hRgnTemp[4] = CreateRectRgn(0, 0, 1, 1);
		hRgnTemp[5] = CreateRectRgn(0, 0, 1, 1);
		hRgnClip = CreateRectRgn(0, 0, 1, 1);
		//CLOVER 先创建 4 个椭圆区域，它们被存储在 hRgnTemp 数组的前 4 个元素中。接着程序创建三个“ 空 ” 区域：
		//在客户区左边和右边的两个椭圆区域先合并：
		CombineRgn(hRgnTemp[4], hRgnTemp[0], hRgnTemp[1], RGN_OR);
		//同样地，在客户区顶部和底部的两个椭圆区域也合并了：
		CombineRgn(hRgnTemp[5], hRgnTemp[2], hRgnTemp[3], RGN_OR);
		//最后两个合并后的区域再合并成 hRgnClip ：
		CombineRgn(hRgnClip, hRgnTemp[4], hRgnTemp[5], RGN_XOR);
		//RGN_XOR 标识符表示要从结果区域中排除重叠的区域。最后，6 个临时区域被删除：
		for (i = 0; i < 6; i++)
			DeleteObject(hRgnTemp[i]);

		SetCursor(hCursor);
		ShowCursor(FALSE);

		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		//相对结果而言，WM_PAINT 消息的处理很简单。视口原点设置在客户区的中心（这样使画直线更容易），在处理 WM_SIZE 消息时创建的区域被选入设备环境作为剪裁区域：
		SetViewportOrgEx(hdc, cxClient / 2, cyClient / 2, NULL);
		SelectClipRgn(hdc, hRgnClip);

		// hypot 计算直角三角形斜边的长
		//现在，剩下要做的就是画直线了，一共画 360 条，每一度画一条。每条线的长度是变量 fRadius ，它表示的是从中心到客户区角落的距离：
		fRadius = _hypot(cxClient / 2.0, cyClient / 2.0);

		for (fAngle = 0.0; fAngle < TWO_PI; fAngle += TWO_PI / 360)
		{
			MoveToEx(hdc, 0, 0, NULL);
			LineTo(hdc, int(fRadius * cos(fAngle) + 0.5), int(-fRadius * sin(fAngle) + 0.5));
		}
		EndPaint(hwnd, &ps);
		return 0;

	case WM_DESTROY:
		//在处理 WM_DESTROY 消息期间，剪裁区域被删除：
		DeleteObject(hRgnClip);

		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}