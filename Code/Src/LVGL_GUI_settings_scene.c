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
		"校准FWD2","校准REV2", "\n",
		"检查传感器","\n",
		"风扇","\n",
		"波段通信","\n",
		"取消",
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

lv_group_t *lvGroupSettings = NULL;
static void settings_btnm_event_handler(lv_obj_t* pSettingsBtn, lv_event_t e)
{
	if(e == LV_EVENT_CLICKED)
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
#ifndef LVGL_SIM
		osMutexAcquire(mtxGUIWidgetsHandle, osWaitForever);
#endif
		lv_obj_del(lvContSettings);
		deregister_group(lvGroupSettings);
		lvGroupSettings = NULL;
#ifndef LVGL_SIM
		osMutexRelease(mtxGUIWidgetsHandle);
#endif
		lvContSettings = NULL;
		lvBtnmSettings = NULL;
	}
}

void init_settings_menu_widgets()
{
	// avoid creating widgets of settings screen more than once
	// UTF8编码
#ifndef LVGL_SIM
	osMutexAcquire(mtxGUIWidgetsHandle, osWaitForever);
#endif
	lv_obj_t* scr = lv_scr_act();
	// 创建外部控制group
	// Solid color fullscreen Background
	if(lvContSettings == NULL)
	{
		lvGroupSettings = lv_group_create();
		register_group(lvGroupSettings);
		lvContSettings = lv_obj_create(scr, NULL);
		lv_obj_set_size(lvContSettings, LV_HOR_RES_MAX, LV_VER_RES_MAX);
		lv_obj_align(lvContSettings, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 0, 0);
		//lv_obj_reset_style_list(lvContSettings, LV_OBJ_PART_MAIN);
		//lv_obj_set_style_local_bg_color(lvContSettings, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, COLOR_BACKGROUND);
		//lv_obj_set_style_local_radius(lvContSettings, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT,0);
		lv_obj_set_style_local_bg_opa(lvContSettings, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
	}
	// Button matrix
	if(lvBtnmSettings == NULL)
	{
		lvBtnmSettings = lv_btnmatrix_create(lvContSettings, NULL);
		lv_obj_set_size(lvBtnmSettings, LV_HOR_RES_MAX *3 / 4, LV_VER_RES_MAX * 7 / 8);
		lv_obj_set_style_local_bg_color(lvBtnmSettings, LV_BTNMATRIX_PART_BG, LV_STATE_DEFAULT, COLOR_BACKGROUND);
		lv_obj_set_style_local_bg_opa(lvBtnmSettings, LV_BTNMATRIX_PART_BG, LV_STATE_DEFAULT, LV_OPA_COVER);
		lv_obj_set_style_local_radius(lvBtnmSettings, LV_BTNMATRIX_PART_BG, LV_STATE_DEFAULT, 0);
		lv_obj_set_style_local_pad_inner(lvBtnmSettings, LV_BTNMATRIX_PART_BG, LV_STATE_DEFAULT, 32);
		lv_obj_add_style(lvBtnmSettings, LV_BTNMATRIX_PART_BTN, &lvStyleBtn);
		lv_obj_add_style(lvBtnmSettings, LV_BTNMATRIX_PART_BTN, &lvStyleCHNTextLarge);
		//lv_obj_set_style_local_text_font(lvBtnmSettings, LV_BTNMATRIX_PART_BTN, LV_STATE_DEFAULT, &CNSans_24);
		//lv_obj_set_style_local_text_color(lvBtnmSettings, LV_BTNMATRIX_PART_BTN, LV_STATE_DEFAULT, COLOR_PLAIN_TEXT);
		lv_obj_set_style_local_pad_top(lvBtnmSettings, LV_BTNMATRIX_PART_BTN, LV_STATE_DEFAULT, 6);
		lv_obj_set_style_local_pad_bottom(lvBtnmSettings, LV_BTNMATRIX_PART_BTN, LV_STATE_DEFAULT, 6);
		lv_obj_align(lvBtnmSettings, lvContSettings, LV_ALIGN_CENTER, 0, 0);
		lv_btnmatrix_set_map(lvBtnmSettings, sSettingsBtns);
		lv_group_add_obj(lvGroupSettings, lvBtnmSettings);
		lv_group_set_editing(lvGroupSettings, true);
		lv_obj_set_event_cb(lvBtnmSettings, settings_btnm_event_handler);
//		lv_obj_set_pos(lvBtnmSettings, x, y);
	}
#ifndef LVGL_SIM
	osMutexRelease(mtxGUIWidgetsHandle);
#endif
}
