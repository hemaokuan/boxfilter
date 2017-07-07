//
// Created by bruce on 2017/7/5.
//

#include <stdio.h>
#include <string.h>
#include <time.h>
#include "Utils.h"

double now_ms(void)
{
    struct timespec res;
    clock_gettime(CLOCK_REALTIME, &res);
    return 1000.0*res.tv_sec + (double)res.tv_nsec/1e6;
}

bool CheckRAWFile(uchar* m_pData, int m_nWidth, int m_nHeight, char* pFileName)
{
    FILE * fp = fopen(pFileName, "rb");
    int mismatch = 0;
    if (fp == NULL)
    {
        printf("refc can't be opened: %s\n", pFileName);
        return false;
    }
    else
    {
        printf("#### check result for %s\n", pFileName);

        uchar * ptest = new uchar[m_nWidth];
        for (int line = 0; line < m_nHeight; line++)
        {

            if (mismatch<10)
            {
                fread(ptest, sizeof(uchar), m_nWidth, fp);
                if (memcmp(ptest, m_pData + line * m_nWidth, m_nWidth * sizeof(uchar)))
                {
                    mismatch++;
                    printf("\t!!!! %d line mismatch\n", line);
                    uchar* pcurr = m_pData + line * m_nWidth;
                    for (int nn = 0; nn<m_nWidth; nn++)
                        if (ptest[nn] != pcurr[nn])
                            printf("%d: %04x %04x\n", nn, ptest[nn], pcurr[nn]);
                }
            }

        }


        delete[] ptest;
        fclose(fp);
        fp = NULL;
    }

    return true;
}

