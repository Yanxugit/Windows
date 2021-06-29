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
		cxClient = LOWORD(lparam);//�ͻ����ĳ���
		cyClient = HIWORD(lparam);//�ͻ����ĸ߶�
		
		return 0;
	
	case WM_PAINT:   //���ƴ���
		hdc = BeginPaint(hwnd, &ps);  //Device Context�豸������
		//SelectObject
		//��һ������(λͼ�����ʡ���ˢ��)ѡ��ָ�����豸�������µĶ������ͬһ���͵��϶���
		SelectObject(hdc,GetStockObject(GRAY_BRUSH));//GetStockObject()�ú�������Ԥ����ı��ñʡ�ˢ�ӡ�������ߵ�ɫ��ľ��

		for (i = 0; i < 10; i++) 
		{
			apt[i].x = cxClient * aptFigure[i].x / 200;
			apt[i].y = cyClient * aptFigure[i].y / 100;
		}
	   //����iMode:�����������ģʽ:ALTERNATE��WINDING����Ĭ�������ΪALTERNATE;
	   //ģʽALTERNATE:��ӷ�������е�һ����������Զ��ˮƽ��һ�����ߣ�ֻ�е������ߴ�Խ�������߿���ʱ���������ű���䣬��Ϊż��������������
	   //ģʽWINDING������һ������Ϊ����������������Ϊż����Ҫ���ݱ߿��ߵķ������жϣ���������ı߿����ڲ�ͬ����ı߿�����Ŀ��ȣ�����䣬�粻�ȣ�����䡣
		SetPolyFillMode(hdc,ALTERNATE);
		Polygon(hdc,apt,10);//�ú�����һ����ֱ�����ŵ��������϶�����ɵĶ���Σ��õ�ǰ���ʻ�������������õ�ǰ��ˢ�Ͷ�������ģʽ�������

		for (i = 0; i < 10; i++)
		{
			apt[i].x += cxClient / 2;
			
		}
		SetPolyFillMode(hdc, WINDING);
		Polygon(hdc, apt, 10);

		EndPaint(hwnd, &ps);
		return 0;


	case WM_DESTROY:  //�����ڹرյ���Ϣ
		PostQuitMessage(0);
		return 0;

	}
	return DefWindowProc(hwnd, message, wparam, lparam);

}