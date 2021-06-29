
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
		//CreateEllipticRgn����һ����Բ������Բ��X1��Y1��X2��Y2�����ȷ���ľ�������
		hRgnTemp[0] = CreateEllipticRgn(0, cyClient / 3, cxClient / 2, 2 * cyClient / 3);
		hRgnTemp[1] = CreateEllipticRgn(cxClient / 2, cyClient / 3, cxClient, 2 * cyClient / 3);
		hRgnTemp[2] = CreateEllipticRgn(cxClient / 3, 0, 2 * cxClient / 3, cyClient / 2);
		hRgnTemp[3] = CreateEllipticRgn(cxClient / 3, cyClient / 2, 2 * cxClient / 3, cyClient);
		hRgnTemp[4] = CreateRectRgn(0, 0, 1, 1);
		hRgnTemp[5] = CreateRectRgn(0, 0, 1, 1);
		hRgnClip = CreateRectRgn(0, 0, 1, 1);
		//CLOVER �ȴ��� 4 ����Բ�������Ǳ��洢�� hRgnTemp �����ǰ 4 ��Ԫ���С����ų��򴴽������� �� �� ����
		//�ڿͻ�����ߺ��ұߵ�������Բ�����Ⱥϲ���
		CombineRgn(hRgnTemp[4], hRgnTemp[0], hRgnTemp[1], RGN_OR);
		//ͬ���أ��ڿͻ��������͵ײ���������Բ����Ҳ�ϲ��ˣ�
		CombineRgn(hRgnTemp[5], hRgnTemp[2], hRgnTemp[3], RGN_OR);
		//��������ϲ���������ٺϲ��� hRgnClip ��
		CombineRgn(hRgnClip, hRgnTemp[4], hRgnTemp[5], RGN_XOR);
		//RGN_XOR ��ʶ����ʾҪ�ӽ���������ų��ص����������6 ����ʱ����ɾ����
		for (i = 0; i < 6; i++)
			DeleteObject(hRgnTemp[i]);

		SetCursor(hCursor);
		ShowCursor(FALSE);

		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		//��Խ�����ԣ�WM_PAINT ��Ϣ�Ĵ���ܼ򵥡��ӿ�ԭ�������ڿͻ��������ģ�����ʹ��ֱ�߸����ף����ڴ��� WM_SIZE ��Ϣʱ����������ѡ���豸������Ϊ��������
		SetViewportOrgEx(hdc, cxClient / 2, cyClient / 2, NULL);
		SelectClipRgn(hdc, hRgnClip);

		// hypot ����ֱ��������б�ߵĳ�
		//���ڣ�ʣ��Ҫ���ľ��ǻ�ֱ���ˣ�һ���� 360 ����ÿһ�Ȼ�һ����ÿ���ߵĳ����Ǳ��� fRadius ������ʾ���Ǵ����ĵ��ͻ�������ľ��룺
		fRadius = _hypot(cxClient / 2.0, cyClient / 2.0);

		for (fAngle = 0.0; fAngle < TWO_PI; fAngle += TWO_PI / 360)
		{
			MoveToEx(hdc, 0, 0, NULL);
			LineTo(hdc, int(fRadius * cos(fAngle) + 0.5), int(-fRadius * sin(fAngle) + 0.5));
		}
		EndPaint(hwnd, &ps);
		return 0;

	case WM_DESTROY:
		//�ڴ��� WM_DESTROY ��Ϣ�ڼ䣬��������ɾ����
		DeleteObject(hRgnClip);

		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}