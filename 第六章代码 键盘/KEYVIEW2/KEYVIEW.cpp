#include<windows.h>
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static DWORD dwCharSet = DEFAULT_CHARSET;          //字符集
	static int cxClientMax, cyClientMax, cxClient, cyClient, cxChar, cyChar;
	static int cLinesMax, cLines;
	static PMSG pmsg;
	static RECT rectScroll;
	static TCHAR szTop[] = TEXT("Message        Key       Char     ")
		TEXT("Repeat Scan Ext ALT Prev Tran");
	static TCHAR szUnd[] = TEXT("_______        ___       ____     ")
		TEXT("______ ____ ___ ___ ____ ____");

	static TCHAR * szFormat[2] = {

		TEXT("%-13s %3d %-15s%c%6u %4d %3s %3s %4s %4s"),
		TEXT("%-13s            0x%04X%1s%c %6u %4d %3s %3s %4s %4s") };
	static TCHAR * szYes = TEXT("Yes");
	static TCHAR * szNo = TEXT("No");
	static TCHAR * szDown = TEXT("Down");
	static TCHAR * szUp = TEXT("Up");

	static TCHAR * szMessage[] = {
		TEXT("WM_KEYDOWN"),    TEXT("WM_KEYUP"),
		TEXT("WM_CHAR"),       TEXT("WM_DEADCHAR"),
		TEXT("WM_SYSKEYDOWN"), TEXT("WM_SYSKEYUP"),
		TEXT("WM_SYSCHAR"),    TEXT("WM_SYSDEADCHAR")
	};
	HDC hdc;
	int i, itype;
	PAINTSTRUCT ps;
	TEXTMETRIC tm;
	TCHAR szBuffer[128], szKeyName[32];

	switch (message)
	{
	case WM_INPUTLANGCHANGE://表示输入法发生了改变，wparam:该输入法使用的字符集，lparam:该输入法的键盘布局
		dwCharSet = wParam;
	case WM_CREATE:
	case WM_DISPLAYCHANGE://表示显示器分辨率发生了改变，wparam:色深，像素位数，lparam:高低位分别对应垂直和水平分辨率
		cxClientMax = GetSystemMetrics(SM_CXMAXIMIZED);
		cyClientMax = GetSystemMetrics(SM_CYMAXIMIZED);
		hdc = GetDC(hwnd);
		SelectObject(hdc, CreateFont(0, 0, 0, 0, 0, 0, 0, 0, dwCharSet, 0, 0, 0, FIXED_PITCH, NULL));
		GetTextMetrics(hdc, &tm);
		cxChar = tm.tmAveCharWidth;
		cyChar = tm.tmHeight;
		DeleteObject(SelectObject(hdc, GetStockObject(SYSTEM_FONT)));
		ReleaseDC(hwnd, hdc);
		if (pmsg)
			free(pmsg);
		cLinesMax = cyClientMax / cyChar;
		pmsg = (PMSG)malloc(cLinesMax * sizeof(MSG));
		cLines = 0;
	case WM_SIZE:
		if (message == WM_SIZE)
		{
			cxClient = LOWORD(lParam);
			cyClient = HIWORD(lParam);
		}
		rectScroll.left = 0;
		rectScroll.right = cxClient;
		rectScroll.top = cyChar;
		rectScroll.bottom = cyChar*(cyClient / cyChar);
		InvalidateRect(hwnd, NULL, true);
		if (message == WM_INPUTLANGCHANGE)
			return true;
		return 0;
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_CHAR:
	case WM_DEADCHAR:
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
	case WM_SYSCHAR:
	case WM_SYSDEADCHAR:
		for (i = cLinesMax - 1; i>0; i--)
		{
			pmsg[i] = pmsg[i - 1];
		}
		pmsg[0].hwnd = hwnd;
		pmsg[0].message = message;
		pmsg[0].wParam = wParam;
		pmsg[0].lParam = lParam;
		cLines = min(cLines + 1, cLinesMax);
		ScrollWindow(hwnd, 0, -cyChar, &rectScroll, &rectScroll);    //将窗口向下滑动一行
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		SelectObject(hdc, CreateFont(0, 0, 0, 0, 0, 0, 0, 0, dwCharSet, 0, 0, 0, FIXED_PITCH, NULL));
		SetBkMode(hdc, TRANSPARENT);
		TextOut(hdc, 0, 0, szTop, lstrlen(szTop));
		TextOut(hdc, 0, 0, szUnd, lstrlen(szUnd));
		for (i = 0; i<min(cLines, cyClient / cyChar - 1); i++)
		{
			itype = pmsg[i].message == WM_CHAR ||
				pmsg[i].message == WM_SYSCHAR ||
				pmsg[i].message == WM_DEADCHAR ||
				pmsg[i].message == WM_SYSDEADCHAR;
			GetKeyNameText(pmsg[i].lParam, szKeyName, sizeof(szKeyName) / sizeof(TCHAR));//该函数检取表示键名的字符串。

			TextOut(hdc, 0, (cyClient / cyChar - 1 - i) * cyChar, szBuffer,
				wsprintf(szBuffer, szFormat[itype],
					szMessage[pmsg[i].message - WM_KEYFIRST],
					pmsg[i].wParam,
					(PTSTR)(itype ? TEXT(" ") : szKeyName),
					(TCHAR)(itype ? pmsg[i].wParam : ' '),
					LOWORD(pmsg[i].lParam),
					HIWORD(pmsg[i].lParam) & 0xFF,
					0x01000000 & pmsg[i].lParam ? szYes : szNo,
					0x20000000 & pmsg[i].lParam ? szYes : szNo,
					0x40000000 & pmsg[i].lParam ? szDown : szUp,
					0x80000000 & pmsg[i].lParam ? szUp : szDown));
		}
		DeleteObject(SelectObject(hdc, GetStockObject(SYSTEM_FONT)));
		EndPaint(hwnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

int WINAPI WinMain(__in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd)
{
	static TCHAR szAppName[] = TEXT("KEYVIEW2");
	HWND hwnd;
	WNDCLASS wndclass;
	MSG msg;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hInstance = hInstance;
	wndclass.lpfnWndProc = WndProc;
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.style = CS_VREDRAW | CS_HREDRAW;
	if (!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("注册类信息发生错误"), szAppName, MB_ICONERROR);
		return 0;
	}

	hwnd = CreateWindow(szAppName, TEXT("JUST A EXAMPLE"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT
		, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

	ShowWindow(hwnd, nShowCmd);
	UpdateWindow(hwnd);
	while (GetMessage(&msg, hwnd, NULL, NULL))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}
