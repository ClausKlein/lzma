/* 7zCrc.h -- CRC32 calculation
2013-01-18 : Igor Pavlov : Public domain */

#ifndef _7Z_CRC_H   // NOLINT
#define _7Z_CRC_H   // NOLINT

#include "7zTypes.h"

EXTERN_C_BEGIN

extern UInt32 g_CrcTable[]; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

/* Call CrcGenerateTable one time before other CRC functions */
void MY_FAST_CALL CrcGenerateTable(void);

#define CRC_INIT_VAL 0xFFFFFFFF
#define CRC_GET_DIGEST(crc) ((crc) ^ CRC_INIT_VAL)
#define CRC_UPDATE_BYTE(crc, b) (g_CrcTable[((crc) ^ (b)) & 0xFF] ^ ((crc) >> 8))

UInt32 MY_FAST_CALL CrcUpdate(UInt32 crc, const void *data, size_t size);
UInt32 MY_FAST_CALL CrcCalc(const void *data, size_t size);

EXTERN_C_END

#endif
