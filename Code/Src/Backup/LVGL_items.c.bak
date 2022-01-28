/*
 * LVGL_items.c
 *
 *  Created on: 2020年12月27日
 *      Author: Zening
 */


#include "LVGL_items.h"
#include "SoftUART.h"

/* Sensor Calibration Window BEGIN */
// LVGL task

void init_sensor_calibration_control_struct(SensorCalibrationControl_t* p, const char* name, MultiLinearValueMapper_t* pMapper)
{
	p->name = name;
	p->pMapper = pMapper;
	p->pGUIObj = NULL;
}
// Display senosr calibration window (for Directional Coupler, NTC and etc)
lv_obj_t* create_sensor_calib_window(
		SensorCalibrationControl_t* pControl
		)
{
	if(pControl->pGUIObj != NULL)
		return pControl->pGUIObj;

	MultiLinearValueMapper_t* pMapper = pControl->pMapper;
	osMutexAcquire(mtxGUIWidgetsHandle, osWaitForever);
	lv_obj_t* scr = lv_scr_act();
	// Create Background Obj
	lv_obj_t* lvContSensorBg = lv_cont_create(scr, NULL);
	lv_obj_set_size(lvContSensorBg, LV_HOR_RES_MAX, LV_VER_RES_MAX);
	lv_obj_align(lvContSensorBg, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 0, 0);
	lv_obj_reset_style_list(lvContSensorBg, LV_CONT_PART_MAIN);
	lv_obj_add_style(lvContSensorBg, LV_CONT_PART_MAIN, &lvStyleBackground);

	// Create Button Array
	for(uint8_t i = 0; i < pMapper->nAnchors; ++i)
	{

	}
	osMutexRelease(mtxGUIWidgetsHandle);
	return lvContSensorBg;

}
lv_obj_t* destory_sensor_calib_window(
		SensorCalibrationControl_t* pControl
		)
{
	return NULL;
}
/* Sensor Calibration Window END */



/* Sensor readings monitoring page BEGIN */
lv_obj_t* lvBgSensorReadingsPage = NULL;
lv_obj_t* lvTableSensorReadingsValues = NULL;
const char sADCNumers[N_SENSORS][5] = {"1.3","1.4","1.5","1.6","1.7","1.8","1.9","1.14",
		"3.1","3.10","3.11","3.12","3.13"};
const char sSensorNames[N_SENSORS][6] = {"FWD1", "FWDP1", "REVP1", "REV1","FWD2","FWDP2","REVP2","REV2",
		"Band","Vmain", "Imain", "TEMP1", "TEMP2"};
const char TableSensorReadingHeaders[3][5] = {"ADC", "Name", "Val"};

// LVGL task to refresh value displays on sensor readings table
lv_task_t* lvTaskSensorReadings = NULL;
static void lv_task_sensor_readings(lv_task_t * task)
{
	if(lvTableSensorReadingsValues == NULL) return;
	// refresh ADC values
	uint8_t idxItem = 0, i;
	static char s[5];
	osMutexAcquire(mtxGUIWidgetsHandle, osWaitForever);
	// ADC1
#if ADC1_N_CHANNELS
	for(i = 0; i < ADC1_N_CHANNELS; ++i)
	{
		uint8_t col = (idxItem / (TABLE_SENSOR_READING_NROWS - 1)) * 3 + 2; // values are displayed on the 3rd column
		uint8_t row = idxItem % (TABLE_SENSOR_READING_NROWS - 1) + 1;
		my_utoa(s, uhADC1ConvertedValues[i]);
		lv_table_set_cell_value(lvTableSensorReadingsValues, row, col, s);

		++idxItem;
	}
#endif
	// ADC2
#if ADC2_N_CHANNELS
	for(i = 0; i < ADC2_N_CHANNELS; ++i)
	{
		uint8_t col = (idxItem / (TABLE_SENSOR_READING_NROWS - 1)) * 3 + 2; // values are displayed on the 3rd column
		uint8_t row = idxItem % (TABLE_SENSOR_READING_NROWS - 1) + 1;
		my_utoa(s, uhADC2ConvertedValues[i]);
		lv_table_set_cell_value(lvTableSensorReadingsValues, row, col, s);
		++idxItem;
	}
#endif
#if ADC3_N_CHANNELS
	for(i = 0; i < ADC3_N_CHANNELS; ++i)
	{
		uint8_t col = (idxItem / (TABLE_SENSOR_READING_NROWS - 1)) * 3 + 2; // values are displayed on the 3rd column
		uint8_t row = idxItem % (TABLE_SENSOR_READING_NROWS - 1) + 1;
		my_utoa(s, uhADC3ConvertedValues[i]);
		lv_table_set_cell_value(lvTableSensorReadingsValues, row, col, s);
		++idxItem;
	}
#endif
	osMutexRelease(mtxGUIWidgetsHandle);
}

static void sensor_readings_page_event_handler(lv_obj_t* p, lv_event_t e)
{
	if(p == lvBgSensorReadingsPage)
	{
		// dismiss sensor reading page
		if(e == LV_EVENT_CLICKED)
			dismiss_sensor_readings_page();
	}
}

void show_sensor_readings_page()
{
	osMutexAcquire(mtxGUIWidgetsHandle, osWaitForever);
	lv_obj_t* scr = lv_scr_act();
	if(lvBgSensorReadingsPage == NULL)
	{
		lvBgSensorReadingsPage = lv_obj_create(scr, NULL);
		lv_obj_set_size(lvBgSensorReadingsPage, LV_HOR_RES_MAX, LV_VER_RES_MAX);
		lv_obj_reset_style_list(lvBgSensorReadingsPage, LV_CONT_PART_MAIN);
		lv_obj_add_style(lvBgSensorReadingsPage, LV_CONT_PART_MAIN, &lvStyleBackground);
		lv_obj_align(lvBgSensorReadingsPage, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 0, 0);
		lv_obj_set_event_cb(lvBgSensorReadingsPage, sensor_readings_page_event_handler);
	}
	if(lvTableSensorReadingsValues == NULL)
	{
		lvTableSensorReadingsValues = lv_table_create(lvBgSensorReadingsPage, NULL);
		lv_table_set_col_cnt(lvTableSensorReadingsValues, TABLE_SENSOR_READING_NCOLS);
		lv_table_set_row_cnt(lvTableSensorReadingsValues, TABLE_SENSOR_READING_NROWS);
		// Set text font style
		lv_obj_add_style(lvTableSensorReadingsValues, LV_TABLE_PART_BG, &lvStyleAlphabetsTextSmall);
		lv_obj_set_style_local_pad_top(lvTableSensorReadingsValues, LV_TABLE_PART_CELL1, LV_STATE_DEFAULT, 4);
		lv_obj_set_style_local_pad_bottom(lvTableSensorReadingsValues, LV_TABLE_PART_CELL1, LV_STATE_DEFAULT, 4);
		lv_obj_set_style_local_pad_inner(lvTableSensorReadingsValues, LV_TABLE_PART_CELL1, LV_STATE_DEFAULT, 4);
		// set column width
		for(uint8_t c = 0; c < TABLE_SENSOR_READING_NCOLS; ++c)
		{
			for(uint8_t r = 0; r < TABLE_SENSOR_READING_NROWS; ++r)
			{
				// align to center
				lv_table_set_cell_align(lvTableSensorReadingsValues, r, c, LV_LABEL_ALIGN_CENTER);
				// draw header
				if(r == 0)
				{
					// set column width to fill 90% of horizontal space
					lv_table_set_col_width(lvTableSensorReadingsValues, c, LV_HOR_RES_MAX * 0.9 / TABLE_SENSOR_READING_NCOLS);
					// set header text (ADC, Sensor, Value)
					lv_table_set_cell_value(lvTableSensorReadingsValues, 0, c, TableSensorReadingHeaders[c % 3]);
				}
			}
		}
		// draw ADC number,Sensor names and initialize values
		for(uint8_t idxItem = 0; idxItem < N_SENSORS; ++idxItem)
		{
			uint8_t col = (idxItem / (TABLE_SENSOR_READING_NROWS - 1)) * 3; // you need 3 columns to represent a group of sensors
			uint8_t row = idxItem % (TABLE_SENSOR_READING_NROWS - 1) + 1;
			lv_table_set_cell_value(lvTableSensorReadingsValues, row, col, sADCNumers[idxItem]);
			lv_table_set_cell_value(lvTableSensorReadingsValues, row, col + 1, sSensorNames[idxItem]);
			// initialize values to a five digit string
			lv_table_set_cell_value(lvTableSensorReadingsValues, row, col + 2, "----");
		}
		lv_obj_align_origo(lvTableSensorReadingsValues, NULL, LV_ALIGN_CENTER, 0, 0);
		// initialize the lvgl task to keep refreshing values
		lvTaskSensorReadings = lv_task_create(lv_task_sensor_readings, 200, LV_TASK_PRIO_MID, NULL);
	}
	osMutexRelease(mtxGUIWidgetsHandle);
}

void dismiss_sensor_readings_page()
{
	osMutexAcquire(mtxGUIWidgetsHandle, osWaitForever);
	if(lvBgSensorReadingsPage)
	{
		lv_obj_del_async(lvBgSensorReadingsPage);
		lvBgSensorReadingsPage = NULL;
		lvTableSensorReadingsValues = NULL;
	}
	if(lvTaskSensorReadings)
	{
		lv_task_del(lvTaskSensorReadings);
		lvTaskSensorReadings = NULL;
	}
	osMutexRelease(mtxGUIWidgetsHandle);
}

/* Sensor readings monitoring page END */


/* Fan settings page BEGIN */
static const char* sFanSettings[] = {
		"20%", "40%", "60%", "80%", "100%", "AUTO", ""
};
static const uint8_t FanSpeeds[] = {20, 40, 60, 80, 100};

lv_obj_t* lvBgFanSettings = NULL;
lv_obj_t* lvBtnmFanSettings = NULL;

static void fan_settings_btnm_event_handler(lv_obj_t* pBtnm, lv_event_t e)
{
	if(e == LV_EVENT_PRESSED)
	{
//		uint16_t nBtnID = lv_msgbox_get_active_btn(pBandsel);
//		const char* sBtnText = lv_msgbox_get_active_btn_text(pSettingsBtn);
		uint16_t idxBtn = lv_btnmatrix_get_active_btn(pBtnm);
		if(idxBtn < (sizeof(FanSpeeds) / sizeof(uint8_t)))
		{
			uint8_t fanspeed = FanSpeeds[idxBtn];
			soft_pwm_driver_set_duty(&fan1, fanspeed);
			soft_pwm_driver_set_duty(&fan2, fanspeed);
			cfg.Fan1Auto = false;
			cfg.Fan2Auto = false;
		}
		if(idxBtn == (sizeof(FanSpeeds) / sizeof(uint8_t))) // AUTO
		{
			cfg.Fan1Auto = true;
			cfg.Fan2Auto = true;
		}
		else // dismiss window
		{
			dismiss_fan_settings_page();
		}
	}
}

void show_fan_settings_page()
{
	if(lvBtnmFanSettings == NULL)
	{
		lv_obj_t* scr = lv_scr_act();
		lvBtnmFanSettings = lv_btnmatrix_create(scr, NULL);
		lv_obj_set_style_local_bg_color(lvBtnmFanSettings, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, COLOR_FOREGROUND);
		lv_obj_set_style_local_text_font(lvBtnmFanSettings, LV_BTNMATRIX_PART_BTN, LV_STATE_DEFAULT, &CNSans_24);
		lv_obj_set_style_local_text_color(lvBtnmFanSettings, LV_BTNMATRIX_PART_BTN, LV_STATE_DEFAULT, COLOR_PLAIN_TEXT);
		lv_obj_set_style_local_radius(lvBtnmFanSettings, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
		lv_obj_set_style_local_bg_opa(lvBtnmFanSettings, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_COVER);
		lv_btnmatrix_set_map(lvBtnmFanSettings, sFanSettings);
		lv_obj_align(lvBtnmFanSettings, lvContSettings, LV_ALIGN_CENTER, 0, 0);
		lv_obj_set_event_cb(lvBtnmFanSettings, fan_settings_btnm_event_handler);
	}
}
void dismiss_fan_settings_page()
{
	osMutexAcquire(mtxGUIWidgetsHandle, osWaitForever);
	if(lvBtnmFanSettings != NULL)
	{
		lv_obj_del_async(lvBtnmFanSettings);
		lvBtnmFanSettings = NULL;
	}
	osMutexRelease(mtxGUIWidgetsHandle);
}
/* Fan settings page END */


/* Communication settings page BEGIN */
lv_obj_t* lvSolidCommSettings = NULL;
lv_obj_t* lvBgCommSettings = NULL;
lv_obj_t* lvLblBaudCAT = NULL;
lv_obj_t* lvRollerBaudCAT = NULL; // Computer
const char* optsRollerBaudrates = "1200\n" "2400\n" "4800\n" "9600\n" "19200";
lv_obj_t* lvLblBaudCIV = NULL;
lv_obj_t* lvRollerBaudBandCIV = NULL; // Band
lv_obj_t* lvLblBandType = NULL;
lv_obj_t* lvRollerBandType = NULL; // YAESU, ICOM, KENWOOD
const char* optsRollerBandTypes = "ICOM CIV\n" "Yaesu\n" "Kenwood";
lv_obj_t* lvBtnCommSettingsOK = NULL;
lv_obj_t* lvLblBtnCommSettingsOK = NULL;

static void comm_settings_event_handler(lv_obj_t * obj, lv_event_t event)
{
	if(obj == lvBtnCommSettingsOK && event == LV_EVENT_RELEASED)
	{
		cfg.baudBand = BaudRates[lv_roller_get_selected(lvRollerBaudBandCIV)];
		cfg.baudCAT = BaudRates[lv_roller_get_selected(lvRollerBaudCAT)];
		cfg.bandMode = (BandMode_t)lv_roller_get_selected(lvRollerBandType);
		// All the changes will be effect by a timed task
		dismiss_comm_Setting_page();
		osThreadFlagsSet(defaultTaskHandle, THREAD_FLAG_SAVE_CONFIG); // send a signal to save config
	}
}

void show_comm_setting_page()
{
	if(lvSolidCommSettings != NULL) return;
	lv_obj_t* scr = lv_scr_act();
	lvSolidCommSettings = lv_obj_create(scr, NULL);
	lv_obj_set_size(lvSolidCommSettings, LV_HOR_RES_MAX * 3/4, LV_VER_RES_MAX * 3 / 4);
	lv_obj_add_style(lvSolidCommSettings, LV_OBJ_PART_MAIN, &lvStyleBackground);
	lv_obj_align(lvSolidCommSettings, NULL, LV_ALIGN_CENTER, 0, 0);

	lvBgCommSettings = lv_cont_create(lvSolidCommSettings, NULL);
	lv_obj_set_size(lvBgCommSettings, LV_HOR_RES_MAX *3 / 4, LV_VER_RES_MAX *3 / 4);
	lv_obj_reset_style_list(lvBgCommSettings, LV_CONT_PART_MAIN);
	lv_obj_add_style(lvBgCommSettings, LV_CONT_PART_MAIN, &lvStyleBackground);
//	lv_cont_set_fit(lvBgCommSettings, LV_FIT_TIGHT);
	lv_cont_set_layout(lvBgCommSettings, LV_LAYOUT_PRETTY_MID);
	lv_obj_set_auto_realign(lvBgCommSettings, true);


	lvLblBaudCAT = lv_label_create(lvBgCommSettings, NULL);
	lv_label_set_align(lvLblBaudCAT , LV_LABEL_ALIGN_CENTER);
	lv_obj_add_style(lvLblBaudCAT, LV_LABEL_PART_MAIN, &lvStyleCHNTextLarge);
	lv_label_set_text(lvLblBaudCAT , "CAT");

	lvRollerBaudCAT = lv_roller_create(lvBgCommSettings, NULL);
	lv_obj_reset_style_list(lvRollerBaudCAT, LV_ROLLER_PART_BG);
	lv_obj_add_style(lvRollerBaudCAT, LV_ROLLER_PART_BG, &lvStyleRoller);
	lv_obj_add_style(lvRollerBaudCAT, LV_ROLLER_PART_SELECTED, &lvStyleRollerSel);
	lv_roller_set_anim_time(lvRollerBaudCAT, 0);
	lv_roller_set_visible_row_count(lvRollerBaudCAT, 3);
	lv_roller_set_options(lvRollerBaudCAT, optsRollerBaudrates, LV_ROLLER_MODE_NORMAL);
	int8_t idxBaudrateCAT =  baudrate2idx(cfg.baudCAT);
	if(idxBaudrateCAT < 0)
	{
		cfg.baudCAT = 9600; // default
		idxBaudrateCAT = baudrate2idx(cfg.baudCAT);
	}
	lv_roller_set_selected(lvRollerBaudCAT, idxBaudrateCAT, LV_ANIM_OFF);

	lvLblBaudCIV = lv_label_create(lvBgCommSettings, lvLblBaudCAT);
	lv_label_set_text(lvLblBaudCIV, "BAND");

	lvRollerBaudBandCIV = lv_roller_create(lvBgCommSettings, NULL);
	lv_obj_reset_style_list(lvRollerBaudBandCIV, LV_ROLLER_PART_BG);
	lv_obj_add_style(lvRollerBaudBandCIV, LV_ROLLER_PART_BG, &lvStyleRoller);
	lv_obj_add_style(lvRollerBaudBandCIV, LV_ROLLER_PART_SELECTED, &lvStyleRollerSel);
	lv_roller_set_anim_time(lvRollerBaudBandCIV, 0);
	lv_roller_set_visible_row_count(lvRollerBaudBandCIV, 3);
	lv_roller_set_options(lvRollerBaudBandCIV, optsRollerBaudrates, LV_ROLLER_MODE_NORMAL);
	int8_t idxBaudBandCIV = baudrate2idx(cfg.baudBand);
	if(idxBaudBandCIV < 0)
	{
		cfg.baudBand = 1200;
		idxBaudBandCIV = baudrate2idx(cfg.baudBand);
	}
	lv_roller_set_selected(lvRollerBaudBandCIV, idxBaudBandCIV, LV_ANIM_OFF);

	lvLblBandType = lv_label_create(lvBgCommSettings, lvLblBaudCAT);
	lv_label_set_text(lvLblBandType, "MODE");

	lvRollerBandType = lv_roller_create(lvBgCommSettings, NULL);
	lv_obj_reset_style_list(lvRollerBandType, LV_ROLLER_PART_BG);
	lv_obj_add_style(lvRollerBandType, LV_ROLLER_PART_BG, &lvStyleRoller);
	lv_obj_add_style(lvRollerBandType, LV_ROLLER_PART_SELECTED, &lvStyleRollerSel);
	lv_roller_set_anim_time(lvRollerBandType, 0);
	lv_roller_set_visible_row_count(lvRollerBandType, 3);
	lv_roller_set_options(lvRollerBandType, optsRollerBandTypes, LV_ROLLER_MODE_NORMAL);
	if(cfg.bandMode >= N_BAND_MODES)
		cfg.bandMode = 0;
	lv_roller_set_selected(lvRollerBandType, (uint8_t)cfg.bandMode, LV_ANIM_OFF);

	lvBtnCommSettingsOK = lv_btn_create(lvBgCommSettings, NULL);
	lv_obj_set_size(lvBtnCommSettingsOK, 80, 48);
	lv_obj_reset_style_list(lvBtnCommSettingsOK, LV_BTN_PART_MAIN);
	lv_obj_add_style(lvBtnCommSettingsOK, LV_BTN_PART_MAIN, &lvStyleBtn);
	lv_obj_set_event_cb(lvBtnCommSettingsOK, comm_settings_event_handler);

	lvLblBtnCommSettingsOK = lv_label_create(lvBtnCommSettingsOK, NULL);
	lv_obj_reset_style_list(lvLblBtnCommSettingsOK, LV_LABEL_PART_MAIN);
	lv_obj_add_style(lvLblBtnCommSettingsOK, LV_LABEL_PART_MAIN, &lvStyleCHNTextSmall);
	lv_label_set_text(lvLblBtnCommSettingsOK,"OK");

	lv_obj_align(lvBgCommSettings, lvSolidCommSettings, LV_ALIGN_CENTER, 0, 0);

}

void dismiss_comm_Setting_page()
{
	if(lvSolidCommSettings)
	{
		lv_obj_del_async(lvSolidCommSettings);
		lvSolidCommSettings= NULL;
	}
}
/* Communication settings page END */

