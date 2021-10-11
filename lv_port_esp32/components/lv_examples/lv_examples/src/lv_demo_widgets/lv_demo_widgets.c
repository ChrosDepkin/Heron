/**
 * @file lv_demo_widgets.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "../../lv_examples.h"
#include "../lv_examples.h"
#include "../../../../lvgl/lvgl.h"

#include "lv_demo_widgets.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include <math.h>



#if LV_USE_DEMO_WIDGETS

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void controls_create(lv_obj_t * parent);
static void visuals_create(lv_obj_t * parent);
static void selectors_create(lv_obj_t * parent);
static void stats_create(lv_obj_t * parent);
static void save_create(lv_obj_t * parent);
static void slider_event_cb(lv_obj_t * slider, lv_event_t e);
static void ta_event_cb(lv_obj_t * ta, lv_event_t e);
static void kb_event_cb(lv_obj_t * ta, lv_event_t e);
static void bar_anim(lv_task_t * t);
//static void arc_anim(lv_obj_t * arc, lv_anim_value_t value);
static void linemeter_anim(lv_obj_t * linemeter, lv_anim_value_t value);
static void gauge_anim(lv_obj_t * gauge, lv_anim_value_t value);
static void table_event_cb(lv_obj_t * table, lv_event_t e);

static void bpm_updater(lv_task_t * t);
static void bank_updater(lv_task_t * t);

#if LV_USE_THEME_MATERIAL
static void color_chg_event_cb(lv_obj_t * sw, lv_event_t e);
#endif
#if LV_DEMO_WIDGETS_SLIDESHOW
static void tab_content_anim_create(lv_obj_t * parent);
static void tab_changer_task_cb(lv_task_t * task);
#endif

#define DEFAULT_VREF    1500        //Use adc2_vref_to_gpio() to obtain a better estimate
#define NO_OF_SAMPLES   16          //Multisampling

/**********************
 *  STATIC VARIABLES
 **********************/
static lv_obj_t * tv;
static lv_obj_t * t1;
static lv_obj_t * t2;
static lv_obj_t * t3;
static lv_obj_t * t4;
static lv_obj_t * t5;
static lv_obj_t * kb;


static esp_adc_cal_characteristics_t *adc_chars2;
static const adc_channel_t channel1 = ADC_CHANNEL_4;
static const adc_channel_t channel2 = ADC_CHANNEL_6;
static const adc_channel_t channel3 = ADC_CHANNEL_7;
static const adc_bits_width_t width2 = ADC_WIDTH_BIT_12;
static const adc_atten_t atten = ADC_ATTEN_DB_11;
static const adc_unit_t unit = ADC_UNIT_1;

/*************
 *  STYLES
 ************/
static lv_style_t style_box;
static lv_style_t arc_line;
static lv_style_t arc_line_indic;
static lv_style_t arc_bg;
static lv_style_t slider_line;
static lv_style_t slider_line1;
static lv_style_t slider_line_cc;
static lv_style_t slider_line_vel;
static lv_style_t textBoxStyle;
static lv_style_t textBoxStyle2;
static lv_style_t arcTextStyle;
static lv_style_t bgBoxStyle;
static lv_style_t arc_line_indic_cc;
static lv_style_t listBtnStyle;
static lv_style_t listBtnStyle2;
static lv_style_t rollerStyle;

static uint32_t adc_reading2 = 0; 

static uint32_t xVal = 0;

static uint8_t bpm = 120; //change
static uint32_t bank = 1; //change
static uint32_t mode = 1; //change
static uint32_t track = 1;  //change

char bpm_s[5];

lv_obj_t * bpm_label[1];
lv_obj_t * bank_label[1];
lv_obj_t * mode_label[4];
lv_obj_t * track_label[1];
lv_obj_t * velocity_label[1];
lv_obj_t * arcText[8];
lv_obj_t * sliderText[2];

lv_obj_t * roller;
lv_obj_t * arc1;
lv_obj_t * arc2;
lv_obj_t * arc3;
lv_obj_t * arc4;
lv_obj_t * arc5;
lv_obj_t * arc6;
lv_obj_t * arc7;
lv_obj_t * arc8;

lv_obj_t * slider1;
lv_obj_t * slider2;

lv_obj_t * listLeft;
lv_obj_t * listRight;

lv_obj_t * listLeftCont;
lv_obj_t * listRightCont;

uint8_t rollerIndex = 0;
uint8_t octave = 0;   //change
uint8_t octaveCheck = 0;
  
uint8_t slider1Val = 0; //change
uint8_t slider2Val = 0; //change
uint8_t slider1Check = 0; 
uint8_t slider2Check = 0;
uint8_t slider1CC = 0;
uint8_t slider2CC = 0;
uint8_t slider1CCCh = 0;
uint8_t slider2CCCh = 0;

uint8_t arcValues[8]; //change
uint8_t arcChecks[8];

uint8_t arcCCVal[8];
uint8_t arcCCChe[8];

uint8_t arcBool[8];
uint8_t sliderBool[2];

uint8_t currBtn = 0;

uint8_t vel = 0;
uint8_t velCheck = 0;

lv_obj_t * listPreset1;
lv_obj_t * listPreset2;
lv_obj_t * listPreset3;
lv_obj_t * listPreset4;
lv_obj_t * listPreset5;
lv_obj_t * listPreset6;
lv_obj_t * listPreset7;
lv_obj_t * listPreset8;

lv_obj_t * listBtnSave;
lv_obj_t * listBtnLoad;
lv_obj_t * listBtnClear;

LV_FONT_DECLARE(novamono_26);
LV_FONT_DECLARE(novamono_28);
LV_FONT_DECLARE(novamono_36);
LV_FONT_DECLARE(novamono_44);
LV_FONT_DECLARE(novamono_48);
LV_FONT_DECLARE(novamono_60);

#define MY_USB_SYMBOL "\xEF\x8A\x87"



/**********************
 *      MACROS
 **********************/

#if defined(LV_LVGL_H_INCLUDE_SIMPLE)
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif


#ifndef LV_ATTRIBUTE_MEM_ALIGN
#define LV_ATTRIBUTE_MEM_ALIGN
#endif

#ifndef LV_ATTRIBUTE_IMG_FEEDBACK55
#define LV_ATTRIBUTE_IMG_FEEDBACK55
#endif
#ifndef LV_ATTRIBUTE_IMG_SWITCH64
#define LV_ATTRIBUTE_IMG_SWITCH64
#endif
#ifndef LV_ATTRIBUTE_IMG_SLIDERS55
#define LV_ATTRIBUTE_IMG_SLIDERS55
#endif
#ifndef LV_ATTRIBUTE_IMG_PRESETS55
#define LV_ATTRIBUTE_IMG_PRESETS55
#endif
#ifndef LV_ATTRIBUTE_IMG_SETTINGS55
#define LV_ATTRIBUTE_IMG_SETTINGS55
#endif
#ifndef LV_ATTRIBUTE_IMG_STATS
#define LV_ATTRIBUTE_IMG_STATS
#endif
#ifndef LV_ATTRIBUTE_IMG_CLEAR
#define LV_ATTRIBUTE_IMG_CLEAR
#endif
#ifndef LV_ATTRIBUTE_IMG_SAVE
#define LV_ATTRIBUTE_IMG_SAVE
#endif
#ifndef LV_ATTRIBUTE_IMG_LOAD
#define LV_ATTRIBUTE_IMG_LOAD
#endif

const LV_ATTRIBUTE_MEM_ALIGN LV_ATTRIBUTE_LARGE_CONST LV_ATTRIBUTE_IMG_FEEDBACK55 uint8_t feedback55_map[] = {
  0x0a, 0x09, 0x0a, 0xff,   /*Color of index 0*/
  0xac, 0xac, 0xac, 0xff,   /*Color of index 1*/
  0xfb, 0xfd, 0xfb, 0xff,   /*Color of index 2*/
  0x77, 0x77, 0x77, 0xff,   /*Color of index 3*/

  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0x00, 0x00, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0x40, 0x00, 0x00, 0x01, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xa0, 0x00, 0x00, 0x00, 0x00, 0x0a, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0x00, 0x01, 0x68, 0x29, 0x40, 0x00, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xa0, 0x00, 0x6a, 0xa8, 0x2a, 0xa9, 0x00, 0x1a, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0x80, 0x06, 0xaa, 0xa8, 0x2a, 0xaa, 0x90, 0x0e, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0x00, 0x6a, 0xaa, 0xa8, 0x2a, 0xaa, 0xa9, 0x00, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xa4, 0x02, 0xaa, 0xaa, 0xab, 0xea, 0xaa, 0xaa, 0x80, 0x1a, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0x90, 0x0a, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xa0, 0x06, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0x80, 0x02, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0x80, 0x02, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0x00, 0x01, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0x40, 0x00, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xa9, 0x02, 0x01, 0xaa, 0xaa, 0xa8, 0x2a, 0xaa, 0xaa, 0x40, 0x80, 0x2a, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xa8, 0x02, 0xaa, 0xaa, 0xaa, 0xa4, 0x1a, 0xaa, 0xaa, 0xaa, 0x80, 0x2a, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xa0, 0x0a, 0xaa, 0xaa, 0xaa, 0xa0, 0x0a, 0xaa, 0xaa, 0xaa, 0xa0, 0x0a, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xa0, 0x0a, 0xaa, 0xaa, 0xaa, 0x90, 0x06, 0xaa, 0xaa, 0xaa, 0xa0, 0x0a, 0xaa, 0xaa, 
  0xaa, 0xaa, 0x80, 0x2a, 0xaa, 0xaa, 0xaa, 0x80, 0x02, 0xaa, 0xaa, 0xaa, 0xa8, 0x02, 0xaa, 0xaa, 
  0xaa, 0xaa, 0x80, 0x2a, 0xaa, 0xaa, 0xaa, 0x80, 0x02, 0xaa, 0xaa, 0xaa, 0xaa, 0x02, 0xaa, 0xaa, 
  0xaa, 0xaa, 0x80, 0xaa, 0xaa, 0xaa, 0xaa, 0x41, 0x41, 0xaa, 0xaa, 0xaa, 0xaa, 0x02, 0xaa, 0xaa, 
  0xaa, 0xaa, 0x00, 0xaa, 0xaa, 0xaa, 0xaa, 0x02, 0x80, 0xaa, 0xaa, 0xaa, 0xaa, 0x00, 0xaa, 0xaa, 
  0xaa, 0xaa, 0x00, 0xaa, 0xaa, 0xaa, 0xaa, 0x02, 0x80, 0xaa, 0xaa, 0xaa, 0xaa, 0x00, 0xaa, 0xaa, 
  0xaa, 0xaa, 0x02, 0xaa, 0xaa, 0xaa, 0xaa, 0x06, 0x90, 0xaa, 0xaa, 0xaa, 0xaa, 0x80, 0xaa, 0xaa, 
  0xaa, 0xaa, 0x02, 0xaa, 0xaa, 0xaa, 0xa9, 0x0a, 0xa0, 0x6a, 0xaa, 0xaa, 0xaa, 0x80, 0xaa, 0xaa, 
  0xaa, 0xa8, 0x02, 0xaa, 0xaa, 0xaa, 0xa8, 0x0a, 0xa0, 0x2a, 0xaa, 0xaa, 0xaa, 0x80, 0x6a, 0xaa, 
  0xaa, 0xa8, 0x00, 0x06, 0xaa, 0xaa, 0xa8, 0x3a, 0xac, 0x2a, 0xaa, 0xaa, 0x90, 0x00, 0x6a, 0xaa, 
  0xaa, 0xa8, 0x00, 0x02, 0xaa, 0xaa, 0xa8, 0x3a, 0xac, 0x2a, 0xaa, 0xaa, 0x80, 0x00, 0x6a, 0xaa, 
  0xaa, 0xa8, 0x00, 0x02, 0xaa, 0xaa, 0xa8, 0x3a, 0xac, 0x2a, 0xaa, 0xaa, 0x80, 0x00, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xa8, 0x06, 0x90, 0x2a, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xa8, 0x03, 0xc0, 0x2a, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0x40, 0x01, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xa0, 0x0a, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
};

const lv_img_dsc_t feedback55 = {
  .header.always_zero = 0,
  .header.w = 64,
  .header.h = 55,
  .data_size = 896,
  .header.cf = LV_IMG_CF_INDEXED_2BIT,
  .data = feedback55_map,
};

const LV_ATTRIBUTE_MEM_ALIGN LV_ATTRIBUTE_LARGE_CONST LV_ATTRIBUTE_IMG_SLIDERS55 uint8_t sliders55_map[] = {
  0x00, 0x00, 0x00, 0xff,   /*Color of index 0*/
  0x93, 0x94, 0x93, 0xff,   /*Color of index 1*/
  0x00, 0x00, 0x00, 0xff,   /*Color of index 2*/
  0xfa, 0xfc, 0xfa, 0xff,   /*Color of index 3*/

  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xf1, 0xff, 0xff, 0xff, 0xfc, 0x3f, 0xff, 0xff, 0xf6, 0x02, 0x7f, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xd2, 0xff, 0xff, 0xff, 0xf4, 0x1f, 0xff, 0xff, 0xe0, 0x00, 0x9f, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xd2, 0xff, 0xff, 0xff, 0xf8, 0x2f, 0xff, 0xff, 0x80, 0x00, 0x07, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xd2, 0xff, 0xff, 0xff, 0xf8, 0x2f, 0xff, 0xfe, 0x02, 0x76, 0x01, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xd2, 0xff, 0xff, 0xff, 0xf8, 0x2f, 0xff, 0xfc, 0x07, 0xfd, 0x82, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xd2, 0xff, 0xff, 0xff, 0xf8, 0x2f, 0xff, 0xf4, 0x2f, 0xff, 0x40, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xd2, 0xff, 0xff, 0xff, 0xf8, 0x2f, 0xff, 0xf8, 0x1f, 0xff, 0xe0, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xd2, 0xff, 0xff, 0xff, 0xf8, 0x2f, 0xff, 0xf8, 0x1f, 0xff, 0xe0, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xd2, 0xff, 0xff, 0xff, 0xf8, 0x2f, 0xff, 0xf4, 0x2f, 0xff, 0xc0, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xd2, 0xff, 0xff, 0xff, 0xf8, 0x2f, 0xff, 0xf4, 0x07, 0xff, 0x82, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xd2, 0xff, 0xff, 0xff, 0xf8, 0x2f, 0xff, 0xfe, 0x01, 0xf6, 0x01, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xd2, 0xff, 0xff, 0xff, 0xf8, 0x2f, 0xff, 0xff, 0x00, 0x00, 0x0b, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xd2, 0xff, 0xff, 0xff, 0xf8, 0x2f, 0xff, 0xff, 0x60, 0x00, 0x2f, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xd2, 0xff, 0xff, 0xff, 0xf8, 0x2f, 0xff, 0xff, 0xf4, 0x02, 0x7f, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xd2, 0xff, 0xff, 0xff, 0xf8, 0x2f, 0xff, 0xff, 0xff, 0x87, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xd2, 0xff, 0xff, 0xff, 0xf8, 0x2f, 0xff, 0xff, 0xff, 0x87, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xd2, 0xff, 0xff, 0xff, 0xf8, 0x2f, 0xff, 0xff, 0xff, 0x87, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xd2, 0xff, 0xff, 0xff, 0xf8, 0x2f, 0xff, 0xff, 0xff, 0x87, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xd2, 0xff, 0xff, 0xff, 0xd0, 0x07, 0xff, 0xff, 0xff, 0x87, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xd2, 0xff, 0xff, 0xff, 0x00, 0x02, 0xff, 0xff, 0xff, 0x87, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xd2, 0xff, 0xff, 0xfe, 0x00, 0x00, 0xbf, 0xff, 0xff, 0x87, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xd2, 0xff, 0xff, 0xf8, 0xbf, 0xfc, 0x2f, 0xff, 0xff, 0x87, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xd2, 0xff, 0xff, 0xd0, 0xff, 0xfc, 0x07, 0xff, 0xff, 0x87, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xd2, 0xff, 0xff, 0xe0, 0xff, 0xfe, 0x0b, 0xff, 0xff, 0x87, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xd2, 0xff, 0xff, 0xe0, 0xff, 0xff, 0x0b, 0xff, 0xff, 0x87, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xd2, 0xff, 0xff, 0xe0, 0xff, 0xfd, 0x0b, 0xff, 0xff, 0x87, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xd2, 0xff, 0xff, 0xd0, 0xff, 0xfc, 0x07, 0xff, 0xff, 0x87, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xd2, 0xff, 0xff, 0xf0, 0x7f, 0xfc, 0x0f, 0xff, 0xff, 0x87, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xd2, 0xff, 0xff, 0xfe, 0x0a, 0xa0, 0xbf, 0xff, 0xff, 0x87, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xd2, 0xff, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0xff, 0x87, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xd2, 0xff, 0xff, 0xff, 0x60, 0x0b, 0xff, 0xff, 0xff, 0x87, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xd2, 0xff, 0xff, 0xff, 0xf8, 0x2f, 0xff, 0xff, 0xff, 0x87, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xd2, 0xff, 0xff, 0xff, 0xf8, 0x2f, 0xff, 0xff, 0xff, 0x87, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xd2, 0xff, 0xff, 0xff, 0xf8, 0x2f, 0xff, 0xff, 0xff, 0x87, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xd2, 0xff, 0xff, 0xff, 0xf8, 0x2f, 0xff, 0xff, 0xff, 0x87, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xfd, 0x80, 0x27, 0xff, 0xff, 0xf8, 0x2f, 0xff, 0xff, 0xff, 0x87, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xf8, 0x00, 0x01, 0xff, 0xff, 0xf8, 0x2f, 0xff, 0xff, 0xff, 0x87, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xe0, 0x08, 0x00, 0x7f, 0xff, 0xf8, 0x2f, 0xff, 0xff, 0xff, 0x87, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0x40, 0x5f, 0x40, 0xbf, 0xff, 0xf8, 0x2f, 0xff, 0xff, 0xff, 0x87, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0x02, 0xff, 0xd0, 0x3f, 0xff, 0xf8, 0x2f, 0xff, 0xff, 0xff, 0x87, 0xff, 0xff, 0xff, 
  0xff, 0xfd, 0x0b, 0xff, 0xf8, 0x3f, 0xff, 0xf8, 0x2f, 0xff, 0xff, 0xff, 0x87, 0xff, 0xff, 0xff, 
  0xff, 0xfd, 0x0b, 0xff, 0xf4, 0x3f, 0xff, 0xf8, 0x2f, 0xff, 0xff, 0xff, 0x87, 0xff, 0xff, 0xff, 
  0xff, 0xfd, 0x0b, 0xff, 0xf4, 0x3f, 0xff, 0xf8, 0x2f, 0xff, 0xff, 0xff, 0x87, 0xff, 0xff, 0xff, 
  0xff, 0xfd, 0x01, 0xff, 0xf8, 0x3f, 0xff, 0xf8, 0x2f, 0xff, 0xff, 0xff, 0x87, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0x80, 0x7f, 0xe0, 0x3f, 0xff, 0xf8, 0x2f, 0xff, 0xff, 0xff, 0x87, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xc0, 0x15, 0x80, 0x7f, 0xff, 0xf8, 0x2f, 0xff, 0xff, 0xff, 0x87, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xd0, 0x00, 0x02, 0xff, 0xff, 0xf8, 0x2f, 0xff, 0xff, 0xff, 0x87, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xf6, 0x00, 0x0b, 0xff, 0xff, 0xf4, 0x1f, 0xff, 0xff, 0xff, 0x87, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0x6a, 0x5f, 0xff, 0xff, 0xfe, 0xbf, 0xff, 0xff, 0xff, 0x6f, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
};

const lv_img_dsc_t sliders55 = {
  .header.always_zero = 0,
  .header.w = 64,
  .header.h = 55,
  .data_size = 896,
  .header.cf = LV_IMG_CF_INDEXED_2BIT,
  .data = sliders55_map,
};

const LV_ATTRIBUTE_MEM_ALIGN LV_ATTRIBUTE_LARGE_CONST LV_ATTRIBUTE_IMG_PRESETS55 uint8_t presets55_map[] = {
  0x06, 0x04, 0x06, 0xff,   /*Color of index 0*/
  0xa1, 0xa0, 0xa1, 0xff,   /*Color of index 1*/
  0xfa, 0xfc, 0xfa, 0xff,   /*Color of index 2*/
  0x68, 0x67, 0x68, 0xff,   /*Color of index 3*/

  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0x40, 0x00, 0x00, 0x00, 0x00, 0x6a, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xa9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2a, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xa8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0a, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xa8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0a, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xa8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0a, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xa8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xa8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0d, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xa8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xaa, 0xaa, 
  0xaa, 0xa8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6a, 0xaa, 
  0xaa, 0xa8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2a, 0xaa, 
  0xaa, 0xa8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2a, 0xaa, 
  0xaa, 0xa8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2a, 0xaa, 
  0xaa, 0xa8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2a, 0xaa, 
  0xaa, 0xa8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2a, 0xaa, 
  0xaa, 0xa8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2a, 0xaa, 
  0xaa, 0xa8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2a, 0xaa, 
  0xaa, 0xa8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2a, 0xaa, 
  0xaa, 0xa8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2a, 0xaa, 
  0xaa, 0xa8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2a, 0xaa, 
  0xaa, 0xa8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2a, 0xaa, 
  0xaa, 0xa8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2a, 0xaa, 
  0xaa, 0xa8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2a, 0xaa, 
  0xaa, 0xa8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2a, 0xaa, 
  0xaa, 0xa8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2a, 0xaa, 
  0xaa, 0xa8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2a, 0xaa, 
  0xaa, 0xa8, 0x00, 0x00, 0x00, 0x00, 0x03, 0xaa, 0xaa, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2a, 0xaa, 
  0xaa, 0xa8, 0x00, 0x00, 0x00, 0x00, 0x0a, 0xaa, 0xaa, 0xa0, 0x00, 0x00, 0x00, 0x00, 0x2a, 0xaa, 
  0xaa, 0xa8, 0x00, 0x00, 0x00, 0x00, 0x2a, 0xaa, 0xaa, 0xa8, 0x00, 0x00, 0x00, 0x00, 0x2a, 0xaa, 
  0xaa, 0xa8, 0x00, 0x00, 0x00, 0x00, 0xaa, 0xa9, 0x5a, 0xaa, 0x00, 0x00, 0x00, 0x00, 0x2a, 0xaa, 
  0xaa, 0xa8, 0x00, 0x00, 0x00, 0x02, 0xaa, 0xac, 0x0a, 0xaa, 0x80, 0x00, 0x00, 0x00, 0x2a, 0xaa, 
  0xaa, 0xa8, 0x00, 0x00, 0x00, 0x02, 0xaa, 0xa0, 0x0a, 0xaa, 0xa0, 0x00, 0x00, 0x00, 0x2a, 0xaa, 
  0xaa, 0xa8, 0x00, 0x00, 0x00, 0x0a, 0xaa, 0xa0, 0x0a, 0xaa, 0xa8, 0x00, 0x00, 0x00, 0x2a, 0xaa, 
  0xaa, 0xa8, 0x00, 0x00, 0x00, 0x2a, 0xaa, 0xa0, 0x0a, 0xaa, 0xa8, 0x00, 0x00, 0x00, 0x2a, 0xaa, 
  0xaa, 0xa8, 0x00, 0x00, 0x00, 0x2a, 0xaa, 0xa0, 0x0a, 0xaa, 0xa8, 0x00, 0x00, 0x00, 0x2a, 0xaa, 
  0xaa, 0xa8, 0x00, 0x00, 0x00, 0x2a, 0xaa, 0xa0, 0x0a, 0xaa, 0xa8, 0x00, 0x00, 0x00, 0x2a, 0xaa, 
  0xaa, 0xa9, 0x00, 0x00, 0x00, 0x2a, 0xaa, 0xa0, 0x0a, 0xaa, 0xa8, 0x00, 0x00, 0x00, 0x6a, 0xaa, 
  0xaa, 0xaa, 0x40, 0x00, 0x00, 0x2a, 0x9f, 0x60, 0x0a, 0xf6, 0xa8, 0x00, 0x00, 0x01, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xc0, 0x2a, 0xb0, 0x00, 0x00, 0x0e, 0xa8, 0x00, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xc0, 0x2a, 0xb0, 0x00, 0x00, 0x02, 0xa8, 0x03, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xc0, 0x2a, 0x90, 0x00, 0x00, 0x06, 0xa8, 0x03, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0x40, 0x2a, 0xa4, 0x00, 0x00, 0x3a, 0xa8, 0x03, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0x80, 0x2a, 0xa9, 0x00, 0x00, 0xea, 0xa8, 0x01, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0x90, 0x0a, 0xaa, 0x40, 0x03, 0xaa, 0xa0, 0x0e, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0x90, 0x02, 0xaa, 0x90, 0x0e, 0xaa, 0x80, 0x06, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xac, 0x00, 0xaa, 0xa4, 0x3a, 0xaa, 0x00, 0x0a, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xa8, 0x00, 0x2a, 0xaa, 0x6a, 0xa8, 0x00, 0x1a, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xa9, 0x00, 0x0a, 0xaa, 0xaa, 0xa0, 0x00, 0xea, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0x40, 0x02, 0xaa, 0xaa, 0x80, 0x03, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0x90, 0x00, 0x00, 0x00, 0x00, 0x0e, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xa7, 0x00, 0x00, 0x00, 0x00, 0x3a, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xc0, 0x00, 0x00, 0x03, 0x6a, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xa7, 0x00, 0x00, 0x36, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0x5f, 0xf5, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
};

const lv_img_dsc_t presets55 = {
  .header.always_zero = 0,
  .header.w = 64,
  .header.h = 55,
  .data_size = 896,
  .header.cf = LV_IMG_CF_INDEXED_2BIT,
  .data = presets55_map,
};

const LV_ATTRIBUTE_MEM_ALIGN LV_ATTRIBUTE_LARGE_CONST LV_ATTRIBUTE_IMG_SWITCH64 uint8_t switch64_map[] = {
  0x06, 0x04, 0x06, 0xff,   /*Color of index 0*/
  0xb4, 0xb5, 0xb4, 0xff,   /*Color of index 1*/
  0x77, 0x78, 0x77, 0xff,   /*Color of index 2*/
  0xfc, 0xfe, 0xfc, 0xff,   /*Color of index 3*/

  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xd5, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0x57, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xfd, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x7f, 0xff, 0xff, 
  0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2f, 0xff, 0xff, 
  0xff, 0xff, 0x60, 0x02, 0xa0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x09, 0xff, 0xff, 
  0xff, 0xfd, 0x80, 0x97, 0xf5, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x7f, 0xff, 
  0xff, 0xfe, 0x02, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xbf, 0xff, 
  0xff, 0xf4, 0x0b, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 
  0xff, 0xf8, 0x27, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2f, 0xff, 
  0xff, 0xf0, 0x1f, 0xff, 0xff, 0xfd, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 
  0xff, 0xd0, 0x1f, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 
  0xff, 0xd0, 0x3f, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 
  0xff, 0xe0, 0x3f, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0b, 0xff, 
  0xff, 0xe0, 0x3f, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0b, 0xff, 
  0xff, 0xd0, 0x1f, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 
  0xff, 0xf0, 0x1f, 0xff, 0xff, 0xfd, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 
  0xff, 0xf8, 0x2f, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2f, 0xff, 
  0xff, 0xf4, 0x0b, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 
  0xff, 0xfe, 0x02, 0xff, 0xff, 0xd0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xbf, 0xff, 
  0xff, 0xfd, 0x00, 0x1f, 0xfd, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 
  0xff, 0xff, 0x40, 0x02, 0xa0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 
  0xff, 0xff, 0xd8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x27, 0xff, 0xff, 
  0xff, 0xff, 0xfd, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xda, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xa7, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0x56, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0x95, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xbf, 0xff, 0xff, 
  0xff, 0xff, 0xd0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 
  0xff, 0xff, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x29, 0xa0, 0x01, 0xff, 0xff, 
  0xff, 0xfd, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0xff, 0xfe, 0x00, 0x7f, 0xff, 
  0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0x40, 0xbf, 0xff, 
  0xff, 0xf4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xd0, 0x1f, 0xff, 
  0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xbf, 0xff, 0xff, 0xf8, 0x2f, 0xff, 
  0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xf4, 0x0f, 0xff, 
  0xff, 0xd0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xfc, 0x07, 0xff, 
  0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xfc, 0x0b, 0xff, 
  0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0xff, 0xff, 0xff, 0xfc, 0x0b, 0xff, 
  0xff, 0xd0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xfc, 0x07, 0xff, 
  0xff, 0xd0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xf4, 0x07, 0xff, 
  0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xf8, 0x0f, 0xff, 
  0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xbf, 0xff, 0xff, 0xd0, 0x2f, 0xff, 
  0xff, 0xf4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2f, 0xff, 0xff, 0xe0, 0x1f, 0xff, 
  0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x09, 0xff, 0xfd, 0x80, 0xbf, 0xff, 
  0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9f, 0x58, 0x02, 0xff, 0xff, 
  0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0b, 0xff, 0xff, 
  0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2f, 0xff, 0xff, 
  0xff, 0xff, 0xfd, 0xa0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0a, 0x7f, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xf5, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x5f, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
};

const lv_img_dsc_t switch64 = {
  .header.always_zero = 0,
  .header.w = 64,
  .header.h = 64,
  .data_size = 1040,
  .header.cf = LV_IMG_CF_INDEXED_2BIT,
  .data = switch64_map,
};

const LV_ATTRIBUTE_MEM_ALIGN LV_ATTRIBUTE_LARGE_CONST LV_ATTRIBUTE_IMG_SETTINGS55 uint8_t settings55_map[] = {
  0x0c, 0x0a, 0x0c, 0xff,   /*Color of index 0*/
  0xa4, 0xa5, 0xa4, 0xff,   /*Color of index 1*/
  0xff, 0xff, 0xff, 0xff,   /*Color of index 2*/
  0x61, 0x61, 0x61, 0xff,   /*Color of index 3*/

  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0x80, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0x80, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0x80, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0x80, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0x80, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xa9, 0x00, 0x00, 0x6a, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0x80, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xa8, 0x00, 0x00, 0x2a, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0x80, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xa4, 0x00, 0x00, 0x1a, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0x80, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xa4, 0x00, 0x00, 0x1a, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0x80, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xa0, 0x1a, 0xa4, 0x0a, 0xa0, 0x1a, 0xa0, 0x0a, 0xaa, 0xaa, 0xaa, 0xaa, 0x80, 
  0xaa, 0xaa, 0xaa, 0xaa, 0x80, 0x02, 0xac, 0x3a, 0xac, 0x3a, 0x80, 0x02, 0xaa, 0xaa, 0xaa, 0xaa, 0x80, 
  0xaa, 0xaa, 0xaa, 0xaa, 0x00, 0x00, 0x50, 0x3a, 0xac, 0x05, 0x00, 0x00, 0xaa, 0xaa, 0xaa, 0xaa, 0x80, 
  0xaa, 0xaa, 0xaa, 0xa8, 0x00, 0x00, 0x00, 0x3a, 0xac, 0x00, 0x00, 0x00, 0x2a, 0xaa, 0xaa, 0xaa, 0x80, 
  0xaa, 0xaa, 0xaa, 0xa0, 0x00, 0x70, 0x00, 0x1a, 0xa4, 0x00, 0x00, 0x00, 0x0a, 0xaa, 0xaa, 0xaa, 0x80, 
  0xaa, 0xaa, 0xaa, 0x90, 0x02, 0xac, 0x0d, 0x6a, 0xa9, 0x00, 0x0a, 0x80, 0x02, 0xaa, 0xaa, 0xaa, 0x80, 
  0xaa, 0xaa, 0xaa, 0xb0, 0x0a, 0xa8, 0x0a, 0xaa, 0xaa, 0x80, 0x6a, 0xa0, 0x02, 0xaa, 0xaa, 0xaa, 0x80, 
  0xaa, 0xaa, 0xaa, 0x90, 0x06, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0x90, 0x06, 0xaa, 0xaa, 0xaa, 0x80, 
  0xaa, 0xaa, 0xaa, 0xa4, 0x0e, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0x80, 0x1a, 0xaa, 0xaa, 0xaa, 0x80, 
  0xaa, 0xaa, 0xaa, 0xab, 0x00, 0xaa, 0xaa, 0xa9, 0x6a, 0xaa, 0xaa, 0x00, 0xea, 0xaa, 0xaa, 0xaa, 0x80, 
  0xaa, 0xaa, 0xaa, 0xaa, 0x00, 0x6a, 0xaa, 0x40, 0x01, 0xaa, 0xa8, 0x00, 0xaa, 0xaa, 0xaa, 0xaa, 0x80, 
  0xaa, 0xaa, 0xaa, 0xaa, 0x00, 0x6a, 0xa7, 0x00, 0x00, 0xda, 0xa8, 0x00, 0xaa, 0xaa, 0xaa, 0xaa, 0x80, 
  0xaa, 0xaa, 0xaa, 0xa9, 0x00, 0xaa, 0x90, 0x00, 0x00, 0x06, 0xaa, 0x00, 0x6a, 0xaa, 0xaa, 0xaa, 0x80, 
  0xaa, 0xaa, 0xa7, 0xc0, 0x00, 0xaa, 0x80, 0x0d, 0x70, 0x02, 0xaa, 0x40, 0x03, 0xda, 0xaa, 0xaa, 0x80, 
  0xaa, 0xaa, 0x90, 0x00, 0x0e, 0xaa, 0xc0, 0x1a, 0xa4, 0x03, 0xaa, 0x80, 0x00, 0x06, 0xaa, 0xaa, 0x80, 
  0xaa, 0xaa, 0x80, 0x00, 0x06, 0xa9, 0x03, 0xaa, 0xaa, 0x00, 0x6a, 0x90, 0x00, 0x06, 0xaa, 0xaa, 0x80, 
  0xaa, 0xaa, 0x80, 0x01, 0x6a, 0xa9, 0x01, 0xaa, 0xaa, 0x40, 0x6a, 0xa9, 0x40, 0x06, 0xaa, 0xaa, 0x80, 
  0xaa, 0xaa, 0x80, 0x1a, 0xaa, 0xa8, 0x02, 0xaa, 0xaa, 0x80, 0x2a, 0xaa, 0xa4, 0x06, 0xaa, 0xaa, 0x80, 
  0xaa, 0xaa, 0x80, 0x1a, 0xaa, 0xa8, 0x0e, 0xaa, 0xaa, 0xb0, 0x2a, 0xaa, 0xa4, 0x06, 0xaa, 0xaa, 0x80, 
  0xaa, 0xaa, 0x80, 0x1a, 0xaa, 0xa8, 0x02, 0xaa, 0xaa, 0x80, 0x2a, 0xaa, 0xa4, 0x06, 0xaa, 0xaa, 0x80, 
  0xaa, 0xaa, 0x80, 0x01, 0x5a, 0xa9, 0x01, 0xaa, 0xaa, 0x40, 0x6a, 0xa9, 0x40, 0x06, 0xaa, 0xaa, 0x80, 
  0xaa, 0xaa, 0x80, 0x00, 0x06, 0xa9, 0x00, 0xaa, 0xaa, 0x00, 0x6a, 0x90, 0x00, 0x06, 0xaa, 0xaa, 0x80, 
  0xaa, 0xaa, 0x9c, 0x00, 0x0e, 0xaa, 0xc0, 0x1a, 0xa4, 0x03, 0xaa, 0x80, 0x00, 0x36, 0xaa, 0xaa, 0x80, 
  0xaa, 0xaa, 0xa5, 0x00, 0x01, 0xaa, 0xb0, 0x0f, 0xf0, 0x0e, 0xaa, 0x00, 0x0f, 0x5a, 0xaa, 0xaa, 0x80, 
  0xaa, 0xaa, 0xaa, 0xaa, 0x03, 0xaa, 0x90, 0x00, 0x00, 0x06, 0xaa, 0x00, 0xaa, 0xaa, 0xaa, 0xaa, 0x80, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xc0, 0x6a, 0xa4, 0x00, 0x00, 0x1a, 0xa9, 0x03, 0xaa, 0xaa, 0xaa, 0xaa, 0x80, 
  0xaa, 0xaa, 0xaa, 0xa9, 0x00, 0x6a, 0xaa, 0x40, 0x01, 0xaa, 0xa9, 0x03, 0xaa, 0xaa, 0xaa, 0xaa, 0x80, 
  0xaa, 0xaa, 0xaa, 0xab, 0x03, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0x00, 0xea, 0xaa, 0xaa, 0xaa, 0x80, 
  0xaa, 0xaa, 0xaa, 0xa4, 0x02, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0x80, 0x1a, 0xaa, 0xaa, 0xaa, 0x80, 
  0xaa, 0xaa, 0xaa, 0x80, 0x06, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0x90, 0x06, 0xaa, 0xaa, 0xaa, 0x80, 
  0xaa, 0xaa, 0xaa, 0x80, 0x0a, 0xa9, 0x02, 0xaa, 0xaa, 0xa4, 0x6a, 0xac, 0x02, 0xaa, 0xaa, 0xaa, 0x80, 
  0xaa, 0xaa, 0xaa, 0x80, 0x02, 0x90, 0x00, 0x6a, 0xa9, 0xf0, 0x36, 0xb0, 0x06, 0xaa, 0xaa, 0xaa, 0x80, 
  0xaa, 0xaa, 0xaa, 0xa0, 0x00, 0x40, 0x00, 0x1a, 0xa4, 0x00, 0x01, 0xc0, 0x1a, 0xaa, 0xaa, 0xaa, 0x80, 
  0xaa, 0xaa, 0xaa, 0xa8, 0x00, 0x00, 0x00, 0x0a, 0xa0, 0x00, 0x00, 0x00, 0x6a, 0xaa, 0xaa, 0xaa, 0x80, 
  0xaa, 0xaa, 0xaa, 0xaa, 0x00, 0x03, 0x50, 0x0a, 0xa0, 0x35, 0xc0, 0x01, 0xaa, 0xaa, 0xaa, 0xaa, 0x80, 
  0xaa, 0xaa, 0xaa, 0xaa, 0x80, 0x06, 0xa4, 0x0a, 0xa0, 0x1a, 0x90, 0x06, 0xaa, 0xaa, 0xaa, 0xaa, 0x80, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xa0, 0x1a, 0xa4, 0x0a, 0xa0, 0x1a, 0xa4, 0x1a, 0xaa, 0xaa, 0xaa, 0xaa, 0x80, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xa9, 0xaa, 0xa4, 0x00, 0x00, 0x1a, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0x80, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xa4, 0x00, 0x00, 0x1a, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0x80, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xa8, 0x00, 0x00, 0x2a, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0x80, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xa9, 0x00, 0x00, 0x6a, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0x80, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0x80, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0x80, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0x80, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0x80, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0x80, 
};

const lv_img_dsc_t settings55 = {
  .header.always_zero = 0,
  .header.w = 65,
  .header.h = 55,
  .data_size = 896,
  .header.cf = LV_IMG_CF_INDEXED_2BIT,
  .data = settings55_map,
};

const LV_ATTRIBUTE_MEM_ALIGN LV_ATTRIBUTE_LARGE_CONST LV_ATTRIBUTE_IMG_STATS uint8_t stats_map[] = {
  0x06, 0x04, 0x06, 0xff,   /*Color of index 0*/
  0xb4, 0xb5, 0xb4, 0xff,   /*Color of index 1*/
  0xfc, 0xfe, 0xfc, 0xff,   /*Color of index 2*/
  0x77, 0x78, 0x77, 0xff,   /*Color of index 3*/

  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xa5, 0x56, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0x90, 0x01, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0x90, 0x01, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0x90, 0x01, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0x90, 0x01, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xa9, 0x00, 0x1a, 0xaa, 0xaa, 0xaa, 0x90, 0x01, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xa9, 0x00, 0x1a, 0xaa, 0xaa, 0xaa, 0x90, 0x01, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xa9, 0x00, 0x1a, 0xaa, 0xaa, 0xaa, 0x90, 0x01, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xa9, 0x00, 0x1a, 0xaa, 0xaa, 0xaa, 0x90, 0x01, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xa9, 0x00, 0x1a, 0xa7, 0xff, 0xaa, 0x90, 0x01, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xa9, 0x00, 0x1a, 0xa4, 0x00, 0x6a, 0x90, 0x01, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xa9, 0x00, 0x1a, 0xa4, 0x00, 0x6a, 0x90, 0x01, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xa9, 0x00, 0x1a, 0xa4, 0x00, 0x6a, 0x90, 0x01, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0x95, 0x5a, 0xa9, 0x00, 0x1a, 0xa4, 0x00, 0x6a, 0x90, 0x01, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0x40, 0x06, 0xa9, 0x00, 0x1a, 0xa4, 0x00, 0x6a, 0x90, 0x01, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0x40, 0x06, 0xa9, 0x00, 0x1a, 0xa4, 0x00, 0x6a, 0x90, 0x01, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0x40, 0x06, 0xa9, 0x00, 0x1a, 0xa4, 0x00, 0x6a, 0x90, 0x01, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0x40, 0x06, 0xa9, 0x00, 0x1a, 0xa4, 0x00, 0x6a, 0x90, 0x01, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0x40, 0x06, 0xa9, 0x00, 0x1a, 0xa4, 0x00, 0x6a, 0x90, 0x01, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0x40, 0x06, 0xa9, 0x00, 0x1a, 0xa4, 0x00, 0x6a, 0x90, 0x01, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0x40, 0x06, 0xa9, 0x00, 0x1a, 0xa4, 0x00, 0x6a, 0x90, 0x01, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0x40, 0x06, 0xa9, 0x00, 0x1a, 0xa4, 0x00, 0x6a, 0x90, 0x01, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0x40, 0x06, 0xa9, 0x00, 0x1a, 0xa4, 0x00, 0x6a, 0x90, 0x01, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0x40, 0x06, 0xa9, 0x00, 0x1a, 0xa4, 0x00, 0x6a, 0x90, 0x01, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0x40, 0x06, 0xa9, 0x00, 0x1a, 0xa4, 0x00, 0x6a, 0x90, 0x01, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0x40, 0x06, 0xa9, 0x00, 0x1a, 0xa4, 0x00, 0x6a, 0x90, 0x01, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0x40, 0x06, 0xa9, 0x00, 0x1a, 0xa4, 0x00, 0x6a, 0x90, 0x01, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0x40, 0x06, 0xa9, 0x00, 0x1a, 0xa4, 0x00, 0x6a, 0x90, 0x01, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0x40, 0x06, 0xa9, 0x00, 0x1a, 0xa4, 0x00, 0x6a, 0x90, 0x01, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0x40, 0x06, 0xa9, 0x00, 0x1a, 0xa4, 0x00, 0x6a, 0x90, 0x01, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0x95, 0x5a, 0xaa, 0x55, 0x6a, 0xa9, 0x55, 0xaa, 0xa5, 0x56, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
};

const lv_img_dsc_t stats = {
  .header.always_zero = 0,
  .header.w = 64,
  .header.h = 55,
  .data_size = 896,
  .header.cf = LV_IMG_CF_INDEXED_2BIT,
  .data = stats_map,
};

const LV_ATTRIBUTE_MEM_ALIGN LV_ATTRIBUTE_LARGE_CONST LV_ATTRIBUTE_IMG_CLEAR uint8_t clear_map[] = {
  0x13, 0x12, 0x13, 0xff,   /*Color of index 0*/
  0xa9, 0xa9, 0xa9, 0xff,   /*Color of index 1*/
  0xff, 0xff, 0xff, 0xff,   /*Color of index 2*/
  0x5d, 0x5d, 0x5d, 0xff,   /*Color of index 3*/

  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0x97, 0xd6, 0xaa, 0xaa, 
  0xaa, 0xa9, 0x00, 0x00, 0x6a, 0xaa, 
  0xaa, 0xac, 0x0f, 0xf0, 0x3a, 0xaa, 
  0xaa, 0xb0, 0x6a, 0xa9, 0x0e, 0xaa, 
  0xaa, 0xc1, 0xaa, 0xaa, 0x43, 0xaa, 
  0xa9, 0x06, 0xaa, 0xaa, 0x90, 0x6a, 
  0xab, 0x3a, 0x76, 0x9d, 0xac, 0xea, 
  0xa8, 0x1a, 0x41, 0x41, 0xa4, 0x2a, 
  0xa4, 0x2a, 0xb0, 0x0e, 0xa8, 0x1a, 
  0xa4, 0xea, 0xac, 0x3a, 0xab, 0x1a, 
  0xa4, 0x2a, 0x90, 0x06, 0xa8, 0x1a, 
  0xa8, 0x2a, 0x43, 0xc1, 0xa8, 0x2a, 
  0xab, 0x1a, 0x4e, 0xb1, 0xa4, 0xea, 
  0xa9, 0x0a, 0x9a, 0xa6, 0xa0, 0x6a, 
  0xaa, 0xce, 0xaa, 0xaa, 0xb3, 0xaa, 
  0xaa, 0x43, 0x6a, 0xa9, 0xc1, 0xaa, 
  0xaa, 0x90, 0x35, 0x5c, 0x06, 0xaa, 
  0xaa, 0xa7, 0x00, 0x00, 0xda, 0xaa, 
  0xaa, 0xaa, 0x7c, 0x3d, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
};

const lv_img_dsc_t clear = {
  .header.always_zero = 0,
  .header.w = 24,
  .header.h = 24,
  .data_size = 160,
  .header.cf = LV_IMG_CF_INDEXED_2BIT,
  .data = clear_map,
};

const LV_ATTRIBUTE_MEM_ALIGN LV_ATTRIBUTE_LARGE_CONST LV_ATTRIBUTE_IMG_LOAD uint8_t load_map[] = {
  0x21, 0x21, 0x21, 0xff,   /*Color of index 0*/
  0xaa, 0xab, 0xaa, 0xff,   /*Color of index 1*/
  0x63, 0x62, 0x63, 0xff,   /*Color of index 2*/
  0xff, 0xff, 0xff, 0xff,   /*Color of index 3*/

  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xfd, 0x7f, 0xff, 0xff, 
  0xff, 0xff, 0xf4, 0x1f, 0xff, 0xff, 
  0xff, 0xff, 0xd0, 0x07, 0xff, 0xff, 
  0xff, 0xff, 0x40, 0x01, 0xff, 0xff, 
  0xff, 0xff, 0x04, 0x10, 0xff, 0xff, 
  0xff, 0xff, 0x7c, 0x3d, 0xff, 0xff, 
  0xff, 0xff, 0xfc, 0x3f, 0xff, 0xff, 
  0xff, 0x60, 0x1c, 0x34, 0x09, 0xff, 
  0xfd, 0x00, 0x1c, 0x34, 0x00, 0x7f, 
  0xfc, 0x1f, 0xfc, 0x3f, 0xf4, 0x3f, 
  0xfc, 0xbf, 0xfc, 0x3f, 0xfe, 0x3f, 
  0xfc, 0xbf, 0xfc, 0x3f, 0xfe, 0x3f, 
  0xfc, 0xbf, 0xfc, 0x3f, 0xfe, 0x3f, 
  0xfc, 0xbf, 0xfc, 0x3f, 0xfe, 0x3f, 
  0xfc, 0xbf, 0xfd, 0x7f, 0xfe, 0x3f, 
  0xfc, 0xbf, 0xff, 0xff, 0xfe, 0x3f, 
  0xfc, 0xbf, 0xff, 0xff, 0xfe, 0x3f, 
  0xfc, 0xbf, 0xff, 0xff, 0xfe, 0x3f, 
  0xfe, 0x25, 0x55, 0x55, 0x58, 0xbf, 
  0xff, 0x80, 0x00, 0x00, 0x02, 0xff, 
  0xff, 0xd5, 0x55, 0x55, 0x57, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
};

const lv_img_dsc_t load = {
  .header.always_zero = 0,
  .header.w = 24,
  .header.h = 24,
  .data_size = 160,
  .header.cf = LV_IMG_CF_INDEXED_2BIT,
  .data = load_map,
};

const LV_ATTRIBUTE_MEM_ALIGN LV_ATTRIBUTE_LARGE_CONST LV_ATTRIBUTE_IMG_SAVE uint8_t save_map[] = {
  0x11, 0x0f, 0x11, 0xff,   /*Color of index 0*/
  0x9f, 0x9f, 0x9f, 0xff,   /*Color of index 1*/
  0xff, 0xff, 0xff, 0xff,   /*Color of index 2*/
  0x56, 0x56, 0x56, 0xff,   /*Color of index 3*/

  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0x56, 0xaa, 0xaa, 0x96, 0xaa, 
  0xab, 0x01, 0xaa, 0xbe, 0x43, 0xaa, 
  0xab, 0x01, 0xaa, 0x82, 0x40, 0xea, 
  0xab, 0x01, 0xaa, 0x82, 0x40, 0xea, 
  0xab, 0x01, 0xaa, 0x82, 0x40, 0xea, 
  0xab, 0x01, 0xaa, 0xbe, 0x40, 0xea, 
  0xab, 0x03, 0x55, 0x55, 0x40, 0xea, 
  0xab, 0x00, 0x00, 0x00, 0x00, 0xea, 
  0xab, 0x35, 0x55, 0x55, 0x5c, 0xea, 
  0xab, 0x1a, 0xaa, 0xaa, 0xa4, 0xea, 
  0xab, 0x1a, 0xaa, 0xaa, 0xa4, 0xea, 
  0xab, 0x1a, 0xaa, 0xaa, 0xa4, 0xea, 
  0xab, 0x1a, 0xaa, 0xaa, 0xa4, 0xea, 
  0xab, 0x1a, 0xaa, 0xaa, 0xa4, 0xea, 
  0xab, 0x1a, 0xaa, 0xaa, 0xa4, 0xea, 
  0xab, 0x1a, 0xaa, 0xaa, 0xa4, 0xea, 
  0xab, 0x1a, 0xaa, 0xaa, 0xa4, 0xea, 
  0xab, 0x3f, 0xff, 0xff, 0xfc, 0xea, 
  0xa9, 0x00, 0x00, 0x00, 0x00, 0x6a, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
  0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 
};

const lv_img_dsc_t save = {
  .header.always_zero = 0,
  .header.w = 24,
  .header.h = 24,
  .data_size = 160,
  .header.cf = LV_IMG_CF_INDEXED_2BIT,
  .data = save_map,
};




/**********************
 *   EVENT FUNCTIONS
 **********************/

static void event_handler0(lv_obj_t * obj, lv_event_t event)
  {
    if(event == LV_EVENT_CLICKED) {
        lv_tabview_set_tab_act(tv, 0, LV_ANIM_OFF);
        printf("Hi");

    }
    else if(event == LV_EVENT_VALUE_CHANGED) {
        printf("Toggled\n");
    }
}

static void event_handler1(lv_obj_t * obj, lv_event_t event)
  {
    if(event == LV_EVENT_CLICKED) {
        lv_tabview_set_tab_act(tv, 1, LV_ANIM_OFF);
        printf("Hi");

    }
    else if(event == LV_EVENT_VALUE_CHANGED) {
        printf("Toggled\n");
    }
}

static void event_handler2(lv_obj_t * obj, lv_event_t event)
  {
    if(event == LV_EVENT_CLICKED) {
        lv_tabview_set_tab_act(tv, 2, LV_ANIM_OFF);
        printf("Hi");

    }
    else if(event == LV_EVENT_VALUE_CHANGED) {
        printf("Toggled\n");
    }
}

static void event_handler3(lv_obj_t * obj, lv_event_t event)
  {
    if(event == LV_EVENT_CLICKED) {
        lv_tabview_set_tab_act(tv, 3, LV_ANIM_OFF);
        printf("Hi");

    }
    else if(event == LV_EVENT_VALUE_CHANGED) {
        printf("Toggled\n");
    }
}

static void event_handler4(lv_obj_t * obj, lv_event_t event)
  {
    if(event == LV_EVENT_CLICKED) {
        lv_tabview_set_tab_act(tv, 4, LV_ANIM_OFF);
        printf("Hi");

    }
    else if(event == LV_EVENT_VALUE_CHANGED) {
        printf("Toggled\n");
    }
}

static void btn_bpm_e(lv_obj_t * obj, lv_event_t e)
  {
    if(e == LV_EVENT_CLICKED) {
        bpm++;
    }
}

static void btn_bank_pos_e(lv_obj_t * obj, lv_event_t e)
  {
    if(e == LV_EVENT_CLICKED) {
      if (bank == 4){
        bank = 1;
        }
      else{
        bank++;
        }
      if (track == 4){
        track = 1;
      }
      else{
        track++;
      }
      if (mode == 5){
        mode = 1;
      }
      else{
        mode++;
      }
      }
}

static void btn_bank_neg_e(lv_obj_t * obj, lv_event_t e)
  {
    if(e == LV_EVENT_CLICKED) {
        if (bank == 1){
            bank = 4;
        }else{
            bank--;   
        }
        if (track == 1){
            track = 4;
        }else{
            track--;   
        }
        if (mode == 1){
            mode = 5;
        }else{
            mode--;   
        }

    }
}

static void rollerEH(lv_obj_t * obj, lv_event_t e){
  if (e == LV_EVENT_VALUE_CHANGED){
    rollerIndex = lv_roller_get_selected(roller);
    if (rollerIndex == 0){
      octave = 0;
    }
        if (rollerIndex == 1){
      octave = 1;
    }
        if (rollerIndex == 2){
      octave = 2;
    }
        if (rollerIndex == 3){
      octave = 3;
    }
        if (rollerIndex == 4){
      octave = 4;
    }
        if (rollerIndex == 5){
      octave = 5;
    }
        if (rollerIndex == 6){
      octave = 6;
    }
        if (rollerIndex == 7){
      octave = 7;
    }
    printf("Change Oct: %d", octave);
    printf("OctCheck: %d", octaveCheck);
  }
}

static void btn_octave_e(lv_obj_t * obj, lv_event_t e){
  if (e == LV_EVENT_CLICKED){
    if (octave == 7){
      octave = 0;
    }
    else{
      octave ++;
    }
  }
}

static void arc1_btn_e(lv_obj_t * obj, lv_event_t e){
  if (e == LV_EVENT_CLICKED){
    if (arcValues[0] == 110){
      arcValues[0] = 0;
    }
    else{
      arcValues[0] = 110;
    }

    if (arcValues[1] == 90){
      arcValues[1] = 0;
    }
    else{
      arcValues[1] = 90;
    }

    if (arcValues[2] == 127){
      arcValues[2] = 0;
    }
    else{
      arcValues[2] = 127;
    }
    if (slider1Val == 80){
      slider1Val = 0;
    }
    else{
      slider1Val = 80;
    }
    if (slider2Val == 20){
      slider2Val = 0;
    }
    else{
      slider2Val = 20;
    }

  }
}

static void arc1_btn2_e(lv_obj_t * obj, lv_event_t e){
  if (e == LV_EVENT_CLICKED){
    if (arcCCVal[0] == 70){
      arcCCVal[0] = 0;
    }
    else{
      arcCCVal[0] = 70;
    }

    if (arcCCVal[1] == 20){
      arcCCVal[1] = 0;
    }
    else{
      arcCCVal[1] = 20;
    }

    if (arcCCVal[2] == 107){
      arcCCVal[2] = 0;
    }
    else{
      arcCCVal[2] = 107;
    }
    if (slider1CC == 20){
      slider1CC = 0;
    }
    else{
      slider1CC = 20;
    }
    if (slider2CC == 110){
      slider2CC = 0;
    }
    else{
      slider2CC = 110;
    }

  }
}

static void arc1_btn3_e(lv_obj_t * obj, lv_event_t e){
  if (e == LV_EVENT_CLICKED){
    if (vel == 100){
      vel = 0;
    }
    else{
      vel = 100;
    }
  }
}

static void arc1_btn4_e(lv_obj_t * obj, lv_event_t e){
  if (e == LV_EVENT_CLICKED){
    if (mode == 1){
      mode = 2;
    }
    else{
      mode = 1;
    }
  }
}

static void arc1_btn5_e(lv_obj_t * obj, lv_event_t e){
  if (e == LV_EVENT_CLICKED){
    /*
    if (sliderBool[0] == 1){
      sliderBool[0] = 0;
    }
    else{
      sliderBool[0] = 1;
    }

    if (sliderBool[1] == 1){
      sliderBool[1] = 0;
    }
    else{
      sliderBool[1] = 1;
    } */

    if (sliderBool[0] == 0){
      sliderBool[0] = 1;
      static char buf[64];
      lv_snprintf(buf, sizeof(buf), "%d", slider1CC);
      const char * texts[] = {buf}; 

      lv_bar_set_value(slider1, slider1CC, LV_ANIM_OFF);
      lv_label_set_text(sliderText[0], buf);
      lv_obj_align(sliderText[0], NULL, LV_ALIGN_CENTER, 0, 35);
      //lv_obj_reset_style_list(slider1, LV_BAR_PART_BG);
      //lv_obj_reset_style_list(slider1, LV_BAR_PART_INDIC);
      //lv_obj_add_style(slider1, LV_BAR_PART_BG, &slider_line1);
      //lv_obj_add_style(slider1, LV_BAR_PART_INDIC, &slider_line_cc);
    }
    else{
      sliderBool[0] = 0;
      static char buf[64];
      lv_snprintf(buf, sizeof(buf), "%d", slider1Val);
      const char * texts[] = {buf}; 

      lv_bar_set_value(slider1, slider1Val, LV_ANIM_OFF);
      lv_label_set_text(sliderText[0], buf);
      lv_obj_align(sliderText[0], NULL, LV_ALIGN_CENTER, 0, 35);
      //lv_obj_reset_style_list(slider1, LV_BAR_PART_INDIC);
      //lv_obj_add_style(slider1, LV_BAR_PART_INDIC, &slider_line1);
    }

    if (sliderBool[1] == 0){
      sliderBool[1] = 1;
      static char buf[64];
      lv_snprintf(buf, sizeof(buf), "%d", slider2CC);
      const char * texts[] = {buf}; 

      lv_bar_set_value(slider2, slider2CC, LV_ANIM_OFF);
      lv_label_set_text(sliderText[1], buf);
      lv_obj_align(sliderText[1], NULL, LV_ALIGN_CENTER, 0, 75);
      //lv_obj_reset_style_list(slider1, LV_BAR_PART_BG);
      //lv_obj_reset_style_list(slider2, LV_BAR_PART_INDIC);
      //lv_obj_add_style(slider2, LV_BAR_PART_BG, &slider_line);
      //lv_obj_add_style(slider2, LV_BAR_PART_INDIC, &slider_line_cc);
    }
    else{
      sliderBool[1] = 0;
      static char buf[64];
      lv_snprintf(buf, sizeof(buf), "%d", slider2Val);
      const char * texts[] = {buf}; 

      lv_bar_set_value(slider2, slider2Val, LV_ANIM_OFF);
      lv_label_set_text(sliderText[1], buf);
      lv_obj_align(sliderText[1], NULL, LV_ALIGN_CENTER, 0, 75);
      //lv_obj_reset_style_list(slider2, LV_BAR_PART_INDIC);
      //lv_obj_add_style(slider2, LV_BAR_PART_INDIC, &slider_line1);
    }

  }
}

static void arc1_e(lv_obj_t * obj, lv_event_t e){
  if (e == LV_EVENT_PRESSED){
    if (arcBool[0] == 0){
      arcBool[0] = 1;
      static char buf[64];
      lv_snprintf(buf, sizeof(buf), "%d", arcCCVal[0]);
      const char * texts[] = {buf}; 

      lv_arc_set_value(arc1, arcCCVal[0]);
      lv_label_set_text(arcText[0], buf);
      lv_obj_align(arcText[0], NULL, LV_ALIGN_CENTER, 0, 0);
      lv_obj_reset_style_list(arc1, LV_ARC_PART_INDIC);
      lv_obj_add_style(arc1, LV_ARC_PART_INDIC, &arc_line_indic_cc);
    }
    else{
      arcBool[0] = 0;
      static char buf[64];
      lv_snprintf(buf, sizeof(buf), "%d", arcValues[0]);
      const char * texts[] = {buf}; 

      lv_arc_set_value(arc1, arcValues[0]);
      lv_label_set_text(arcText[0], buf);
      lv_obj_align(arcText[0], NULL, LV_ALIGN_CENTER, 0, 0);
      lv_obj_reset_style_list(arc1, LV_ARC_PART_INDIC);
      lv_obj_add_style(arc1, LV_ARC_PART_INDIC, &arc_line_indic);
    }
  }
}

static void arc2_e(lv_obj_t * obj, lv_event_t e){
  if (e == LV_EVENT_PRESSED){
    if (arcBool[1] == 0){
      arcBool[1] = 1;
      static char buf[64];
      lv_snprintf(buf, sizeof(buf), "%d", arcCCVal[1]);
      const char * texts[] = {buf}; 

      lv_arc_set_value(arc2, arcCCVal[1]);
      lv_label_set_text(arcText[1], buf);
      lv_obj_align(arcText[1], NULL, LV_ALIGN_CENTER, 0, 0);
      lv_obj_reset_style_list(arc2, LV_ARC_PART_INDIC);
      lv_obj_add_style(arc2, LV_ARC_PART_INDIC, &arc_line_indic_cc);
    }
    else{
      arcBool[1] = 0;
      static char buf[64];
      lv_snprintf(buf, sizeof(buf), "%d", arcValues[1]);
      const char * texts[] = {buf}; 

      lv_arc_set_value(arc2, arcValues[1]);
      lv_label_set_text(arcText[1], buf);
      lv_obj_align(arcText[1], NULL, LV_ALIGN_CENTER, 0, 0);
      lv_obj_reset_style_list(arc2, LV_ARC_PART_INDIC);
      lv_obj_add_style(arc2, LV_ARC_PART_INDIC, &arc_line_indic);
    }
  }
}

static void arc3_e(lv_obj_t * obj, lv_event_t e){
  if (e == LV_EVENT_PRESSED){
    if (arcBool[2] == 0){
      arcBool[2] = 1;
      static char buf[64];
      lv_snprintf(buf, sizeof(buf), "%d", arcCCVal[2]);
      const char * texts[] = {buf}; 

      lv_arc_set_value(arc3, arcCCVal[2]);
      lv_label_set_text(arcText[2], buf);
      lv_obj_align(arcText[2], NULL, LV_ALIGN_CENTER, 0, 0);
      lv_obj_reset_style_list(arc3, LV_ARC_PART_INDIC);
      lv_obj_add_style(arc3, LV_ARC_PART_INDIC, &arc_line_indic_cc);
    }
    else{
      arcBool[2] = 0;
      static char buf[64];
      lv_snprintf(buf, sizeof(buf), "%d", arcValues[2]);
      const char * texts[] = {buf}; 

      lv_arc_set_value(arc3, arcValues[2]);
      lv_label_set_text(arcText[2], buf);
      lv_obj_align(arcText[2], NULL, LV_ALIGN_CENTER, 0, 0);
      lv_obj_reset_style_list(arc3, LV_ARC_PART_INDIC);
      lv_obj_add_style(arc3, LV_ARC_PART_INDIC, &arc_line_indic);
    }
  }
}

static void arc4_e(lv_obj_t * obj, lv_event_t e){
  if (e == LV_EVENT_PRESSED){
    if (arcBool[3] == 0){
      arcBool[3] = 1;
      static char buf[64];
      lv_snprintf(buf, sizeof(buf), "%d", arcCCVal[3]);
      const char * texts[] = {buf}; 

      lv_arc_set_value(arc4, arcCCVal[3]);
      lv_label_set_text(arcText[3], buf);
      lv_obj_align(arcText[3], NULL, LV_ALIGN_CENTER, 0, 0);
      lv_obj_reset_style_list(arc4, LV_ARC_PART_INDIC);
      lv_obj_add_style(arc4, LV_ARC_PART_INDIC, &arc_line_indic_cc);
    }
    else{
      arcBool[3] = 0;
      static char buf[64];
      lv_snprintf(buf, sizeof(buf), "%d", arcValues[3]);
      const char * texts[] = {buf}; 

      lv_arc_set_value(arc4, arcValues[3]);
      lv_label_set_text(arcText[3], buf);
      lv_obj_align(arcText[3], NULL, LV_ALIGN_CENTER, 0, 0);
      lv_obj_reset_style_list(arc4, LV_ARC_PART_INDIC);
      lv_obj_add_style(arc4, LV_ARC_PART_INDIC, &arc_line_indic);
    }
  }
}

static void arc5_e(lv_obj_t * obj, lv_event_t e){
  if (e == LV_EVENT_PRESSED){
    if (arcBool[4] == 0){
      arcBool[4] = 1;
      static char buf[64];
      lv_snprintf(buf, sizeof(buf), "%d", arcCCVal[4]);
      const char * texts[] = {buf}; 

      lv_arc_set_value(arc5, arcCCVal[4]);
      lv_label_set_text(arcText[4], buf);
      lv_obj_align(arcText[4], NULL, LV_ALIGN_CENTER, 0, 0);
      lv_obj_reset_style_list(arc5, LV_ARC_PART_INDIC);
      lv_obj_add_style(arc5, LV_ARC_PART_INDIC, &arc_line_indic_cc);
    }
    else{
      arcBool[4] = 0;
      static char buf[64];
      lv_snprintf(buf, sizeof(buf), "%d", arcValues[4]);
      const char * texts[] = {buf}; 

      lv_arc_set_value(arc5, arcValues[4]);
      lv_label_set_text(arcText[4], buf);
      lv_obj_align(arcText[4], NULL, LV_ALIGN_CENTER, 0, 0);
      lv_obj_reset_style_list(arc5, LV_ARC_PART_INDIC);
      lv_obj_add_style(arc5, LV_ARC_PART_INDIC, &arc_line_indic);
    }
  }
}

static void arc6_e(lv_obj_t * obj, lv_event_t e){
  if (e == LV_EVENT_PRESSED){
    if (arcBool[5] == 0){
      arcBool[5] = 1;
      static char buf[64];
      lv_snprintf(buf, sizeof(buf), "%d", arcCCVal[5]);
      const char * texts[] = {buf}; 

      lv_arc_set_value(arc6, arcCCVal[5]);
      lv_label_set_text(arcText[5], buf);
      lv_obj_align(arcText[5], NULL, LV_ALIGN_CENTER, 0, 0);
      lv_obj_reset_style_list(arc6, LV_ARC_PART_INDIC);
      lv_obj_add_style(arc6, LV_ARC_PART_INDIC, &arc_line_indic_cc);
    }
    else{
      arcBool[5] = 0;
      static char buf[64];
      lv_snprintf(buf, sizeof(buf), "%d", arcValues[5]);
      const char * texts[] = {buf}; 

      lv_arc_set_value(arc6, arcValues[5]);
      lv_label_set_text(arcText[5], buf);
      lv_obj_align(arcText[5], NULL, LV_ALIGN_CENTER, 0, 0);
      lv_obj_reset_style_list(arc6, LV_ARC_PART_INDIC);
      lv_obj_add_style(arc6, LV_ARC_PART_INDIC, &arc_line_indic);
    }
  }
}

static void arc7_e(lv_obj_t * obj, lv_event_t e){
  if (e == LV_EVENT_PRESSED){
    if (arcBool[6] == 0){
      arcBool[6] = 1;
      static char buf[64];
      lv_snprintf(buf, sizeof(buf), "%d", arcCCVal[6]);
      const char * texts[] = {buf}; 

      lv_arc_set_value(arc7, arcCCVal[6]);
      lv_label_set_text(arcText[6], buf);
      lv_obj_align(arcText[6], NULL, LV_ALIGN_CENTER, 0, 0);
      lv_obj_reset_style_list(arc7, LV_ARC_PART_INDIC);
      lv_obj_add_style(arc7, LV_ARC_PART_INDIC, &arc_line_indic_cc);
    }
    else{
      arcBool[6] = 0;
      static char buf[64];
      lv_snprintf(buf, sizeof(buf), "%d", arcValues[6]);
      const char * texts[] = {buf}; 

      lv_arc_set_value(arc7, arcValues[6]);
      lv_label_set_text(arcText[6], buf);
      lv_obj_align(arcText[6], NULL, LV_ALIGN_CENTER, 0, 0);
      lv_obj_reset_style_list(arc7, LV_ARC_PART_INDIC);
      lv_obj_add_style(arc7, LV_ARC_PART_INDIC, &arc_line_indic);
    }
  }
}

static void arc8_e(lv_obj_t * obj, lv_event_t e){
  if (e == LV_EVENT_PRESSED){
    if (arcBool[7] == 0){
      arcBool[7] = 1;
      static char buf[64];
      lv_snprintf(buf, sizeof(buf), "%d", arcCCVal[7]);
      const char * texts[] = {buf}; 

      lv_arc_set_value(arc8, arcCCVal[7]);
      lv_label_set_text(arcText[7], buf);
      lv_obj_align(arcText[7], NULL, LV_ALIGN_CENTER, 0, 0);
      lv_obj_reset_style_list(arc8, LV_ARC_PART_INDIC);
      lv_obj_add_style(arc8, LV_ARC_PART_INDIC, &arc_line_indic_cc);
    }
    else{
      arcBool[7] = 0;
      static char buf[64];
      lv_snprintf(buf, sizeof(buf), "%d", arcValues[7]);
      const char * texts[] = {buf}; 

      lv_arc_set_value(arc8, arcValues[7]);
      lv_label_set_text(arcText[7], buf);
      lv_obj_align(arcText[7], NULL, LV_ALIGN_CENTER, 0, 0);
      lv_obj_reset_style_list(arc8, LV_ARC_PART_INDIC);
      lv_obj_add_style(arc8, LV_ARC_PART_INDIC, &arc_line_indic);
    }
  }
}

static void slider1_e(lv_obj_t * obj, lv_event_t e){
  if (e == LV_EVENT_PRESSED){
    if (sliderBool[0] == 0){
      sliderBool[0] = 1;
      static char buf[64];
      lv_snprintf(buf, sizeof(buf), "%d", slider1CC);
      const char * texts[] = {buf}; 

      lv_bar_set_value(slider1, slider1CC, LV_ANIM_OFF);
      lv_label_set_text(sliderText[0], buf);
      lv_obj_align(sliderText[0], NULL, LV_ALIGN_CENTER, 0, 35);
      //lv_obj_reset_style_list(slider1, LV_BAR_PART_BG);
      //lv_obj_reset_style_list(slider1, LV_BAR_PART_INDIC);
      //lv_obj_add_style(slider1, LV_BAR_PART_BG, &slider_line);
      //lv_obj_add_style(slider1, LV_BAR_PART_INDIC, &slider_line_cc);
    }
    else{
      sliderBool[0] = 0;
      static char buf[64];
      lv_snprintf(buf, sizeof(buf), "%d", slider1Val);
      const char * texts[] = {buf}; 

      lv_bar_set_value(slider1, slider1Val, LV_ANIM_OFF);
      lv_label_set_text(sliderText[0], buf);
      lv_obj_align(sliderText[0], NULL, LV_ALIGN_CENTER, 0, 35);
      //lv_obj_reset_style_list(slider1, LV_BAR_PART_INDIC);
      //lv_obj_add_style(slider1, LV_BAR_PART_INDIC, &slider_line1);
    }
  }
}

static void slider2_e(lv_obj_t * obj, lv_event_t e){
  if (e == LV_EVENT_PRESSED){
    if (sliderBool[1] == 0){
      sliderBool[1] = 1;
      static char buf[64];
      lv_snprintf(buf, sizeof(buf), "%d", slider2CC);
      const char * texts[] = {buf}; 

      lv_bar_set_value(slider2, slider2CC, LV_ANIM_OFF);
      lv_label_set_text(sliderText[1], buf);
      lv_obj_align(sliderText[1], NULL, LV_ALIGN_CENTER, 0, 75);
      //lv_obj_reset_style_list(slider1, LV_BAR_PART_BG);
      //lv_obj_reset_style_list(slider2, LV_BAR_PART_INDIC);
      //lv_obj_add_style(slider2, LV_BAR_PART_INDIC, &slider_line_cc);
    }
    else{
      sliderBool[1] = 0;
      static char buf[64];
      lv_snprintf(buf, sizeof(buf), "%d", slider2Val);
      const char * texts[] = {buf}; 

      lv_bar_set_value(slider2, slider1Val, LV_ANIM_OFF);
      lv_label_set_text(sliderText[1], buf);
      lv_obj_align(sliderText[1], NULL, LV_ALIGN_CENTER, 0, 75);
      //lv_obj_reset_style_list(slider2, LV_BAR_PART_INDIC);
     //lv_obj_add_style(slider2, LV_BAR_PART_INDIC, &slider_line1);
    }
  }
}

static void listPreset1_e(lv_obj_t * obj, lv_event_t e){
  if(e == LV_EVENT_CLICKED){
    printf("Event");
        if(lv_btn_get_state(listPreset2) == LV_BTN_STATE_CHECKED_RELEASED){
      lv_btn_set_state(listPreset2, LV_BTN_STATE_PRESSED);
      lv_btn_set_state(listPreset2, LV_BTN_STATE_RELEASED);
    }
        if(lv_btn_get_state(listPreset3) == LV_BTN_STATE_CHECKED_RELEASED){
      lv_btn_set_state(listPreset3, LV_BTN_STATE_PRESSED);
      lv_btn_set_state(listPreset3, LV_BTN_STATE_RELEASED);
    }
        if(lv_btn_get_state(listPreset4) == LV_BTN_STATE_CHECKED_RELEASED){
      lv_btn_set_state(listPreset4, LV_BTN_STATE_PRESSED);
      lv_btn_set_state(listPreset4, LV_BTN_STATE_RELEASED);
    }
        if(lv_btn_get_state(listPreset5) == LV_BTN_STATE_CHECKED_RELEASED){
      lv_btn_set_state(listPreset5, LV_BTN_STATE_PRESSED);
      lv_btn_set_state(listPreset5, LV_BTN_STATE_RELEASED);
    }
        if(lv_btn_get_state(listPreset6) == LV_BTN_STATE_CHECKED_RELEASED){
      lv_btn_set_state(listPreset6, LV_BTN_STATE_PRESSED);
      lv_btn_set_state(listPreset6, LV_BTN_STATE_RELEASED);
    }
        if(lv_btn_get_state(listPreset7) == LV_BTN_STATE_CHECKED_RELEASED){
      lv_btn_set_state(listPreset7, LV_BTN_STATE_PRESSED);
      lv_btn_set_state(listPreset7, LV_BTN_STATE_RELEASED);
    }
        if(lv_btn_get_state(listPreset8) == LV_BTN_STATE_CHECKED_RELEASED){
      lv_btn_set_state(listPreset8, LV_BTN_STATE_PRESSED);
      lv_btn_set_state(listPreset8, LV_BTN_STATE_RELEASED);
    }
    if(lv_btn_get_state(listPreset1) == LV_BTN_STATE_CHECKED_RELEASED){
      currBtn = 1;
    }
  }
}

static void listPreset2_e(lv_obj_t * obj, lv_event_t e){
  if(e == LV_EVENT_CLICKED){
    printf("Event");
        if(lv_btn_get_state(listPreset1) == LV_BTN_STATE_CHECKED_RELEASED){
      lv_btn_set_state(listPreset1, LV_BTN_STATE_PRESSED);
      lv_btn_set_state(listPreset1, LV_BTN_STATE_RELEASED);
    }
        if(lv_btn_get_state(listPreset3) == LV_BTN_STATE_CHECKED_RELEASED){
      lv_btn_set_state(listPreset3, LV_BTN_STATE_PRESSED);
      lv_btn_set_state(listPreset3, LV_BTN_STATE_RELEASED);
    }
        if(lv_btn_get_state(listPreset4) == LV_BTN_STATE_CHECKED_RELEASED){
      lv_btn_set_state(listPreset4, LV_BTN_STATE_PRESSED);
      lv_btn_set_state(listPreset4, LV_BTN_STATE_RELEASED);
    }
        if(lv_btn_get_state(listPreset5) == LV_BTN_STATE_CHECKED_RELEASED){
      lv_btn_set_state(listPreset5, LV_BTN_STATE_PRESSED);
      lv_btn_set_state(listPreset5, LV_BTN_STATE_RELEASED);
    }
        if(lv_btn_get_state(listPreset6) == LV_BTN_STATE_CHECKED_RELEASED){
      lv_btn_set_state(listPreset6, LV_BTN_STATE_PRESSED);
      lv_btn_set_state(listPreset6, LV_BTN_STATE_RELEASED);
    }
        if(lv_btn_get_state(listPreset7) == LV_BTN_STATE_CHECKED_RELEASED){
      lv_btn_set_state(listPreset7, LV_BTN_STATE_PRESSED);
      lv_btn_set_state(listPreset7, LV_BTN_STATE_RELEASED);
    }
        if(lv_btn_get_state(listPreset8) == LV_BTN_STATE_CHECKED_RELEASED){
      lv_btn_set_state(listPreset8, LV_BTN_STATE_PRESSED);
      lv_btn_set_state(listPreset8, LV_BTN_STATE_RELEASED);
    }
    if(lv_btn_get_state(listPreset2) == LV_BTN_STATE_CHECKED_RELEASED){
      currBtn = 2;
    }
  }
}

static void listPreset3_e(lv_obj_t * obj, lv_event_t e){
  if(e == LV_EVENT_CLICKED){
    printf("Event");
        if(lv_btn_get_state(listPreset2) == LV_BTN_STATE_CHECKED_RELEASED){
      lv_btn_set_state(listPreset2, LV_BTN_STATE_PRESSED);
      lv_btn_set_state(listPreset2, LV_BTN_STATE_RELEASED);
    }
        if(lv_btn_get_state(listPreset1) == LV_BTN_STATE_CHECKED_RELEASED){
      lv_btn_set_state(listPreset1, LV_BTN_STATE_PRESSED);
      lv_btn_set_state(listPreset1, LV_BTN_STATE_RELEASED);
    }
        if(lv_btn_get_state(listPreset4) == LV_BTN_STATE_CHECKED_RELEASED){
      lv_btn_set_state(listPreset4, LV_BTN_STATE_PRESSED);
      lv_btn_set_state(listPreset4, LV_BTN_STATE_RELEASED);
    }
        if(lv_btn_get_state(listPreset5) == LV_BTN_STATE_CHECKED_RELEASED){
      lv_btn_set_state(listPreset5, LV_BTN_STATE_PRESSED);
      lv_btn_set_state(listPreset5, LV_BTN_STATE_RELEASED);
    }
        if(lv_btn_get_state(listPreset6) == LV_BTN_STATE_CHECKED_RELEASED){
      lv_btn_set_state(listPreset6, LV_BTN_STATE_PRESSED);
      lv_btn_set_state(listPreset6, LV_BTN_STATE_RELEASED);
    }
        if(lv_btn_get_state(listPreset7) == LV_BTN_STATE_CHECKED_RELEASED){
      lv_btn_set_state(listPreset7, LV_BTN_STATE_PRESSED);
      lv_btn_set_state(listPreset7, LV_BTN_STATE_RELEASED);
    }
        if(lv_btn_get_state(listPreset8) == LV_BTN_STATE_CHECKED_RELEASED){
      lv_btn_set_state(listPreset8, LV_BTN_STATE_PRESSED);
      lv_btn_set_state(listPreset8, LV_BTN_STATE_RELEASED);
    }
    if(lv_btn_get_state(listPreset3) == LV_BTN_STATE_CHECKED_RELEASED){
      currBtn = 3;
    }
  }
}

static void listPreset4_e(lv_obj_t * obj, lv_event_t e){
  if(e == LV_EVENT_CLICKED){
    printf("Event");
        if(lv_btn_get_state(listPreset2) == LV_BTN_STATE_CHECKED_RELEASED){
      lv_btn_set_state(listPreset2, LV_BTN_STATE_PRESSED);
      lv_btn_set_state(listPreset2, LV_BTN_STATE_RELEASED);
    }
        if(lv_btn_get_state(listPreset3) == LV_BTN_STATE_CHECKED_RELEASED){
      lv_btn_set_state(listPreset3, LV_BTN_STATE_PRESSED);
      lv_btn_set_state(listPreset3, LV_BTN_STATE_RELEASED);
    }
        if(lv_btn_get_state(listPreset1) == LV_BTN_STATE_CHECKED_RELEASED){
      lv_btn_set_state(listPreset1, LV_BTN_STATE_PRESSED);
      lv_btn_set_state(listPreset1, LV_BTN_STATE_RELEASED);
    }
        if(lv_btn_get_state(listPreset5) == LV_BTN_STATE_CHECKED_RELEASED){
      lv_btn_set_state(listPreset5, LV_BTN_STATE_PRESSED);
      lv_btn_set_state(listPreset5, LV_BTN_STATE_RELEASED);
    }
        if(lv_btn_get_state(listPreset6) == LV_BTN_STATE_CHECKED_RELEASED){
      lv_btn_set_state(listPreset6, LV_BTN_STATE_PRESSED);
      lv_btn_set_state(listPreset6, LV_BTN_STATE_RELEASED);
    }
        if(lv_btn_get_state(listPreset7) == LV_BTN_STATE_CHECKED_RELEASED){
      lv_btn_set_state(listPreset7, LV_BTN_STATE_PRESSED);
      lv_btn_set_state(listPreset7, LV_BTN_STATE_RELEASED);
    }
        if(lv_btn_get_state(listPreset8) == LV_BTN_STATE_CHECKED_RELEASED){
      lv_btn_set_state(listPreset8, LV_BTN_STATE_PRESSED);
      lv_btn_set_state(listPreset8, LV_BTN_STATE_RELEASED);
    }
    if(lv_btn_get_state(listPreset4) == LV_BTN_STATE_CHECKED_RELEASED){
      currBtn = 4;
    }
  }
}

static void listPreset5_e(lv_obj_t * obj, lv_event_t e){
  if(e == LV_EVENT_CLICKED){
    printf("Event");
        if(lv_btn_get_state(listPreset2) == LV_BTN_STATE_CHECKED_RELEASED){
      lv_btn_set_state(listPreset2, LV_BTN_STATE_PRESSED);
      lv_btn_set_state(listPreset2, LV_BTN_STATE_RELEASED);
    }
        if(lv_btn_get_state(listPreset3) == LV_BTN_STATE_CHECKED_RELEASED){
      lv_btn_set_state(listPreset3, LV_BTN_STATE_PRESSED);
      lv_btn_set_state(listPreset3, LV_BTN_STATE_RELEASED);
    }
        if(lv_btn_get_state(listPreset4) == LV_BTN_STATE_CHECKED_RELEASED){
      lv_btn_set_state(listPreset4, LV_BTN_STATE_PRESSED);
      lv_btn_set_state(listPreset4, LV_BTN_STATE_RELEASED);
    }
        if(lv_btn_get_state(listPreset1) == LV_BTN_STATE_CHECKED_RELEASED){
      lv_btn_set_state(listPreset1, LV_BTN_STATE_PRESSED);
      lv_btn_set_state(listPreset1, LV_BTN_STATE_RELEASED);
    }
        if(lv_btn_get_state(listPreset6) == LV_BTN_STATE_CHECKED_RELEASED){
      lv_btn_set_state(listPreset6, LV_BTN_STATE_PRESSED);
      lv_btn_set_state(listPreset6, LV_BTN_STATE_RELEASED);
    }
        if(lv_btn_get_state(listPreset7) == LV_BTN_STATE_CHECKED_RELEASED){
      lv_btn_set_state(listPreset7, LV_BTN_STATE_PRESSED);
      lv_btn_set_state(listPreset7, LV_BTN_STATE_RELEASED);
    }
        if(lv_btn_get_state(listPreset8) == LV_BTN_STATE_CHECKED_RELEASED){
      lv_btn_set_state(listPreset8, LV_BTN_STATE_PRESSED);
      lv_btn_set_state(listPreset8, LV_BTN_STATE_RELEASED);
    }
    if(lv_btn_get_state(listPreset5) == LV_BTN_STATE_CHECKED_RELEASED){
      currBtn = 5;
    }
  }
}

static void listPreset6_e(lv_obj_t * obj, lv_event_t e){
  if(e == LV_EVENT_CLICKED){
    printf("Event");
        if(lv_btn_get_state(listPreset2) == LV_BTN_STATE_CHECKED_RELEASED){
      lv_btn_set_state(listPreset2, LV_BTN_STATE_PRESSED);
      lv_btn_set_state(listPreset2, LV_BTN_STATE_RELEASED);
    }
        if(lv_btn_get_state(listPreset3) == LV_BTN_STATE_CHECKED_RELEASED){
      lv_btn_set_state(listPreset3, LV_BTN_STATE_PRESSED);
      lv_btn_set_state(listPreset3, LV_BTN_STATE_RELEASED);
    }
        if(lv_btn_get_state(listPreset4) == LV_BTN_STATE_CHECKED_RELEASED){
      lv_btn_set_state(listPreset4, LV_BTN_STATE_PRESSED);
      lv_btn_set_state(listPreset4, LV_BTN_STATE_RELEASED);
    }
        if(lv_btn_get_state(listPreset5) == LV_BTN_STATE_CHECKED_RELEASED){
      lv_btn_set_state(listPreset5, LV_BTN_STATE_PRESSED);
      lv_btn_set_state(listPreset5, LV_BTN_STATE_RELEASED);
    }
        if(lv_btn_get_state(listPreset1) == LV_BTN_STATE_CHECKED_RELEASED){
      lv_btn_set_state(listPreset1, LV_BTN_STATE_PRESSED);
      lv_btn_set_state(listPreset1, LV_BTN_STATE_RELEASED);
    }
        if(lv_btn_get_state(listPreset7) == LV_BTN_STATE_CHECKED_RELEASED){
      lv_btn_set_state(listPreset7, LV_BTN_STATE_PRESSED);
      lv_btn_set_state(listPreset7, LV_BTN_STATE_RELEASED);
    }
        if(lv_btn_get_state(listPreset8) == LV_BTN_STATE_CHECKED_RELEASED){
      lv_btn_set_state(listPreset8, LV_BTN_STATE_PRESSED);
      lv_btn_set_state(listPreset8, LV_BTN_STATE_RELEASED);
    }
    if(lv_btn_get_state(listPreset6) == LV_BTN_STATE_CHECKED_RELEASED){
      currBtn = 6;
    }
  }
}

static void listPreset7_e(lv_obj_t * obj, lv_event_t e){
  if(e == LV_EVENT_CLICKED){
    printf("Event");
        if(lv_btn_get_state(listPreset2) == LV_BTN_STATE_CHECKED_RELEASED){
      lv_btn_set_state(listPreset2, LV_BTN_STATE_PRESSED);
      lv_btn_set_state(listPreset2, LV_BTN_STATE_RELEASED);
    }
        if(lv_btn_get_state(listPreset3) == LV_BTN_STATE_CHECKED_RELEASED){
      lv_btn_set_state(listPreset3, LV_BTN_STATE_PRESSED);
      lv_btn_set_state(listPreset3, LV_BTN_STATE_RELEASED);
    }
        if(lv_btn_get_state(listPreset4) == LV_BTN_STATE_CHECKED_RELEASED){
      lv_btn_set_state(listPreset4, LV_BTN_STATE_PRESSED);
      lv_btn_set_state(listPreset4, LV_BTN_STATE_RELEASED);
    }
        if(lv_btn_get_state(listPreset5) == LV_BTN_STATE_CHECKED_RELEASED){
      lv_btn_set_state(listPreset5, LV_BTN_STATE_PRESSED);
      lv_btn_set_state(listPreset5, LV_BTN_STATE_RELEASED);
    }
        if(lv_btn_get_state(listPreset6) == LV_BTN_STATE_CHECKED_RELEASED){
      lv_btn_set_state(listPreset6, LV_BTN_STATE_PRESSED);
      lv_btn_set_state(listPreset6, LV_BTN_STATE_RELEASED);
    }
        if(lv_btn_get_state(listPreset1) == LV_BTN_STATE_CHECKED_RELEASED){
      lv_btn_set_state(listPreset1, LV_BTN_STATE_PRESSED);
      lv_btn_set_state(listPreset1, LV_BTN_STATE_RELEASED);
    }
        if(lv_btn_get_state(listPreset8) == LV_BTN_STATE_CHECKED_RELEASED){
      lv_btn_set_state(listPreset8, LV_BTN_STATE_PRESSED);
      lv_btn_set_state(listPreset8, LV_BTN_STATE_RELEASED);
    }
    if(lv_btn_get_state(listPreset7) == LV_BTN_STATE_CHECKED_RELEASED){
      currBtn = 7;
    }
  }
}

static void listPreset8_e(lv_obj_t * obj, lv_event_t e){
  if(e == LV_EVENT_CLICKED){
    printf("Event");
        if(lv_btn_get_state(listPreset2) == LV_BTN_STATE_CHECKED_RELEASED){
      lv_btn_set_state(listPreset2, LV_BTN_STATE_PRESSED);
      lv_btn_set_state(listPreset2, LV_BTN_STATE_RELEASED);
    }
        if(lv_btn_get_state(listPreset3) == LV_BTN_STATE_CHECKED_RELEASED){
      lv_btn_set_state(listPreset3, LV_BTN_STATE_PRESSED);
      lv_btn_set_state(listPreset3, LV_BTN_STATE_RELEASED);
    }
        if(lv_btn_get_state(listPreset4) == LV_BTN_STATE_CHECKED_RELEASED){
      lv_btn_set_state(listPreset4, LV_BTN_STATE_PRESSED);
      lv_btn_set_state(listPreset4, LV_BTN_STATE_RELEASED);
    }
        if(lv_btn_get_state(listPreset5) == LV_BTN_STATE_CHECKED_RELEASED){
      lv_btn_set_state(listPreset5, LV_BTN_STATE_PRESSED);
      lv_btn_set_state(listPreset5, LV_BTN_STATE_RELEASED);
    }
        if(lv_btn_get_state(listPreset6) == LV_BTN_STATE_CHECKED_RELEASED){
      lv_btn_set_state(listPreset6, LV_BTN_STATE_PRESSED);
      lv_btn_set_state(listPreset6, LV_BTN_STATE_RELEASED);
    }
        if(lv_btn_get_state(listPreset7) == LV_BTN_STATE_CHECKED_RELEASED){
      lv_btn_set_state(listPreset7, LV_BTN_STATE_PRESSED);
      lv_btn_set_state(listPreset7, LV_BTN_STATE_RELEASED);
    }
        if(lv_btn_get_state(listPreset1) == LV_BTN_STATE_CHECKED_RELEASED){
      lv_btn_set_state(listPreset1, LV_BTN_STATE_PRESSED);
      lv_btn_set_state(listPreset1, LV_BTN_STATE_RELEASED);
    }
    if(lv_btn_get_state(listPreset8) == LV_BTN_STATE_CHECKED_RELEASED){
      currBtn = 8;
    }
  }
}

static void savePreset_e(lv_obj_t * obj, lv_event_t e){
  switch(currBtn){
    case 1:
    if(lv_btn_get_state(listPreset1) == LV_BTN_STATE_CHECKED_RELEASED){
      lv_btn_set_state(listPreset1, LV_BTN_STATE_PRESSED);
      lv_btn_set_state(listPreset1, LV_BTN_STATE_RELEASED);
    }
    case 2:
    if(lv_btn_get_state(listPreset2) == LV_BTN_STATE_CHECKED_RELEASED){
      lv_btn_set_state(listPreset2, LV_BTN_STATE_PRESSED);
      lv_btn_set_state(listPreset2, LV_BTN_STATE_RELEASED);
    }
    case 3:
    if(lv_btn_get_state(listPreset3) == LV_BTN_STATE_CHECKED_RELEASED){
      lv_btn_set_state(listPreset3, LV_BTN_STATE_PRESSED);
      lv_btn_set_state(listPreset3, LV_BTN_STATE_RELEASED);
    }
    case 4:
    if(lv_btn_get_state(listPreset4) == LV_BTN_STATE_CHECKED_RELEASED){
      lv_btn_set_state(listPreset4, LV_BTN_STATE_PRESSED);
      lv_btn_set_state(listPreset4, LV_BTN_STATE_RELEASED);
    }
    case 5:
    if(lv_btn_get_state(listPreset5) == LV_BTN_STATE_CHECKED_RELEASED){
      lv_btn_set_state(listPreset5, LV_BTN_STATE_PRESSED);
      lv_btn_set_state(listPreset5, LV_BTN_STATE_RELEASED);
    }
    case 6:
    if(lv_btn_get_state(listPreset6) == LV_BTN_STATE_CHECKED_RELEASED){
      lv_btn_set_state(listPreset6, LV_BTN_STATE_PRESSED);
      lv_btn_set_state(listPreset6, LV_BTN_STATE_RELEASED);
    }
    case 7:
    if(lv_btn_get_state(listPreset7) == LV_BTN_STATE_CHECKED_RELEASED){
      lv_btn_set_state(listPreset7, LV_BTN_STATE_PRESSED);
      lv_btn_set_state(listPreset7, LV_BTN_STATE_RELEASED);
    }
    case 8:
    if(lv_btn_get_state(listPreset8) == LV_BTN_STATE_CHECKED_RELEASED){
      lv_btn_set_state(listPreset8, LV_BTN_STATE_PRESSED);
      lv_btn_set_state(listPreset8, LV_BTN_STATE_RELEASED);
    }
  }
}

static void loadPreset_e(lv_obj_t * obj, lv_event_t e){
  switch(currBtn){
    case 1:
    if(lv_btn_get_state(listPreset1) == LV_BTN_STATE_CHECKED_RELEASED){
      lv_btn_set_state(listPreset1, LV_BTN_STATE_PRESSED);
      lv_btn_set_state(listPreset1, LV_BTN_STATE_RELEASED);
    }
    case 2:
    if(lv_btn_get_state(listPreset2) == LV_BTN_STATE_CHECKED_RELEASED){
      lv_btn_set_state(listPreset2, LV_BTN_STATE_PRESSED);
      lv_btn_set_state(listPreset2, LV_BTN_STATE_RELEASED);
    }
    case 3:
    if(lv_btn_get_state(listPreset3) == LV_BTN_STATE_CHECKED_RELEASED){
      lv_btn_set_state(listPreset3, LV_BTN_STATE_PRESSED);
      lv_btn_set_state(listPreset3, LV_BTN_STATE_RELEASED);
    }
    case 4:
    if(lv_btn_get_state(listPreset4) == LV_BTN_STATE_CHECKED_RELEASED){
      lv_btn_set_state(listPreset4, LV_BTN_STATE_PRESSED);
      lv_btn_set_state(listPreset4, LV_BTN_STATE_RELEASED);
    }
    case 5:
    if(lv_btn_get_state(listPreset5) == LV_BTN_STATE_CHECKED_RELEASED){
      lv_btn_set_state(listPreset5, LV_BTN_STATE_PRESSED);
      lv_btn_set_state(listPreset5, LV_BTN_STATE_RELEASED);
    }
    case 6:
    if(lv_btn_get_state(listPreset6) == LV_BTN_STATE_CHECKED_RELEASED){
      lv_btn_set_state(listPreset6, LV_BTN_STATE_PRESSED);
      lv_btn_set_state(listPreset6, LV_BTN_STATE_RELEASED);
    }
    case 7:
    if(lv_btn_get_state(listPreset7) == LV_BTN_STATE_CHECKED_RELEASED){
      lv_btn_set_state(listPreset7, LV_BTN_STATE_PRESSED);
      lv_btn_set_state(listPreset7, LV_BTN_STATE_RELEASED);
    }
    case 8:
    if(lv_btn_get_state(listPreset8) == LV_BTN_STATE_CHECKED_RELEASED){
      lv_btn_set_state(listPreset8, LV_BTN_STATE_PRESSED);
      lv_btn_set_state(listPreset8, LV_BTN_STATE_RELEASED);
    }
  }
}

static void clearPreset_e(lv_obj_t * obj, lv_event_t e){
  switch(currBtn){
    case 1:
    if(lv_btn_get_state(listPreset1) == LV_BTN_STATE_CHECKED_RELEASED){
      lv_btn_set_state(listPreset1, LV_BTN_STATE_PRESSED);
      lv_btn_set_state(listPreset1, LV_BTN_STATE_RELEASED);
    }
    case 2:
    if(lv_btn_get_state(listPreset2) == LV_BTN_STATE_CHECKED_RELEASED){
      lv_btn_set_state(listPreset2, LV_BTN_STATE_PRESSED);
      lv_btn_set_state(listPreset2, LV_BTN_STATE_RELEASED);
    }
    case 3:
    if(lv_btn_get_state(listPreset3) == LV_BTN_STATE_CHECKED_RELEASED){
      lv_btn_set_state(listPreset3, LV_BTN_STATE_PRESSED);
      lv_btn_set_state(listPreset3, LV_BTN_STATE_RELEASED);
    }
    case 4:
    if(lv_btn_get_state(listPreset4) == LV_BTN_STATE_CHECKED_RELEASED){
      lv_btn_set_state(listPreset4, LV_BTN_STATE_PRESSED);
      lv_btn_set_state(listPreset4, LV_BTN_STATE_RELEASED);
    }
    case 5:
    if(lv_btn_get_state(listPreset5) == LV_BTN_STATE_CHECKED_RELEASED){
      lv_btn_set_state(listPreset5, LV_BTN_STATE_PRESSED);
      lv_btn_set_state(listPreset5, LV_BTN_STATE_RELEASED);
    }
    case 6:
    if(lv_btn_get_state(listPreset6) == LV_BTN_STATE_CHECKED_RELEASED){
      lv_btn_set_state(listPreset6, LV_BTN_STATE_PRESSED);
      lv_btn_set_state(listPreset6, LV_BTN_STATE_RELEASED);
    }
    case 7:
    if(lv_btn_get_state(listPreset7) == LV_BTN_STATE_CHECKED_RELEASED){
      lv_btn_set_state(listPreset7, LV_BTN_STATE_PRESSED);
      lv_btn_set_state(listPreset7, LV_BTN_STATE_RELEASED);
    }
    case 8:
    if(lv_btn_get_state(listPreset8) == LV_BTN_STATE_CHECKED_RELEASED){
      lv_btn_set_state(listPreset8, LV_BTN_STATE_PRESSED);
      lv_btn_set_state(listPreset8, LV_BTN_STATE_RELEASED);
    }
  }
}

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_demo_widgets(void)
  {
  static lv_style_t bgStyle;
  lv_style_init(&bgStyle);
  lv_style_set_bg_color(&bgStyle, LV_STATE_DEFAULT, lv_color_hex(0xF2EFEA));
  tv = lv_tabview_create(lv_scr_act(), NULL);
  lv_obj_add_style(tv, LV_PAGE_PART_BG, &bgStyle);
  #if LV_USE_THEME_MATERIAL
    if(LV_THEME_DEFAULT_INIT == lv_theme_material_init) {
        lv_disp_size_t disp_size = lv_disp_get_size_category(NULL);
        if(disp_size >= LV_DISP_SIZE_MEDIUM) {
            lv_obj_set_style_local_pad_left(tv, LV_TABVIEW_PART_TAB_BG, LV_STATE_DEFAULT, LV_HOR_RES / 2);
            lv_obj_t * sw = lv_switch_create(lv_scr_act(), NULL);
            if(lv_theme_get_flags() & LV_THEME_MATERIAL_FLAG_DARK)
                lv_switch_on(sw, LV_ANIM_ON);
            lv_obj_set_event_cb(sw, color_chg_event_cb);
            lv_obj_set_pos(sw, LV_DPX(10), LV_DPX(10));
            lv_obj_set_style_local_value_str(sw, LV_SWITCH_PART_BG, LV_STATE_DEFAULT, "Dark");
            lv_obj_set_style_local_value_align(sw, LV_SWITCH_PART_BG, LV_STATE_DEFAULT, LV_ALIGN_OUT_RIGHT_MID);
            lv_obj_set_style_local_value_ofs_x(sw, LV_SWITCH_PART_BG, LV_STATE_DEFAULT, LV_DPI/35);
        }
    }
  #endif

  

    static lv_style_t labelStyle1;
    lv_style_set_text_font(&labelStyle1, LV_STATE_DEFAULT, &novamono_48);
    lv_style_set_text_color(&labelStyle1, LV_STATE_DEFAULT, LV_COLOR_BLACK);

    lv_tabview_set_btns_pos(tv, LV_TABVIEW_TAB_POS_BOTTOM);

    lv_obj_set_width(tv, 320);
    lv_obj_align(tv, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 0, 0);

    t1 = lv_tabview_add_tab(tv, "Stats");
    t2 = lv_tabview_add_tab(tv, "Switches");
    t3 = lv_tabview_add_tab(tv, "Presets");
    t4 = lv_tabview_add_tab(tv, "Feedback");
    t5 = lv_tabview_add_tab(tv, "Settings");

    lv_obj_t * but1 = lv_imgbtn_create(lv_scr_act(), NULL);
    lv_obj_t * but2 = lv_imgbtn_create(lv_scr_act(), NULL);
    lv_obj_t * but3 = lv_imgbtn_create(lv_scr_act(), NULL);
    lv_obj_t * but4 = lv_imgbtn_create(lv_scr_act(), NULL);
    lv_obj_t * but5 = lv_imgbtn_create(lv_scr_act(), NULL);


    LV_IMG_DECLARE(feedback55);
    LV_IMG_DECLARE(sliders55);
    LV_IMG_DECLARE(settings55);
    LV_IMG_DECLARE(switch64);
    LV_IMG_DECLARE(presets55);
    LV_IMG_DECLARE(stats);


    static lv_style_t style;
    lv_style_init(&style);
    //lv_style_set_image_recolor_opa(&style, LV_STATE_PRESSED, LV_OPA_30);
    //lv_style_set_image_recolor(&style, LV_STATE_PRESSED, LV_COLOR_BLACK);
    //lv_style_set_text_color(&style, LV_STATE_DEFAULT, LV_COLOR_WHITE);

  //Button1
    lv_imgbtn_set_src(but1, LV_BTN_STATE_RELEASED, &stats);
    lv_imgbtn_set_src(but1, LV_BTN_STATE_PRESSED, &stats);
    lv_imgbtn_set_src(but1, LV_BTN_STATE_DISABLED, &stats);
    lv_imgbtn_set_src(but1, LV_BTN_STATE_CHECKED_RELEASED, &stats);
    lv_imgbtn_set_src(but1, LV_BTN_STATE_CHECKED_PRESSED, &stats);
    lv_imgbtn_set_src(but1, LV_BTN_STATE_CHECKED_DISABLED, &stats);
    lv_imgbtn_set_checkable(but1, true);
    lv_obj_add_style(but1, LV_IMGBTN_PART_MAIN, &style);
    lv_obj_align(but1, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 186);
    lv_obj_set_event_cb(but1, event_handler0);    /*Button event*/


  //Button2
    lv_imgbtn_set_src(but2, LV_BTN_STATE_RELEASED, &sliders55);
    lv_imgbtn_set_src(but2, LV_BTN_STATE_PRESSED, &sliders55);
    lv_imgbtn_set_src(but2, LV_BTN_STATE_DISABLED, &sliders55);
    lv_imgbtn_set_src(but2, LV_BTN_STATE_CHECKED_RELEASED, &sliders55);
    lv_imgbtn_set_src(but2, LV_BTN_STATE_CHECKED_PRESSED, &sliders55);
    lv_imgbtn_set_src(but2, LV_BTN_STATE_CHECKED_DISABLED, &sliders55);
    lv_imgbtn_set_checkable(but2, true);
    lv_obj_add_style(but2, LV_IMGBTN_PART_MAIN, &style);
    lv_obj_align(but2, NULL, LV_ALIGN_IN_TOP_LEFT, 64, 186);
    lv_obj_set_event_cb(but2, event_handler1);    /*Button event*/


  //Button3
    lv_imgbtn_set_src(but3, LV_BTN_STATE_RELEASED, &presets55);
    lv_imgbtn_set_src(but3, LV_BTN_STATE_PRESSED, &presets55);
    lv_imgbtn_set_src(but3, LV_BTN_STATE_DISABLED, &presets55);
    lv_imgbtn_set_src(but3, LV_BTN_STATE_CHECKED_RELEASED, &presets55);
    lv_imgbtn_set_src(but3, LV_BTN_STATE_CHECKED_PRESSED, &presets55);
    lv_imgbtn_set_src(but3, LV_BTN_STATE_CHECKED_DISABLED, &presets55);
    lv_imgbtn_set_checkable(but3, true);
    lv_obj_add_style(but3, LV_IMGBTN_PART_MAIN, &style);
    lv_obj_align(but3, NULL, LV_ALIGN_IN_TOP_LEFT, 128, 186);
    lv_obj_set_event_cb(but3, event_handler2);    /*Button event*/


  //Button4
    lv_imgbtn_set_src(but4, LV_BTN_STATE_RELEASED, &feedback55);
    lv_imgbtn_set_src(but4, LV_BTN_STATE_PRESSED, &feedback55);
    lv_imgbtn_set_src(but4, LV_BTN_STATE_DISABLED, &feedback55);
    lv_imgbtn_set_src(but4, LV_BTN_STATE_CHECKED_RELEASED, &feedback55);
    lv_imgbtn_set_src(but4, LV_BTN_STATE_CHECKED_PRESSED, &feedback55);
    lv_imgbtn_set_src(but4, LV_BTN_STATE_CHECKED_DISABLED, &feedback55);
    lv_imgbtn_set_checkable(but4, true);
    lv_obj_add_style(but4, LV_IMGBTN_PART_MAIN, &style);
    lv_obj_align(but4, NULL, LV_ALIGN_IN_TOP_LEFT, 192, 186);
    lv_obj_set_event_cb(but4, event_handler3);    /*Button event*/


  //Button5
    lv_imgbtn_set_src(but5, LV_BTN_STATE_RELEASED, &settings55);
    lv_imgbtn_set_src(but5, LV_BTN_STATE_PRESSED, &settings55);
    lv_imgbtn_set_src(but5, LV_BTN_STATE_DISABLED, &settings55);
    lv_imgbtn_set_src(but5, LV_BTN_STATE_CHECKED_RELEASED, &settings55);
    lv_imgbtn_set_src(but5, LV_BTN_STATE_CHECKED_PRESSED, &settings55);
    lv_imgbtn_set_src(but5, LV_BTN_STATE_CHECKED_DISABLED, &settings55);
    lv_imgbtn_set_checkable(but5, true);
    lv_obj_add_style(but5, LV_IMGBTN_PART_MAIN, &style);
    lv_obj_align(but5, NULL, LV_ALIGN_IN_TOP_LEFT, 255, 186);
    lv_obj_set_event_cb(but5, event_handler4);    /*Button event*/
    lv_obj_set_size(but5,65,55);

  /****************
   *  ArcValues
   ****************/

  for (int i = 1; i < 8; ++i){
    arcValues[i] = 0;
    arcChecks[i] = 0;
    arcCCVal[i] = 0;
    arcChecks[i] = 0;
    arcBool[i] = 0;
  }

  /****************
   *  STYLE INIT
   ****************/
  lv_style_init(&style_box);
  lv_style_set_value_align(&style_box, LV_STATE_DEFAULT, LV_ALIGN_OUT_BOTTOM_LEFT);
  lv_style_set_value_ofs_y(&style_box, LV_STATE_DEFAULT, - LV_DPX(10));
  lv_style_set_margin_top(&style_box, LV_STATE_DEFAULT, LV_DPX(30));

  lv_style_init(&arc_line);
  lv_style_set_line_width(&arc_line, LV_STATE_DEFAULT, 4);
  lv_style_set_line_color(&arc_line, LV_STATE_DEFAULT, LV_COLOR_BLACK);
  lv_style_set_bg_opa(&arc_line,LV_STATE_DEFAULT, LV_OPA_TRANSP);
  lv_style_set_bg_color(&arc_line,LV_STATE_DEFAULT, LV_COLOR_BLACK);
  lv_style_set_text_color(&arc_line,LV_STATE_DEFAULT, LV_COLOR_BLACK);
  //lv_style_set_border_color(&arc_line,LV_STATE_DEFAULT, LV_COLOR_BLACK);
  lv_style_set_border_width(&arc_line, LV_STATE_DEFAULT, 0);

  lv_style_init(&arc_line_indic);
  lv_style_set_line_width(&arc_line_indic, LV_STATE_DEFAULT, 4);
  lv_style_set_line_color(&arc_line_indic, LV_STATE_DEFAULT, LV_COLOR_MAKE(0xFF, 0x25, 0x4e));
  lv_style_set_bg_opa(&arc_line_indic, LV_STATE_DEFAULT, LV_OPA_COVER);

  lv_style_init(&arc_line_indic_cc);
  lv_style_set_line_width(&arc_line_indic_cc, LV_STATE_DEFAULT, 4);
  lv_style_set_line_color(&arc_line_indic_cc, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x52, 0xd1, 0xdc));
  lv_style_set_bg_opa(&arc_line_indic_cc, LV_STATE_DEFAULT, LV_OPA_COVER);

  lv_style_init(&arc_bg);
  lv_style_set_bg_opa(&arc_bg, LV_STATE_DEFAULT, LV_OPA_TRANSP);
  lv_style_set_border_width(&arc_bg, LV_STATE_DEFAULT, 0);
  lv_style_set_line_width(&arc_bg, LV_STATE_DEFAULT, 4);

  lv_style_init(&slider_line);
  lv_style_set_line_width(&slider_line, LV_STATE_DEFAULT, 6);
  lv_style_set_line_color(&slider_line, LV_STATE_DEFAULT, LV_COLOR_BLACK);
  lv_style_set_bg_opa(&slider_line,LV_STATE_DEFAULT, LV_OPA_COVER);
  lv_style_set_bg_color(&slider_line,LV_STATE_DEFAULT, LV_COLOR_BLACK);
  lv_style_set_text_color(&slider_line,LV_STATE_DEFAULT, LV_COLOR_BLACK);
  lv_style_set_border_color(&slider_line,LV_STATE_DEFAULT, LV_COLOR_BLACK);

  lv_style_init(&slider_line1);
  lv_style_set_line_width(&slider_line1, LV_STATE_DEFAULT, 6);
  lv_style_set_line_color(&slider_line1, LV_STATE_DEFAULT, LV_COLOR_MAKE(0xFF, 0x25, 0x4e));
  lv_style_set_bg_opa(&slider_line1,LV_STATE_DEFAULT, LV_OPA_COVER);

  lv_style_init(&slider_line_cc);
  lv_style_set_line_width(&slider_line_cc, LV_STATE_DEFAULT, 6);
  lv_style_set_line_color(&slider_line_cc, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x52, 0xd1, 0xdc));
  lv_style_set_bg_opa(&slider_line_cc,LV_STATE_DEFAULT, LV_OPA_COVER);

  lv_style_init(&slider_line_vel);
  lv_style_set_line_width(&slider_line_vel, LV_STATE_DEFAULT, 6);
  lv_style_set_line_color(&slider_line_vel, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x3A, 0x7D, 0x44));
  lv_style_set_bg_opa(&slider_line_vel,LV_STATE_DEFAULT, LV_OPA_COVER);

/*
  lv_style_init(&slider_line1);
  lv_style_set_line_width(&slider_line1, LV_STATE_DEFAULT, 6);
  lv_style_set_line_color(&slider_line1, LV_STATE_DEFAULT, LV_COLOR_RED);
  lv_style_set_bg_opa(&slider_line1,LV_STATE_DEFAULT, LV_OPA_COVER);
  lv_style_set_bg_color(&slider_line1,LV_STATE_DEFAULT, LV_COLOR_RED);
  lv_style_set_text_color(&slider_line1,LV_STATE_DEFAULT, LV_COLOR_RED);
  lv_style_set_border_color(&slider_line1,LV_STATE_DEFAULT, LV_COLOR_RED);
*/
  lv_style_init(&textBoxStyle);
  lv_style_set_pad_all(&textBoxStyle, LV_STATE_DEFAULT, 10);
  lv_style_set_text_color(&textBoxStyle, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x40, 0x3D, 0x58));
  lv_style_set_text_letter_space(&textBoxStyle, LV_STATE_DEFAULT, 5);
  lv_style_set_text_line_space(&textBoxStyle, LV_STATE_DEFAULT, 20);
  lv_style_set_text_font(&textBoxStyle, LV_STATE_DEFAULT, &novamono_44);

  lv_style_init(&rollerStyle);
  lv_style_set_pad_all(&rollerStyle, LV_STATE_DEFAULT, 10);
  lv_style_set_text_color(&rollerStyle, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x00, 0x00, 0x00));
  lv_style_set_text_letter_space(&rollerStyle, LV_STATE_DEFAULT, 5);
  lv_style_set_text_line_space(&rollerStyle, LV_STATE_DEFAULT, 20);
  lv_style_set_text_font(&rollerStyle, LV_STATE_DEFAULT, &novamono_44);
  lv_style_set_bg_color(&rollerStyle, LV_STATE_DEFAULT, LV_COLOR_BLACK);
  lv_style_set_bg_opa(&rollerStyle, LV_STATE_DEFAULT, LV_OPA_TRANSP);
  lv_style_set_outline_width(&rollerStyle, LV_STATE_DEFAULT, 0);

  lv_style_init(&textBoxStyle2);
  lv_style_set_pad_all(&textBoxStyle2, LV_STATE_DEFAULT, 10);
  lv_style_set_text_color(&textBoxStyle2, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x40, 0x3D, 0x58));
  lv_style_set_text_letter_space(&textBoxStyle2, LV_STATE_DEFAULT, 5);
  lv_style_set_text_line_space(&textBoxStyle2, LV_STATE_DEFAULT, 20);
  lv_style_set_text_font(&textBoxStyle2, LV_STATE_DEFAULT, &novamono_60);
  lv_style_set_border_color(&textBoxStyle2, LV_STATE_DEFAULT, LV_COLOR_BLACK);

  lv_style_init(&arcTextStyle);
  lv_style_set_pad_all(&arcTextStyle, LV_STATE_DEFAULT, 0);
  lv_style_set_text_color(&arcTextStyle, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x40, 0x3D, 0x58));
  lv_style_set_text_letter_space(&arcTextStyle, LV_STATE_DEFAULT, 5);
  lv_style_set_text_line_space(&arcTextStyle, LV_STATE_DEFAULT, 20);
  lv_style_set_text_font(&arcTextStyle, LV_STATE_DEFAULT, &novamono_26);

  lv_style_init(&bgBoxStyle);
  lv_style_set_bg_opa(&bgBoxStyle, LV_STATE_DEFAULT, LV_OPA_COVER); 
  lv_style_set_bg_color(&bgBoxStyle, LV_STATE_DEFAULT, LV_COLOR_MAKE(0xFF, 0xFF, 0xFF));
  lv_style_set_radius(&bgBoxStyle, LV_STATE_DEFAULT, 3);
  lv_style_set_outline_width(&bgBoxStyle, LV_STATE_DEFAULT, 2);
  lv_style_set_outline_color(&bgBoxStyle, LV_STATE_DEFAULT, LV_COLOR_RED);
  lv_style_set_outline_pad(&bgBoxStyle, LV_STATE_DEFAULT, 0);

  lv_style_init(&listBtnStyle);
  lv_style_set_bg_opa(&listBtnStyle, LV_STATE_DEFAULT, LV_OPA_COVER); 
  lv_style_set_bg_color(&listBtnStyle, LV_STATE_DEFAULT, LV_COLOR_MAKE(0xFF, 0xFF, 0xFF));
  //lv_style_set_radius(&bgBoxStyle, LV_STATE_DEFAULT, 3);
  //lv_style_set_outline_width(&bgBoxStyle, LV_STATE_DEFAULT, 2);
  lv_style_set_outline_color(&listBtnStyle, LV_STATE_DEFAULT, LV_COLOR_RED);
  //lv_style_set_outline_pad(&bgBoxStyle, LV_STATE_DEFAULT, 0);
  lv_style_set_text_font(&listBtnStyle, LV_STATE_DEFAULT, &novamono_26);
  lv_style_set_text_letter_space(&listBtnStyle, LV_STATE_DEFAULT, 5);
  lv_style_set_bg_color(&listBtnStyle, LV_BTN_STATE_CHECKED_PRESSED, LV_COLOR_MAKE(0x6a, 0x25, 0x25));

  lv_style_init(&listBtnStyle2);
  lv_style_set_bg_opa(&listBtnStyle2, LV_STATE_DEFAULT, LV_OPA_COVER); 
  lv_style_set_bg_color(&listBtnStyle2, LV_STATE_DEFAULT, LV_COLOR_MAKE(0xFF, 0xFF, 0xFF));
  //lv_style_set_radius(&bgBoxStyle, LV_STATE_DEFAULT, 3);
  //lv_style_set_outline_width(&bgBoxStyle, LV_STATE_DEFAULT, 2);
  lv_style_set_outline_color(&listBtnStyle2, LV_STATE_DEFAULT, LV_COLOR_RED);
  //lv_style_set_outline_pad(&bgBoxStyle, LV_STATE_DEFAULT, 0);
  lv_style_set_text_font(&listBtnStyle2, LV_STATE_DEFAULT, &novamono_26);
  lv_style_set_text_letter_space(&listBtnStyle2, LV_STATE_DEFAULT, 5);
  lv_style_set_bg_color(&listBtnStyle2, LV_BTN_STATE_PRESSED, LV_COLOR_MAKE(0x6a, 0x25, 0x25));








  stats_create(t1);
  visuals_create(t2);
  save_create(t3);
  controls_create(t4);
  selectors_create(t5);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

//Tasks
static void bpm_updater(lv_task_t * t)
  {
  static char buf[64];

  lv_snprintf(buf, sizeof(buf), "#000000 BPM   %d", bpm);
  const char * texts[] = {buf}; 
  //lv_obj_set_style_local_value_str(bar, LV_LABEL_ALIGN_CENTER, LV_STATE_DEFAULT, buf);
  lv_label_set_recolor(bpm_label[0], true);
  lv_label_set_text(bpm_label[0], buf);

  //lv_label_set_text(bar, buf);
  }

static void bank_updater(lv_task_t * t)
  {
    static char buf[64];
    lv_snprintf(buf, sizeof(buf), "#000000 BANK  %d", bank);

    const char * texts[] = {buf}; 
    //lv_obj_set_style_local_value_str(bar, LV_LABEL_ALIGN_CENTER, LV_STATE_DEFAULT, buf);
    lv_label_set_recolor(bank_label[0], true);
    lv_label_set_text(bank_label[0], buf);

    //lv_label_set_text(bar, buf);
  }

static void track_updater(lv_task_t * t)
  {
  static char buf[64];
  lv_snprintf(buf, sizeof(buf), "#000000 TRACK %d", track);

  const char * texts[] = {buf}; 
  //lv_obj_set_style_local_value_str(bar, LV_LABEL_ALIGN_CENTER, LV_STATE_DEFAULT, buf);
  lv_label_set_recolor(track_label[0], true);
  lv_label_set_text(track_label[0], buf);

  //lv_label_set_text(bar, buf);
  }

static void mode_updater(lv_task_t * t)
  {
  static char buf[64];
  if (mode == 1){
    lv_snprintf(buf, sizeof(buf), "U", mode); //utility
  }
  if (mode == 2){
    lv_snprintf(buf, sizeof(buf), "K", mode); //keyboard
  }
  if (mode == 3){
    lv_snprintf(buf, sizeof(buf), "S", mode); //sequencer
  }
  if (mode == 4){
    lv_snprintf(buf, sizeof(buf), "N", mode); //note
  }
  if (mode == 5){
    lv_snprintf(buf, sizeof(buf), "L", mode); //length
  }

  const char * texts[] = {buf}; 
  //lv_obj_set_style_local_value_str(bar, LV_LABEL_ALIGN_CENTER, LV_STATE_DEFAULT, buf);
  lv_label_set_recolor(mode_label[0], true);
  lv_label_set_text(mode_label[0], buf);

  //lv_label_set_text(bar, buf);
  }

static void velocity_updater(lv_task_t * t)
  {
  static char buf[64];
  lv_snprintf(buf, sizeof(buf), "#000000 VEL %d", vel); //utility

  const char * texts[] = {buf}; 
  //lv_obj_set_style_local_value_str(bar, LV_LABEL_ALIGN_CENTER, LV_STATE_DEFAULT, buf);
  lv_label_set_recolor(velocity_label[0], true);
  lv_label_set_text(velocity_label[0], buf);
  }

static void octave_updater(lv_task_t * t)
    {
    if (octave == octaveCheck){

    }
    else{
    rollerIndex = lv_roller_get_selected(roller);
      if (rollerIndex != octave){
        lv_roller_set_selected(roller, octave, LV_ANIM_OFF);
      }
      octaveCheck = octave;
    }
  }

static void arc_anim(lv_task_t * t)
    {
      //Arc1
        if(arcBool[0] == 0){
          if (arcValues[0] == arcChecks[0]){ //Value of Arc1

          }
          else{
          static char buf[64];
          lv_snprintf(buf, sizeof(buf), "%d", arcValues[0]);
          const char * texts[] = {buf}; 

          lv_arc_set_value(arc1, arcValues[0]);
          lv_label_set_text(arcText[0], buf);
          lv_obj_align(arcText[0], NULL, LV_ALIGN_CENTER, 0, 0);
          //lv_label_set_align(arc1Text[0], LV_LABEL_ALIGN_CENTER);

          arcChecks[0] = arcValues[0];
          }
        }else{
          if (arcCCVal[0] == arcCCChe[0]){ //Value of Arc1

          }
          else{
          static char buf[64];
          lv_snprintf(buf, sizeof(buf), "%d", arcCCVal[0]);
          const char * texts[] = {buf}; 

          lv_arc_set_value(arc1, arcCCVal[0]);
          lv_label_set_text(arcText[0], buf);
          lv_obj_align(arcText[0], NULL, LV_ALIGN_CENTER, 0, 0);
          //lv_label_set_align(arc1Text[0], LV_LABEL_ALIGN_CENTER);

          arcCCChe[0] = arcCCVal[0];
          }
        }


      //Arc2
      if(arcBool[1] == 0){
        if (arcValues[1] == arcChecks[1]){

        }
        else{
        static char buf[64];
        lv_snprintf(buf, sizeof(buf), "%d", arcValues[1]);
        const char * texts[] = {buf}; 
        
        lv_arc_set_value(arc2, arcValues[1]);
        lv_label_set_text(arcText[1], buf);
        lv_obj_align(arcText[1], NULL, LV_ALIGN_CENTER, 0, 0);

        arcChecks[1] = arcValues[1];
        }
      }else{
        if (arcCCVal[1] == arcCCChe[1]){ //Value of Arc1
          }
          else{
          static char buf[64];
          lv_snprintf(buf, sizeof(buf), "%d", arcCCVal[1]);
          const char * texts[] = {buf}; 

          lv_arc_set_value(arc2, arcCCVal[1]);
          lv_label_set_text(arcText[1], buf);
          lv_obj_align(arcText[1], NULL, LV_ALIGN_CENTER, 0, 0);
          //lv_label_set_align(arc1Text[0], LV_LABEL_ALIGN_CENTER);

          arcCCChe[1] = arcCCVal[1];
          }
      }


        //Arc3

      if(arcBool[2] == 0){
        if (arcValues[2] == arcChecks[2]){

        }
        else{
        static char buf[64];
        lv_snprintf(buf, sizeof(buf), "%d", arcValues[2]);
        const char * texts[] = {buf}; 
        
        lv_arc_set_value(arc3, arcValues[2]);
        lv_label_set_text(arcText[2], buf);
        lv_obj_align(arcText[2], NULL, LV_ALIGN_CENTER, 0, 0);

        arcChecks[2] = arcValues[2];
        }
      }else{
        if (arcCCVal[2] == arcCCChe[2]){ //Value of Arc1
          }
          else{
          static char buf[64];
          lv_snprintf(buf, sizeof(buf), "%d", arcCCVal[2]);
          const char * texts[] = {buf}; 

          lv_arc_set_value(arc3, arcCCVal[2]);
          lv_label_set_text(arcText[2], buf);
          lv_obj_align(arcText[2], NULL, LV_ALIGN_CENTER, 0, 0);
          //lv_label_set_align(arc1Text[0], LV_LABEL_ALIGN_CENTER);

          arcCCChe[2] = arcCCVal[2];
          }
      }

        //Arc4

      if(arcBool[3] == 0){
        if (arcValues[3] == arcChecks[3]){

        }
        else{
        static char buf[64];
        lv_snprintf(buf, sizeof(buf), "%d", arcValues[3]);
        const char * texts[] = {buf}; 
        
        lv_arc_set_value(arc4, arcValues[3]);
        lv_label_set_text(arcText[3], buf);
        lv_obj_align(arcText[3], NULL, LV_ALIGN_CENTER, 0, 0);

        arcChecks[3] = arcValues[3];
        }
      }else{
        if (arcCCVal[3] == arcCCChe[3]){ //Value of Arc1
          }
          else{
          static char buf[64];
          lv_snprintf(buf, sizeof(buf), "%d", arcCCVal[3]);
          const char * texts[] = {buf}; 

          lv_arc_set_value(arc4, arcCCVal[3]);
          lv_label_set_text(arcText[3], buf);
          lv_obj_align(arcText[3], NULL, LV_ALIGN_CENTER, 0, 0);
          //lv_label_set_align(arc1Text[0], LV_LABEL_ALIGN_CENTER);

          arcCCChe[3] = arcCCVal[3];
          }
      }

        //Arc5

      if(arcBool[4] == 0){
        if (arcValues[4] == arcChecks[4]){

        }
        else{
        static char buf[64];
        lv_snprintf(buf, sizeof(buf), "%d", arcValues[4]);
        const char * texts[] = {buf}; 
        
        lv_arc_set_value(arc5, arcValues[4]);
        lv_label_set_text(arcText[4], buf);
        lv_obj_align(arcText[4], NULL, LV_ALIGN_CENTER, 0, 0);

        arcChecks[4] = arcValues[4];
        }
      }else{
        if (arcCCVal[4] == arcCCChe[4]){ //Value of Arc1
          }
          else{
          static char buf[64];
          lv_snprintf(buf, sizeof(buf), "%d", arcCCVal[4]);
          const char * texts[] = {buf}; 

          lv_arc_set_value(arc5, arcCCVal[4]);
          lv_label_set_text(arcText[4], buf);
          lv_obj_align(arcText[4], NULL, LV_ALIGN_CENTER, 0, 0);
          //lv_label_set_align(arc1Text[0], LV_LABEL_ALIGN_CENTER);

          arcCCChe[4] = arcCCVal[4];
          }
      }


      //Arc6

      if(arcBool[5] == 0){
        if (arcValues[5] == arcChecks[5]){

        }
        else{
        static char buf[64];
        lv_snprintf(buf, sizeof(buf), "%d", arcValues[5]);
        const char * texts[] = {buf}; 
        
        lv_arc_set_value(arc6, arcValues[5]);
        lv_label_set_text(arcText[5], buf);
        lv_obj_align(arcText[5], NULL, LV_ALIGN_CENTER, 0, 0);

        arcChecks[5] = arcValues[5];
        }
      }else{
        if (arcCCVal[5] == arcCCChe[5]){ //Value of Arc1
          }
          else{
          static char buf[64];
          lv_snprintf(buf, sizeof(buf), "%d", arcCCVal[5]);
          const char * texts[] = {buf}; 

          lv_arc_set_value(arc6, arcCCVal[5]);
          lv_label_set_text(arcText[5], buf);
          lv_obj_align(arcText[5], NULL, LV_ALIGN_CENTER, 0, 0);
          //lv_label_set_align(arc1Text[0], LV_LABEL_ALIGN_CENTER);

          arcCCChe[5] = arcCCVal[5];
          }
      }


      //Arc7

      if(arcBool[6] == 0){
        if (arcValues[6] == arcChecks[6]){

        }
        else{
        static char buf[64];
        lv_snprintf(buf, sizeof(buf), "%d", arcValues[6]);
        const char * texts[] = {buf}; 
        
        lv_arc_set_value(arc7, arcValues[6]);
        lv_label_set_text(arcText[6], buf);
        lv_obj_align(arcText[6], NULL, LV_ALIGN_CENTER, 0, 0);

        arcChecks[6] = arcValues[6];
        }
      }else{
        if (arcCCVal[6] == arcCCChe[6]){ //Value of Arc1
          }
          else{
          static char buf[64];
          lv_snprintf(buf, sizeof(buf), "%d", arcCCVal[6]);
          const char * texts[] = {buf}; 

          lv_arc_set_value(arc7, arcCCVal[6]);
          lv_label_set_text(arcText[6], buf);
          lv_obj_align(arcText[6], NULL, LV_ALIGN_CENTER, 0, 0);
          //lv_label_set_align(arc1Text[0], LV_LABEL_ALIGN_CENTER);

          arcCCChe[6] = arcCCVal[6];
          }
      }

      //Arc8

      if(arcBool[7] == 0){
        if (arcValues[7] == arcChecks[7]){

        }
        else{
        static char buf[64];
        lv_snprintf(buf, sizeof(buf), "%d", arcValues[7]);
        const char * texts[] = {buf}; 
        
        lv_arc_set_value(arc8, arcValues[7]);
        lv_label_set_text(arcText[7], buf);
        lv_obj_align(arcText[7], NULL, LV_ALIGN_CENTER, 0, 0);

        arcChecks[7] = arcValues[7];
        }
      }else{
        if (arcCCVal[7] == arcCCChe[7]){ //Value of Arc1

          }
          else{
          static char buf[64];
          lv_snprintf(buf, sizeof(buf), "%d", arcCCVal[7]);
          const char * texts[] = {buf}; 

          lv_arc_set_value(arc8, arcCCVal[7]);
          lv_label_set_text(arcText[7], buf);
          lv_obj_align(arcText[7], NULL, LV_ALIGN_CENTER, 0, 0);
          //lv_label_set_align(arc1Text[0], LV_LABEL_ALIGN_CENTER);

          arcCCChe[7] = arcCCVal[7];
          }
      }


    }

static void slider_anim(lv_task_t * t){
  if(mode == 2){
    lv_obj_reset_style_list(slider1, LV_BAR_PART_INDIC);
    lv_obj_add_style(slider1, LV_BAR_PART_INDIC, &slider_line_vel);

      if (vel == velCheck){

        }
      else{
        lv_bar_set_value(slider1, vel, LV_ANIM_OFF);
        velCheck = vel;
        static char buf[64];
        lv_snprintf(buf, sizeof(buf), "%d", vel);
        const char * texts[] = {buf}; 

        lv_label_set_text(sliderText[0], buf);
        lv_obj_align(sliderText[0], NULL, LV_ALIGN_CENTER, 0, 35);
      }
        lv_bar_set_value(slider2, slider2Val, LV_ANIM_OFF);
        slider2Check = slider2Val;
        static char buf[64];
        lv_snprintf(buf, sizeof(buf), "%d", slider2Val);
        const char * texts[] = {buf}; 

        lv_label_set_text(sliderText[1], buf);
        lv_obj_align(sliderText[1], NULL, LV_ALIGN_CENTER, 0, 75);
  }

  //Mode != 2
  else{
    printf("Mode 1");
    //lv_obj_reset_style_list(slider1, LV_BAR_PART_INDIC);
    if(sliderBool[0] == 0){
      lv_obj_add_style(slider1, LV_BAR_PART_INDIC, &slider_line1);

    }
    else{
      lv_obj_add_style(slider1, LV_BAR_PART_INDIC, &slider_line_cc);
    }

    if(sliderBool[0] == 0){
      if (slider1Val == slider1Check){

      } 
      else{
          lv_bar_set_value(slider1, slider1Val, LV_ANIM_OFF);
          slider1Check = slider1Val;
          static char buf[64];
          lv_snprintf(buf, sizeof(buf), "%d", slider1Val);
          const char * texts[] = {buf}; 

          lv_label_set_text(sliderText[0], buf);
          lv_obj_align(sliderText[0], NULL, LV_ALIGN_CENTER, 0, 35);
        }
      }
    else{
      if (slider1CC == slider1CCCh){

      } 
      else{
          lv_bar_set_value(slider1, slider1CC, LV_ANIM_OFF);
          slider1CCCh = slider1CC;
          static char buf[64];
          lv_snprintf(buf, sizeof(buf), "%d", slider1CC);
          const char * texts[] = {buf}; 

          lv_label_set_text(sliderText[0], buf);
          lv_obj_align(sliderText[0], NULL, LV_ALIGN_CENTER, 0, 35);
        }
    }

    if(sliderBool[1] == 0){
      if (slider2Val == slider2Check){

      } 
      else{
          lv_bar_set_value(slider2, slider2Val, LV_ANIM_OFF);
          slider2Check = slider2Val;
          static char buf[64];
          lv_snprintf(buf, sizeof(buf), "%d", slider2Val);
          const char * texts[] = {buf}; 

          lv_label_set_text(sliderText[1], buf);
          lv_obj_align(sliderText[1], NULL, LV_ALIGN_CENTER, 0, 75);
        }
      }
    else{
      if (slider2CC == slider2CCCh){

      } 
      else{
          lv_bar_set_value(slider2, slider2CC, LV_ANIM_OFF);
          slider2CCCh = slider2CC;
          static char buf[64];
          lv_snprintf(buf, sizeof(buf), "%d", slider2CC);
          const char * texts[] = {buf}; 

          lv_label_set_text(sliderText[1], buf);
          lv_obj_align(sliderText[1], NULL, LV_ALIGN_CENTER, 0, 75);
        }
    }

    }
  }

/*********************
 ****** Screens ******
 *********************/
static void stats_create(lv_obj_t * parent)
  {
    lv_page_set_scrl_layout(parent, LV_LAYOUT_OFF);
    lv_disp_size_t disp_size = lv_disp_get_size_category(NULL);

    //BPM
    bpm_label[0] = lv_label_create(parent, NULL);
    lv_obj_align(bpm_label[0], NULL, LV_ALIGN_IN_TOP_LEFT, 10, 10);
    //lv_obj_set_style_local_text_font(bpm_label[0], NULL, NULL, &novamono_48);
    lv_obj_add_style(bpm_label[0], LV_LABEL_PART_MAIN, &textBoxStyle);

    //Bank
    bank_label[0] = lv_label_create(parent, NULL);
    lv_obj_align(bank_label[0], NULL, LV_ALIGN_IN_TOP_LEFT, 10, 70);
    //lv_obj_set_style_local_text_font(bpm_label[0], NULL, NULL, &novamono_28);
    lv_obj_add_style(bank_label[0], LV_LABEL_PART_MAIN, &textBoxStyle);

    //Track
    track_label[0] = lv_label_create(parent, NULL);
    lv_obj_align(track_label[0], NULL, LV_ALIGN_IN_TOP_LEFT, 10, 130);
    //lv_obj_set_style_local_text_font(bpm_label[0], NULL, NULL, &novamono_28);
    lv_obj_add_style(track_label[0], LV_LABEL_PART_MAIN, &textBoxStyle);

    //Mode
    mode_label[0] = lv_label_create(parent, NULL);
    lv_obj_align(mode_label[0], NULL, LV_ALIGN_IN_TOP_LEFT, 270, 10);
    //lv_obj_set_style_local_text_font(bpm_label[0], NULL, NULL, &novamono_28);
    lv_obj_add_style(mode_label[0], LV_LABEL_PART_MAIN, &textBoxStyle2);

    //Velocity
    velocity_label[0] = lv_label_create(parent, NULL);
    lv_obj_align(velocity_label[0], NULL, LV_ALIGN_IN_TOP_LEFT, 163, 70);
    //lv_obj_set_style_local_text_font(bpm_label[0], NULL, NULL, &novamono_28);
    lv_obj_add_style(velocity_label[0], LV_LABEL_PART_MAIN, &textBoxStyle);

  /*
    lv_obj_t * btn_bpm = lv_btn_create(parent, NULL);     //Add a button the current screen
        lv_obj_set_pos(btn_bpm, 200, 30);                            //Set its position
    lv_obj_set_size(btn_bpm, 40, 30);                          //Set its size
    lv_obj_set_event_cb(btn_bpm, btn_bpm_e); 

    lv_obj_t * btn_bank = lv_btn_create(parent, NULL);     //Add a button the current screen
    lv_obj_set_pos(btn_bank, 200, 90);                            //Set its position
    lv_obj_set_size(btn_bank, 40, 30);                          //Set its size
    lv_obj_set_event_cb(btn_bank, btn_bank_pos_e); 

    lv_obj_t * btn_bank_neg = lv_btn_create(parent, NULL);     //Add a button the current screen
    lv_obj_set_pos(btn_bank_neg, 200, 140);                            //Set its position
    lv_obj_set_size(btn_bank_neg, 40, 30);                          //Set its size
    lv_obj_set_event_cb(btn_bank_neg, btn_bank_neg_e); 

    lv_obj_t * octave_btn = lv_btn_create(parent, NULL);     //Add a button the current screen
        lv_obj_set_pos(octave_btn, 250, 80);                            //Set its position    
        lv_obj_set_size(octave_btn, 40, 30);                          //Set its size
    lv_obj_set_event_cb(octave_btn, btn_octave_e); 
  */

    roller = lv_roller_create(parent, NULL);
    lv_roller_set_options(roller,
                    "C0 - C2\n"
                    "C1 - C3\n"
                    "C2 - C4\n"
                    "C3 - C5\n"
                    "C4 - C6\n"
                    "C5 - C7\n"
                    "C6 - C8",
                    LV_ROLLER_MODE_NORMAL);
    lv_obj_add_style(roller, LV_ROLLER_PART_BG, &rollerStyle);
    lv_obj_add_style(roller, LV_ROLLER_PART_SELECTED, &rollerStyle);
    lv_obj_set_style_local_value_str(roller, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, "");
    lv_roller_set_auto_fit(roller, false);
    lv_obj_align(roller, NULL, LV_ALIGN_IN_TOP_LEFT, 170, 130);
    lv_roller_set_visible_row_count(roller, 1);
    lv_obj_set_width(roller, 120);
    lv_obj_set_event_cb(roller, rollerEH);  


    lv_task_t * t = lv_task_create(bpm_updater, 100, LV_TASK_PRIO_MID, NULL);
    lv_task_ready(t);  
    lv_task_t * u = lv_task_create(bank_updater, 100, LV_TASK_PRIO_MID, NULL);
    lv_task_ready(u); 
    lv_task_t * v = lv_task_create(track_updater, 100, LV_TASK_PRIO_MID, NULL);
    lv_task_ready(v);  
    lv_task_t * w = lv_task_create(mode_updater, 100, LV_TASK_PRIO_MID, NULL);
    lv_task_ready(w);  
    lv_task_t * x = lv_task_create(octave_updater, 100, LV_TASK_PRIO_MID, NULL);
    lv_task_ready(x);  
    lv_task_t * y = lv_task_create(velocity_updater, 100, LV_TASK_PRIO_MID, NULL);
    lv_task_ready(y);  
  }

static void visuals_create(lv_obj_t * parent)
  {
    lv_page_set_scrl_layout(parent, LV_LAYOUT_OFF);

    lv_disp_size_t disp_size = lv_disp_get_size_category(NULL);

    //Button for testing
    lv_obj_t * arc1_btn = lv_btn_create(parent, NULL);     //Add a button the current screen
    lv_obj_set_pos(arc1_btn, 20, 130);                            //Set its position
    lv_obj_set_size(arc1_btn, 80, 50);                          //Set its size
    lv_obj_set_event_cb(arc1_btn, arc1_btn_e); 

    //Button for testing CC
    lv_obj_t * arc1_btn2 = lv_btn_create(parent, NULL);     //Add a button the current screen
    lv_obj_set_pos(arc1_btn2, 100, 130);                            //Set its position
    lv_obj_set_size(arc1_btn2, 80, 50);                          //Set its size
    lv_obj_set_event_cb(arc1_btn2, arc1_btn2_e);

    //Button for testing vel
    lv_obj_t * arc1_btn3 = lv_btn_create(parent, NULL);     //Add a button the current screen
    lv_obj_set_pos(arc1_btn3, 180, 130);                            //Set its position
    lv_obj_set_size(arc1_btn3, 80, 50);                          //Set its size
    lv_obj_set_event_cb(arc1_btn3, arc1_btn3_e);

    //Button for changing mode
    lv_obj_t * arc1_btn4 = lv_btn_create(parent, NULL);     //Add a button the current screen
    lv_obj_set_pos(arc1_btn4, 180, 80);                            //Set its position
    lv_obj_set_size(arc1_btn4, 80, 50);                          //Set its size
    lv_obj_set_event_cb(arc1_btn4, arc1_btn4_e);

    //Button for changing slider to CC
    lv_obj_t * arc1_btn5 = lv_btn_create(parent, NULL);     //Add a button the current screen
    lv_obj_set_pos(arc1_btn5, 180, 30);                            //Set its position
    lv_obj_set_size(arc1_btn5, 80, 50);                          //Set its size
    lv_obj_set_event_cb(arc1_btn5, arc1_btn5_e);
  }

static void save_create(lv_obj_t * parent)
  {
    lv_page_set_scrl_layout(parent, LV_LAYOUT_OFF);
    lv_disp_size_t disp_size = lv_disp_get_size_category(NULL);

    listLeft = lv_list_create(parent, NULL);

    lv_obj_add_style(listLeft, LV_STATE_DEFAULT, &bgBoxStyle);
    lv_obj_set_size(listLeft, 120,145);
    lv_obj_align(listLeft, NULL, LV_ALIGN_CENTER, -80,0);    

    listPreset1 = lv_list_add_btn(listLeft, NULL, "PRESET 1");
    listPreset2 = lv_list_add_btn(listLeft, NULL, "PRESET 2");
    listPreset3 = lv_list_add_btn(listLeft, NULL, "PRESET 3");
    listPreset4 = lv_list_add_btn(listLeft, NULL, "PRESET 4");
    listPreset5 = lv_list_add_btn(listLeft, NULL, "PRESET 5");
    listPreset6 = lv_list_add_btn(listLeft, NULL, "PRESET 6");
    listPreset7 = lv_list_add_btn(listLeft, NULL, "PRESET 7");
    listPreset8 = lv_list_add_btn(listLeft, NULL, "PRESET 8");

    listRight = lv_list_create(parent, NULL);

    lv_obj_add_style(listRight, LV_STATE_DEFAULT, &bgBoxStyle);
    lv_obj_set_size(listRight, 120,145);
    lv_obj_align(listRight, NULL, LV_ALIGN_CENTER, 80,0);

    listBtnSave = lv_list_add_btn(listRight, &save, "SAVE");
    listBtnLoad = lv_list_add_btn(listRight, &load, "LOAD");
    listBtnClear = lv_list_add_btn(listRight, &clear, "CLEAR");

    lv_obj_add_style(listPreset1, LV_STATE_DEFAULT, &listBtnStyle);
    lv_obj_add_style(listPreset2, LV_STATE_DEFAULT, &listBtnStyle);
    lv_obj_add_style(listPreset3, LV_STATE_DEFAULT, &listBtnStyle);
    lv_obj_add_style(listPreset4, LV_STATE_DEFAULT, &listBtnStyle);
    lv_obj_add_style(listPreset5, LV_STATE_DEFAULT, &listBtnStyle);
    lv_obj_add_style(listPreset6, LV_STATE_DEFAULT, &listBtnStyle);
    lv_obj_add_style(listPreset7, LV_STATE_DEFAULT, &listBtnStyle);
    lv_obj_add_style(listPreset8, LV_STATE_DEFAULT, &listBtnStyle);

    lv_obj_add_style(listBtnSave, LV_STATE_DEFAULT, &listBtnStyle2);
    lv_obj_add_style(listBtnLoad, LV_STATE_DEFAULT, &listBtnStyle2);
    lv_obj_add_style(listBtnClear, LV_STATE_DEFAULT, &listBtnStyle2);

    lv_btn_set_checkable(listPreset1, true);
    lv_btn_set_checkable(listPreset2, true);
    lv_btn_set_checkable(listPreset3, true);
    lv_btn_set_checkable(listPreset4, true);
    lv_btn_set_checkable(listPreset5, true);
    lv_btn_set_checkable(listPreset6, true);
    lv_btn_set_checkable(listPreset7, true);
    lv_btn_set_checkable(listPreset8, true);
    //lv_btn_set_checkable(listBtnSave, true);
    //lv_btn_set_checkable(listBtnLoad, true);
    //lv_btn_set_checkable(listBtnClear, true);

    lv_obj_set_event_cb(listPreset1, listPreset1_e); 
    lv_obj_set_event_cb(listPreset2, listPreset2_e); 
    lv_obj_set_event_cb(listPreset3, listPreset3_e); 
    lv_obj_set_event_cb(listPreset4, listPreset4_e); 
    lv_obj_set_event_cb(listPreset5, listPreset5_e); 
    lv_obj_set_event_cb(listPreset6, listPreset6_e); 
    lv_obj_set_event_cb(listPreset7, listPreset7_e); 
    lv_obj_set_event_cb(listPreset8, listPreset8_e); 

    lv_obj_set_event_cb(listBtnSave, savePreset_e); 
    lv_obj_set_event_cb(listBtnLoad, loadPreset_e); 
    lv_obj_set_event_cb(listBtnClear, clearPreset_e); 



  }

static void controls_create(lv_obj_t * parent)
  {
    lv_page_set_scrl_layout(parent, LV_LAYOUT_OFF);

    lv_disp_size_t disp_size = lv_disp_get_size_category(NULL);

    int row1 = 0;
    int row2 = 60;
    int col1 = 12;
    int col2 = 83;
    int col3 = 155;
    int col4 = 226;

    //Row1 *******

    // ARC1
    arc1 = lv_arc_create(parent, NULL);
    lv_obj_set_drag_parent(arc1, false);
    lv_arc_set_bg_angles(arc1, 0, 270);
    lv_arc_set_rotation(arc1, 135);
    lv_arc_set_angles(arc1, 0, 270);
    lv_obj_set_size(arc1,  80, 80);
    lv_arc_set_range(arc1, 0, 127);

    lv_obj_align(arc1, NULL, LV_ALIGN_IN_TOP_LEFT, col1, row1);
    //lv_obj_add_style(arc1, LV_ARC_PART_BG, &arc_line);
    lv_obj_add_style(arc1, LV_ARC_PART_BG, &arc_line);
    lv_obj_add_style(arc1, LV_ARC_PART_INDIC, &arc_line_indic);

    arcText[0] = lv_label_create(arc1, NULL);
    lv_label_set_text(arcText[0], " 0 ");
    lv_obj_align(arcText[0], NULL, LV_ALIGN_CENTER, -6, 0);
    lv_obj_set_width(arcText[0], 50);
    lv_obj_add_style(arcText[0], LV_STATE_DEFAULT, &arcTextStyle);
    lv_label_set_align(arcText[0], LV_LABEL_ALIGN_CENTER);
    //lv_label_set_long_mode(arc1Text[0], LV_LABEL_LONG_BREAK);

    lv_obj_set_event_cb(arc1, arc1_e); 


    // ARC2
    arc2 = lv_arc_create(parent, NULL);
    lv_obj_set_drag_parent(arc2, false);
    lv_arc_set_bg_angles(arc2, 0, 270);
    lv_arc_set_rotation(arc2, 135);
    lv_arc_set_angles(arc2, 0, 0);
    lv_obj_set_size(arc2,  80, 80);
    lv_arc_set_range(arc2, 0, 127);


    lv_obj_align(arc2, NULL, LV_ALIGN_IN_TOP_LEFT, col2, row1);
    //lv_obj_add_style(arc1, LV_ARC_PART_MAIN, &arc_line);
    lv_obj_add_style(arc2, LV_ARC_PART_BG, &arc_line);
    lv_obj_add_style(arc2, LV_ARC_PART_INDIC, &arc_line_indic);

    arcText[1] = lv_label_create(arc2, NULL);
    lv_label_set_text(arcText[1], " 0 ");
    lv_obj_align(arcText[1], NULL, LV_ALIGN_CENTER, -6, 0);
    lv_obj_set_width(arcText[1], 50);
    lv_obj_add_style(arcText[1], LV_STATE_DEFAULT, &arcTextStyle);
    lv_label_set_align(arcText[1], LV_LABEL_ALIGN_CENTER);
    //lv_label_set_long_mode(arc1Text[0], LV_LABEL_LONG_BREAK);

        lv_obj_set_event_cb(arc2, arc2_e); 


    // ARC3
    arc3 = lv_arc_create(parent, NULL);
    lv_obj_set_drag_parent(arc3, false);
    lv_arc_set_bg_angles(arc3, 0, 270);
    lv_arc_set_rotation(arc3, 135);
    lv_arc_set_angles(arc3, 0, 0);
    lv_obj_set_size(arc3,  80, 80);
    lv_arc_set_range(arc3, 0, 127);

    lv_obj_align(arc3, NULL, LV_ALIGN_IN_TOP_LEFT, col3, row1);
    //lv_obj_add_style(arc1, LV_ARC_PART_MAIN, &arc_line);
    lv_obj_add_style(arc3, LV_ARC_PART_BG, &arc_line);
    lv_obj_add_style(arc3, LV_ARC_PART_INDIC, &arc_line_indic);

    arcText[2] = lv_label_create(arc3, NULL);
    lv_label_set_text(arcText[2], " 0 ");
    lv_obj_align(arcText[2], NULL, LV_ALIGN_CENTER, -6, 0);
    lv_obj_set_width(arcText[2], 50);
    lv_obj_add_style(arcText[2], LV_STATE_DEFAULT, &arcTextStyle);
    lv_label_set_align(arcText[2], LV_LABEL_ALIGN_CENTER);
    //lv_label_set_long_mode(arc1Text[0], LV_LABEL_LONG_BREAK);

    lv_obj_set_event_cb(arc3, arc3_e); 


    // ARC4
    arc4 = lv_arc_create(parent, NULL);
    lv_obj_set_drag_parent(arc4, false);
    lv_arc_set_bg_angles(arc4, 0, 270);
    lv_arc_set_rotation(arc4, 135);
    lv_arc_set_angles(arc4, 0, 0);
    lv_obj_set_size(arc4,  80, 80);
    lv_arc_set_range(arc4, 0, 127);

    lv_obj_align(arc4, NULL, LV_ALIGN_IN_TOP_LEFT, col4, row1);
    //lv_obj_add_style(arc1, LV_ARC_PART_MAIN, &arc_line);
    lv_obj_add_style(arc4, LV_ARC_PART_BG, &arc_line);
    lv_obj_add_style(arc4, LV_ARC_PART_INDIC, &arc_line_indic);

    arcText[3] = lv_label_create(arc4, NULL);
    lv_label_set_text(arcText[3], " 0 ");
    lv_obj_align(arcText[3], NULL, LV_ALIGN_CENTER, -6, 0);
    lv_obj_set_width(arcText[3], 50);
    lv_obj_add_style(arcText[3], LV_STATE_DEFAULT, &arcTextStyle);
    lv_label_set_align(arcText[3], LV_LABEL_ALIGN_CENTER);
    //lv_label_set_long_mode(arc1Text[0], LV_LABEL_LONG_BREAK);

    lv_obj_set_event_cb(arc4, arc4_e); 

    //Row1 *******
    // ARC5
    arc5 = lv_arc_create(parent, NULL);
    lv_obj_set_drag_parent(arc5, false);
    lv_arc_set_bg_angles(arc5, 0, 270);
    lv_arc_set_rotation(arc5, 135);
    lv_arc_set_angles(arc5, 0, 0);
    lv_obj_set_size(arc5,  80, 80);
    lv_arc_set_range(arc5, 0, 127);

    lv_obj_align(arc5, NULL, LV_ALIGN_IN_TOP_LEFT, col1, row2);
    //lv_obj_add_style(arc1, LV_ARC_PART_MAIN, &arc_line);
    lv_obj_add_style(arc5, LV_ARC_PART_BG, &arc_line);
    lv_obj_add_style(arc5, LV_ARC_PART_INDIC, &arc_line_indic);

    arcText[4] = lv_label_create(arc5, NULL);
    lv_label_set_text(arcText[4], " 0 ");
    lv_obj_align(arcText[4], NULL, LV_ALIGN_CENTER, -6, 0);
    lv_obj_set_width(arcText[4], 50);
    lv_obj_add_style(arcText[4], LV_STATE_DEFAULT, &arcTextStyle);
    lv_label_set_align(arcText[4], LV_LABEL_ALIGN_CENTER);

    lv_obj_set_event_cb(arc5, arc5_e); 

    // ARC6
    arc6 = lv_arc_create(parent, NULL);
    lv_obj_set_drag_parent(arc6, false);
    lv_arc_set_bg_angles(arc6, 0, 270);
    lv_arc_set_rotation(arc6, 135);
    lv_arc_set_angles(arc6, 0, 0);
    lv_obj_set_size(arc6,  80, 80);
    lv_arc_set_range(arc6, 0, 127);

    lv_obj_align(arc6, NULL, LV_ALIGN_IN_TOP_LEFT, col2, row2);
    //lv_obj_add_style(arc1, LV_ARC_PART_MAIN, &arc_line);
    lv_obj_add_style(arc6, LV_ARC_PART_BG, &arc_line);
    lv_obj_add_style(arc6, LV_ARC_PART_INDIC, &arc_line_indic);

    arcText[5] = lv_label_create(arc6, NULL);
    lv_label_set_text(arcText[5], " 0 ");
    lv_obj_align(arcText[5], NULL, LV_ALIGN_CENTER, -6, 0);
    lv_obj_set_width(arcText[5], 50);
    lv_obj_add_style(arcText[5], LV_STATE_DEFAULT, &arcTextStyle);
    lv_label_set_align(arcText[5], LV_LABEL_ALIGN_CENTER);

    lv_obj_set_event_cb(arc6, arc6_e); 

    // ARC7
    arc7 = lv_arc_create(parent, NULL);
    lv_obj_set_drag_parent(arc7, false);
    lv_arc_set_bg_angles(arc7, 0, 270);
    lv_arc_set_rotation(arc7, 135);
    lv_arc_set_angles(arc7, 0, 0);
    lv_obj_set_size(arc7,  80, 80);
    lv_arc_set_range(arc7, 0, 127);

    lv_obj_align(arc7, NULL, LV_ALIGN_IN_TOP_LEFT, col3, row2);
    //lv_obj_add_style(arc1, LV_ARC_PART_MAIN, &arc_line);
    lv_obj_add_style(arc7, LV_ARC_PART_BG, &arc_line);
    lv_obj_add_style(arc7, LV_ARC_PART_INDIC, &arc_line_indic);

    arcText[6] = lv_label_create(arc7, NULL);
    lv_label_set_text(arcText[6], " 0 ");
    lv_obj_align(arcText[6], NULL, LV_ALIGN_CENTER, -6, 0);
    lv_obj_set_width(arcText[6], 50);
    lv_obj_add_style(arcText[6], LV_STATE_DEFAULT, &arcTextStyle);
    lv_label_set_align(arcText[6], LV_LABEL_ALIGN_CENTER);

    lv_obj_set_event_cb(arc7, arc7_e); 

    // ARC8
    arc8 = lv_arc_create(parent, NULL);
    lv_obj_set_drag_parent(arc8, false);
    lv_arc_set_bg_angles(arc8, 0, 270);
    lv_arc_set_rotation(arc8, 135);
    lv_arc_set_angles(arc8, 0, 0);  
    lv_obj_set_size(arc8,  80, 80);
    lv_arc_set_range(arc8, 0, 127);

    lv_obj_align(arc8, NULL, LV_ALIGN_IN_TOP_LEFT, col4, row2);
    //lv_obj_add_style(arc1, LV_ARC_PART_MAIN, &arc_line);
    lv_obj_add_style(arc8, LV_ARC_PART_BG, &arc_line);
    lv_obj_add_style(arc8, LV_ARC_PART_INDIC, &arc_line_indic);

    arcText[7] = lv_label_create(arc8, NULL);
    lv_label_set_text(arcText[7], " 0 ");
    lv_obj_align(arcText[7], NULL, LV_ALIGN_CENTER, -6, 0);
    lv_obj_set_width(arcText[7], 50);
    lv_obj_add_style(arcText[7], LV_STATE_DEFAULT, &arcTextStyle);
    lv_label_set_align(arcText[7], LV_LABEL_ALIGN_CENTER);

    lv_obj_set_event_cb(arc8, arc8_e); 

/*
    //Button for testing
    lv_obj_t * arc1_btn = lv_btn_create(parent, NULL);     //Add a button the current screen
    lv_obj_set_pos(arc1_btn, 20, 130);                            //Set its position
    lv_obj_set_size(arc1_btn, 80, 50);                          //Set its size
    lv_obj_set_event_cb(arc1_btn, arc1_btn_e); 

    //Button for testing CC
    lv_obj_t * arc1_btn2 = lv_btn_create(parent, NULL);     //Add a button the current screen
    lv_obj_set_pos(arc1_btn2, 100, 130);                            //Set its position
    lv_obj_set_size(arc1_btn2, 80, 50);                          //Set its size
    lv_obj_set_event_cb(arc1_btn2, arc1_btn2_e); 
*/


    //SLIDER 1
    slider1 = lv_bar_create(parent, NULL);
    //lv_obj_add_style(slider1, LV_BAR_PART_BG, &slider_line);
    //lv_obj_add_style(slider1, LV_BAR_PART_INDIC, &slider_line1);
    lv_obj_align(slider1, NULL, LV_ALIGN_IN_TOP_LEFT, 10, 140);
    lv_obj_set_size(slider1, 296, 5);
    lv_bar_set_range(slider1, 0, 127);

    lv_obj_set_event_cb(slider1, slider1_e); 


    //SLIDER 2
    slider2 = lv_bar_create(parent, NULL);
    //lv_obj_add_style(slider2, LV_BAR_PART_BG, &slider_line);
    //lv_obj_add_style(slider2, LV_BAR_PART_INDIC, &slider_line1);
    lv_obj_align(slider2, NULL, LV_ALIGN_IN_TOP_LEFT, 10, 150);
    lv_obj_set_size(slider2, 296, 5);
    lv_bar_set_range(slider2, 0, 127);

    lv_obj_set_event_cb(slider2, slider2_e); 

    sliderText[0] = lv_label_create(parent, NULL);
    sliderText[1] = lv_label_create(parent, NULL);

    lv_label_set_text(sliderText[0], " 0 ");
    lv_label_set_text(sliderText[1], " 0 ");    
    lv_obj_add_style(sliderText[0], LV_STATE_DEFAULT, &arcTextStyle);
    lv_obj_add_style(sliderText[1], LV_STATE_DEFAULT, &arcTextStyle);

    lv_obj_align(sliderText[0], NULL, LV_ALIGN_CENTER, 0, 35);
    lv_label_set_align(sliderText[0], LV_LABEL_ALIGN_CENTER);
    lv_obj_align(sliderText[1], NULL, LV_ALIGN_CENTER, 0, 75);
    lv_label_set_align(sliderText[1], LV_LABEL_ALIGN_CENTER);

    //Tasks
    lv_task_create(arc_anim, 100, LV_TASK_PRIO_LOW, NULL);
    lv_task_create(slider_anim, 100, LV_TASK_PRIO_LOW, NULL);

  }

static void selectors_create(lv_obj_t * parent)
  {
    lv_page_set_scrl_layout(parent, LV_LAYOUT_OFF);

    lv_disp_size_t disp_size = lv_disp_get_size_category(NULL);
    lv_coord_t grid_h = lv_page_get_height_grid(parent, 1, 1);
    lv_coord_t grid_w;
    if(disp_size <= LV_DISP_SIZE_SMALL) grid_w = lv_page_get_width_grid(parent, 1, 1);
    else grid_w = lv_page_get_width_grid(parent, 2, 1);

    lv_obj_t * cal = lv_calendar_create(parent, NULL);
    lv_obj_set_drag_parent(cal, true);
    if(disp_size > LV_DISP_SIZE_MEDIUM) {
        lv_obj_set_size(cal, LV_MATH_MIN(grid_h, grid_w), LV_MATH_MIN(grid_h, grid_w));
    } else {
        lv_obj_set_size(cal, grid_w, grid_w);
        if(disp_size <= LV_DISP_SIZE_SMALL) {
            lv_obj_set_style_local_text_font(cal, LV_CALENDAR_PART_BG, LV_STATE_DEFAULT, lv_theme_get_font_small());
        }
    }

    static lv_calendar_date_t hl[] = {
            {.year = 2020, .month = 1, .day = 5},
            {.year = 2020, .month = 1, .day = 9},
    };


    lv_obj_t * btn;
    lv_obj_t * h = lv_cont_create(parent, NULL);
    lv_obj_set_drag_parent(h, true);
    if(disp_size <= LV_DISP_SIZE_SMALL) {
        lv_cont_set_fit2(h, LV_FIT_NONE, LV_FIT_TIGHT);
        lv_obj_set_width(h, lv_page_get_width_fit(parent));
        lv_cont_set_layout(h, LV_LAYOUT_COLUMN_MID);
    } else if(disp_size <= LV_DISP_SIZE_MEDIUM) {
        lv_obj_set_size(h, lv_obj_get_width(cal), lv_obj_get_height(cal));
        lv_cont_set_layout(h, LV_LAYOUT_PRETTY_TOP);
    } else {
        lv_obj_set_click(h, false);
        lv_obj_set_style_local_bg_opa(h, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);
        lv_obj_set_style_local_border_opa(h, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);
        lv_obj_set_style_local_pad_left(h, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
        lv_obj_set_style_local_pad_right(h, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
        lv_obj_set_style_local_pad_top(h, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
        lv_obj_set_size(h, LV_MATH_MIN(grid_h, grid_w), LV_MATH_MIN(grid_h, grid_w));
        lv_cont_set_layout(h, LV_LAYOUT_PRETTY_TOP);
    }


    lv_obj_t * roller = lv_roller_create(h, NULL);
    lv_roller_set_options(roller,
                    "C0 - C2\n"
                    "C1 - C3\n"
                    "C2 - C4\n"
                    "C3 - C5\n"
                    "C4 - C6\n"
                    "C5 - C7\n"
                    "C6 - C8\n",
                    LV_ROLLER_MODE_NORMAL);
    lv_obj_add_style(roller, LV_CONT_PART_MAIN, &textBoxStyle);
    lv_obj_set_style_local_value_str(roller, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, "Roller");
    lv_roller_set_auto_fit(roller, false);
    lv_roller_set_align(roller, LV_LABEL_ALIGN_CENTER);
    lv_roller_set_visible_row_count(roller, 1);
    lv_obj_set_width(roller, 120); 
    lv_roller_set_selected(roller, rollerIndex, LV_ANIM_OFF);

    lv_obj_set_event_cb(roller, rollerEH);  
    rollerIndex = lv_roller_get_selected(roller);

    lv_obj_t * dd = lv_dropdown_create(h, NULL);
    lv_obj_add_style(dd, LV_CONT_PART_MAIN, &style_box);
    lv_obj_set_style_local_value_str(dd, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, "Dropdown");
    lv_obj_set_width(dd, lv_obj_get_width_grid(h, disp_size <= LV_DISP_SIZE_SMALL ? 1 : 2, 1));
    lv_dropdown_set_options(dd, "Alpha\nBravo\nCharlie\nDelta\nEcho\nFoxtrot\nGolf\nHotel\nIndia\nJuliette\nKilo\nLima\nMike\nNovember\n"
            "Oscar\nPapa\nQuebec\nRomeo\nSierra\nTango\nUniform\nVictor\nWhiskey\nXray\nYankee\nZulu");

    lv_obj_t * list = lv_list_create(parent, NULL);
    lv_list_set_scroll_propagation(list, true);
    lv_obj_set_size(list, grid_w, grid_h);

    const char * txts[] = {LV_SYMBOL_SAVE, "Save", LV_SYMBOL_CUT, "Cut", LV_SYMBOL_COPY, "Copy",
            LV_SYMBOL_OK, "This is a substantially long text to scroll on the list", LV_SYMBOL_EDIT, "Edit", LV_SYMBOL_WIFI, "Wifi",
            LV_SYMBOL_BLUETOOTH, "Bluetooth",  LV_SYMBOL_GPS, "GPS", LV_SYMBOL_USB, "USB",
            LV_SYMBOL_SD_CARD, "SD card", LV_SYMBOL_CLOSE, "Close", NULL};

    uint32_t i;
    for(i = 0; txts[i] != NULL; i += 2) {
        btn = lv_list_add_btn(list, txts[i], txts[i + 1]);
        lv_btn_set_checkable(btn, true);

        /*Make a button disabled*/
        if(i == 4) {
            lv_btn_set_state(btn, LV_BTN_STATE_DISABLED);
        }
    }

    lv_calendar_set_highlighted_dates(cal, hl, 2);


    static lv_style_t style_cell4;
    lv_style_init(&style_cell4);
    lv_style_set_bg_opa(&style_cell4, LV_STATE_DEFAULT, LV_OPA_50);
    lv_style_set_bg_color(&style_cell4, LV_STATE_DEFAULT, LV_COLOR_GRAY);

    lv_obj_t * page = lv_page_create(parent ,NULL);
    lv_obj_set_size(page, grid_w, grid_h);
    lv_coord_t table_w_max = lv_page_get_width_fit(page);
    lv_page_set_scroll_propagation(page, true);

    lv_obj_t * table1 = lv_table_create(page, NULL);
    lv_obj_add_style(table1, LV_TABLE_PART_CELL4, &style_cell4);
    /*Clean the background style of the table because it is placed on a page*/
    lv_obj_clean_style_list(table1, LV_TABLE_PART_BG);
    lv_obj_set_drag_parent(table1, true);
    lv_obj_set_event_cb(table1, table_event_cb);
    lv_table_set_col_cnt(table1, disp_size > LV_DISP_SIZE_SMALL ? 3 : 2);
    if(disp_size > LV_DISP_SIZE_SMALL) {
        lv_table_set_col_width(table1, 0, LV_MATH_MAX(30, 1 * table_w_max  / 5));
        lv_table_set_col_width(table1, 1, LV_MATH_MAX(60, 2 * table_w_max / 5));
        lv_table_set_col_width(table1, 2, LV_MATH_MAX(60, 2 * table_w_max / 5));
    } else {
        lv_table_set_col_width(table1, 0, LV_MATH_MAX(30, 1 * table_w_max  / 4 - 1));
        lv_table_set_col_width(table1, 1, LV_MATH_MAX(60, 3 * table_w_max / 4 - 1));
    }

    lv_table_set_cell_value(table1, 0, 0, "#");
    lv_table_set_cell_value(table1, 1, 0, "1");
    lv_table_set_cell_value(table1, 2, 0, "2");
    lv_table_set_cell_value(table1, 3, 0, "3");
    lv_table_set_cell_value(table1, 4, 0, "4");
    lv_table_set_cell_value(table1, 5, 0, "5");
    lv_table_set_cell_value(table1, 6, 0, "6");

    lv_table_set_cell_value(table1, 0, 1, "NAME");
    lv_table_set_cell_value(table1, 1, 1, "Mark");
    lv_table_set_cell_value(table1, 2, 1, "Jacob");
    lv_table_set_cell_value(table1, 3, 1, "John");
    lv_table_set_cell_value(table1, 4, 1, "Emily");
    lv_table_set_cell_value(table1, 5, 1, "Ivan");
    lv_table_set_cell_value(table1, 6, 1, "George");

    if(disp_size > LV_DISP_SIZE_SMALL) {
        lv_table_set_cell_value(table1, 0, 2, "CITY");
        lv_table_set_cell_value(table1, 1, 2, "Moscow");
        lv_table_set_cell_value(table1, 2, 2, "New York");
        lv_table_set_cell_value(table1, 3, 2, "Oslo");
        lv_table_set_cell_value(table1, 4, 2, "London");
        lv_table_set_cell_value(table1, 5, 2, "Texas");
        lv_table_set_cell_value(table1, 6, 2, "Athen");
    }
  }

//Events
static void slider_event_cb(lv_obj_t * slider, lv_event_t e)
  {
    if(e == LV_EVENT_VALUE_CHANGED) {
        if(lv_slider_get_type(slider) == LV_SLIDER_TYPE_NORMAL) {
            static char buf[16];
            lv_snprintf(buf, sizeof(buf), "%d", lv_slider_get_value(slider));
            lv_obj_set_style_local_value_str(slider, LV_SLIDER_PART_KNOB, LV_STATE_DEFAULT, buf);
        } else {
            static char buf[32];
            lv_snprintf(buf, sizeof(buf), "%d-%d", lv_slider_get_left_value(slider), lv_slider_get_value(slider));
            lv_obj_set_style_local_value_str(slider, LV_SLIDER_PART_INDIC, LV_STATE_DEFAULT, buf);
        }

    }

  }

static void ta_event_cb(lv_obj_t * ta, lv_event_t e)
  {
    if(e == LV_EVENT_RELEASED) {
        if(kb == NULL) {
            lv_obj_set_height(tv, LV_VER_RES / 2);
            kb = lv_keyboard_create(lv_scr_act(), NULL);
            lv_obj_set_event_cb(kb, kb_event_cb);

            lv_indev_wait_release(lv_indev_get_act());
        }
        lv_textarea_set_cursor_hidden(ta, false);
        lv_page_focus(t1, lv_textarea_get_label(ta), LV_ANIM_ON);
        lv_keyboard_set_textarea(kb, ta);
    } else if(e == LV_EVENT_DEFOCUSED) {
        lv_textarea_set_cursor_hidden(ta, true);
    }
  }

static void kb_event_cb(lv_obj_t * _kb, lv_event_t e)
  {
    lv_keyboard_def_event_cb(kb, e);

    if(e == LV_EVENT_CANCEL) {
        if(kb) {
            lv_obj_set_height(tv, LV_VER_RES);
            lv_obj_del(kb);
            kb = NULL;
        }
    }
  }

static void bar_anim(lv_task_t * t)
  {
    static uint32_t x = 0;
    lv_obj_t * bar = t->user_data;

    static char buf[64];
    lv_snprintf(buf, sizeof(buf), "Linear Pot %d/%d", xVal, lv_bar_get_max_value(bar));
    lv_obj_set_style_local_value_str(bar, LV_BAR_PART_BG, LV_STATE_DEFAULT, buf);

    esp_err_t status2 = adc_vref_to_gpio(ADC_UNIT_1, GPIO_NUM_35);
    //esp_err_tadc1_config_channel_atten(adc1_channel_tchannel, adc_atten_tatten)

    //Configure ADC
    adc1_config_width(width2);
    adc1_config_channel_atten(channel1, atten);
    adc1_config_channel_atten(channel2, atten);
    adc1_config_channel_atten(channel3, atten);


    //Characterize ADC
    adc_chars2 = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_value_t val_type2 = esp_adc_cal_characterize(unit, atten, width2, DEFAULT_VREF, adc_chars2);

    //Continuously sample ADC
    adc_reading2 = 0;           
    //Multisampling
    for (int i = 0; i < NO_OF_SAMPLES; i++) {
        adc_reading2 += adc1_get_raw((adc1_channel_t)channel2);
    }
    adc_reading2 /= NO_OF_SAMPLES;
    printf("Raw: %.2d\n", adc_reading2);
    float xVal = roundf((((float)adc_reading2+1)/1000)*100)/10;
    xVal = ((xVal/41)*100);
    printf("X - %.4f\n", xVal);

    lv_bar_set_value(bar, xVal, LV_ANIM_OFF);
  }

static void linemeter_anim(lv_obj_t * linemeter, lv_anim_value_t value)
  {
    lv_linemeter_set_value(linemeter, value);

    static char buf[64];
    lv_snprintf(buf, sizeof(buf), "%d", value);
    lv_obj_t * label = lv_obj_get_child(linemeter, NULL);
    lv_label_set_text(label, buf);
    lv_obj_align(label, linemeter, LV_ALIGN_CENTER, 0, 0);
  }

static void gauge_anim(lv_obj_t * gauge, lv_anim_value_t value)
  {
    lv_gauge_set_value(gauge, 0, value);

    static char buf[64];
    lv_snprintf(buf, sizeof(buf), "%d", value);
    lv_obj_t * label = lv_obj_get_child(gauge, NULL);
    lv_label_set_text(label, buf);
    lv_obj_align(label, gauge, LV_ALIGN_IN_TOP_MID, 0, lv_obj_get_y(label));
  }

static void table_event_cb(lv_obj_t * table, lv_event_t e)
  {
    if(e == LV_EVENT_CLICKED) {
        uint16_t row;
        uint16_t col;
        lv_res_t r = lv_table_get_pressed_cell(table, &row, &col);
        if(r == LV_RES_OK && col && row) {  /*Skip the first row and col*/
            if(lv_table_get_cell_type(table, row, col) == 1) {
                lv_table_set_cell_type(table, row, col, 4);
            } else {
                lv_table_set_cell_type(table, row, col, 1);
            }
        }
    }
  }

#if LV_USE_THEME_MATERIAL
static void color_chg_event_cb(lv_obj_t * sw, lv_event_t e)
  {
    if(LV_THEME_DEFAULT_INIT != lv_theme_material_init) return;
    if(e == LV_EVENT_VALUE_CHANGED) {
        uint32_t flag = LV_THEME_MATERIAL_FLAG_LIGHT;
        if(lv_switch_get_state(sw)) flag = LV_THEME_MATERIAL_FLAG_DARK;

        LV_THEME_DEFAULT_INIT(lv_theme_get_color_primary(), lv_theme_get_color_secondary(),
                flag,
                lv_theme_get_font_small(), lv_theme_get_font_normal(), lv_theme_get_font_subtitle(), lv_theme_get_font_title());
    }
  }
#endif

#if LV_DEMO_WIDGETS_SLIDESHOW

static void tab_content_anim_create(lv_obj_t * parent)
{
    lv_anim_t a;
    lv_obj_t * scrl = lv_page_get_scrl(parent);
    lv_coord_t y_start = lv_obj_get_style_pad_top(parent, LV_PAGE_PART_BG);
    lv_coord_t anim_h = lv_obj_get_height(scrl) - lv_obj_get_height_fit(parent);
    uint32_t anim_time = lv_anim_speed_to_time(LV_DPI, 0, anim_h);

    lv_anim_init(&a);
    lv_anim_set_var(&a, scrl);
    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_obj_set_y);
    lv_anim_set_values(&a, y_start, y_start - anim_h);
    lv_anim_set_time(&a, anim_time);
    lv_anim_set_playback_time(&a, anim_time);
    lv_anim_set_playback_delay(&a, 200);
    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
    lv_anim_set_repeat_delay(&a, 200);
    lv_anim_start(&a);
}
static void tab_changer_task_cb(lv_task_t * task)
{
    uint16_t act = lv_tabview_get_tab_act(tv);
    act++;
    if(act >= 3) act = 0;

    lv_tabview_set_tab_act(tv, act, LV_ANIM_ON);

    switch(act) {
    case 0:
        tab_content_anim_create(t1);
        break;
    case 1:
        tab_content_anim_create(t2);
        break;
    case 2:
        tab_content_anim_create(t3);
        break;
    }
}
#endif

#endif
