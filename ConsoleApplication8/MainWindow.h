
#include "glutWindow.hpp"
#include "Line.hpp"
#include "Circle.hpp"

class MainWindow : public glutWindow
{
public:
	MainWindow(int argc, char** argv);
	~MainWindow();
	void drawLine(Line* l);
	void drawCircle(Circle* c);
private:

protected:
	void WINAPI MainWindow::_DDA_DrawLine(LPVOID lp);
	void WINAPI MainWindow::_DrawCircle(LPVOID lp);
	void WINAPI MainWindow::PolyScan(LPVOID);

	void circlePlot(int xc, int yc, POINT circpt);

};

