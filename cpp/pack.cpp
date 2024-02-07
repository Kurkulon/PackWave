//#include "types.h"

#include "..\..\G_LIBS_BF592\cpp\fdct_imp.h"
#include "..\..\G_LIBS_BF592\cpp\mdct_imp.h"
#include "..\..\G_LIBS_BF592\cpp\wavepack_imp.h"

#include "PackWave.h"

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void PW_Pack_uLaw_12Bit(i16* src, byte* dst, u16 len)
{
	WavePack_uLaw_12Bit((u16*)src, dst, len);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void PW_Unpack_uLaw_12Bit(byte* src, i16* dst, u16 len)
{
	WaveUnpack_uLaw_12Bit(src, (u16*)dst, len);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void PW_Pack_uLaw_16Bit(i16* src, byte* dst, u16 len)
{
	WavePack_uLaw_16Bit((u16*)src, dst, len);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void PW_Unpack_uLaw_16Bit(byte* src, i16* dst, u16 len)
{
	WaveUnpack_uLaw_16Bit(src, (u16*)dst, len);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void PW_Pack_ADPCMIMA(i16* src, byte* dst, u16 len)
{
	WavePack_ADPCMIMA((u16*)src, dst, len);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void PW_Unpack_ADPCMIMA(byte* src, i16* dst, u16 len)
{
	WaveUnpack_ADPCMIMA(src, (u16*)dst, len);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

u16 PW_Pack_FDCT(i16* src, byte* dst, u16 len, u16 shift, u16 OVRLAP, u16 maxPackLen, u16* packedLen)
{
	return WavePack_FDCT(src, dst, len, shift, OVRLAP, maxPackLen, packedLen);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

u16 PW_Unpack_FDCT(byte* src, i16* dst, u16 srcLen, u16 OVRLAP)
{
	return WaveUnpack_FDCT(src, dst, srcLen, OVRLAP);
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

#define MAX_SAMPLE_RATE 32000
#define MAX_FRAMESIZE   (MAX_SAMPLE_RATE/50)
#define MEASURE_WMOPS   1
#define WMOPS           1

void PW_Pack_G722(Word16* input, Word16* out_words, Word16 frame_size, Word16 number_of_bits_per_frame, Word16 number_of_regions)
{
    //Word16  samples;
   // Word16  input[MAX_FRAMESIZE];
    Word16  history[MAX_FRAMESIZE];
    //Word16  number_of_16bit_words_per_frame;
    //Word16  out_words[MAX_BITS_PER_FRAME / 16];
    Word16  mag_shift;
    //Word16  i;
    Word16  mlt_coefs[MAX_FRAMESIZE];
    Word16  frame_cnt = 0;

    for (Word16 i = 0; i < frame_size; i++) history[i] = 0;

    /* Convert input samples to rmlt coefs */
    mag_shift = samples_to_rmlt_coefs(input, history, mlt_coefs, frame_size);


    /* Encode the mlt coefs */
    encoder(number_of_bits_per_frame, number_of_regions, mlt_coefs, mag_shift, out_words);

}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void PW_Unpack_G722(Word16* out_words, Word16* output, Word16 frame_size, Word16 number_of_bits_per_frame, Word16 number_of_regions)
{
    Word16 i;
   // Word16 words;
   // Word16 output[MAX_DCT_LENGTH];
    //Word16 number_of_16bit_words_per_frame;
    //Word16 out_words[MAX_BITS_PER_FRAME / 16];
    Word16 frame_error_flag = 0;
    Word16 frame_cnt = 0;
    Word16 decoder_mlt_coefs[MAX_DCT_LENGTH];
    Word16 mag_shift;
    Word16 old_mag_shift = 0;
    Word16 old_decoder_mlt_coefs[MAX_DCT_LENGTH];
    Word16 old_samples[MAX_DCT_LENGTH >> 1];
    Bit_Obj bitobj;
    Rand_Obj randobj;

    for (i = 0; i < frame_size; i++) old_decoder_mlt_coefs[i] = 0;

    for (i = 0; i < (frame_size >> 1); i++) old_samples[i] = 0;

    /* initialize the random number generator */
    randobj.seed0 = 1;
    randobj.seed1 = 1;
    randobj.seed2 = 1;
    randobj.seed3 = 1;

    /* reinit the current word to point to the start of the buffer */
    bitobj.code_word_ptr = out_words;
    bitobj.current_word = *out_words;
    bitobj.code_bit_count = 0;
    bitobj.number_of_bits_left = number_of_bits_per_frame;

    decoder(&bitobj, &randobj, number_of_regions, decoder_mlt_coefs, &mag_shift, &old_mag_shift, old_decoder_mlt_coefs, frame_error_flag);

    /* convert the decoder_mlt_coefs to samples */
    rmlt_coefs_to_samples(decoder_mlt_coefs, old_samples, output, frame_size, mag_shift);

    /* For ITU testing, off the 2 lsbs. */
    for (i = 0; i < frame_size; i++) output[i] &= 0xfffc;

}

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

        len = PW_Pack_FDCT(src, buf, len, 4, 3, FDCT_N, &packedLen);
        len = PW_Unpack_FDCT(buf, dst, packedLen, 3);

        det = PW_Determination(src, dst, len, &dev);
        compressRatio = len * 2.0f / packedLen;

        break;

    case PACK_DCT1:

        if (len > ArraySize(buf)) len = ArraySize(buf);

        len = PW_Pack_FDCT(src, buf, len, 3, 7, FDCT_N, &packedLen);
        len = PW_Unpack_FDCT(buf, dst, packedLen, 7);

        det = PW_Determination(src, dst, len, &dev);
        compressRatio = len * 2.0f / packedLen;

        break;

    case PACK_DCT2:

        if (len > ArraySize(buf)) len = ArraySize(buf);

        len = PW_Pack_FDCT(src, buf, len, 2, 7, FDCT_N, &packedLen);
        len = PW_Unpack_FDCT(buf, dst, packedLen, 7);

        det = PW_Determination(src, dst, len, &dev);
        compressRatio = len * 2.0f / packedLen;

        break;

    case PACK_DCT3:

        if (len > ArraySize(buf)) len = ArraySize(buf);

        len = PW_Pack_FDCT(src, buf, len, 1, 7, FDCT_N, &packedLen);
        len = PW_Unpack_FDCT(buf, dst, packedLen, 7);

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
