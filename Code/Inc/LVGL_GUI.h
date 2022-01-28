#ifndef __LVGL_GUI_H
#define __LVGL_GUI_H

#include "main.h"
#include "lvgl.h"
#include "RadioTypes.h"
#include "math.h"
#include "cmsis_os.h"
#include <string.h>

// acuqire this mutex before modifying LVGL GUI contents
extern osMutexId_t mtxGUIWidgetsHandle;

#define COLOR_PLAIN_TEXT LV_COLOR_WHITE
#define COLOR_BACKGROUND LV_COLOR_MAKE(37, 40, 42)
#define COLOR_FOREGROUND LV_COLOR_BLUE
#define COLOR_ACCENT LV_COLOR_ORANGE
#define COLOR_INTERACTABLE LV_COLOR_MAKE(0, 160, 225) // cyan blue
#define COLOR_INTERACTED LV_COLOR_MAKE(0, 120, 180) // darkened cyan blue
#define COLOR_BANNER_IDLE LV_COLOR_BLUE
#define COLOR_BANNER_ACTIVE LV_COLOR_MAKE(0xfa,0xcc,0x2e) // dark yellow
#define COLOR_BANNER_TRANSMITTING LV_COLOR_MAKE(0xdf,0x01,0x01) // dark red
#define COLOR_ROLLER_BACKGROUND LV_COLOR_MAKE(0xee, 0xee, 0xee)


#define PROGBAR_MAX_VALUE_FWDPWR 1000
#define PROGBAR_MAX_VALUE_REVPWR 1000
#define PROGBAR_MAX_VALUE_SWR 1000 // maps 0.0f-1.0f to 0-1000

//LV_FONT_DECLARE(CNSans_14);
LV_FONT_DECLARE(CNSans_24);
LV_FONT_DECLARE(lv_font_montserrat_16);
LV_FONT_DECLARE(lv_font_montserrat_24);
LV_FONT_DECLARE(lv_font_montserrat_48);
/* Styles */
extern lv_style_t lvStyleBackground;
// 上下栏syle
extern lv_style_t lvStyleBanner;
// 小号英文数字
extern lv_style_t lvStyleAlphabetsTextSmall;
// 大号英文数字
extern lv_style_t lvStyleAlphabetsTextLarge;
// 小号中文
extern lv_style_t lvStyleCHNTextSmall;
// 大号中文
extern lv_style_t lvStyleCHNTextLarge;
// 功率进度条样式
extern lv_style_t lvStyleProgBar;
// 按钮样式
extern lv_style_t lvStyleBtn;
// Roller选择器样式
extern lv_style_t lvStyleRoller;
extern lv_style_t lvStyleRollerSel;

/* Main Page */
extern bool isMainWidgetsCreated;
// 容器最上方分区-发射状态
extern lv_obj_t* lvContTopBanner;
// 发射状态
extern lv_obj_t* lvLblTransmissionState;
extern const char strTransmissionStateIdle[];
extern const char strTransmissionStateActive[];
extern const char strTransmissionStateOnAir[];
extern lv_obj_t* lvBtnEnableMainPower;
extern bool isTransmitting;
extern bool MainPowerEnabled;

// 上方分区-前向功率
// 前向功率标签
extern lv_obj_t* lvLblFwdPwrTitle; // lv_label: Forward Power
// 前向功率标尺位图
extern lv_obj_t* lvBmpScaleFwdPwr; // lv_image:
// 前向功率进度条
extern lv_obj_t* lvProgbarFwdPwr;
// 峰值功率
extern lv_obj_t* lvLblFwdPwrValue;

// 中上部分区左区：
// 反向功率标签
extern lv_obj_t* lvLblRevPwrTitle;
// 反射功率标尺位图
extern lv_obj_t* lvBmpScaleRevPwr;
// 反向功率进度条
extern lv_obj_t* lvProgbarRevPwr;
// 反射功率峰值
extern lv_obj_t* lvLblRevPwrValue;
// 中上部分区右区：
// SWR标签
extern lv_obj_t* lvLblSWRTitle;
// SWR进度条
extern lv_obj_t* lvProgbarSWR;
// SWR标尺
extern lv_obj_t* lvBmpScaleSWR;
// SWR峰值
extern lv_obj_t* lvLblSWRValue;

// 底部分区
extern lv_obj_t* lvContBottomBanner;
extern const char* strFreqRanges[];

// 波段
extern lv_obj_t* lvLblBand;
// 频率范围
extern lv_obj_t* lvLblFreqRange;
// 自动波段解码器模式
extern char strBandDecoderMode[16];
extern lv_obj_t* lvLblBandDecoderMode;

// 底栏区
// 核心温度
extern lv_obj_t* lvLblCoreTemp;
// 周边温度
extern lv_obj_t* lvLblAmbTemp;
// 电流
extern lv_obj_t* lvLblImain;
// 电压
extern lv_obj_t* lvLblVmain;

// create main scene
void init_main_widgets();


// show pop-up warnings
typedef struct{
	lv_obj_t* lvContTitle;
	lv_obj_t* lvContBody;
	lv_obj_t* lvLblTitle;
	lv_obj_t* lvLblContent;
	lv_obj_t* lvBtnOK;
	lv_obj_t* lvBtnCancel;
} MsgBox_t;

// create a message box object and show it
extern osSemaphoreId_t sphWarnMsgBoxDismissed;
#define FLAG_WARNING_MSG_BOX_DISMISSED 0x02
lv_obj_t* show_msgbox_warning(const char* title, const char* content);

// create a message box object for band selecting
extern const char* strBands[];
lv_obj_t* show_band_selector();
// How to calculate the logarithm scales
// x: mapped to scale 0-1.0  y: physical value
// y(x) = y0 * e^((x-x0)/(x1-x0) * log(y1/y0))
// x(y) = log(y / y0)/log(y1/y0) * x0 + (x1 - x0)
#define PWR_LOGSCALE_X0 0.3f
#define PWR_LOGSCALE_Y0 350.0f
#define PWR_LOGSCALE_X1 0.8f
#define PWR_LOGSCALE_LOGR 1.049822f // LOGR =log(1000.0f / 350.0f)
#define PWR_LOGSCALE_ANCHORPOINT_RANGE (PWR_LOGSCALE_X1 - PWR_LOGSCALE_X0)
#define SWR_LOGSCALE_X0 0.0f
#define SWR_LOGSCALE_Y0 1.0f
#define SWR_LOGSCALE_X1 0.5f
#define SWR_LOGSCALE_LOGR 0.693147f // LOGR = log(2.0/1.0)
#define SWR_LOGSCALE_ANCHORPOINT_RANGE (SWR_LOGSCALE_X1 - SWR_LOGSCALE_X0)

void number2text(char* dest,
		int number, // value without decimal point
		uint8_t decimal, // digits of decimals (0: integer, 1: 1.1, 2: 1.10)
		char suffix);

void my_utoa(char* dest, uint16_t num);
void my_i16toa(char* dest, int16_t num);

void GUI_set_transmission_state(Transmission_State_t state);

void GUI_set_band_info(band_t band);

void GUI_set_band_decoder_info(BandMode_t mode);

void GUI_set_FWD_progbar(uint16_t watt);

void GUI_set_FWD_peak_value(uint16_t watt);

void GUI_set_REV_progbar(uint16_t watt);

void GUI_set_REV_peak_value(uint16_t watt);

void GUI_set_SWR_progbar(float SWR);

void GUI_set_SWR_peak_value(float SWR);

void GUI_set_core_temperature(int16_t C);

void GUI_set_ambient_temperature(int16_t C);

void GUI_set_Vmain(int V);

void GUI_set_Imain(int mA);
/* Main Scene Methods end*/


/* TS Calib Scene Methods begin */
/* Touch Screen Calibration Scene begin */
extern lv_obj_t* lvContCalib; // touch screen calibration background
extern lv_obj_t* lvLblCalibPrompt; // text: Calibrating
extern lv_obj_t* lvCircTouchPoint;
// create calibration scene
void init_touchscreen_calib_widgets();
void dismiss_touchscreen_calib_widgets();
void GUI_set_TSCalib_touchpoint_pos(uint16_t real_X, uint16_t real_Y);
/* TS Calib Scene Methods end */

/* Settings Menu Scene begin */
extern lv_obj_t* lvContSettings;
extern lv_obj_t* lvBtnmSettings;
void init_settings_menu_widgets();
void dismiss_settings_menu_widgets();
/* Settings Menu Scene end */



#endif
