#include <link.h>               // for struct link_map
#include <dlfcn.h>              // for dlopen()
#include <stdlib.h>
#include <string.h>

#include "probeinfo.h"
#include "probe_helper.h"
#include "probe_capi.h"
#include "probe_event.h"
#include "probe_file.h"
#include "probe_graphics.h"
#include "probe_memory.h"
#include "probe_socket.h"
#include "probe_thread.h"
#include "probe_ui.h"

#include "dahelper.h"
#include "got_patching.h"
#include "get_got.h"

/*****************************************************************************
 *                              FEATURES                                     *
 *****************************************************************************/


/* Features list, defined in probe_* headers */
#define FEATURES_LIST \
	X(memory_feature) \
	X(memory_feature_always) \
	X(helper_feature) \
	X(capi_feature) \
	X(event_feature) \
	X(file_feature) \
	X(file_feature_always) \
	X(graphics_feature) \
	X(graphics_feature_always) \
	X(network_feature) \
	X(network_feature_always) \
	X(thread_feature) \
	X(thread_feature_always) \
	X(ui_feature)


/* Declare array of features length */
#define X(feature) +1

static unsigned int features_cnt = (FEATURES_LIST);
static struct feature_desc_t *features[(FEATURES_LIST)] = { NULL };

#undef X


/* Declare array's indexes */
#define X(feature) feature##_ind,

enum {
	FEATURES_LIST
};

#undef X


/* Declare array of features */
#define X(feature) features[feature##_ind] = feature;

/* Can be initialized only dynamically, because struct addresses are resolved
 * only at a runtime */
static void _init_features_ptrs(void)
{
	if (features == NULL) {
		FEATURES_LIST
	}
}

#undef X

#undef FEATURES_LIST








static void _process_target_bins(char *bins_data)
{
	uint32_t cnt;
	uint32_t len;
	char *ptr = bins_data;
	char *path;
	unsigned int i;
	int ret;

	cnt = *(uint32_t *)ptr;
	ptr += sizeof(cnt);

	for (i = 0; i < cnt; i++) {
		len = *(uint32_t *)ptr;
		ptr += sizeof(len);

		path = malloc(len);
		if (path == NULL) {
			// TODO Error! error! error!
			return;
		}

		memcpy(path, ptr, len);
		ptr += len;

		/* TODO Think about removing binaries */
		ret = add_binary(path);
		if (ret != 0)
			free(path);
	}
}

static void *_get_orig_addr(unsigned long addr)
{
	Dl_info sym_info;
	int ret;

	ret = dladdr((void *)addr, &sym_info);
	if (ret == 0)
		return NULL;

	if (sym_info.dli_saddr == NULL)
		return NULL;

	return *(void **)addr;
}

static void _process_feature(struct link_map *map,
			     struct feature_desc_t *feature)
{
	unsigned long addr;
	void *orig;
	unsigned int i;

	for (i = 0; i < feature->cnt; i++) {
		addr = lmap_reladdr_by_name(map, feature->probes[i].orig_name);
		if (addr == 0)
			continue;

		orig = _get_orig_addr(addr);
		if (orig != NULL) {
//			feature->probes[i].orig_ptr = orig;
			*(void **)addr = feature->probes[i].handler_ptr;
		}
	}
}

static int _patch_bin(char *path, __attribute__((unused))void * data)
{
	struct link_map *map;
	int ret = 0;
	unsigned int i;

	/* This doesn't perform loading binary, but returns its link_map */
	map = (struct link_map *)dlopen(path, RTLD_NOLOAD);
	if (map == NULL)
		return 0;

	/* Iterate features */
	for (i = 0; i < features_cnt; i++) {
		if (!isOptionEnabled(features[i]->feature))
			continue;
		_process_feature(map, features[i]);
	}

	/* dlopen() leads to a usage increment, so, decrement it with dlclose() */
	dlclose(map);

	return ret;
}

static void _patch_target_bins(void)
{
	bin_cb_t cb;

	/* TODO Think of using dl_iterate_phdr(). Maybe it is faster to iterate
	 * all mapped binaries, but not all target, since there is some probes
	 * that should be executed for all bins (malloc, free, etc. */

	cb = &_patch_bin;

	do_for_each_binary(cb, NULL);

	/* TODO Handle ret */
}


#if (defined __x86_64__) || (defined __arm__) || (defined __aarch64__)

# define DL_FIXUP_VALUE_TYPE ElfW(Addr)
# define ARCH_FIXUP_ATTRIBUTE

#elif (defined __i386__)

# define DL_FIXUP_VALUE_TYPE ElfW(Addr)
# define ARCH_FIXUP_ATTRIBUTE __attribute__ ((regparm (3), stdcall, unused))

#else
# error "This arch is not supported"
#endif


#include "linker_info.h"

/* This function is a wrapper for _dl_fixup() at Linux linker, so, we need it
 * to have exactly the same prototype. Different fixup definitions are used for
 * some architectures. Using definitions from glibc headers directly looks like
 * a bad decision cause these definitions are declared in different headers
 * each of which contains an enormous amount of another data absolutely useless
 * here.*/

DL_FIXUP_VALUE_TYPE __attribute ((noinline)) ARCH_FIXUP_ATTRIBUTE
__dl_fixup_wrapper (
# ifdef ELF_MACHINE_RUNTIME_FIXUP_ARGS
	   ELF_MACHINE_RUNTIME_FIXUP_ARGS,
# endif
	   struct link_map *l, ElfW(Word) reloc_arg)
{
	const char *func_name;
//	DL_FIXUP_VALUE_TYPE orig_addr;
	unsigned int i, j;
	unsigned long got_addr;
	unsigned long handler_addr;


	/* TODO App binaries in link_map structures have no name in l_name field.
	 * Check whether it is equal to '\0' */
	if (!check_binary(l->l_name) && l->l_name[0] != '\0')
		/* Just call original and go away */
		return dl_fixup_p(l, reloc_arg);

	func_name = lmap_get_name(l, reloc_arg);
	if (func_name == NULL)
		return dl_fixup_p(l, reloc_arg);

	got_addr = lmap_reladdr_by_offset(l, reloc_arg);

//	orig_addr = dl_fixup_p(l, reloc_arg);
	/* TODO It is going to be used to get original address of a functions.
	 * Temporary all original functions are searched in probes, so this call
	 * is useless */
	dl_fixup_p(l, reloc_arg);

	/* Iterate features to find target probe */
	for (i = 0; i < features_cnt; i++) {
		if (!isOptionEnabled(features[i]->feature))
			continue;
		for (j = 0; j < features[i]->cnt; j++) {
			/* Using strcmp() cause orig_name is predefined */
			if (strcmp(features[i]->probes[j].orig_name, func_name) == 0) {
				/* FIXME Possible race condition, cause GOT entry already
				 * patched by linker and points to an original handler, but not
				 * our handler. Ideas:
				 * - set probe right after _dl_fixup() exec
				 *   and patch all entries by it;
				 * - check where are treads when we starting. */
//				features[i]->probes[j].orig_ptr = (void *)orig_addr;
				*(void **)got_addr = features[i]->probes[j].handler_ptr;
				handler_addr = (unsigned long)features[i]->probes[j].handler_ptr;
			}
		}
	}

	/* TODO generate header at compile time with linker _dl_fixup() address to
	 * use here. Let it be called dl_fixup_p */

	/* TODO Think of several handlers at the same time (malloc()/free() can be
	 * gathered always and profiled for a special binary, for example): which
	 * of them should be executed? */
	return handler_addr;
}




void process_got_patching(char *data)
{
	_init_features_ptrs();
	_process_target_bins(data);
	_patch_target_bins();
}


void restore_got_patching(void)
{
	// TODO patch all binaries back

	cleanup_binaries();
}
