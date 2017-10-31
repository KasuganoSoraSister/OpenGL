
#include "string"
#include<vector>  
#include "map"
#include "window.hpp"

using namespace std;

class glutWindow;

class glutWindow : public Window
{
public:
	typedef void(WINAPI glutWindow::*fnDraw)(LPVOID);

	struct Func_Infomation {
		fnDraw lpFunc = nullptr;
		LPVOID lpParam = nullptr;
	};

	typedef INT_PTR(glutWindow::*fnWndProc)(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	typedef std::map<UINT, std::pair<glutWindow*, fnWndProc>> mapWndProc;


	glutWindow(int argc, char** argv)
		:_draw(&glutWindow::draw,this)
	{
	}

	~glutWindow() {}

	void gluCreateglutWindow(string name, int wight = 800, int hight = 600, int x = 200, int y = 300) {
		GUID g;

		//Func_draw.push_back({ static_cast<glutWindow::fnDraw>(&glutWindow::OnInit) ,nullptr });
		glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);   // GLUT显示模式：单缓冲区、RGB颜色模型
		glutInitWindowPosition(x, y);   // 窗口左上角的位置
		glutInitWindowSize(wight, hight);      // 显示窗口的大小
		glutCreateWindow(name.c_str()); // 创建显示窗口，加上标题

		CoCreateGuid(&g);
		char a[100];
		sprintf(a, "%lld", g.Data1);
		glutSetWindowTitle(a);

		_hwnd =  FindWindowA("GLUT", a);
		if (_hwnd == 0) MessageBox(0, L"获取窗口句柄失败", 0, 0);
		_oldProc = (WNDPROC)SetWindowLong(_hwnd,GWL_WNDPROC,(LONG)_subThunk.GetThunk());
		glutSetWindowTitle(name.c_str());
		glClearColor(1.0, 1.0, 1.0, 0.0);  // 设置背景颜色
		glMatrixMode(GL_PROJECTION);       // 设置投影参数
		gluOrtho2D(0.0, 600.0, 0.0, 400.0); // 设置场景的大小
		glutDisplayFunc((void(GLUTCALLBACK *)())_draw.GetThunk()); 
		glutReshapeFunc(reshape21);   // 这个必须有
		glutMainLoop();			
		
		return;
	}

	virtual string Text() {
		return "1";
	}
	
	DWORD gg(LPVOID) { return 0; };

	void Lock() { IsLock = true; }
	void UnLock() { IsLock = false; }

private:
	bool IsLock = false;

	static void reshape21(int w, int h)
	{
		// 这里要设置opengl在窗口的绘图区域，一般是整个窗口
		glViewport(0, 0, w, h);

		// 一般只在窗口reshape时才重新设定下投影矩阵和模型矩阵
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		// 镜头默认在坐标原点看向Z轴负方向，而glOrtho最后两个参数表示
		// 距离镜头zNear到zFar距离间的图形才可以看到
		// 而且默认坐标系Z轴正方向是指向屏幕朝外侧的，
		// 你设置成0, 40就只能看到Z坐标( -40, 0 )间的物体，
		// 所以zNear给个负数，就能看到Z坐标为正的物体了
		RECT r;
		//GetWindowRect( mWindow->_hwnd(), &r);
		glOrtho(0, w, 0, h, -40, 40);

		// 投影矩阵设好后再将当前矩阵设置为模型矩阵，
		// 方便后续的图形绘制和图形变换
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}

	LRESULT draw(HWND hwnd, UINT m, WPARAM wParam, LPARAM lParam)
	{
		while (IsLock == true);
		try {
			glClear(GL_COLOR_BUFFER_BIT);
			printf("%d\n", Func_draw.size());

			for(int i = 0;i < Func_draw.size();i++)
			{
				printf("%d\n", Func_draw.size());

				Func_Infomation f = Func_draw[i];
				(this->*f.lpFunc)(f.lpParam);
			}

			//glPopMatrix();
			glFlush();     // 处理绘图pipeLine
		}
		catch (...) {

		}
		return 0;
	}

	LRESULT SubProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		if (_subMessages.count(message))
			return (this->_subMessages[message].first->*_subMessages[message].second)(hwnd, message, wParam, lParam);

		return CallWindowProcW(_oldProc, hwnd, message, wParam, lParam);
	}

protected:
	vector <Func_Infomation> Func_draw;
	Win32Thunk<WNDPROC,glutWindow> _draw;
};

