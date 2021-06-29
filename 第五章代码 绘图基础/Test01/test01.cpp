#include <Windows.h>
#define NUMLINES ((int) (sizeof(devcaps) / sizeof(devcaps[0])))
 struct
 {
	     int iIndex;
	    TCHAR *szLabel;
		TCHAR *szDesc;
	 }
devcaps[] =
 {
	     HORZSIZE, TEXT("HORZSIZE"), TEXT("Width in millimeters:"),
		 VERTSIZE, TEXT("VERTSIZE"), TEXT("Height in milimeters:"),
		 HORZRES, TEXT("HORZRES"), TEXT("Width in pixels:"),
		 VERTRES, TEXT("VERTRES"), TEXT("Height in raster lines:"),
		 BITSPIXEL, TEXT("BITSPIXEL"), TEXT("Color bits per pixel:"),
		 PLANES, TEXT("PLANES"), TEXT("Number of color planes:"),
		 NUMBRUSHES, TEXT("NUMBRUSHES"), TEXT("Number of device brushes"),
		 NUMPENS, TEXT("NUMPENS"), TEXT("Number of device pens:"),
		 NUMMARKERS, TEXT("NUMMARKERS"), TEXT("Number of device markers:"),
		 NUMFONTS, TEXT("NUMFONTS"), TEXT("Number of device fonts:"),
		 NUMCOLORS, TEXT("NUMCOLORS"), TEXT("Number of device colors:"),
		 PDEVICESIZE, TEXT("PDEVICESIZE"), TEXT("Size of device structure:"),
		 ASPECTX, TEXT("ASPECTX"), TEXT("Relative width of pixel:"),
		 ASPECTY, TEXT("ASPECTY"), TEXT("Relative height of pixel:"),
		 ASPECTXY, TEXT("ASPECTXY"), TEXT("Relative diagonal of pixel:"),
		 LOGPIXELSX, TEXT("LOGPIXELSX"), TEXT("Horizontal dots per inch:"),
		 LOGPIXELSY, TEXT("LOGPIXELSY"), TEXT("Vertical dots per inch:"),
		 SIZEPALETTE, TEXT("SIZEPALETTE"), TEXT("Number of palette entries:"),
		 NUMRESERVED, TEXT("NUMRESERVED"), TEXT("Reserved palette entries:"),
		 COLORRES, TEXT("COLORRES"), TEXT("Actual color resolution:")
		 };
LRESULT CALLBACK WndPorc(HWND, UINT, WPARAM, LPARAM);//���ûص�����  WndProc�����ķ���ֵ����ΪLRESULT�������͵ȼ���LONG.


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hprevInstance, PSTR szCmdLine, int iCmdShow)
{
	static WCHAR szAppName[] = TEXT("Devcaps1");  //����app����

	HWND     hwnd;
	MSG      msg;
	WNDCLASS wndclass;
	
	wndclass.style = CS_HREDRAW | CS_VREDRAW ;						//ָ���������ͣ����֡����񡱣�����·���������ʹ�ð�λ��������������
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
		TEXT("��ֱ��"), //���ڱ���
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
	static int cxChar, cyChar, cxCaps;
	TCHAR szBuffer[10];
	int i;
	HDC          hdc;
	PAINTSTRUCT  ps;

	TEXTMETRIC  tm;
	//RECT         rect;

	switch (message)
	{

	case  WM_CREATE: // ��Ҫ��ȡϵͳ������ʱ
		hdc = GetDC(hwnd);

		GetTextMetrics(hdc, &tm);
		cxChar = tm.tmAveCharWidth;
		cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2)*cxChar / 2;
		cyChar = tm.tmHeight + tm.tmExternalLeading;

		ReleaseDC(hwnd, hdc);
		return 0;
	case WM_PAINT:   //���ƴ���
		hdc = BeginPaint(hwnd, &ps);  //Device Context�豸������

		GetClientRect();

			//for (i = 0; i != NUMLINES; ++i)
			//	       {
			//	           TextOut(hdc, 0, cyChar * i, devcaps[i].szLabel,lstrlen(devcaps[i].szLabel));
			//	         TextOut(hdc, 14 * cxCaps, cyChar * i, devcaps[i].szDesc, lstrlen(devcaps[i].szDesc));
			//	         SetTextAlign(hdc, TA_RIGHT | TA_TOP);
			//	         TextOut(hdc, 14 * cxCaps + 35 * cxChar, cyChar * i, szBuffer, wsprintf(szBuffer, TEXT("%5d"), GetDeviceCaps(hdc, devcaps[i].iIndex)));
			//	           SetTextAlign(hdc, TA_LEFT | TA_TOP);
			//	       }
		

		EndPaint(hwnd, &ps);
		return 0;

	
	case WM_DESTROY:  //�����ڹرյ���Ϣ
		PostQuitMessage(0);
		return 0;

	}
	return DefWindowProc(hwnd, message, wparam, lparam);

}