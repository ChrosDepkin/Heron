/*******************************************************************************
 * Size: 26 px
 * Bpp: 1
 * Opts: 
 ******************************************************************************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

#ifndef NOVAMONO_26
#define NOVAMONO_26 1
#endif

#if NOVAMONO_26

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0020 " " */
    0x0,

    /* U+0021 "!" */
    0xff, 0xff, 0xc,

    /* U+002C "," */
    0xda,

    /* U+002D "-" */
    0xfe,

    /* U+002E "." */
    0xc0,

    /* U+002F "/" */
    0x6, 0xc, 0x30, 0x61, 0x87, 0xc, 0x30, 0x61,
    0x83, 0x0,

    /* U+0030 "0" */
    0x7b, 0x3c, 0xf3, 0xcf, 0xfc, 0xf3, 0xcf, 0x37,
    0x80,

    /* U+0031 "1" */
    0x30, 0xe1, 0xc5, 0x83, 0x6, 0xc, 0x18, 0x30,
    0x63, 0xf8,

    /* U+0032 "2" */
    0x7d, 0x8f, 0x18, 0x30, 0xe3, 0x8e, 0x38, 0x61,
    0x83, 0xf8,

    /* U+0033 "3" */
    0x7d, 0x8f, 0x18, 0x30, 0x63, 0x81, 0x83, 0xc7,
    0x8d, 0xf0,

    /* U+0034 "4" */
    0x38, 0x70, 0xe2, 0xc5, 0x9b, 0x3f, 0x8c, 0x18,
    0x30, 0x60,

    /* U+0035 "5" */
    0xff, 0x83, 0x6, 0xf, 0xd8, 0xc1, 0x83, 0xc7,
    0x8d, 0xf0,

    /* U+0036 "6" */
    0x7d, 0x8f, 0x6, 0xf, 0xd8, 0xf1, 0xe3, 0xc7,
    0x8d, 0xf0,

    /* U+0037 "7" */
    0xfe, 0xc, 0x30, 0xe1, 0x86, 0xc, 0x18, 0x30,
    0x60, 0xc0,

    /* U+0038 "8" */
    0x7d, 0x8f, 0xbb, 0xe3, 0x87, 0x1b, 0x77, 0xc7,
    0x8d, 0xf0,

    /* U+0039 "9" */
    0x7d, 0x8f, 0x1e, 0x3c, 0x78, 0xdf, 0x83, 0xc7,
    0x8d, 0xf0,

    /* U+003F "?" */
    0x7b, 0x3c, 0xc7, 0x18, 0xe3, 0xc, 0x0, 0x3,
    0x0,

    /* U+0041 "A" */
    0x7d, 0x8f, 0x1e, 0x3c, 0x7f, 0xf1, 0xe3, 0xc7,
    0x8f, 0x18,

    /* U+0042 "B" */
    0xfd, 0x8f, 0x1e, 0x3c, 0x7f, 0xb1, 0xe3, 0xc7,
    0x8f, 0xf0,

    /* U+0043 "C" */
    0x7d, 0x8f, 0x1e, 0xc, 0x18, 0x30, 0x60, 0xc7,
    0x8d, 0xf0,

    /* U+0044 "D" */
    0xf9, 0x9b, 0x1e, 0x3c, 0x78, 0xf1, 0xe3, 0xc7,
    0x9b, 0xe0,

    /* U+0045 "E" */
    0xff, 0x83, 0x6, 0xc, 0x1f, 0x30, 0x60, 0xc1,
    0x83, 0xf8,

    /* U+0046 "F" */
    0xff, 0x83, 0x6, 0xc, 0x1f, 0x30, 0x60, 0xc1,
    0x83, 0x0,

    /* U+0047 "G" */
    0x7d, 0x8f, 0x1e, 0xc, 0x1b, 0xf1, 0xe3, 0xc7,
    0x8d, 0xf0,

    /* U+0048 "H" */
    0xc7, 0x8f, 0x1e, 0x3c, 0x7f, 0xf1, 0xe3, 0xc7,
    0x8f, 0x18,

    /* U+0049 "I" */
    0xfe, 0x60, 0xc1, 0x83, 0x6, 0xc, 0x18, 0x30,
    0x63, 0xf8,

    /* U+004A "J" */
    0x1e, 0xc, 0x18, 0x30, 0x60, 0xc1, 0x83, 0xc7,
    0x8d, 0xf0,

    /* U+004B "K" */
    0xc7, 0x8f, 0x1e, 0x3d, 0xfe, 0x37, 0xe3, 0xc7,
    0x8f, 0x18,

    /* U+004C "L" */
    0xc1, 0x83, 0x6, 0xc, 0x18, 0x30, 0x60, 0xc1,
    0x83, 0xf8,

    /* U+004D "M" */
    0xc7, 0xdf, 0xbf, 0xfd, 0x7a, 0xf1, 0xe3, 0xc7,
    0x8f, 0x18,

    /* U+004E "N" */
    0xc7, 0x8f, 0x9f, 0x3f, 0x7a, 0xf7, 0xe7, 0xcf,
    0x8f, 0x18,

    /* U+004F "O" */
    0x7d, 0x8f, 0x1e, 0x3c, 0x78, 0xf1, 0xe3, 0xc7,
    0x8d, 0xf0,

    /* U+0050 "P" */
    0xfd, 0x8f, 0x1e, 0x3c, 0x7f, 0xb0, 0x60, 0xc1,
    0x83, 0x0,

    /* U+0051 "Q" */
    0x7d, 0x8f, 0x1e, 0x3c, 0x78, 0xf1, 0xe3, 0xc7,
    0xfd, 0xf0, 0x81, 0xc0,

    /* U+0052 "R" */
    0xfd, 0x8f, 0x1e, 0x3c, 0x7f, 0xb2, 0x66, 0xc5,
    0x8f, 0x18,

    /* U+0053 "S" */
    0x7d, 0x8f, 0x1e, 0xc, 0xf, 0x81, 0x83, 0xc7,
    0x8d, 0xf0,

    /* U+0054 "T" */
    0xfe, 0x60, 0xc1, 0x83, 0x6, 0xc, 0x18, 0x30,
    0x60, 0xc0,

    /* U+0055 "U" */
    0xc7, 0x8f, 0x1e, 0x3c, 0x78, 0xf1, 0xe3, 0xc7,
    0x8d, 0xf0,

    /* U+0056 "V" */
    0xc7, 0x8f, 0x1e, 0x3c, 0x78, 0xf1, 0xb6, 0x6c,
    0x70, 0xe0,

    /* U+0057 "W" */
    0xc7, 0x8f, 0x1e, 0x3c, 0x7a, 0xf5, 0xff, 0xef,
    0xdf, 0x18,

    /* U+0058 "X" */
    0xc7, 0x8d, 0xb1, 0x43, 0x82, 0xe, 0x14, 0x6d,
    0x8f, 0x18,

    /* U+0059 "Y" */
    0xcf, 0x37, 0x9e, 0x30, 0xc3, 0xc, 0x30, 0xc3,
    0x0,

    /* U+005A "Z" */
    0xfe, 0xc, 0x30, 0x41, 0x82, 0xc, 0x10, 0x61,
    0x83, 0xf8
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 117, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1, .adv_w = 117, .box_w = 2, .box_h = 11, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 4, .adv_w = 117, .box_w = 2, .box_h = 4, .ofs_x = 2, .ofs_y = -3},
    {.bitmap_index = 5, .adv_w = 117, .box_w = 7, .box_h = 1, .ofs_x = 0, .ofs_y = 5},
    {.bitmap_index = 6, .adv_w = 117, .box_w = 2, .box_h = 1, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 7, .adv_w = 117, .box_w = 7, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 17, .adv_w = 117, .box_w = 6, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 26, .adv_w = 117, .box_w = 7, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 36, .adv_w = 117, .box_w = 7, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 46, .adv_w = 117, .box_w = 7, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 56, .adv_w = 117, .box_w = 7, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 66, .adv_w = 117, .box_w = 7, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 76, .adv_w = 117, .box_w = 7, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 86, .adv_w = 117, .box_w = 7, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 96, .adv_w = 117, .box_w = 7, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 106, .adv_w = 117, .box_w = 7, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 116, .adv_w = 117, .box_w = 6, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 125, .adv_w = 117, .box_w = 7, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 135, .adv_w = 117, .box_w = 7, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 145, .adv_w = 117, .box_w = 7, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 155, .adv_w = 117, .box_w = 7, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 165, .adv_w = 117, .box_w = 7, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 175, .adv_w = 117, .box_w = 7, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 185, .adv_w = 117, .box_w = 7, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 195, .adv_w = 117, .box_w = 7, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 205, .adv_w = 117, .box_w = 7, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 215, .adv_w = 117, .box_w = 7, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 225, .adv_w = 117, .box_w = 7, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 235, .adv_w = 117, .box_w = 7, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 245, .adv_w = 117, .box_w = 7, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 255, .adv_w = 117, .box_w = 7, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 265, .adv_w = 117, .box_w = 7, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 275, .adv_w = 117, .box_w = 7, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 285, .adv_w = 117, .box_w = 7, .box_h = 13, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 297, .adv_w = 117, .box_w = 7, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 307, .adv_w = 117, .box_w = 7, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 317, .adv_w = 117, .box_w = 7, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 327, .adv_w = 117, .box_w = 7, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 337, .adv_w = 117, .box_w = 7, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 347, .adv_w = 117, .box_w = 7, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 357, .adv_w = 117, .box_w = 7, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 367, .adv_w = 117, .box_w = 6, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 376, .adv_w = 117, .box_w = 7, .box_h = 11, .ofs_x = 0, .ofs_y = 0}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/

static const uint8_t glyph_id_ofs_list_0[] = {
    0, 1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 2, 3, 4, 5,
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
const lv_font_t novamono_26 = {
#else
lv_font_t novamono_26 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 14,          /*The maximum line height required by the font*/
    .base_line = 3,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = 3,
    .underline_thickness = 1,
#endif
    .dsc = &font_dsc           /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
};



#endif /*#if NOVAMONO_26*/

