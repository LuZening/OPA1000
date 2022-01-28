/*
 * LVGL_GUI_Settings.c
 *
 *  Created on: 2020年12月23日
 *      Author: Zening
 */


#include "LVGL_GUI.h"
#include "LVGL_items.h"

lv_obj_t* lvContSettings = NULL;
lv_obj_t* lvBtnmSettings = NULL;

const char* sSettingsBtns[] = {
		"校准FWD1", "校准REV1","\n",
		"校准FWD2","校准REV2","\n",
		"检查SENSOR","\n",
		"FAN","\n",
		"COM","\n",
		"EXIT",
		""};

// collection of callback functions when the button matrix of settings page is pressed
void (*settingsBtnmContFuncs[])(void) = {
		NULL, NULL,
		NULL, NULL,
		show_sensor_readings_page,
		show_fan_settings_page,
		show_comm_setting_page,
		dismiss_settings_menu_widgets
};

static void settings_btnm_event_handler(lv_obj_t* pSettingsBtn, lv_event_t e)
{
	if(e == LV_EVENT_PRESSED)
	{
//		uint16_t nBtnID = lv_msgbox_get_active_btn(pBandsel);
//		const char* sBtnText = lv_msgbox_get_active_btn_text(pSettingsBtn);
		uint16_t idxBtn = lv_btnmatrix_get_active_btn(pSettingsBtn);
		if(idxBtn < (sizeof(settingsBtnmContFuncs) / sizeof(void*)))
		{
			// invoke callback function
			void (*pFunc)(void) = settingsBtnmContFuncs[idxBtn];
			if(pFunc)
				pFunc();
			else
				dismiss_settings_menu_widgets();
		}
	}


}

void dismiss_settings_menu_widgets()
{
	if(lvContSettings)
	{
		osMutexAcquire(mtxGUIWidgetsHandle, osWaitForever);
		lv_obj_del_async(lvContSettings);
		osMutexRelease(mtxGUIWidgetsHandle);
		lvContSettings = NULL;
		lvBtnmSettings = NULL;
	}
}

void init_settings_menu_widgets()
{
	// avoid creating widgets of settings screen more than once
	// UTF8编码

	osMutexAcquire(mtxGUIWidgetsHandle, osWaitForever);
	lv_obj_t* scr = lv_scr_act();
	// Solid color fullscreen Background
	if(lvContSettings == NULL)
	{
		lvContSettings = lv_obj_create(scr, NULL);
		lv_obj_set_size(lvContSettings, LV_HOR_RES_MAX, LV_VER_RES_MAX);
		lv_obj_align(lvContSettings, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 0, 0);
		lv_obj_reset_style_list(lvContSettings, LV_OBJ_PART_MAIN);
		lv_obj_set_style_local_bg_color(lvContSettings, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, COLOR_BACKGROUND);
		lv_obj_set_style_local_radius(lvContSettings, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT,0);
		lv_obj_set_style_local_bg_opa(lvContSettings, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_COVER);
	}
	// Button matrix
	if(lvBtnmSettings == NULL)
	{
		lvBtnmSettings = lv_btnmatrix_create(lvContSettings, NULL);
		lv_obj_set_size(lvBtnmSettings, LV_HOR_RES_MAX *3 / 4, LV_VER_RES_MAX * 7 / 8);
		lv_obj_set_style_local_bg_color(lvBtnmSettings, LV_BTNMATRIX_PART_BG, LV_STATE_DEFAULT, COLOR_FOREGROUND);
		lv_obj_set_style_local_text_font(lvBtnmSettings, LV_BTNMATRIX_PART_BTN, LV_STATE_DEFAULT, &CNSans_24);
		lv_obj_set_style_local_text_color(lvBtnmSettings, LV_BTNMATRIX_PART_BTN, LV_STATE_DEFAULT, COLOR_PLAIN_TEXT);
		lv_obj_set_style_local_radius(lvBtnmSettings, LV_BTNMATRIX_PART_BG, LV_STATE_DEFAULT, 0);
		lv_obj_set_style_local_bg_opa(lvBtnmSettings, LV_BTNMATRIX_PART_BG, LV_STATE_DEFAULT, LV_OPA_COVER);
		lv_obj_set_style_local_pad_inner(lvBtnmSettings, LV_BTNMATRIX_PART_BG, LV_STATE_DEFAULT, 32);
		lv_obj_set_style_local_pad_top(lvBtnmSettings, LV_BTNMATRIX_PART_BTN, LV_STATE_DEFAULT, 6);
		lv_obj_set_style_local_pad_bottom(lvBtnmSettings, LV_BTNMATRIX_PART_BTN, LV_STATE_DEFAULT, 6);
		lv_btnmatrix_set_map(lvBtnmSettings, sSettingsBtns);
		lv_obj_align(lvBtnmSettings, lvContSettings, LV_ALIGN_CENTER, 0, 0);
		lv_obj_set_event_cb(lvBtnmSettings, settings_btnm_event_handler);
//		lv_obj_set_pos(lvBtnmSettings, x, y);
	}
	osMutexRelease(mtxGUIWidgetsHandle);
}
