/*
 *  DA probe
 *
 * Copyright (c) 2000 - 2016 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Contact:
 *
 * Jaewon Lim <jaewon81.lim@samsung.com>
 * Woojin Jung <woojin2.jung@samsung.com>
 * Juyoung Kim <j0.kim@samsung.com>
 * Anatolii Nikulin <nikulin.a@samsung.com>
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation; either version 2.1 of the License, or (at your option)
 * any later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc., 51
 * Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *
 * Contributors:
 * - S-Core Co., Ltd
 *
 */

#include <vconf.h>
#include <sensor_internal.h>

#include "daprobe.h"
#include "dahelper.h"
#include "common_probe_init.h"
#include "event_probes_list.h"
#include "probe_event.h"

struct rotation_s {
	int handle;
	int angle;
	int auto_rotate;
};

static struct rotation_s rotation;
static int rotation_initialized = 0;

static int __get_angle(sensor_data_t *data)
{
	int event, m;

	m = 0;
	if (data->value_count > 0) {
		event = (int)data->values[0];
	} else {
		PRINTERR("Failed to get sensor data");
		return 0;
	}

	switch (event) {
	case AUTO_ROTATION_DEGREE_0:
		m = 0;
		break;

	case AUTO_ROTATION_DEGREE_90:
		m = 90;
		break;

	case AUTO_ROTATION_DEGREE_180:
		m = 180;
		break;

	case AUTO_ROTATION_DEGREE_270:
		m = 270;
		break;

	default:
		break;
	}

	return m;
}

static void __auto_rotate_screen_cb(keynode_t *key,
				    void __attribute__((unused)) *data)
{
	rotation.auto_rotate = vconf_keynode_get_bool(key);
}

static void __rotation_changed_cb(sensor_t __attribute__((unused)) sensor,
				  unsigned int event_type,
				  sensor_data_t *data,
				  void __attribute__((unused)) *user_data)
{
	if (event_type != AUTO_ROTATION_CHANGE_STATE_EVENT)
		return;

	rotation.angle = __get_angle(data);
	if (!rotation.auto_rotate)
		return;

	on_orientation_changed(rotation.angle, false);

}

static int __rotate_event_init(void)
{
	bool r;
	sensor_data_t data;
	const char *fname;
	static bool (*__sensord_start)(int handle, int option);
	static int (*__sensord_connect)(sensor_t sensor);
	static bool (*__sensord_disconnect)(int handle);
	static sensor_t (*__sensord_get_sensor)(sensor_type_t type);
	static bool (*__sensord_register_event)(int handle,
						unsigned int event_type,
						unsigned int interval,
						unsigned int max_batch_latency,
						sensor_cb_t cb,
						void *user_data);
	static bool (*__sensord_unregister_event)(int handle,
						  unsigned int event_type);
	static bool (*__sensord_get_data)(int handle, unsigned int data_id,
					  sensor_data_t* sensor_data);

	fname = "sensord_start";
	rtld_default_set_once(__sensord_start, fname);
	if (!__sensord_start)
		goto not_found;


	fname = "sensord_connect";
	rtld_default_set_once(__sensord_connect, fname);
	if (!__sensord_connect)
		goto not_found;

	fname = "sensord_disconnect";
	rtld_default_set_once(__sensord_disconnect, fname);
	if (!__sensord_disconnect)
		goto not_found;

	fname = "sensord_get_sensor";
	rtld_default_set_once(__sensord_get_sensor, fname);
	if (!__sensord_get_sensor)
		goto not_found;

	fname = "sensord_register_event";
	rtld_default_set_once(__sensord_register_event, fname);
	if (!__sensord_register_event)
		goto not_found;

	fname = "sensord_unregister_event";
	rtld_default_set_once(__sensord_unregister_event, fname);
	if (!__sensord_unregister_event)
		goto not_found;

	fname = "sensord_get_data";
	rtld_default_set_once(__sensord_get_data, fname);
	if (!__sensord_get_data)
		goto not_found;

	sensor_t sensor = __sensord_get_sensor(AUTO_ROTATION_SENSOR);

	rotation.angle = 0;
	rotation.handle = __sensord_connect(sensor);
	if (rotation.handle < 0) {
		PRINTERR("Failed to connect sensord");
		return -1;
	}

	r = __sensord_register_event(rotation.handle,
				     AUTO_ROTATION_CHANGE_STATE_EVENT,
				     SENSOR_INTERVAL_NORMAL,
				     0,
				     __rotation_changed_cb,
				     NULL);
	if (!r) {
		PRINTERR("Failed to register event");
		__sensord_disconnect(rotation.handle);
		return -1;
	}

	r = __sensord_start(rotation.handle, 0);
	if (!r) {
		PRINTERR("Failed to start sensord");
		__sensord_unregister_event(rotation.handle,
					   AUTO_ROTATION_CHANGE_STATE_EVENT);
		__sensord_disconnect(rotation.handle);
		return -1;
	}

	rotation.auto_rotate = 0;
	vconf_get_bool(VCONFKEY_SETAPPL_AUTO_ROTATE_SCREEN_BOOL,
		       &rotation.auto_rotate);

	if (vconf_notify_key_changed(VCONFKEY_SETAPPL_AUTO_ROTATE_SCREEN_BOOL,
				     __auto_rotate_screen_cb, NULL) != 0)
		PRINTERR("Failed to register callback for %s",
			 VCONFKEY_SETAPPL_AUTO_ROTATE_SCREEN_BOOL);

	r = __sensord_get_data(rotation.handle, AUTO_ROTATION_SENSOR, &data);
	if (!r) {
		PRINTMSG("sensord_get_data failed");
		rotation.angle = 0;
	} else {
		rotation.angle = __get_angle(&data);
	}
	current_angle_set(rotation.angle);
	rotation_initialized = 1;

	return 0;

not_found:
	PRINTERR("Failed to get symbol \"%s\"\n", fname);

	return -1;
}

EAPI int PROBE_NAME(ecore_wl_init)(const char *name)
{
	/* TODO Support old preload */
	int res = 0;
//	static int (*ecore_wl_initp)(const char *name);
	int (*ecore_wl_initp)(const char *name);
	PRINTMSG("(%s)", name);

	ecore_wl_initp = (void *)GET_ORIG_FUNC(event_feature, ecore_wl_init);

//	rtld_default_set_once(ecore_wl_initp, "ecore_wl_init");
	res = ecore_wl_initp(name);

	if (!rotation_initialized) {
		if (__rotate_event_init() < 0)
			PRINTERR("Failed to initialize rotation");
	}

	return res;
}
