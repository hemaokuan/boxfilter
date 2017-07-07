#pragma once

#include "Utils.h"

class BoxFilterBorderThread {
public:
	BoxFilterBorderThread(void);
	~BoxFilterBorderThread(void);

	void init(int width, int height, int mWidth = 5, int mHeight = 5);
	void boxfilter(uchar* img, uchar* dst);

private:
	int mWidth;
	int mHeight;
	int width;
	int height;
};


typedef struct _BoxFilterCommonParam {
	int mWidth;
	int mHeight;
	int width;
	int height;
	int nStart;
	uchar* img;
	uchar* dst;
} BoxFilterCommonParam;

typedef struct _BoxFilterParam {
	int startH;
	int endH;
	int thread_no;
	BoxFilterCommonParam* pCommonParam;
} BoxFilterParam;