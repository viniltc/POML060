#ifndef CRC32_H
#define CRC32_H

#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>
#include <stddef.h>

/**
 * Cyclic Redundancy Check from ISO 3309, ITU-T V.42, Gzip and PNG.
 * Algorithms are described on Computation of CRC in Wikipedia.
 * This variant of CRC-32 uses LSB-first order, sets the initial CRC to FFFFFFFF16, and complements the final CRC.
 * This code adapted from http://rosettacode.org/wiki/CRC#C
 */
uint32_t crc32(uint32_t crc, const uint8_t *buf, size_t len);

/*
 * If a message m = payload + crc32(payload)
 * Then crc32(m)= CRC32_CHECKSUM_VALID
 */
#define CRC32_CHECKSUM_VALID (0x2144df1c)

#ifdef __cplusplus
}
#endif

#endif // CRC32_H
