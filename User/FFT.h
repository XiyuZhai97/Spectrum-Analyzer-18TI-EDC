#ifndef FFT_H
#define FFT_H
                                                 //���帣��Ҷ�任�ĵ���
#define PI 3.1415926535897932384626433832795028841971               //����Բ����ֵ

struct compx {float real,imag;};                                    //����һ�������ṹ

extern void create_sin_tab(float *sin_t,int PointNum);
extern void FFT(struct compx *xin);

#endif // FFT_H
