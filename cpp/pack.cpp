//#include "types.h"

#define FDCT_LOG2N 6

#include "..\..\G_LIBS\cpp\WavePack\fdct_imp.h"
#include "..\..\G_LIBS\cpp\WavePack\mdct_imp.h"
#include "..\..\G_LIBS\cpp\WavePack\wavepack_imp.h"

#include "PackWave.h"

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void PW_Pack_uLaw_12Bit(i16* src, byte* dst, u16 len)
{
	WavePack_uLaw_12Bit(src, dst, len);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void PW_Unpack_uLaw_12Bit(byte* src, i16* dst, u16 len)
{
	WaveUnpack_uLaw_12Bit(src, (u16*)dst, len);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void PW_Pack_uLaw_16Bit(i16* src, byte* dst, u16 len)
{
	WavePack_uLaw_16Bit(src, dst, len);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void PW_Unpack_uLaw_16Bit(byte* src, i16* dst, u16 len)
{
	WaveUnpack_uLaw_16Bit(src, (u16*)dst, len);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void PW_Pack_ADPCMIMA(i16* src, byte* dst, u16 len)
{
	WavePack_ADPCMIMA(src, dst, len);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void PW_Unpack_ADPCMIMA(byte* src, i16* dst, u16 len)
{
	WaveUnpack_ADPCMIMA(src, (u16*)dst, len);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

u16 PW_Pack_FDCT(i16* src, byte* dst, u16 len, u16 shift, u16 OVRLAP, u16 maxPackLen, u16* packedLen)
{
	return WavePack_FDCT12(src, dst, len, shift, OVRLAP, maxPackLen, packedLen);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

u16 PW_Unpack_FDCT(byte* src, i16* dst, u16 srcLen, u16 OVRLAP)
{
	return WaveUnpack_FDCT12(src, dst, srcLen, OVRLAP);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

float PW_Determination(i16* w1, i16* w2, u16 len, float* pE)
{
    if (len == 0) return 0;

    float e = 0;
    float y = 0;

    for (u32 i = 0; i < len; i++)
    {
        float t = (float)(w1[i] - w2[i]);
        e += t * t;

        y += (float)(w1[i] * w1[i]);
    };

    if (pE != 0) *pE = sqrtf(e / len);

    return (1 - e / (y - y / len));
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

u16 PW_TestPackUnpack(i16* src, i16* dst, u16 len, u16 packType, float* pCompressRatio, float* pDet, float* pDev)
{
    byte buf[4092];

    float det = 0;
    float dev = 0;
    float compressRatio = 0;

    u16 packedLen = 0;

    switch (packType)
    {
    case PACK_ULAW12:

        len &= ~1;

        if (len > ArraySize(buf)) len = ArraySize(buf);

        PW_Pack_uLaw_12Bit(src, buf, len);
        PW_Unpack_uLaw_12Bit(buf, dst, len);

        det = PW_Determination(src, dst, len, &dev);
        compressRatio = 2;

        break;

    case PACK_ULAW16:

        len &= ~1;

        if (len > ArraySize(buf)) len = ArraySize(buf);

        PW_Pack_uLaw_16Bit(src, buf, len);
        PW_Unpack_uLaw_16Bit(buf, dst, len);

        det = PW_Determination(src, dst, len, &dev);
        compressRatio = 2;

        break;

    case PACK_ADPCMIMA:

        len &= ~3;

        if (len > ArraySize(buf)) len = ArraySize(buf);

        PW_Pack_ADPCMIMA(src, buf, len);
        PW_Unpack_ADPCMIMA(buf, dst, len);

        det = PW_Determination(src, dst, len, &dev);
        compressRatio = 4;

        break;

    case PACK_DCT0:

        if (len > ArraySize(buf)) len = ArraySize(buf);

        len = PW_Pack_FDCT(src, buf, len, 4, 15, FDCT_N, &packedLen);
        len = PW_Unpack_FDCT(buf, dst, packedLen, 15);

        det = PW_Determination(src, dst, len, &dev);
        compressRatio = len * 2.0f / packedLen;

        break;

    case PACK_DCT1:

        if (len > ArraySize(buf)) len = ArraySize(buf);

        len = PW_Pack_FDCT(src, buf, len, 3, 15, FDCT_N, &packedLen);
        len = PW_Unpack_FDCT(buf, dst, packedLen, 15);

        det = PW_Determination(src, dst, len, &dev);
        compressRatio = len * 2.0f / packedLen;

        break;

    case PACK_DCT2:

        if (len > ArraySize(buf)) len = ArraySize(buf);

        len = PW_Pack_FDCT(src, buf, len, 2, 15, FDCT_N, &packedLen);
        len = PW_Unpack_FDCT(buf, dst, packedLen, 15);

        det = PW_Determination(src, dst, len, &dev);
        compressRatio = len * 2.0f / packedLen;

        break;

    case PACK_DCT3:

        if (len > ArraySize(buf)) len = ArraySize(buf);

        len = PW_Pack_FDCT(src, buf, len, 1, 15, FDCT_N, &packedLen);
        len = PW_Unpack_FDCT(buf, dst, packedLen, 15);

        det = PW_Determination(src, dst, len, &dev);
        compressRatio = len * 2.0f / packedLen;

        break;
    };

    if (pCompressRatio != 0) *pCompressRatio = compressRatio;

    if (pDet != 0) *pDet = det;
    if (pDev != 0) *pDev = dev;

    return len;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
