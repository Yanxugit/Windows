#include <Windows.h>
#include "sysmets.h"


LRESULT CALLBACK WndPorc(HWND, UINT, WPARAM, LPARAM);//���ûص�����  WndProc�����ķ���ֵ����ΪLRESULT�������͵ȼ���LONG.


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hprevInstance, PSTR szCmdLine, int iCmdShow)
{
	static WCHAR szAppName[] = TEXT("HelloWindows");  //����app����

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
		TEXT("THIS  IS  YANXU  WINDOWS"), //���ڱ���
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
	HDC          hdc;
	PAINTSTRUCT  ps;
	//RECT         rect;
	TEXTMETRIC   tm;
	static int cxChar,cxCaps, cYChar;
	TCHAR szBuffer[10];

	static int cxClient, cyClient; //���������ʾ��͸�

	switch (message)
	{

	case WM_SIZE:
		cxClient = LOWORD(lparam);
		cyClient = HIWORD(lparam);
		//StringCchPrintf(szBuffer,128,TEXT("�ֱ����� %d  *  %d px"),cxClient,cyClient);

		return 0;
	case  WM_CREATE: // ��Ҫ��ȡϵͳ������ʱ
		//PlaySound(TEXT("Helloein.wav"), NULL, SND_FILENAME | SND_ASYNC);
		hdc = GetDC(hwnd);//��ȡ�����豸����

		GetTextMetrics(hdc,&tm);//��ȡĬ��ϵͳ����ĳߴ�
		cxChar = tm.tmAveCharWidth; //ƽ���ַ����
		/*SYSMETS1������д�ַ���ƽ����ȱ����ھ�̬���� cxCaps �С�
			�ڵȿ������У�cxCaps����cxChar���ڱ�������У�cxCaps��ΪcxChar ��1.5����
			��TEXTMETRIC�ṹ�У�tmPitchAndFamily�ֶεĵ�λ���������Ƿ�Ϊ�ȿ�����:1���������壬0����ȿ����塣
			SYSMETS1������������� cxCaps :*/
		cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2)* cxChar / 2;


		cYChar = tm.tmHeight + tm.tmExternalLeading;//�ַ����ܸ߶�


		ReleaseDC(hwnd,hdc);//����
	

		return 0;
	case WM_PAINT:   //���ƴ���
		hdc = BeginPaint(hwnd, &ps);  //Device Context�豸������
									  //GetClientRect(hwnd,&rect);  //��ÿͻ�����λ������

									  //DrawText(hdc,TEXT("��������ĵ�һ��Windows���ڳ���"),-1,&rect,DT_SINGLELINE | DT_CENTER  | DT_VCENTER);

		//TextOut(hdc, 400, 300, TEXT("Yan Xu King!"), 12);
		for (int i = 0; i < NUMLINES; i++)
		{
			//  cYChar * i  �������ָ����ÿ���ַ��Ķ�������ڿͻ����Ķ�����λ�á�

				//TEXTOUT����ʾ�ı�������Ҫ��GDI�����������﷨����:
				//TextOut(hdc,x,y,psText,iLength);

			TextOut(hdc,0,cYChar * i,	sysmetrics[i].szLabel,lstrlen(sysmetrics[i].szLabel));

			TextOut(hdc, 22*cxCaps, cYChar * i, sysmetrics[i].szDesc, lstrlen(sysmetrics[i].szDesc));



			SetTextAlign(hdc,TA_RIGHT  | TA_TOP); //��ʾλ��
			/*֪ͨ Windows��̵�TextOut��������ʹ�õ����꽫�ӿͻ��������Ͻǿ�ʼ�������Ǵ����Ͻǿ�ʼ��*/
			
			/*���� 40xcxChar ��ֵ�����˵ڶ����͵������Ŀ�ȡ�*/

			TextOut(hdc, 22 * cxCaps + 40 * cxChar, cYChar * i, szBuffer, wsprintf(szBuffer,TEXT("%5d"),GetSystemMetrics(sysmetrics[i].Index)));
			
			
			//�ڵ�������� TextOut����һ��SetTextAlign�������ý����뷽ʽ���Ϊ������ʽ��
			SetTextAlign(hdc,TA_LEFT | TA_TOP); //��ʾλ��

		}

		
		EndPaint(hwnd, &ps);
		return 0;

		//��������ť���º󵯳���Ϣ��
	case WM_LBUTTONDOWN:
		MessageBox(NULL, TEXT("��������ұ������ˣ�"), TEXT("��ʾ"), 0);
		return 0;

		//����Ҽ���ť���º󵯳���Ϣ��
	case WM_RBUTTONDOWN:
		MessageBox(NULL, TEXT("�����ұ��ұ������ˣ�"), TEXT("��ʾ"), 0);
		return 0;
	case WM_DESTROY:  //�����ڹرյ���Ϣ
		PostQuitMessage(0);
		return 0;

	}
	return DefWindowProc(hwnd, message, wparam, lparam);

}