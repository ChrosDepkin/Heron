/*******************************************************************************
 * Size: 28 px
 * Bpp: 4
 * Opts: 
 ******************************************************************************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

#ifndef NOVAMONO_28
#define NOVAMONO_28 1
#endif

#if NOVAMONO_28

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0020 " " */

    /* U+0021 "!" */
    0x6f, 0x66, 0xf6, 0x6f, 0x66, 0xf6, 0x6f, 0x66,
    0xf6, 0x6f, 0x66, 0xf6, 0x6f, 0x60, 0x0, 0x0,
    0x4, 0xc4, 0x6f, 0x60,

    /* U+002E "." */
    0x4c, 0x46, 0xf6,

    /* U+0030 "0" */
    0xb, 0xff, 0xfb, 0xb, 0xfe, 0xce, 0xfb, 0xfe,
    0x10, 0x1e, 0xff, 0xc0, 0x0, 0xcf, 0xfc, 0x0,
    0xc, 0xff, 0xc0, 0x0, 0xcf, 0xfc, 0x6f, 0x6c,
    0xff, 0xc4, 0xc4, 0xcf, 0xfc, 0x0, 0xc, 0xff,
    0xc0, 0x0, 0xcf, 0xfe, 0x10, 0x1e, 0xfb, 0xfe,
    0xce, 0xfb, 0xb, 0xff, 0xfb, 0x0,

    /* U+0031 "1" */
    0x0, 0xcf, 0x60, 0x0, 0xcf, 0xf6, 0x0, 0x2f,
    0xff, 0x60, 0x2, 0xd7, 0xf6, 0x0, 0x12, 0x6f,
    0x60, 0x0, 0x6, 0xf6, 0x0, 0x0, 0x6f, 0x60,
    0x0, 0x6, 0xf6, 0x0, 0x0, 0x6f, 0x60, 0x0,
    0x6, 0xf6, 0x0, 0x0, 0x6f, 0x60, 0xc, 0xcd,
    0xfd, 0xcc, 0xff, 0xff, 0xff, 0xf0,

    /* U+0032 "2" */
    0xb, 0xff, 0xfb, 0xb, 0xfe, 0xce, 0xfb, 0xfe,
    0x10, 0x1e, 0xfa, 0x80, 0x0, 0xcf, 0x0, 0x0,
    0xd, 0xf0, 0x0, 0x9, 0xfe, 0x0, 0x8, 0xfe,
    0x20, 0x6, 0xff, 0x30, 0x5, 0xff, 0x40, 0x4,
    0xff, 0x50, 0x0, 0xff, 0x60, 0x0, 0xf, 0xfc,
    0xcc, 0xcc, 0xff, 0xff, 0xff, 0xf0,

    /* U+0033 "3" */
    0xb, 0xff, 0xfb, 0xb, 0xfe, 0xce, 0xfb, 0xfe,
    0x10, 0x1e, 0xfe, 0xa0, 0x0, 0xcf, 0x0, 0x0,
    0xc, 0xf0, 0x0, 0x3, 0xff, 0x0, 0x6f, 0xff,
    0x80, 0x4, 0xce, 0xfb, 0x0, 0x0, 0x1e, 0xf8,
    0x60, 0x0, 0xcf, 0xfe, 0x10, 0x1e, 0xfb, 0xfe,
    0xce, 0xfb, 0xb, 0xff, 0xfb, 0x0,

    /* U+0034 "4" */
    0x0, 0xb, 0xfb, 0x0, 0x2, 0xff, 0xb0, 0x0,
    0x9f, 0xfb, 0x0, 0x1f, 0xcf, 0xb0, 0x7, 0xf5,
    0xfb, 0x0, 0xed, 0x1f, 0xb0, 0x5f, 0x61, 0xfb,
    0xc, 0xe0, 0x1f, 0xb0, 0xff, 0xff, 0xff, 0xfd,
    0xdd, 0xdf, 0xfd, 0x0, 0x1, 0xfb, 0x0, 0x0,
    0x1f, 0xb0, 0x0, 0x1, 0xfb, 0x0,

    /* U+0035 "5" */
    0xff, 0xff, 0xff, 0xff, 0xfc, 0xcc, 0xcc, 0xfc,
    0x0, 0x0, 0xf, 0xc0, 0x0, 0x0, 0xfc, 0x0,
    0x0, 0xf, 0xcc, 0xff, 0xb0, 0xff, 0xec, 0xef,
    0xba, 0xa1, 0x1, 0xef, 0x0, 0x0, 0xc, 0xf9,
    0x60, 0x0, 0xcf, 0xfe, 0x10, 0x1e, 0xfb, 0xfe,
    0xce, 0xfb, 0xb, 0xff, 0xfb, 0x0,

    /* U+0036 "6" */
    0xb, 0xff, 0xfb, 0xb, 0xfe, 0xce, 0xfb, 0xfe,
    0x20, 0x2e, 0xff, 0xc0, 0x0, 0x46, 0xfc, 0x0,
    0x0, 0xf, 0xcc, 0xff, 0xb0, 0xff, 0xec, 0xef,
    0xbf, 0xe1, 0x1, 0xef, 0xfc, 0x0, 0xc, 0xff,
    0xc0, 0x0, 0xcf, 0xfe, 0x10, 0x1e, 0xfb, 0xfe,
    0xce, 0xfb, 0xb, 0xff, 0xfb, 0x0,

    /* U+0037 "7" */
    0xff, 0xff, 0xff, 0xfc, 0xcc, 0xcc, 0xff, 0x0,
    0x0, 0x1f, 0xa0, 0x0, 0x8, 0xf3, 0x0, 0x1,
    0xfb, 0x0, 0x0, 0x7f, 0x40, 0x0, 0xe, 0xc0,
    0x0, 0x5, 0xf6, 0x0, 0x0, 0x6f, 0x60, 0x0,
    0x6, 0xf6, 0x0, 0x0, 0x6f, 0x60, 0x0, 0x6,
    0xf6, 0x0, 0x0, 0x6f, 0x60, 0x0,

    /* U+0038 "8" */
    0xb, 0xff, 0xfb, 0xb, 0xfe, 0xce, 0xfb, 0xfe,
    0x10, 0x1e, 0xff, 0xf7, 0x7, 0xff, 0x4f, 0xfb,
    0xff, 0x40, 0x5f, 0xff, 0x50, 0x4, 0xff, 0xf4,
    0x3, 0xff, 0xbf, 0xf3, 0xef, 0x80, 0x8f, 0xef,
    0xc0, 0x0, 0xcf, 0xfe, 0x10, 0x1e, 0xfb, 0xfe,
    0xce, 0xfb, 0xb, 0xff, 0xfb, 0x0,

    /* U+0039 "9" */
    0xb, 0xff, 0xfb, 0xb, 0xfe, 0xce, 0xfb, 0xfe,
    0x10, 0x1e, 0xff, 0xc0, 0x0, 0xcf, 0xfc, 0x0,
    0xc, 0xff, 0xc0, 0x0, 0xcf, 0xff, 0x30, 0x3f,
    0xf8, 0xff, 0xff, 0xff, 0x8, 0xcc, 0x9c, 0xf0,
    0x0, 0x0, 0xcf, 0xfd, 0x10, 0x1d, 0xfb, 0xfe,
    0xce, 0xfb, 0xb, 0xff, 0xfb, 0x0,

    /* U+003F "?" */
    0xb, 0xff, 0xfb, 0xb, 0xfe, 0xce, 0xfb, 0xfe,
    0x10, 0x1e, 0xfa, 0x80, 0x0, 0xcf, 0x0, 0x0,
    0x8f, 0xe0, 0x0, 0x7f, 0xf3, 0x0, 0x4f, 0xf4,
    0x0, 0x6, 0xf7, 0x0, 0x0, 0x6f, 0x60, 0x0,
    0x1, 0x51, 0x0, 0x0, 0x0, 0x0, 0x0, 0x4,
    0xc4, 0x0, 0x0, 0x6f, 0x60, 0x0,

    /* U+0041 "A" */
    0xb, 0xff, 0xfb, 0xb, 0xfe, 0xce, 0xfb, 0xfe,
    0x10, 0x1e, 0xff, 0xc0, 0x0, 0xcf, 0xfc, 0x0,
    0xc, 0xff, 0xc0, 0x0, 0xcf, 0xff, 0xff, 0xff,
    0xff, 0xfc, 0xcc, 0xff, 0xfc, 0x0, 0xc, 0xff,
    0xc0, 0x0, 0xcf, 0xfc, 0x0, 0xc, 0xff, 0xc0,
    0x0, 0xcf, 0xfc, 0x0, 0xc, 0xf0,

    /* U+0042 "B" */
    0xff, 0xff, 0xfb, 0xf, 0xfc, 0xce, 0xfb, 0xfc,
    0x0, 0x1e, 0xff, 0xc0, 0x0, 0xcf, 0xfc, 0x0,
    0xc, 0xff, 0xc0, 0x3, 0xff, 0xff, 0xff, 0xff,
    0x8f, 0xfc, 0xce, 0xfb, 0xfc, 0x0, 0x1e, 0xff,
    0xc0, 0x0, 0xcf, 0xfc, 0x0, 0x1e, 0xff, 0xfc,
    0xce, 0xfb, 0xff, 0xff, 0xfb, 0x0,

    /* U+0043 "C" */
    0xb, 0xff, 0xfb, 0xb, 0xfe, 0xce, 0xfb, 0xfe,
    0x10, 0x1e, 0xff, 0xc0, 0x0, 0x8a, 0xfc, 0x0,
    0x0, 0xf, 0xc0, 0x0, 0x0, 0xfc, 0x0, 0x0,
    0xf, 0xc0, 0x0, 0x0, 0xfc, 0x0, 0x0, 0xf,
    0xc0, 0x0, 0x8a, 0xfe, 0x10, 0x1e, 0xfb, 0xfe,
    0xce, 0xfb, 0xb, 0xff, 0xfb, 0x0,

    /* U+0044 "D" */
    0xff, 0xff, 0xc0, 0xf, 0xfc, 0xef, 0xc0, 0xfc,
    0x0, 0xcf, 0xbf, 0xc0, 0x1, 0xef, 0xfc, 0x0,
    0xc, 0xff, 0xc0, 0x0, 0xcf, 0xfc, 0x0, 0xc,
    0xff, 0xc0, 0x0, 0xcf, 0xfc, 0x0, 0xc, 0xff,
    0xc0, 0x1, 0xef, 0xfc, 0x0, 0xcf, 0xbf, 0xfc,
    0xef, 0xc0, 0xff, 0xff, 0xc0, 0x0,

    /* U+0045 "E" */
    0xff, 0xff, 0xff, 0xff, 0xfc, 0xcc, 0xcc, 0xfc,
    0x0, 0x0, 0xf, 0xc0, 0x0, 0x0, 0xfc, 0x0,
    0x0, 0xf, 0xc0, 0x0, 0x0, 0xff, 0xff, 0xf4,
    0xf, 0xfc, 0xcc, 0x30, 0xfc, 0x0, 0x0, 0xf,
    0xc0, 0x0, 0x0, 0xfc, 0x0, 0x0, 0xf, 0xfc,
    0xcc, 0xcc, 0xff, 0xff, 0xff, 0xf0,

    /* U+0046 "F" */
    0xff, 0xff, 0xff, 0xff, 0xfc, 0xcc, 0xcc, 0xfc,
    0x0, 0x0, 0xf, 0xc0, 0x0, 0x0, 0xfc, 0x0,
    0x0, 0xf, 0xc0, 0x0, 0x0, 0xff, 0xff, 0xf4,
    0xf, 0xfc, 0xcc, 0x30, 0xfc, 0x0, 0x0, 0xf,
    0xc0, 0x0, 0x0, 0xfc, 0x0, 0x0, 0xf, 0xc0,
    0x0, 0x0, 0xfc, 0x0, 0x0, 0x0,

    /* U+0047 "G" */
    0xb, 0xff, 0xfb, 0xb, 0xfe, 0xce, 0xfb, 0xfe,
    0x10, 0x1e, 0xff, 0xc0, 0x0, 0xae, 0xfc, 0x0,
    0x0, 0xf, 0xc0, 0x0, 0x0, 0xfc, 0x8, 0xff,
    0xff, 0xc0, 0x6c, 0xff, 0xfc, 0x0, 0xc, 0xff,
    0xc0, 0x0, 0xcf, 0xfe, 0x10, 0x1e, 0xfb, 0xfe,
    0xce, 0xfb, 0xb, 0xff, 0xfb, 0x0,

    /* U+0048 "H" */
    0xfc, 0x0, 0xc, 0xff, 0xc0, 0x0, 0xcf, 0xfc,
    0x0, 0xc, 0xff, 0xc0, 0x0, 0xcf, 0xfc, 0x0,
    0xc, 0xff, 0xc0, 0x0, 0xcf, 0xff, 0xff, 0xff,
    0xff, 0xfc, 0xcc, 0xff, 0xfc, 0x0, 0xc, 0xff,
    0xc0, 0x0, 0xcf, 0xfc, 0x0, 0xc, 0xff, 0xc0,
    0x0, 0xcf, 0xfc, 0x0, 0xc, 0xf0,

    /* U+0049 "I" */
    0xff, 0xff, 0xff, 0xfc, 0xcd, 0xfd, 0xcc, 0x0,
    0x6f, 0x60, 0x0, 0x6, 0xf6, 0x0, 0x0, 0x6f,
    0x60, 0x0, 0x6, 0xf6, 0x0, 0x0, 0x6f, 0x60,
    0x0, 0x6, 0xf6, 0x0, 0x0, 0x6f, 0x60, 0x0,
    0x6, 0xf6, 0x0, 0x0, 0x6f, 0x60, 0xc, 0xcd,
    0xfd, 0xcc, 0xff, 0xff, 0xff, 0xf0,

    /* U+004A "J" */
    0x0, 0x8, 0xff, 0xf0, 0x0, 0x6c, 0xff, 0x0,
    0x0, 0xc, 0xf0, 0x0, 0x0, 0xcf, 0x0, 0x0,
    0xc, 0xf0, 0x0, 0x0, 0xcf, 0x0, 0x0, 0xc,
    0xf0, 0x0, 0x0, 0xcf, 0x0, 0x0, 0xc, 0xfa,
    0x80, 0x0, 0xcf, 0xfe, 0x10, 0x1e, 0xfb, 0xfe,
    0xce, 0xfb, 0xb, 0xff, 0xfb, 0x0,

    /* U+004B "K" */
    0xfc, 0x0, 0xc, 0xff, 0xc0, 0x0, 0xcf, 0xfc,
    0x0, 0xc, 0xff, 0xc0, 0x0, 0xcf, 0xfc, 0x0,
    0x6f, 0xff, 0xc5, 0xdf, 0xe6, 0xff, 0xff, 0xc0,
    0xf, 0xf9, 0xdf, 0xe5, 0xfc, 0x0, 0x6f, 0xff,
    0xc0, 0x0, 0xcf, 0xfc, 0x0, 0xc, 0xff, 0xc0,
    0x0, 0xcf, 0xfc, 0x0, 0xc, 0xf0,

    /* U+004C "L" */
    0xfc, 0x0, 0x0, 0xf, 0xc0, 0x0, 0x0, 0xfc,
    0x0, 0x0, 0xf, 0xc0, 0x0, 0x0, 0xfc, 0x0,
    0x0, 0xf, 0xc0, 0x0, 0x0, 0xfc, 0x0, 0x0,
    0xf, 0xc0, 0x0, 0x0, 0xfc, 0x0, 0x0, 0xf,
    0xc0, 0x0, 0x0, 0xfc, 0x0, 0x0, 0xf, 0xfc,
    0xcc, 0xcc, 0xff, 0xff, 0xff, 0xf0,

    /* U+004D "M" */
    0xfe, 0x0, 0xe, 0xff, 0xf7, 0x7, 0xff, 0xff,
    0xe0, 0xef, 0xff, 0xff, 0xcf, 0xff, 0xfc, 0xdf,
    0xdc, 0xff, 0xc6, 0xf6, 0xcf, 0xfc, 0xc, 0xc,
    0xff, 0xc0, 0x10, 0xcf, 0xfc, 0x0, 0xc, 0xff,
    0xc0, 0x0, 0xcf, 0xfc, 0x0, 0xc, 0xff, 0xc0,
    0x0, 0xcf, 0xfc, 0x0, 0xc, 0xf0,

    /* U+004E "N" */
    0xf8, 0x0, 0xc, 0xff, 0xf1, 0x0, 0xcf, 0xff,
    0x70, 0xc, 0xff, 0xfe, 0x0, 0xcf, 0xff, 0xf6,
    0xc, 0xff, 0xcd, 0xe0, 0xcf, 0xfc, 0x6f, 0x5c,
    0xff, 0xc0, 0xed, 0xcf, 0xfc, 0x6, 0xff, 0xff,
    0xc0, 0xe, 0xff, 0xfc, 0x0, 0x7f, 0xff, 0xc0,
    0x1, 0xff, 0xfc, 0x0, 0x8, 0xf0,

    /* U+004F "O" */
    0xb, 0xff, 0xfb, 0xb, 0xfe, 0xce, 0xfb, 0xfe,
    0x10, 0x1e, 0xff, 0xc0, 0x0, 0xcf, 0xfc, 0x0,
    0xc, 0xff, 0xc0, 0x0, 0xcf, 0xfc, 0x0, 0xc,
    0xff, 0xc0, 0x0, 0xcf, 0xfc, 0x0, 0xc, 0xff,
    0xc0, 0x0, 0xcf, 0xfe, 0x10, 0x1e, 0xfb, 0xfe,
    0xce, 0xfb, 0xb, 0xff, 0xfb, 0x0,

    /* U+0050 "P" */
    0xff, 0xff, 0xfb, 0xf, 0xfc, 0xce, 0xfb, 0xfc,
    0x0, 0x1e, 0xff, 0xc0, 0x0, 0xcf, 0xfc, 0x0,
    0xc, 0xff, 0xc0, 0x3, 0xff, 0xff, 0xff, 0xff,
    0x8f, 0xfc, 0xcc, 0x80, 0xfc, 0x0, 0x0, 0xf,
    0xc0, 0x0, 0x0, 0xfc, 0x0, 0x0, 0xf, 0xc0,
    0x0, 0x0, 0xfc, 0x0, 0x0, 0x0,

    /* U+0051 "Q" */
    0xb, 0xff, 0xfb, 0xb, 0xfe, 0xce, 0xfb, 0xfe,
    0x10, 0x1e, 0xff, 0xc0, 0x0, 0xcf, 0xfc, 0x0,
    0xc, 0xff, 0xc0, 0x0, 0xcf, 0xfc, 0x0, 0xc,
    0xff, 0xc0, 0x0, 0xcf, 0xfc, 0x0, 0xc, 0xff,
    0xc0, 0x0, 0xcf, 0xfe, 0x20, 0x2e, 0xfa, 0xff,
    0xdf, 0xfa, 0xb, 0xff, 0xfb, 0x0, 0x6, 0xf7,
    0x0, 0x0, 0x5f, 0xff, 0x20, 0x0, 0xac, 0xc1,

    /* U+0052 "R" */
    0xff, 0xff, 0xfb, 0xf, 0xfc, 0xce, 0xfb, 0xfc,
    0x0, 0x1e, 0xff, 0xc0, 0x0, 0xcf, 0xfc, 0x0,
    0xc, 0xff, 0xc0, 0x3, 0xff, 0xff, 0xff, 0xff,
    0x8f, 0xfc, 0xef, 0x90, 0xfc, 0x2, 0xf9, 0xf,
    0xc0, 0xa, 0xf2, 0xfc, 0x0, 0x2f, 0x9f, 0xc0,
    0x0, 0xcf, 0xfc, 0x0, 0xc, 0xf0,

    /* U+0053 "S" */
    0xb, 0xff, 0xfb, 0xb, 0xfe, 0xce, 0xfb, 0xfe,
    0x10, 0x1e, 0xff, 0xc0, 0x0, 0xae, 0xfc, 0x0,
    0x0, 0xf, 0xf3, 0x0, 0x0, 0x8f, 0xff, 0xfb,
    0x0, 0x8c, 0xce, 0xfb, 0x0, 0x0, 0x1e, 0xf8,
    0x60, 0x0, 0xcf, 0xfe, 0x10, 0x1e, 0xfb, 0xfe,
    0xce, 0xfb, 0xb, 0xff, 0xfb, 0x0,

    /* U+0054 "T" */
    0xff, 0xff, 0xff, 0xfc, 0xcd, 0xfd, 0xcc, 0x0,
    0x6f, 0x60, 0x0, 0x6, 0xf6, 0x0, 0x0, 0x6f,
    0x60, 0x0, 0x6, 0xf6, 0x0, 0x0, 0x6f, 0x60,
    0x0, 0x6, 0xf6, 0x0, 0x0, 0x6f, 0x60, 0x0,
    0x6, 0xf6, 0x0, 0x0, 0x6f, 0x60, 0x0, 0x6,
    0xf6, 0x0, 0x0, 0x6f, 0x60, 0x0,

    /* U+0055 "U" */
    0xfc, 0x0, 0xc, 0xff, 0xc0, 0x0, 0xcf, 0xfc,
    0x0, 0xc, 0xff, 0xc0, 0x0, 0xcf, 0xfc, 0x0,
    0xc, 0xff, 0xc0, 0x0, 0xcf, 0xfc, 0x0, 0xc,
    0xff, 0xc0, 0x0, 0xcf, 0xfc, 0x0, 0xc, 0xff,
    0xc0, 0x0, 0xcf, 0xfe, 0x10, 0x1e, 0xfb, 0xfe,
    0xce, 0xfb, 0xb, 0xff, 0xfb, 0x0,

    /* U+0056 "V" */
    0xfc, 0x0, 0xc, 0xff, 0xc0, 0x0, 0xcf, 0xfc,
    0x0, 0xc, 0xff, 0xc0, 0x0, 0xcf, 0xfc, 0x0,
    0xc, 0xff, 0xc0, 0x0, 0xcf, 0xfc, 0x0, 0xc,
    0xfe, 0xd0, 0x0, 0xde, 0x7f, 0x40, 0x4f, 0x71,
    0xfb, 0xb, 0xf1, 0x8, 0xf6, 0xf8, 0x0, 0x1f,
    0xff, 0x10, 0x0, 0x9f, 0x90, 0x0,

    /* U+0057 "W" */
    0xfc, 0x0, 0xc, 0xff, 0xc0, 0x0, 0xcf, 0xfc,
    0x0, 0xc, 0xff, 0xc0, 0x0, 0xcf, 0xfc, 0x0,
    0xc, 0xff, 0xc0, 0x10, 0xcf, 0xfc, 0xc, 0xc,
    0xff, 0xc6, 0xf6, 0xcf, 0xfc, 0xdf, 0xdc, 0xff,
    0xff, 0xcf, 0xff, 0xff, 0xe0, 0xef, 0xff, 0xf7,
    0x7, 0xff, 0xfe, 0x0, 0xe, 0xf0,

    /* U+0058 "X" */
    0xfc, 0x0, 0xc, 0xfb, 0xf1, 0x1, 0xfb, 0x4f,
    0x70, 0x7f, 0x40, 0xce, 0x1e, 0xc0, 0x5, 0xfc,
    0xf5, 0x0, 0xd, 0xfd, 0x0, 0x0, 0x7f, 0x70,
    0x0, 0xd, 0xfd, 0x0, 0x5, 0xfc, 0xf5, 0x0,
    0xce, 0x1e, 0xc0, 0x4f, 0x70, 0x7f, 0x4b, 0xf1,
    0x1, 0xfb, 0xfc, 0x0, 0xc, 0xf0,

    /* U+0059 "Y" */
    0xfc, 0x0, 0xc, 0xfb, 0xf1, 0x1, 0xfb, 0x4f,
    0x70, 0x7f, 0x40, 0xce, 0x1e, 0xc0, 0x5, 0xfc,
    0xf5, 0x0, 0xd, 0xfd, 0x0, 0x0, 0x6f, 0x60,
    0x0, 0x6, 0xf6, 0x0, 0x0, 0x6f, 0x60, 0x0,
    0x6, 0xf6, 0x0, 0x0, 0x6f, 0x60, 0x0, 0x6,
    0xf6, 0x0, 0x0, 0x6f, 0x60, 0x0,

    /* U+005A "Z" */
    0xff, 0xff, 0xff, 0xfc, 0xcc, 0xcd, 0xfb, 0x0,
    0x0, 0x8f, 0x30, 0x0, 0xe, 0xc0, 0x0, 0x7,
    0xf5, 0x0, 0x0, 0xed, 0x0, 0x0, 0x6f, 0x60,
    0x0, 0xd, 0xe0, 0x0, 0x5, 0xf7, 0x0, 0x0,
    0xce, 0x0, 0x0, 0x4f, 0x80, 0x0, 0xb, 0xfd,
    0xcc, 0xcc, 0xff, 0xff, 0xff, 0xf0
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 126, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 0, .adv_w = 126, .box_w = 3, .box_h = 13, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 20, .adv_w = 126, .box_w = 3, .box_h = 2, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 23, .adv_w = 126, .box_w = 7, .box_h = 13, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 69, .adv_w = 126, .box_w = 7, .box_h = 13, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 115, .adv_w = 126, .box_w = 7, .box_h = 13, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 161, .adv_w = 126, .box_w = 7, .box_h = 13, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 207, .adv_w = 126, .box_w = 7, .box_h = 13, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 253, .adv_w = 126, .box_w = 7, .box_h = 13, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 299, .adv_w = 126, .box_w = 7, .box_h = 13, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 345, .adv_w = 126, .box_w = 7, .box_h = 13, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 391, .adv_w = 126, .box_w = 7, .box_h = 13, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 437, .adv_w = 126, .box_w = 7, .box_h = 13, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 483, .adv_w = 126, .box_w = 7, .box_h = 13, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 529, .adv_w = 126, .box_w = 7, .box_h = 13, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 575, .adv_w = 126, .box_w = 7, .box_h = 13, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 621, .adv_w = 126, .box_w = 7, .box_h = 13, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 667, .adv_w = 126, .box_w = 7, .box_h = 13, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 713, .adv_w = 126, .box_w = 7, .box_h = 13, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 759, .adv_w = 126, .box_w = 7, .box_h = 13, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 805, .adv_w = 126, .box_w = 7, .box_h = 13, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 851, .adv_w = 126, .box_w = 7, .box_h = 13, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 897, .adv_w = 126, .box_w = 7, .box_h = 13, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 943, .adv_w = 126, .box_w = 7, .box_h = 13, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 989, .adv_w = 126, .box_w = 7, .box_h = 13, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1035, .adv_w = 126, .box_w = 7, .box_h = 13, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1081, .adv_w = 126, .box_w = 7, .box_h = 13, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1127, .adv_w = 126, .box_w = 7, .box_h = 13, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1173, .adv_w = 126, .box_w = 7, .box_h = 13, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1219, .adv_w = 126, .box_w = 7, .box_h = 13, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1265, .adv_w = 126, .box_w = 7, .box_h = 16, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 1321, .adv_w = 126, .box_w = 7, .box_h = 13, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1367, .adv_w = 126, .box_w = 7, .box_h = 13, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1413, .adv_w = 126, .box_w = 7, .box_h = 13, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1459, .adv_w = 126, .box_w = 7, .box_h = 13, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1505, .adv_w = 126, .box_w = 7, .box_h = 13, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1551, .adv_w = 126, .box_w = 7, .box_h = 13, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1597, .adv_w = 126, .box_w = 7, .box_h = 13, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1643, .adv_w = 126, .box_w = 7, .box_h = 13, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1689, .adv_w = 126, .box_w = 7, .box_h = 13, .ofs_x = 0, .ofs_y = 0}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/

static const uint16_t unicode_list_0[] = {
    0x0, 0x1, 0xe, 0x10, 0x11, 0x12, 0x13, 0x14,
    0x15, 0x16, 0x17, 0x18, 0x19, 0x1f
};

/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 32, .range_length = 32, .glyph_id_start = 1,
        .unicode_list = unicode_list_0, .glyph_id_ofs_list = NULL, .list_length = 14, .type = LV_FONT_FMT_TXT_CMAP_SPARSE_TINY
    },
    {
        .range_start = 65, .range_length = 26, .glyph_id_start = 15,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    }
};



/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LV_VERSION_CHECK(8, 0, 0)
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = NULL,
    .kern_scale = 0,
    .cmap_num = 2,
    .bpp = 4,
    .kern_classes = 0,
    .bitmap_format = 0,
#if LV_VERSION_CHECK(8, 0, 0)
    .cache = &cache
#endif
};


/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LV_VERSION_CHECK(8, 0, 0)
const lv_font_t novamono_28 = {
#else
lv_font_t novamono_28 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 16,          /*The maximum line height required by the font*/
    .base_line = 3,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0)
    .underline_position = 3,
    .underline_thickness = 1,
#endif
    .dsc = &font_dsc           /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
};



#endif /*#if NOVAMONO_28*/
