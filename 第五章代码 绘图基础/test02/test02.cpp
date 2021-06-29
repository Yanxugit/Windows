#include <Windows.h>
#include <math.h>
#define NUM  1000
#define TWOPI (2*3.14159)

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

LRESULT CALLBACK WndPorc(HWND  hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	static int cxClient, cyClient;
	
	int i;
	HDC          hdc;
	PAINTSTRUCT  ps;
	POINT        apt[NUM];
	//TEXTMETRIC  tm;
	//RECT         rect;

	switch (message)
	{

	case  WM_SIZE: 
		cxClient = LOWORD(lparam);
		cyClient = HIWORD(lparam);

		return 0;
	case WM_PAINT:   //���ƴ���
		hdc = BeginPaint(hwnd, &ps);  //Device Context�豸������
		
		
		//������
		Rectangle(hdc, cxClient / 8, cyClient / 8, 7 * cxClient / 8, 7 * cyClient / 8);   //Rectangle(hdc,xLeft,yTop,xRight,yBottom)
		 //���Խ���
		MoveToEx(hdc, 0, 0, NULL);
		LineTo(hdc, cxClient, cyClient);

		MoveToEx(hdc, 0, cyClient, NULL);
		LineTo(hdc, cxClient, 0);
		//����Բ
		Ellipse(hdc, cxClient / 8, cyClient / 8, 7 * cxClient / 8, 7 * cyClient / 8);     //Ellipse(hdc,xLeft,yTop,xRight,yBottom)
		//��Բ�Ǿ���
		RoundRect(hdc, cxClient / 4, cyClient / 4, 3 * cxClient / 4, 3 * cyClient / 4, cxClient / 4, cyClient / 4);//RoundRect(hdc, ,xLeft,yTop,xRight,yBottom, xCornerEllipse, yCornerEllipse);
		//������
		//for ( i = 0; i < NUM; i++)
	//	{
		//	apt[i].x = i*cxClient / NUM;
		//	apt[i].y = (int)(cyClient / 2 * (1 - sin(TWOPI*i / NUM)));
		//}


		//Polyline(hdc,apt,NUM);
		EndPaint(hwnd, &ps);
		return 0;


	case WM_DESTROY:  //�����ڹرյ���Ϣ
		PostQuitMessage(0);
		return 0;

	}
	return DefWindowProc(hwnd, message, wparam, lparam);

}