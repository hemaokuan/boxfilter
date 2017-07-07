#include <string.h>
#include "BoxFilterBorder.h"


BoxFilterBorder::BoxFilterBorder(void) {

}

BoxFilterBorder::~BoxFilterBorder(void) {

}

void BoxFilterBorder::init(int width, int height, int mWidth, int mHeight) {
	this->mWidth = mWidth;
	this->mHeight = mHeight;
	this->width = width;
	this->height = height;
}

void BoxFilterBorder::boxfilter(uchar* img, uchar* dst) {
	int j, x, y;
    int sum = mWidth*mHeight;
    int nStart = mWidth/2;

    int* buff = new int[width];
	memset(buff, 0, width * sizeof(int));

	for (y = 0; y < mHeight; y++) {

		int m = abs(y - nStart);

		for (x = 0; x < width; x++) {
			uchar pixel = img[m*width + x];
			buff[x] += pixel;
		}
	}

	for (y = 0; y < height; y++) {
		int Xsum = 0;
		
		for (j = 0; j < mWidth; j++) {
			int n = abs(j - nStart);
			Xsum += buff[n];
		}

		for (x = 0; x < width; x++) {

			dst[y*width + x] = (Xsum + sum / 2) / sum;

			int n0 = abs(x - nStart);
			int n1 = (x + nStart + 1);
			n1 = n1 >= width ? 2 * width - n1 - 2 : n1;

			Xsum = Xsum - buff[n0] + buff[n1];
		}

		int m0 = abs(y - nStart);
		int m1 = y + nStart + 1;
		m1 = m1 >= height ? 2 * height - 2 - m1:m1;

		for (x = 0; x < width; x++) {
			uchar pixel = img[m0*width + x];
			uchar pixel2 = img[m1*width + x];
			buff[x] = buff[x] - pixel + pixel2;
		}
	}

    delete[] buff;
}



