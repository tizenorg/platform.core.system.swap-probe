/*
 *  DA probe
 *
 * Copyright (c) 2000 - 2014 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Contact:
 *
 * Vitaliy Cherepanov <v.cherepanov@samsung.com>
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
 * - Samsung RnD Institute Russia
 *
 */

#include "da_gles20.h"
#include "daprobe.h"
#include "binproto.h"
#include "common_probe_init.h"

static enum DaOptions _sopt = OPT_GLES;

/* Evas open gl API functions */

void evas_gl_free(Evas_GL *evas_gl)
{
	typedef void (*methodType)(Evas_GL *evas_gl);
	BEFORE_EVAS_GL(evas_gl_free);
	evas_gl_freep(evas_gl);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "p",
	      evas_gl);
}

void evas_gl_config_free(Evas_GL_Config *cfg)
{
	typedef void (*methodType)(Evas_GL_Config *cfg);
	BEFORE_EVAS_GL(evas_gl_config_free);
	evas_gl_config_freep(cfg);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "p",
	      cfg);
}

void evas_gl_surface_destroy(Evas_GL *evas_gl, Evas_GL_Surface *surf)
{
	typedef void (*methodType)(Evas_GL *evas_gl, Evas_GL_Surface *surf);
	BEFORE_EVAS_GL(evas_gl_surface_destroy);
	evas_gl_surface_destroyp(evas_gl, surf);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "pp",
	      evas_gl, surf);
}

void evas_gl_context_destroy(Evas_GL *evas_gl, Evas_GL_Context *ctx)
{
	typedef void (*methodType)(Evas_GL *evas_gl, Evas_GL_Context *ctx);
	BEFORE_EVAS_GL(evas_gl_context_destroy);
	evas_gl_context_destroyp(evas_gl, ctx);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "pp",
	      evas_gl, ctx);
}

Evas_GL *evas_gl_new(Evas *e)
{
	typedef Evas_GL *(*methodType)(Evas *e);
	BEFORE_EVAS_GL(evas_gl_new);
	Evas_GL *res = evas_gl_newp(e);
	GL_GET_ERROR();
	AFTER('p', res, APITYPE_CONTEXT, "", "p",
	      e);
	return res;
}

Evas_GL_Config *evas_gl_config_new(void)
{
	typedef Evas_GL_Config *(*methodType)(void);
	BEFORE_EVAS_GL(evas_gl_config_new);
	Evas_GL_Config *res = evas_gl_config_newp();
	GL_GET_ERROR();
	AFTER_NO_PARAM('p', res, APITYPE_CONTEXT, "");
	return res;
}

Evas_GL_Surface *evas_gl_surface_create(Evas_GL *evas_gl, Evas_GL_Config *cfg,
					int w, int h)
{
	typedef Evas_GL_Surface *(*methodType)(Evas_GL *evas_gl,
					       Evas_GL_Config *cfg,
					       int w, int h);
	BEFORE_EVAS_GL(evas_gl_surface_create);
	Evas_GL_Surface *res = evas_gl_surface_createp(evas_gl, cfg, w, h);
	GL_GET_ERROR();
	AFTER('p', res, APITYPE_CONTEXT, "", "ppdd",
	      evas_gl, cfg, w, h);
	return res;
}

Evas_GL_Context *evas_gl_context_create(Evas_GL *evas_gl,
					Evas_GL_Context *share_ctx)
{
	typedef Evas_GL_Context *(*methodType)(Evas_GL *evas_gl,
					       Evas_GL_Context *share_ctx);
	BEFORE_EVAS_GL(evas_gl_context_create);
	Evas_GL_Context *res = evas_gl_context_createp(evas_gl, share_ctx);
	GL_GET_ERROR();
	AFTER('p', res, APITYPE_CONTEXT, "", "pp",
	      evas_gl, share_ctx);
	return res;
}

Eina_Bool evas_gl_make_current(Evas_GL *evas_gl, Evas_GL_Surface *surf,
			       Evas_GL_Context *ctx)
{
	typedef Eina_Bool (*methodType)(Evas_GL *evas_gl, Evas_GL_Surface *surf,
					Evas_GL_Context *ctx);
	BEFORE_EVAS_GL(evas_gl_make_current);
	Eina_Bool res = evas_gl_make_currentp(evas_gl, surf, ctx);
	GL_GET_ERROR();
	AFTER('b', res, APITYPE_CONTEXT, "", "ppp",
	      evas_gl, surf, ctx);
	return res;
}

const char *evas_gl_string_query(Evas_GL *evas_gl, int name)
{
	typedef const char *(*methodType)(Evas_GL *evas_gl, int name);
	BEFORE_EVAS_GL(evas_gl_string_query);
	const char *res = evas_gl_string_queryp(evas_gl, name);
	GL_GET_ERROR();
	AFTER('s', res, APITYPE_CONTEXT, "", "pd",
	      evas_gl, name);
	return res;
}

Evas_GL_Func evas_gl_proc_address_get(Evas_GL *evas_gl, const char *name)
{
	typedef Evas_GL_Func(*methodType)(Evas_GL *evas_gl, const char *name);
	BEFORE_EVAS_GL(evas_gl_proc_address_get);
	Evas_GL_Func res = evas_gl_proc_address_getp(evas_gl, name);
	GL_GET_ERROR();
	AFTER('p', res, APITYPE_CONTEXT, "", "ps",
	      evas_gl, name);
	return res;
}

Eina_Bool evas_gl_native_surface_get(Evas_GL *evas_gl, Evas_GL_Surface *surf,
				     Evas_Native_Surface *ns)
{
	typedef Eina_Bool(*methodType)(Evas_GL *evas_gl, Evas_GL_Surface *surf,
				   Evas_Native_Surface *ns);
	BEFORE_EVAS_GL(evas_gl_native_surface_get);
	Eina_Bool res = evas_gl_native_surface_getp(evas_gl, surf, ns);
	GL_GET_ERROR();
	AFTER('b', res, APITYPE_CONTEXT, "", "ppp",
	      evas_gl, surf, ns);
	return res;
}

Evas_GL_API *evas_gl_api_get(Evas_GL *evas_gl)
{
	typedef Evas_GL_API *(*methodType)(Evas_GL *evas_gl);
	BEFORE_EVAS_GL(evas_gl_api_get);
	Evas_GL_API *res = evas_gl_api_getp(evas_gl);

	/* save original api functions and rewrite it by probes */
	save_orig_gl_api_list(res);
	change_gl_api_list(res);

	GL_GET_ERROR();
	AFTER('p', res, APITYPE_CONTEXT, "", "p",
	      evas_gl);
	return res;
}