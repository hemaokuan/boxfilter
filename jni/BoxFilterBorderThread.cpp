#include "BoxFilterBorderThread.h"
#include <string.h>
#include <stdio.h>
#include <pthread.h>

#define THREAD_NUM 4

BoxFilterBorderThread::BoxFilterBorderThread(void) {

}

BoxFilterBorderThread::~BoxFilterBorderThread(void) {

}

void BoxFilterBorderThread::init(int width, int height, int mWidth, int mHeight) {
	this->mWidth = mWidth;
	this->mHeight = mHeight;
	this->width = width;
	this->height = height;
}

void* boxfilterF(void* arg) {
	BoxFilterParam* pParam = (BoxFilterParam*)arg;
	int mWidth = pParam->pCommonParam->mWidth;
	int mHeight = pParam->pCommonParam->mHeight;
	int width = pParam->pCommonParam->width;
	int height = pParam->pCommonParam->height;
	int nStart = pParam->pCommonParam->nStart;
	uchar* img = pParam->pCommonParam->img;
	uchar* dst = pParam->pCommonParam->dst;

	int startH = pParam->startH;
	int endH = pParam->endH;
    int sum = mWidth*mHeight;

    printf("start thread id = %d\n", pParam->thread_no);
    double start = now_ms();

    int *buff = new int[width];
    memset(buff, 0, width *sizeof(int));

	int j, x, y;
	for (y = startH; y < startH + mHeight; y++) {

		int m = abs(y - nStart);

		for (x = 0; x < width; x++) {
			uchar pixel = img[m*width + x];
			buff[x] += pixel;
		}
	}

	for (y = startH; y < endH; y++) {
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
		m1 = m1 >= height ? 2 * height - 2 - m1 : m1;

		for (x = 0; x < width; x++) {
			uchar pixel = img[m0*width + x];
			uchar pixel2 = img[m1*width + x];
			buff[x] = buff[x] - pixel + pixel2;
		}
	}

    double end = now_ms();
    printf("end thread id = %d, cost = %f ms\n",  pParam->thread_no, end-start);

    delete[] buff;
	return NULL;
}

void BoxFilterBorderThread::boxfilter(uchar* img, uchar* dst) {
	BoxFilterCommonParam cPara = { mWidth, mHeight, width, height, mWidth/2, img, dst};
	BoxFilterParam tPara[THREAD_NUM];
	pthread_t thread[THREAD_NUM];
	int heightUnit = (height + THREAD_NUM / 2) / THREAD_NUM;
	for (int i = 0; i < THREAD_NUM; i++) {
		tPara[i].startH = i*heightUnit;
		tPara[i].endH = min((i + 1)*heightUnit, height);
		tPara[i].thread_no = i;
		tPara[i].pCommonParam = &cPara;
		pthread_create(&thread[i], NULL, boxfilterF, &tPara[i]);
	}

	for (int k = 0; k < THREAD_NUM; k++) {
		pthread_join(thread[k], NULL);
	}
}





