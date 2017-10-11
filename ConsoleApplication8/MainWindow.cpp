#include "stdafx.h"
#include "MainWindow.h"
#include "vector"


MainWindow::MainWindow(int argc, char** argv)
	:glutWindow(argc, argv)
{

}

MainWindow::~MainWindow()
{

}

void MainWindow::drawLine(Line* l)
{
	Func_draw.push_back({ static_cast<glutWindow::fnDraw>(&MainWindow::_DDA_DrawLine) ,(LPVOID)l});
	glutPostRedisplay();
}

void WINAPI MainWindow::_DDA_DrawLine(LPVOID lp)
{
	glColor3f(1, 0, 0);
	glBegin(GL_POINTS);
	glPointSize(1);

	Line* l = static_cast<Line*>(lp)/*new Line({ 100,100 }, { 200,200 })*/;

	long YDis = (l->getEnd().x - l->getStart().x);
	long XDis = (l->getEnd().y - l->getStart().y);
	long MaxStep = max(abs(XDis), abs(YDis));
	float fXUnitLen = 1.0f;  // X方向的单位步进  
	float fYUnitLen = 1.0f;  // Y方向的单位步进  

	fYUnitLen = static_cast<float>(YDis) / static_cast<float>(MaxStep);
	fXUnitLen = static_cast<float>(XDis) / static_cast<float>(MaxStep);

	float x = static_cast<float>(l->getStart().x);
	float y = static_cast<float>(l->getStart().y);

	for (long i = 1; i <= MaxStep; i++)
	{
		x = x + fXUnitLen;
		y = y + fYUnitLen;
		glVertex2i(int(x + 0.5), int(y + 0.5));
	}

	glEnd();
}
