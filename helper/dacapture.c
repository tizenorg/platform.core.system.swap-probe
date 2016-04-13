/*
 *  DA probe
 *
 * Copyright (c) 2000 - 2013 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Contact:
 *
 * Jaewon Lim <jaewon81.lim@samsung.com>
 * Woojin Jung <woojin2.jung@samsung.com>
 * Juyoung Kim <j0.kim@samsung.com>
 * Anastasia Lyupa <a.lyupa@samsung.com>
 * Anatolii Nukulin <nikulin.a@samsung.com>
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
 * - Samsung RnD Institute Russia
 *
 */
/*
 * We refer to the Evas example source code with BSD
 * (Evas/src/examples/evas-buffer-simple.c)
 *
 */

#include <stdlib.h>		// for system
#include <sys/types.h>		// for stat, getpid
#include <sys/stat.h>		// fot stat, chmod
#include <unistd.h>		// fot stat, getpid
#include <pthread.h>		// for mutex
#include <sys/param.h>		// MIN, MAX
#include <sys/mman.h>		// mmap

#include <Ecore.h>
#include <Evas.h>
#include <Evas_Engine_Buffer.h>

#include <wayland-client.h>
#include <wayland-tbm-client.h>
#include <tbm_bufmgr.h>
#include <tbm_surface.h>
#include <tbm_surface_internal.h>
#include <tizen-extension-client-protocol.h>
#include <screenshooter-client-protocol.h>

#include "real_functions.h"
#include "daprobe.h"
#include "dahelper.h"

#include "binproto.h"

#define MAX_HEIGHT		720
#define CAPTURE_TIMEOUT		2.0
#define MAX_PATH_LENGTH 256

struct efl_data {
	struct wl_display *wl_display;
	struct wl_registry *wl_registry;
	struct screenshooter *screenshooter;
	struct wl_list output_list;
	int min_x, min_y, max_x, max_y;
	int buffer_copy_done;
	int width, height;
};

struct screenshooter_output {
	struct wl_output *output;
	struct wl_buffer *buffer;
	int width, height, offset_x, offset_y;
	void *data;
	struct wl_list link;
};

static void
display_handle_geometry(void __attribute__((unused)) *data,
			struct wl_output *wl_output,
			int x,
			int y,
			int __attribute__((unused)) physical_width,
			int __attribute__((unused)) physical_height,
			int __attribute__((unused)) subpixel,
			const char __attribute__((unused)) *make,
			const char __attribute__((unused)) *model,
			int __attribute__((unused)) transform)
{
	struct screenshooter_output *output;

	output = wl_output_get_user_data(wl_output);

	if (wl_output == output->output) {
		output->offset_x = x;
		output->offset_y = y;
	}
}

static void
display_handle_mode(void __attribute__((unused)) *data,
		    struct wl_output *wl_output,
		    uint32_t flags,
		    int width,
		    int height,
		    int __attribute__((unused)) refresh)
{
	struct screenshooter_output *output;

	output = wl_output_get_user_data(wl_output);

	if ((wl_output == output->output) &&
	    (flags & WL_OUTPUT_MODE_CURRENT)) {
		output->width = width;
		output->height = height;
	}
}

static const struct wl_output_listener output_listener = {
	display_handle_geometry,
	display_handle_mode,
	NULL,
	NULL
};

static void
screenshot_done(void *data,
		struct screenshooter __attribute__((unused)) *screenshooter)
{
	struct efl_data *sdata = data;
	sdata->buffer_copy_done = 1;
}

static const struct screenshooter_listener screenshooter_listener = {
	screenshot_done
};

static void
handle_global(void *data, struct wl_registry *registry,
	      uint32_t name, const char *interface,
	      uint32_t __attribute__((unused)) version)
{
	struct efl_data *sdata = data;

	if (strcmp(interface, "wl_output") == 0) {
		struct screenshooter_output *output = malloc(sizeof(*output));

		if (output) {
			output->output = wl_registry_bind(registry, name,
							  &wl_output_interface,
							  1);
			wl_list_insert(&sdata->output_list, &output->link);
			wl_output_add_listener(output->output,
					       &output_listener,
					       output);
		}
	} else if (strcmp(interface, "screenshooter") == 0) {
		sdata->screenshooter = wl_registry_bind(registry, name,
						 &screenshooter_interface,
						 1);
		screenshooter_add_listener(sdata->screenshooter,
					   &screenshooter_listener,
					   sdata);
	}
}

static const struct wl_registry_listener registry_listener = {
	handle_global,
	NULL
};

static void  __set_buffer_size(struct efl_data *data)
{
	struct screenshooter_output *output;

	data->min_x = data->min_y = INT_MAX;
	data->max_x = data->max_y = INT_MIN;
	int position = 0;

	wl_list_for_each_reverse(output, &data->output_list, link) {
		output->offset_x = position;
		position += output->width;
	}

	wl_list_for_each(output, &data->output_list, link) {
		data->min_x = MIN(data->min_x, output->offset_x);
		data->min_y = MIN(data->min_y, output->offset_y);
		data->max_x = MAX(data->max_x, output->offset_x + output->width);
		data->max_y = MAX(data->max_y, output->offset_y + output->height);
	}

	if (data->max_x <= data->min_x || data->max_y <= data->min_y) {
		data->width = 0;
		data->height = 0;
	} else {
		data->width = data->max_x - data->min_x;
		data->height = data->max_y - data->min_y;
	}
}

static Evas* create_canvas(int width, int height)
{
	static void (*__evas_output_method_set_p)(Evas *e, int render_method);
	static void (*__evas_output_size_set_p)(Evas *e, int w, int h);
	static void (*__evas_output_viewport_set_p)(Evas *e, Evas_Coord x,
						    Evas_Coord y, Evas_Coord w,
						    Evas_Coord h);
	static int (*__evas_render_method_lookup_p)(const char *name);
	static Evas_Engine_Info *(*__evas_engine_info_get_p)(const Evas *e);
	static Eina_Bool (*__evas_engine_info_set_p)(const Evas *e,
						     Evas_Engine_Info *info);
	static Evas *(*__evas_new_p)(void);
	static void (*__evas_free_p)(Evas *e);
	Evas *canvas;
	Evas_Engine_Info_Buffer *einfo;
	int method;
	void *pixels;
	Eina_Bool ret;

	rtld_default_set_once(__evas_output_method_set_p,
			      "evas_output_method_set");
	rtld_default_set_once(__evas_output_size_set_p,
			      "evas_output_size_set");
	rtld_default_set_once(__evas_output_viewport_set_p,
			      "evas_output_viewport_set");
	rtld_default_set_once(__evas_render_method_lookup_p,
			      "evas_render_method_lookup");
	rtld_default_set_once(__evas_engine_info_get_p,
			      "evas_engine_info_get");
	rtld_default_set_once(__evas_engine_info_set_p,
			      "evas_engine_info_set");
	rtld_default_set_once(__evas_new_p, "evas_new");
	rtld_default_set_once(__evas_free_p, "evas_free");

	method = __evas_render_method_lookup_p("buffer");
	if (unlikely(method <= 0)) {
		PRINTERR("evas was not compiled with 'buffer' engine!");
		return NULL;
	}

	canvas = __evas_new_p();
	if (unlikely(canvas == NULL)) {
		PRINTERR("could not instantiate new evas canvas");
		return NULL;
	}

	__evas_output_method_set_p(canvas, method);
	__evas_output_size_set_p(canvas, width, height);
	__evas_output_viewport_set_p(canvas, 0, 0, width, height);

	einfo = (Evas_Engine_Info_Buffer *)__evas_engine_info_get_p(canvas);
	if (unlikely(einfo == NULL)) {
		PRINTERR("could not get evas engine info!");
		__evas_free_p(canvas);
		return NULL;
	}

	// ARGB32 is sizeof(int), that is 4 bytes, per pixel
	pixels = real_malloc(width * height * sizeof(int));
	if (unlikely(pixels == NULL)) {
		PRINTERR("could not allocate canvas pixels!");
		__evas_free_p(canvas);
		return NULL;
	}

	einfo->info.depth_type = EVAS_ENGINE_BUFFER_DEPTH_ARGB32;
	einfo->info.dest_buffer = pixels;
	einfo->info.dest_buffer_row_bytes = width * sizeof(int);
	einfo->info.use_color_key = 0;
	einfo->info.alpha_threshold = 0;
	einfo->info.func.new_update_region = NULL;
	einfo->info.func.free_update_region = NULL;

	ret = __evas_engine_info_set_p(canvas,(Evas_Engine_Info*)einfo);
	if (unlikely(ret == EINA_FALSE)) {
		PRINTERR("could not set evas engine info!");
		__evas_free_p(canvas);
		return NULL;
	}

	return canvas;
}

static void destroy_canvas(Evas* canvas)
{
	static Evas_Engine_Info *(*__evas_engine_info_get_p)(const Evas *e);
	static void (*__evas_free_p)(Evas *e);
	Evas_Engine_Info_Buffer *einfo;

	rtld_default_set_once(__evas_engine_info_get_p,
			      "evas_engine_info_get");
	rtld_default_set_once(__evas_free_p, "evas_free");

	einfo = (Evas_Engine_Info_Buffer *)__evas_engine_info_get_p(canvas);
	if (unlikely(einfo == NULL)) {
		PRINTERR("could not get evas engine info!");
		__evas_free_p(canvas);
		return;
	}

	free(einfo->info.dest_buffer);
	__evas_free_p(canvas);
}

static int __save_to_png(const char *path, unsigned char *buf,
			 int width, int height)
{
	int ret = -1, err;
	Evas *ev = NULL;
	Evas_Object *img;

	static void (*__evas_object_resize_p)(Evas_Object *obj,
					      Evas_Coord w,
					      Evas_Coord h);
	static void (*__evas_object_image_data_set_p)(Evas_Object *obj,
						      void *data);
	static void (*__evas_object_image_size_set_p)(Evas_Object *obj,
						      int w, int h);
	static void (*__evas_object_image_fill_set_p)(Evas_Object *obj,
						      Evas_Coord x,
						      Evas_Coord y,
						      Evas_Coord w,
						      Evas_Coord h);
	static void (*__evas_object_image_data_update_add_p)(Evas_Object *obj,
							     int x, int y,
							     int w, int h);
	static Eina_Bool (*__evas_object_image_save_p)(Evas_Object *obj,
						       const char *file,
						       const char *key,
						       const char *flags);
	static Evas_Object *(*__evas_object_image_add_p)(Evas *e);

	rtld_default_set_once(__evas_object_resize_p,
			      "evas_object_resize");
	rtld_default_set_once(__evas_object_image_data_set_p,
			      "evas_object_image_data_set");
	rtld_default_set_once(__evas_object_image_size_set_p,
			      "evas_object_image_size_set");
	rtld_default_set_once(__evas_object_image_fill_set_p,
			      "evas_object_image_fill_set");
	rtld_default_set_once(__evas_object_image_data_update_add_p,
			      "evas_object_image_data_update_add");
	rtld_default_set_once(__evas_object_image_save_p,
			      "evas_object_image_save");
	rtld_default_set_once(__evas_object_image_add_p,
			      "evas_object_image_add");

	ev = create_canvas(width, height);
	if (unlikely(ev == NULL)) {
		PRINTERR("failed to create canvas");
		return -1;
	}

	// make image buffer
	img = __evas_object_image_add_p(ev);
	if (unlikely(img == NULL)) {
		PRINTERR("failed to add image to canvas");
		goto out_canvas;
	}

	//image buffer set
	__evas_object_image_data_set_p(img, NULL);
	__evas_object_image_size_set_p(img, width, height);
	__evas_object_image_data_set_p(img, buf);

	// resize image
	if (height > MAX_HEIGHT) {
		width = width * MAX_HEIGHT / height;
		height = MAX_HEIGHT;
		__evas_object_resize_p(img, width, height);
		__evas_object_image_fill_set_p(img, 0, 0, width, height);
	}
	__evas_object_image_data_update_add_p(img, 0, 0, width, height);

	//save image to png file
	err = __evas_object_image_save_p(img, path, NULL, "compress=5");
	if (err != 0)
		ret = 0;

out_canvas:
	destroy_canvas(ev);

	return ret;
}

static struct efl_data *__edata = NULL;

void __wayland_deinit(void)
{
	struct screenshooter_output *output, *next;

	if (!__edata)
		return;

	wl_list_for_each_safe(output, next, &__edata->output_list, link)
		free(output);

	wl_registry_destroy(__edata->wl_registry);
	wl_display_disconnect(__edata->wl_display);
	free(__edata);
}

struct efl_data *__wayland_init(void)
{
	struct wl_display *display = NULL;
	struct wl_registry *registry;
	struct efl_data *data;
	const char *wayland_socket = NULL;

	if (__edata)
		return __edata;

	wayland_socket = getenv("WAYLAND_SOCKET");
	if (!wayland_socket)
		wayland_socket = getenv("WAYLAND_DISPLAY");

	if (!wayland_socket) {
		PRINTERR("must be launched by wayland");
		return NULL;
	}

	data = malloc(sizeof(*data));
	if (!data)
		return NULL;

	data->screenshooter = NULL;
	wl_list_init(&data->output_list);
	data->min_x = 0;
	data->min_y = 0;
	data->max_x = 0;
	data->max_y = 0;
	data->buffer_copy_done = 0;

	display = wl_display_connect(wayland_socket);
	if (display == NULL) {
		fprintf(stderr, "failed to create display: %m\n");
		goto fail;
	}
	data->wl_display = display;

	/* wl_list_init(&output_list); */
	registry = wl_display_get_registry(display);
	wl_registry_add_listener(registry, &registry_listener, data);
	wl_display_dispatch(display);
	wl_display_roundtrip(display);

	data->wl_registry = registry;

	__edata = data;

	return data;
fail:
	free(data);
	return NULL;
}
static int __capture_screnshot_wayland(const char *path)
{
	struct screenshooter_output *output;
	struct efl_data *data;
	tbm_surface_h t_surface;
	struct wl_buffer *buffer;
	struct wayland_tbm_client *tbm_client;
	tbm_surface_info_s tsuri;
	int plane_idx = 0;
	int ret = -1;
	int err;

	data =  __wayland_init();
	if (!data) {
		PRINTERR("failed to init wayland protocol\n");
		return -1;
	}

	if (!data->screenshooter) {
		PRINTERR("display doesn't support screenshooter\n");
		return -1;
	}

	__set_buffer_size(data);

	tbm_client = wayland_tbm_client_init(data->wl_display);
	if (!tbm_client) {
		PRINTERR("failed to init tbm client\n");
		return -1;
	}

	t_surface = tbm_surface_create(data->width, data->height, TBM_FORMAT_XRGB8888);
	if (!t_surface) {
		PRINTERR("failed to create tbm_surface\n");
		goto fail_tbm_client;
	}

	buffer = wayland_tbm_client_create_buffer(tbm_client, t_surface);
	if (!buffer) {
		PRINTERR("failed to create wl_buffer for screenshot\n");
		goto fail_tbm_surface;
	}

	wl_list_for_each(output, &data->output_list, link) {
		screenshooter_shoot(data->screenshooter,
					   output->output,
					   buffer);
		data->buffer_copy_done = 0;
		while (!data->buffer_copy_done) {
			wl_display_roundtrip(data->wl_display);
		}
	}

	err = tbm_surface_map(t_surface,
			      TBM_SURF_OPTION_READ | TBM_SURF_OPTION_WRITE,
			      &tsuri);
	if (err != TBM_SURFACE_ERROR_NONE) {
		PRINTERR("failed to map tbm_surface\n");
		goto fail_map_surface;
	}

	ret = __save_to_png(path, tsuri.planes[plane_idx].ptr,
			    data->width, data->height);

	tbm_surface_unmap(t_surface);

fail_map_surface:
	wayland_tbm_client_destroy_buffer(tbm_client, buffer);

fail_tbm_surface:
	tbm_surface_destroy(t_surface);

fail_tbm_client:
	wayland_tbm_client_deinit(tbm_client);

	return ret;
}

int captureScreen()
{
	static pthread_mutex_t captureScreenLock = PTHREAD_MUTEX_INITIALIZER;
	char dstpath[MAX_PATH_LENGTH];
	int ret = -1;

	pthread_mutex_lock(&captureScreenLock);

	inc_current_event_index();

	snprintf(dstpath, sizeof(dstpath),
		 SCREENSHOT_DIRECTORY "/%d_%d.png", getpid(),
		 getCurrentEventIndex());

	ret = __capture_screnshot_wayland(dstpath);
	if (!ret) {
		if (chmod(dstpath, 0777) == -1)
			PRINTWRN("cannot chmod -R777 <%s>", dstpath);

		/* welcome to the hell */
		log_t log;
		PREPARE_LOCAL_BUF_THOUGH((char *)&log);

		/* skip header */
		LOCAL_BUF += offsetof(log_t, data);
		/* pack screenshot name */
		BUF_PTR = pack_string(LOCAL_BUF, dstpath); /* file name */
		LOCAL_BUF = BUF_PTR;

		/* pack probe */
		PACK_COMMON_BEGIN(MSG_PROBE_SCREENSHOT, API_ID_captureScreen, "", 0);
		PACK_COMMON_END_THOUGH('d', 0, 0, 0, 0xffffffff);
		PACK_SCREENSHOT(dstpath, current_angle_get());
		SET_MSG_LEN();
		log.length = GET_MSG_LEN() + MSG_HDR_LEN + strlen(dstpath) + 1;

		/* send all message */
		printLog(&log, APP_MSG_IMAGE);
		ret = 0;
	}

	pthread_mutex_unlock(&captureScreenLock);
	return ret;
}

// =======================================================================
// screen shot manipulation functions
// =======================================================================

static Eina_Bool _captureTimer(void __unused * data)
{
	SCREENSHOT_TIMEOUT();

	return ECORE_CALLBACK_CANCEL;
}

int activateCaptureTimer()
{
	static Ecore_Timer *(*__ecore_timer_add_p)(double in,
						   Ecore_Task_Cb func,
						   const void *data);

	rtld_default_set_once(__ecore_timer_add_p, "ecore_timer_add");
	__ecore_timer_add_p(CAPTURE_TIMEOUT, _captureTimer, NULL);
	return 0;
}

void _cb_render_post(void __unused * data, Evas __unused * e,
		     void __unused * eventinfo)
{
	SCREENSHOT_DONE();
}
