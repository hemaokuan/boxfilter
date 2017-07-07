
#include <stdio.h>
#include <stdlib.h>
#include "BoxFilterNeon.h"
#include "BoxFilterBorder.h"
#include "BoxFilterBorderThread.h"
#include "BoxFilterBorderNeonThread.h"

#define TESTCASE3

static int FRAME_WIDTH, FRAME_HEIGHT, FRAME_SIZE;

static char* getTestCaseOutput() {
#ifdef TESTCASE1
    return "/sdcard/bruce/flower_blur.yuv";
#endif

#ifdef TESTCASE2
    return "sdcard/bruce/insect_blur.yuv";
#endif

#ifdef TESTCASE3
    return "/sdcard/bruce/teleRectedImage_blur.nv21";
#endif
}

static char* getTestCaseInput() {
#ifdef TESTCASE1
    return "/sdcard/bruce/flower.yuv";
#endif

#ifdef TESTCASE2
    return "/sdcard/bruce/insect.yuv";
#endif

#ifdef TESTCASE3
    return "/sdcard/bruce/teleRectedImage.nv21";
#endif
}

int main(int argc, char** argv) {

#ifdef TESTCASE1
    FRAME_WIDTH = 1024;
    FRAME_HEIGHT = 768;
#endif

#ifdef TESTCASE2
    FRAME_WIDTH = 650;
    FRAME_HEIGHT = 432;
#endif

#ifdef TESTCASE3
    FRAME_WIDTH = 4096;
    FRAME_HEIGHT = 3072;
#endif

    double start, end;

    FRAME_SIZE = FRAME_WIDTH*FRAME_HEIGHT*3/2;

    FILE* pInputFp = fopen(getTestCaseInput(), "rb");
    if (pInputFp == NULL) {
        printf("File Open fail.\n");
        exit(0);
    }

    uint8_t* src = (uint8_t*)malloc(sizeof(uint8_t) * FRAME_SIZE);
    uint8_t* dst = (uint8_t*)malloc(sizeof(uint8_t) * FRAME_SIZE);
    memset(src, 0, FRAME_SIZE);
    memset(dst, 0, FRAME_SIZE);

    fread(src, 1, FRAME_SIZE, pInputFp);
    memcpy(dst, src, FRAME_SIZE*sizeof(uint8_t));
    fclose(pInputFp);

    BoxFilterBorder box;
    box.init(FRAME_WIDTH, FRAME_HEIGHT, 5, 5);
    start = now_ms();
    box.boxfilter(src, dst);
    end = now_ms();
    printf("c blur cost: %f ms\n", end - start);


//        FILE* pOutputFp_neon = fopen(getTestCaseOutput(), "w+");
//        if (pOutputFp_neon == NULL) {
//            printf("File Open fail.\n");
//            free(src);
//            free(dst);
//            exit(0);
//        }
//        fwrite(dst, FRAME_SIZE, 1, pOutputFp_neon);
//        fclose(pOutputFp_neon);

//    BoxFilterNeon box_neon;
//    box_neon.init(FRAME_WIDTH, FRAME_HEIGHT, 5, 5);
//    start = now_ms();
//    box_neon.boxfilter(src, dst);
//    end = now_ms();
//    printf("neon blur cost: %f ms\n", end - start);

    BoxFilterBorderThread box_thread;
    box_thread.init(FRAME_WIDTH, FRAME_HEIGHT, 5, 5);
    start = now_ms();
    box_thread.boxfilter(src, dst);
    end = now_ms();
    printf("c_thread blur cost: %f ms\n", end - start);

//    BoxFilterBorderNeonThread box_neon_thread;
//    box_neon_thread.init(FRAME_WIDTH, FRAME_HEIGHT, 5, 5);
//    start = now_ms();
//    box_neon_thread.boxfilter(src, dst);
//    end = now_ms();
//    printf("neon_thread blur cost: %f ms\n", end - start);

    //Check Raw
    CheckRAWFile(dst, FRAME_WIDTH, FRAME_HEIGHT, getTestCaseOutput());

    free(src);
    free(dst);

    return 0;
}