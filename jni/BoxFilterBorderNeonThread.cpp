#include "BoxFilterBorderNeonThread.h"
#include <string.h>
#include <pthread.h>
#include "arm_neon.h"

#define THREAD_NUM 4

BoxFilterBorderNeonThread::BoxFilterBorderNeonThread(void) {

}

BoxFilterBorderNeonThread::~BoxFilterBorderNeonThread(void) {

}

void BoxFilterBorderNeonThread::init(int width, int height, int mWidth, int mHeight) {
	this->mWidth = mWidth;
	this->mHeight = mHeight;
	this->width = width;
	this->height = height;
}

void* boxfilterneonF(void* arg) {
	BoxFilterNeonParam* pParam = (BoxFilterNeonParam*)arg;
	int mWidth = pParam->pCommonParam->mWidth;
	int mHeight = pParam->pCommonParam->mHeight;
	int width = pParam->pCommonParam->width;
	int height = pParam->pCommonParam->height;
	int nStart = pParam->pCommonParam->nStart;
	uchar* img = pParam->pCommonParam->img;
	uchar* dst = pParam->pCommonParam->dst;

	int startH = pParam->startH;
	int endH = pParam->endH;

	int j, x, y;
	int sum = mWidth*mHeight;
	float fsum = 1.0/sum;

	int buffWidth = width + nStart*2 + 8;

	uint16_t* buff = new uint16_t[buffWidth];
	memset(buff, 0, buffWidth*sizeof(uint16_t));

	uint8x8_t pixel, pixel2;
	uint16x8_t buffvalue = vdupq_n_u16(0);

	for (y = startH; y < startH + mHeight; y++) {

		int h = abs(y - nStart);
		for (x = 0; x < width; x += 8) {
			int idx = x + nStart;

			pixel = vld1_u8(img+h*width+x);

			buffvalue = vld1q_u16(buff+idx);
			buffvalue = vaddq_u16(buffvalue, (vmovl_u8(pixel)));
			vst1q_u16(buff+idx, buffvalue);
		}
	}

	for (x = 0; x < nStart; x++) {
		buff[x] = buff[2*nStart-x];
	}

	for (x = width + nStart; x < buffWidth; x++) {
		buff[x] = buff[2*(width+nStart-1) -x];
	}

	uint32x4_t c0, c1;
	for (y = startH; y < endH; y++) {
		for (x = 0; x < width; x+=8) {
			c0 = vdupq_n_u32(sum/2);
			c1 = vdupq_n_u32(sum/2);
			for (j = 0; j < mWidth; j++) {
				uint16x8_t temp = vld1q_u16(buff+x+j);
				c0 = vaddw_u16(c0, vget_low_u16(temp));
				c1 = vaddw_u16(c1, vget_high_u16(temp));

			}

			uint16x8_t c = vcombine_u16(vmovn_u32(vcvtq_u32_f32(vmulq_n_f32(vcvtq_f32_u32(c0), fsum))),
										vmovn_u32(vcvtq_u32_f32(vmulq_n_f32(vcvtq_f32_u32(c1), fsum))));


			vst1_u8(dst+y*width+x, vmovn_u16(c));
		}

		int m0 = abs(y - nStart);
		int m1 = y+nStart+1;
		m1 = (m1 >= height) ? 2 * height - 2 -m1:m1;

		for (x = 0; x < width; x+=8) {
			int idx = x+nStart;
			pixel = vld1_u8(img+m0*width+x);
			pixel2 = vld1_u8(img+m1*width+x);

			buffvalue = vld1q_u16(buff+idx);
			buffvalue = vaddq_u16(vsubq_u16(buffvalue, (vmovl_u8(pixel))), (vmovl_u8(pixel2)));
			vst1q_u16(buff+idx, buffvalue);
		}

		for (x = 0; x < nStart; x++) {
			buff[x] = buff[2*nStart-x];
		}

		for (x = width+nStart; x < buffWidth; x++) {
			buff[x] = buff[2*(width+nStart-1)-x];
		}
	}

	delete[] buff;

	return NULL;
}

void BoxFilterBorderNeonThread::boxfilter(uchar* img, uchar* dst) {
	BoxFilterCommonNeonParam cPara = {mWidth, mHeight, width, height, mWidth/2, img, dst};
	BoxFilterNeonParam tPara[THREAD_NUM];
	pthread_t thread[THREAD_NUM];
	int heightUnit = (height + THREAD_NUM / 2) / THREAD_NUM;
	for (int i = 0; i < THREAD_NUM; i++) {
		tPara[i].startH = i*heightUnit;
		tPara[i].endH = min((i + 1)*heightUnit, height);
		tPara[i].thread_no = i;
		tPara[i].pCommonParam = &cPara;
		pthread_create(&thread[i], NULL, boxfilterneonF, &tPara[i]);
	}

	for (int k = 0; k < THREAD_NUM; k++) {
		pthread_join(thread[k], NULL);
	}
}