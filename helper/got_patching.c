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
#define X(feature) features[feature##_ind] = &feature;

/* Can be initialized only dynamically, because struct addresses are resolved
 * only at a runtime */
static void _init_features_ptrs(void)
{
	FEATURES_LIST
}

#undef X

#undef FEATURES_LIST





static bool inited = false;

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

	ret = dladdr(*(void **)addr, &sym_info);
	if (ret == 0)
		return NULL;

	if (sym_info.dli_saddr == NULL)
		return NULL;

	return sym_info.dli_saddr;
}

static inline bool _is_for_all_feature(struct feature_desc_t *feature)
{
	unsigned int i;

	/* Find if we should check all the binaries. It is so, if there is any 
	 * always or inverted probe for this feature */
	for (i = 0; i < feature->cnt; i++)
		if ((feature->probes[i].flags & GT_ALWAYS_PROBE) ||
		    (feature->probes[i].flags & (GT_TARGET_PROBE | GT_INVERTED_PROBE)))
			return true;

	return false;
}

/* TODO Remove hardcoded ignored bins, make it flexible */
static inline bool _is_ignored(const char *path)
{
	const char probe_lib[] = "/usr/lib/da_probe_tizen.so";
	const char linux_gate[] = "linux-gate.so.1";
	/* TODO Find why these libs leads to fail */
	const char systemd[] = "/usr/lib/libsystemd.so.0";
	const char udev[] = "/usr/lib/libudev.so.1";

	if (path[0] == '\0' ||
	    !strcmp(probe_lib, path) ||
	    !strcmp(linux_gate, path) ||
	    !strcmp(systemd, path) ||
	    !strcmp(udev, path))
		return true;

	return false;
}

static int _patch_bin(struct dl_phdr_info *info, size_t __unused size, void *data)
{
	struct link_map *map;
	const char *path;
	int ret = 0;
	bool target_bin;
	bool is_for_all;
	unsigned int i;
	unsigned long addr;
	void *orig, *handle;
	struct feature_desc_t *feature = (struct feature_desc_t *)data;

	/* Get binary path */
	path = info->dlpi_name;
	if (path == NULL) {
		PRINTERR("No path for phdr!\n");
		return -EINVAL;
	}

	/* Check if this binary a target one */
	target_bin = check_binary(path);
	is_for_all = _is_for_all_feature(feature);
	if ((!target_bin && !is_for_all) || _is_ignored(path))
		return 0;

	/* TODO Add queue and patching when possible! */
	/* Check if memory is consistent, dlopen() is possible */
	if (_r_debug.r_state != RT_CONSISTENT)
		return 0;

	/* This doesn't perform loading binary, but returns its link_map */
	handle = dlopen(path, RTLD_LAZY | RTLD_NOLOAD);
	if (handle == NULL) {
		PRINTERR("Cannot get handle for %s\n", path);
		return -EINVAL;
	}

	ret = dlinfo(handle, RTLD_DI_LINKMAP, &map);
	if (ret != 0) {
		PRINTERR("dlinfo() failed. Return value %d\n", ret);
		PRINTERR("Error itself %s\n", dlerror());
		goto fail_get_map;
	}

	for (i = 0; i < feature->cnt; i++) {
		/* If feature is not always and flags doesn't satisfy binary's status -
		 * continue */
		if (~(feature->probes[i].flags & GT_ALWAYS_PROBE) &&
		    (~(feature->probes[i].flags & GT_INVERTED_PROBE) && !target_bin) &&
		    (~(feature->probes[i].flags & GT_TARGET_PROBE) && target_bin))
			continue;

		addr = lmap_reladdr_by_name(map, feature->probes[i].orig_name);
		if (addr == 0)
			continue;

		orig = _get_orig_addr(addr);
		if (orig != NULL) {
//			feature->probes[i].orig_ptr = orig;
			*(ElfW(Addr) *)addr = feature->probes[i].handler_ptr;
		}
	}

fail_get_map:
	/* dlopen() leads to a usage increment, so, decrement it with dlclose() */
	dlclose(handle);

	return ret;
}

static void _patch_target_bins(struct feature_desc_t *feature)
{
	dl_iterate_phdr(_patch_bin, feature);
}

static void _process_features(void)
{
	unsigned int i;

	for (i = 0; i < features_cnt; i++) {
		if (!isOptionEnabled(features[i]->feature))
			continue;
		_patch_target_bins(features[i]);
	}
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

typedef DL_FIXUP_VALUE_TYPE ARCH_FIXUP_ATTRIBUTE (*dl_fixup_t) (
# ifdef ELF_MACHINE_RUNTIME_FIXUP_ARGS
	   ELF_MACHINE_RUNTIME_FIXUP_ARGS,
# endif
	   struct link_map *l, ElfW(Word) reloc_arg);

static dl_fixup_t dl_fixup_p = 0;

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
	unsigned int i, j;
	unsigned long got_addr;
	ElfW(Addr) exec_addr;

	/* TODO App binaries in link_map structures have no name in l_name field.
	 * Check whether it is equal to '\0' */
	if (!check_binary(l->l_name) && l->l_name[0] != '\0')
		/* Just call original and go away */
		return dl_fixup_p(l, reloc_arg);

	func_name = lmap_get_name(l, reloc_arg);
	if (func_name == NULL)
		return dl_fixup_p(l, reloc_arg);

	/* TODO Fix it */
//	got_addr = lmap_reladdr_by_offset(l, reloc_arg);
	got_addr = lmap_reladdr_by_name(l, func_name);

	exec_addr = dl_fixup_p(l, reloc_arg);

	/* Iterate features to find target probe */
	for (i = 0; i < features_cnt; i++) {
		if (!isOptionEnabled(features[i]->feature))
			continue;

		if ((!check_binary(l->l_name) &&
		     !_is_for_all_feature(features[i])) ||
		    _is_ignored(l->l_name))
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
				features[i]->probes[j].orig_ptr = exec_addr;
				*(ElfW(Addr) *)got_addr = features[i]->probes[j].handler_ptr;
				exec_addr = (unsigned long)features[i]->probes[j].handler_ptr;
			}
		}
	}

	/* TODO generate header at compile time with linker _dl_fixup() address to
	 * use here. Let it be called dl_fixup_p */

	/* TODO Think of several handlers at the same time (malloc()/free() can be
	 * gathered always and profiled for a special binary, for example): which
	 * of them should be executed? */
	return exec_addr;
}




static int _init_linker_addr(void)
{
	void *handle;
	struct link_map *lmap;
	int ret = 0;

	handle = dlopen(linker_path, RTLD_LAZY | RTLD_NOLOAD);
	if (handle == NULL) {
		PRINTERR("Cannot get linker handler. Expected linker path %s\n",
				 linker_path);
		return -EINVAL;
	}

	ret = dlinfo(handle, RTLD_DI_LINKMAP, &lmap);
	if (ret != 0) {
		PRINTERR("dlinfo() for linker failed. Return value %d\n", ret);
		PRINTERR("Error itself %s\n", dlerror());
		goto init_linker_addr_end;
	}

	dl_fixup_p = (dl_fixup_t)(dl_fixup_off + lmap->l_addr);

init_linker_addr_end:
	dlclose(handle);

	return ret;
}

void process_got_patching(char *data)
{
	if (!inited) {
		_init_linker_addr();
		_init_features_ptrs();
		inited = true;
	}
	_process_target_bins(data);
	_process_features();
}


void restore_got_patching(void)
{
	// TODO patch all binaries back

	cleanup_binaries();
}
