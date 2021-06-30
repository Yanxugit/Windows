#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT("KeyView1");
	HWND  hwnd;
	MSG  msg;
	WNDCLASS  wndclass;

	wndclass.style  = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc  = WndProc;
	wndclass.cbClsExtra  = 0;
	wndclass.cbWndExtra  = 0;
	wndclass.hInstance  = hInstance;
	wndclass.hIcon  = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor  = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName  = NULL;
	wndclass.lpszClassName = szAppName;

	if (!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("This program requires Windows NT!"),
			szAppName, MB_ICONERROR);
		return 0;
	}

	hwnd = CreateWindow(szAppName, TEXT("Keyboard Message Viewer #1"),
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
	static int  cxClientMax, cyClientMax, cxClient, cyClient, cxChar, cyChar;
	static int  cLinesMax, cLines;
	static PMSG  pmsg;
	static RECT  rectScroll;
	static TCHAR szTop[] = TEXT("Message        Key       Char     ")
		TEXT("Repeat Scan Ext ALT Prev Tran ");
	static TCHAR szUnd[] = TEXT("_______        ___       ____      ")
		TEXT("______ ____ ___ ___ ____ ____ ");

	static TCHAR * szFormat[2] = {
		TEXT("%-13s %3d %-15s%c%6u %4d %3s %3s %4s %4s "),
		TEXT("%-13s            0x%04X%1s%c %6u %4d %3s %3s %4s %4s ") };

	static TCHAR * szYes  = TEXT("Yes ");
	static TCHAR * szNo  = TEXT("No ");
	static TCHAR * szDown = TEXT("Down ");
	static TCHAR * szUp  = TEXT("Up ");

	static TCHAR * szMessage[] = {
		TEXT("WM_KEYDOWN "), TEXT("WM_KEYUP "),
		TEXT("WM_CHAR "), TEXT("WM_DEADCHAR "),
		TEXT("WM_SYSKEYDOWN "), TEXT("WM_SYSKEYUP "),
		TEXT("WM_SYSCHAR "), TEXT("WM_SYSDEADCHAR ") };
	HDC  hdc;
	int	 i, iType;
	PAINTSTRUCT	 ps;
	TCHAR	 szBuffer[128], szKeyName[32];
	TEXTMETRIC	 tm;

	switch (message)
	{
	case WM_CREATE:
	case WM_DISPLAYCHANGE:

		//获取客户端区域的最大大小

		cxClientMax = GetSystemMetrics(SM_CXMAXIMIZED);
		cyClientMax = GetSystemMetrics(SM_CYMAXIMIZED);



		// 获取固定间距字体的字符大小

		hdc = GetDC(hwnd);

		SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));
		GetTextMetrics(hdc, &tm);
		cxChar = tm.tmAveCharWidth;
		cyChar = tm.tmHeight;

		ReleaseDC(hwnd, hdc);



		// 为显示行分配内存

		if (pmsg)
			free(pmsg);

		cLinesMax = cyClientMax / cyChar;	 //窗口最大化时显示字符行
		pmsg = (PMSG)malloc(cLinesMax * sizeof(MSG));
		cLines = 0;
		// fall through  连续执行下面一个标签
	case WM_SIZE:
		if (message == WM_SIZE)
		{
			cxClient = LOWORD(lParam);
			cyClient = HIWORD(lParam);
		}


		// 计算滚动矩形

		rectScroll.left   = 0;
		rectScroll.right  = cxClient;
		rectScroll.top  = cyChar;
		rectScroll.bottom = cyChar * (cyClient / cyChar);  // 取(cyClient / cyChar)整数



		InvalidateRect(hwnd, NULL, TRUE);
		return 0;

	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_CHAR:
	case WM_DEADCHAR:
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
	case WM_SYSCHAR:
	case WM_SYSDEADCHAR:
		// 重新排列存储阵列
		for (i = cLinesMax - 1; i > 0; i--)
		{
			// 丢弃最后一个结构中的数据，预留第一个结构。
			// 可以这样理解，把全部结构数据向后顺移一位。
			pmsg[i] = pmsg[i - 1];
		}

		// 存储新消息

		pmsg[0].hwnd = hwnd;
		pmsg[0].message = message;
		pmsg[0].wParam = wParam;
		pmsg[0].lParam = lParam;

		//有按键消息来时，cLines加1，最大不超过窗口最大化时显示字符行
		cLines = min(cLines + 1, cLinesMax);
		// 向上滚动显示屏
		ScrollWindow(hwnd, 0, -cyChar, &rectScroll, &rectScroll);

		break;// 例如，调用DefWindowProc以便系统消息工作

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));


		//因为两个字符串绘画坐标一样，必须设定透明模式以防止第二个字符串擦除第一个字符串
		SetBkMode(hdc, TRANSPARENT);
		TextOut(hdc, 0, 0, szTop, lstrlen(szTop));
		TextOut(hdc, 0, 0, szUnd, lstrlen(szUnd));



		for (i = 0; i < min(cLines, cyClient / cyChar - 1); i++)
		{

			//如果是字符消息则标记 1 ，如果是按键消息则标记 0
			iType = pmsg[i].message == WM_CHAR ||
				pmsg[i].message == WM_SYSCHAR ||
				pmsg[i].message == WM_DEADCHAR ||
				pmsg[i].message == WM_SYSDEADCHAR;


			GetKeyNameText(pmsg[i].lParam, szKeyName, sizeof(szKeyName) / sizeof(TCHAR));

			TextOut(hdc, 0, (cyClient / cyChar - 1 - i) * cyChar, szBuffer, wsprintf(szBuffer, szFormat[iType],

				//计算得出消息类型，并索引。
				szMessage[pmsg[i].message - WM_KEYFIRST],

				//如果是按键消息就输出虚拟键代码，显示为十进制有符号整数，宽度为3个字符。
				//如果是字符消息就输出ANSI或Unicode字符代码，显示为大写十六进制数，宽度为4个字符，空白处用 0 填充。
				pmsg[i].wParam,

				//如果是按键消息就输出键名(szKeyName),并采用左对齐，宽度为15个字符。
				//如果是字符消息就输出空格，宽度为1个字符。
				(PTSTR)(iType ? TEXT(" ") : szKeyName),

				// 如果是按键消息就输出空格。
				// 如果是字符消息就输出该字符。pmsg[i].wParam = wParam （字符的对应编码，使用强制类型转换得到相应字符）
				(TCHAR)(iType ? pmsg[i].wParam : ' '),

				//低十六位记录着按键的重复计数次数。用十进制方式显示为无符号整数，宽度为6个字符。
				LOWORD(pmsg[i].lParam),

				//取高十六位中的低八位，解释为键盘扫描码(OEM),显示为十进制有符号整数，宽度为4个字符。
				HIWORD(pmsg[i].lParam) & 0xFF,

				// 0000 0001 0000 0000 0000 0000 0000 0000 读取 扩充键旗标(Extended Key Flag) 状态。
				0x01000000 & pmsg[i].lParam ? szYes  : szNo,

				// 0010 0000 0000 0000 0000 0000 0000 0000 读取 内容代码(Context Code) 状态。
				0x20000000 & pmsg[i].lParam ? szYes  : szNo,

				// 0100 0000 0000 0000 0000 0000 0000 0000 读取 键的先前状态(Previous Key State)
				0x40000000 & pmsg[i].lParam ? szDown : szUp,

				// 1000 0000 0000 0000 0000 0000 0000 0000 读取 转换状态(Transition State)
				0x80000000 & pmsg[i].lParam ? szUp  : szDown));
		}
		EndPaint(hwnd, &ps);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}