#include <Windows.h>
#include <math.h>

LRESULT CALLBACK WndPorc(HWND, UINT, WPARAM, LPARAM);//���ûص�����  WndProc�����ķ���ֵ����ΪLRESULT�������͵ȼ���LONG.


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hprevInstance, PSTR szCmdLine, int iCmdShow)
{
	static WCHAR szAppName[] = TEXT("SineWave");  //����app����

	HWND     hwnd;
	MSG      msg;
	WNDCLASS wndclass;

	wndclass.style = CS_HREDRAW | CS_VREDRAW;						//ָ���������ͣ����֡����񡱣�����·���������ʹ�ð�λ��������������
	wndclass.lpfnWndProc = WndPorc;									//ָ�����ڹ��̣������ǻص������� *
																	//���������ֶ���������ṹ�� Windows�ڲ�ά���Ĵ��ڽṹ��Ԥ��
																	//һЩ����Ŀռ�:
	wndclass.cbClsExtra = 0;										//Ԥ���Ķ���ռ䣬һ��Ϊ 0
	wndclass.cbWndExtra = 0;										//Ԥ���Ķ���ռ䣬һ��Ϊ 0
																	//���������ֶα�ʾӦ�ó����ʵ�����(��WinMain��һ������):
	wndclass.hInstance = hInstance;									//Ӧ�ó����ʵ�����
																	//��������Ϊ���л��ڸô�����Ĵ����趨һ��ͼ��:
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);				//Ϊ���л��ڸô�����Ĵ����趨һ��ͼ��
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);					//Ϊ���л��ڸô�����Ĵ����趨һ�����ָ��
																	/*
																	��һ���ֶ�Ϊ���ര�ڵĿͻ���ָ���˱���ɫ��
																	�ֶ���hbrBackground ��ǰ׺hbr��ʾ����ˢ�ľ����(handle to a brush)��
																	��ˢ��һ��ͼ��ѧ�����ʾ������������������ɫģʽ��
																	Windows�м�����׼�Ļ�ˢ���ֳơ���桱��ˢ��
																	�����GetStockObject�ĵ��÷���һ����ɫ��ˢ�ľ��:
																	*/
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	//ָ�����ڱ���ɫ
																	//��������һ���ֶ�ָ���˴�����Ĳ˵���
																	//���ڳ��� HELLOWIN�����κβ˵������Ը��ֶα���ΪNULL:
	wndclass.lpszMenuName = NULL;									//ָ�����ڲ˵�
																	//��󣬱���Ϊ�����ำ��һ�����ơ���һ��С������˵��
																	//������ƿ��Լ򵥵��ó�������ʾ��
																	//�籣���ڱ���szAppName�е��ַ�����HelloWin��:
	wndclass.lpszClassName = szAppName;								//ָ������������ *

	if (!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("This program  requires Windows NT!"), szAppName, MB_ICONERROR);
		return 0;
	}

	hwnd = CreateWindow(szAppName,                        //��������
		TEXT("Sine Wave Using PolyLine!"), //���ڱ���
		WS_OVERLAPPEDWINDOW,              //������ʽ
		CW_USEDEFAULT,	                  //��ʼxλ��
		CW_USEDEFAULT,                    //��ʼyλ��
		CW_USEDEFAULT,                    //��ʼx�ߴ�
		CW_USEDEFAULT,                    //��ʼy�ߴ�
		NULL,                             //�����ھ��
		NULL,							  //���ڲ˵����
		hInstance,						  //����ʵ�����
		NULL                              //��������
	);


	ShowWindow(hwnd, iCmdShow);  //��ʾ���� ��  ���������ô��ڵ���ʾ״̬��
	UpdateWindow(hwnd);  //�����ƹ�Ӧ�ó������Ϣ���У�ֱ�ӷ��� WM_PAINT ��Ϣ��ָ�����ڵĴ��ڹ���


	while (GetMessage(&msg, NULL, 0, 0))   //�����������Ǵӵ�ǰ�̵߳���Ϣ�������ȡһ����Ϣ������ MSG �ṹ �С�
	{
		TranslateMessage(&msg);  //�������������Ϣת��Ϊ�ַ���Ϣ���ַ���Ϣ�����͵���ǰ�̵߳���Ϣ�����
		DispatchMessage(&msg);  //��������һ����Ϣ�����ڹ��̣��ص���������ͨ������Ϣ�� GetMessage ������á�Windows �Ŀ���Ȩ�ڸú���������Ӧ�ó���
	}


	return msg.wParam;
}
void DrawBezier(HDC hdc, POINT apt[])
{
	PolyBezier(hdc, apt, 4);

	MoveToEx(hdc, apt[0].x, apt[0].y, NULL);
	LineTo(hdc, apt[1].x, apt[1].y);

	MoveToEx(hdc, apt[2].x, apt[2].y, NULL);
	LineTo(hdc, apt[3].x, apt[3].y);
}

LRESULT CALLBACK WndPorc(HWND  hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	static int cxClient, cyClient;

	int i;
	HDC          hdc;
	PAINTSTRUCT  ps;
	static POINT        apt[4];
	//TEXTMETRIC  tm;
	//RECT         rect;

	switch (message)
	{

	case  WM_SIZE:
		cxClient = LOWORD(lparam);
		cyClient = HIWORD(lparam);
		apt[0].x = cxClient / 4;
		apt[0].y = cyClient / 2;

		apt[1].x = cxClient / 2;
		apt[1].y = cyClient / 4;

		apt[2].x = cxClient / 2;
		apt[2].y = 3 * cyClient / 4;

		apt[3].x = 3 * cxClient / 4;
		apt[3].y = cyClient / 2;
		return 0;
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MOUSEMOVE:
		if (wparam & MK_LBUTTON || wparam & MK_RBUTTON)
		{
			hdc = GetDC(hwnd);

			SelectObject(hdc, GetStockObject(WHITE_PEN));
			DrawBezier(hdc, apt);

			if (wparam & MK_LBUTTON)
			{
				apt[1].x = LOWORD(lparam);
				apt[1].y = HIWORD(lparam);
			}

			if (wparam & MK_RBUTTON)
			{
				apt[2].x = LOWORD(lparam);
				apt[2].y = HIWORD(lparam);
			}

			SelectObject(hdc, GetStockObject(BLACK_PEN));
			DrawBezier(hdc, apt);

			ReleaseDC(hwnd, hdc);
		}
	case WM_PAINT:   //���ƴ���
		hdc = BeginPaint(hwnd, &ps);  //Device Context�豸������

		DrawBezier(hdc, apt);

		EndPaint(hwnd, &ps);
		return 0;


	case WM_DESTROY:  //�����ڹرյ���Ϣ
		PostQuitMessage(0);
		return 0;

	}
	return DefWindowProc(hwnd, message, wparam, lparam);

}