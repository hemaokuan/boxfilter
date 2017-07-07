#pragma once

#include "Utils.h"


class BoxFilterBorderNeonThread {
public:
	BoxFilterBorderNeonThread(void);
	~BoxFilterBorderNeonThread(void);

	void init(int width, int height, int mWidth = 5, int mHeight = 5);
	void boxfilter(uchar* img, uchar* dst);

private:
	int mWidth;
	int mHeight;
	int width;
	int height;
};


typedef struct _BoxFilterCommonNeonParam {
	int mWidth;
	int mHeight;
	int width;
	int height;
	int nStart;
	uchar* img;
	uchar* dst;
} BoxFilterCommonNeonParam;

typedef struct _BoxFilterNeonParam {
	int startH;
	int endH;
	int thread_no;
	BoxFilterCommonNeonParam* pCommonParam;
} BoxFilterNeonParam;