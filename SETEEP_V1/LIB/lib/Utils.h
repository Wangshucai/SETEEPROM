#ifndef __UTILS_H
#define __UTILS_H

#include <stdint.h>
#ifndef TRUE
#define TRUE				1
#endif

#ifndef FALSE
#define FALSE				0
#endif

#ifndef NULL
#define NULL				(void *)0
#endif

#ifndef INVALID_HANDLE
#define INVALID_HANDLE				-32768
#endif

#ifndef ENABLED
#define ENABLED				TRUE
#endif

#ifndef DISABLED
#define DISABLED			FALSE
#endif

#ifndef min
#define min(x,y)    (((x) < (y)) ? (x) : (y))
#endif

#ifndef max
#define max(x,y)    (((x) > (y)) ? (x) : (y))
#endif

#ifndef dim
#define dim(x)  		(sizeof(x) / sizeof(x[0]))
#endif

#endif
