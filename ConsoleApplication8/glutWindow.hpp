
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
		glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);   // GLUT��ʾģʽ������������RGB��ɫģ��
		glutInitWindowPosition(x, y);   // �������Ͻǵ�λ��
		glutInitWindowSize(wight, hight);      // ��ʾ���ڵĴ�С
		glutCreateWindow(name.c_str()); // ������ʾ���ڣ����ϱ���

		CoCreateGuid(&g);
		char a[100];
		sprintf(a, "%lld", g.Data1);
		glutSetWindowTitle(a);

		_hwnd =  FindWindowA("GLUT", a);
		if (_hwnd == 0) MessageBox(0, L"��ȡ���ھ��ʧ��", 0, 0);
		_oldProc = (WNDPROC)SetWindowLong(_hwnd,GWL_WNDPROC,(LONG)_subThunk.GetThunk());
		glutSetWindowTitle(name.c_str());
		glClearColor(1.0, 1.0, 1.0, 0.0);  // ���ñ�����ɫ
		glMatrixMode(GL_PROJECTION);       // ����ͶӰ����
		gluOrtho2D(0.0, 600.0, 0.0, 400.0); // ���ó����Ĵ�С
		glutDisplayFunc((void(GLUTCALLBACK *)())_draw.GetThunk()); 
		glutReshapeFunc(reshape21);   // ���������
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
		// ����Ҫ����opengl�ڴ��ڵĻ�ͼ����һ������������
		glViewport(0, 0, w, h);

		// һ��ֻ�ڴ���reshapeʱ�������趨��ͶӰ�����ģ�;���
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		// ��ͷĬ��������ԭ�㿴��Z�Ḻ���򣬶�glOrtho�������������ʾ
		// ���뾵ͷzNear��zFar������ͼ�βſ��Կ���
		// ����Ĭ������ϵZ����������ָ����Ļ�����ģ�
		// �����ó�0, 40��ֻ�ܿ���Z����( -40, 0 )������壬
		// ����zNear�������������ܿ���Z����Ϊ����������
		RECT r;
		//GetWindowRect( mWindow->_hwnd(), &r);
		glOrtho(0, w, 0, h, -40, 40);

		// ͶӰ������ú��ٽ���ǰ��������Ϊģ�;���
		// ���������ͼ�λ��ƺ�ͼ�α任
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
			glFlush();     // �����ͼpipeLine
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

