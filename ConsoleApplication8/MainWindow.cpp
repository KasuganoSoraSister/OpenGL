#include "stdafx.h"
#include "MainWindow.h"
#include "vector"
#include "memory"

#define POINTNUM 3

MainWindow::MainWindow(int argc, char** argv)
	:glutWindow(argc, argv)
{
	Func_draw.push_back({ static_cast<glutWindow::fnDraw>(&MainWindow::PolyScan) ,(LPVOID)0 });
}

MainWindow::~MainWindow()
{

}

void MainWindow::drawLine(Line* l)
{
	Func_draw.push_back({ static_cast<glutWindow::fnDraw>(&MainWindow::_DDA_DrawLine) ,(LPVOID)l});
	glutPostRedisplay();
}

void MainWindow::drawCircle(Circle* c) {
	Func_draw.push_back({ static_cast<glutWindow::fnDraw>(&MainWindow::_DrawCircle) ,(LPVOID)c});
	glutPostRedisplay();
}


typedef struct XET {
	float x;
	float dx, ymax;
	XET* next;
}AET, NET;

struct point {
	float x;
	float y;
}

polypoint[POINTNUM] = { 80,120,20,30,150,50 };

void WINAPI MainWindow::PolyScan(LPVOID) {
	int MaxY = 0;
	int i;
	for (i = 0; i < POINTNUM; i++) {
		if (polypoint[i].y > MaxY)
			MaxY = polypoint[i].y;
	}
	AET *pAET = new AET;
	pAET->next = NULL;

	NET *pNET[1024];
	for (i = 0; i <= MaxY; i++) {
		pNET[i] = new NET;
		pNET[i]->next = NULL;
	}
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_POINTS);
	for (i = 0; i < MaxY; i++) {
		for (int j = 0; j < POINTNUM; j++) {
			if (polypoint[j].y == i) {
				if (polypoint[(j - 1 + POINTNUM) % POINTNUM].y > polypoint[j].y) {
					NET *p = new NET;
					p->x = polypoint[j].x;
					p->ymax = polypoint[(j - 1 + POINTNUM) % POINTNUM].y;
					p->dx = (polypoint[(j - 1 + POINTNUM) % POINTNUM].x - polypoint[j].x) / (polypoint[(j - 1 + POINTNUM) % POINTNUM].y - polypoint[j].y);
					p->next = pNET[i]->next;
					pNET[i]->next = p;
				}
				if (polypoint[(j + 1 + POINTNUM) % POINTNUM].y > polypoint[j].y) {
					NET *p = new NET;
					p->x = polypoint[j].x;
					p->ymax = polypoint[(j + 1 + POINTNUM) % POINTNUM].y;
					p->dx = (polypoint[(j + 1 + POINTNUM) % POINTNUM].x - polypoint[j].x) / (polypoint[(j + 1 + POINTNUM) % POINTNUM].y - polypoint[j].y);
					p->next = pNET[i]->next;
					pNET[i]->next = p;
				}
			}
		}
	}
	for (i = 0; i <= MaxY; i++) {
		NET *p = pAET->next;
		while (p) {
			p->x = p->x + p->dx;
			p = p->next;
		}
		AET *tq = pAET;
		p = pAET->next;
		tq->next = NULL;
		while (p) {
			while (tq->next && p->x >= tq->next->x)
				tq = tq->next;
			NET *s = p->next;
			p->next = tq->next;
			tq->next = p;
			p = s;
			tq = pAET;
		}
		AET *q = pAET;
		p = q->next;
		while (p) {
			if (p->ymax == i) {
				q->next = p->next;
				delete p;
				p = q->next;
			}
			else {
				q = q->next;
				p = q->next;
			}
		}
		p = pNET[i]->next;
		q = pAET;
		while (p) {
			while (q->next && p->x >= q->next->x)
				q = q->next;
			NET *s = p->next;
			p->next = q->next;
			q->next = p;
			p = s;
			q = pAET;
		}
		p = pAET->next;
		while (p && p->next) {
			for (float j = p->x; j <= p->next->x; j++) {
				glVertex2i(static_cast<int>(j), i);
			}
			p = p->next->next;
		}
	}
	glEnd();
	glFlush();
}
void MainWindow::circlePlot(int xc,int yc,POINT circpt) {
	glVertex2i(xc + circpt.x, yc + circpt.y);
	glVertex2i(xc + circpt.x, yc - circpt.y);
	glVertex2i(xc - circpt.x, yc + circpt.y);
	glVertex2i(xc - circpt.x, yc - circpt.y);

	glVertex2i(xc + circpt.y, yc + circpt.x);
	glVertex2i(xc - circpt.y, yc + circpt.x);
	glVertex2i(xc + circpt.y, yc - circpt.x);
	glVertex2i(xc - circpt.y, yc - circpt.x);
}

void WINAPI MainWindow::_DrawCircle(LPVOID lp) {
	glColor3f(0.8, 0.2, 0);
	glBegin(GL_POINTS);
	glPointSize(1);
	Circle* c = static_cast<Circle*>(lp);
	POINT p = {0};
	p.y = c->getR();
	int np = 1 - c->getR();

	circlePlot(c->Point().x, c->Point().y, p);

	while (p.x < p.y)
	{
		p.x++;
		if (np < 0)
		{
			np += 2 * p.x + 1;
		}
		else
		{
			p.y--;
			np += 2 * (p.x - p.y) + 1;
		}
		circlePlot(c->Point().x, c->Point().y, p);
	}
	glEnd();
}


void WINAPI MainWindow::_DDA_DrawLine(LPVOID lp)
{
	glColor3f(0, 0.5, 0.5);
	glBegin(GL_POINTS);
	glPointSize(1);

	Line* l = static_cast<Line*>(lp)/*new Line({ 100,100 }, { 200,200 })*/;

	long YDis = (l->getEnd().y - l->getStart().y);
	long XDis = (l->getEnd().x - l->getStart().x);
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
