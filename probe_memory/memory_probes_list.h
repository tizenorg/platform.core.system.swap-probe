#ifndef __MEMORY_PROBES_LIST_H__
#define __MEMORY_PROBES_LIST_H__

#define PROBES_LIST_MEMORY \
	X(PROBE_NAME(malloc), "malloc", GT_TARGET_PROBE | GT_INVERTED_PROBE) \
	X(PROBE_NAME(free), "free", GT_TARGET_PROBE | GT_INVERTED_PROBE) \
	X(PROBE_NAME(realloc), "realloc", GT_TARGET_PROBE | GT_INVERTED_PROBE) \
	X(PROBE_NAME(calloc), "calloc", GT_TARGET_PROBE | GT_INVERTED_PROBE) \
	X(PROBE_NAME(memset), "memset", GT_TARGET_PROBE) \
	X(PROBE_NAME(memcmp), "memcmp", GT_TARGET_PROBE) \
	X(PROBE_NAME(memcpy), "memcpy", GT_TARGET_PROBE)

#define PROBES_LIST_MEMORY_ALWAYS \
	X(CONCAT(PROBE_NAME(malloc), _always), "malloc", GT_ALWAYS_PROBE) \
	X(CONCAT(PROBE_NAME(free), _always), "free", GT_ALWAYS_PROBE) \
	X(CONCAT(PROBE_NAME(realloc), _always), "realloc", GT_ALWAYS_PROBE) \
	X(CONCAT(PROBE_NAME(calloc), _always), "calloc", GT_ALWAYS_PROBE) \
	X(CONCAT(PROBE_NAME(memset), _always), "memset", GT_ALWAYS_PROBE) \
	X(CONCAT(PROBE_NAME(memcmp), _always), "memcmp", GT_ALWAYS_PROBE) \
	X(CONCAT(PROBE_NAME(memcpy), _always), "memcpy", GT_ALWAYS_PROBE)

#define PROBES_LIST \
	PROBES_LIST_MEMORY


#define X(func_name, orig_name, type)         \
	GET_FUNC_ID(func_name),

#ifdef __cplusplus
extern "C" {
#endif

enum memory_funcs_id_t {
	PROBES_LIST
};
#undef X

enum memory_funcs_id_t get_memory_feature_id(void *ptr);
enum memory_funcs_id_t get_memory_feature_always_id(void *ptr);

#ifdef __cplusplus
}
#endif

#endif /* __MEMORY_PROBES_LIST_H__ */
