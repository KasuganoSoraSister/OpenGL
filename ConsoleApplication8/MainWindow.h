
#include "glutWindow.hpp"
#include "Line.hpp"

class MainWindow : public glutWindow
{
public:
	MainWindow(int argc, char** argv);
	~MainWindow();
	void drawLine(Line* l);
private:

protected:
	void WINAPI MainWindow::_DDA_DrawLine(LPVOID lp);
};

