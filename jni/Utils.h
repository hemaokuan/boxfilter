//
// Created by bruce on 2017/7/5.
//

#ifndef MYAPPLICATION_UTILS_H
#define MYAPPLICATION_UTILS_H

#define abs(v) (v) >= 0 ? (v) : -(v)
#define min(x,y) (x) < (y) ? (x) : (y)

typedef unsigned char uchar;
typedef unsigned short ushort;

double now_ms(void);
bool CheckRAWFile(uchar* m_pData, int m_nWidth, int m_nHeight, char *pFileName);

#endif //MYAPPLICATION_UTILS_H
