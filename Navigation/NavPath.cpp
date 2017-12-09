
#include "stdafx.h"
#include "NavPath.h"

float NavPath::getOrientation(int n) const
{
	if (n == 0)
		return 0;
	else
	{
		int dx = _path[n - 1].getX() - _path[n].getX();
		int dy = _path[n - 1].getY() - _path[n].getY();

		if (dx < 0)
		{
			if (dy < 0)
				return M_PI * 0.75f;
			else if (dy > 0)
				return M_PI * 0.25f;
			else
				return M_PI * 0.5f;
		}
		else if (dx > 0)
		{
			if (dy < 0)
				return -M_PI*0.75f;
			else if (dy > 0)
				return -M_PI * 0.25f;
			else
				return -M_PI * 0.5f;
		}
		else
		{
			if (dy < 0)
				return M_PI;
			else
				return 0;
		}
	}
}

void NavPath::outputPath(ofstream& ofs) const
{
	ofs << "Path [" << _path.size() << "]:" << endl;
	for (int i = 0; i < _path.size(); ++i)
	{
		ofs << "(" << _path[i].getX() << " , " << _path[i].getY() << ")" << endl;
	}

	ofs << endl;
}