/*
 * LVGL_GUI.c
 *
 *  Created on: 2020年6月11日
 *      Author: Zening
 */

#include "LVGL_GUI.h"
/* Styles */
// 背景板
lv_style_t lvStyleBackground;
// 上下标题栏
lv_style_t lvStyleBanner;
// 小号英文数字
lv_style_t lvStyleAlphabetsTextSmall;
// 大号英文数字
lv_style_t lvStyleAlphabetsTextLarge;
// 小号中文
lv_style_t lvStyleCHNTextSmall;
// 大号中文
lv_style_t lvStyleCHNTextLarge;
// 功率进度条样式
lv_style_t lvStyleProgBarBg;
lv_style_t lvStyleProgBarInd;
// 弹窗样式
lv_style_t lvStyleMsgBox;
// 子窗口样式
lv_style_t lvStyleSubWindow;
//按钮样式
lv_style_t lvStyleBtn;
// Roller选择器样式
lv_style_t lvStyleRoller;
lv_style_t lvStyleRollerSel;


/* Main Page */
// 容器最上方分区-发射状态
lv_obj_t* lvContTopBanner = NULL;
// 发射状态
lv_obj_t* lvLblTransmissionState = NULL;
const char strTransmissionStateIdle[] = "待机";
const char strTransmissionStateActive[] = "已激活";
const char strTransmissionStateOnAir[] = "发射中";
// 激活主电源按钮
lv_obj_t* lvBtnEnableMainPower = NULL;
lv_obj_t* lvLblEnableMainPowerBtn = NULL;

// 容器上方分区
lv_obj_t* lvContFwdPwr = NULL;
// 前向功率标签
lv_obj_t* lvLblFwdPwrTitle = NULL; // lv_label: Forward Power
// 前向功率标尺位图
lv_obj_t* lvBmpScaleFwdPwr = NULL; // lv_image:
// 前向功率进度条
lv_obj_t* lvProgbarFwdPwr = NULL;
// 峰值功率
lv_obj_t* lvLblFwdPwrValue = NULL;

// 中上部分区左区：
lv_obj_t* lvContRevPwr = NULL;
// 反向功率标签
lv_obj_t* lvLblRevPwrTitle = NULL;
// 反射功率标尺位图
lv_obj_t* lvBmpScaleRevPwr = NULL;
// 反向功率进度条
lv_obj_t* lvProgbarRevPwr = NULL;
// 反射功率峰值
lv_obj_t* lvLblRevPwrValue = NULL;

// 中上部分区右区：
lv_obj_t* lvContSWR = NULL;
// SWR标签
lv_obj_t* lvLblSWRTitle = NULL;
// SWR进度条
lv_obj_t* lvProgbarSWR = NULL;
// SWR标尺
lv_obj_t* lvBmpScaleSWR = NULL;
// SWR峰值
lv_obj_t* lvLblSWRValue = NULL;

// 中下部分区
lv_obj_t* lvContCurrentInfo = NULL;
// 波段
lv_obj_t* lvLblBand = NULL;
// 频率范围
lv_obj_t* lvLblFreqRange=NULL;
// 自动波段解码器模式
char strBandDecoderMode[16] = "";
lv_obj_t* lvLblBandDecoderMode;
const char* strFreqRanges[] = {
	"1.800-2.100 MHz", // 160m
	"3.500-3.900 MHz", // 80m
	"7.000-10.150 MHz", // 40-30m
	"14.000-18.168 MHz", // 20-17mm
	"21.000-29.700 MHz" // 15-10m
};
const char* strBands[] = {
		"160m",
		"80m",
		"40-30m",
		"20-17m",
		"15-10m"
};


// 底栏区
lv_obj_t* lvContBottomBanner = NULL;
// 核心温度
lv_obj_t* lvLblCoreTemp = NULL;
// 周边温度
lv_obj_t* lvLblAmbTemp = NULL;
// 电流
lv_obj_t* lvLblImain = NULL;
// 电压
lv_obj_t* lvLblVmain = NULL;
// 菜单按钮
lv_obj_t* lvBtnMenu = NULL;
lv_obj_t* lvLblMenuBtn = NULL;

static void init_styles()
{
	static bool isStyleInitialized = false;
	if(isStyleInitialized)
		return;
	isStyleInitialized = true;
	lv_style_init(&lvStyleBackground);
	lv_style_set_bg_color(&lvStyleBanner, LV_STATE_DEFAULT, COLOR_BACKGROUND);
	lv_style_set_bg_opa(&lvStyleBanner, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_style_set_radius(&lvStyleBanner, LV_STATE_DEFAULT, 0);
	// Top, Bottom banner styles at different states
	// @ Color Idle: Blue, Active: Deep Yellow, On Air: Red
	// Idle
	lv_style_init(&lvStyleBanner);
	lv_style_set_bg_color(&lvStyleBanner, LV_STATE_DEFAULT, LV_COLOR_BLUE);
	lv_style_set_bg_opa(&lvStyleBanner, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_style_set_radius(&lvStyleBanner, LV_STATE_DEFAULT, 0);

	//@Color: white
	//@Font: 16pt
	lv_style_init(&lvStyleAlphabetsTextSmall);
	lv_style_set_text_color(&lvStyleAlphabetsTextSmall, LV_STATE_DEFAULT, COLOR_PLAIN_TEXT);
	lv_style_set_text_font(&lvStyleAlphabetsTextSmall, LV_STATE_DEFAULT, &lv_font_montserrat_16);
	lv_style_set_pad_left(&lvStyleAlphabetsTextSmall, LV_STATE_DEFAULT, 8);
	//@Color: white
	//@Font: 24pt
	lv_style_init(&lvStyleAlphabetsTextLarge);
	lv_style_set_text_color(&lvStyleAlphabetsTextLarge, LV_STATE_DEFAULT, COLOR_PLAIN_TEXT);
	lv_style_set_text_font(&lvStyleAlphabetsTextLarge, LV_STATE_DEFAULT, &lv_font_montserrat_24);
	lv_style_set_pad_left(&lvStyleAlphabetsTextLarge, LV_STATE_DEFAULT, 12);

	//@Color: white
	//@Font: 16pt
	lv_style_init(&lvStyleCHNTextSmall);
	lv_style_set_text_color(&lvStyleCHNTextSmall, LV_STATE_DEFAULT, COLOR_PLAIN_TEXT);
	lv_style_set_text_font(&lvStyleCHNTextSmall, LV_STATE_DEFAULT, &CNSans_24);
	lv_style_set_pad_left(&lvStyleCHNTextSmall, LV_STATE_DEFAULT, 12);

	//@Color: white
	//@Font: 24pt
	lv_style_init(&lvStyleCHNTextLarge);
	lv_style_set_text_color(&lvStyleCHNTextLarge, LV_STATE_DEFAULT, COLOR_PLAIN_TEXT);
	lv_style_set_text_font(&lvStyleCHNTextLarge, LV_STATE_DEFAULT, &CNSans_24);
	lv_style_set_pad_left(&lvStyleCHNTextLarge, LV_STATE_DEFAULT, 12);

	lv_style_init(&lvStyleProgBarBg);
	lv_style_set_bg_color(&lvStyleProgBarBg, LV_STATE_DEFAULT, LV_COLOR_BLACK);
	lv_style_set_bg_opa(&lvStyleProgBarBg, LV_STATE_DEFAULT, 127);
	lv_style_set_radius(&lvStyleProgBarBg, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_inner(&lvStyleProgBarBg, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_top(&lvStyleProgBarBg, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_left(&lvStyleProgBarBg, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_right(&lvStyleProgBarBg, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_bottom(&lvStyleProgBarBg, LV_STATE_DEFAULT, 0);

	lv_style_init(&lvStyleProgBarInd);
	lv_style_set_radius(&lvStyleProgBarInd, LV_STATE_DEFAULT, 0);
	lv_style_set_bg_opa(&lvStyleProgBarInd, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_style_set_bg_color(&lvStyleProgBarInd, LV_STATE_DEFAULT, LV_COLOR_AQUA);
	lv_style_set_bg_grad_color(&lvStyleProgBarInd, LV_STATE_DEFAULT, LV_COLOR_ORANGE);
	lv_style_set_bg_main_stop(&lvStyleProgBarInd, LV_STATE_DEFAULT, (int)(0.7 * 256));
	lv_style_set_bg_grad_stop(&lvStyleProgBarInd, LV_STATE_DEFAULT, (int)(0.85 * 256));
	lv_style_set_bg_grad_dir(&lvStyleProgBarInd, LV_STATE_DEFAULT, LV_GRAD_DIR_HOR);

	lv_style_init(&lvStyleMsgBox);
	lv_style_set_radius(&lvStyleMsgBox, LV_STATE_DEFAULT, 0);
	lv_style_set_bg_color(&lvStyleMsgBox, LV_STATE_DEFAULT, LV_COLOR_BLUE);
	lv_style_set_bg_opa(&lvStyleMsgBox, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_style_set_shadow_color(&lvStyleMsgBox, LV_STATE_DEFAULT, LV_COLOR_BLACK);
	lv_style_set_shadow_opa(&lvStyleMsgBox, LV_STATE_DEFAULT, LV_OPA_70);
	lv_style_set_shadow_spread(&lvStyleMsgBox, LV_STATE_DEFAULT, 30);
	lv_style_set_shadow_width(&lvStyleMsgBox, LV_STATE_DEFAULT, 30);
	lv_style_set_text_color(&lvStyleMsgBox, LV_STATE_DEFAULT, COLOR_PLAIN_TEXT);
	lv_style_set_text_font(&lvStyleMsgBox, LV_STATE_DEFAULT, &CNSans_24);

	lv_style_init(&lvStyleSubWindow);
	lv_style_set_radius(&lvStyleSubWindow, LV_STATE_DEFAULT, 0);
	lv_style_set_bg_color(&lvStyleSubWindow, LV_STATE_DEFAULT, COLOR_BACKGROUND);
	lv_style_set_bg_opa(&lvStyleSubWindow, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_style_set_shadow_color(&lvStyleSubWindow, LV_STATE_DEFAULT, LV_COLOR_BLACK);
	lv_style_set_shadow_opa(&lvStyleSubWindow, LV_STATE_DEFAULT, LV_OPA_70);
	lv_style_set_shadow_spread(&lvStyleSubWindow, LV_STATE_DEFAULT, 30);
	lv_style_set_shadow_width(&lvStyleSubWindow, LV_STATE_DEFAULT, 30);
	lv_style_set_border_width(&lvStyleSubWindow, LV_STATE_DEFAULT, 3);
	lv_style_set_border_color(&lvStyleSubWindow, LV_STATE_DEFAULT, COLOR_FOREGROUND);
	lv_style_set_border_opa(&lvStyleSubWindow, LV_STATE_DEFAULT, LV_OPA_COVER);

	lv_style_init(&lvStyleBtn);
	lv_style_set_radius(&lvStyleBtn, LV_STATE_DEFAULT, 0);
	lv_style_set_bg_opa(&lvStyleBtn, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_style_set_bg_color(&lvStyleBtn, LV_STATE_DEFAULT, COLOR_INTERACTABLE);
	lv_style_set_bg_color(&lvStyleBtn, LV_BTN_STATE_PRESSED, COLOR_INTERACTED);
	lv_style_set_bg_color(&lvStyleBtn, LV_BTN_STATE_CHECKED_PRESSED, COLOR_INTERACTED);

	lv_style_init(&lvStyleRoller);
	lv_style_set_radius(&lvStyleRoller, LV_STATE_DEFAULT, 3);
	lv_style_set_bg_opa(&lvStyleRoller, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_style_set_bg_color(&lvStyleRoller, LV_STATE_DEFAULT, COLOR_ROLLER_BACKGROUND);
	lv_style_set_text_color(&lvStyleRoller, LV_STATE_DEFAULT, LV_COLOR_BLACK);
	lv_style_set_text_font(&lvStyleRoller, LV_STATE_DEFAULT, &lv_font_montserrat_24);
	lv_style_set_text_line_space(&lvStyleRoller, LV_STATE_DEFAULT, 32);

	lv_style_init(&lvStyleRollerSel);
	lv_style_set_bg_opa(&lvStyleRollerSel, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_style_set_bg_color(&lvStyleRollerSel, LV_STATE_DEFAULT, COLOR_BACKGROUND);
	lv_style_set_text_color(&lvStyleRollerSel, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	lv_style_set_text_font(&lvStyleRollerSel, LV_STATE_DEFAULT, &lv_font_montserrat_24);

}

// open Band selector window
volatile bool isBandSelectorShown = false;
static void lbl_freq_range_event_handle(lv_obj_t* obj, lv_event_t e)
{
	if(e == LV_EVENT_CLICKED && !isBandSelectorShown)
	{
		isBandSelectorShown = true;
		osMutexAcquire(mtxGUIWidgetsHandle, osWaitForever);
		show_band_selector();
		osMutexRelease(mtxGUIWidgetsHandle);
	}
}
// Button Event: toggle main power
bool MainPowerEnabled = false;
bool isTransmitting = false;
static void toggle_main_power(lv_obj_t* obj, lv_event_t e)
{
	if(e == LV_EVENT_CLICKED)
	{
		if(HAL_GPIO_ReadPin(Transmit_GPIO_Port, Transmit_Pin) == GPIO_PIN_RESET) // NOT transmitting
		{
			MainPowerEnabled = !MainPowerEnabled;
			osMutexAcquire(mtxGUIWidgetsHandle, osWaitForever);
			// change style FROM IDLE to ACTIVE
			if(MainPowerEnabled)
			{
				lv_obj_set_style_local_bg_color(lvContTopBanner, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, COLOR_BANNER_ACTIVE);
				lv_obj_set_style_local_bg_color(lvContBottomBanner, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, COLOR_BANNER_ACTIVE);

			}
			else
			{
				lv_obj_set_style_local_bg_color(lvContTopBanner, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, COLOR_BANNER_IDLE);
				lv_obj_set_style_local_bg_color(lvContBottomBanner, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, COLOR_BANNER_IDLE);
			}
			osMutexRelease(mtxGUIWidgetsHandle);
		}
	}
}

// Button Event: invoke menu
static void invoke_menu(lv_obj_t* obj, lv_event_t e)
{
	if(e == LV_EVENT_CLICKED)
	{
		osMutexAcquire(mtxGUIWidgetsHandle, osWaitForever);
		init_settings_menu_widgets();
		osMutexRelease(mtxGUIWidgetsHandle);
	}
}

bool isMainWidgetsCreated = false;
void init_main_widgets()
{
	if(isMainWidgetsCreated)
		return;
	init_styles();
	lv_obj_t* scr = lv_scr_act();
	static const int HEIGHT_CONT_FWD_PWR = 120;
	static const int PROGBAR_ANIM_TIME_MS = 200;
	static const int FWDPWR_PROGBAR_HEIGHT = 30;
	static const int REVPWR_PROGBAR_HEIGHT = 24;
	static const int SWR_PROGBAR_HEIGHT = 24;
	static const int FWDPWR_SCALE_HEIGHT = 30;
	static const int MARGIN_TO_BORDER_X = 30;
	static const int MARGIN_TO_BORDER_Y = 10;

	// 顶区容器
	lvContTopBanner = lv_obj_create(scr, NULL);
	lv_obj_align(lvContTopBanner, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
	lv_obj_set_size(lvContTopBanner, LV_HOR_RES_MAX, 48);
	lv_obj_reset_style_list(lvContTopBanner, LV_OBJ_PART_MAIN);
	lv_obj_add_style(lvContTopBanner, LV_OBJ_PART_MAIN, &lvStyleBanner);
	// 发射状态
	lvLblTransmissionState = lv_label_create(lvContTopBanner, NULL);
	lv_obj_align(lvLblTransmissionState, NULL, LV_ALIGN_CENTER, 0, 0);
	lv_label_set_align(lvLblTransmissionState, LV_LABEL_ALIGN_CENTER);
	lv_obj_add_style(lvLblTransmissionState, LV_LABEL_PART_MAIN, &lvStyleCHNTextLarge);
	lv_label_set_text(lvLblTransmissionState, strTransmissionStateIdle);
	// 主电源激活按钮
	lvBtnEnableMainPower = lv_btn_create(lvContTopBanner, NULL);
	lv_obj_set_height(lvBtnEnableMainPower , 40);
	lv_obj_reset_style_list(lvBtnEnableMainPower , LV_BTN_PART_MAIN);
	lv_obj_add_style(lvBtnEnableMainPower , LV_BTN_PART_MAIN, &lvStyleBtn);
	lv_obj_set_event_cb(lvBtnEnableMainPower , toggle_main_power);
	lvLblEnableMainPowerBtn = lv_label_create(lvBtnEnableMainPower, lvLblTransmissionState);
	lv_label_set_text(lvLblEnableMainPowerBtn , "激活");

	// 上区容器
	lvContFwdPwr = lv_obj_create(scr, NULL);
	lv_obj_reset_style_list(lvContFwdPwr, LV_OBJ_PART_MAIN);
	lv_obj_set_size(lvContFwdPwr, LV_HOR_RES_MAX, HEIGHT_CONT_FWD_PWR); // 1/4 height
	lv_obj_align(lvContFwdPwr, lvContTopBanner, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
	// lvLblFwdPwrTitle 前向功率标签
	lvLblFwdPwrTitle = lv_label_create(lvContFwdPwr, NULL);
	lv_label_set_align(lvLblFwdPwrTitle, LV_LABEL_ALIGN_LEFT);
	lv_label_set_text(lvLblFwdPwrTitle, "发射");
	lv_obj_add_style(lvLblFwdPwrTitle, LV_LABEL_PART_MAIN, &lvStyleCHNTextLarge);
	lv_obj_align(lvLblFwdPwrTitle, NULL, LV_ALIGN_IN_TOP_LEFT, MARGIN_TO_BORDER_X, MARGIN_TO_BORDER_Y);
	// lvLblFwdPwrValue 峰值功率
	lvLblFwdPwrValue = lv_label_create(lvContFwdPwr, NULL);
	lv_label_set_align(lvLblFwdPwrValue , LV_LABEL_ALIGN_RIGHT);
	lv_obj_add_style(lvLblFwdPwrValue , LV_LABEL_PART_MAIN, &lvStyleAlphabetsTextLarge);
	lv_obj_align(lvLblFwdPwrValue, NULL, LV_ALIGN_IN_TOP_RIGHT, -50, MARGIN_TO_BORDER_Y);
	lv_label_set_text(lvLblFwdPwrValue , "0W");
	// lvProgbarFwdPwr 前向功率进度条
	lvProgbarFwdPwr = lv_bar_create(lvContFwdPwr, NULL);
	lv_obj_set_size(lvProgbarFwdPwr, 700, FWDPWR_PROGBAR_HEIGHT);
	lv_bar_set_range(lvProgbarFwdPwr, 0, PROGBAR_MAX_VALUE_FWDPWR);
	lv_bar_set_start_value(lvProgbarFwdPwr, 0, LV_ANIM_OFF);
	lv_bar_set_value(lvProgbarFwdPwr, PROGBAR_MAX_VALUE_FWDPWR / 2, LV_ANIM_OFF);
	//lv_bar_set_anim_time(lvProgbarSWR, PROGBAR_ANIM_TIME_MS);
	lv_obj_align(lvProgbarFwdPwr, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, -40);
	lv_obj_add_style(lvProgbarFwdPwr, LV_BAR_PART_BG, &lvStyleProgBarBg);
	lv_obj_reset_style_list(lvProgbarFwdPwr, LV_BAR_PART_INDIC);
	lv_obj_add_style(lvProgbarFwdPwr, LV_BAR_PART_INDIC, &lvStyleProgBarInd);
	//lv_obj_set_style_local_bg_color(lvProgbarFwdPwr, LV_BAR_PART_INDIC, LV_STATE_DEFAULT, LV_COLOR_GREEN);

	// 上中区左容器
	lvContRevPwr = lv_obj_create(scr, NULL);
	lv_obj_reset_style_list(lvContRevPwr, LV_OBJ_PART_MAIN);
	lv_obj_set_size(lvContRevPwr, LV_HOR_RES_MAX / 2, HEIGHT_CONT_FWD_PWR / 3 * 2);
	lv_obj_align(lvContRevPwr, lvContFwdPwr, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);
	// lvLblRevPwrTitle 反射功率标签
	lvLblRevPwrTitle = lv_label_create(lvContRevPwr, NULL);
	lv_label_set_align(lvLblRevPwrTitle, LV_LABEL_ALIGN_LEFT);
	lv_label_set_text(lvLblRevPwrTitle, "反射");
	lv_obj_add_style(lvLblRevPwrTitle, LV_LABEL_PART_MAIN, &lvStyleCHNTextLarge);
	lv_obj_align(lvLblRevPwrTitle, NULL, LV_ALIGN_IN_TOP_LEFT, MARGIN_TO_BORDER_X, MARGIN_TO_BORDER_Y);
	// lvLblRevPwrValue 峰值功率
	lvLblRevPwrValue = lv_label_create(lvContRevPwr, NULL);
	lv_label_set_align(lvLblRevPwrValue, LV_LABEL_ALIGN_RIGHT);
	lv_obj_add_style(lvLblRevPwrValue, LV_LABEL_PART_MAIN, &lvStyleAlphabetsTextLarge);
	lv_obj_align(lvLblRevPwrValue, NULL, LV_ALIGN_IN_TOP_RIGHT, -50, MARGIN_TO_BORDER_Y);
	lv_label_set_text(lvLblRevPwrValue, "0W");
	// lvProgbarRevPwr 反向功率进度条
	lvProgbarRevPwr = lv_bar_create(lvContRevPwr, NULL);
	lv_obj_set_size(lvProgbarRevPwr, 300, REVPWR_PROGBAR_HEIGHT);
	lv_bar_set_range(lvProgbarRevPwr, 0, PROGBAR_MAX_VALUE_REVPWR);
	lv_bar_set_start_value(lvProgbarRevPwr, 0, LV_ANIM_OFF);
	lv_bar_set_value(lvProgbarRevPwr, PROGBAR_MAX_VALUE_REVPWR / 2, LV_ANIM_OFF);
	//lv_bar_set_anim_time(lvProgbarSWR, PROGBAR_ANIM_TIME_MS);
	lv_obj_align(lvProgbarRevPwr, NULL, LV_ALIGN_IN_TOP_MID, 0, 40);
	lv_obj_add_style(lvProgbarRevPwr, LV_BAR_PART_BG, &lvStyleProgBarBg);
	lv_obj_reset_style_list(lvProgbarRevPwr, LV_BAR_PART_INDIC);
	lv_obj_add_style(lvProgbarRevPwr, LV_BAR_PART_INDIC, &lvStyleProgBarInd);

	// 上中区右容器
	lvContSWR = lv_obj_create(scr, NULL);
	lv_obj_reset_style_list(lvContSWR, LV_OBJ_PART_MAIN);
	lv_obj_set_size(lvContSWR, LV_HOR_RES_MAX / 2, HEIGHT_CONT_FWD_PWR / 3 * 2);
	lv_obj_align(lvContSWR, lvContFwdPwr, LV_ALIGN_OUT_BOTTOM_RIGHT, 0, 0);
	// lvLblSWRTitle SWR标签
	lvLblSWRTitle = lv_label_create(lvContSWR, NULL);
	lv_label_set_align(lvLblSWRTitle, LV_LABEL_ALIGN_LEFT);
	lv_label_set_text(lvLblSWRTitle, "驻波比");
	lv_obj_add_style(lvLblSWRTitle, LV_LABEL_PART_MAIN, &lvStyleCHNTextLarge);
	lv_obj_align(lvLblSWRTitle, NULL, LV_ALIGN_IN_TOP_LEFT, MARGIN_TO_BORDER_X, MARGIN_TO_BORDER_Y);
	// lvLblSWRValue 峰值SWR
	lvLblSWRValue = lv_label_create(lvContSWR, NULL);
	lv_label_set_align(lvLblSWRValue, LV_LABEL_ALIGN_RIGHT);
	lv_label_set_text(lvLblSWRValue, "1.2");
	lv_obj_add_style(lvLblSWRValue, LV_LABEL_PART_MAIN, &lvStyleAlphabetsTextLarge);
	lv_obj_align(lvLblSWRValue, NULL, LV_ALIGN_IN_TOP_RIGHT, -50, MARGIN_TO_BORDER_Y);
	// lvProgbarSWR SWR进度条
	lvProgbarSWR = lv_bar_create(lvContSWR, NULL);
	lv_obj_set_size(lvProgbarSWR, 300, SWR_PROGBAR_HEIGHT);
	lv_bar_set_range(lvProgbarSWR, 0, PROGBAR_MAX_VALUE_SWR);
	lv_bar_set_start_value(lvProgbarSWR, 0, LV_ANIM_OFF);
	lv_bar_set_value(lvProgbarSWR, PROGBAR_MAX_VALUE_SWR / 2, LV_ANIM_OFF);
	//lv_bar_set_anim_time(lvProgbarSWR, PROGBAR_ANIM_TIME_MS);
	lv_obj_align(lvProgbarSWR, NULL, LV_ALIGN_IN_TOP_MID, 0, 40);
	lv_obj_add_style(lvProgbarSWR, LV_BAR_PART_BG, &lvStyleProgBarBg);
	lv_obj_reset_style_list(lvProgbarSWR, LV_BAR_PART_INDIC);
	lv_obj_add_style(lvProgbarSWR, LV_BAR_PART_INDIC, &lvStyleProgBarInd);

	// 中区容器
	lvContCurrentInfo = lv_obj_create(scr, NULL);
	lv_obj_align(lvContCurrentInfo, lvContRevPwr, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);
	lv_obj_set_size(lvContCurrentInfo, LV_HOR_RES_MAX, 160);
	// lvLblFreqRange 频率范围
	lvLblFreqRange = lv_label_create(lvContCurrentInfo, NULL);
	lv_label_set_text(lvLblFreqRange, "7.000 - 7.150 MHz");
	lv_label_set_align(lvLblFreqRange, LV_LABEL_ALIGN_CENTER);
	lv_obj_add_style(lvLblFreqRange, LV_LABEL_PART_MAIN, &lvStyleCHNTextLarge);
	lv_obj_set_style_local_text_font(lvLblFreqRange, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_48);
	lv_obj_set_style_local_text_color(lvLblFreqRange, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_YELLOW);
	lv_obj_align(lvLblFreqRange, NULL, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_event_cb(lvContCurrentInfo, lbl_freq_range_event_handle);
	// 自动波段指示
	lvLblBandDecoderMode = lv_label_create(lvContCurrentInfo, lvLblFwdPwrTitle);
	lv_obj_align(lvLblBandDecoderMode, NULL, LV_ALIGN_CENTER, 0, 58);
	lv_label_set_text_static(lvLblBandDecoderMode, strBandDecoderMode);
	// 底部容器
	lvContBottomBanner = lv_cont_create(scr, NULL);
	lv_cont_set_layout(lvContBottomBanner, LV_LAYOUT_PRETTY_MID); // 设定布局: 行，居中，等距排列
	lv_obj_set_size(lvContBottomBanner, LV_HOR_RES_MAX, 48);
//	lv_obj_align_ori(lvContBottomBanner, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 0, 0);
	lv_obj_align(lvContBottomBanner, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 0);
	lv_obj_reset_style_list(lvContBottomBanner, LV_OBJ_PART_MAIN);
	lv_obj_add_style(lvContBottomBanner, LV_OBJ_PART_MAIN, &lvStyleBanner);
	// 底栏元素1：电压
	lvLblVmain = lv_label_create(lvContBottomBanner, NULL);
	lv_label_set_align(lvLblVmain, LV_LABEL_ALIGN_CENTER);
	lv_obj_add_style(lvLblVmain, LV_LABEL_PART_MAIN, &lvStyleAlphabetsTextSmall);
	lv_label_set_text(lvLblVmain, "N/A");
	// 底栏元素2：电流
	lvLblImain = lv_label_create(lvContBottomBanner, lvLblVmain);
	lv_label_set_text(lvLblImain, "N/A");
	// 底栏元素3: 核心温度
	lvLblCoreTemp = lv_label_create(lvContBottomBanner, lvLblVmain);
//	lv_label_set_text(lvLblCoreTemp, "N/A");
	// 底栏元素4: 周边温度
	lvLblAmbTemp = lv_label_create(lvContBottomBanner, lvLblVmain);
//	lv_label_set_text(lvLblCore)
	// 底栏元素4: 调出菜单
	lvBtnMenu = lv_btn_create(lvContBottomBanner, NULL);
	lv_obj_set_height(lvBtnMenu, 40);
	lv_obj_reset_style_list(lvBtnMenu, LV_BTN_PART_MAIN);
	lv_obj_add_style(lvBtnMenu, LV_BTN_PART_MAIN, &lvStyleBtn);
	lv_obj_set_event_cb(lvBtnMenu, invoke_menu);
	lvLblMenuBtn = lv_label_create(lvBtnMenu, lvLblFwdPwrTitle);
	lv_label_set_text(lvLblMenuBtn, "设置");
	isMainWidgetsCreated = true;
}

lv_obj_t* lvContCalib = NULL; // touch screen calibration background
lv_obj_t* lvLblCalibPrompt = NULL; // text: Calibrating
lv_obj_t* lvCircTouchPoint = NULL;
// create calibration scene
void init_touchscreen_calib_widgets()
{
	init_styles();
	lv_obj_t* scr = lv_scr_act();
	// Solid color fullscreen Background
	if(lvContCalib == NULL)
	{
		lvContCalib = lv_obj_create(scr, NULL);
		lv_obj_set_size(lvContCalib, LV_HOR_RES_MAX, LV_VER_RES_MAX);
		lv_obj_align(lvContCalib, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 0, 0);
		lv_obj_reset_style_list(lvContCalib, LV_OBJ_PART_MAIN);
		lv_obj_set_style_local_bg_color(lvContCalib, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
		lv_obj_set_style_local_radius(lvContCalib, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT,0);
		lv_obj_set_style_local_bg_opa(lvContCalib, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_COVER);
	}
	// Prompt Text
	if(lvLblCalibPrompt == NULL)
	{
		lvLblCalibPrompt = lv_label_create(lvContCalib, NULL);
		lv_label_set_align(lvLblCalibPrompt, LV_LABEL_ALIGN_CENTER);
		lv_label_set_text(lvLblCalibPrompt, "校准屏幕");
		lv_obj_add_style(lvLblCalibPrompt, LV_LABEL_PART_MAIN, &lvStyleCHNTextLarge);
		lv_obj_set_style_local_text_color(lvLblCalibPrompt, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
		lv_obj_align(lvLblCalibPrompt, NULL, LV_ALIGN_CENTER, 0, 50);
	}
	// Touch Point
	if(lvCircTouchPoint == NULL)
	{
		lvCircTouchPoint = lv_led_create(lvContCalib, NULL);
		lv_obj_reset_style_list(lvCircTouchPoint, LV_OBJ_PART_MAIN);
		lv_obj_set_size(lvCircTouchPoint, 10, 10);
		lv_obj_set_style_local_radius(lvCircTouchPoint, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 5);
		lv_obj_set_style_local_bg_color(lvCircTouchPoint, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_RED);
		lv_obj_set_style_local_bg_opa(lvCircTouchPoint, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_COVER);
		lv_obj_set_pos(lvCircTouchPoint, LV_HOR_RES_MAX * 0.1, LV_VER_RES_MAX * 0.1); // Upper Left Point
	}
}

void dismiss_touchscreen_calib_widgets()
{
	lv_obj_del_async(lvContCalib);
	lvContCalib = NULL;
	lvLblCalibPrompt = NULL;
	lvCircTouchPoint = NULL;
}

static void msgbox_event_handler(lv_obj_t* pMsgBox, lv_event_t e)
{
	if(e == LV_EVENT_VALUE_CHANGED)
	{
		if(strcmp(lv_msgbox_get_active_btn_text(pMsgBox), "关闭") == 0 )
		{
			osMutexAcquire(mtxGUIWidgetsHandle, osWaitForever);
			lv_obj_del(pMsgBox);
			osMutexRelease(mtxGUIWidgetsHandle);
			osSemaphoreRelease(sphWarnMsgBoxDismissed);
		}
	}
}

osSemaphoreId_t sphWarnMsgBoxDismissed = NULL;
lv_obj_t* show_msgbox_warning(const char* title, const char* content)
{
	static const uint16_t MSGBOX_WIDTH = LV_HOR_RES_MAX / 4;
	static const uint16_t MSGBOX_HEIGHT = LV_VER_RES_MAX / 4;
	static const char* btns[] = {"关闭", ""};
	static const osSemaphoreAttr_t WarnMsgBoxDismissedSemaphore_attr = {
		  .name = "WarnMsgBoxDismissSema",
		  .cb_mem = NULL,
		  .cb_size = 0,
	};
	// use a semaphore to notify the Alert task that the popup window has been dismissed
	if(sphWarnMsgBoxDismissed == NULL)
	{
		sphWarnMsgBoxDismissed = osSemaphoreNew(1, 0, &WarnMsgBoxDismissedSemaphore_attr);
	}
	lv_obj_t* scr = lv_scr_act();
	lv_obj_t* pMsgBox = lv_msgbox_create(scr, NULL);
	lv_msgbox_set_text(pMsgBox, content);
	lv_msgbox_add_btns(pMsgBox, btns);
	lv_obj_set_width(pMsgBox, MSGBOX_WIDTH);
	lv_obj_set_event_cb(pMsgBox, msgbox_event_handler);
	return pMsgBox;
}

static void bandsel_event_handler(lv_obj_t* pBandsel, lv_event_t e)
{
	if(e == LV_EVENT_VALUE_CHANGED)
	{
//		uint16_t nBtnID = lv_msgbox_get_active_btn(pBandsel);
		const char* sBtnText = lv_msgbox_get_active_btn_text(pBandsel);
		if(strncmp(sBtnText, "取消", sizeof("取消")+1) == 0)
		{
		}
		else
		{
			uint8_t i;
			for(i = 0; i < N_BANDS; ++i)
			{
				if(strcmp(sBtnText, strBands[i]) == 0)
				{
					osMutexAcquire(mtxBandDataHandle, osWaitForever);
					cfg.Band_source = BAND_FROM_REMOTE;
					cfg.Band_soft = (band_t)i;
					osMutexRelease(mtxBandDataHandle);
					isBandSelectorShown = false;
				}
			}
		}
		osMutexAcquire(mtxGUIWidgetsHandle, osWaitForever);
		lv_obj_del(pBandsel);
		osMutexRelease(mtxGUIWidgetsHandle);
		isBandSelectorShown = false;
	}
}

lv_obj_t* show_band_selector()
{
	static const uint16_t MSGBOX_WIDTH = LV_HOR_RES_MAX / 4 * 3;
	static const uint16_t MSGBOX_HEIGHT = LV_VER_RES_MAX / 3;
	static const char* sBtns[] = {
		"160m",
		"80m",
		"40-30m",
		"20-17m",
		"15-10m",
		"\n",
		"Decoder",
		"取消",
		""
	};
	lv_obj_t* scr = lv_scr_act();
	lv_obj_t* lvMsgBox = lv_msgbox_create(scr, NULL);
	lv_obj_set_size(lvMsgBox, MSGBOX_WIDTH, MSGBOX_HEIGHT);
	lv_obj_align(lvMsgBox, scr, LV_ALIGN_CENTER, 0, -MSGBOX_HEIGHT / 2);
	lv_msgbox_set_text(lvMsgBox, "选择波段");
	lv_msgbox_add_btns(lvMsgBox, sBtns);
//	lv_obj_reset_style_list(lvMsgBox, LV_MSGBOX_PART_BG);
	lv_obj_add_style(lvMsgBox, LV_MSGBOX_PART_BG, &lvStyleMsgBox);
	lv_obj_set_event_cb(lvMsgBox, bandsel_event_handler);
	lv_obj_set_style_local_border_color(lvMsgBox, LV_MSGBOX_PART_BTN, LV_STATE_DEFAULT, LV_COLOR_GRAY);
	lv_obj_set_style_local_radius(lvMsgBox, LV_MSGBOX_PART_BTN, LV_STATE_DEFAULT, 0);
	return lvMsgBox;
}
//MsgBox_t* show_msgbox_warning(const char* title, const char* content)
//{
//	static const uint16_t MSGBOX_WIDTH = LV_HOR_RES_MAX / 3;
//	static const uint16_t MSGBOX_HEIGHT = LV_VER_RES_MAX / 3;
//	static const uint16_t TITLE_CONT_HEIGHT = MSGBOX_HEIGHT / 6;
//	static const uint16_t BODY_CONT_HEIGHT = MSGBOX_HEIGHT - 2 * TITLE_CONT_HEIGHT;
//	static const uint16_t BODY_TEXT_HEIGHT = BODY_CONT_HEIGHT * 4 / 5;
//	static const uint16_t BODY_TEXT_WIDTH = MSGBOX_WIDTH * 4 / 5;
//	static const lv_color_t colorTitleBg =  LV_COLOR_BLUE;
//	static const lv_color_t colorBodyBg = LV_COLOR_GRAY;
//	MsgBox_t* p = (MsgBox_t*)malloc(sizeof(MsgBox_t));
//	lv_obj_t* scr = lv_scr_act();
//	// title container
//	p->lvContTitle = lv_cont_create(scr, NULL);
//	lv_obj_set_size(p->lvContTitle, MSGBOX_WIDTH, TITLE_CONT_HEIGHT);
//	lv_obj_align(p->lvContTitle, NULL, LV_ALIGN_CENTER, -MSGBOX_WIDTH / 2, -MSGBOX_HEIGHT / 2);
//	lv_obj_reset_style_list(p->lvContTitle, LV_OBJ_PART_MAIN);
//	lv_obj_set_style_local_bg_color(p->lvContTitle, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, colorTitleBg);
//	lv_obj_set_style_local_radius(p->lvContTitle, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT,0);
//	// body container
//	p->lvContBody = lv_cont_create(scr, NULL);
//	lv_obj_set_size(p->lvContBody, MSGBOX_WIDTH, BODY_CONT_HEIGHT);
//	lv_obj_align(p->lvContBody, NULL, LV_ALIGN_CENTER, -MSGBOX_WIDTH / 2, -MSGBOX_HEIGHT / 2 + TITLE_CONT_HEIGHT);
//	lv_obj_reset_style_list(p->lvContBody, LV_OBJ_PART_MAIN);
//	lv_obj_set_style_local_bg_color(p->lvContBody, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, colorBodyBg);
//	lv_obj_set_style_local_radius(p->lvContBody, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT,0);
//	// title
//	p->lvLblTitle = lv_label_create(p->lvContTitle, NULL);
//	lv_obj_align(p->lvLblTitle, NULL, LV_ALIGN_IN_LEFT_MID, 5, 0);
//	lv_label_set_align(p->lvLblTitle, LV_LABEL_ALIGN_LEFT);
//	lv_obj_add_style(p->lvLblTitle, LV_LABEL_PART_MAIN, &lvStyleCHNTextLarge);
//	lv_label_set_text(p->lvLblTitle, title);
//	// content
//	p->lvLblContent = lv_label_create(p->lvContBody, NULL);
//	lv_obj_align(p->lvLblContent, NULL, LV_ALIGN_IN_TOP_MID, -BODY_TEXT_WIDTH / 2,  -BODY_TEXT_HEIGHT / 2);
//	lv_label_set_long_mode(p->lvLblContent, LV_LABEL_LONG_BREAK);
//	lv_label_set_align(p->lvLblContent, LV_LABEL_ALIGN_CENTER);
//	lv_obj_set_size(p->lvLblContent, BODY_TEXT_WIDTH, BODY_TEXT_HEIGHT);
//	lv_obj_add_style(p->lvLblContent, LV_LABEL_PART_MAIN, &lvStyleCHNTextLarge);
//	lv_label_set_text(p->lvLblContent, content);
//	// button
//	p->lvBtnOK = lv_label_create(p->lvContBody, NULL);
//	lv_obj_align(p->lvBtnOK, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, -5);
//
//
//	return p;
//}


void number2text(char* dest,
		int number, // value without decimal point
		uint8_t decimal, // digits of decimals (0: integer, 1: 1.1, 2: 1.10)
		char suffix)
{
	uint8_t digits[9];
	uint8_t n = 0, i = 0;
	if (number < 0)
	{
		number = -number;
		dest[i++] = '-';
	}

	do
	{
		digits[n++] = number % 10 + '0';
		number /= 10;
		if(n == decimal)
		    digits[n++] = '.';
	}while(number);
	// append 0 before dot
	while(n <= decimal)
	{
		if(n < decimal)
			digits[n++] = '0';
		else
			digits[n++] = '.';
	}
	// append 0 after trailing dot
	if(n > 0 && digits[n-1] == '.')
		digits[n++] = '0';
	while(n)
	{
		dest[i++] = digits[--n];
	}
	dest[i] = suffix;
	if(suffix != 0) dest[i+1] = 0;
}

void my_i16toa(char* dest, int16_t num)
{
	char* start = dest;
    if(num)
    {
        bool sign=num<0;
        num=sign?-num:num;
        while(num)
        {
            uint8_t n=num%10;
            *dest++ = '0'+n;
            num /= 10;
        }
        if(sign)
            *dest++='-';
        *dest=0;
//        reverse(res);
        dest--;
        while(start<dest)
        {
        	char temp = *start;
        	*start = *dest;
        	*dest = temp;
        	start++;
        	dest--;
        }
    }
    else
    {
        *dest++='0';
        *dest='\0';
    }
}

void my_utoa(char* dest, uint16_t num)
{
	char* start = dest;
    if(num)
    {
//        sign=num<0;
//        num=sign?-num:num;
        while(num)
        {
            uint8_t n=num%10;
            *dest++ = '0'+n;
            num /= 10;
        }
//        if(sign)
//            *dest++='-';
        *dest=0;
//        reverse(res);
        dest--;
        while(start<dest)
        {
        	char temp = *start;
        	*start = *dest;
        	*dest = temp;
        	start++;
        	dest--;
        }
    }
    else
    {
        *dest++='0';
        *dest='\0';
    }
}

void GUI_set_transmission_state(Transmission_State_t state)
{
	const char* content = NULL;
	switch(state)
	{
	case STANDBY:
		content = strTransmissionStateActive;
		break;
	case RECEIVING:
		content = strTransmissionStateIdle;
		break;
	case TRANSMITTING:
		content = strTransmissionStateOnAir;
	}
	lv_label_set_text_static(lvLblTransmissionState, content);
}

void GUI_set_band_info(band_t band)
{
	if(lvLblFreqRange == NULL)
		return;
	lv_label_set_text(lvLblFreqRange, strFreqRanges[(uint8_t)band]);
}

void GUI_set_band_decoder_info(BandMode_t mode)
{
	if(lvLblBandDecoderMode == NULL)
		return;
	switch(mode)
	{
	case BAND_MODE_ICOM:
		strcpy(strBandDecoderMode, "ICOM");
		break;
	case BAND_MODE_YAESU:
		strcpy(strBandDecoderMode, "YAESU");
		break;
	case BAND_MODE_KENWOOD:
		strcpy(strBandDecoderMode, "KENWOOD");
		break;
	default:
		*strBandDecoderMode = 0;
	}
	lv_label_set_text_static(lvLblBandDecoderMode, strBandDecoderMode);
}

void GUI_set_FWD_progbar(uint16_t watt)
{
	float FWD1_progress = (logf(watt / PWR_LOGSCALE_Y0) / PWR_LOGSCALE_LOGR
			* PWR_LOGSCALE_ANCHORPOINT_RANGE
			+ PWR_LOGSCALE_X0); // 0-1.0f
	lv_bar_set_value(lvProgbarFwdPwr,
			FWD1_progress * PROGBAR_MAX_VALUE_FWDPWR,
			LV_ANIM_OFF);
}

void GUI_set_FWD_peak_value(uint16_t watt)
{
	static char sValue[8] = "----W";
	if(lvLblFwdPwrValue == NULL) return;
	number2text(sValue, watt, 0, 'W');

	lv_label_set_text_static(lvLblFwdPwrValue, sValue);
}

void GUI_set_REV_progbar(uint16_t watt)
{
	float REV1_progress = (logf(watt / PWR_LOGSCALE_Y0) / PWR_LOGSCALE_LOGR
			* PWR_LOGSCALE_ANCHORPOINT_RANGE
			+ PWR_LOGSCALE_X0); // 0-1.0f
	lv_bar_set_value(lvProgbarRevPwr,
			REV1_progress * PROGBAR_MAX_VALUE_REVPWR,
			LV_ANIM_OFF);
}


void GUI_set_REV_peak_value(uint16_t watt)
{
	static char sValue[8] = "----W";
	if(lvLblRevPwrValue == NULL) return;
	number2text(sValue, watt, 0, 'W');
	lv_label_set_text_static(lvLblRevPwrValue, sValue);

}

void GUI_set_SWR_progbar(float SWR)
{
	if(lvProgbarSWR == NULL) return;
	float SWR_progress = (logf(SWR / SWR_LOGSCALE_Y0) / SWR_LOGSCALE_LOGR
			* SWR_LOGSCALE_ANCHORPOINT_RANGE
			+ SWR_LOGSCALE_X0); // 0-1.0f
	lv_bar_set_value(lvProgbarSWR,
			SWR_progress * PROGBAR_MAX_VALUE_SWR,
			LV_ANIM_OFF);
}

void GUI_set_SWR_peak_value(float SWR)
{
	static char sValue[5] = "-.-";
	number2text(sValue, (int)(SWR * 10), 1, '\0');
	lv_label_set_text_static(lvLblSWRValue, sValue);
}

void GUI_set_core_temperature(int16_t C)
{
	static char sValue[4] = "---";
	if(C < -99) C = -99;
	else if(C > 999) C = 999;
	my_i16toa(sValue, C);
	lv_label_set_text_static(lvLblCoreTemp, sValue);
}

void GUI_set_ambient_temperature(int16_t C)
{
	static char sValue[4] = "---";
	if(C < -99) C = -99;
	else if(C > 999) C = 999;
	my_i16toa(sValue, C);
	lv_label_set_text_static(lvLblAmbTemp, sValue);
}
void GUI_set_Imain(int mA)
{
	static char sValue[7] = "-000.0A";
	number2text(sValue, mA / 100, 1, 'A');
	lv_label_set_text_static(lvLblImain, sValue);
}
void GUI_set_Vmain(int V)
{
	static char sValue[6] = "-000V";
	number2text(sValue, V, 0, 'V');
	lv_label_set_text_static(lvLblVmain, sValue);
}

/* TS Calib Scene Methods begin */
void GUI_set_TSCalib_touchpoint_pos(uint16_t real_X, uint16_t real_Y)
{
	lv_obj_set_pos(lvCircTouchPoint, real_X , real_Y); // Upper Left Point
}
/* TS Calib Scene Methods end */
