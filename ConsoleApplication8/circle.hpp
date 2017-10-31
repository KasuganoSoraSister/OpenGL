#include "windows.h"

class Circle {
public:
	Circle(POINT p = {0,0}, int r = 0) {
		this->p = p;
		this->r = r;
	}

	virtual inline POINT Point() {
		return p;
	}

	virtual inline int getR() {
		return r;
	}

	virtual inline int setR(int r) {
		return this->r = r;
	}

	virtual inline POINT setPoint(POINT p) {
		return this->p = p;
	}

private:
	int r;
	POINT p;
};