#ifndef __DA_EVAS_GL_H__
#define __DA_EVAS_GL_H__

HANDLER_WRAPPERS_DEF(void, evas_gl_free, Evas_GL *, evas_gl);
HANDLER_WRAPPERS_DEF(void, evas_gl_config_free, Evas_GL_Config *, cfg);
HANDLER_WRAPPERS_DEF(void, evas_gl_surface_destroy, Evas_GL *, evas_gl, Evas_GL_Surface *, surf);
HANDLER_WRAPPERS_DEF(void, evas_gl_context_destroy, Evas_GL *, evas_gl, Evas_GL_Context *, ctx);
HANDLER_WRAPPERS_DEF(Evas_GL *, evas_gl_new, Evas *, e);
HANDLER_WRAPPERS_DEF(Evas_GL_Config *, evas_gl_config_new, void);
HANDLER_WRAPPERS_DEF(Evas_GL_Surface *, evas_gl_surface_create, Evas_GL *, evas_gl, Evas_GL_Config *, cfg, int, w, int, h);
HANDLER_WRAPPERS_DEF(Evas_GL_Context *, evas_gl_context_create, Evas_GL *, evas_gl, Evas_GL_Context *, share_ctx);
HANDLER_WRAPPERS_DEF(Eina_Bool, evas_gl_make_current, Evas_GL *, evas_gl, Evas_GL_Surface *, surf, Evas_GL_Context *, ctx);
HANDLER_WRAPPERS_DEF(const char *, evas_gl_string_query, Evas_GL *, evas_gl, int, name);
HANDLER_WRAPPERS_DEF(Evas_GL_Func, evas_gl_proc_address_get, Evas_GL *, evas_gl, const char *, name);
HANDLER_WRAPPERS_DEF(Eina_Bool, evas_gl_native_surface_get, Evas_GL *, evas_gl, Evas_GL_Surface *, surf, Evas_Native_Surface *, ns);
HANDLER_WRAPPERS_DEF(Evas_GL_API *, evas_gl_api_get, Evas_GL *, evas_gl);
HANDLER_WRAPPERS_DEF(Evas_GL_API*, elm_glview_gl_api_get, const Evas_Object *, obj);
HANDLER_WRAPPERS_DEF(Evas_GL_API *, evas_gl_context_api_get, Evas_GL *, evas_gl, Evas_GL_Context *, ctx);


#endif /* __DA_EVAS_GL_H__ */
