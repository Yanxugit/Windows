#define WINVER 0x0500
#include <windows.h>
#include "sysmets.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT("SysMets3");
	HWND  hwnd;
	MSG msg;
	WNDCLASS wndclass;

	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc  = WndProc;
	wndclass.cbClsExtra  = 0;
	wndclass.cbWndExtra  = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon  = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName  = NULL;
	wndclass.lpszClassName = szAppName;

	if (!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("Program requires Windows NT!"),
			szAppName, MB_ICONERROR);
		return 0;
	}

	hwnd = CreateWindow(szAppName, TEXT("Get System Metrics No. 3"),
		WS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_HSCROLL,//水平和垂直滚动条
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
	static int cxChar, cxCaps, cyChar, cxClient, cyClient, iMaxWidth;
	HDC  hdc;
	int  i, x, y, iVertPos, iHorzPos, iPaintBeg, iPaintEnd;
	PAINTSTRUCT ps;
	SCROLLINFO  si;
	TCHAR szBuffer[10];
	TEXTMETRIC  tm;

	switch (message)
	{
	case WM_CREATE:

		//字体初始化和滚动条初始化
		hdc = GetDC(hwnd);

		GetTextMetrics(hdc, &tm);
		cxChar = tm.tmAveCharWidth;
		cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2) * cxChar / 2;
		cyChar = tm.tmHeight + tm.tmExternalLeading;

		ReleaseDC(hwnd, hdc);

		// Save the width of the three columns

		iMaxWidth = 40 * cxChar + 22 * cxCaps; //水平滚动的最大范围
		return 0;

	case WM_SIZE:

		//获取客户区大小
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);

		// 设置垂直滚动条的范围和每页的大小

		//注意要使用setscrollinfo和getscrollinfo函数，则必须在使用之前初始化SI结构体中cbsize和fmask成员

		//同时还得初始化根据fmask指定的成员，

		si.cbSize = sizeof(si); //结构体大小
		si.fMask  = SIF_RANGE | SIF_PAGE; //指定了要初始化滚动条范围和每页大小
		si.nMin  = 0;
		si.nMax  = NUMLINES - 1; //滚动条范围
		si.nPage  = cyClient / cyChar; // 每页大小

			//初始化后就可以使用这些函数了
			SetScrollInfo(hwnd, SB_VERT, &si, TRUE);

		// 水平滚动条范围和每页大小

		si.cbSize = sizeof(si);
		si.fMask  = SIF_RANGE | SIF_PAGE;
		si.nMin  = 0;
		si.nMax  = 2 + iMaxWidth / cxChar;
		si.nPage  = cxClient / cxChar;
		SetScrollInfo(hwnd, SB_HORZ, &si, TRUE);
		return 0;

	case WM_VSCROLL:
		// Get all the vertial scroll bar information

		si.cbSize = sizeof(si);
		si.fMask  = SIF_ALL;//获取结构体全部信息
		GetScrollInfo(hwnd, SB_VERT, &si);

		// Save the position for comparison later on

		iVertPos = si.nPos;

		switch (LOWORD(wParam))
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
		// Set the position and then retrieve it.  Due to adjustments
		//   by Windows it may not be the same as the value set.

		si.fMask = SIF_POS;//这里没只指定结构体大小是因为在上面已经指定过了
		SetScrollInfo(hwnd, SB_VERT, &si, TRUE);
		GetScrollInfo(hwnd, SB_VERT, &si);

		// If the position has changed, scroll the window and update it

		if (si.nPos != iVertPos) //判断滚动条的位置
		{
			ScrollWindow(hwnd, 0, cyChar * (iVertPos - si.nPos),
				NULL, NULL);//使得文本在垂直方向滚动

			UpdateWindow(hwnd);//立即更新，产生WM_PAINT消息（不进入队列）
		}
		return 0;

	case WM_HSCROLL:
		//水平滚动条相关信息，和垂直滚动条类似

		si.cbSize = sizeof(si);
		si.fMask  = SIF_ALL;

		// 设置位置，然后检索它。由于调整SBY窗口，它可能与设置的值不相同。

		GetScrollInfo(hwnd, SB_HORZ, &si);
		iHorzPos = si.nPos;

		switch (LOWORD(wParam))
		{
		case SB_LINELEFT:
			si.nPos -= 1;
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
		// 设置位置，然后检索它。由于调整SBY窗口，它可能与设置的值不相同。
		//   by Windows it may not be the same as the value set.

		si.fMask = SIF_POS;
		SetScrollInfo(hwnd, SB_HORZ, &si, TRUE);
		GetScrollInfo(hwnd, SB_HORZ, &si);

		// 如果位置发生变化，请将窗口跟新

		if (si.nPos != iHorzPos)
		{
			ScrollWindow(hwnd, cxChar * (iHorzPos - si.nPos), 0,
				NULL, NULL);
		}
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		// 获取垂直滚动条位置

		si.cbSize = sizeof(si);
		si.fMask  = SIF_POS;
		GetScrollInfo(hwnd, SB_VERT, &si);
		iVertPos = si.nPos;

		// 获取水平滚动条位置

		GetScrollInfo(hwnd, SB_HORZ, &si);
		iHorzPos = si.nPos;

		// 这句话不怎么好理解，体味流程就可以了，不要纠结细节，知道改变了水平或垂直滚动条位置，文本会跟着滚动就可以了

		iPaintBeg = max(0, iVertPos + ps.rcPaint.top / cyChar);
		iPaintEnd = min(NUMLINES - 1,
			iVertPos + ps.rcPaint.bottom / cyChar);

		for (i = iPaintBeg; i <= iPaintEnd; i++)
		{
			x = cxChar * (1 - iHorzPos);
			y = cyChar * (i - iVertPos);

			TextOut(hdc, x, y,
				sysmetrics[i].szLabel,
				lstrlen(sysmetrics[i].szLabel));

			TextOut(hdc, x + 22 * cxCaps, y,
				sysmetrics[i].szDesc,
				lstrlen(sysmetrics[i].szDesc));

			SetTextAlign(hdc, TA_RIGHT | TA_TOP);

			TextOut(hdc, x + 22 * cxCaps + 40 * cxChar, y, szBuffer,
				wsprintf(szBuffer, TEXT("%5d"),
					GetSystemMetrics(sysmetrics[i].Index)));

			SetTextAlign(hdc, TA_LEFT | TA_TOP);
		}

		EndPaint(hwnd, &ps);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}