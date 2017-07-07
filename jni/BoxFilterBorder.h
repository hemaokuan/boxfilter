#pragma once

#include "Utils.h"

class BoxFilterBorder {
public:
	BoxFilterBorder(void);
	~BoxFilterBorder(void);

	void init(int width, int height, int mWidth = 5, int mHeight = 5);
	void boxfilter(uchar* img, uchar* dst);

private:
	int mWidth;
	int mHeight;
	int width;
	int height;
};