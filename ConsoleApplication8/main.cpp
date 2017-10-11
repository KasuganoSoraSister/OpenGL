#include "stdafx.h"
#include <math.h>
#include "MainWindow.h"


MainWindow* MW;

struct param
{
	int argc;
	char** argv;
};

DWORD WINAPI mainThread(LPVOID LP) {

	param* p = static_cast<param*>(LP);

	MW = new MainWindow(p->argc, p->argv);
	MW->gluCreateglutWindow("MyWindow");

	return 0;
}

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	

	param lp = { argc,argv };
	HANDLE h = CreateThread(0, 0, mainThread, (LPVOID)& lp, 0, 0);
	Sleep(2000);
	
	MW->drawLine(new Line({ 100,100 }, { 200,200 }));

	//glutInit(&argc, argv);  // ��ʼ��GLUT����
	//glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);   // GLUT��ʾģʽ������������RGB��ɫģ��
	//glutInitWindowPosition(50, 100);   // �������Ͻǵ�λ��
	//glutInitWindowSize(800, 600);      // ��ʾ���ڵĴ�С
	//glutCreateWindow("An Example of OpenGL"); // ������ʾ���ڣ����ϱ���
	//init();
	//printf("%d\n", GetActiveWindow());
	//glutDisplayFunc((void(GLUTCALLBACK *)())draw);           // ���û�ͼ����
	//glutSpecialFunc(move);			 // ���ܼ���Ӧ
	//glutMouseFunc(mouseButtonDown);  // ��갴����Ӧ
	//glutMotionFunc(mouseMotion);     // ����ƶ���Ӧ
	//HWND hWNd;
	//printf("%d\n", hWNd = FindWindow(L"GLUT", L"An Example of OpenGL"));
	//printf("%X", GetWindowLong(hWNd, GWL_WNDPROC));
	//glutMainLoop();				 // �����¼�����ѭ��
	system("pause");

	
}
