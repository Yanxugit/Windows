#include<Windows.h>

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,PSTR szCmdLine,int iCmdShow) 
{
	WCHAR str1[] = TEXT("y—÷–Ò≥…∑Ò");
	wchar_t *str2 =TEXT("title");
	MessageBox(NULL,str1,str2,0);
	return 0;

}