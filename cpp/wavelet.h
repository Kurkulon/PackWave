#ifndef WAVELET_H__INCLUDED
#define WAVELET_H__INCLUDED

#include "types.h"

//#define FWT_LOG2N	6
//#define FWT_MIN		2
#define FWT_DAUB8

#define FWT_FLOAT

#ifndef FWT_FLOAT

typedef i16 FWT_DATA;
typedef i16 FWT_FLTR;

#define FWT_FIXBITS 10

#define FLOAT_FWT(x)	((int)((x)*(1<<FWT_FIXBITS)+.5))
#define MULT_FWT(x)		(((x)+(1<<(FWT_FIXBITS-1)))>>FWT_FIXBITS)

#else

typedef float FWT_DATA;
typedef float FWT_FLTR;

#define FLOAT_FWT(x)	((FWT_FLTR)(x))
#define MULT_FWT(x)		(x)

#endif 

extern void Wavelet(FWT_DATA *vec, u16 log2n, u16 step, FWT_DATA *tmp);
extern void InvWavelet(FWT_DATA *f, u16 log2n, u16 step, FWT_DATA *tmp);

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#endif

