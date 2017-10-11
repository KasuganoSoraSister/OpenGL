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

	//glutInit(&argc, argv);  // 初始化GLUT环境
	//glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);   // GLUT显示模式：单缓冲区、RGB颜色模型
	//glutInitWindowPosition(50, 100);   // 窗口左上角的位置
	//glutInitWindowSize(800, 600);      // 显示窗口的大小
	//glutCreateWindow("An Example of OpenGL"); // 创建显示窗口，加上标题
	//init();
	//printf("%d\n", GetActiveWindow());
	//glutDisplayFunc((void(GLUTCALLBACK *)())draw);           // 调用绘图函数
	//glutSpecialFunc(move);			 // 功能键响应
	//glutMouseFunc(mouseButtonDown);  // 鼠标按键响应
	//glutMotionFunc(mouseMotion);     // 鼠标移动响应
	//HWND hWNd;
	//printf("%d\n", hWNd = FindWindow(L"GLUT", L"An Example of OpenGL"));
	//printf("%X", GetWindowLong(hWNd, GWL_WNDPROC));
	//glutMainLoop();				 // 进入事件处理循环
	system("pause");

	
}
