/*******************************************************************************
 * Size: 44 px
 * Bpp: 1
 * Opts: 
 ******************************************************************************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

#ifndef NOVAMONO_44
#define NOVAMONO_44 1
#endif

#if NOVAMONO_44

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0020 " " */
    0x0,

    /* U+0021 "!" */
    0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x1f, 0xf0,

    /* U+002B "+" */
    0xe, 0x1, 0xc0, 0x38, 0x7, 0xf, 0xff, 0xff,
    0xff, 0xf8, 0x70, 0xe, 0x1, 0xc0, 0x38, 0x0,

    /* U+002C "," */
    0xff, 0xb5, 0xa0,

    /* U+002D "-" */
    0xff, 0xff, 0xff, 0xff, 0x80,

    /* U+002E "." */
    0xff, 0x80,

    /* U+0030 "0" */
    0x3f, 0x8f, 0xfb, 0xff, 0xf8, 0xfe, 0xf, 0xc1,
    0xf8, 0x3f, 0x7, 0xe0, 0xfd, 0xdf, 0xbb, 0xf7,
    0x7e, 0xf, 0xc1, 0xf8, 0x3f, 0x7, 0xf1, 0xff,
    0xfd, 0xff, 0x1f, 0xc0,

    /* U+0031 "1" */
    0xe, 0x3, 0xc0, 0xf8, 0x3f, 0x7, 0xe0, 0xdc,
    0x13, 0x80, 0x70, 0xe, 0x1, 0xc0, 0x38, 0x7,
    0x0, 0xe0, 0x1c, 0x3, 0x80, 0x70, 0xe, 0x1f,
    0xff, 0xff, 0xff, 0xf0,

    /* U+0032 "2" */
    0x3f, 0x8f, 0xfb, 0xff, 0xf8, 0xfe, 0xf, 0xc1,
    0xc0, 0x38, 0x7, 0x1, 0xe0, 0x78, 0x1e, 0x7,
    0x81, 0xe0, 0x78, 0x1e, 0x7, 0x80, 0xe0, 0x1f,
    0xff, 0xff, 0xff, 0xf0,

    /* U+0033 "3" */
    0x3f, 0x8f, 0xfb, 0xff, 0xf8, 0xfe, 0xf, 0xc1,
    0xc0, 0x38, 0x7, 0x1, 0xe1, 0xfc, 0x3f, 0x7,
    0xf0, 0x1e, 0x1, 0xf8, 0x3f, 0x7, 0xf1, 0xff,
    0xfd, 0xff, 0x1f, 0xc0,

    /* U+0034 "4" */
    0x7, 0x80, 0xf0, 0x3e, 0x7, 0xc1, 0xf8, 0x3f,
    0xe, 0xe1, 0xdc, 0x33, 0x8e, 0x71, 0x8e, 0x71,
    0xcf, 0xff, 0xff, 0xff, 0xf8, 0x1c, 0x3, 0x80,
    0x70, 0xe, 0x1, 0xc0,

    /* U+0035 "5" */
    0xff, 0xff, 0xff, 0xff, 0xf0, 0xe, 0x1, 0xc0,
    0x38, 0x7, 0x7c, 0xff, 0xdf, 0xff, 0xc7, 0x80,
    0x70, 0xe, 0x1, 0xf8, 0x3f, 0x7, 0xf1, 0xff,
    0xfd, 0xff, 0x1f, 0xc0,

    /* U+0036 "6" */
    0x3f, 0x8f, 0xfb, 0xff, 0xf8, 0xfe, 0xf, 0xc0,
    0x38, 0x7, 0x7c, 0xff, 0xdf, 0xff, 0xc7, 0xf0,
    0x7e, 0xf, 0xc1, 0xf8, 0x3f, 0x7, 0xf1, 0xff,
    0xfd, 0xff, 0x1f, 0xc0,

    /* U+0037 "7" */
    0xff, 0xff, 0xff, 0xff, 0x80, 0x70, 0xc, 0x3,
    0x80, 0x60, 0x1c, 0x3, 0x0, 0xe0, 0x38, 0x7,
    0x0, 0xe0, 0x1c, 0x3, 0x80, 0x70, 0xe, 0x1,
    0xc0, 0x38, 0x7, 0x0,

    /* U+0038 "8" */
    0x3f, 0x8f, 0xfb, 0xff, 0xf8, 0xfe, 0xf, 0xe3,
    0xde, 0xf1, 0xfc, 0x1f, 0x3, 0xe0, 0xfe, 0x3d,
    0xef, 0xbf, 0xe3, 0xf8, 0x3f, 0x7, 0xf1, 0xff,
    0xfd, 0xff, 0x1f, 0xc0,

    /* U+0039 "9" */
    0x3f, 0x8f, 0xfb, 0xff, 0xf8, 0xfe, 0xf, 0xc1,
    0xf8, 0x3f, 0x7, 0xe0, 0xfe, 0x3f, 0xff, 0xbf,
    0xf3, 0xee, 0x1, 0xc0, 0x3f, 0x7, 0xf1, 0xff,
    0xfd, 0xff, 0x1f, 0xc0,

    /* U+003F "?" */
    0x3f, 0x8f, 0xfb, 0xff, 0xf8, 0xfe, 0xf, 0xc1,
    0xc0, 0x78, 0x1f, 0x7, 0xc1, 0xf0, 0x3c, 0x7,
    0x0, 0xe0, 0x1c, 0x0, 0x0, 0x0, 0x0, 0x1,
    0xc0, 0x38, 0x7, 0x0,

    /* U+0041 "A" */
    0x3f, 0x8f, 0xfb, 0xff, 0xf8, 0xfe, 0xf, 0xc1,
    0xf8, 0x3f, 0x7, 0xe0, 0xff, 0xff, 0xff, 0xff,
    0xfe, 0xf, 0xc1, 0xf8, 0x3f, 0x7, 0xe0, 0xfc,
    0x1f, 0x83, 0xf0, 0x70,

    /* U+0042 "B" */
    0xff, 0x9f, 0xfb, 0xff, 0xf0, 0xfe, 0xf, 0xc1,
    0xf8, 0x3f, 0x7, 0xe1, 0xff, 0xff, 0xff, 0x7f,
    0xfe, 0x1f, 0xc1, 0xf8, 0x3f, 0x7, 0xe1, 0xff,
    0xff, 0xff, 0x7f, 0xc0,

    /* U+0043 "C" */
    0x3f, 0x8f, 0xfb, 0xff, 0xf8, 0xfe, 0xf, 0xc1,
    0xf8, 0x7, 0x0, 0xe0, 0x1c, 0x3, 0x80, 0x70,
    0xe, 0x1, 0xc0, 0x38, 0x3f, 0x7, 0xf1, 0xff,
    0xfd, 0xff, 0x1f, 0xc0,

    /* U+0044 "D" */
    0xfe, 0x1f, 0xe3, 0xfe, 0x71, 0xee, 0x1f, 0xc1,
    0xf8, 0x3f, 0x7, 0xe0, 0xfc, 0x1f, 0x83, 0xf0,
    0x7e, 0xf, 0xc1, 0xf8, 0x3f, 0xf, 0xe3, 0xdf,
    0xf3, 0xfc, 0x7f, 0x0,

    /* U+0045 "E" */
    0xff, 0xff, 0xff, 0xff, 0xf0, 0xe, 0x1, 0xc0,
    0x38, 0x7, 0x0, 0xe0, 0x1f, 0xf3, 0xfe, 0x7f,
    0xce, 0x1, 0xc0, 0x38, 0x7, 0x0, 0xe0, 0x1f,
    0xff, 0xff, 0xff, 0xf0,

    /* U+0046 "F" */
    0xff, 0xff, 0xff, 0xff, 0xf0, 0xe, 0x1, 0xc0,
    0x38, 0x7, 0x0, 0xe0, 0x1f, 0xf3, 0xfe, 0x7f,
    0xce, 0x1, 0xc0, 0x38, 0x7, 0x0, 0xe0, 0x1c,
    0x3, 0x80, 0x70, 0x0,

    /* U+0047 "G" */
    0x3f, 0x8f, 0xfb, 0xff, 0xf8, 0xfe, 0xf, 0xc1,
    0xf8, 0x7, 0x0, 0xe0, 0x1c, 0xff, 0x9f, 0xf3,
    0xfe, 0xf, 0xc1, 0xf8, 0x3f, 0x7, 0xf1, 0xff,
    0xfd, 0xff, 0x1f, 0xc0,

    /* U+0048 "H" */
    0xe0, 0xfc, 0x1f, 0x83, 0xf0, 0x7e, 0xf, 0xc1,
    0xf8, 0x3f, 0x7, 0xe0, 0xff, 0xff, 0xff, 0xff,
    0xfe, 0xf, 0xc1, 0xf8, 0x3f, 0x7, 0xe0, 0xfc,
    0x1f, 0x83, 0xf0, 0x70,

    /* U+0049 "I" */
    0xff, 0xff, 0xff, 0xff, 0x87, 0x0, 0xe0, 0x1c,
    0x3, 0x80, 0x70, 0xe, 0x1, 0xc0, 0x38, 0x7,
    0x0, 0xe0, 0x1c, 0x3, 0x80, 0x70, 0xe, 0x1f,
    0xff, 0xff, 0xff, 0xf0,

    /* U+004A "J" */
    0x7, 0xe0, 0xfc, 0x1f, 0x80, 0x70, 0xe, 0x1,
    0xc0, 0x38, 0x7, 0x0, 0xe0, 0x1c, 0x3, 0x80,
    0x70, 0xe, 0x1, 0xf8, 0x3f, 0x7, 0xf1, 0xff,
    0xfd, 0xff, 0x1f, 0xc0,

    /* U+004B "K" */
    0xe0, 0xfc, 0x1f, 0x83, 0xf0, 0x7e, 0xf, 0xc1,
    0xf8, 0x3f, 0x1f, 0xef, 0xdf, 0xe3, 0xfc, 0x7f,
    0xee, 0x3f, 0xc1, 0xf8, 0x3f, 0x7, 0xe0, 0xfc,
    0x1f, 0x83, 0xf0, 0x70,

    /* U+004C "L" */
    0xe0, 0x1c, 0x3, 0x80, 0x70, 0xe, 0x1, 0xc0,
    0x38, 0x7, 0x0, 0xe0, 0x1c, 0x3, 0x80, 0x70,
    0xe, 0x1, 0xc0, 0x38, 0x7, 0x0, 0xe0, 0x1f,
    0xff, 0xff, 0xff, 0xf0,

    /* U+004D "M" */
    0xe0, 0xfe, 0x3f, 0xc7, 0xfd, 0xff, 0xbf, 0xf7,
    0xff, 0xff, 0x77, 0xee, 0xfc, 0x9f, 0x93, 0xf0,
    0x7e, 0xf, 0xc1, 0xf8, 0x3f, 0x7, 0xe0, 0xfc,
    0x1f, 0x83, 0xf0, 0x70,

    /* U+004E "N" */
    0xc0, 0xfc, 0x1f, 0x83, 0xf8, 0x7f, 0xf, 0xf1,
    0xfe, 0x3f, 0xe7, 0xec, 0xfd, 0xdf, 0xbb, 0xf3,
    0x7e, 0x7f, 0xc7, 0xf8, 0xff, 0xf, 0xe1, 0xfc,
    0x1f, 0x83, 0xf0, 0x30,

    /* U+004F "O" */
    0x3f, 0x8f, 0xfb, 0xff, 0xf8, 0xfe, 0xf, 0xc1,
    0xf8, 0x3f, 0x7, 0xe0, 0xfc, 0x1f, 0x83, 0xf0,
    0x7e, 0xf, 0xc1, 0xf8, 0x3f, 0x7, 0xf1, 0xff,
    0xfd, 0xff, 0x1f, 0xc0,

    /* U+0050 "P" */
    0xff, 0x9f, 0xfb, 0xff, 0xf0, 0xfe, 0xf, 0xc1,
    0xf8, 0x3f, 0x7, 0xe1, 0xff, 0xff, 0xff, 0x7f,
    0xce, 0x1, 0xc0, 0x38, 0x7, 0x0, 0xe0, 0x1c,
    0x3, 0x80, 0x70, 0x0,

    /* U+0051 "Q" */
    0x3f, 0x8f, 0xfb, 0xff, 0xf8, 0xfe, 0xf, 0xc1,
    0xf8, 0x3f, 0x7, 0xe0, 0xfc, 0x1f, 0x83, 0xf0,
    0x7e, 0xf, 0xc1, 0xf8, 0x3f, 0x7, 0xf1, 0xff,
    0xfd, 0xff, 0x1f, 0xc0, 0xe0, 0x1f, 0x83, 0xf0,
    0x3e,

    /* U+0052 "R" */
    0xff, 0x9f, 0xfb, 0xff, 0xf0, 0xfe, 0xf, 0xc1,
    0xf8, 0x3f, 0x7, 0xe1, 0xff, 0xff, 0xff, 0x7f,
    0xce, 0x31, 0xc7, 0x38, 0x67, 0xe, 0xe0, 0xfc,
    0x1f, 0x83, 0xf0, 0x70,

    /* U+0053 "S" */
    0x3f, 0x8f, 0xfb, 0xff, 0xf8, 0xfe, 0xf, 0xc1,
    0xf8, 0x7, 0x0, 0xf0, 0x1f, 0xf1, 0xff, 0x1f,
    0xf0, 0x1e, 0x1, 0xf8, 0x3f, 0x7, 0xf1, 0xff,
    0xfd, 0xff, 0x1f, 0xc0,

    /* U+0054 "T" */
    0xff, 0xff, 0xff, 0xff, 0x87, 0x0, 0xe0, 0x1c,
    0x3, 0x80, 0x70, 0xe, 0x1, 0xc0, 0x38, 0x7,
    0x0, 0xe0, 0x1c, 0x3, 0x80, 0x70, 0xe, 0x1,
    0xc0, 0x38, 0x7, 0x0,

    /* U+0055 "U" */
    0xe0, 0xfc, 0x1f, 0x83, 0xf0, 0x7e, 0xf, 0xc1,
    0xf8, 0x3f, 0x7, 0xe0, 0xfc, 0x1f, 0x83, 0xf0,
    0x7e, 0xf, 0xc1, 0xf8, 0x3f, 0x7, 0xf1, 0xff,
    0xfd, 0xff, 0x1f, 0xc0,

    /* U+0056 "V" */
    0xe0, 0xfc, 0x1f, 0x83, 0xf0, 0x7e, 0xf, 0xc1,
    0xf8, 0x3f, 0x7, 0xe0, 0xfc, 0x1f, 0x83, 0xf0,
    0x76, 0xc, 0xe3, 0x8c, 0x61, 0xdc, 0x1b, 0x83,
    0xe0, 0x3c, 0x7, 0x0,

    /* U+0057 "W" */
    0xe0, 0xfc, 0x1f, 0x83, 0xf0, 0x7e, 0xf, 0xc1,
    0xf8, 0x3f, 0x7, 0xe0, 0xfc, 0x9f, 0x93, 0xf7,
    0x7e, 0xef, 0xff, 0xfe, 0xff, 0xdf, 0xfb, 0xfe,
    0x3f, 0xc7, 0xf0, 0x70,

    /* U+0058 "X" */
    0xe0, 0xfc, 0x1d, 0x83, 0x38, 0xe3, 0x18, 0x77,
    0x6, 0xe0, 0xf8, 0xf, 0x1, 0xc0, 0x38, 0xf,
    0x1, 0xf0, 0x76, 0xe, 0xe1, 0x8c, 0x71, 0xcc,
    0x1b, 0x83, 0xf0, 0x70,

    /* U+0059 "Y" */
    0xe0, 0xfc, 0x1f, 0x83, 0xb8, 0xe7, 0x1c, 0x77,
    0xe, 0xe0, 0xf8, 0x1f, 0x1, 0xc0, 0x38, 0x7,
    0x0, 0xe0, 0x1c, 0x3, 0x80, 0x70, 0xe, 0x1,
    0xc0, 0x38, 0x7, 0x0,

    /* U+005A "Z" */
    0xff, 0xff, 0xff, 0xff, 0x80, 0xe0, 0x1c, 0x7,
    0x0, 0xe0, 0x38, 0x7, 0x1, 0xc0, 0x38, 0xe,
    0x1, 0xc0, 0x70, 0xe, 0x3, 0x80, 0x70, 0x1f,
    0xff, 0xff, 0xff, 0xf0
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 198, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1, .adv_w = 198, .box_w = 3, .box_h = 20, .ofs_x = 4, .ofs_y = 0},
    {.bitmap_index = 9, .adv_w = 198, .box_w = 11, .box_h = 11, .ofs_x = 0, .ofs_y = 4},
    {.bitmap_index = 25, .adv_w = 198, .box_w = 3, .box_h = 7, .ofs_x = 4, .ofs_y = -4},
    {.bitmap_index = 28, .adv_w = 198, .box_w = 11, .box_h = 3, .ofs_x = 0, .ofs_y = 8},
    {.bitmap_index = 33, .adv_w = 198, .box_w = 3, .box_h = 3, .ofs_x = 4, .ofs_y = 0},
    {.bitmap_index = 35, .adv_w = 198, .box_w = 11, .box_h = 20, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 63, .adv_w = 198, .box_w = 11, .box_h = 20, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 91, .adv_w = 198, .box_w = 11, .box_h = 20, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 119, .adv_w = 198, .box_w = 11, .box_h = 20, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 147, .adv_w = 198, .box_w = 11, .box_h = 20, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 175, .adv_w = 198, .box_w = 11, .box_h = 20, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 203, .adv_w = 198, .box_w = 11, .box_h = 20, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 231, .adv_w = 198, .box_w = 11, .box_h = 20, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 259, .adv_w = 198, .box_w = 11, .box_h = 20, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 287, .adv_w = 198, .box_w = 11, .box_h = 20, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 315, .adv_w = 198, .box_w = 11, .box_h = 20, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 343, .adv_w = 198, .box_w = 11, .box_h = 20, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 371, .adv_w = 198, .box_w = 11, .box_h = 20, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 399, .adv_w = 198, .box_w = 11, .box_h = 20, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 427, .adv_w = 198, .box_w = 11, .box_h = 20, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 455, .adv_w = 198, .box_w = 11, .box_h = 20, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 483, .adv_w = 198, .box_w = 11, .box_h = 20, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 511, .adv_w = 198, .box_w = 11, .box_h = 20, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 539, .adv_w = 198, .box_w = 11, .box_h = 20, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 567, .adv_w = 198, .box_w = 11, .box_h = 20, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 595, .adv_w = 198, .box_w = 11, .box_h = 20, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 623, .adv_w = 198, .box_w = 11, .box_h = 20, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 651, .adv_w = 198, .box_w = 11, .box_h = 20, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 679, .adv_w = 198, .box_w = 11, .box_h = 20, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 707, .adv_w = 198, .box_w = 11, .box_h = 20, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 735, .adv_w = 198, .box_w = 11, .box_h = 20, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 763, .adv_w = 198, .box_w = 11, .box_h = 20, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 791, .adv_w = 198, .box_w = 11, .box_h = 24, .ofs_x = 0, .ofs_y = -4},
    {.bitmap_index = 824, .adv_w = 198, .box_w = 11, .box_h = 20, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 852, .adv_w = 198, .box_w = 11, .box_h = 20, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 880, .adv_w = 198, .box_w = 11, .box_h = 20, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 908, .adv_w = 198, .box_w = 11, .box_h = 20, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 936, .adv_w = 198, .box_w = 11, .box_h = 20, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 964, .adv_w = 198, .box_w = 11, .box_h = 20, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 992, .adv_w = 198, .box_w = 11, .box_h = 20, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1020, .adv_w = 198, .box_w = 11, .box_h = 20, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1048, .adv_w = 198, .box_w = 11, .box_h = 20, .ofs_x = 0, .ofs_y = 0}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/

static const uint8_t glyph_id_ofs_list_0[] = {
    0, 1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 2, 3, 4, 5, 0,
    6, 7, 8, 9, 10, 11, 12, 13,
    14, 15, 0, 0, 0, 0, 0, 16
};

/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 32, .range_length = 32, .glyph_id_start = 1,
        .unicode_list = NULL, .glyph_id_ofs_list = glyph_id_ofs_list_0, .list_length = 32, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_FULL
    },
    {
        .range_start = 65, .range_length = 26, .glyph_id_start = 18,
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
    .bpp = 1,
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
const lv_font_t novamono_44 = {
#else
lv_font_t novamono_44 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 24,          /*The maximum line height required by the font*/
    .base_line = 4,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = 4,
    .underline_thickness = 2,
#endif
    .dsc = &font_dsc           /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
};



#endif /*#if NOVAMONO_44*/
