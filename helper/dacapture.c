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
#include "wayland-api.h"
#include "real_functions.h"
#include "daprobe.h"
#include "dahelper.h"

#include "binproto.h"

#define MAX_HEIGHT		720
#define CAPTURE_TIMEOUT		2.0
#define MAX_PATH_LENGTH 256

struct screenshot_data {
	struct wl_shm *shm;
	struct screenshooter *screenshooter;
	struct wl_list output_list;
	int min_x, min_y, max_x, max_y;
	int buffer_copy_done;
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
	struct screenshot_data *sdata = data;
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
	struct screenshot_data *sdata = data;

	if (strcmp(interface, "wl_output") == 0) {
		struct screenshooter_output *output = malloc(sizeof(*output));

		if (output) {
			PRINTMSG("allocate %p", output);
			output->output = wl_registry_bind(registry, name,
							  &wl_output_interface,
							  1);
			wl_list_insert(&sdata->output_list, &output->link);
			wl_output_add_listener(output->output,
					       &output_listener,
					       output);
		}
	} else if (strcmp(interface, "wl_shm") == 0) {
		sdata->shm = wl_registry_bind(registry, name, 
					      &wl_shm_interface, 1);
	} else if (strcmp(interface, "screenshooter") == 0) {
		sdata->screenshooter = wl_registry_bind(registry, name,
						 &screenshooter_interface,
						 1);
	}
}

static const struct wl_registry_listener registry_listener = {
	handle_global,
	NULL
};

static struct wl_buffer *
create_shm_buffer(struct wl_shm *shm, int width, int height, void **data_out)
{
	char filename[] = "/tmp/wayland-shm-XXXXXX";
	struct wl_shm_pool *pool;
	struct wl_buffer *buffer;
	int fd, size, stride;
	void *data;

	stride = width * 4;
	size = stride * height;

	fd = mkstemp(filename);
	if (fd < 0)
		return NULL;

	if (ftruncate(fd, size) < 0) {
		close(fd);
		return NULL;
	}

	data = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	unlink(filename);

	if (data == MAP_FAILED) {
		close(fd);
		return NULL;
	}

	pool = wl_shm_create_pool(shm, fd, size);
	close(fd);
	buffer = wl_shm_pool_create_buffer(pool, 0, width, height, stride,
					   WL_SHM_FORMAT_XRGB8888);
	wl_shm_pool_destroy(pool);
	*data_out = data;

	return buffer;
}

static void *__screenshot_to_buf(struct screenshot_data *sdata, int width,
				 int height)
{
	int output_stride, buffer_stride, i;
	void *data, *d, *s;
	struct screenshooter_output *output, *next;

	buffer_stride = width * 4;
	data = malloc(buffer_stride * height);
	if (!data)
		return NULL;

	wl_list_for_each_safe(output, next, &sdata->output_list, link) {
		output_stride = output->width * 4;
		s = output->data;
		d = data + (output->offset_y - sdata->min_y) * buffer_stride +
			   (output->offset_x - sdata->min_x) * 4;

		for (i = 0; i < output->height; i++) {
			memcpy(d, s, output_stride);
			d += buffer_stride;
			s += output_stride;
		}

		wl_list_remove(&output->link);
		free(output);
	}

	return data;
}

static int
set_buffer_size(struct screenshot_data *sdata, int *width, int *height)
{
	struct screenshooter_output *output;

	sdata->min_x = sdata->min_y = INT_MAX;
	sdata->max_x = sdata->max_y = INT_MIN;
	int position = 0;

	wl_list_for_each_reverse(output, &sdata->output_list, link) {
		output->offset_x = position;
		position += output->width;
	}

	wl_list_for_each(output, &sdata->output_list, link) {
		sdata->min_x = MIN(sdata->min_x, output->offset_x);
		sdata->min_y = MIN(sdata->min_y, output->offset_y);
		sdata->max_x = MAX(sdata->max_x,
				   output->offset_x + output->width);
		sdata->max_y = MAX(sdata->max_y,
				   output->offset_y + output->height);
	}

	if (sdata->max_x <= sdata->min_x || sdata->max_y <= sdata->min_y)
		return -1;

	*width = sdata->max_x - sdata->min_x;
	*height = sdata->max_y - sdata->min_y;

	return 0;
}

static void *__capture_screnshot_wayland(int *width, int *height)
{
	struct wl_display *display = NULL;
	struct wl_registry *registry;
	struct screenshooter_output *output;
	void *buf = NULL;
	struct screenshot_data *sdata;
	const char *wayland_socket = NULL;

	wayland_socket = getenv("WAYLAND_SOCKET");
	if (!wayland_socket)
		wayland_socket = getenv("WAYLAND_DISPLAY");

	if (!wayland_socket) {
		PRINTERR("must be launched by wayland");
		return NULL;
	}

	sdata = malloc(sizeof(*sdata));
	if (!sdata)
		return NULL;

	sdata->shm = NULL;
	sdata->screenshooter = NULL;
	wl_list_init(&sdata->output_list);
	sdata->min_x = 0;
	sdata->min_y = 0;
	sdata->max_x = 0;
	sdata->max_y = 0;
	sdata->buffer_copy_done = 0;

	display = wl_display_connect(wayland_socket);
	if (display == NULL)
		goto out;

	/* wl_list_init(&output_list); */
	registry = wl_display_get_registry(display);
	wl_registry_add_listener(registry, &registry_listener, sdata);
	wl_display_dispatch(display);
	wl_display_roundtrip(display);

	if (sdata->screenshooter == NULL) {
		PRINTERR("display doesn't support screenshooter");
		return NULL;
	}

	screenshooter_add_listener(sdata->screenshooter, &screenshooter_listener,
				   sdata);

	if (set_buffer_size(sdata, width, height))
		return NULL;


	wl_list_for_each(output, &sdata->output_list, link) {
		output->buffer = create_shm_buffer(sdata->shm, output->width,
						   output->height, &output->data);
		screenshooter_shoot(sdata->screenshooter,
					   output->output,
					   output->buffer);
		sdata->buffer_copy_done = 0;
		while (!sdata->buffer_copy_done) {
			wl_display_roundtrip(display);
		}
	}

	buf = __screenshot_to_buf(sdata, *width, *height);

out:
	if (display)
		wl_display_disconnect(display);
	free(sdata);
	return buf;
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

int captureScreen()
{
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
	static pthread_mutex_t captureScreenLock = PTHREAD_MUTEX_INITIALIZER;
	char dstpath[MAX_PATH_LENGTH];
	char *scrimage;
	int width, height, err;
	Evas *ev = NULL;
	Evas_Object *img;
	probeInfo_t probeInfo;
	int ret = -1;

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

	pthread_mutex_lock(&captureScreenLock);
	setProbePoint(&probeInfo);

	scrimage = __capture_screnshot_wayland(&width, &height);
	if (unlikely(scrimage == NULL)) {
		ret = -1;
		goto out;
	}

	ev = create_canvas(width, height);
	if (unlikely(ev == NULL)) {
		ret = -1;
		goto out_image;
	}

	snprintf(dstpath, sizeof(dstpath),
		 SCREENSHOT_DIRECTORY "/%d_%d.png", getpid(),
		 probeInfo.eventIndex);

	// make image buffer
	img = __evas_object_image_add_p(ev);
	if (unlikely(img == NULL)) {
		ret = -1;
		goto out_canvas;
	}

	//image buffer set
	__evas_object_image_data_set_p(img, NULL);
	__evas_object_image_size_set_p(img, width, height);
	__evas_object_image_data_set_p(img, scrimage);

	// resize image
	if (height > MAX_HEIGHT) {
		width = width * MAX_HEIGHT / height;
		height = MAX_HEIGHT;
		__evas_object_resize_p(img, width, height);
		__evas_object_image_fill_set_p(img, 0, 0, width, height);
	}
	__evas_object_image_data_update_add_p(img, 0, 0, width, height);

	//save image to png file
	err = __evas_object_image_save_p(img, dstpath, NULL, "compress=5");
	if (err != 0) {
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
		PACK_COMMON_END_THOUGH('d', 0, 0, 0);
		PACK_SCREENSHOT(dstpath, current_angle_get());
		SET_MSG_LEN();
		log.length = GET_MSG_LEN() + MSG_HDR_LEN + strlen(dstpath) + 1;

		/* send all message */
		printLog(&log, APP_MSG_IMAGE);
		ret = 0;
	}

	// release resources
out_canvas:
	destroy_canvas(ev);

out_image:
	free(scrimage);

out:
	pthread_mutex_unlock(&captureScreenLock);
	return ret;
}

int initialize_screencapture()
{
	// remove all previous screenshot in dir
//	remove_indir(SCREENSHOT_DIRECTORY);

	// make screenshot directory
//	mkdir(SCREENSHOT_DIRECTORY, 0777);

	return 0;
}

int finalize_screencapture()
{
	return 0;
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
