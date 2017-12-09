
#include "stdafx.h"
#include "NavMap.h"
#include "AS_PathFinder.h"

#define BESTPLACE_MAX_ITER				5

static const int NavPlaceBottomLeftOffsets[30] = {
	-1, -1,
	-1,  0,
	 0, -1,
	-2, -2,
	-2, -1,
	-1, -2,
	-2,  0,
	 0, -2,
	-3, -3,
	-3, -2,
	-2, -3,
	-3, -1,
	-1, -3,
	-3,  0,
	 0, -3
};
static const int NavPlaceBottomLeftCount = 15;

static const int NavPlaceBottomRightOffsets[30] = {
	1, -1,
	1, 0,
	0, -1,
	2, -2,
	2, -1,
	1, -2,
	2, 0,
	0, -2,
	3, -3,
	3, -2,
	2, -3,
	3, -1,
	1, -3,
	3, 0,
	0, -3
};
static const int NavPlaceBottomRightCount = 15;

static const int NavPlaceTopLeftOffsets[30] = {
	-1, 1,
	-1, 0,
	 0, 1,
	-2, 2,
	-2, 1,
	-1, 2,
	-2, 0,
	 0, 2,
	-3, 3,
	-3, 2,
	-2, 3,
	-3, 1,
	-1, 3,
	-3, 0,
	 0, 3

};
static const int NavPlaceTopLeftCount = 15;

static const int NavPlaceTopRightOffsets[30] = {
	1, 1,
	1, 0,
	0, 1,
	2, 2,
	2, 1,
	1, 2,
	2, 0,
	0, 2,
	3, 3,
	3, 2,
	2, 3,
	3, 1,
	1, 3,
	3, 0,
	0, 3
};
static const int NavPlaceTopRightCount = 15;

static const int NavPlaceBottomOffsets[30] = {
	 0, -1,
	-1, -1,
	 1, -1,
	 0, -2,
	-1, -2,
	 1, -2,
	-2, -2,
	 2, -2,
	 0, -3,
	-1, -3,
	 1, -3,
	-2, -3,
	 2, -3,
	-3, -3,
	 3, -3
};
static const int NavPlaceBottomCount = 15;

static const int NavPlaceTopOffsets[30] = {
	 0, 1,
	-1, 1,
	 1, 1,
	 0, 2,
	-1, 2,
	 1, 2,
	-2, 2,
	 2, 2,
	 0, 3,
	-1, 3,
	 1, 3,
	-2, 3,
	 2, 3,
	-3, 3,
	 3, 3
};
static const int NavPlaceTopCount = 15;

static const int NavPlaceLeftOffsets[30] = {
	-1,  0,
	-1, -1,
	-1,  1,
	-2,  0,
	-2, -1,
	-2,  1,
	-2, -2,
	-2,  2,
	-3,  0,
	-3, -1,
	-3,  1,
	-3, -2,
	-3,  2,
	-3, -3,
	-3,  3
};
static const int NavPlaceLeftCount = 15;

static const int NavPlaceRightOffsets[30] = {
	1,  0,
	1, -1,
	1,  1,
	2,  0,
	2, -1,
	2,  1,
	2, -2,
	2,  2,
	3,  0,
	3, -1,
	3,  1,
	3, -2,
	3,  2,
	3, -3,
	3,  3
};
static const int NavPlaceRightCount = 15;

NavMap::NavMap(int w, int h) : _width(w), _height(h)
{
	_map=new nByte[w*h];
	memset(_map,0,sizeof(nByte)*w*h);

	_pathFinder=new AS_PathFinder(w,h);
}

bool NavMap::findBestPlace(int x, int y, float dx, float dy, int& rx, int& ry, bool reverse) const
{
#define PLACE_TEST(tx,ty)		\
	if (getFlag(tx, ty, NAV_WAKABLE))	{ rx = tx; ry = ty; return true; }

	if (getFlag(x, y, NAV_WAKABLE)==0)
	{
		int dix = (int)roundf(dx);
		int diy = (int)roundf(dy);

		const int *offsets = 0;
		int offsetCount = 0;

		if (dix < 0)
		{
			if (diy < 0)
			{
				offsets = NavPlaceBottomLeftOffsets;
				offsetCount = NavPlaceBottomLeftCount;
			}
			else if (diy > 0)
			{
				offsets = NavPlaceTopLeftOffsets;
				offsetCount = NavPlaceTopLeftCount;
			}
			else
			{
				offsets = NavPlaceLeftOffsets;
				offsetCount = NavPlaceLeftCount;
			}
		}
		else if (dix > 0)
		{
			if (diy < 0)
			{
				offsets = NavPlaceBottomRightOffsets;
				offsetCount = NavPlaceBottomRightCount;
			}
			else if (diy > 0)
			{
				offsets = NavPlaceTopRightOffsets;
				offsetCount = NavPlaceTopRightCount;
			}
			else
			{
				offsets = NavPlaceRightOffsets;
				offsetCount = NavPlaceRightCount;
			}
		}
		else
		{
			if (diy < 0)
			{
				offsets = NavPlaceBottomOffsets;
				offsetCount = NavPlaceBottomCount;
			}
			else
			{
				offsets = NavPlaceTopOffsets;
				offsetCount = NavPlaceTopCount;
			}
		}

		int testx, testy;
		for (int i = 0; i < offsetCount; ++i)
		{
			testx = x + offsets[i * 2];
			testy = y + offsets[i * 2 + 1];

			if (getFlag(testx, testy, NAV_WAKABLE))
			{
				rx = testx;
				ry = testy;
				return true;
			}
		}

		return false;

/*		if (dix < 0)
		{
			if (diy < 0)
			{
				int sx = x - 1;
				int sy;

				for (int i = 0; i<BESTPLACE_MAX_ITER; ++i)
				{
					for (sy = y; sy >= y - i -1; --sy)
					{
						PLACE_TEST(sx, sy);
					}

					sy = y - i - 1;

					for (int ssx = sx + 1; ssx <= x; ++ssx)
					{
						PLACE_TEST(ssx, sy);
					}

					--sx;
				}

				return false;
			}
			else if (diy > 0)
			{
				int sx = x - 1;
				int sy;

				for (int i = 0; i<BESTPLACE_MAX_ITER; ++i)
				{
					for (sy = y; sy <= y + i + 1; ++sy)
					{
						PLACE_TEST(sx, sy);
					}

					sy = y + i + 1;

					for (int ssx = sx + 1; ssx <= x; ++ssx)
					{
						PLACE_TEST(ssx, sy);
					}

					--sx;
				}
				return false;
			}
			else
			{
				int sx = x - 1;
				for (int i = 0; i<BESTPLACE_MAX_ITER; ++i)
				{
					for (int sy = y - i - 1; sy <= y + i + 1; ++sy)
					{
						PLACE_TEST(sx, sy);
					}

					--sx;
				}

				return false;
			}
		}
		else if (dix > 0)
		{
			if (diy < 0)
			{
				int sx = x + 1;
				int sy;

				for (int i = 0; i<BESTPLACE_MAX_ITER; ++i)
				{
					for (sy = y; sy >= y - i - 1; --sy)
					{
						PLACE_TEST(sx, sy);
					}

					sy = y - i - 1;

					for (int ssx = sx - 1; ssx >= x; --ssx)
					{
						PLACE_TEST(ssx, sy);
					}

					++sx;
				}

				return false;
			}
			else if (diy > 0)
			{
				int sx = x - 1;
				int sy;

				for (int i = 0; i<BESTPLACE_MAX_ITER; ++i)
				{
					for (sy = y; sy <= y + i + 1; ++sy)
					{
						PLACE_TEST(sx, sy);
					}

					sy = y + i + 1;

					for (int ssx = sx - 1; ssx >= x; --ssx)
					{
						PLACE_TEST(ssx, sy);
					}

					++sx;
				}
				return false;
			}
			else
			{
				int sx = x + 1;
				for (int i = 0; i<BESTPLACE_MAX_ITER; ++i)
				{
					for (int sy = y - i - 1; sy <= y + i + 1; ++sy)
					{
						PLACE_TEST(sx, sy);
					}

					++sx;
				}

				return false;
			}
		}
		else
		{
			if (dy < 0)
			{
				int sy = y - 1;
				for (int i = 0; i < BESTPLACE_MAX_ITER; ++i)
				{
					for (int sx = x - i - 1; sx <= x + i + 1; ++sx)
					{
						PLACE_TEST(sx, sy);
					}

					--sy;
				}

				return false;
			}
			else
			{
				int sy = y + 1;
				for (int i = 0; i < BESTPLACE_MAX_ITER; ++i)
				{
					for (int sx = x - i - 1; sx <= x + i + 1; ++sx)
					{
						PLACE_TEST(sx, sy);
					}

					++sy;
				}

				return false;
			}
		}*/
	}
	else
	{
		rx = x;
		ry = y;
		return true;
	}
}