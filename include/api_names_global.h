#ifndef __API_NAMES_GLOBAL_H__
#define __API_NAMES_GLOBAL_H__

#define GET_VALUE_2(x,y) x##y
#define GET_VALUE_1(x,y) GET_VALUE_2(x, y)
#define PROBE_NAME(func) GET_VALUE_1(PROBE_NAME_PREFIX, func)

#define PROBE_NAME_PREFIX __PROBE__

#endif
