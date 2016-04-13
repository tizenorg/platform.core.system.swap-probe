#ifndef __GET_GOT_H__
#define __GET_GOT_H__

/* Pass address - get function name */
const char *lmap_get_name(const struct link_map *map, unsigned long offset);

/* Pass name - get GOT entry address */
unsigned long lmap_reladdr_by_name(const struct link_map *map, const char *name);

/* Pass offset - get GOT entry address */
unsigned long lmap_reladdr_by_offset(const struct link_map *map,
				     unsigned long offset);

#endif /* __GET_GOT_H__ */
