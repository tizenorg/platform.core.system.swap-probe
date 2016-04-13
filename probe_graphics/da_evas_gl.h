#ifndef __DA_EVAS_GL_H__
#define __DA_EVAS_GL_H__

void PROBE_NAME(evas_gl_free)(Evas_GL *evas_gl);
void PROBE_NAME(evas_gl_config_free)(Evas_GL_Config *cfg);
void PROBE_NAME(evas_gl_surface_destroy)(Evas_GL *evas_gl, Evas_GL_Surface *surf);
void PROBE_NAME(evas_gl_context_destroy)(Evas_GL *evas_gl, Evas_GL_Context *ctx);
Evas_GL *PROBE_NAME(evas_gl_new)(Evas *e);
Evas_GL_Config *PROBE_NAME(evas_gl_config_new)(void);
Evas_GL_Surface *PROBE_NAME(evas_gl_surface_create)(Evas_GL *evas_gl, Evas_GL_Config *cfg, int w, int h);
Evas_GL_Context *PROBE_NAME(evas_gl_context_create)(Evas_GL *evas_gl, Evas_GL_Context *share_ctx);
Eina_Bool PROBE_NAME(evas_gl_make_current)(Evas_GL *evas_gl, Evas_GL_Surface *surf, Evas_GL_Context *ctx);
const char *PROBE_NAME(evas_gl_string_query)(Evas_GL *evas_gl, int name);
Evas_GL_Func PROBE_NAME(evas_gl_proc_address_get)(Evas_GL *evas_gl, const char *name);
Eina_Bool PROBE_NAME(evas_gl_native_surface_get)(Evas_GL *evas_gl, Evas_GL_Surface *surf, Evas_Native_Surface *ns);
Evas_GL_API *PROBE_NAME(evas_gl_api_get)(Evas_GL *evas_gl);
Evas_GL_API* PROBE_NAME(elm_glview_gl_api_get)(const Evas_Object *obj);
Evas_GL_API *PROBE_NAME(evas_gl_context_api_get)(Evas_GL *evas_gl, Evas_GL_Context *ctx);


#endif /* __DA_EVAS_GL_H__ */
