/*
 * LVGL_items.h
 *
 *  Created on: 2020年12月27日
 *      Author: Zening
 */

#ifndef LVGL_ITEMS_H_
#define LVGL_ITEMS_H_

#include <stdbool.h>
#include "LVGL_GUI.h"
#include "MultiLinearValueMapper.h"
#include "Sensor_task.h"

/* Sensor Calibration Window BEGIN */
typedef struct {
	const char* name;
	MultiLinearValueMapper_t* pMapper;
	lv_obj_t* pGUIObj; // NULL if not shown(or deleted)
} SensorCalibrationControl_t;
void init_sensor_calibration_control_struct(SensorCalibrationControl_t* p, const char* name, MultiLinearValueMapper_t* pMapper);
// Display senosr calibration window (for Directional Coupler, NTC and etc)
lv_obj_t* create_sensor_calib_window(
		SensorCalibrationControl_t* pControl
		);
lv_obj_t* destory_sensor_calib_window(
		SensorCalibrationControl_t* pControl
		);
/* Sensor Calibration Window END */


/* Sensor readings monitoring page BEGIN */

#define TABLE_SENSOR_READING_NCOLS 6
#define TABLE_SENSOR_READING_NROWS 9

extern lv_obj_t* lvBgSensorReadingsPage;
extern lv_obj_t* lvTableSensorReadingsValues;
void show_sensor_readings_page();
void dismiss_sensor_readings_page();

/* Sensor readings monitoring page END */

/* Fan settings page BEGIN */
extern lv_obj_t* lvBgFanSettings;
extern lv_obj_t* lvBtnmFanSettings;
void show_fan_settings_page();
void dismiss_fan_settings_page();
/* Fan settings page END */

/* Communication settings page BEGIN */
extern lv_obj_t* lvSolidCommSettings;
extern lv_obj_t* lvBgCommSettings;
extern lv_obj_t* lvLblBaudCAT;
extern lv_obj_t* lvRollerBaudCAT; // Computer
extern lv_obj_t* lvLblBaudCIV;
extern lv_obj_t* lvRollerBaudBandCIV; // Band
extern lv_obj_t* lvLblBandType;
extern lv_obj_t* lvRollerBandType; // YAESU, ICOM, KENWOOD
void show_comm_setting_page();
void dismiss_comm_Setting_page();
/* Communication settings page END */
#endif /* LVGL_ITEMS_H_ */
