
#include "windows.h"
#include "math.h"

class Line
{
public:
	inline Line(POINT start, POINT end) {
		this->start = start;
		this->end = end;
	}
	~Line() {};
	virtual inline int GetLenth() {
		int x = (end.x - start.x);
		int y = (end.y - start.y);
		return sqrt(abs(x * x + y * y));
	}
	virtual inline POINT getStart() { return start; }
	virtual inline POINT getEnd() { return end; }

private:
	POINT start, end;
};
