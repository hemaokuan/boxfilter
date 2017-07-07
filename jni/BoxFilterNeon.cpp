#include "BoxFilterNeon.h"
#include <string.h>
#include <stdio.h>
#include <arm_neon.h>


BoxFilterNeon::BoxFilterNeon(void) {

}

BoxFilterNeon::~BoxFilterNeon(void) {

}

void BoxFilterNeon::init(int width, int height, int mWidth, int mHeight) {
	this->width = width;
	this->height = height;
    this->mWidth = mWidth;
    this->mHeight = mHeight;
}

void BoxFilterNeon::boxfilter(uchar* img, uchar* dst) {
	int j,x,y;
    int sum = mWidth*mHeight;
    int nStart = mWidth/2;

	float fsum = 1.0/sum;

    int buffWidth = width + nStart*2 + 8;

    uint16_t* buff = new uint16_t[buffWidth];
    memset(buff, 0, buffWidth*sizeof(uint16_t));

    uint8x8_t pixel, pixel2;
    uint16x8_t buffvalue = vdupq_n_u16(0);
    for (y = 0; y < mHeight; y++) {
        int h = abs(y - nStart);
        for (x = 0; x < width; x += 8) {
            int idx = x + nStart;

            pixel = vld1_u8(img + h * width + x);

            buffvalue = vld1q_u16(buff + idx);
            buffvalue = vaddq_u16(buffvalue, (vmovl_u8(pixel)));
            vst1q_u16(buff + idx, buffvalue);
        }
    }

    for (x = 0; x < nStart; x++) {
        buff[x] = buff[2*nStart-x];
    }

    for (x = width + nStart; x < buffWidth; x++) {
        buff[x] = buff[2*(width+nStart-1) -x];
    }

    uint32x4_t c0, c1;
    for (y = 0; y < height; y++) {
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
}


