#include <windows.h>

#define BUFFER(x,y) *(pBuffer + y * cxBuffer + x)    //定义了一个缓冲区，要访问缓冲区的内容，需要知道行和列的位置，才能访问具体某个位置的内容

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT("Typer");
	HWND         hwnd;
	MSG          msg;
	WNDCLASS     wndclass;

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
		MessageBox(NULL, TEXT("This program requires Windows NT!"),
			szAppName, MB_ICONERROR);
		return 0;
	}

	hwnd = CreateWindow(szAppName, TEXT("Typing Program"),
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
	static DWORD   dwCharSet = DEFAULT_CHARSET;
	static int     cxChar, cyChar, cxClient, cyClient, cxBuffer, cyBuffer,
		xCaret, yCaret;
	static TCHAR * pBuffer = NULL;
	HDC            hdc;
	int            x, y, i;
	PAINTSTRUCT    ps;
	TEXTMETRIC     tm;

	switch (message)
	{
	case WM_INPUTLANGCHANGE:
		dwCharSet = wParam;

	case WM_CREATE:                    //相当于初始化，获取字体相关信息
		hdc = GetDC(hwnd);
		SelectObject(hdc, CreateFont(0, 0, 0, 0, 0, 0, 0, 0,
			dwCharSet, 0, 0, 0, FIXED_PITCH, NULL));   //创建一种字体

		GetTextMetrics(hdc, &tm);
		cxChar = tm.tmAveCharWidth;
		cyChar = tm.tmHeight;

		DeleteObject(SelectObject(hdc, GetStockObject(SYSTEM_FONT)));
		ReleaseDC(hwnd, hdc);
		// fall through               
	case WM_SIZE:
		// obtain window size in pixels

		if (message == WM_SIZE)
		{
			cxClient = LOWORD(lParam);
			cyClient = HIWORD(lParam);
		}
		// calculate window size in characters

		cxBuffer = max(1, cxClient / cxChar);
		cyBuffer = max(1, cyClient / cyChar);

		// allocate memory for buffer and clear it

		if (pBuffer != NULL)
			free(pBuffer);

		pBuffer = (TCHAR *)malloc(cxBuffer * cyBuffer * sizeof(TCHAR));//缓冲区大小：行和列相乘，在乘每一个点对于的大小

		for (y = 0; y < cyBuffer; y++)
			for (x = 0; x < cxBuffer; x++)
				BUFFER(x, y) = ' ';                      //对缓冲区每个点初始化为空

														 // set caret to upper left corner

		xCaret = 0;
		yCaret = 0;

		if (hwnd == GetFocus())
			SetCaretPos(xCaret * cxChar, yCaret * cyChar);

		InvalidateRect(hwnd, NULL, TRUE);
		return 0;

	case WM_SETFOCUS:                                //窗体获得焦点，在wm_create和wm_size，wm_paint消息后被调用
													 // create and show the caret

		CreateCaret(hwnd, NULL, cxChar, cyChar);
		SetCaretPos(xCaret * cxChar, yCaret * cyChar);
		ShowCaret(hwnd);               //会一直的闪烁，直到碰到setcaretpos函数，重新确定光标的坐标
		return 0;

	case WM_KILLFOCUS:
		// hide and destroy the caret

		HideCaret(hwnd);
		DestroyCaret();
		return 0;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_HOME:
			xCaret = 0;
			break;

		case VK_END:
			xCaret = cxBuffer - 1;
			break;

		case VK_PRIOR:
			yCaret = 0;
			break;

		case VK_NEXT:
			yCaret = cyBuffer - 1;
			break;

		case VK_LEFT:
			xCaret = max(xCaret - 1, 0);
			break;

		case VK_RIGHT:
			xCaret = min(xCaret + 1, cxBuffer - 1);
			break;

		case VK_UP:
			yCaret = max(yCaret - 1, 0);
			break;

		case VK_DOWN:
			yCaret = min(yCaret + 1, cyBuffer - 1);
			break;

		case VK_DELETE:
			for (x = xCaret; x < cxBuffer - 1; x++)
				BUFFER(x, yCaret) = BUFFER(x + 1, yCaret);

			BUFFER(cxBuffer - 1, yCaret) = ' ';

			HideCaret(hwnd);
			hdc = GetDC(hwnd);

			SelectObject(hdc, CreateFont(0, 0, 0, 0, 0, 0, 0, 0,
				dwCharSet, 0, 0, 0, FIXED_PITCH, NULL));

			TextOut(hdc, xCaret * cxChar, yCaret * cyChar,
				&BUFFER(xCaret, yCaret),
				cxBuffer - xCaret);

			DeleteObject(SelectObject(hdc, GetStockObject(SYSTEM_FONT)));
			ReleaseDC(hwnd, hdc);
			ShowCaret(hwnd);
			break;
		}
		SetCaretPos(xCaret * cxChar, yCaret * cyChar);  //在这个坐标位置闪烁，不需要调用ShowCaret，因为第一次调用ShowCaret后，就一直闪烁
		return 0;          //碰到SetCaretPos，重新确定了坐标，就在这个位置一直闪烁

	case WM_CHAR:
		for (i = 0; i < (int)LOWORD(lParam); i++)
		{
			switch (wParam)
			{
			case '\b':                    // backspace
				if (xCaret > 0)
				{
					xCaret--;
					SendMessage(hwnd, WM_KEYDOWN, VK_DELETE, 1);
				}
				break;

			case '\t':                    // tab
				do
				{
					SendMessage(hwnd, WM_CHAR, ' ', 1);
				} while (xCaret % 8 != 0);
				break;

			case '\n':                    // line feed
				if (++yCaret == cyBuffer)
					yCaret = 0;
				break;

			case '\r':                    // carriage return
				xCaret = 0;

				if (++yCaret == cyBuffer)
					yCaret = 0;
				break;

			case '\x1B':                  // escape
				for (y = 0; y < cyBuffer; y++)
					for (x = 0; x < cxBuffer; x++)
						BUFFER(x, y) = ' ';

				xCaret = 0;
				yCaret = 0;

				InvalidateRect(hwnd, NULL, FALSE);
				break;

			default:                      // character codes
				BUFFER(xCaret, yCaret) = (TCHAR)wParam;

				HideCaret(hwnd);  //要再窗口中显示内容，需要把闪烁光标隐藏
				hdc = GetDC(hwnd);

				SelectObject(hdc, CreateFont(0, 0, 0, 0, 0, 0, 0, 0,
					dwCharSet, 0, 0, 0, FIXED_PITCH, NULL));

				TextOut(hdc, xCaret * cxChar, yCaret * cyChar,
					&BUFFER(xCaret, yCaret), 1);

				DeleteObject(
					SelectObject(hdc, GetStockObject(SYSTEM_FONT)));
				ReleaseDC(hwnd, hdc);
				ShowCaret(hwnd);  //内容显示完成后，需要把光标重新显示出来（不能省略这个语句）

				if (++xCaret == cxBuffer)
				{
					xCaret = 0;

					if (++yCaret == cyBuffer)
						yCaret = 0;
				}
				break;
			}
		}

		SetCaretPos(xCaret * cxChar, yCaret * cyChar);  //对字符消息改变了光标的坐标后，在新坐标进行闪烁
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		SelectObject(hdc, CreateFont(0, 0, 0, 0, 0, 0, 0, 0,
			dwCharSet, 0, 0, 0, FIXED_PITCH, NULL));

		for (y = 0; y < cyBuffer; y++)
			TextOut(hdc, 0, y * cyChar, &BUFFER(0, y), cxBuffer);//着行输出窗口中的内容，其中cychar为每一行高度，cxbuffer为每行显示的字符数
																 //cybuffer用于控制行，每次循环，行数加一，输出该行的内容

		DeleteObject(SelectObject(hdc, GetStockObject(SYSTEM_FONT)));
		EndPaint(hwnd, &ps);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}