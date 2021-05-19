/*
 * Written in 2009 by Lloyd Hilaiel
 *
 * License
 *
 * All the cruft you find here is public domain.  You don't have to credit
 * anyone to use this code, but my personal request is that you mention
 * Igor Pavlov for his hard, high quality work.
 *
 * Various compiled-in tests for the easylzma library which excercise
 * API correctness and handling of corrupt data.
 */

#include "simple.h"

#include <stdio.h>
#include <string.h>

static const char *sampleData =
    "Overview\n"
    "\n"
    "Easylzma is a C library and command line tools for LZMA compression and \n"
    "decompression.  It uses a Igor Pavlov's reference implementation and SDK\n"
    "written in C.\n"
    "\n"
    "License\n"
    "\n"
    "All the cruft you find here is public domain.  You don't have to credit\n"
    "anyone to use this code, but my personal request is that you mention\n"
    "Igor Pavlov for his hard, high quality work.\n"
    "\n"
    "Project Goals\n"
    "\n"
    "1. A tiny C wrapper and portable build system around a subset of\n"
    "   Igor Pavlov's public domain LZMA compression and decompression\n"
    "   implementation.\n"
    "2. A tiny and straighforward API\n"
    "3. Support for multiple different prominent LZMA file formats (see "
    "section on\n"
    "   file formats below)\n"
    "4. easy to build and use everywhere (doze and nix alike)\n"
    "5. public domain licensing through and through.  (hats off to Igor)\n"
    "\n"
    "Current State:\n"
    "\n"
    "THIS IS A WORK IN PROGRESS.  The code here should be considered "
    "pre-alpha,\n"
    "and this should only be used by tinkerers or hackers at this point.  "
    "Once\n"
    "feature completion is attained this message will be updated.  See the\n"
    "TODO file distributed with the source for remaining work to be done.\n"
    "\n"
    "Platforms Supported\n"
    "\n"
    "0.0.2 has been successfully compiled and run basic round trip testing\n"
    "on the following platforms & compilers:\n"
    "\n"
    "  * win32 - visual studio 2005\n"
    "  * osx - 10.4 & 10.5 (intel)\n"
    "  * netbsd ppc - 4.0.1 with gcc 4.1.2\n"
    "    (NOTE: memory allocation errors when dict size is default)\n"
    "  * freebsd 6.1 - amd64 gcc 3.4.4\n"
    "\n"
    "Features\n"
    "\n"
    "XXX: write me (and the code)\n"
    "\n"
    "Usage\n"
    "\n"
    "XXX: write me (and the code)\n"
    "\n"
    "The Saga of LZMA File Formats, and a couple cents.\n"
    "\n"
    "As far as I can tell, there are at least four different ways to put LZMA\n"
    "compressed data in a stream:\n"
    "\n"
    "1. The LZMA-Alone format, which consists of a 13 byte header including\n"
    "   compression properties, dictionary size, and the uncompressed size of\n"
    "   the file, followed by compressed data.  This format has some support\n"
    "   in Igor Pavlov's reference implementation and is in widespread use, "
    "as\n"
    "   it's supported by lzmautils: http://tukaani.org/lzma/\n"
    "\n"
    "   The canonical (afaict) implementation of this format (lzmautis) is\n"
    "   BSD licensed.\n"
    "\n"
    "2. The lzip format (http://www.nongnu.org/lzip/lzip.html) - which\n"
    "   includes a CRC footer and leading \"magic number\".  The former\n"
    "   affords data integrity gaurantees, while the latter simplifies\n"
    "   heuristic determination of file format.  This format looks to have\n"
    "   reasonably widespread usage, though not quite as significant as\n"
    "   LZMA-Alone.\n"
    "\n"
    "   The only implementation of this format I can find (lzip) is GPL "
    "licensed.\n"
    "\n"
    "3. the xz format ( http://tukaani.org/xz/xz-file-format.txt ) which is\n"
    "   a more complex representation that includes CRC support and a magic\n"
    "   number.  This format is to be supported by the next iteration of\n"
    "   XZ Utils which is currently in beta.  The source may be obtained\n"
    "   here: git://ctrl.tukaani.org/xz.git\n"
    "\n"
    "   This format will address some criticisms to the LZMA-Alone format and\n"
    "   was developed collaboratively by Lasse Collin (the current maintainer\n"
    "   of XZ utils) and Igor Pavlov (the author of 7zip and the refrence\n"
    "   implementation of LZMA).\n"
    "\n"
    "   The xz format will employ LZMA2 which consists of extensions on top\n"
    "   of LZMA, in the xz utils maintainer's words:\n"
    "\n"
    "   \"The primary compression algorithm in .xz is currently LZMA2, which\n"
    "    is an extension on top of the orignal LZMA to fix a few practical\n"
    "    issues, like adding support for flushing the encoder (equivalent\n"
    "    to zlib's Z_SYNC_FLUSH), which isn't possible with the original\n"
    "    LZMA.\"\n"
    "\n"
    "   Again, maintainers words, regarding licensing:\n"
    "\n"
    "   \"XZ Utils currently contains a zlib-like compression library and a \n"
    "    gzip-like command line tool. It's currently under LGPLv2.1+ but I "
    "will \n"
    "    put it into the public domain before the first stable release.\"\n"
    "\n"
    "4. The 7zip disk format which can contain multiple files possibly stored "
    "in\n"
    "   LZMA compressed format.\n"
    "\n"
    "Given the state of things, the goal of this project is to develop "
    "something\n"
    "based on the existing formats, and quickly leverage code generated by the "
    "XZ\n"
    "Utils project, or simply kill this thing if that project produces "
    "something\n"
    "that's easy to embed and has a clean API at a similar level of "
    "abstraction\n"
    "as easylzma.\n"
    "\n"
    "lloyd - sometime in oh nine.\n";

/* a test that we can round trip compress/decompress data using LZMA or LZIP
 * formats */
static int roundTripTest(elzma_file_format format)
{
    int rc;
    unsigned char *compressed;
    unsigned char *decompressed;
    size_t sz;

    rc = simpleCompress(format, (unsigned char *)sampleData, strlen(sampleData),
                        &compressed, &sz);

    if (rc != ELZMA_E_OK)
        return rc;

    /* gross assurance that compression is actually compressing */
    if (sz > strlen(sampleData)) {
        free(compressed);
        return 1;
    }

    rc = simpleDecompress(format, compressed, sz, &decompressed, &sz);

    free(compressed);

    if (rc != ELZMA_E_OK)
        return rc;

    if (sz != strlen(sampleData) || 0 != memcmp(decompressed, sampleData, sz)) {
        free(decompressed);
        return 1;
    }

    return ELZMA_E_OK;
}

/* "correct" lzip generated from the lzip program */
/*|LZIP...3.?..????|*/
/*|....?e2~........|*/
static unsigned char correctLzip[] = {
    0x4c, 0x5a, 0x49, 0x50, 0x01, 0x0c, 0x00, 0x33, 0x1b, 0xec,
    0x15, 0x07, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00,
    0xa8, 0x65, 0x32, 0x7e, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

/* "correct" lzip generated from lzma utils */
static unsigned char correctLzma[] = {
    0x5d, 0x00, 0x00, 0x80, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x33, 0x1b, 0xec, 0x14, 0x00, 0x00, 0x00};

/* lzip with a bad CRC */
static unsigned char corruptCRC[] = {
    0x4c, 0x5a, 0x49, 0x50, 0x01, 0x0c, 0x00, 0x33, 0x1b, 0xec,
    0x15, 0x07, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00,
    0xa8, 0x65, 0x31, 0x7e, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

/* lzip with a bad uncompressed size */
static unsigned char corruptSize[] = {
    0x4c, 0x5a, 0x49, 0x50, 0x01, 0x0c, 0x00, 0x33, 0x1b, 0xec,
    0x15, 0x07, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00,
    0xa8, 0x65, 0x32, 0x7e, 0x04, 0x01, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

/* lzma with a bad uncompressed size */
static unsigned char corruptSizeLzma[] = {
    0x5d, 0x00, 0x00, 0x80, 0x00, 0x04, 0x01, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x33, 0x1b, 0xec, 0x14, 0x00, 0x00, 0x00};

/* lzma with a bad uncompressed size */
static unsigned char corruptSizeLzma2[] = {
    0x5d, 0x00, 0x00, 0x80, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x33, 0x1b, 0xec, 0x14, 0x00, 0x00, 0x00};

/* tests */
static struct
{
    const char *testName; /* the name of the test */
    int expectedCode;     /* the expected output of the test */
    elzma_file_format format;
    unsigned char *data; /* input data */
    unsigned int dataSize;
} tests[] = {
    {"correct lzip", ELZMA_E_OK, ELZMA_lzip, correctLzip, sizeof(correctLzip)},
    {"lzip as lzma", ELZMA_E_DECOMPRESS_ERROR, ELZMA_lzma, correctLzip,
     sizeof(correctLzip)},
    {"correct lzma", ELZMA_E_OK, ELZMA_lzma, correctLzma, sizeof(correctLzma)},
    {"lzma as lzip", ELZMA_E_CORRUPT_HEADER, ELZMA_lzip, correctLzma,
     sizeof(correctLzma)},
    {"corrupt crc", ELZMA_E_CRC32_MISMATCH, ELZMA_lzip, corruptCRC,
     sizeof(corruptCRC)},
    {"bad lzip size", ELZMA_E_SIZE_MISMATCH, ELZMA_lzip, corruptSize,
     sizeof(corruptSize)},
    {"bad lzma size", ELZMA_E_INSUFFICIENT_INPUT, ELZMA_lzma, corruptSizeLzma,
     sizeof(corruptSizeLzma)},
    {"bad lzma size 2", ELZMA_E_SIZE_MISMATCH, ELZMA_lzma, corruptSizeLzma2,
     sizeof(corruptSizeLzma2)}};

int main(void)
{
    unsigned int i;
    unsigned int testsPassed = 0;
    unsigned int testsRun = 0;

    int rc = 0;

    printf("round trip lzma test:    ");
    fflush(stdout);
    testsRun++;
    if (ELZMA_E_OK != (rc = roundTripTest(ELZMA_lzma))) {
        printf("fail! (%d)\n", rc);
    } else {
        testsPassed++;
        printf("ok\n");
    }

    printf("round trip lzip test:    ");
    fflush(stdout);
    testsRun++;
    if (ELZMA_E_OK != (rc = roundTripTest(ELZMA_lzip))) {
        printf("fail (%d)!\n", rc);
    } else {
        testsPassed++;
        printf("ok\n");
    }

    /* now run through the tests table */
    for (i = 0; i < sizeof(tests) / sizeof(tests[0]); i++) {
        unsigned char *decompressed = NULL;
        size_t sz = 0;

        printf("%s test:    ", tests[i].testName);
        rc = simpleDecompress(tests[i].format, tests[i].data, tests[i].dataSize,
                              &decompressed, &sz);

        testsRun++;
        if (rc != tests[i].expectedCode) {
            printf("fail - got %d - expected %d\n", rc, tests[i].expectedCode);
        } else {
            testsPassed++;
            printf("ok\n");
            free(decompressed);
        }
    }

    printf("\n%d/%d tests passed\n", testsPassed, testsRun);

    return (testsPassed == testsRun) ? 0 : 1;
}
