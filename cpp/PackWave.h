#ifndef PACKWAVE_H__31_01_2024__17_19
#define PACKWAVE_H__31_01_2024__17_19

#include "..\..\G_LIBS_BF592\cpp\types.h"
#include "..\..\G_LIBS_BF592\cpp\fdct.h"
#include "..\..\G_LIBS_BF592\cpp\mdct.h"

enum PackType { PACK_NO = 0, PACK_ULAW12, PACK_ULAW16, PACK_ADPCMIMA, PACK_DCT0, PACK_DCT1, PACK_DCT2, PACK_DCT3 };

extern float PW_Determination(i16* w1, i16* w2, u16 len, float* pE);
extern void PW_Pack_uLaw_12Bit(i16* src, byte* dst, u16 len);
extern void PW_Unpack_uLaw_12Bit(byte* src, i16* dst, u16 len);
extern void PW_Pack_uLaw_16Bit(i16* src, byte* dst, u16 len);
extern void PW_Unpack_uLaw_16Bit(byte* src, i16* dst, u16 len);
extern void PW_Pack_ADPCMIMA(i16* src, byte* dst, u16 len);
extern void PW_Unpack_ADPCMIMA(byte* src, i16* dst, u16 len);

extern	u16	 PW_Pack_FDCT(i16* src, byte* dst, u16 len, u16 shift, u16 OVRLAP, u16 maxPackLen, u16* packedLen);
extern	u16  PW_Unpack_FDCT(byte* src, i16* dst, u16 srcLen, u16 OVRLAP);

extern u16 PW_TestPackUnpack(i16* src, i16* dst, u16 len, u16 packType, float* pCompressRatio, float* pDet, float* pDev); // возвращает длину распакованой волнушки

extern u16 WavePack_MDCT(MDCT_LookUp* init, i16* src, MDCT_DATA* dst, u16 len, u16 shift, u16* packedLen);
extern u16 WaveUnpack_MDCT(MDCT_LookUp* init, MDCT_DATA* src, i16* dst, u16 len);

#endif //PACKWAVE_H__31_01_2024__17_19
